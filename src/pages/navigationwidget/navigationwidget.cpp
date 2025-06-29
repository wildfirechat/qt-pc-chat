#include "navigationwidget.h"
#include <QIcon>
#include <QSpacerItem>
#include <QMouseEvent>
#include "../utility/avatarwidget.h"
#include "../../wfc/client/ChatClient.h"
#include "../utility/badgebutton.h"


NavigationWidget::NavigationWidget(QWidget *parent) : QWidget(parent), m_dragging(false)
{
    setupUI();
    settingsDialog = new SettingsDialog(this);
}

NavigationWidget::~NavigationWidget()
{
}

void NavigationWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 10, 0, 10);
    mainLayout->setSpacing(10);

    // 头像
    avatarLabel = new AvatarWidget(40, this);
    avatarLabel->setRadius(6);

    WFCLib::UserInfo userInfo = WFCLib::ChatClient::Instance()->getUserInfo(WFCLib::ChatClient::Instance()->getCurrentUserId(), false);
    if(!userInfo.portrait.empty()) {
        avatarLabel->setUrl(userInfo.portrait.c_str());
    }

    // 导航按钮
    chatBtn = createBadgeNavButton(":/images/tabbar_message_hover.png", "消息");
    contactBtn = createBadgeNavButton(":/images/tabbar_contact.png", "联系人");
    favoriteBtn = createNavButton(":/images/tabbar_fav.png", "收藏");
    tagBtn = createNavButton(":/images/tabbar_navi.png", "标签");
    settingsBtn = createNavButton(":/images/tabbar_work.png", "设置");


    // 添加到布局
    mainLayout->addWidget(avatarLabel, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(chatBtn, 0, Qt::AlignHCenter);
    mainLayout->addWidget(contactBtn, 0, Qt::AlignHCenter);
    mainLayout->addWidget(favoriteBtn, 0, Qt::AlignHCenter);
    mainLayout->addWidget(tagBtn, 0, Qt::AlignHCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(settingsBtn, 0, Qt::AlignHCenter);

    // 连接信号和槽
    connect(chatBtn, &QPushButton::clicked, this, &NavigationWidget::onChatBtnClicked);
    connect(contactBtn, &QPushButton::clicked, this, &NavigationWidget::onContactBtnClicked);
    connect(favoriteBtn, &QPushButton::clicked, this, &NavigationWidget::onFavoriteBtnClicked);
    connect(tagBtn, &QPushButton::clicked, this, &NavigationWidget::onTagBtnClicked);
    connect(settingsBtn, &QPushButton::clicked, this, &NavigationWidget::onSettingsBtnClicked);

    connect(avatarLabel, &AvatarWidget::clicked, this, &NavigationWidget::onAvatarClicked);

    connect(WFCLib::ChatClient::Instance(), &WFCLib::ChatClient::receiveMessages, this, &NavigationWidget::reloadBadge);
    connect(WFCLib::ChatClient::Instance(), &WFCLib::ChatClient::recalledMessage, this, &NavigationWidget::reloadBadge);
    connect(WFCLib::ChatClient::Instance(), &WFCLib::ChatClient::deletedMessage, this, &NavigationWidget::reloadBadge);
    connect(WFCLib::ChatClient::Instance(), &WFCLib::ChatClient::unreadStatusCleared, this, &NavigationWidget::reloadBadge);
    connect(WFCLib::ChatClient::Instance(), &WFCLib::ChatClient::unreadStatusAllCleared, this, &NavigationWidget::reloadBadge);
    connect(WFCLib::ChatClient::Instance(), &WFCLib::ChatClient::friendRequestUpdated, this, &NavigationWidget::reloadBadge);
    connect(WFCLib::ChatClient::Instance(), &WFCLib::ChatClient::userSettingUpdated, this, &NavigationWidget::reloadBadge);

    reloadBadge();
}

QPushButton* NavigationWidget::createNavButton(const QString &ic_normal, const QString &title) {
    QPushButton *btn = new QPushButton(this);
    btn->setIcon(QIcon(ic_normal));
    btn->setIconSize(QSize(24, 24));
    btn->setFixedSize(44, 44);
    btn->setFlat(true);
    btn->setToolTip(title);
    btn->setCheckable(true);
    btn->focusWidget();
    btn->setCheckable(false);
    return btn;
}

BadgeButton *NavigationWidget::createBadgeNavButton(const QString &ic_normal, const QString &title)
{
    BadgeButton *btn = new BadgeButton(this);
    btn->setIcon(QIcon(ic_normal));
    btn->setIconSize(QSize(24, 24));
    btn->setFixedSize(44, 44);
    btn->setFlat(true);
    btn->setToolTip(title);
    btn->setCheckable(true);
    btn->focusWidget();
    btn->setCheckable(false);
    return btn;
}

void NavigationWidget::reloadBadge()
{
    WFCLib::UnreadCount unread = WFCLib::ChatClient::Instance()->getUnreadCount({WFCLib::Single_Type, WFCLib::Group_Type, WFCLib::Channel_Type}, {0});
    chatBtn->setBadgeNumber(unread.unread);

    int unreadFriendRequest = WFCLib::ChatClient::Instance()->getUnreadFriendRequestStatus();
    contactBtn->setBadgeNumber(unreadFriendRequest);
}

void NavigationWidget::onChatBtnClicked()
{
    emit navItemSelected(0);
    chatBtn->setIcon(QIcon(":/images/tabbar_message_hover.png"));
    contactBtn->setIcon(QIcon(":/images/tabbar_contact.png"));
    favoriteBtn->setIcon(QIcon(":/images/tabbar_fav.png"));
    tagBtn->setIcon(QIcon(":/images/tabbar_navi.png"));
}

void NavigationWidget::onContactBtnClicked()
{
    emit navItemSelected(1);
    chatBtn->setIcon(QIcon(":/images/tabbar_message.png"));
    contactBtn->setIcon(QIcon(":/images/tabbar_contact_hover.png"));
    favoriteBtn->setIcon(QIcon(":/images/tabbar_fav.png"));
    tagBtn->setIcon(QIcon(":/images/tabbar_navi.png"));
}

void NavigationWidget::onFavoriteBtnClicked()
{
    emit navItemSelected(2);
    chatBtn->setIcon(QIcon(":/images/tabbar_message.png"));
    contactBtn->setIcon(QIcon(":/images/tabbar_contact.png"));
    favoriteBtn->setIcon(QIcon(":/images/tabbar_fav.png"));
    tagBtn->setIcon(QIcon(":/images/tabbar_navi.png"));
}

void NavigationWidget::onTagBtnClicked()
{
    emit navItemSelected(3);
    chatBtn->setIcon(QIcon(":/images/tabbar_message.png"));
    contactBtn->setIcon(QIcon(":/images/tabbar_contact.png"));
    favoriteBtn->setIcon(QIcon(":/images/tabbar_fav.png"));
    tagBtn->setIcon(QIcon(":/images/tabbar_navi_hover.png"));
}


void NavigationWidget::onSettingsBtnClicked()
{
    settingsDialog->show();
}

void NavigationWidget::onAvatarClicked(const QPoint &position)
{
    emit showUserDialog(WFCLib::ChatClient::Instance()->getCurrentUserId().c_str(), "", mapToGlobal(position));
}

void NavigationWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_dragPosition = event->globalPos() - parentWidget()->parentWidget()->pos();
        event->accept();
    }
}

void NavigationWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        parentWidget()->parentWidget()->move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

void NavigationWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = false;
        event->accept();
    }
}








