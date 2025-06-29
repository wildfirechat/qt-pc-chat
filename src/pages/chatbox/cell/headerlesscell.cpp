// headerlesscell.cpp
#include "headerlesscell.h"

void HeaderlessCell::paintContentArea(QPainter *painter, const QStyleOptionViewItem &option, int vOffset, const MessageData &message, QMap<int, QRect> *itemAreas) const
{
    QSize size = sizeContentHint(option, message);
    QRect backgroundRect((option.rect.width()-(size.width()+10))/2, option.rect.top()+vOffset, size.width()+10, size.height()+10);
    paintContentBackground(painter, backgroundRect);

    QRect clientRect((option.rect.width()-size.width())/2, option.rect.top()+vOffset+5, size.width(), size.height());
    itemAreas->remove(0);
    itemAreas->insert(1, clientRect);
    paintClientArea(painter, clientRect, message);
}

QSize HeaderlessCell::sizeContentHint(const QStyleOptionViewItem &option, const MessageData &message) const
{
    return sizeClientHint(option, option.rect.width()/2, message);
}

void HeaderlessCell::paintContentBackground(QPainter *painter, QRect rect) const
{
    // 保存当前的画笔状态
    QPen oldPen = painter->pen();

    // 设置一个透明的画笔
    QPen transparentPen(Qt::NoPen);
    painter->setPen(transparentPen);

    QColor semiTransparentBlue(0, 0, 0, 25);
    painter->setBrush(semiTransparentBlue);
    painter->drawRoundedRect(rect, 5, 5);

    painter->setPen(oldPen);
}
