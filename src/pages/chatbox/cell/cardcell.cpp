#include "cardcell.h"

CardCell::CardCell() {}

void CardCell::paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const
{

}

QSize CardCell::sizeClientMainHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const
{
    return QSize(100, 20);
}
