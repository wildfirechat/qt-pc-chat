// contactlistwidget.h
#ifndef CONTACTLISTWIDGET_H
#define CONTACTLISTWIDGET_H

#include <QWidget>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStyledItemDelegate>

enum ContactType {
    ContactType_NEW_FRIEND, //"新朋友",
    ContactType_GROUP, //"群聊",
    ContactType_CHANNEL, //"频道",
    ContactType_ORG, //"组织架构",
    ContactType_FRIEND //"联系人"
};

class ContactListModel;
class ContactListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ContactListWidget(QWidget *parent = nullptr);
    ~ContactListWidget();

signals:
    void itemSelected(int type, const QString &itemId);

private slots:
    void onItemClicked(const QModelIndex &index);

private:
    void setupUI();

    QTreeView *treeView;
    ContactListModel *model;
};

#endif // CONTACTLISTWIDGET_H
