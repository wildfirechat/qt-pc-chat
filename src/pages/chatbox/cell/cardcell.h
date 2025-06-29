#ifndef CARDCELL_H
#define CARDCELL_H

#include "headercell.h"

class CardCell : public HeaderCell
{
public:
    CardCell();


    // HeaderCell interface
protected:
    void paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const;
    QSize sizeClientMainHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const;
};

#endif // CARDCELL_H
