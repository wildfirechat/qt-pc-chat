#ifndef FAVORITELISTWIDGET_H
#define FAVORITELISTWIDGET_H

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>

class FavoriteListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FavoriteListWidget(QWidget *parent = nullptr);
    ~FavoriteListWidget();

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

#endif // FAVORITELISTWIDGET_H
