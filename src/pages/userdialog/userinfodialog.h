#ifndef USERINFODIALOG_H
#define USERINFODIALOG_H

#include <QDialog>
#include <QObject>
#include <QLabel>
#include <QFocusEvent>
#include <QLineEdit>
#include <QPushButton>

class AvatarWidget;
class UserInfoDialog : public QDialog
{
    Q_OBJECT
public:
    explicit UserInfoDialog(QWidget *parent = nullptr);
    void setUserAndPosition(const QString &userId, const QPoint &position);
protected:
    void focusOutEvent(QFocusEvent *event) override;
signals:
    void chatToUser(const QString &userId);
    void callToUser(const QString &userId, bool isAudio);

private:
    QString currentUserId;
private:
    AvatarWidget *avatarLabel;  // 头像标签
    QLabel *nameLabel;    // 名称标签
    QLabel *wfIdLabel; // 微信号标签
    QLabel *regionLabel;  // 地区标签
    QLabel *remarkLabel;  // 备注标签
    QLineEdit *remarkEdit; // 备注输入框
    QLabel *momentsLabel; // 朋友圈标签
    QLabel *groupChatLabel; // 共同群聊标签
    QLabel *sourceLabel; // 来源标签
    QPushButton *messageButton; // 发消息按钮
    QPushButton *voiceCallButton; // 语音聊天按钮
    QPushButton *videoCallButton; // 视频聊天按钮
};

#endif // USERINFODIALOG_H
