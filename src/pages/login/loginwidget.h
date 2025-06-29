#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLayout>
#include <QLineEdit>
#include <QMouseEvent>
#include "codelineedit.h"
#include "../../wfc/client/ChatClient.h"

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

private slots:
    void onDownloadBtnPressed();
    void onSwitchBtnPressed();
    void onSendCodePressed();
    void onLoginBtnPressed();

    void onConnectionsStatusChanged(WFCLib::ConnectionStatus status);

    void onReceiveMessages(const std::list<WFCLib::Message> &messages, bool hasMore);

private:
    void setupQrSubWidges();
    void setupPwdSubWidges();
    void updateLoginType();
    bool qrcodeLogin;

    int windowWidth;

    QPoint m_dragPosition;

    //qr widgets
    QLabel *qrcodeLabel;
    QLabel *titleLabel;
    QLabel *hintLabel;
    QPushButton *downloadBtn;

    //password widgets;
    QLabel *logoLabel;
    QLineEdit *mobileEdit;
    QLineEdit *passwordEdit;
    bool pwdLogin;
    CodeLineEdit *codeEdit;
    QPushButton *switchPwdCodeBtn;
    QPushButton *loginBtn;

    //common
    QPushButton *switchBtn;
    QVBoxLayout *layout;
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void onLoginSuccess(const QString &userId, const QString &token, bool newUser, const QString &resetCode);
    void onLoginFailed(int errCode, const QString &message);

    void resetLoginUI();
    void showError(const QString &message);

    void createPCSession(const QString &userId);
    void loginPCSession(const QString &appToken);

    void connect2ImServer(const QString &userId, const QString &token);
};
#endif // LOGINWIDGET_H
