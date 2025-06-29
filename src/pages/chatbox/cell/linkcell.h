#ifndef LINKCELL_H
#define LINKCELL_H

#include "headercell.h"

class LinkCell : public HeaderCell
{
public:
    LinkCell();


    // HeaderCell interface
protected:
    void paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const;
    QSize sizeClientMainHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const;
};

#endif // LINKCELL_H
