#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <../../wfc/client/ChatClient.h>

/*
 * 这里是整个窗口的布局类。水平分为导航区、内容区和详情区三个区块。导航区包括当前用户头像，和消息/联系人/收藏/设置等按钮。
 * 内容区对应导航区的选择会显示消息会话列表/联系人列表/收藏列表等，这个内容区对应的是个列表。
 * 详情区对应的是内容区选定具体的项目显示的内容，比如选中会话，会展示消息界面；比如选中某个联系人，会显示这个联系人的信息。
 */
class NavigationWidget;
class ContentWidget;
class DetailWidget;
class UserInfoDialog;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onConnectionsStatusChanged(WFCLib::ConnectionStatus status);
    void onShowUserDialog(const QString &userId, const QString &groupId, const QPoint &position);

private:
    void setupUI();
    void setupConnections();
    void applyStyles();

    NavigationWidget *navigationWidget;
    ContentWidget *contentWidget;
    DetailWidget *detailWidget;
    
    QSplitter *mainSplitter;

    UserInfoDialog *userInfoDialog;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H    
