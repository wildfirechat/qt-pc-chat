#ifndef CALLCELL_H
#define CALLCELL_H

#include "headercell.h"

class CallCell : public HeaderCell
{
public:
    CallCell();

    // HeaderCell interface
protected:
    void paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const;
    QSize sizeClientMainHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const;
};

#endif // CALLCELL_H
