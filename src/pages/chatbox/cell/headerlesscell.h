#ifndef HEADERLESSCELL_H
#define HEADERLESSCELL_H

#include "basecell.h"

class HeaderlessCell : public BaseCell
{
public:
    HeaderlessCell() : BaseCell() {};
    virtual ~HeaderlessCell() {}

    // BaseCell interface
protected:
    void paintContentArea(QPainter *painter, const QStyleOptionViewItem &option, int vOffset, const MessageData &message, QMap<int, QRect> *itemAreas) const;
    QSize sizeContentHint(const QStyleOptionViewItem &option, const MessageData &message) const;

protected:
    virtual void paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const = 0;
    virtual QSize sizeClientHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const = 0;

private:
    void paintContentBackground(QPainter *painter, QRect rect) const;
};

#endif // HEADERLESSCELL_H
