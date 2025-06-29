
// basecell.cpp
#include "basecell.h"
#include "../../tools/timeformatter.h"
#include <QPainter>

void BaseCell::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, const MessageData &message, QMap<int, QRect> *itemAreas) const
{
    painter->save();
    paintMessageTime(painter, option, message);
    QSize timeSize = sizeMessageTimeHint(option, message);
    paintContentArea(painter, option, timeSize.height(), message, itemAreas);
    painter->restore();
}

QSize BaseCell::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index, const MessageData &message) const
{
    QSize timeSize = sizeMessageTimeHint(option, message);
    QSize contentSize = sizeContentHint(option, message);
    return QSize(qMax(timeSize.width(), contentSize.width()), timeSize.height() + contentSize.height() + 8);
}

void BaseCell::paintMessageTime(QPainter *painter, const QStyleOptionViewItem &option, const MessageData &message) const
{
    if(message.showTime) {
        QPen oldPen = painter->pen();
        QPen newPen = oldPen;
        newPen.setColor(QColor(145, 145, 145));
        painter->setPen(newPen);
        QString timeStr = TimeFormatter::formatDetailTime(message.message.timestamp);
        QRect timeRect(option.rect.x(), option.rect.y(), option.rect.width(), 20);
        painter->drawText(timeRect, Qt::AlignCenter, timeStr);
        painter->setPen(oldPen);
    }
}

QSize BaseCell::sizeMessageTimeHint(const QStyleOptionViewItem &option, const MessageData &message) const
{
    if(message.showTime) {
        return QSize(option.rect.width(), 24);
    } else {
        return QSize(0, 4);
    }
}
