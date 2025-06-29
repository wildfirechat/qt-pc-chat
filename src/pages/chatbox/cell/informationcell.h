#ifndef INFORMATIONCELL_H
#define INFORMATIONCELL_H

#include "headerlesscell.h"

class InformationCell : public HeaderlessCell
{
public:
    InformationCell();

    // HeaderlessCell interface
protected:
    void paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const;
    QSize sizeClientHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const;
};

#endif // INFORMATIONCELL_H
