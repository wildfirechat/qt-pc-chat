#include "settingswidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QSlider>
#include <QGroupBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QFileDialog>
#include <QColorDialog>
#include <QListWidget>
#include <QTabWidget>
#include <QRadioButton>
#include <QButtonGroup>
#include <QTimeEdit>
#include <../../wfc/client/ChatClient.h>

SettingsWidget::SettingsWidget(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

SettingsWidget::~SettingsWidget()
{
}

void SettingsWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    // 标题
    QLabel *titleLabel = new QLabel("设置", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    titleLabel->setAlignment(Qt::AlignCenter);

    // 创建标签页
    QTabWidget *tabWidget = new QTabWidget(this);

    // 账号设置页
    QWidget *accountPage = new QWidget(this);
    QVBoxLayout *accountLayout = new QVBoxLayout(accountPage);

    QHBoxLayout *avatarLayout = new QHBoxLayout();
    QLabel *avatarLabel = new QLabel("当前头像:", accountPage);
    QLabel *avatarPreview = new QLabel(accountPage);
    avatarPreview->setPixmap(QIcon(":/icons/avatar.png").pixmap(64, 64));
    avatarPreview->setFixedSize(64, 64);
    avatarPreview->setStyleSheet("border: 1px solid #ccc; border-radius: 32px;");
    QPushButton *changeAvatarBtn = new QPushButton("更改头像", accountPage);

    avatarLayout->addWidget(avatarLabel);
    avatarLayout->addWidget(avatarPreview);
    avatarLayout->addWidget(changeAvatarBtn);
    avatarLayout->addStretch();

    QLineEdit *usernameEdit = new QLineEdit("用户名", accountPage);
    QLineEdit *signatureEdit = new QLineEdit("个性签名", accountPage);
    QPushButton *changePasswordBtn = new QPushButton("修改密码", accountPage);
    QPushButton *logoutBtn = new QPushButton("退出登录", accountPage);
    logoutBtn->setStyleSheet("color: red;");

    accountLayout->addLayout(avatarLayout);
    accountLayout->addWidget(new QLabel("用户名:", accountPage));
    accountLayout->addWidget(usernameEdit);
    accountLayout->addWidget(new QLabel("个性签名:", accountPage));
    accountLayout->addWidget(signatureEdit);
    accountLayout->addWidget(changePasswordBtn);
    accountLayout->addWidget(logoutBtn);
    accountLayout->addStretch();

    // 通知设置页
    QWidget *notificationPage = new QWidget(this);
    QVBoxLayout *notificationLayout = new QVBoxLayout(notificationPage);

    QGroupBox *messageGroup = new QGroupBox("消息通知", notificationPage);
    QVBoxLayout *messageLayout = new QVBoxLayout(messageGroup);

    QCheckBox *soundCheckBox = new QCheckBox("声音提醒", messageGroup);
    soundCheckBox->setChecked(true);
    QCheckBox *vibrateCheckBox = new QCheckBox("震动提醒", messageGroup);
    QCheckBox *previewCheckBox = new QCheckBox("显示消息内容", messageGroup);
    QCheckBox *desktopCheckBox = new QCheckBox("桌面通知", messageGroup);
    desktopCheckBox->setChecked(true);

    messageLayout->addWidget(soundCheckBox);
    messageLayout->addWidget(vibrateCheckBox);
    messageLayout->addWidget(previewCheckBox);
    messageLayout->addWidget(desktopCheckBox);

    QGroupBox *reminderGroup = new QGroupBox("提醒设置", notificationPage);
    QVBoxLayout *reminderLayout = new QVBoxLayout(reminderGroup);

    QCheckBox *mentionCheckBox = new QCheckBox("@我时提醒", reminderGroup);
    mentionCheckBox->setChecked(true);
    QCheckBox *fileCheckBox = new QCheckBox("文件接收提醒", reminderGroup);
    QCheckBox *friendRequestCheckBox = new QCheckBox("好友请求提醒", reminderGroup);

    reminderLayout->addWidget(mentionCheckBox);
    reminderLayout->addWidget(fileCheckBox);
    reminderLayout->addWidget(friendRequestCheckBox);

    QGroupBox *doNotDisturbGroup = new QGroupBox("免打扰模式", notificationPage);
    QVBoxLayout *dndLayout = new QVBoxLayout(doNotDisturbGroup);

    QRadioButton *dndOffBtn = new QRadioButton("关闭", doNotDisturbGroup);
    QRadioButton *dndOnBtn = new QRadioButton("开启", doNotDisturbGroup);
    QRadioButton *dndScheduleBtn = new QRadioButton("定时开启", doNotDisturbGroup);

    QButtonGroup *dndGroup = new QButtonGroup(this);
    dndGroup->addButton(dndOffBtn);
    dndGroup->addButton(dndOnBtn);
    dndGroup->addButton(dndScheduleBtn);
    dndOffBtn->setChecked(true);

    QHBoxLayout *dndTimeLayout = new QHBoxLayout();
    QLabel *startTimeLabel = new QLabel("开始时间:", doNotDisturbGroup);
    QTimeEdit *startTimeEdit = new QTimeEdit(doNotDisturbGroup);
    startTimeEdit->setTime(QTime(22, 0));
    QLabel *endTimeLabel = new QLabel("结束时间:", doNotDisturbGroup);
    QTimeEdit *endTimeEdit = new QTimeEdit(doNotDisturbGroup);
    endTimeEdit->setTime(QTime(8, 0));

    dndTimeLayout->addWidget(startTimeLabel);
    dndTimeLayout->addWidget(startTimeEdit);
    dndTimeLayout->addWidget(endTimeLabel);
    dndTimeLayout->addWidget(endTimeEdit);

    dndLayout->addWidget(dndOffBtn);
    dndLayout->addWidget(dndOnBtn);
    dndLayout->addWidget(dndScheduleBtn);
    dndLayout->addLayout(dndTimeLayout);

    notificationLayout->addWidget(messageGroup);
    notificationLayout->addWidget(reminderGroup);
    notificationLayout->addWidget(doNotDisturbGroup);
    notificationLayout->addStretch();

    // 外观设置页
    QWidget *appearancePage = new QWidget(this);
    QVBoxLayout *appearanceLayout = new QVBoxLayout(appearancePage);

    QGroupBox *themeGroup = new QGroupBox("主题", appearancePage);
    QVBoxLayout *themeLayout = new QVBoxLayout(themeGroup);

    QRadioButton *lightThemeBtn = new QRadioButton("浅色主题", themeGroup);
    QRadioButton *darkThemeBtn = new QRadioButton("深色主题", themeGroup);
    QRadioButton *systemThemeBtn = new QRadioButton("跟随系统", themeGroup);

    QButtonGroup *themeGroupBtn = new QButtonGroup(this);
    themeGroupBtn->addButton(lightThemeBtn);
    themeGroupBtn->addButton(darkThemeBtn);
    themeGroupBtn->addButton(systemThemeBtn);
    lightThemeBtn->setChecked(true);

    themeLayout->addWidget(lightThemeBtn);
    themeLayout->addWidget(darkThemeBtn);
    themeLayout->addWidget(systemThemeBtn);

    QGroupBox *fontGroup = new QGroupBox("字体设置", appearancePage);
    QVBoxLayout *fontLayout = new QVBoxLayout(fontGroup);

    QLabel *fontSizeLabel = new QLabel("字体大小:", fontGroup);
    QSlider *fontSizeSlider = new QSlider(Qt::Horizontal, fontGroup);
    fontSizeSlider->setMinimum(8);
    fontSizeSlider->setMaximum(20);
    fontSizeSlider->setValue(12);

    QLabel *fontSizePreview = new QLabel("预览文本", fontGroup);
    fontSizePreview->setStyleSheet("font-size: 12pt;");

    connect(fontSizeSlider, &QSlider::valueChanged, [fontSizePreview](int value) {
        fontSizePreview->setStyleSheet(QString("font-size: %1pt;").arg(value));
    });

    fontLayout->addWidget(fontSizeLabel);
    fontLayout->addWidget(fontSizeSlider);
    fontLayout->addWidget(fontSizePreview);

    QGroupBox *layoutGroup = new QGroupBox("布局设置", appearancePage);
    QVBoxLayout *layoutLayout = new QVBoxLayout(layoutGroup);

    QCheckBox *compactModeCheckBox = new QCheckBox("紧凑模式", layoutGroup);
    QCheckBox *showAvatarCheckBox = new QCheckBox("显示头像", layoutGroup);
    showAvatarCheckBox->setChecked(true);

    layoutLayout->addWidget(compactModeCheckBox);
    layoutLayout->addWidget(showAvatarCheckBox);

    appearanceLayout->addWidget(themeGroup);
    appearanceLayout->addWidget(fontGroup);
    appearanceLayout->addWidget(layoutGroup);
    appearanceLayout->addStretch();

    // 隐私设置页
    QWidget *privacyPage = new QWidget(this);
    QVBoxLayout *privacyLayout = new QVBoxLayout(privacyPage);

    QGroupBox *visibilityGroup = new QGroupBox("个人资料可见性", privacyPage);
    QVBoxLayout *visibilityLayout = new QVBoxLayout(visibilityGroup);

    QComboBox *profileVisibilityCombo = new QComboBox(visibilityGroup);
    profileVisibilityCombo->addItems({"所有人", "仅好友", "仅自己"});

    visibilityLayout->addWidget(new QLabel("个人资料:", visibilityGroup));
    visibilityLayout->addWidget(profileVisibilityCombo);

    QGroupBox *contactGroup = new QGroupBox("联系人管理", privacyPage);
    QVBoxLayout *contactLayout = new QVBoxLayout(contactGroup);

    QCheckBox *allowSearchByPhoneCheckBox = new QCheckBox("允许通过手机号搜索", contactGroup);
    allowSearchByPhoneCheckBox->setChecked(true);
    QCheckBox *allowSearchByEmailCheckBox = new QCheckBox("允许通过邮箱搜索", contactGroup);
    QCheckBox *autoAcceptFileCheckBox = new QCheckBox("自动接收文件", contactGroup);

    contactLayout->addWidget(allowSearchByPhoneCheckBox);
    contactLayout->addWidget(allowSearchByEmailCheckBox);
    contactLayout->addWidget(autoAcceptFileCheckBox);

    privacyLayout->addWidget(visibilityGroup);
    privacyLayout->addWidget(contactGroup);
    privacyLayout->addStretch();

    // 高级设置页
    QWidget *advancedPage = new QWidget(this);
    QVBoxLayout *advancedLayout = new QVBoxLayout(advancedPage);

    QGroupBox *storageGroup = new QGroupBox("存储设置", advancedPage);
    QVBoxLayout *storageLayout = new QVBoxLayout(storageGroup);

    QLabel *downloadPathLabel = new QLabel("文件下载路径:", storageGroup);
    QLineEdit *downloadPathEdit = new QLineEdit(QDir::homePath() + "/Downloads", storageGroup);
    QPushButton *browsePathBtn = new QPushButton("浏览...", storageGroup);

    QHBoxLayout *pathLayout = new QHBoxLayout();
    pathLayout->addWidget(downloadPathEdit);
    pathLayout->addWidget(browsePathBtn);

    QLabel *cacheSizeLabel = new QLabel("缓存大小: 24.5MB", storageGroup);
    QPushButton *clearCacheBtn = new QPushButton("清除缓存", storageGroup);

    storageLayout->addWidget(downloadPathLabel);
    storageLayout->addLayout(pathLayout);
    storageLayout->addWidget(cacheSizeLabel);
    storageLayout->addWidget(clearCacheBtn);

    QGroupBox *securityGroup = new QGroupBox("安全设置", advancedPage);
    QVBoxLayout *securityLayout = new QVBoxLayout(securityGroup);

    QCheckBox *autoUpdateCheckBox = new QCheckBox("自动更新", securityGroup);
    autoUpdateCheckBox->setChecked(true);
    QCheckBox *encryptDataCheckBox = new QCheckBox("加密聊天数据", securityGroup);

    securityLayout->addWidget(autoUpdateCheckBox);
    securityLayout->addWidget(encryptDataCheckBox);

    advancedLayout->addWidget(storageGroup);
    advancedLayout->addWidget(securityGroup);
    advancedLayout->addStretch();

    // 添加标签页
    tabWidget->addTab(accountPage, "账号");
    tabWidget->addTab(notificationPage, "通知");
    tabWidget->addTab(appearancePage, "外观");
    tabWidget->addTab(privacyPage, "隐私");
    tabWidget->addTab(advancedPage, "高级");

    // 添加到主布局
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(tabWidget);

    // 连接信号和槽
    connect(changeAvatarBtn, &QPushButton::clicked, [=]() {
        QString fileName = QFileDialog::getOpenFileName(this, "选择头像", "", "图像文件 (*.png *.jpg *.jpeg)");
        if (!fileName.isEmpty()) {
            avatarPreview->setPixmap(QPixmap(fileName).scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    });

    connect(browsePathBtn, &QPushButton::clicked, [=]() {
        QString dir = QFileDialog::getExistingDirectory(this, "选择下载路径", QDir::homePath());
        if (!dir.isEmpty()) {
            downloadPathEdit->setText(dir);
        }
    });

    connect(clearCacheBtn, &QPushButton::clicked, [=]() {
        // 清除缓存的逻辑
        cacheSizeLabel->setText("缓存大小: 0MB");
    });

    connect(logoutBtn, &QPushButton::clicked, [this](){
        WFCLib::ChatClient::Instance()->disconnect();
    });
}
