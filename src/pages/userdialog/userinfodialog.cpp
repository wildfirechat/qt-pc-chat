#include "userinfodialog.h"
#include <QLayout>
#include "../../wfc/client/ChatClient.h"
#include "../utility/avatarwidget.h"
#include "../utility/defaultportraitfactory.h"

UserInfoDialog::UserInfoDialog(QWidget *parent) : QDialog(parent) {
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    // 1. 头像及名称部分
    QHBoxLayout *topLayout = new QHBoxLayout;

    avatarLabel = new AvatarWidget(60, this);
    avatarLabel->setRadius(6);

    QVBoxLayout *nameInfoLayout = new QVBoxLayout;
    nameLabel = new QLabel(this);
    nameLabel->setStyleSheet("font-weight: bold;");
    wfIdLabel = new QLabel(this);

    nameInfoLayout->addWidget(nameLabel);
    nameInfoLayout->addWidget(wfIdLabel);

    /*
    regionLabel = new QLabel("地区: 广东 深圳", this);
    nameInfoLayout->addWidget(regionLabel);
    */

    topLayout->addWidget(avatarLabel);
    topLayout->addLayout(nameInfoLayout);

    // 2. 备注部分
    QHBoxLayout *remarkLayout = new QHBoxLayout;
    remarkLabel = new QLabel("备注", this);
    remarkEdit = new QLineEdit(this);
    remarkEdit->setPlaceholderText("添加备注名");
    remarkLayout->addWidget(remarkLabel);
    remarkLayout->addWidget(remarkEdit);


    // 5. 功能按钮部分
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    messageButton = new QPushButton("发消息", this);
    buttonLayout->addWidget(messageButton);

    voiceCallButton = new QPushButton("语音聊天", this);
    videoCallButton = new QPushButton("视频聊天", this);
    buttonLayout->addWidget(voiceCallButton);
    buttonLayout->addWidget(videoCallButton);

    // 整体布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(remarkLayout);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    connect(messageButton, &QPushButton::clicked, [this]() {
        hide();
        emit chatToUser(currentUserId);
    });
    connect(voiceCallButton, &QPushButton::clicked, [this]() {
        hide();
        emit callToUser(currentUserId, true);
    });
    connect(videoCallButton, &QPushButton::clicked, [this]() {
        hide();
        emit callToUser(currentUserId, false);
    });
}

void UserInfoDialog::setUserAndPosition(const QString &userId, const QPoint &position)
{
    this->move(position);
    currentUserId = userId;

    // 设置窗口大小
    setFixedWidth(240);

    WFCLib::UserInfo userInfo = WFCLib::ChatClient::Instance()->getUserInfo(userId.toStdString());
    QString name = userInfo.getReadableName().c_str();


    if(userInfo.portrait.empty()) {
        avatarLabel->setDefaultImage(DefaultPortraitFactory::defaultPortraitOfUser(name, 80));
    }
    avatarLabel->setUrl(userInfo.portrait.c_str());


    nameLabel->setText(userInfo.displayName.c_str());
    QString nameStr = "野火号: ";
    nameStr += userInfo.name.c_str();
    wfIdLabel->setText(nameStr);

    if(!userInfo.friendAlias.empty()) {
        remarkEdit->setText(userInfo.friendAlias.c_str());
    }

    if(userId.toStdString() != WFCLib::ChatClient::Instance()->getCurrentUserId()) {
        voiceCallButton->setHidden(false);
        videoCallButton->setHidden(false);
    } else {
        voiceCallButton->setHidden(true);
        videoCallButton->setHidden(true);
    }

    show();
}

void UserInfoDialog::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    hide();
}
