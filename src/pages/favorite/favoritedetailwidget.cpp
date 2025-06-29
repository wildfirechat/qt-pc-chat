#include "favoritedetailwidget.h"
#include <QFileInfo>
#include <QDateTime>
#include <QMessageBox>

FavoriteDetailWidget::FavoriteDetailWidget(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

FavoriteDetailWidget::~FavoriteDetailWidget()
{
}

void FavoriteDetailWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);
    
    // 收藏信息
    QWidget *infoWidget = new QWidget(this);
    QGridLayout *infoLayout = new QGridLayout(infoWidget);
    infoLayout->setContentsMargins(0, 0, 0, 0);
    infoLayout->setSpacing(10);
    
    infoLayout->addWidget(new QLabel("类型:", this), 0, 0);
    typeLabel = new QLabel("文件", this);
    infoLayout->addWidget(typeLabel, 0, 1);
    
    infoLayout->addWidget(new QLabel("名称:", this), 1, 0);
    nameLabel = new QLabel("文件名", this);
    nameLabel->setWordWrap(true);
    infoLayout->addWidget(nameLabel, 1, 1);
    
    infoLayout->addWidget(new QLabel("时间:", this), 2, 0);
    timeLabel = new QLabel("2023-06-10 10:30", this);
    infoLayout->addWidget(timeLabel, 2, 1);
    
    infoLayout->addWidget(new QLabel("大小:", this), 3, 0);
    sizeLabel = new QLabel("2.4MB", this);
    infoLayout->addWidget(sizeLabel, 3, 1);
    
    // 预览区域
    previewLabel = new QLabel("预览", this);
    previewLabel->setAlignment(Qt::AlignCenter);
    previewLabel->setStyleSheet("border: 1px solid #e0e0e0; border-radius: 4px; min-height: 200px;");
    
    // 内容区域（用于文本类收藏）
    contentEdit = new QTextEdit(this);
    contentEdit->setReadOnly(true);
    contentEdit->setStyleSheet("border: 1px solid #e0e0e0; border-radius: 4px;");
    
    // 操作按钮
    QHBoxLayout *actionLayout = new QHBoxLayout();
    actionLayout->setSpacing(15);
    
    QPushButton *openBtn = new QPushButton("打开", this);
    openBtn->setFixedHeight(40);
    openBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; border-radius: 4px; }"
                           "QPushButton:hover { background-color: #45a049; }");
    
    QPushButton *removeBtn = new QPushButton("移除收藏", this);
    removeBtn->setFixedHeight(40);
    removeBtn->setStyleSheet("QPushButton { color: red; border: 1px solid #e0e0e0; border-radius: 4px; }"
                             "QPushButton:hover { background-color: #ffebee; }");
    
    actionLayout->addStretch();
    actionLayout->addWidget(openBtn);
    actionLayout->addWidget(removeBtn);
    
    // 添加到主布局
    mainLayout->addWidget(infoWidget);
    mainLayout->addWidget(previewLabel);
    mainLayout->addWidget(contentEdit);
    mainLayout->addLayout(actionLayout);
    
    // 连接信号和槽
    connect(openBtn, &QPushButton::clicked, this, &FavoriteDetailWidget::onOpenFile);
    connect(removeBtn, &QPushButton::clicked, this, &FavoriteDetailWidget::onRemoveFavorite);
}

void FavoriteDetailWidget::setFavoriteId(const QString &favoriteId)
{
    currentFavoriteId = favoriteId;
    loadFavoriteInfo(favoriteId);
}

void FavoriteDetailWidget::loadFavoriteInfo(const QString &favoriteId)
{
    // 根据ID加载收藏信息
    QString type, name, time, size, content;
    filePath = "";
    
    if (favoriteId == "file1") {
        type = "文件";
        name = "项目需求文档.docx";
        time = "2023-06-10 09:30";
        size = "2.4MB";
        filePath = "C:/Documents/项目需求文档.docx";
        content = "";
    } else if (favoriteId == "file2") {
        type = "文件";
        name = "设计原型图.fig";
        time = "2023-06-09 16:45";
        size = "5.8MB";
        filePath = "C:/Documents/设计原型图.fig";
        content = "";
    } else if (favoriteId == "image1") {
        type = "图片";
        name = "会议照片.jpg";
        time = "2023-06-05 14:20";
        size = "1.2MB";
        filePath = ":/icons/avatar1.png"; // 使用头像作为示例图片
        content = "";
    } else if (favoriteId == "link1") {
        type = "链接";
        name = "重要网址";
        time = "2023-06-03 11:15";
        size = "-";
        filePath = "https://www.example.com";
        content = "这是一个重要的参考网站，包含了项目所需的各种资源和文档。";
    } else if (favoriteId == "text1") {
        type = "文本";
        name = "会议记录";
        time = "2023-06-02 15:30";
        size = "12KB";
        content = "会议主题：项目进度讨论\n\n参会人员：张三、李四、王五\n\n会议内容：\n1. 张三汇报了当前项目的进展情况，已完成80%。\n2. 李四提出了几个优化建议，大家讨论后决定采纳。\n3. 王五提醒了项目截止日期，要求大家加快进度。\n\n下一步计划：\n1. 张三继续完成剩余的20%。\n2. 李四负责优化方案的实施。\n3. 周五再次开会讨论最终结果。";
    }
    
    // 更新UI
    typeLabel->setText(type);
    nameLabel->setText(name);
    timeLabel->setText(time);
    sizeLabel->setText(size);
    contentEdit->setText(content);
    
    // 设置预览
    if (type == "图片") {
        previewLabel->setPixmap(QPixmap(filePath).scaled(previewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        previewLabel->show();
        contentEdit->hide();
    } else if (type == "链接") {
        previewLabel->setText("点击下方按钮打开链接：\n" + filePath);
        previewLabel->show();
        contentEdit->hide();
    } else if (type == "文本") {
        previewLabel->hide();
        contentEdit->show();
    } else {
        previewLabel->setText("文件预览：" + name);
        previewLabel->show();
        contentEdit->hide();
    }
}

void FavoriteDetailWidget::onOpenFile()
{
    if (filePath.isEmpty()) {
        QMessageBox::information(this, "提示", "无法打开该收藏项");
        return;
    }
    
    // 打开文件或链接
    if (filePath.startsWith("http")) {
        QDesktopServices::openUrl(QUrl(filePath));
    } else {
        // 实际应用中应该打开真实的文件路径
        QMessageBox::information(this, "文件打开", "即将打开文件：" + nameLabel->text());
    }
}

void FavoriteDetailWidget::onRemoveFavorite()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认移除", "确定要移除收藏 " + nameLabel->text() + " 吗？",
                                 QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        QMessageBox::information(this, "移除成功", "收藏 " + nameLabel->text() + " 已被移除");
        // 这里应该有实际移除收藏的逻辑
    }
}    