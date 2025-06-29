#include "conversationsettingswidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include "../../wfc/client/ChatClient.h"
#include "../../utility/avatarwidget.h"

const int ITEM_HEIGHT = 50;  // 定义每一项的固定高度
const int SEPARATOR_HEIGHT = 10;  // 定义分隔线的高度

ConversationSettingsWidget::ConversationSettingsWidget(QWidget *parent)
    : QWidget{parent}
{
    setupUI();
}

void ConversationSettingsWidget::setupUI()
{
    // 创建主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(5);

    // 添加群信息设置部分
    addGroupInfoSection(mainLayout);

    // 添加分隔线
    groupSeparator = addSeparator(mainLayout);

    // 添加聊天设置部分
    addChatSettingsSection(mainLayout);

    // 添加分隔线
    addSeparator(mainLayout);

    // 添加群成员部分（使用QListWidget）
    addMemberSection(mainLayout);

    // 添加分隔线
    addSeparator(mainLayout);

    // 添加底部按钮
    addBottomButtons(mainLayout);

    // 在布局最下方添加一个伸缩项，让内容靠紧在上面
    mainLayout->addStretch();

    setLayout(mainLayout);

    connect(dndWidget->checkBox, &QCheckBox::clicked, this, &ConversationSettingsWidget::silentSettingChecked);
    connect(pinWidget->checkBox, &QCheckBox::clicked, this, &ConversationSettingsWidget::topSettingChecked);
    connect(saveWidget->checkBox, &QCheckBox::clicked, this, &ConversationSettingsWidget::saveGroupSettingChecked);
    connect(groupShowNicknamesWidget->checkBox, &QCheckBox::clicked, this, &ConversationSettingsWidget::showNameSettingChecked);

    connect(clearWidget->button, &QPushButton::clicked, this, &ConversationSettingsWidget::clearButtonPressed);
    connect(leaveGroupWidget->button, &QPushButton::clicked, this, &ConversationSettingsWidget::leaveButtonPressed);
}

void ConversationSettingsWidget::setConversation(const WFCLib::Conversation &conversation)
{
    currentConversation = conversation;
    WFCLib::ConversationInfo conversationInfo = WFCLib::ChatClient::Instance()->getConversationInfo(conversation);
    if(conversation.conversationType == WFCLib::Single_Type) {
        groupNameWidget->hide();
        groupAnnouncementWidget->hide();
        groupRemarkWidget->hide();
        groupNicknameWidget->hide();
        groupSeparator->hide();

        searchWidget->show();
        dndWidget->setChecked(conversationInfo.isSilent);
        pinWidget->setChecked(conversationInfo.isTop);

        saveWidget->hide();
        groupShowNicknamesWidget->hide();

        memberListWidget->show();
        updateMemberList(conversation);

        leaveGroupWidget->hide();
    } else if(conversation.conversationType == WFCLib::Group_Type) {
        WFCLib::GroupInfo groupInfo = WFCLib::ChatClient::Instance()->getGroupInfo(conversation.target);
        groupNameWidget->show();
        groupNameWidget->setText(groupInfo.name.c_str());

        groupAnnouncementWidget->show();

        groupRemarkWidget->show();
        groupRemarkWidget->setText(groupInfo.remark.c_str());

        WFCLib::UserInfo userInfo = WFCLib::ChatClient::Instance()->getUserInfo(WFCLib::ChatClient::Instance()->getCurrentUserId(), conversation.conversationType == WFCLib::Group_Type?conversation.target:"", false);
        groupNicknameWidget->show();
        groupNicknameWidget->setText(userInfo.groupAlias.c_str());

        groupSeparator->show();

        searchWidget->show();
        dndWidget->setChecked(conversationInfo.isSilent);
        pinWidget->setChecked(conversationInfo.isTop);

        saveWidget->show();
        saveWidget->setChecked(WFCLib::ChatClient::Instance()->isFavGroup(conversation.target));

        groupShowNicknamesWidget->show();
        groupShowNicknamesWidget->setChecked(!WFCLib::ChatClient::Instance()->isHiddenGroupMemberName(conversation.target));

        memberListWidget->show();
        updateMemberList(conversation);

        leaveGroupWidget->show();
    } else if(conversation.conversationType == WFCLib::Channel_Type) {
        groupNameWidget->hide();
        groupAnnouncementWidget->hide();
        groupRemarkWidget->hide();
        groupNicknameWidget->hide();
        groupSeparator->hide();

        searchWidget->show();
        dndWidget->setChecked(conversationInfo.isSilent);
        pinWidget->setChecked(conversationInfo.isTop);

        saveWidget->hide();
        groupShowNicknamesWidget->hide();

        memberListWidget->hide();
        leaveGroupWidget->hide();
    } else {

    }
}

void ConversationSettingsWidget::updateMemberList(const WFCLib::Conversation &conversation)
{
    memberListWidget->clear();

    // 添加群成员项
    QStringList memberIds;
    if(conversation.conversationType == WFCLib::Single_Type) {
        memberIds.append(conversation.target.c_str());
    } else if(conversation.conversationType == WFCLib::Group_Type) {
        auto groupMembers = WFCLib::ChatClient::Instance()->getGroupMembers(conversation.target, true);
        foreach (auto member, groupMembers) {
            memberIds.append(member.memberId.c_str());
        }
    } else {
        return;
    }

    for (const QString &memberId : memberIds) {
        WFCLib::UserInfo userInfo = WFCLib::ChatClient::Instance()->getUserInfo(memberId.toStdString(), conversation.conversationType == WFCLib::Group_Type?conversation.target:"", false);
        QListWidgetItem *item = new QListWidgetItem(memberListWidget);
        item->setSizeHint(QSize(0, ITEM_HEIGHT));
        memberListWidget->addItem(item);

        MemberItemWidget *widget = new MemberItemWidget(userInfo.getReadableName().c_str(), memberListWidget);
        widget->setAvatarUrl(userInfo.portrait.c_str());
        memberListWidget->setItemWidget(item, widget);
    }

    // 计算列表的高度（标题 + 成员数量 * 项高度）
    int listHeight = 40 + std::min(6, (int)memberIds.size()) * ITEM_HEIGHT;
    memberListWidget->setFixedHeight(listHeight);
}

void ConversationSettingsWidget::addMemberSection(QVBoxLayout *layout) {
    // 创建群成员专用的QListWidget
    memberListWidget = new QListWidget(this);
    memberListWidget->setStyleSheet(
        "QListWidget {"
        "    border: none;"
        "    background-color: white;"
        "}"
        "QListWidget::item {"
        "    border: none;"
        "    padding: 0px;"
        "}"
        "QListWidget::item:selected {"
        "    background-color: #E8F0FE;"
        "}"
        );
    memberListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    memberListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 添加标题项
    QListWidgetItem *titleItem = new QListWidgetItem(memberListWidget);
    titleItem->setSizeHint(QSize(0, 40));
    titleItem->setFlags(titleItem->flags() & ~Qt::ItemIsSelectable);
    memberListWidget->addItem(titleItem);

    QWidget *titleWidget = new QWidget(memberListWidget);
    QHBoxLayout *titleLayout = new QHBoxLayout(titleWidget);
    titleLayout->setContentsMargins(15, 0, 15, 0);

    QLabel *titleLabel = new QLabel("群成员", memberListWidget);
    titleLabel->setStyleSheet("font-size: 12px; color: #888888;");

    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();

    memberListWidget->setItemWidget(titleItem, titleWidget);

    // 将成员列表添加到主布局
    layout->addWidget(memberListWidget);
}

QWidget* ConversationSettingsWidget::addSeparator(QVBoxLayout *layout) {
    QWidget *separatorWidget = new QWidget(this);
    separatorWidget->setFixedHeight(SEPARATOR_HEIGHT);
    separatorWidget->setStyleSheet("background-color: #F5F5F5;");
    layout->addWidget(separatorWidget);
    return separatorWidget;
}

void ConversationSettingsWidget::addGroupInfoSection(QVBoxLayout *layout) {
    // 添加群聊名称
    groupNameWidget = new TextItemWidget("群聊名称", "野火即时通讯技术交流群", this);
    groupNameWidget->setFixedHeight(ITEM_HEIGHT);
    layout->addWidget(groupNameWidget);

    // 添加群公告
    groupAnnouncementWidget = new TextItemWidget("群公告", "群主未设置", this);
    groupAnnouncementWidget->setFixedHeight(ITEM_HEIGHT);
    layout->addWidget(groupAnnouncementWidget);

    // 添加备注
    groupRemarkWidget = new TextItemWidget("备注", "群聊的备注仅自己可见", this);
    groupRemarkWidget->setFixedHeight(ITEM_HEIGHT);
    layout->addWidget(groupRemarkWidget);

    // 添加我的昵称
    groupNicknameWidget = new TextItemWidget("我在本群的昵称", "野火", this);
    groupNicknameWidget->setFixedHeight(ITEM_HEIGHT);
    layout->addWidget(groupNicknameWidget);
}

void ConversationSettingsWidget::addChatSettingsSection(QVBoxLayout *layout) {
    // 添加查找聊天内容
    searchWidget = new InputItemWidget("查找聊天内容", "搜索关键词", this);
    searchWidget->setFixedHeight(ITEM_HEIGHT);
    layout->addWidget(searchWidget);

    // 添加消息免打扰
    dndWidget = new SettingItemWidget("消息免打扰", false, this);
    dndWidget->setFixedHeight(ITEM_HEIGHT);
    layout->addWidget(dndWidget);

    // 添加置顶聊天
    pinWidget = new SettingItemWidget("置顶聊天", false, this);
    pinWidget->setFixedHeight(ITEM_HEIGHT);
    layout->addWidget(pinWidget);

    // 添加保存到通讯录
    saveWidget = new SettingItemWidget("保存到通讯录", false, this);
    saveWidget->setFixedHeight(ITEM_HEIGHT);
    layout->addWidget(saveWidget);

    // 添加显示群成员昵称
    groupShowNicknamesWidget = new SettingItemWidget("显示群成员昵称", true, this);
    groupShowNicknamesWidget->setFixedHeight(ITEM_HEIGHT);
    layout->addWidget(groupShowNicknamesWidget);
}

void ConversationSettingsWidget::addBottomButtons(QVBoxLayout *layout) {
    // 添加清空聊天记录按钮
    clearWidget = new ButtonItemWidget(
        "清空聊天记录",
        "QPushButton {"
        "    color: #FF5252;"
        "    background-color: white;"
        "    border: none;"
        "    border-radius: 5px;"
        "    padding: 4px;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #FFF0F0;"
        "}",
        this
        );
    clearWidget->setFixedHeight(ITEM_HEIGHT);
    layout->addWidget(clearWidget);

    // 添加退出群聊按钮
    leaveGroupWidget = new ButtonItemWidget(
        "退出群聊",
        "QPushButton {"
        "    color: #FF5252;"
        "    background-color: white;"
        "    border: none;"
        "    border-radius: 5px;"
        "    padding: 4px;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #FFF0F0;"
        "}",
        this
        );
    leaveGroupWidget->setFixedHeight(ITEM_HEIGHT);
    layout->addWidget(leaveGroupWidget);
}

void ConversationSettingsWidget::silentSettingChecked(bool checked)
{
    WFCLib::ChatClient::Instance()->setConversationSilent(currentConversation, checked, NULL, 0);
}

void ConversationSettingsWidget::topSettingChecked(bool checked)
{
    WFCLib::ChatClient::Instance()->setConversationTop(currentConversation, checked, NULL, 0);
}

void ConversationSettingsWidget::saveGroupSettingChecked(bool checked)
{
    WFCLib::ChatClient::Instance()->setFavGroup(currentConversation.target, checked, NULL, 0);
}

void ConversationSettingsWidget::showNameSettingChecked(bool checked)
{
    WFCLib::ChatClient::Instance()->setHiddenGroupMemberName(!checked, currentConversation.target, NULL, 0);
}

void ConversationSettingsWidget::clearButtonPressed()
{
    WFCLib::ChatClient::Instance()->clearMessages(currentConversation);
}

void ConversationSettingsWidget::leaveButtonPressed()
{
    WFCLib::ChatClient::Instance()->quitGroup(currentConversation.target);
}

MemberItemWidget::MemberItemWidget(const QString &name, QWidget *parent) : QWidget(parent) {
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(10, 5, 10, 5);

    // 添加头像（使用占位图标）
    avatarLabel = new AvatarWidget(40, this);
    avatarLabel->setStyleSheet("background-color: #E0E0E0; border-radius: 5px;");

    // 添加名称
    nameLabel = new QLabel(name, this);
    nameLabel->setStyleSheet("font-size: 14px;");

    layout->addWidget(avatarLabel);
    layout->addWidget(nameLabel);
    layout->addStretch();
}

void MemberItemWidget::setAvatarUrl(const QString &avatarUrl)
{
    avatarLabel->setUrl(avatarUrl);
}

void MemberItemWidget::setAvatarImage(const QPixmap &pixmap)
{
    avatarLabel->setImage(pixmap);
}

SettingItemWidget::SettingItemWidget(const QString &title, bool checked, QWidget *parent) : QWidget(parent) {
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(10, 15, 10, 15);

    // 添加标题
    QLabel *titleLabel = new QLabel(title, this);
    titleLabel->setStyleSheet("font-size: 14px;");

    // 添加开关
    checkBox = new QCheckBox(this);
    checkBox->setChecked(checked);

    layout->addWidget(titleLabel);
    layout->addStretch();
    layout->addWidget(checkBox);
}

void SettingItemWidget::setChecked(bool checked)
{
    checkBox->setChecked(checked);
}

TextItemWidget::TextItemWidget(const QString &title, const QString &text, QWidget *parent) : QWidget(parent) {
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(10, 15, 10, 15);

    // 添加标题
    QLabel *titleLabel = new QLabel(title, this);
    titleLabel->setStyleSheet("font-size: 14px;");

    // 添加文本
    textLabel = new QLabel(text, this);
    textLabel->setStyleSheet("font-size: 14px; color: #888888;");
    textLabel->setAlignment(Qt::AlignRight);

    layout->addWidget(titleLabel);
    layout->addStretch();
    layout->addWidget(textLabel);
}

void TextItemWidget::setText(const QString &text)
{
    textLabel->setText(text);
}

InputItemWidget::InputItemWidget(const QString &title, const QString &placeholder, QWidget *parent) : QWidget(parent) {
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);

    // 添加标题
    QLabel *titleLabel = new QLabel(title, this);
    titleLabel->setStyleSheet("font-size: 14px;");

    // 添加输入框
    lineEdit = new QLineEdit(this);
    lineEdit->setPlaceholderText(placeholder);
    lineEdit->setStyleSheet("border: 1px solid #CCCCCC; border-radius: 3px; padding: 5px;");

    layout->addWidget(titleLabel);
    layout->addWidget(lineEdit, 1);
}

ButtonItemWidget::ButtonItemWidget(const QString &text, const QString &style, QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 15, 20, 15);

    // 添加按钮
    button = new QPushButton(text, this);
    button->setStyleSheet(style);
    button->setCursor(Qt::PointingHandCursor);

    layout->addWidget(button);
}
