#ifndef LOCATIONCELL_H
#define LOCATIONCELL_H

#include "headercell.h"

class LocationCell : public HeaderCell
{
public:
    LocationCell();


    // HeaderCell interface
protected:
    void paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const;
    QSize sizeClientMainHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const;
};

#endif // LOCATIONCELL_H
