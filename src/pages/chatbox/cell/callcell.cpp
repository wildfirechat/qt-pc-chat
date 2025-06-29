#include "callcell.h"


#define CALL_ICON_SIZE 16
#define TEXT_SIZE 10

CallCell::CallCell() {}

void CallCell::paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const
{
    const WFCLib::CallStartMessageContent *callContent = static_cast<const WFCLib::CallStartMessageContent*>(message.message.content);
    QRect receiveIconRect(rect.x(), rect.y(), CALL_ICON_SIZE, CALL_ICON_SIZE);
    painter->drawPixmap(receiveIconRect, QPixmap(callContent->audioOnly?":/images/msg_cell_voice_call.png":":/images/msg_cell_video_call.png"));
    QRect receiveTextRect(rect.x() + CALL_ICON_SIZE + 8, rect.y(), 60, 14);
    painter->drawText(receiveTextRect, callContent->digest(message.message).c_str());
}

QSize CallCell::sizeClientMainHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const
{
    return QSize(CALL_ICON_SIZE + 8 + 54, CALL_ICON_SIZE);
}
