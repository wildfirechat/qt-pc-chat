#include "chatdetailwidget.h"
#include <QFileInfo>
#include <QScrollBar>
#include <QFontDatabase>
#include <QDebug>
#include <QTimer>
#include <QEvent>
#include <QKeyEvent>
#include <QGuiApplication>
#include <QResizeEvent>
#include <QMainWindow>
#include <QPropertyAnimation>
#include <QTextCursor>
#include <QTextBlock>
#include <QBuffer>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <QUuid>
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QSplitter>
#include <QSettings>
#include <QStandardItemModel>
#include "../../wfc/client/ChatClient.h"
#include "chatdelegate.h"
#include "../../vendor/zenshot/starter.h"
#include "../../vendor/zenshot/core/gparams.h"
#include "../../vendor/ezviewer/imageviewer_mainwindow.h"
#include "settings/conversationsettingswidget.h"
#include "emojdialog.h"
#include "atmemberdelegate.h"
#include "atmemberlistview.h"
#include "../voip/voipwebviewwidget.h"
#include "../voip/avenginekitproxy.h"
#include <QMediaPlayer>
#include <QCamera>
#include <QCameraInfo>
// #include <QCameraViewfinder>
#include <QAudioInput>
#include <QAudioDeviceInfo>
#include <QDebug>



#define INPUT_SPLITER_STATE "input_splitter/state"

ChatDetailWidget::ChatDetailWidget(QWidget *parent) : QWidget(parent), isAtBottom(true)
{
    setupUI();

    // 初始化AvEngineKitProxy
    AvEngineKitProxy::instance()->setup(WFCLib::ChatClient::Instance());
}

ChatDetailWidget::~ChatDetailWidget()
{
}

void ChatDetailWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 顶部标题栏
    QWidget *titleWidget = new QWidget(this);
    titleWidget->setFixedHeight(50);
    titleWidget->setStyleSheet("background-color: #f5f5f5; border-bottom: 1px solid #e0e0e0;");
    QHBoxLayout *titleLayout = new QHBoxLayout(titleWidget);
    titleLayout->setContentsMargins(15, 0, 15, 0);

    partnerNameLabel = new QLabel("聊天对象", this);
    partnerNameLabel->setStyleSheet("font-size: 16px; font-weight: bold;");

    QPushButton *infoButton = new QPushButton(this);
    infoButton->setFixedSize(60, 30);
    infoButton->setFlat(true);
    infoButton->setIcon(QIcon(":/images/chats_menu.png"));

    titleLayout->addWidget(partnerNameLabel);
    titleLayout->addStretch();
    titleLayout->addWidget(infoButton);

    // 消息列表
    messageListView = new QListView(this);
    messageListView->setSpacing(10);
    messageListView->setStyleSheet("border: none;");
    messageListView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    messageListView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    chatDelegate = new ChatDelegate(this);
    model = new ChatModel(this);
    messageListView->setModel(model);
    messageListView->setItemDelegate(chatDelegate);

    // 设置选择行为
    messageListView->setSelectionBehavior(QAbstractItemView::SelectRows);
    messageListView->setSelectionMode(QAbstractItemView::SingleSelection);
    messageListView->setVerticalScrollMode(QListView::ScrollPerPixel);
    messageListView->setStyleSheet(R"(
        QListView {
            border: none;
        }

        /* 滚动条整体样式 */
        QScrollBar:vertical {
            background: #f0f0f0;
            width: 8px;
            margin: 0px;
        }

        /* 滑块样式 */
        QScrollBar::handle:vertical {
            background: #b0b0b0;
            border-radius: 4px;
            min-height: 20px;
        }

        /* 滑块悬停样式 */
        QScrollBar::handle:vertical:hover {
            background: #808080;
        }

        /* 上下箭头按钮 */
        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical {
            height: 0px;
            subcontrol-position: bottom;
            subcontrol-origin: margin;
        }

        /* 滚动条背景区域 */
        QScrollBar::add-page:vertical,
        QScrollBar::sub-page:vertical {
            background: none;
        }
    )");

    // 获取垂直滚动条
    QScrollBar *scrollBar = messageListView->verticalScrollBar();
    scrollBar->hide();
    scrollBar->setSingleStep(15);

    // 滚动条计时器
    scrollEndTimer = new QTimer(this);
    scrollEndTimer->setSingleShot(true);
    scrollEndTimer->setInterval(1000); // 1000ms无变化隐藏
    connect(scrollEndTimer, &QTimer::timeout, [scrollBar]() {
        scrollBar->hide();
    });

    // 连接信号槽
    connect(scrollBar, &QScrollBar::valueChanged, this, [this, scrollBar](int value) {
        scrollBar->show();
        // 每次值变化时重置定时器
        scrollEndTimer->start();

        // 计算最大可见范围
        int maxVisibleRange = scrollBar->maximum() - scrollBar->minimum();

        // 计算当前可见范围
        int currentVisibleRange = scrollBar->value() + scrollBar->pageStep();

        // 判断是否滚动到底部（误差范围5像素）
        if (currentVisibleRange >= maxVisibleRange - 5) {
            isAtBottom = true;
        } else {
            isAtBottom = false;
        }

        if(value < 5) {
            qDebug() << "is at the top";
            loadHistoryMessages();
        }
    });

    // 底部输入区域
    QWidget *inputWidget = new QWidget(this);
    inputWidget->setMinimumHeight(120);
    inputWidget->setStyleSheet("background-color: #f5f5f5; border-top: 1px solid #e0e0e0;");
    QVBoxLayout *inputLayout = new QVBoxLayout(inputWidget);
    inputLayout->setContentsMargins(15, 10, 15, 10);
    inputLayout->setSpacing(10);

    // 工具按钮
    QHBoxLayout *toolLayout = new QHBoxLayout();
    toolLayout->setSpacing(15);

    fileButton = new QPushButton("📎", this);
    fileButton->setFixedSize(30, 30);
    fileButton->setStyleSheet("QPushButton { border-radius: 15px; }"
                              "QPushButton:hover { background-color: #e9e9e9; }");

    emojiButton = new QPushButton("😀", this);
    emojiButton->setFixedSize(30, 30);
    emojiButton->setStyleSheet("QPushButton { border-radius: 15px; }"
                               "QPushButton:hover { background-color: #e9e9e9; }");

    capturerButton = new QPushButton(QIcon(":/images/chatbox_scissors.png"), "", this);
    capturerButton->setFixedSize(30, 30);
    capturerButton->setStyleSheet("QPushButton { border-radius: 15px; }"
                               "QPushButton:hover { background-color: #e9e9e9; }");

    voiceCallButton = new QPushButton("📞", this);
    voiceCallButton->setFixedSize(30, 30);
    voiceCallButton->setStyleSheet("QPushButton { border-radius: 15px; }"
                               "QPushButton:hover { background-color: #e9e9e9; }");

    videoCallButton = new QPushButton("📷", this);
    videoCallButton->setFixedSize(30, 30);
    videoCallButton->setStyleSheet("QPushButton { border-radius: 15px; }"
                               "QPushButton:hover { background-color: #e9e9e9; }");

    toolLayout->addWidget(fileButton);
    toolLayout->addWidget(emojiButton);
    toolLayout->addWidget(capturerButton);
    toolLayout->addWidget(voiceCallButton);
    toolLayout->addWidget(videoCallButton);
    toolLayout->addStretch();

    // 输入框和发送按钮
    QHBoxLayout *sendLayout = new QHBoxLayout();
    sendLayout->setSpacing(10);

    messageEdit = new InputTextEdit(this);
    messageEdit->setPlaceholderText("输入消息...");
    messageEdit->setStyleSheet("QTextEdit { border: 1px solid #e0e0e0; border-radius: 4px; padding: 5px; }");
    messageEdit->setMinimumHeight(60);
    messageEdit->installEventFilter(this);


    sendButton = new QPushButton("发送", this);
    sendButton->setFixedSize(70, 35);
    sendButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; border-radius: 4px; }"
                             "QPushButton:hover { background-color: #45a049; }"
                             "QPushButton:pressed { background-color: #3d8b40; }");

    sendLayout->addWidget(messageEdit);
    sendLayout->addWidget(sendButton);

    inputLayout->addLayout(toolLayout);
    inputLayout->addLayout(sendLayout);

    // 添加到主布局
    mainLayout->addWidget(titleWidget);

    emojDialog = new EmojDialog(this);

    inputSplitter = new QSplitter(Qt::Vertical, this);
    inputSplitter->addWidget(messageListView);
    inputSplitter->addWidget(inputWidget);

    mainLayout->addWidget(inputSplitter);

    // 连接信号和槽
    connect(infoButton, &QPushButton::clicked, this, &ChatDetailWidget::toggleSidePanel);
    connect(messageListView, &QListView::clicked, this, &ChatDetailWidget::onItemClicked);

    connect(sendButton, &QPushButton::clicked, this, &ChatDetailWidget::onSendMessage);
    connect(fileButton, &QPushButton::clicked, this, &ChatDetailWidget::onUploadFile);
    connect(WFCLib::ChatClient::Instance(), &WFCLib::ChatClient::receiveMessages, this, &ChatDetailWidget::onReceiveMessages);
    connect(WFCLib::ChatClient::Instance(), &WFCLib::ChatClient::conversationMessageCleared, this, &ChatDetailWidget::onMessageCleared);
    connect(WFCLib::ChatClient::Instance(), &WFCLib::ChatClient::messageSendUploaded, this, &ChatDetailWidget::onMessageSendUploaded);

    connect(emojiButton, &QPushButton::clicked, [this]() {
        emojDialog->move(emojiButton->mapToGlobal(QPoint(0, -204)));
        emojDialog->show();
    });

    connect(emojDialog, &EmojDialog::emojiSelected, this, &ChatDetailWidget::onEmojiSelected);

    connect(capturerButton, &QPushButton::clicked, [this](){
        if (m_Shotting)
        {
            return;
        }


        m_Shotting = true;
        Starter* starter = nullptr;
        if (m_Starer.empty())
        {
            starter = new Starter(false);
            connect(starter, SIGNAL(ShotDone(Starter*, int)), this, SLOT(OnShotDone(Starter*, int)));
        }
        else
        {
            starter = m_Starer.back();
            m_Starer.pop_back();
        }

#if !NZENTAO_VER_
        m_CurrentStarter = starter;
#endif // NZENTAO_VER_
        starter->init();
    });
    connect(voiceCallButton, &QPushButton::clicked, [this]() {startCall(true);});
    connect(videoCallButton, &QPushButton::clicked, [this]() {startCall(false);});

    connect(chatDelegate, &ChatDelegate::avatarRightClicked, this, &ChatDetailWidget::onAvatarRightClicked);
    connect(chatDelegate, &ChatDelegate::messageContentClicked, this, &ChatDetailWidget::onMessageContentClicked);

    // 创建侧边面板（覆盖层）
    sidePanel = new QFrame(this);
    sidePanel->setFrameShape(QFrame::StyledPanel);
    sidePanel->setAutoFillBackground(true);
    sidePanel->setStyleSheet("background-color: #f0f0f0;");
    sidePanel->setVisible(false);

    // 侧边面板布局
    QVBoxLayout *sideLayout = new QVBoxLayout(sidePanel);

    // 创建设置
    settingWidget = new ConversationSettingsWidget(sidePanel);

    sideLayout->addWidget(settingWidget);

    // 创建成员选择列表
    memberListView = new AtMemberListView(this);
    memberListView->setWindowFlags(Qt::Popup);
    memberListView->setFocusPolicy(Qt::NoFocus);
    memberListView->setMinimumWidth(160);
    memberListView->hide();
    memberListModel = new QStandardItemModel(this);
    memberListView->setModel(memberListModel);
    memberListView->setItemDelegate(new AtMemberDelegate(this));

    connect(messageEdit, &InputTextEdit::atTriggered, this, &ChatDetailWidget::onAtTriggered);
    connect(memberListView, &QListView::clicked, this, &ChatDetailWidget::onMemberSelected);

    imageViewer = new IVMainWindow(this);

    audioPlayer = new QMediaPlayer(this);
    playingTimer = new QTimer(this);
    playingTimer->setSingleShot(false);
    playingTimer->setInterval(500);
    connect(playingTimer, &QTimer::timeout, [this]() {
        model->refreshMessage(playingMessageId);
    });

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    connect(audioPlayer, &QMediaPlayer::playbackStateChanged, this, [this](QMediaPlayer::PlaybackState newState)
#else
    connect(audioPlayer, &QMediaPlayer::stateChanged, this, [this](QMediaPlayer::State newState)
#endif
    {
        if(newState ==
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    QMediaPlayer::PlaybackState::PlayingState
#else
    QMediaPlayer::State::PlayingState
#endif

                    )
        {
            model->setMessagePlaying(playingMessageId, true);
            playingTimer->start();
        }
        else if(newState ==
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
                   QMediaPlayer::PlaybackState::StoppedState
#else
                   QMediaPlayer::State::StoppedState
#endif
                   )
        {
            model->setMessagePlaying(playingMessageId, false);
            playingTimer->stop();
            model->refreshMessage(playingMessageId);
        }
    });
}

void ChatDetailWidget::resizeEvent(QResizeEvent *event) {
    // 计算面板宽度（主窗口的四分之一）
    panelWidth = 256;

    // 调整侧边面板位置
    if (sidePanel->isVisible()) {
        sidePanel->setGeometry(width() - panelWidth, 50, panelWidth, height()-50);
    } else {
        sidePanel->setGeometry(width(), 50, panelWidth, height()-50);
    }
}

void ChatDetailWidget::keyPressEvent(QKeyEvent *event) {
    bool isMac = QSysInfo::productType() == "macos";
    Qt::KeyboardModifiers pasteMod = isMac ? Qt::MetaModifier : Qt::ControlModifier;

    if ((event->modifiers() & pasteMod) &&
        (event->key() == Qt::Key_V)) {
        // 处理粘贴事件
        handlePaste();
        event->accept();
    } else {
        QWidget::keyPressEvent(event);
    }
}

void ChatDetailWidget::setChatConversation(const WFCLib::Conversation &conversation)
{
    // 在窗口显示后设置尺寸（确保splitter已正确布局）
    if(currentConversation.target.empty()) {
        // 恢复分割器状态
        QSettings settings;
        if (settings.contains(INPUT_SPLITER_STATE)) {
            inputSplitter->restoreState(settings.value(INPUT_SPLITER_STATE).toByteArray());
        } else {
        QTimer::singleShot(0, [this]() {
            int totalHeight = inputSplitter->size().height();
            inputSplitter->setSizes({totalHeight - 60, 60});
        });
        }
    }

    isAtBottom = true;

    if(sidePanel->isVisible()) {
        sidePanel->hide();
    }

    if(!currentConversation.target.empty()) {
        QString message = messageEdit->toPlainText().trimmed();
        WFCLib::ConversationInfo info = WFCLib::ChatClient::Instance()->getConversationInfo(currentConversation);
        if (message.isEmpty()) {
            if(!info.draft.empty()) {
                WFCLib::ChatClient::Instance()->setConversationDraft(currentConversation, "");
            }
        } else if(message.toStdString() != info.draft) {
            WFCLib::ChatClient::Instance()->setConversationDraft(currentConversation, message.toStdString());
            messageEdit->clear();
        }
    }

    currentConversation = conversation;

    WFCLib::ConversationInfo info = WFCLib::ChatClient::Instance()->getConversationInfo(currentConversation);
    if(!info.draft.empty()) {
        messageEdit->setText(info.draft.c_str());
    }

    // 根据ID设置聊天对象名称
    QString partnerId = conversation.target.c_str();

    if(conversation.conversationType == WFCLib::Single_Type) {
        WFCLib::UserInfo userInfo = WFCLib::ChatClient::Instance()->getUserInfo(partnerId.toStdString(), true);
        partnerNameLabel->setText(userInfo.displayName.c_str());
    } else if(conversation.conversationType == WFCLib::Group_Type) {
        WFCLib::GroupInfo groupInfo = WFCLib::ChatClient::Instance()->getGroupInfo(partnerId.toStdString(), true);
        QString groupName = groupInfo.name.empty()?"群聊":groupInfo.name.c_str();
        if(groupInfo.memberCount > 0) {
            partnerNameLabel->setText(QString("%1(%2)").arg(groupName).arg(groupInfo.memberCount));
        } else {
            partnerNameLabel->setText(groupName);
        }
    } else if(conversation.conversationType == WFCLib::Channel_Type) {
        WFCLib::ChannelInfo channelInfo = WFCLib::ChatClient::Instance()->getChannelInfo(partnerId.toStdString(), true);
        partnerNameLabel->setText(channelInfo.name.c_str());
    }

    if(playingMessageId > 0) {
        audioPlayer->stop();
        playingMessageId = 0;
    }

    // 清空现有消息并加载历史消息
    model->clear();

    long firstUnreadId = WFCLib::ChatClient::Instance()->getConversationFirstUnreadMessageId(conversation);
    model->setFirstUnreadMessageId(firstUnreadId);
    loadChatHistory(conversation);
}

bool ChatDetailWidget::hasContent()
{
    return !currentConversation.target.empty();
}

void ChatDetailWidget::loadChatHistory(const WFCLib::Conversation  &conversation)
{
    WFCLib::ChatClient::Instance()->clearUnreadStatus(conversation);
    model->clear();
    auto messages = WFCLib::ChatClient::Instance()->getMessages(conversation, {}, 0, 10, "");
    model->addMessages(messages);
    messageListView->scrollToBottom();
}

bool ChatDetailWidget::fetchEditorContents(QTextEdit *textEdit, QList<QImage> &imagePaths, QStringList &filePaths, QStringList &texts)
{
    QTextDocument* doc = messageEdit->document();
    QTextCursor cursor(doc);

    // 移动到文档开始
    cursor.movePosition(QTextCursor::Start);

    // 遍历所有文本块
    QTextBlock block = cursor.block();
    QTextBlock::Iterator it;

    for (it = block.begin(); !it.atEnd(); ++it) {
        QTextFragment fragment = it.fragment();
        if (fragment.isValid()) {
            // 判断是否为图像
            QTextImageFormat imageFormat = fragment.charFormat().toImageFormat();
            if (imageFormat.isValid()) {
                // 提取图像资源名
                QString resourceName = imageFormat.name();

                // 从文档资源中获取QImage
                QVariant resource = doc->resource(QTextDocument::ImageResource, QUrl(resourceName));
                if (resource.type() == QVariant::Image) {
                    QImage image = resource.value<QImage>();
                    imagePaths.append(image);
                }
            } else {
                // 提取普通文本
                texts.append(fragment.text());
            }
        }
    }

    return !imagePaths.empty() || !filePaths.empty() || !texts.empty();
}

void ChatDetailWidget::handlePaste() {
    const QClipboard* clipboard = QApplication::clipboard();
    const QMimeData* mimeData = clipboard->mimeData();

    // 处理文本
    if (mimeData->hasText()) {
        QString text = mimeData->text();
        messageEdit->insertPlainText(text);
    }

    // 处理图片
    if (mimeData->hasImage()) {
        QImage image = qvariant_cast<QImage>(mimeData->imageData());
        if (!image.isNull()) {
            QTextCursor cursor = messageEdit->textCursor();
            QTextDocument* doc = messageEdit->document();

            QString uuidString = QUuid::createUuid().toString(QUuid::WithoutBraces).replace("-", "");
            // 注册图片资源（命名为uuidString）
            doc->addResource(QTextDocument::ImageResource, QUrl(uuidString), image);

            // 使用HTML标签插入并指定尺寸
            cursor.insertHtml("<img src=\"" + uuidString + "\" width=\"200\" height=\"200\">");
            messageEdit->setTextCursor(cursor);
        }
    }

    // 处理其他格式（如URL、HTML等）
    if (mimeData->hasUrls()) {
        QList<QUrl> urls = mimeData->urls();
        qDebug() << "粘贴的URL:" << urls;
    }
}

void ChatDetailWidget::startCall(bool audioOnly) {
    // log the route host
    qDebug() << "Route host: " << WFCLib::ChatClient::Instance()->getHost().c_str();

    // 使用AvEngineKitProxy发起通话
    QStringList participants;
    if (currentConversation.conversationType == WFCLib::Single_Type) {
        // 单聊，添加对方作为参与者
        participants.append(QString::fromStdString(currentConversation.target));
    } else if (currentConversation.conversationType == WFCLib::Group_Type) {
        // 群聊，需要获取群成员
        std::list<WFCLib::GroupMember> groupMembers = WFCLib::ChatClient::Instance()->getGroupMembers(currentConversation.target, false);
        for (const auto& member : groupMembers) {
            if (member.memberId != WFCLib::ChatClient::Instance()->getCurrentUserId()) {
                participants.append(QString::fromStdString(member.memberId));
            }
        }
    }

    AvEngineKitProxy::instance()->startCall(currentConversation, audioOnly, participants);
}

#define LOAD_REMOTE_MESSAGE_DATA_TYPE 1
#define LOAD_MESSAGES_DATA_TYPE 2
void ChatDetailWidget::loadHistoryMessages()
{
    if(isLoading) {
        return;
    }

    isLoading = true;
    WFCLib::ChatClient::Instance()->getMessagesV2(currentConversation, {}, model->getTopMessageId(true), 20, "", this, LOAD_MESSAGES_DATA_TYPE);
}

void ChatDetailWidget::closeEvent(QCloseEvent *event)
{
    // 保存分割器状态
    QSettings settings;
    settings.setValue(INPUT_SPLITER_STATE, inputSplitter->saveState());

    QWidget::closeEvent(event);
}

void ChatDetailWidget::onSuccess(int dataType, const std::list<WFCLib::Message> &messages)
{
    QTimer::singleShot(0, this, [this, dataType, messages]() {
        int insertCount = 0;
        if(dataType == LOAD_REMOTE_MESSAGE_DATA_TYPE) {
            isLoading = false;
            insertCount = model->addMessages(messages);
        } else if(dataType == LOAD_MESSAGES_DATA_TYPE) {
            if(!messages.empty()) {
                insertCount = model->addMessages(messages);
                isLoading = false;
            } else {
                WFCLib::ChatClient::Instance()->getRemoteMessages(currentConversation, {}, model->getTopMessageUid(true), 20, this, LOAD_REMOTE_MESSAGE_DATA_TYPE);
            }
        }
        if(insertCount > 0) {
            // QTimer::singleShot(0, this, [this, insertCount]() {
                QModelIndex index = model->index(insertCount-1, 0);
                messageListView->scrollTo(index, QAbstractItemView::PositionAtTop);
            // });
        }
    });
}

void ChatDetailWidget::onFailure(int dataType, int errorCode)
{
    QTimer::singleShot(0, this, [this, dataType]() {
        if(dataType == LOAD_REMOTE_MESSAGE_DATA_TYPE) {
            isLoading = false;
        } else if(dataType == LOAD_MESSAGES_DATA_TYPE) {
            isLoading = false;
        }
    });
}

void ChatDetailWidget::onSendMessage()
{
    bool hasDocument = false;
    QList<QImage> images;
    QStringList filePaths;
    QStringList texts;
    if(fetchEditorContents(messageEdit, images, filePaths, texts)) {
        //有图片
        foreach (auto image, images) {
            if (image.isNull()) {
                continue; // 图片加载失败
            }

            //save image first
            QTemporaryDir tempDir;
            if (tempDir.isValid()) {
                QString path = tempDir.path(); // 自动生成唯一目录
            }
            QTemporaryFile tempFile;
            if (tempFile.open()) {
                QString fileName = tempFile.fileName();
                if(image.save(fileName, "JPEG", 50)) {
                    WFCLib::ImageMessageContent imageContent;
                    imageContent.localPath = fileName.toStdString();

                    QByteArray byteArray;
                    QBuffer buffer(&byteArray);
                    buffer.open(QIODevice::WriteOnly);
                    if(image.width()/image.height() > 3 || image.height()/image.width() > 3) {
                        // 1. 缩放到120x120并保持宽高比
                        // 这里使用Qt::KeepAspectRatioByExpanding确保最短边为120，然后居中裁剪
                        QImage scaledImage = image.scaled(
                            120, 120,
                            Qt::KeepAspectRatioByExpanding, // 保持宽高比并扩展
                            Qt::SmoothTransformation         // 使用高质量缩放算法
                            );

                        // 2. 居中裁剪到120x120（如果图片不是正方形）
                        int x = (scaledImage.width() - 120) / 2;
                        int y = (scaledImage.height() - 120) / 2;
                        QImage croppedImage = scaledImage.copy(x, y, 120, 120);
                        croppedImage.save(&buffer, "JPEG", 20);
                    } else {
                        QImage scaledImage = image.scaled(120, 120, Qt::KeepAspectRatio);
                        scaledImage.save(&buffer, "JPEG", 20);
                    }
                    imageContent.thumbnail = byteArray.toStdString();

                    sendMessage(imageContent);
                } else {
                    qDebug() << "save failure!";
                }
            }
        }

        //有文件
        foreach(auto fileName, filePaths) {
            QFileInfo fileInfo(fileName);
            QString fileSize;

            qint64 size = fileInfo.size();
            if (size < 1024)
                fileSize = QString("%1 B").arg(size);
            else if (size < 1024 * 1024)
                fileSize = QString("%1 KB").arg(size / 1024.0, 0, 'f', 1);
            else
                fileSize = QString("%1 MB").arg(size / (1024.0 * 1024.0), 0, 'f', 1);

            WFCLib::FileMessageContent fileContent;
            fileContent.localPath = fileInfo.absoluteFilePath().toStdString();
            fileContent.name = fileInfo.fileName().toStdString();
            fileContent.size = fileInfo.size();

            sendMessage(fileContent);
        }

        //有文本
        if(!texts.empty()) {
            QString message = texts.join(" ");
            if (!message.isEmpty()) {
                WFCLib::TextMessageContent content;
                content.content = message.toStdString();
                QVector<AtEntry> atEntrys = messageEdit->getAtEntrys();
                if(!atEntrys.empty()) {
                    content.mentionedType = 1;
                    for(const AtEntry &entry : atEntrys) {
                        content.mentionedTargets.push_back(entry.userId.toStdString());
                    }
                }

                sendMessage(content);
            }
        }

        // 当不再需要整个文档时，销毁它
        messageEdit->clear();
        WFCLib::ChatClient::Instance()->setConversationDraft(currentConversation, "");
    }
}

void ChatDetailWidget::onUploadFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择文件", "", "所有文件 (*.*)");
    if (!fileName.isEmpty()) {
        QFileInfo fileInfo(fileName);
        QString fileSize;

        qint64 size = fileInfo.size();
        if (size < 1024)
            fileSize = QString("%1 B").arg(size);
        else if (size < 1024 * 1024)
            fileSize = QString("%1 KB").arg(size / 1024.0, 0, 'f', 1);
        else
            fileSize = QString("%1 MB").arg(size / (1024.0 * 1024.0), 0, 'f', 1);

        WFCLib::FileMessageContent fileContent;
        fileContent.localPath = fileInfo.absoluteFilePath().toStdString();
        fileContent.name = fileInfo.fileName().toStdString();
        fileContent.size = fileInfo.size();

        sendMessage(fileContent);
    }
}

void ChatDetailWidget::onMessageSendUploaded(long messageId, const std::string &remoteUrl)
{
    model->onMessageMediaUploaded(messageId, remoteUrl);
}

void ChatDetailWidget::onEmojiSelected(const QString &emoji)
{
    messageEdit->insertPlainText(emoji);
    messageEdit->setFocus();
}

void ChatDetailWidget::onReceiveMessages(const std::list<WFCLib::Message> &messages, bool hasMore)
{
    bool currentHasNewMsg = false;
    foreach (auto msg, messages) {
        if(msg.conversation == currentConversation) {
            model->addMessage(msg);
            currentHasNewMsg = true;
        }
    }
    if(currentHasNewMsg) {
        WFCLib::ChatClient::Instance()->clearUnreadStatus(currentConversation);
        if(isAtBottom) messageListView->scrollToBottom();
    }
}

void ChatDetailWidget::sendMessage(const WFCLib::MessageContent &content)
{
    auto msg = WFCLib::ChatClient::Instance()->sendMessage(currentConversation, content, {}, 0, nullptr, 0);
    model->addMessage(msg);

    if(isAtBottom) messageListView->scrollToBottom();
}

void ChatDetailWidget::toggleSidePanel() {
    // 计算面板宽度（主窗口的四分之一）
    panelWidth = 256;
    if (sidePanel->isVisible()) {
        // 隐藏侧边面板
        QPropertyAnimation *anim = new QPropertyAnimation(sidePanel, "geometry");
        anim->setDuration(300);
        anim->setStartValue(QRect(width() - panelWidth, 50, panelWidth, height()-50));
        anim->setEndValue(QRect(width(), 50, panelWidth, height()-50));
        anim->setEasingCurve(QEasingCurve::OutCubic);

        connect(anim, &QPropertyAnimation::finished, [this]() {
            sidePanel->hide();
        });

        anim->start(QAbstractAnimation::DeleteWhenStopped);
    } else {
        settingWidget->setConversation(currentConversation);
        // 显示侧边面板
        sidePanel->setGeometry(width(), 50, panelWidth, height()-50);
        sidePanel->show();

        QPropertyAnimation *anim = new QPropertyAnimation(sidePanel, "geometry");
        anim->setDuration(300);
        anim->setStartValue(QRect(width(), 50, panelWidth, height()-50));
        anim->setEndValue(QRect(width() - panelWidth, 50, panelWidth, height()-50));
        anim->setEasingCurve(QEasingCurve::OutCubic);

        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void ChatDetailWidget::onMessageCleared(const WFCLib::Conversation &conversation, int64_t before)
{
    if(currentConversation == conversation) {
        loadChatHistory(conversation);
    }
}

void ChatDetailWidget::onAvatarRightClicked(const QModelIndex &index)
{
    qDebug() << "on avatar right clicked!";
}

void ChatDetailWidget::onMessageContentClicked(const QModelIndex &index)
{
    qDebug() << "on message content clicked!";
    const MessageData msg = model->data(index, Qt::UserRole).value<MessageData>();
    WFCLib::MessageContentType contentType = msg.message.content->getPrototype()->getType();
    if(contentType == WFCLib::MESSAGE_CONTENT_TYPE_IMAGE) {
        imageViewer->showImageMessage(msg.message);
    } else if(contentType == WFCLib::MESSAGE_CONTENT_TYPE_SOUND) {
        if(msg.voicePlaying) {
            audioPlayer->stop();
            playingMessageId = 0;
        } else {
            playingMessageId = msg.message.messageId;
            WFCLib::SoundMessageContent *soundCont = (WFCLib::SoundMessageContent *)msg.message.content;

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
            audioPlayer->setSource(QUrl(soundCont->remoteUrl.c_str()));
#else
            audioPlayer->setMedia(QUrl(soundCont->remoteUrl.c_str()));
#endif
            audioPlayer->play();
        }
    }
}

void ChatDetailWidget::onAtTriggered(const QString &prefix)
{
    std::list<WFCLib::GroupMember> allMembers = WFCLib::ChatClient::Instance()->getGroupMembers(currentConversation.target, false);
    // 过滤匹配的成员
    QStringList matchedMembers;
    for (WFCLib::GroupMember &member : allMembers) {
        if(prefix.isEmpty()) {
            matchedMembers << member.memberId.c_str();
        } else {
            WFCLib::UserInfo userInfo = WFCLib::ChatClient::Instance()->getUserInfo(member.memberId, currentConversation.target, false);
            if(userInfo.displayName.rfind(prefix.toStdString(), 0) == 0 || userInfo.friendAlias.rfind(prefix.toStdString(), 0) == 0 || userInfo.groupAlias.rfind(prefix.toStdString(), 0) == 0) {
                matchedMembers << member.memberId.c_str();
            }
        }
    }

    if (matchedMembers.isEmpty()) {
        memberListView->hide();
        return;
    }

    // 更新成员列表
    memberListModel->clear();
    for(const QString &memberId : matchedMembers) {
        QStandardItem *item = new QStandardItem(memberId);
        memberListModel->appendRow(item);
    }

    int height = std::min(200, 28*(int)matchedMembers.count() + 8);
    memberListView->setFixedHeight(height);

    // 计算列表位置
    QTextCursor cursor = messageEdit->textCursor();
    QRect rect = messageEdit->cursorRect(cursor);
    rect.setWidth(memberListView->minimumWidth());
    rect.moveTopLeft(messageEdit->mapToGlobal(rect.topLeft()));
    rect.setY(rect.y()-height);

    // 显示成员列表
    memberListView->setGeometry(rect);
    memberListView->show();

    QTimer::singleShot(3000, [this]() {
        messageEdit->setFocus();
    });
}

void ChatDetailWidget::onMemberSelected(const QModelIndex &index)
{
    QString selectedMember = index.data().toString();
    memberListView->hide();
    WFCLib::UserInfo userInfo = WFCLib::ChatClient::Instance()->getUserInfo(selectedMember.toStdString(), currentConversation.target);
    messageEdit->addAtEntry(userInfo.getReadableName().c_str(), selectedMember);
}

void ChatDetailWidget::OnShotDone(Starter *starter, int code)
{
    m_Shotting = false;
    starter->cleanup();
    GParams::instance()->CleanUpArgs();
    delete starter;
    if(code == 1) {
        handlePaste();
    }
}

void ChatDetailWidget::onItemClicked(const QModelIndex &index)
{
    // 这里判断点击位置是否在头像区域
    // 获取当前被点击项的矩形区域
    QRect itemRect = messageListView->visualRect(index);

    MessageData msg = index.data(Qt::UserRole).value<MessageData>();
    QRect avatarRect;
    int vOffset = msg.showTime?24:4;
    if(msg.message.direction == WFCLib::Direction_Receive) {
        avatarRect = QRect(itemRect.x()+5 - 5, itemRect.y()+vOffset-5, 40+10, 40+10);
    } else {
        avatarRect = QRect(itemRect.x()+itemRect.width()-5-60 - 5, itemRect.y()+vOffset-5, 40+10, 40+10);
    }
    QPoint point = messageListView->viewport()->mapFromGlobal(QCursor::pos());
    if (avatarRect.contains(point)) {
        emit showUserDialog(msg.message.from.c_str(), msg.message.conversation.conversationType == WFCLib::Group_Type?msg.message.conversation.target.c_str():"", QCursor::pos());
    } else {
        qDebug() << "no portrait";
    }
}

bool ChatDetailWidget::eventFilter(QObject *obj, QEvent *event) {
    // 检查是否为QTextEdit且事件为按键按下
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        // 检测Enter键（主键盘和数字键盘）
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            // 获取平台信息
            static bool isMacOS = QGuiApplication::platformName().startsWith("cocoa");

            // 判断修饰键组合
            const bool hasCtrl = keyEvent->modifiers() & Qt::ControlModifier;
            const bool hasCommand = keyEvent->modifiers() & Qt::MetaModifier;

            // Mac系统: Command+Enter交给系统，其他情况拦截
            // 其他系统: Ctrl+Enter交给系统，其他情况拦截
            const bool systemHandled = (isMacOS && hasCommand) || (!isMacOS && hasCtrl);

            if (systemHandled) {
                qDebug() << "系统处理组合键";
                return QObject::eventFilter(obj, event);
            } else {
                qDebug() << "回车键被按下发送消息";
                onSendMessage();
                return true;  // 拦截默认行为
            }
        }
    }
    // 默认处理其他事件
    return QObject::eventFilter(obj, event);
}
