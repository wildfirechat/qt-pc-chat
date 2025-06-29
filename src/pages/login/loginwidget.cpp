#include "loginwidget.h"
#include <QAction>
#include <QRegularExpressionValidator>
#include "../../wfc/client/ChatClient.h"
#include "QDebug"
#include "../../appservice/appservice.h"
#include <QPropertyAnimation>
#include "../../vendor//qrcode/QrCodeGenerator.h"
#include "../../config.h"
#include "../mainwindow/mainwindow.h"
#include <QSettings>

LoginWidget::LoginWidget(QWidget *parent)
    : QWidget(parent), windowWidth(400)
{
    qrcodeLogin = true;
    setWindowFlags(Qt::FramelessWindowHint);
    setFixedSize(QSize(windowWidth, 500));

    QPushButton* closeButton = new QPushButton("X", this);
    closeButton->setFlat(true);
    closeButton->setFixedSize(QSize(24, 24));
    connect(closeButton, &QPushButton::clicked, this, &QWidget::close);
    // 设置关闭按钮的位置和样式
    closeButton->move(this->width() - closeButton->width(), 0);


    layout = new QVBoxLayout;
    layout->setAlignment(Qt::AlignVCenter);
    setStyleSheet("background-color: white;");
    setLayout(layout);

    setupQrSubWidges();
    setupPwdSubWidges();
    updateLoginType();

    switchBtn = new QPushButton;
    switchBtn->setText("使用密码/验证码登录");
    switchBtn->setFlat(true); // 禁用默认的3D效果
    switchBtn->setStyleSheet("color: blue;"); // 可选：设置文本颜色
    layout->addWidget(switchBtn);


    connect(switchBtn, &QPushButton::clicked, this, &LoginWidget::onSwitchBtnPressed);
    connect(WFCLib::ChatClient::Instance(), &WFCLib::ChatClient::connectionStatusChanged, this, &LoginWidget::onConnectionsStatusChanged);
    connect(WFCLib::ChatClient::Instance(), &WFCLib::ChatClient::receiveMessages, this, &LoginWidget::onReceiveMessages);

    QSettings settings("WfcData", "WFC");
    QString savedPhoneNum = settings.value("wfc/phone_number", "").toString();
    QString savedToken = settings.value("wfc/token", "").toString();
    QString savedUserId = settings.value("wfc/user_id", "").toString();

    if(!savedPhoneNum.isEmpty()) {
        mobileEdit->setText(savedPhoneNum);
    }

    if(!savedToken.isEmpty() && !savedUserId.isEmpty()) {
        connect2ImServer(savedUserId, savedToken);
    }

    WFCLib::ChatClient::Instance()->setDefaultPortraitProvider(AppService::sharedAppService());
}

#define WIDGET_WIDTH 250
void LoginWidget::setupQrSubWidges() {
    qrcodeLabel = new QLabel;
    qrcodeLabel->setFixedSize(QSize(250, 250));
    qrcodeLabel->setStyleSheet("background-color: gray;");
    qrcodeLabel->setScaledContents(true);
    qrcodeLabel->setText("正在生成二维码");
    qrcodeLabel->setAlignment(Qt::AlignCenter);

    createPCSession(nullptr);

    titleLabel = new QLabel;
    titleLabel->setText("扫码登录野火IM");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 16pt;");

    hintLabel = new QLabel;
    hintLabel->setText("野火IM PC客户端需要配合您的手机客户端登录使用\n不是使用微信扫码登录！！！");
    hintLabel->setAlignment(Qt::AlignCenter);
    hintLabel->setStyleSheet("color: gray;");

    downloadBtn = new QPushButton;
    downloadBtn->setText("点击下载野火IM移动端");
    downloadBtn->setFlat(true); // 禁用默认的3D效果
    downloadBtn->setStyleSheet("color: blue; text-decoration: underline;"); // 可选：设置文本颜色

    layout->addWidget(qrcodeLabel, 0, Qt::AlignHCenter);
    layout->addWidget(titleLabel, 0, Qt::AlignHCenter);
    layout->addWidget(hintLabel, 0, Qt::AlignHCenter);
    layout->addWidget(downloadBtn, 0, Qt::AlignHCenter);

    connect(downloadBtn, &QPushButton::clicked, this, &LoginWidget::onDownloadBtnPressed);
}

void LoginWidget::setupPwdSubWidges() {
    logoLabel = new QLabel;
    logoLabel->setFixedSize(QSize(180, 180));
    logoLabel->setStyleSheet("background-color: gray;");
    logoLabel->setPixmap(QPixmap(":/images/icon.png"));
    logoLabel->setScaledContents(true);

    mobileEdit = new QLineEdit;
    mobileEdit->setFixedWidth(WIDGET_WIDTH);
    mobileEdit->setFixedHeight(36);
    mobileEdit->setStyleSheet("font-size: 14pt;");
    mobileEdit->setPlaceholderText("请输入电话号码");

    QRegularExpression intRegex("^[0-9]+$");
    QRegularExpressionValidator* intValidatorMobile = new QRegularExpressionValidator(intRegex, mobileEdit);
    mobileEdit->setValidator(intValidatorMobile);

    passwordEdit = new QLineEdit;
    passwordEdit->setFixedWidth(WIDGET_WIDTH);
    passwordEdit->setFixedHeight(36);
    passwordEdit->setStyleSheet("font-size: 14pt");
    passwordEdit->setPlaceholderText("请输入密码");
    passwordEdit->setHidden(true);

    codeEdit = new CodeLineEdit;
    codeEdit->setFixedWidth(WIDGET_WIDTH);
    codeEdit->setFixedHeight(36);
    codeEdit->setStyleSheet("font-size: 14pt");
    codeEdit->setPlaceholderText("请输入验证码");
    QRegularExpressionValidator* intValidatorCode= new QRegularExpressionValidator(intRegex, codeEdit);
    codeEdit->setValidator(intValidatorCode);
    codeEdit->enableSendCode(false);
    connect(codeEdit->clearButton, &QToolButton::clicked, this, &LoginWidget::onSendCodePressed);


    connect(mobileEdit, &QLineEdit::textChanged, this, [this](const QString& text) {
        // 定义手机号码的正则表达式
        QRegularExpression phoneRegex("^1[1-9]\\d{9}$");
        this->codeEdit->enableSendCode(phoneRegex.match(text).hasMatch());
    });

    switchPwdCodeBtn = new QPushButton;
    switchPwdCodeBtn->setFlat(true);
    switchPwdCodeBtn->setStyleSheet("color: blue;"); // 可选：设置文本颜色
    switchPwdCodeBtn->setText("使用密码登录");
    switchPwdCodeBtn->setFixedSize(QSize(120, 36));
    connect(switchPwdCodeBtn, &QPushButton::clicked, this, [this]() {
        this->pwdLogin = !this->pwdLogin;
        this->updateLoginType();
    });

    loginBtn = new QPushButton;
    loginBtn->setFixedWidth(WIDGET_WIDTH);
    loginBtn->setFixedHeight(36);
    loginBtn->setText("登录");

    connect(loginBtn, &QPushButton::clicked, this, &LoginWidget::onLoginBtnPressed);

    layout->addWidget(logoLabel, 0, Qt::AlignHCenter);
    layout->addWidget(mobileEdit, 0, Qt::AlignHCenter);
    layout->addWidget(passwordEdit, 0, Qt::AlignHCenter);
    layout->addWidget(codeEdit, 0, Qt::AlignHCenter);
    layout->addWidget(switchPwdCodeBtn, 0, Qt::AlignHCenter);
    layout->addWidget(loginBtn, 0, Qt::AlignHCenter);
}

void LoginWidget::createPCSession(const QString &userId) {
    AppService::sharedAppService()->createPCSession(userId, [this](const QString &userId, const QString &appToken, int status, long expired, int platform, const QString &deviceName){
        QString qrUrl(QR_CODE_PREFIX_PC_SESSION + appToken);
        QImage qrImage = qrcode::generateQr(qrUrl, 250, 1, qrcode::ErrorCorrection::QUARTILE);
        this->qrcodeLabel->setPixmap(QPixmap::fromImage(qrImage));
        loginPCSession(appToken);
    }, [this](int errCode, const QString &message) {
        qrcodeLabel->setText("获取PC token失败，请检查应用服务是否正确！");
    });
}

void LoginWidget::loginPCSession(const QString &appToken) {
    AppService::sharedAppService()->loginWithPCSession(appToken, [this](const QString &userId, const QString &token, bool newUser, const QString &userName, const QString &portrait, const QString &resetCode){
        onLoginSuccess(userId, token, newUser, resetCode);
    }, [this, appToken](int errCode, const QString &message, const QString &userName, const QString &portrait) {
        if(errCode == 9) {
            this->qrcodeLabel->setText("请点击确认登陆");
            loginPCSession(appToken);
        } else if(errCode == 18) {
            createPCSession(nullptr);
        } else {
            createPCSession(nullptr);
        }
    });
}

void LoginWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void LoginWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

void LoginWidget::updateLoginType() {
    if(qrcodeLogin) {
        //qr widgets
        qrcodeLabel->setHidden(false);
        titleLabel->setHidden(false);
        hintLabel->setHidden(false);
        downloadBtn->setHidden(false);

        //password widgets;
        logoLabel->setHidden(true);
        mobileEdit->setHidden(true);
        passwordEdit->setHidden(true);
        // bool pwdLogin;
        codeEdit->setHidden(true);
        switchPwdCodeBtn->setHidden(true);
        loginBtn->setHidden(true);
    } else {
        //qr widgets
        qrcodeLabel->setHidden(true);
        titleLabel->setHidden(true);
        hintLabel->setHidden(true);
        downloadBtn->setHidden(true);

        //password widgets;
        logoLabel->setHidden(false);
        mobileEdit->setHidden(false);
        passwordEdit->setHidden(!pwdLogin);
        // bool pwdLogin;
        codeEdit->setHidden(pwdLogin);
        switchPwdCodeBtn->setHidden(false);
        switchPwdCodeBtn->setText(pwdLogin?"使用验证码登录":"使用密码登录");
        loginBtn->setHidden(false);
    }
}

void LoginWidget::onDownloadBtnPressed() {

}

void LoginWidget::onSwitchBtnPressed() {
    qrcodeLogin = !qrcodeLogin;
    updateLoginType();
}

void LoginWidget::onSendCodePressed() {
    //Send code here and start count down
    codeEdit->startCountDown(60);
}

void LoginWidget::onLoginBtnPressed() {
    QString mobile = mobileEdit->text();
    if (mobile.isEmpty()) {
        showError("请输入手机号");
        return;
    }

    // 禁用按钮防止重复提交
    loginBtn->setEnabled(false);
    loginBtn->setText(pwdLogin ? "密码登录中..." : "验证码登录中...");
    // progressIndicator->startAnimation(); // 添加加载指示器

    // 登录逻辑
    auto successHandler = [this](const QString &userId, const QString &token, bool newUser, const QString &resetCode) {
        QMetaObject::invokeMethod(this, [this, userId, token, newUser, resetCode] {
            resetLoginUI();
            onLoginSuccess(userId, token, newUser, resetCode);
        }, Qt::QueuedConnection);
    };

    auto errorHandler = [this](int errCode, const QString &message) {
        QMetaObject::invokeMethod(this, [this, errCode, message] {
            resetLoginUI();
            onLoginFailed(errCode, message);
        }, Qt::QueuedConnection);
    };


    if(pwdLogin) {
        QString password = passwordEdit->text();
        if (password.isEmpty()) {
            resetLoginUI();
            showError("请输入密码");
            return;
        }
        AppService::sharedAppService()->loginWithMobilePwd(mobile, password, successHandler, errorHandler);

    } else {
        QString code = codeEdit->text();
        if (code.isEmpty()) {
            resetLoginUI();
            showError("请输入验证码");
            return;
        }
        AppService::sharedAppService()->loginWithMobile(mobile, code, successHandler, errorHandler);

    }
}

// 重置登录UI状态的辅助方法
void LoginWidget::resetLoginUI() {
    loginBtn->setEnabled(true);
    loginBtn->setText(pwdLogin ? "密码登录" : "验证码登录");
    // progressIndicator->stopAnimation();
}

// 显示错误信息的辅助方法
void LoginWidget::showError(const QString &message) {
    // 添加错误提示动画
    // QPropertyAnimation *animation = new QPropertyAnimation(errorLabel, "opacity", this);
    // animation->setDuration(3000);
    // animation->setStartValue(1.0);
    // animation->setEndValue(0.0);
    // animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void LoginWidget::onLoginSuccess(const QString &userId, const QString &token, bool newUser, const QString &resetCode) {
    QSettings settings("WfcData", "WFC");
    settings.setValue("wfc/phone_number", mobileEdit->text());
    settings.setValue("wfc/token", token);
    settings.setValue("wfc/user_id", userId);
    settings.sync();

    connect2ImServer(userId, token);
}

void LoginWidget::connect2ImServer(const QString &userId, const QString &token) {
    loginBtn->setEnabled(false);
    loginBtn->setText("连接中...");
    qrcodeLabel->setText("连接中...");
    WFCLib::ChatClient::Instance()->connect(userId.toStdString(), token.toStdString());
}

void LoginWidget::onLoginFailed(int errCode, const QString &message) {

}

void LoginWidget::onConnectionsStatusChanged(WFCLib::ConnectionStatus status) {
    if(status < 0) {
        if(status != WFCLib::kConnectionStatusLogout) {
            WFCLib::ChatClient::Instance()->disconnect();
        }

        loginBtn->setEnabled(true);
        loginBtn->setText(pwdLogin ? "密码登录" : "验证码登录");
        if(isHidden()) {
            show();
        }

        QSettings settings("WfcData", "WFC");
        settings.remove("wfc/token");
        settings.remove("wfc/user_id");
        settings.sync();
    } else if(status == 1) {
        //goto main window
        MainWindow *m = new MainWindow;
        hide();
        m->show();
    }
}

void LoginWidget::onReceiveMessages(const std::list<WFCLib::Message> &messages, bool hasMore) {
    qDebug() << messages.size();
}

LoginWidget::~LoginWidget() {}
