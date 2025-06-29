#ifndef TEXTCELL_H
#define TEXTCELL_H

#include "headercell.h"

class TextCell : public HeaderCell
{
public:
    TextCell();

    // HeaderCell interface
protected:
    void paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const;
    QSize sizeClientMainHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const;
};

#endif // TEXTCELL_H
