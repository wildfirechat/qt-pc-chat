#include "favoritelistwidget.h"
#include <QVBoxLayout>

FavoriteListWidget::FavoriteListWidget(QWidget *parent) : QWidget(parent)
{
    setupUI();
    initData();
}

FavoriteListWidget::~FavoriteListWidget()
{
}

void FavoriteListWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    listView = new QListView(this);
    model = new QStandardItemModel(0, 1, this);

    listView->setModel(model);

    // 设置选择行为
    listView->setSelectionBehavior(QAbstractItemView::SelectRows);
    listView->setSelectionMode(QAbstractItemView::SingleSelection);

    // 连接点击事件
    connect(listView, &QListView::clicked, this, &FavoriteListWidget::onItemClicked);

    mainLayout->addWidget(listView);
}

void FavoriteListWidget::initData()
{
    // 添加示例收藏数据
    QStringList ids = {"file1", "file2", "image1", "link1", "text1"};
    QStringList types = {"文件", "文件", "图片", "链接", "文本"};
    QStringList names = {"项目需求文档.docx", "设计原型图.fig", "会议照片.jpg", "重要网址", "会议记录"};
    QStringList times = {"09:30", "昨天", "上周", "2023-06-10", "2023-06-12"};
    QStringList sizes = {"2.4MB", "5.8MB", "1.2MB", "-", "12KB"};

    for (int i = 0; i < 5; ++i) {
        QString displayText = QString("%1: %2 (%3, %4)").arg(types[i]).arg(names[i]).arg(times[i]).arg(sizes[i]);
        QStandardItem *item = new QStandardItem(displayText);
        item->setData(ids[i], Qt::UserRole);
        model->appendRow(item);
    }
}

void FavoriteListWidget::onItemClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        QString itemId = model->data(index, Qt::UserRole).toString();
        emit itemSelected(itemId);
    }
}
