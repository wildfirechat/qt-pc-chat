#ifndef FRIENDSELECTABLEDELEGATE_H
#define FRIENDSELECTABLEDELEGATE_H

#include <QStyledItemDelegate>


class FriendSelectableDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit FriendSelectableDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;


    // QAbstractItemDelegate interface
public:
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // FRIENDSELECTABLEDELEGATE_H
