#include "compositecell.h"

CompositeCell::CompositeCell() {}

void CompositeCell::paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const
{

}

QSize CompositeCell::sizeClientMainHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const
{
    return QSize(100, 20);
}
