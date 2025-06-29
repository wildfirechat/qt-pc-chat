#ifndef DETAILWIDGET_H
#define DETAILWIDGET_H

#include <QWidget>
#include <QStackedWidget>

class ChatDetailWidget;
class ContactDetailWidget;
class FavoriteDetailWidget;
class TagDetailWidget;
class EmptyDetailWidget;
class FriendRequestDetailWidget;
class GroupDetailWidget;
class ChannelDetailWidget;

namespace WFCLib {
class Conversation;
} // namespace WFCLib

class DetailWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DetailWidget(QWidget *parent = nullptr);
    ~DetailWidget();

signals:
    void showUserDialog(const QString &userId, const QString &groupId, const QPoint &position);
    void chatToUser(const QString &userId);
    void callToUser(const QString &userId, bool isAudio);

public slots:
    void onShowUserDialog(const QString &userId, const QString &groupId, const QPoint &position);
    void onChatToUser(const QString &userId);
    void onCallToUser(const QString &userId, bool isAudio);
    //导航切换tab页面
    void showPage(int index);
    //中间内容区选定项目
    void showDetail(int pageIndex, const QString &itemId);
    //中间内容区的消会话列表选定会话
    void showChatDetail(int pageIndex, const WFCLib::Conversation &itemId);
    //中间内容区的联系人列表选定项目
    void showContactDetail(int pageIndex, int type, const QString &itemId);

private:
    void setupUI();
    
    QStackedWidget *stackedWidget;
    ChatDetailWidget *chatDetailWidget;
    ContactDetailWidget *contactDetailWidget;
    FavoriteDetailWidget *favoriteDetailWidget;
    TagDetailWidget *tagDetailWidget;
    EmptyDetailWidget *emptyDetailWidget;
    FriendRequestDetailWidget *friendRequestDetailWidget;
    GroupDetailWidget *groupDetailWidget;
    ChannelDetailWidget *channelDetailWidget;
};

#endif // DETAILWIDGET_H    
