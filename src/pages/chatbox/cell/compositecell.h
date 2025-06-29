#ifndef COMPOSITECELL_H
#define COMPOSITECELL_H
#include "headercell.h"

class CompositeCell : public HeaderCell
{
public:
    CompositeCell();


    // HeaderCell interface
protected:
    void paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const;
    QSize sizeClientMainHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const;
};

#endif // COMPOSITECELL_H
