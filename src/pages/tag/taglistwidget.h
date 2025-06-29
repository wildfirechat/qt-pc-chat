#ifndef TAGLISTWIDGET_H
#define TAGLISTWIDGET_H

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>

class TagListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TagListWidget(QWidget *parent = nullptr);
    ~TagListWidget();

signals:
    void itemSelected(const QString &itemId);

private slots:
    void onItemClicked(const QModelIndex &index);

private:
    void setupUI();
    void initData();

    QListView *listView;
    QStandardItemModel *model;
};

#endif // TAGLISTWIDGET_H
