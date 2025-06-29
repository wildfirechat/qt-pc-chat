#ifndef SELECTEDFRIENDDELEGATE_H
#define SELECTEDFRIENDDELEGATE_H

#include <QStyledItemDelegate>

class SelectedFriendDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit SelectedFriendDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;


    // QAbstractItemDelegate interface
public:
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // SELECTEDFRIENDDELEGATE_H
