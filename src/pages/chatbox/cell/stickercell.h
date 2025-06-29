#ifndef STICKERCELL_H
#define STICKERCELL_H

#include "headercell.h"

class StickerCell : public HeaderCell
{
public:
    StickerCell();


    // HeaderCell interface
protected:
    void paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const;
    QSize sizeClientMainHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const;
};

#endif // STICKERCELL_H
