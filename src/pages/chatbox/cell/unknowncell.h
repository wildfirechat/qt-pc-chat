#ifndef UNKNOWNCELL_H
#define UNKNOWNCELL_H

#include "headerlesscell.h"

class UnknownCell : public HeaderlessCell
{
public:
    UnknownCell();

    // HeaderlessCell interface
protected:
    void paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const;
    QSize sizeClientHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const;
};

#endif // UNKNOWNCELL_H
