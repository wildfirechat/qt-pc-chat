#include "detailwidget.h"
#include "../chatbox/chatdetailwidget.h"
#include "../userdetail/contactdetailwidget.h"
#include "../favorite/favoritedetailwidget.h"
#include "../tag/tagdetailwidget.h"
#include "../contactlist/contactlistwidget.h"
#include "../friendrequestdetail/friendrequestdetailwidget.h"
#include "../groupdetail/groupdetailwidget.h"
#include "../channeldetail/channeldetailwidget.h"
#include "emptydetailwidget.h"
#include <QVBoxLayout>

DetailWidget::DetailWidget(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

DetailWidget::~DetailWidget()
{
}

void DetailWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    
    stackedWidget = new QStackedWidget(this);
    stackedWidget->setStyleSheet("background-color: #f0f0f0");
    
    // 创建各个类型的详情widget
    chatDetailWidget = new ChatDetailWidget(this);
    contactDetailWidget = new ContactDetailWidget(this);
    favoriteDetailWidget = new FavoriteDetailWidget(this);
    tagDetailWidget = new TagDetailWidget(this);
    emptyDetailWidget = new EmptyDetailWidget(this);

    friendRequestDetailWidget = new FriendRequestDetailWidget(this);
    groupDetailWidget = new GroupDetailWidget(this);
    channelDetailWidget = new ChannelDetailWidget(this);
    
    // 添加到堆栈窗口
    stackedWidget->addWidget(chatDetailWidget);
    stackedWidget->addWidget(contactDetailWidget);
    stackedWidget->addWidget(favoriteDetailWidget);
    stackedWidget->addWidget(tagDetailWidget);
    stackedWidget->addWidget(emptyDetailWidget);
    stackedWidget->addWidget(friendRequestDetailWidget);
    stackedWidget->addWidget(groupDetailWidget);
    stackedWidget->addWidget(channelDetailWidget);
    
    mainLayout->addWidget(stackedWidget);
    
    // 默认显示空详情页
    stackedWidget->setCurrentWidget(emptyDetailWidget);

    connect(chatDetailWidget, &ChatDetailWidget::showUserDialog, this, &DetailWidget::onShowUserDialog);
    connect(contactDetailWidget, &ContactDetailWidget::chatToUser, this, &DetailWidget::onChatToUser);
    connect(contactDetailWidget, &ContactDetailWidget::callToUser, this, &DetailWidget::onCallToUser);
}

void DetailWidget::showDetail(int pageIndex, const QString &itemId)
{
    // 根据页面索引和项目ID显示相应的详情
    switch (pageIndex) {
    case 0: // 聊天
        // chatDetailWidget->setChatConversation(itemId);
        stackedWidget->setCurrentWidget(chatDetailWidget);
        break;
    case 1: // 联系人
        contactDetailWidget->setContactId(itemId);
        stackedWidget->setCurrentWidget(contactDetailWidget);
        break;
    case 2: // 收藏
        favoriteDetailWidget->setFavoriteId(itemId);
        stackedWidget->setCurrentWidget(favoriteDetailWidget);
        break;
    case 3: // 标签
        tagDetailWidget->setTagId(itemId);
        stackedWidget->setCurrentWidget(tagDetailWidget);
        break;
    default:
        stackedWidget->setCurrentWidget(emptyDetailWidget);
        break;
    }
}

void DetailWidget::showChatDetail(int pageIndex, const WFCLib::Conversation &itemId)
{
    chatDetailWidget->setChatConversation(itemId);
    stackedWidget->setCurrentWidget(chatDetailWidget);
}

void DetailWidget::showContactDetail(int pageIndex, int type, const QString &itemId)
{
    if(type == ContactType_NEW_FRIEND) {
        friendRequestDetailWidget->setUserId(itemId);
        stackedWidget->setCurrentWidget(friendRequestDetailWidget);
    } else if(type == ContactType_GROUP) {
        groupDetailWidget->setGroupId(itemId);
        stackedWidget->setCurrentWidget(groupDetailWidget);
    } else if(type == ContactType_CHANNEL) {
        channelDetailWidget->setChannelId(itemId);
        stackedWidget->setCurrentWidget(channelDetailWidget);
    } else if(type == ContactType_ORG) {

    } else if(type == ContactType_FRIEND) {
        contactDetailWidget->setContactId(itemId);
        stackedWidget->setCurrentWidget(contactDetailWidget);
    }
}

void DetailWidget::onShowUserDialog(const QString &userId, const QString &groupId, const QPoint &position)
{
    emit showUserDialog(userId, groupId, position);
}

void DetailWidget::onChatToUser(const QString &userId)
{
    emit chatToUser(userId);
}

void DetailWidget::onCallToUser(const QString &userId, bool isAudio)
{
    emit callToUser(userId, isAudio);
}

void DetailWidget::showPage(int pageIndex)
{
    switch (pageIndex) {
    case 0: // 聊天
        if(chatDetailWidget->hasContent()) {
            stackedWidget->setCurrentWidget(chatDetailWidget);
        } else {
            stackedWidget->setCurrentWidget(emptyDetailWidget);
        }
        break;
    case 1: // 联系人
        if(contactDetailWidget->hasContent()) {
            stackedWidget->setCurrentWidget(contactDetailWidget);
        } else {
            stackedWidget->setCurrentWidget(emptyDetailWidget);
        }
        break;
    case 2: // 收藏
        stackedWidget->setCurrentWidget(favoriteDetailWidget);
        break;
    case 3: // 标签
        stackedWidget->setCurrentWidget(tagDetailWidget);
        break;
    default:
        stackedWidget->setCurrentWidget(emptyDetailWidget);
        break;
    }
}
