#include "richnotificationcell.h"

RichNotificationCell::RichNotificationCell() {}

void RichNotificationCell::paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const
{

}

QSize RichNotificationCell::sizeClientMainHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const
{
    return QSize(100, 20);
}
