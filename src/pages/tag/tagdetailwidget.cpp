#include "tagdetailwidget.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMessageBox>

TagDetailWidget::TagDetailWidget(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

TagDetailWidget::~TagDetailWidget()
{
}

void TagDetailWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);
    
    // 标签信息
    QWidget *tagInfoWidget = new QWidget(this);
    QHBoxLayout *tagInfoLayout = new QHBoxLayout(tagInfoWidget);
    tagInfoLayout->setContentsMargins(0, 0, 0, 0);
    tagInfoLayout->setSpacing(10);
    
    tagNameLabel = new QLabel("标签名称", this);
    tagNameLabel->setStyleSheet("font-size: 20px; font-weight: bold;");
    
    tagCountLabel = new QLabel("(12个项目)", this);
    tagCountLabel->setStyleSheet("color: #888888;");
    
    tagInfoLayout->addWidget(tagNameLabel);
    tagInfoLayout->addWidget(tagCountLabel);
    tagInfoLayout->addStretch();
    
    // 标签项目列表
    itemsListWidget = new QListWidget(this);
    itemsListWidget->setStyleSheet("QListWidget { border: 1px solid #e0e0e0; border-radius: 4px; }"
                                  "QListWidget::item { padding: 10px; border-bottom: 1px solid #e0e0e0; }"
                                  "QListWidget::item:last-child { border-bottom: none; }");
    
    // 添加到主布局
    mainLayout->addWidget(tagInfoWidget);
    mainLayout->addWidget(itemsListWidget);
    
    // 连接信号和槽
    connect(itemsListWidget, &QListWidget::itemClicked, [this](QListWidgetItem *item) {
        QString itemId = item->data(Qt::UserRole).toString();
        // 根据itemId显示对应的详情
        emit itemSelected(3, itemId);
    });
}

void TagDetailWidget::setTagId(const QString &tagId)
{
    currentTagId = tagId;
    loadTagItems(tagId);
}

void TagDetailWidget::loadTagItems(const QString &tagId)
{
    // 清空现有项目
    itemsListWidget->clear();
    
    // 根据标签ID加载相关项目
    QString tagName;
    QStringList itemNames, itemIds, itemTypes;
    
    if (tagId == "tag1") {
        tagName = "工作";
        itemNames += {"项目需求文档", "设计原型图", "会议记录", "任务分配表", "进度报告"};
        itemIds += {"file1", "file2", "text1", "file3", "file4"};
        itemTypes += {"文件", "文件", "文本", "文件", "文件"};
    } else if (tagId == "tag2") {
        tagName = "学习";
        itemNames += {"技术文档", "教程视频", "参考书籍", "学习笔记", "编程练习"};
        itemIds += {"file5", "video1", "book1", "text2", "code1"};
        itemTypes += {"文件", "视频", "文件", "文本", "代码"};
    } else if (tagId == "tag3") {
        tagName = "生活";
        itemNames += {"旅行照片", "食谱收藏", "健身计划", "电影推荐", "音乐列表"};
        itemIds += {"image1", "text3", "text4", "list1", "list2"};
        itemTypes += {"图片", "文本", "文本", "列表", "列表"};
    } else if (tagId == "tag4") {
        tagName = "重要";
        itemNames += {"合同文件", "密码管理", "紧急联系人", "医疗信息", "保险单"};
        itemIds += {"file6", "text5", "contact1", "text6", "file7"};
        itemTypes += {"文件", "文本", "联系人", "文本", "文件"};
    } else if (tagId == "tag5") {
        tagName = "待办";
        itemNames += {"提交报告", "回复邮件", "准备演示", "购买机票", "预约医生"};
        itemIds += {"task1", "task2", "task3", "task4", "task5"};
        itemTypes += {"任务", "任务", "任务", "任务", "任务"};
    }
    
    // 更新标签信息
    tagNameLabel->setText(tagName);
    tagCountLabel->setText(QString("(%1个项目)").arg(itemNames.size()));
    
    // 添加项目到列表
    for (int i = 0; i < itemNames.size(); ++i) {
        QListWidgetItem *item = new QListWidgetItem(itemsListWidget);
        
        QWidget *itemWidget = new QWidget(itemsListWidget);
        QHBoxLayout *itemLayout = new QHBoxLayout(itemWidget);
        itemLayout->setContentsMargins(0, 0, 0, 0);
        itemLayout->setSpacing(10);
        
        QLabel *typeLabel = new QLabel(itemTypes[i], itemWidget);
        typeLabel->setFixedWidth(60);
        typeLabel->setAlignment(Qt::AlignCenter);
        typeLabel->setStyleSheet("background-color: #f0f0f0; border-radius: 4px; padding: 2px;");
        
        QLabel *nameLabel = new QLabel(itemNames[i], itemWidget);
        
        itemLayout->addWidget(typeLabel);
        itemLayout->addWidget(nameLabel);
        itemLayout->addStretch();
        
        itemWidget->setLayout(itemLayout);
        itemWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        itemWidget->adjustSize();
        
        item->setSizeHint(itemWidget->size());
        item->setData(Qt::UserRole, itemIds[i]);
        itemsListWidget->setItemWidget(item, itemWidget);
    }
}    
