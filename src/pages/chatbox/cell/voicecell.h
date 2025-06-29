#ifndef VOICECELL_H
#define VOICECELL_H

#include "headercell.h"

class VoiceCell : public HeaderCell
{
public:
    VoiceCell();


    // HeaderCell interface
protected:
    void paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const;
    QSize sizeClientMainHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const;
};

#endif // VOICECELL_H
