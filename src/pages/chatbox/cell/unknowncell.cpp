// unknowncell.cpp
#include "unknowncell.h"

UnknownCell::UnknownCell() {}

void UnknownCell::paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const
{
    painter->drawText(rect, Qt::AlignCenter, "当前版本不支持消息类型，请升级客户端或者手机端查看");
}

QSize UnknownCell::sizeClientHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const
{
    QFontMetrics fm(option.font);
    return fm.boundingRect(QRect(0, 0, maxWidth, 0), Qt::AlignCenter, "当前版本不支持消息类型，请升级客户端或者手机端查看").size();
}
