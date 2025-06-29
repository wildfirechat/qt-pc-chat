#ifndef RECALLCELL_H
#define RECALLCELL_H

#include "headerlesscell.h"

class RecallCell : public HeaderlessCell
{
public:
    RecallCell();

    // HeaderlessCell interface
protected:
    void paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const;
    QSize sizeClientHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const;
};

#endif // RECALLCELL_H
