#ifndef ATMEMBERDELEGATE_H
#define ATMEMBERDELEGATE_H

#include <QStyledItemDelegate>



class AtMemberDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit AtMemberDelegate(QObject *parent = nullptr);

    // 绘制列表项
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    // 设置列表项大小
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
};


#endif // ATMEMBERDELEGATE_H
