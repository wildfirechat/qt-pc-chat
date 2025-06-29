#include "emptydetailwidget.h"

EmptyDetailWidget::EmptyDetailWidget(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

EmptyDetailWidget::~EmptyDetailWidget()
{
}

void EmptyDetailWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(20);
    
    // 创建一个居中的空状态提示
    QWidget *centerWidget = new QWidget(this);
    centerWidget->setStyleSheet("background-color: #f9f9f9;");
    QVBoxLayout *centerLayout = new QVBoxLayout(centerWidget);
    centerLayout->setContentsMargins(20, 20, 20, 20);
    centerLayout->setSpacing(15);
    
    QLabel *iconLabel = new QLabel("📄", this);
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setStyleSheet("font-size: 64px; color: #cccccc;");
    
    QLabel *titleLabel = new QLabel("未选择项目", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #666666;");
    
    QLabel *subtitleLabel = new QLabel("请从左侧列表中选择一个项目查看详情", this);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet("color: #999999;");
    
    centerLayout->addStretch();
    centerLayout->addWidget(iconLabel);
    centerLayout->addWidget(titleLabel);
    centerLayout->addWidget(subtitleLabel);
    centerLayout->addStretch();
    
    mainLayout->addWidget(centerWidget);
}    