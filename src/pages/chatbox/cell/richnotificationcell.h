#ifndef RICHNOTIFICATIONCELL_H
#define RICHNOTIFICATIONCELL_H

#include "headercell.h"

class RichNotificationCell : public HeaderCell
{
public:
    RichNotificationCell();


    // HeaderCell interface
protected:
    void paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const;
    QSize sizeClientMainHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const;
};

#endif // RICHNOTIFICATIONCELL_H
