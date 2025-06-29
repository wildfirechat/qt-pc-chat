// contactlistwidget.cpp
#include "contactlistwidget.h"
#include <QVBoxLayout>
#include "contactlistmodel.h"
#include "contactitemdelegate.h"
#include "../../wfc/client/ChatClient.h"


ContactListWidget::ContactListWidget(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

ContactListWidget::~ContactListWidget()
{
}

void ContactListWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    treeView = new QTreeView(this);
    treeView->setHeaderHidden(true);
    treeView->setStyleSheet("border: none;");
    treeView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    treeView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    model = new ContactListModel(this);
    treeView->setModel(model);
    treeView->setItemDelegate(new ContactItemDelegate(this));

    // 设置选择行为
    treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    treeView->setSelectionMode(QAbstractItemView::SingleSelection);

    // 连接点击事件
    connect(treeView, &QTreeView::clicked, this, &ContactListWidget::onItemClicked);

    mainLayout->addWidget(treeView);

    treeView->expand(model->index(4, 0));

    connect(WFCLib::ChatClient::Instance(), &WFCLib::ChatClient::contactUpdated, model, &ContactListModel::loadData);
}

void ContactListWidget::onItemClicked(const QModelIndex &index)
{
    if (index.parent().isValid()) {
        int itemType = index.parent().row();
        QString itemId = model->data(index, Qt::UserRole).toString();
        emit itemSelected(itemType, itemId);
    }
}
