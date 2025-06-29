#ifndef CONVERSATIONSETTINGSWIDGET_H
#define CONVERSATIONSETTINGSWIDGET_H

#include <QWidget>
#include <QListWidget>
#include "../../../wfc/model/Conversation.h"

class QLabel;
class AvatarWidget;
class QCheckBox;
class QPushButton;

// 自定义列表项控件 - 用于群成员项
class MemberItemWidget : public QWidget {
    Q_OBJECT
public:
    MemberItemWidget(const QString &name, QWidget *parent = nullptr);
    void setName(const QString &name);
    void setAvatarUrl(const QString &avatarUrl);
    void setAvatarImage(const QPixmap &pixmap);
private:
    QLabel *nameLabel;

public:
    AvatarWidget *avatarLabel;
};

// 自定义列表项控件 - 用于设置项（带开关）
class SettingItemWidget : public QWidget {
    Q_OBJECT
public:
    SettingItemWidget(const QString &title, bool checked = false, QWidget *parent = nullptr);
    void setChecked(bool checked);

public:
    QCheckBox *checkBox;
};

// 自定义列表项控件 - 用于设置项（带文本）
class TextItemWidget : public QWidget {
    Q_OBJECT
public:
    TextItemWidget(const QString &title, const QString &text, QWidget *parent = nullptr);
    void setText(const QString &text);
private:
    QLabel *textLabel;
};

// 自定义列表项控件 - 用于输入项
class InputItemWidget : public QWidget {
    Q_OBJECT
public:
    InputItemWidget(const QString &title, const QString &placeholder, QWidget *parent = nullptr);

public:
    QLineEdit *lineEdit;
};

// 自定义列表项控件 - 用于按钮项
class ButtonItemWidget : public QWidget {
    Q_OBJECT
public:
    ButtonItemWidget(const QString &text, const QString &style, QWidget *parent = nullptr);
    QPushButton *button;
};

namespace WFCLib {
class Conversation;
}
class QVBoxLayout;
class ConversationSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConversationSettingsWidget(QWidget *parent = nullptr);
    void setConversation(const WFCLib::Conversation &conversation);

private:
    void setupUI();

private:
    void updateMemberList(const WFCLib::Conversation &conversation);

    // 添加群成员部分（使用QListWidget）
    void addMemberSection(QVBoxLayout *layout);

    // 添加分隔线
    QWidget* addSeparator(QVBoxLayout *layout);

    // 添加群信息设置部分
    void addGroupInfoSection(QVBoxLayout *layout);

    // 添加聊天设置部分
    void addChatSettingsSection(QVBoxLayout *layout);

    // 添加底部按钮
    void addBottomButtons(QVBoxLayout *layout);

private slots:
    void silentSettingChecked(bool checked);
    void topSettingChecked(bool checked);
    void saveGroupSettingChecked(bool checked);
    void showNameSettingChecked(bool checked);

    void clearButtonPressed();
    void leaveButtonPressed();

private:
    // 群聊名称
    TextItemWidget *groupNameWidget;
    // 群公告
    TextItemWidget *groupAnnouncementWidget;
    // 备注
    TextItemWidget *groupRemarkWidget;
    // 我的昵称
    TextItemWidget *groupNicknameWidget;
    // 群组信息分割线
    QWidget* groupSeparator;

    // 查找聊天内容
    InputItemWidget *searchWidget;
    // 消息免打扰
    SettingItemWidget *dndWidget;
    // 置顶聊天
    SettingItemWidget *pinWidget;
    // 保存到通讯录
    SettingItemWidget *saveWidget;
    // 显示群成员昵称
    SettingItemWidget *groupShowNicknamesWidget;

    // 成员专用的QListWidget
    QListWidget *memberListWidget;

    // 清除消息按钮
    ButtonItemWidget *clearWidget;
    // 退出群聊按钮
    ButtonItemWidget *leaveGroupWidget;

private:
    WFCLib::Conversation currentConversation;
};

#endif // CONVERSATIONSETTINGSWIDGET_H
