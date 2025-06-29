#include "stickercell.h"

StickerCell::StickerCell() {}

void StickerCell::paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const
{

}

QSize StickerCell::sizeClientMainHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const
{
    return QSize(100, 20);
}
