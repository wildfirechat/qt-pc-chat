#include "mainwindow.h"
#include "../navigationwidget/navigationwidget.h"
#include "../userdialog/userinfodialog.h"
#include "contentwidget.h"
#include "detailwidget.h"
#include <QHBoxLayout> // 引入水平布局头文件
#include <QSettings>

#define MAIN_SPLITER_STATE "main_splitter/state"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    setupConnections();
    applyStyles();
    connect(WFCLib::ChatClient::Instance(), &WFCLib::ChatClient::connectionStatusChanged, this, &MainWindow::onConnectionsStatusChanged);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    setWindowTitle("野火IM");
    resize(1000, 700);
    setMinimumHeight(600);
    setMinimumWidth(880);

    // 创建水平布局
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(0, 0, 0, 1);

    // 创建左侧导航栏（固定64pt宽度）
    navigationWidget = new NavigationWidget(this);
    navigationWidget->setFixedWidth(56);
    mainLayout->addWidget(navigationWidget);

    // 创建中间内容区和右侧详情区的分割器
    mainSplitter = new QSplitter(Qt::Horizontal, this);
    contentWidget = new ContentWidget(mainSplitter);
    contentWidget->setMinimumWidth(220);
    detailWidget = new DetailWidget(mainSplitter);
    detailWidget->setMinimumWidth(500);

    mainSplitter->addWidget(contentWidget);
    mainSplitter->addWidget(detailWidget);


    // 设置分割器大小
    mainSplitter->setSizes({220, 636});
    mainSplitter->setCollapsible(0, false);
    mainLayout->addWidget(mainSplitter);

    // 恢复分割器状态
    QSettings settings;
    if (settings.contains(MAIN_SPLITER_STATE)) {
        mainSplitter->restoreState(settings.value(MAIN_SPLITER_STATE).toByteArray());
    }

    // 创建一个中心部件并设置布局
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // 创建用户信息窗口，其他界面需要显示时使用
    userInfoDialog = new UserInfoDialog(this);
    userInfoDialog->hide();
}

void MainWindow::setupConnections()
{
    // 连接导航栏和内容区
    connect(navigationWidget, &NavigationWidget::navItemSelected,
            contentWidget, &ContentWidget::showPage);
    connect(navigationWidget, &NavigationWidget::navItemSelected,
            detailWidget, &DetailWidget::showPage);

    // 连接内容区和详情区
    connect(contentWidget, &ContentWidget::itemSelected,
            detailWidget, &DetailWidget::showDetail);

    connect(contentWidget, &ContentWidget::conversationSelected,
            detailWidget, &DetailWidget::showChatDetail);

    connect(contentWidget, &ContentWidget::contactSelected,
            detailWidget, &DetailWidget::showContactDetail);

    connect(navigationWidget, &NavigationWidget::showUserDialog, this, &MainWindow::onShowUserDialog);
    connect(detailWidget, &DetailWidget::showUserDialog, this, &MainWindow::onShowUserDialog);
    connect(userInfoDialog, &UserInfoDialog::chatToUser, contentWidget, &ContentWidget::chatToUser);
    connect(detailWidget, &DetailWidget::chatToUser, contentWidget, &ContentWidget::chatToUser);
}

void MainWindow::applyStyles()
{
    setStyleSheet(R"(
        MainWindow {
            background-color: #E4E4E4;
        }

        QSplitter::handle {
            background-color: #e0e0e0;
            width: 1px;
        }
    )");
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // 保存分割器状态
    QSettings settings;
    settings.setValue(MAIN_SPLITER_STATE, mainSplitter->saveState());

    QMainWindow::closeEvent(event);
}

void MainWindow::onConnectionsStatusChanged(WFCLib::ConnectionStatus status) {
    if(status < WFCLib::kConnectionStatusUnconnected) {
        hide();
        disconnect(this);
    }
}

void MainWindow::onShowUserDialog(const QString &userId, const QString &groupId, const QPoint &position)
{
    userInfoDialog->setUserAndPosition(userId, position);
}
