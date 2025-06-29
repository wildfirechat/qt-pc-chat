#ifndef CHATDELEGATE_H
#define CHATDELEGATE_H

#include <QStyledItemDelegate>
#include "cell/basecell.h"
#include "cell/unknowncell.h"



namespace WFCLib {
    class Message;
}; // namespace WFCLib

class ChatDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    explicit ChatDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option, const QModelIndex &index) override;

signals:
    void avatarRightClicked(const QModelIndex &index);
    void messageContentClicked(const QModelIndex &index);

private:
    const BaseCell *cellForMessage(const WFCLib::MessageContent *content) const;
    void registerAllCell();
    QMap<int, BaseCell*> cellMaps;
    const UnknownCell unknownCell;
private:
    static QHash<QModelIndex, QMap<int, QRect>*> itemAreasMap; // 用于存储每个项的区域信息
};
#endif // CHATDELEGATE_H
