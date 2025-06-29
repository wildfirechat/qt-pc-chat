#include "linkcell.h"

LinkCell::LinkCell() {}

void LinkCell::paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const
{

}

QSize LinkCell::sizeClientMainHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const
{
    return QSize(100, 20);
}
