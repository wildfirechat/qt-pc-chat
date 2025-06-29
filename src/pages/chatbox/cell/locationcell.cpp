#include "locationcell.h"

LocationCell::LocationCell() {}

void LocationCell::paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const
{

}

QSize LocationCell::sizeClientMainHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const
{
    return QSize(100, 20);
}
