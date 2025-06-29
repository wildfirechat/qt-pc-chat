#ifndef HEADERCELL_H
#define HEADERCELL_H

#include "basecell.h"

class HeaderCell : public BaseCell
{
public:
    HeaderCell() : BaseCell() {};
    virtual ~HeaderCell() {}

    // BaseCell interface
protected:
    void paintContentArea(QPainter *painter, const QStyleOptionViewItem &option, int vOffset, const MessageData &message, QMap<int, QRect> *itemAreas) const;
    QSize sizeContentHint(const QStyleOptionViewItem &option, const MessageData &message) const;

protected:
    virtual void paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const = 0;
    virtual QSize sizeClientMainHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const = 0;

    //泡泡区的背景颜色，默认左白右绿，可以覆盖修改颜色.
    virtual QColor bubbleBackgroundColor(int messageDirection) const;

    //额外区域，除了泡泡区以外，还需要显示一些额外的，跟消息内容相关的部分，比如引用的消息等，显示在泡泡区的下面。不包括消息的状态，阅读回执等。
    virtual void paintClientSupplementary(QPainter *painter, QRect rect, const MessageData &message) const;
    virtual QSize sizeClientSupplementaryHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const;

private:
    void paintAvatar(QPainter *painter, QRect rect, const MessageData &message) const;
    void paintBubble(QPainter *painter, QRect rect, const MessageData &message) const;
    QSize sizeAvatarHint(const QStyleOptionViewItem &option) const;
};

#endif // HEADERCELL_H
