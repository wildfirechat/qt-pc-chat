#include "voicecell.h"
#include "../../wfc/client/ChatClient.h"
#include <QDateTime>

VoiceCell::VoiceCell() {}


#define VOICE_ICON_SIZE 16
#define TEXT_SIZE 10
void VoiceCell::paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const
{
    const WFCLib::SoundMessageContent *soundContent = static_cast<const WFCLib::SoundMessageContent*>(message.message.content);
    QString icon;
    QRect iconRect;
    QRect textRect;
    if(message.message.direction == WFCLib::Direction_Receive) {
        iconRect = QRect(rect.x(), rect.y(), VOICE_ICON_SIZE, VOICE_ICON_SIZE);
        if(message.voicePlaying) {
            icon = ":/images/voice_from_" + QString::number(QDateTime::currentSecsSinceEpoch()%3+1) + ".png";;
        } else {
            icon = ":/images/voice_from.png";
        }
        textRect = QRect(rect.x() + rect.width() - 10, rect.y() + rect.height() - 16, 14, 14);
    } else {
        iconRect = QRect(rect.x() + rect.width() - VOICE_ICON_SIZE, rect.y(), VOICE_ICON_SIZE, VOICE_ICON_SIZE);
        if(message.voicePlaying) {
            icon = ":/images/voice_to_" + QString::number(QDateTime::currentSecsSinceEpoch()%3+1) + ".png";
        } else {
            icon = ":/images/voice_to.png";
        }
        textRect = QRect(rect.x(), rect.y() + rect.height() - 16, 14, 14);
    }
    painter->drawPixmap(iconRect, QPixmap(icon));
    painter->drawText(textRect, QString::number(soundContent->duration) + "'");
}

QSize VoiceCell::sizeClientMainHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const
{
    const WFCLib::SoundMessageContent *soundContent = static_cast<const WFCLib::SoundMessageContent*>(message.message.content);
    return QSize(VOICE_ICON_SIZE + 8 + soundContent->duration*10, VOICE_ICON_SIZE);
}
