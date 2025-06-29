#include "contactdetailwidget.h"
#include <QMessageBox>
#include "../../wfc/client/ChatClient.h"
#include "../utility/defaultportraitfactory.h"
#include "../utility/avatarwidget.h"

#define DELETE_FRIEND_CALLBACK_TYPE 1
#define SET_ALIAS_CALLBACK_TYPE 2
#define BLOCK_CALLBACK_TYPE 3

ContactDetailWidget::ContactDetailWidget(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

ContactDetailWidget::~ContactDetailWidget()
{
}

void ContactDetailWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);
    
    // 头像和基本信息
    QWidget *headerWidget = new QWidget(this);
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(20);
    
    avatarLabel = new AvatarWidget(80, this);
    avatarLabel->setRadius(6);
    
    QVBoxLayout *infoLayout = new QVBoxLayout();
    infoLayout->setContentsMargins(0, 0, 0, 0);
    infoLayout->setSpacing(5);
    
    nameLabel = new QLabel("联系人姓名", this);
    nameLabel->setStyleSheet("font-size: 20px; font-weight: bold;");
    
    statusLabel = new QLabel("在线", this);
    statusLabel->setStyleSheet("color: #4CAF50;");
    
    infoLayout->addWidget(nameLabel);
    infoLayout->addWidget(statusLabel);
    infoLayout->addStretch();
    
    headerLayout->addWidget(avatarLabel);
    headerLayout->addLayout(infoLayout);
    headerLayout->addStretch();
    
    // 操作按钮
    QHBoxLayout *actionLayout = new QHBoxLayout();
    actionLayout->setSpacing(15);
    
    QPushButton *messageBtn = new QPushButton("发消息", this);
    messageBtn->setIcon(QIcon(":/icons/message.png"));
    messageBtn->setFixedHeight(40);
    messageBtn->setStyleSheet("QPushButton { border: 1px solid #e0e0e0; border-radius: 4px; }"
                             "QPushButton:hover { background-color: #e9e9e9; }");
    
    QPushButton *videoBtn = new QPushButton("视频通话", this);
    videoBtn->setIcon(QIcon(":/icons/video.png"));
    videoBtn->setFixedHeight(40);
    videoBtn->setStyleSheet("QPushButton { border: 1px solid #e0e0e0; border-radius: 4px; }"
                            "QPushButton:hover { background-color: #e9e9e9; }");
    
    actionLayout->addWidget(messageBtn);
    actionLayout->addWidget(videoBtn);
    actionLayout->addStretch();
    
    // 联系人信息
    QGroupBox *infoGroup = new QGroupBox("联系人信息", this);
    QVBoxLayout *infoGroupLayout = new QVBoxLayout(infoGroup);
    infoGroupLayout->setContentsMargins(15, 15, 15, 15);
    infoGroupLayout->setSpacing(15);
    
    QHBoxLayout *phoneLayout = new QHBoxLayout();
    QLabel *phoneIconLabel = new QLabel("📞", this);
    phoneIconLabel->setFixedWidth(20);
    phoneLabel = new QLabel("电话号码", this);
    
    phoneLayout->addWidget(phoneIconLabel);
    phoneLayout->addWidget(phoneLabel);
    phoneLayout->addStretch();
    
    QHBoxLayout *emailLayout = new QHBoxLayout();
    QLabel *emailIconLabel = new QLabel("📧", this);
    emailIconLabel->setFixedWidth(20);
    emailLabel = new QLabel("邮箱地址", this);
    
    emailLayout->addWidget(emailIconLabel);
    emailLayout->addWidget(emailLabel);
    emailLayout->addStretch();
    
    QHBoxLayout *signatureLayout = new QHBoxLayout();
    QLabel *signatureIconLabel = new QLabel("✒️", this);
    signatureIconLabel->setFixedWidth(20);
    signatureLabel = new QLabel("个性签名", this);
    
    signatureLayout->addWidget(signatureIconLabel);
    signatureLayout->addWidget(signatureLabel);
    signatureLayout->addStretch();
    
    infoGroupLayout->addLayout(phoneLayout);
    infoGroupLayout->addLayout(emailLayout);
    infoGroupLayout->addLayout(signatureLayout);
    
    // 底部操作
    QPushButton *deleteBtn = new QPushButton("删除联系人", this);
    deleteBtn->setFixedHeight(40);
    deleteBtn->setStyleSheet("QPushButton { color: red; border: 1px solid #e0e0e0; border-radius: 4px; }"
                            "QPushButton:hover { background-color: #ffebee; }");
    
    // 添加到主布局
    mainLayout->addWidget(headerWidget);
    mainLayout->addLayout(actionLayout);
    mainLayout->addWidget(infoGroup);
    mainLayout->addStretch();
    mainLayout->addWidget(deleteBtn);
    
    // 连接信号和槽
    connect(messageBtn, &QPushButton::clicked, this, &ContactDetailWidget::onSendMessage);
    connect(videoBtn, &QPushButton::clicked, this, &ContactDetailWidget::onVideoCall);
    connect(deleteBtn, &QPushButton::clicked, this, &ContactDetailWidget::onDeleteContact);
}

void ContactDetailWidget::setContactId(const QString &contactId)
{
    currentContactId = contactId;
    loadContactInfo(contactId);
}

bool ContactDetailWidget::hasContent()
{
    return !currentContactId.isEmpty();
}

void ContactDetailWidget::loadContactInfo(const QString &contactId)
{
    // 根据ID加载联系人信息
    QString name, phone, email, signature, status;
    int avatarIndex = 1;
    
    auto userInfo = WFCLib::ChatClient::Instance()->getUserInfo(contactId.toStdString(), true);
    name = userInfo.getReadableName().c_str();

    if(userInfo.portrait.empty()) {
        avatarLabel->setDefaultImage(DefaultPortraitFactory::defaultPortraitOfUser(name, 80));
    }
    avatarLabel->setUrl(userInfo.portrait.c_str());

    phone = "13900139002";
    email = "lisi@example.com";
    signature = "人生在勤，不索何获";
    status = "忙碌";
    avatarIndex = 2;
    
    // 更新UI
    nameLabel->setText(name);
    phoneLabel->setText(phone);
    emailLabel->setText(email);
    signatureLabel->setText(signature);
    
    // 设置状态颜色
    if (status == "在线") {
        statusLabel->setText("● " + status);
        statusLabel->setStyleSheet("color: #4CAF50;");
    } else if (status == "忙碌") {
        statusLabel->setText("● " + status);
        statusLabel->setStyleSheet("color: #F44336;");
    } else {
        statusLabel->setText("● " + status);
        statusLabel->setStyleSheet("color: #9E9E9E;");
    }

}

void ContactDetailWidget::onSuccess(int dataType)
{
    if(dataType == DELETE_FRIEND_CALLBACK_TYPE) {
        QMessageBox::information(this, "删除成功", "联系人 " + nameLabel->text() + " 已被删除");
    } else if(dataType == SET_ALIAS_CALLBACK_TYPE) {

    } else if(dataType == BLOCK_CALLBACK_TYPE) {

    }
}

void ContactDetailWidget::onFailure(int dataType, int errorCode)
{
    if(dataType == DELETE_FRIEND_CALLBACK_TYPE) {
        QMessageBox::information(this, "删除失败", "联系人 " + nameLabel->text() + " 删除失败:" + QString::number(errorCode));
    } else if(dataType == SET_ALIAS_CALLBACK_TYPE) {

    } else if(dataType == BLOCK_CALLBACK_TYPE) {

    }
}

void ContactDetailWidget::onSendMessage()
{
    emit chatToUser(currentContactId);
}

void ContactDetailWidget::onVideoCall()
{
    QMessageBox::information(this, "视频通话", "即将发起与 " + nameLabel->text() + " 的视频通话");
}

void ContactDetailWidget::onDeleteContact()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除", "确定要删除联系人 " + nameLabel->text() + " 吗？",
                                 QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        WFCLib::ChatClient::Instance()->deleteFriend(currentContactId.toStdString(), this, DELETE_FRIEND_CALLBACK_TYPE);
    }
}    
