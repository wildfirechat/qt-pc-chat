// textcell.cpp
#include "textcell.h"

TextCell::TextCell() {}

void TextCell::paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const
{
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, message.message.content->digest(message.message).c_str());
}

QSize TextCell::sizeClientMainHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const
{
    QFontMetrics fm(option.font);
    return fm.boundingRect(QRect(0, 0, maxWidth, 0), Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, message.message.content->digest(message.message).c_str()).size();
}
