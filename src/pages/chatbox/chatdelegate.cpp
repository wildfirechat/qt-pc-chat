#include "chatdelegate.h"
#include "../../wfc/client/ChatClient.h"
#include "cell/textcell.h"
#include "cell/informationcell.h"
#include "cell/unknowncell.h"
#include "cell/headerlesscell.h"
#include "cell/basecell.h"
#include "cell/imagecell.h"
#include "cell/filecell.h"
#include "cell/recallcell.h"
#include "cell/notificationcell.h"
#include "cell/voicecell.h"
#include "cell/callcell.h"
#include "cell/cardcell.h"
#include "cell/compositecell.h"
#include "cell/linkcell.h"
#include "cell/locationcell.h"
#include "cell/richnotificationcell.h"
#include "cell/stickercell.h"
#include "cell/videocell.h"
#include "messagedata.h"
#include <QEvent>
#include <QMouseEvent>


static NotificationCell notificationCell;

QHash<QModelIndex, QMap<int, QRect>*> ChatDelegate::itemAreasMap; // 用于存储每个项的区域信息
void ChatDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    MessageData msg = index.data(Qt::UserRole).value<MessageData>();
    const BaseCell *cell = cellForMessage(msg.message.content);
    // 存储区域信息供hitTest使用
    if (!itemAreasMap.contains(index)) {
        itemAreasMap[index] = new QMap<int, QRect>();
    }
    QMap<int, QRect> *itemAreas = itemAreasMap.value(index);
    cell->paint(painter, option, index, msg, itemAreas);
}

QSize ChatDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    MessageData msg = index.data(Qt::UserRole).value<MessageData>();

    const BaseCell *cell = cellForMessage(msg.message.content);
    return cell->sizeHint(option, index, msg);
}

bool ChatDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) {
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        QMap<int, QRect> *itemAreas = itemAreasMap.value(index);

        if (mouseEvent->button() == Qt::RightButton && itemAreas->value(0).contains(mouseEvent->pos())) {
            emit avatarRightClicked(index);
            return true;
        }

        if (mouseEvent->button() == Qt::LeftButton && itemAreas->value(1).contains(mouseEvent->pos())) {
            emit messageContentClicked(index);
            return true;
        }
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

const BaseCell *ChatDelegate::cellForMessage(const WFCLib::MessageContent *content) const
{
    if (dynamic_cast<const WFCLib::NotificationMessageContent*>(content)) {
        return &notificationCell;
    } else {
        int msgType = content->getPrototype()->getType();

        if(cellMaps.keys().contains(msgType)) {
            return cellMaps.value(msgType);
        } else {
            return &unknownCell;
        }
    }
}

void ChatDelegate::registerAllCell()
{
    cellMaps[WFCLib::MESSAGE_CONTENT_TYPE_TEXT] = new TextCell();
    cellMaps[WFCLib::MESSAGE_CONTENT_TYPE_IMAGE] = new ImageCell();
    cellMaps[WFCLib::MESSAGE_CONTENT_TYPE_FILE] = new FileCell();
    cellMaps[WFCLib::MESSAGE_CONTENT_TYPE_P_TEXT] = new TextCell();
    cellMaps[WFCLib::MESSAGE_CONTENT_TYPE_TIP] = new InformationCell();
    cellMaps[WFCLib::MESSAGE_CONTENT_TYPE_RECALL] = new RecallCell();
    cellMaps[WFCLib::MESSAGE_CONTENT_TYPE_SOUND] = new VoiceCell();
    cellMaps[WFCLib::VOIP_CONTENT_TYPE_START] = new CallCell();
    cellMaps[WFCLib::MESSAGE_CONTENT_TYPE_VIDEO] = new VideoCell();
    // cellMaps[WFCLib::MESSAGE_CONTENT_TYPE_CARD] = new CardCell();
    // cellMaps[WFCLib::MESSAGE_CONTENT_TYPE_COMPOSITE] = new CompositeCell();
    // cellMaps[WFCLib::MESSAGE_CONTENT_TYPE_LINK] = new LinkCell();
    cellMaps[WFCLib::MESSAGE_CONTENT_TYPE_LOCATION] = new LocationCell();
    // cellMaps[WFCLib::MESSAGE_CONTENT_TYPE_RICH_NOTIFICATION] = new RichNotificationCell();
    cellMaps[WFCLib::MESSAGE_CONTENT_TYPE_STICKER] = new StickerCell();
}

ChatDelegate::ChatDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
    registerAllCell();
}
