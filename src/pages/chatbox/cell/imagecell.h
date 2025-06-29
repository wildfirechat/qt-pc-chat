#ifndef IMAGECELL_H
#define IMAGECELL_H

#include "headercell.h"

class ImageCell : public HeaderCell
{
public:
    ImageCell();

    // HeaderCell interface
protected:
    void paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const;
    QSize sizeClientMainHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const;
};

#endif // IMAGECELL_H
