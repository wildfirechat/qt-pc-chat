#include "imagecell.h"
#include <QDebug>

ImageCell::ImageCell() {}

void ImageCell::paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const
{
    const WFCLib::ImageMessageContent *imageContent = static_cast<const WFCLib::ImageMessageContent*>(message.message.content);
    QImage image;
    if (image.loadFromData(QByteArray::fromStdString(imageContent->thumbnail), "JPEG")) {
        painter->drawImage(rect, image);
    } else {
        qDebug() << "failure";
    }
}

QSize ImageCell::sizeClientMainHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const
{
    const WFCLib::ImageMessageContent *imageContent = static_cast<const WFCLib::ImageMessageContent*>(message.message.content);
    QImage image;
    if (image.loadFromData(QByteArray::fromStdString(imageContent->thumbnail), "JPEG")) {
        return image.size();
    }

    return QSize(120, 120);
}
