#include "taglistwidget.h"
#include <QVBoxLayout>

TagListWidget::TagListWidget(QWidget *parent) : QWidget(parent)
{
    setupUI();
    initData();
}

TagListWidget::~TagListWidget()
{
}

void TagListWidget::setupUI()
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
    connect(listView, &QListView::clicked, this, &TagListWidget::onItemClicked);

    mainLayout->addWidget(listView);
}

void TagListWidget::initData()
{
    // 添加示例标签数据
    QStringList ids = {"tag1", "tag2", "tag3", "tag4", "tag5"};
    QStringList tags = {"工作", "学习", "生活", "重要", "待办"};
    QStringList counts = {"12", "8", "5", "7", "3"};

    for (int i = 0; i < 5; ++i) {
        QString displayText = QString("%1 (%2)").arg(tags[i]).arg(counts[i]);
        QStandardItem *item = new QStandardItem(displayText);
        item->setData(ids[i], Qt::UserRole);
        model->appendRow(item);
    }
}

void TagListWidget::onItemClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        QString itemId = model->data(index, Qt::UserRole).toString();
        emit itemSelected(itemId);
    }
}
