// informationcell.cpp
#include "informationcell.h"

InformationCell::InformationCell() {}

void InformationCell::paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const
{
    painter->drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, message.message.content->digest(message.message).c_str());
}

QSize InformationCell::sizeClientHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const
{
    QFontMetrics fm(option.font);
    return fm.boundingRect(QRect(0, 0, maxWidth, 10000), Qt::AlignCenter | Qt::TextWordWrap, message.message.content->digest(message.message).c_str()).size();
}
