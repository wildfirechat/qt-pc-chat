// headercell.cpp
#include "headercell.h"
#include "../../../tools/imageloader.h"
#include "../../utility/defaultportraitfactory.h"

#define AVATAR_SIZE 40

void HeaderCell::paintContentArea(QPainter *painter, const QStyleOptionViewItem &option, int vOffset, const MessageData &message, QMap<int, QRect> *itemAreas) const
{
    QSize avatarSize = sizeAvatarHint(option);
    QSize clientSize = sizeClientMainHint(option, option.rect.width()/2, message);
    clientSize.setHeight(std::max(clientSize.height(), 20));
    QSize supplementarySize = sizeClientSupplementaryHint(option, option.rect.width()/2, message);

    QRect avatarRect;
    QRect bubbleRect;
    QRect clientRect;
    QRect supplementaryRect;
    if(message.message.direction == WFCLib::Direction_Receive) {
        avatarRect = QRect(option.rect.x()+5, option.rect.y()+vOffset, avatarSize.width(), avatarSize.height());
        bubbleRect = QRect(option.rect.x()+5 + avatarSize.width() + 5, option.rect.y()+vOffset, clientSize.width()+20, clientSize.height()+10);
        clientRect = QRect(option.rect.x()+5 + avatarSize.width() + 5 + 10, option.rect.y()+vOffset+5, clientSize.width(), clientSize.height());
        supplementaryRect = QRect(option.rect.x()+5 + avatarSize.width() + 5 + 5, option.rect.y()+vOffset+5 + clientSize.height(), supplementarySize.width(), supplementarySize.height());
    } else {
        avatarRect = QRect(option.rect.x()+option.rect.width()-5-avatarSize.width(), option.rect.y()+vOffset, avatarSize.width(), avatarSize.height());
        bubbleRect = QRect(option.rect.x()+option.rect.width()-5-avatarSize.width()-5-clientSize.width()-20, option.rect.y()+vOffset, clientSize.width()+20, clientSize.height()+10);
        clientRect = QRect(option.rect.x()+option.rect.width()-5-avatarSize.width()-5-clientSize.width()-20+5, option.rect.y()+vOffset+5, clientSize.width(), clientSize.height());
        supplementaryRect = QRect(option.rect.x()+option.rect.width()-5-avatarSize.width()-5-clientSize.width()-10, option.rect.y()+vOffset + 5 + clientSize.height() + 5, supplementarySize.width(), supplementarySize.height());
    }
    itemAreas->insert(0, avatarRect);
    itemAreas->insert(1, bubbleRect);
    paintAvatar(painter, avatarRect, message);
    paintBubble(painter, bubbleRect, message);
    paintClientArea(painter, clientRect, message);
}

QSize HeaderCell::sizeContentHint(const QStyleOptionViewItem &option, const MessageData &message) const
{
    QSize avatarSize = sizeAvatarHint(option);
    QSize clientSize = sizeClientMainHint(option, option.rect.width()/2, message);
    QSize supplementarySize = sizeClientSupplementaryHint(option, option.rect.width()/2, message);

    //泡泡区的内边距是5，所以高度为内容区+10
    return QSize(option.rect.width(), qMax(avatarSize.height(), clientSize.height()+10+supplementarySize.height()));
}

QColor HeaderCell::bubbleBackgroundColor(int messageDirection) const
{
    if(messageDirection == WFCLib::Direction_Receive) {
        return Qt::white;
    } else {
        return QColor(168,234,120);
    }
}

//如果有额外区域，需要让子类重写
void HeaderCell::paintClientSupplementary(QPainter *painter, QRect rect, const MessageData &message) const
{

}

//如果有额外区域，需要让子类重写
QSize HeaderCell::sizeClientSupplementaryHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const
{
    return QSize(0, 0);
}

void HeaderCell::paintAvatar(QPainter *painter, QRect rect, const MessageData &message) const
{
    WFCLib::UserInfo userInfo = WFCLib::ChatClient::Instance()->getUserInfo(message.message.from, message.message.conversation.conversationType == WFCLib::Group_Type?message.message.conversation.target:"");
    QString avatarUrl = userInfo.portrait.c_str();
    // 绘制头像
    if (!avatarUrl.isEmpty()) {
        // 如果图像已加载，绘制它
        if(ImageLoader::getInstance()->containCache(avatarUrl)) {
            const QPixmap *p = ImageLoader::getInstance()->cachedPixmap(avatarUrl);
            painter->drawPixmap(rect, (*p).scaled(AVATAR_SIZE, AVATAR_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            // 绘制默认图像
            QPixmap defaultPixmap(rect.size());
            defaultPixmap.fill(Qt::lightGray);
            painter->drawPixmap(rect, defaultPixmap);

            // 异步加载头像（通过AvatarWidget的静态缓存机制）
            ImageLoader::getInstance()->loadUrl(avatarUrl);
        }
    } else {
        painter->drawPixmap(rect, DefaultPortraitFactory::defaultPortraitOfUser(userInfo.getReadableName().c_str(), AVATAR_SIZE));
    }
}

void HeaderCell::paintBubble(QPainter *painter, QRect rect, const MessageData &message) const
{
    // 保存当前的画笔状态
    QPen oldPen = painter->pen();

    // 设置一个透明的画笔
    QPen transparentPen(Qt::NoPen);
    painter->setPen(transparentPen);

    painter->setBrush(bubbleBackgroundColor(message.message.direction));
    int arrowWidth = 5;
    int arrowHeight = 10;
    int arrowX;
    int arrowY;
    QPolygon arrow;
    if(message.message.direction == WFCLib::Direction_Receive) {
        arrowX = rect.x()+arrowWidth;
        arrowY = rect.y()+10;
        arrow << QPoint(arrowX, arrowY)
              << QPoint(arrowX, arrowY + arrowHeight)
              << QPoint(arrowX - arrowWidth, arrowY + arrowHeight/2);

        rect.setX(rect.x()+arrowWidth);
        rect.setWidth(rect.width()-arrowWidth);
    } else {
        arrowX = rect.x()+rect.width()-arrowWidth;
        arrowY = rect.y()+10;
        arrow << QPoint(arrowX, arrowY)
              << QPoint(arrowX, arrowY + arrowHeight)
              << QPoint(arrowX + arrowWidth, arrowY + arrowHeight/2);

        rect.setX(rect.x()-arrowWidth);
        rect.setWidth(rect.width()-arrowWidth);
    }

    painter->drawPolygon(arrow);
    painter->drawRoundedRect(rect, 3, 3);

    painter->setPen(oldPen);
}

QSize HeaderCell::sizeAvatarHint(const QStyleOptionViewItem &option) const
{
    return QSize(AVATAR_SIZE, AVATAR_SIZE);
}
