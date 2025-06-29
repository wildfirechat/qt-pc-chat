#ifndef FILECELL_H
#define FILECELL_H

#include "headercell.h"

class FileCell : public HeaderCell
{
public:
    FileCell();

    // HeaderCell interface
protected:
    void paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const;
    QSize sizeClientMainHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const;


    // HeaderCell interface
protected:
    QColor bubbleBackgroundColor(int messageDirection) const;

private:
    QPixmap getFileIcon(const QString &fileName) const;
};

#endif // FILECELL_H
