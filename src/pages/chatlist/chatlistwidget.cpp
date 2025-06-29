#include "chatlistwidget.h"
#include <QVBoxLayout>
#include <../../wfc/client/ChatClient.h>
#include "chatlistdelegate.h"
#include <QMenu>
#include <QScrollBar>
#include <QTimer>

ChatListWidget::ChatListWidget(QWidget *parent) : QWidget(parent)
{
    setupUI();
    connect(WFCLib::ChatClient::Instance(), &WFCLib::ChatClient::receiveMessages, this, &ChatListWidget::onReceiveMessages);
    connect(WFCLib::ChatClient::Instance(), &WFCLib::ChatClient::userSettingUpdated, this, &ChatListWidget::onUserSettingUpdated);
    connect(WFCLib::ChatClient::Instance(), &WFCLib::ChatClient::groupInfoUpdated, this, &ChatListWidget::onGroupInfoUpdated);
    connect(WFCLib::ChatClient::Instance(), &WFCLib::ChatClient::userInfoUpdated, this, &ChatListWidget::onUserInfoUpdated);

    connect(WFCLib::ChatClient::Instance(), &WFCLib::ChatClient::messageSendPrepared, this, &ChatListWidget::reloadData);
    connect(WFCLib::ChatClient::Instance(), &WFCLib::ChatClient::messageSendSuccess, this, &ChatListWidget::reloadData);
    connect(WFCLib::ChatClient::Instance(), &WFCLib::ChatClient::messageSendFailure, this, &ChatListWidget::reloadData);

    connect(WFCLib::ChatClient::Instance(), &WFCLib::ChatClient::draftUpdated, this, &ChatListWidget::reloadData);


    connect(WFCLib::ChatClient::Instance(), &WFCLib::ChatClient::conversationSilentUpdated, this, &ChatListWidget::reloadData);
    connect(WFCLib::ChatClient::Instance(), &WFCLib::ChatClient::conversationTopUpdated, this, &ChatListWidget::reloadData);

    connect(WFCLib::ChatClient::Instance(), &WFCLib::ChatClient::unreadStatusCleared, this, &ChatListWidget::onUnreadStatusCleared);
    connect(WFCLib::ChatClient::Instance(), &WFCLib::ChatClient::conversationMessageCleared, this, &ChatListWidget::onMessageCleared);
}

void ChatListWidget::chatToUser(const QString &userId)
{
    WFCLib::Conversation conversation;
    conversation.conversationType = WFCLib::Single_Type;
    conversation.target = userId.toStdString();

    chatToConversaton(conversation);
}

void ChatListWidget::chatToConversaton(const WFCLib::Conversation &conversation)
{
    QModelIndex index = model->focusConversaton(conversation);
    listView->setCurrentIndex(index);
    listView->scrollTo(index, QListView::PositionAtCenter);

    onItemClicked(index);
}

void ChatListWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    listView = new QListView(this);
    listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    model = new ChatListModel(this);

    listView->setModel(model);
    listView->setItemDelegate(new ChatListDelegate(this));

    // 设置选择行为
    listView->setSelectionBehavior(QAbstractItemView::SelectRows);
    listView->setSelectionMode(QAbstractItemView::SingleSelection);
    listView->setVerticalScrollMode(QListView::ScrollPerPixel);
    listView->setStyleSheet(R"(
        QListView {
            border: none;
        }

        QScrollBar:vertical {
            background: transparent;
            width: 8px;
            margin: 0;
            border-radius: 4px;
            opacity: 0;
        }

        QScrollBar::handle:vertical {
            background: rgba(150, 150, 150, 0.8);
            border-radius: 4px;
            min-height: 20px;
        }

        QListView:hover QScrollBar:vertical,
        QScrollBar:vertical:hover {
            opacity: 1;
            transition: opacity 0.2s ease-in-out;
        }

        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical {
            height: 0;
        }
    )");

    // 获取垂直滚动条
    QScrollBar *scrollBar = listView->verticalScrollBar();
    scrollBar->hide();

    // 滚动条计时器
    scrollEndTimer = new QTimer(this);
    scrollEndTimer->setSingleShot(true);
    scrollEndTimer->setInterval(1000); // 1000ms无变化隐藏
    connect(scrollEndTimer, &QTimer::timeout, [scrollBar]() {
        scrollBar->hide();
    });

    // 连接信号槽
    connect(scrollBar, &QScrollBar::valueChanged, this, [this, scrollBar]() {
        scrollBar->show();
        // 每次值变化时重置定时器
        // scrollEndTimer->start();
    });


    // 连接点击事件
    connect(listView, &QListView::clicked, this, &ChatListWidget::onItemClicked);


    // 设置上下文菜单策略
    listView->setContextMenuPolicy(Qt::CustomContextMenu);

    // 连接信号到槽
    connect(listView, &QListView::customContextMenuRequested, [this](const QPoint &pos) {
        QModelIndex index = listView->indexAt(pos);

        if (index.isValid()) {
            // 创建上下文菜单
            QMenu *contextMenu = new QMenu(this);
            contextMenu->setStyleSheet("QMenu::item:selected { background-color: lightgray; }");

            const WFCLib::ConversationInfo info = this->model->infoAt(index);

            if(info.isTop) {
                QAction *action = new QAction("取消置顶", this);
                contextMenu->addAction(action);
                QObject::connect(action, &QAction::triggered, [index, this]() {
                    this->model->setTopAt(index, false);
                });
            } else {
                QAction *action = new QAction("置顶", this);
                contextMenu->addAction(action);
                QObject::connect(action, &QAction::triggered, [index, this]() {
                    this->model->setTopAt(index, true);
                });
            }

            if(info.unreadCount.unread > 0) {
                QAction *action = new QAction("设置为已读", this);
                contextMenu->addAction(action);
                QObject::connect(action, &QAction::triggered, [index, this]() {
                    this->model->setReadAt(index, true);
                });
            } else {
                QAction *action = new QAction("设置为未读", this);
                contextMenu->addAction(action);
                QObject::connect(action, &QAction::triggered, [index, this]() {
                    this->model->setReadAt(index, false);
                });
            }

            if(info.isSilent) {
                QAction *action = new QAction("允许消息通知", this);
                contextMenu->addAction(action);
                QObject::connect(action, &QAction::triggered, [index, this]() {
                    this->model->setSilentAt(index, false);
                });
            } else {
                QAction *action = new QAction("消息免打扰", this);
                contextMenu->addAction(action);
                QObject::connect(action, &QAction::triggered, [index, this]() {
                    this->model->setSilentAt(index, true);
                });
            }

            QAction *actionDelete = new QAction("删除", this);
            contextMenu->addAction(actionDelete);

            listView->setCurrentIndex(index);

            QObject::connect(actionDelete, &QAction::triggered, [index, this]() {
                model->removeAt(index);
            });

            // 获取全局坐标
            QPoint globalPos = listView->mapToGlobal(pos);
            // 显示菜单
            contextMenu->exec(globalPos);
        }
    });


    mainLayout->addWidget(listView);
}

void ChatListWidget::reloadData()
{
    QModelIndex index = listView->currentIndex();
    if(index.row() >=0 && index.row() <= model->rowCount(index)-1) {
        WFCLib::ConversationInfo convInfo = model->infoAt(index);
        model->reloadData();
        index = model->indexOf(convInfo);
        listView->setCurrentIndex(index);
    } else {
        model->reloadData();
    }
}

void ChatListWidget::onItemClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        WFCLib::ConversationInfo itemId = model->data(index, Qt::UserRole).value<WFCLib::ConversationInfo>();
        emit itemSelected(itemId.conversation);
    }
}

void ChatListWidget::onReceiveMessages(const std::list<WFCLib::Message> &messages, bool hasMore)
{
    reloadData();
}

void ChatListWidget::onUserSettingUpdated()
{
    reloadData();
}

void ChatListWidget::onGroupInfoUpdated(const std::list<WFCLib::GroupInfo> &groupInfos)
{
    reloadData();
}

void ChatListWidget::onUserInfoUpdated(const std::list<WFCLib::UserInfo> &userInfos)
{
    reloadData();
}

void ChatListWidget::onUnreadStatusCleared(const WFCLib::Conversation &conversation)
{
    reloadData();
}

void ChatListWidget::onMessageCleared(const WFCLib::Conversation &conversation, int64_t before)
{
    reloadData();
}
