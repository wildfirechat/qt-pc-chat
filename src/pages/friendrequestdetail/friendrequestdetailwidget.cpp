#include "friendrequestdetailwidget.h"
#include <QLayout>

FriendRequestDetailWidget::FriendRequestDetailWidget(QWidget *parent)
    : QWidget{parent}
{
    // 创建主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 创建水平布局用于水平居中
    QHBoxLayout *horizontalLayout = new QHBoxLayout();

    // 创建标签
    label = new QLabel("好友请求详情没有实现", this);
    label->setAlignment(Qt::AlignCenter); // 确保文本在标签内居中

    // 将标签添加到水平布局
    horizontalLayout->addWidget(label);

    // 将水平布局添加到垂直布局
    mainLayout->addLayout(horizontalLayout);

    // 设置垂直布局的对齐方式为居中
    mainLayout->setAlignment(Qt::AlignCenter);

    // 设置布局的边距和间距
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(0);

    // 设置此widget使用该布局
    setLayout(mainLayout);
}

void FriendRequestDetailWidget::setUserId(const QString &userId)
{

}
