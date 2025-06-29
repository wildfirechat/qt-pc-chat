#include "videocell.h"

VideoCell::VideoCell() {}

void VideoCell::paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const
{

}

QSize VideoCell::sizeClientMainHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const
{
    return QSize(100, 20);
}
