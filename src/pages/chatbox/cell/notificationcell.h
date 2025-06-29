#ifndef NOTIFICATIONCELL_H
#define NOTIFICATIONCELL_H

#include "headerlesscell.h"

class NotificationCell : public HeaderlessCell
{
public:
    NotificationCell();


    // HeaderlessCell interface
protected:
    void paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const;
    QSize sizeClientHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const;
};

#endif // NOTIFICATIONCELL_H
