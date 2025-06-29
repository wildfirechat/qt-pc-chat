#ifndef CONTACTDETAILWIDGET_H
#define CONTACTDETAILWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include "../../wfc/client/ChatClient.h"


class AvatarWidget;
class ContactDetailWidget : public QWidget, public WFCLib::GeneralVoidCallback
{
    Q_OBJECT
public:
    explicit ContactDetailWidget(QWidget *parent = nullptr);
    ~ContactDetailWidget();
    
    void setContactId(const QString &contactId);
    bool hasContent();

signals:
    void chatToUser(const QString &userId);
    void callToUser(const QString &userId, bool isAudio);

private slots:
    void onSendMessage();
    void onVideoCall();
    void onDeleteContact();

private:
    void setupUI();
    void loadContactInfo(const QString &contactId);
    
    QString currentContactId;
    AvatarWidget *avatarLabel;
    QLabel *nameLabel;
    QLabel *statusLabel;
    QLabel *phoneLabel;
    QLabel *emailLabel;
    QLabel *signatureLabel;

    // GeneralVoidCallback interface
public:
    void onSuccess(int dataType);
    void onFailure(int dataType, int errorCode);
};

#endif // CONTACTDETAILWIDGET_H    
