#include "contentwidget.h"
#include "../chatlist/chatlistwidget.h"
#include "../contactlist/contactlistwidget.h"
#include "../favorite/favoritelistwidget.h"
#include "../tag/taglistwidget.h"
#include "../friendselectiondialog/friendselectiondialog.h"
#include "../utility/loadingdialog.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QMessageBox>

#define CALLBACK_TYPE_CREATE_GROUP 1

ContentWidget::ContentWidget(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

ContentWidget::~ContentWidget()
{
}

void ContentWidget::setupUI()
{
    setStyleSheet("border: 0px; border-right: 1px solid #e5e5e5;");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    
    QHBoxLayout *headerLayout = new QHBoxLayout(this);
    headerLayout->setContentsMargins(2, 10, 2, 2);
    searchInput = new QLineEdit(this);
    searchInput->setPlaceholderText("搜索...");
    searchInput->addAction(QIcon(":/images/search.png"), QLineEdit::LeadingPosition);

    addBtn = new QPushButton(this);
    addBtn->setText("+");
    addBtn->setFixedSize(22, 22);

    headerLayout->addWidget(searchInput);
    headerLayout->addWidget(addBtn);
    mainLayout->addLayout(headerLayout);


    stackedWidget = new QStackedWidget(this);
    stackedWidget->setStyleSheet("background-color: #f4f4f4");
    
    // 创建各个类型的widget
    chatListWidget = new ChatListWidget(this);
    contactListWidget = new ContactListWidget(this);
    favoriteListWidget = new FavoriteListWidget(this);
    tagListWidget = new TagListWidget(this);
    
    // 添加到堆栈窗口
    stackedWidget->addWidget(chatListWidget);
    stackedWidget->addWidget(contactListWidget);
    stackedWidget->addWidget(favoriteListWidget);
    stackedWidget->addWidget(tagListWidget);
    
    mainLayout->addWidget(stackedWidget);
    
    // 连接信号
    connect(addBtn, &QPushButton::clicked, [this](bool checked) {
        FriendSelectionDialog dialog(this);
        // 显示对话框并获取结果
        if (dialog.exec() == QDialog::Accepted) {
            // 获取已选择的好友
            QStringList selected = dialog.selectedFriends();
            if(selected.empty()) {
                return;
            } else if(selected.size() == 1) {
                chatToUser(selected.first());
            } else {
                std::list<std::string> members;
                foreach (auto qid, selected) {
                    members.push_back(qid.toStdString());
                }
                m_loadingDialog->start();
                WFCLib::ChatClient::Instance()->createGroup("", "群聊", "", WFCLib::GroupType_Restricted, "", members, "", {0}, this, CALLBACK_TYPE_CREATE_GROUP);
            }
        }
    });

    connect(chatListWidget, &ChatListWidget::itemSelected, [=](const WFCLib::Conversation &itemId) {
        emit conversationSelected(0, itemId);
    });
    
    connect(contactListWidget, &ContactListWidget::itemSelected, [=](int type, const QString &itemId) {
        emit contactSelected(1, type, itemId);
    });
    
    connect(favoriteListWidget, &FavoriteListWidget::itemSelected, [=](const QString &itemId) {
        emit itemSelected(2, itemId);
    });
    
    connect(tagListWidget, &TagListWidget::itemSelected, [=](const QString &itemId) {
        emit itemSelected(3, itemId);
    });

    connect(searchInput, &QLineEdit::textChanged, this, &ContentWidget::performSearch);

    m_loadingDialog = new LoadingDialog(this);
}

void ContentWidget::onSuccess(int dataType, const std::string &value)
{
    QTimer::singleShot(0, this, [this, dataType, value]() {
        if(dataType == 1) {
            m_loadingDialog->stop();
            WFCLib::Conversation conversation;
            conversation.conversationType = WFCLib::Group_Type;
            conversation.target = value;
            qInfo() << "create group success" << value.c_str();
            chatListWidget->chatToConversaton(conversation);
        }
    });
}

void ContentWidget::onFailure(int dataType, int errorCode)
{
    QTimer::singleShot(0, this, [this, dataType, errorCode]() {
        if(dataType == 1) {
            m_loadingDialog->stop();
            QMessageBox::critical(this, "失败", QString::number(errorCode));
        }
    });
}

void ContentWidget::showPage(int index)
{
    if (index >= 0 && index < stackedWidget->count()) {
        stackedWidget->setCurrentIndex(index);
    }
    if(index == 0) {
        addBtn->setHidden(false);
    } else {
        addBtn->setHidden(true);
    }
}

void ContentWidget::performSearch()
{
    QString keyword = searchInput->text().trimmed().toLower();
    qDebug() << "performSearch:" << keyword;
}

void ContentWidget::chatToUser(const QString &userId)
{
    if(stackedWidget->currentIndex() != 0) {
        showPage(0);
    }
    chatListWidget->chatToUser(userId);
}

void ContentWidget::callToUser(const QString &userId, bool isAudio)
{

}
