#ifndef VIDEOCELL_H
#define VIDEOCELL_H

#include "headercell.h"

class VideoCell : public HeaderCell
{
public:
    VideoCell();


    // HeaderCell interface
protected:
    void paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const;
    QSize sizeClientMainHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const;
};

#endif // VIDEOCELL_H
