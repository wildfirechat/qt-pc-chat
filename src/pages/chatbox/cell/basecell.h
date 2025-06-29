#ifndef BASECELL_H
#define BASECELL_H

#include <QObject>
#include <QPainter>
#include <QStyleOptionViewItem>
#include "../messagedata.h"

namespace WFCLib {
    class Message;
}; // namespace WFCLib

class BaseCell
{
public:
    BaseCell() {}
    virtual ~BaseCell() {}
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index, const MessageData &message, QMap<int, QRect> *itemAreas) const;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index, const MessageData &message) const;

protected:
    virtual void paintContentArea(QPainter *painter, const QStyleOptionViewItem &option, int vOffset, const MessageData &message, QMap<int, QRect> *itemAreas) const = 0;
    virtual QSize sizeContentHint(const QStyleOptionViewItem &option, const MessageData &message) const = 0;

private:
    void paintMessageTime(QPainter *painter, const QStyleOptionViewItem &option, const MessageData &message) const;
    QSize sizeMessageTimeHint(const QStyleOptionViewItem &option, const MessageData &message) const;
};

#endif // BASECELL_H
