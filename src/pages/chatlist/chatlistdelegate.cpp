#include "chatlistdelegate.h"
#include <QPainter>
#include "../../tools/imageloader.h"
#include "../utility/defaultportraitfactory.h"
#include "../../wfc/client/ChatClient.h"
#include "../../tools/timeformatter.h"


ChatListDelegate::ChatListDelegate(QObject *parent) : QStyledItemDelegate{parent} {}

void ChatListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // 保存 painter 状态
    painter->save();

    WFCLib::ConversationInfo conversationInfo = index.data(Qt::UserRole).value<WFCLib::ConversationInfo>();
    // 获取头像URL（假设模型的第0列存储头像URL）
    QString avatarUrl;
    QString name;

    if(conversationInfo.conversation.conversationType == WFCLib::Single_Type) {
        WFCLib::UserInfo userInfo = WFCLib::ChatClient::Instance()->getUserInfo(conversationInfo.conversation.target);
        avatarUrl = userInfo.portrait.c_str();
        name = userInfo.displayName.c_str();
    } else if(conversationInfo.conversation.conversationType == WFCLib::Group_Type) {
        WFCLib::GroupInfo groupInfo = WFCLib::ChatClient::Instance()->getGroupInfo(conversationInfo.conversation.target);
        avatarUrl = groupInfo.portrait.c_str();
        name = groupInfo.name.c_str();
    } else if(conversationInfo.conversation.conversationType == WFCLib::Channel_Type) {
        WFCLib::ChannelInfo channelInfo = WFCLib::ChatClient::Instance()->getChannelInfo(conversationInfo.conversation.target);
        avatarUrl = channelInfo.portrait.c_str();
        name = channelInfo.name.c_str();
    } else {
        name = "不支持的会话";
    }


    // 获取单元格矩形区域
    QRect rect = option.rect;

    // 绘制背景
    if (option.state & QStyle::State_Selected) {
        painter->fillRect(rect, QBrush(QColor(222,222,222)));
    } else {
        if(conversationInfo.isTop) {
            painter->fillRect(rect, QBrush(QColor(230,230,230)));
        } else {
            painter->fillRect(rect, option.palette.base());
        }
    }


    // 绘制图标（头像）
    int iconSize = 36; // 图标大小为单元格高度减边距
    int top = rect.top() + (rect.size().height()-iconSize)/2;
    int bottom = top + iconSize;
    QRect iconRect(rect.left() + 5, top, iconSize, iconSize);

    // 绘制头像
    if (!avatarUrl.isEmpty()) {
        // 如果图像已加载，绘制它
        if(ImageLoader::getInstance()->containCache(avatarUrl)) {
            const QPixmap *p = ImageLoader::getInstance()->cachedPixmap(avatarUrl);
            painter->drawPixmap(iconRect, *p);
        } else {
            // 绘制默认图像
            QPixmap defaultPixmap(iconRect.size());
            defaultPixmap.fill(Qt::lightGray);
            painter->drawPixmap(iconRect, defaultPixmap);

            // 异步加载头像（通过AvatarWidget的静态缓存机制）
            ImageLoader::getInstance()->loadUrl(avatarUrl, index.row(), index.column());
        }
    } else {
        painter->drawPixmap(iconRect, DefaultPortraitFactory::defaultPortraitOfUser(name));
    }

    if(conversationInfo.unreadCount.unread > 0) {
        int radis = 8;
        int extraWidth = 0;

        if(conversationInfo.isSilent) {
            radis = 4;
        } else {
            if(conversationInfo.unreadCount.unread > 100) extraWidth = 8;
            else if(conversationInfo.unreadCount.unread > 10) extraWidth = 4;
        }


        QRect unreadRect(rect.left() + 5 + iconSize - radis, top - radis, 2*radis+extraWidth, 2*radis);

        painter->setRenderHint(QPainter::Antialiasing); // 启用抗锯齿

        // 绘制红色圆形
        QBrush brush(Qt::red);
        QPen transparentPen(Qt::NoPen);
        painter->setPen(transparentPen);
        painter->setBrush(brush);
        painter->drawRoundedRect(unreadRect, radis, radis);

        // 设置文本颜色和字体
        QPen pen(Qt::white);
        painter->setPen(pen);
        QFont font = painter->font();
        font.setPointSize(8);
        font.setBold(true);
        painter->setFont(font);

        // 绘制数字
        if(!conversationInfo.isSilent) {
            QString numberText = QString::number(conversationInfo.unreadCount.unread);
            painter->drawText(unreadRect, Qt::AlignCenter, numberText);
        }
    }

    QString timeStr = TimeFormatter::formatMilliseconds(conversationInfo.timestamp);

    // 创建字体并设置大小（点大小）
    QFont font("Arial", 8);
    painter->setFont(font);

    // 创建QFontMetrics对象
    QFontMetrics fm(font);


    // 除了头像外，其他向上提4个点
    top -= 2;
    // 计算字符串的边界矩形
    QRect timerect = fm.boundingRect(timeStr);

    // 获取字符串的宽度
    int timewidth = timerect.width();

    QRect timeRect(rect.size().width() - timewidth-4, top, timewidth, 12);
    painter->setPen(option.palette.placeholderText().color());
    painter->drawText(timeRect, Qt::AlignLeft, timeStr);


    font.setPixelSize(14);
    painter->setFont(font);

    QRect txtRect(iconSize + 10, top, rect.size().width() - iconSize - 10 - timewidth - 8, 14+2);
    painter->setPen(option.palette.text().color());
    painter->drawText(txtRect, Qt::AlignLeft, name);

    QRect digestRect(iconSize + 10, bottom-14, rect.size().width() - iconSize - 10 - 8 - 16, 20);
    if(conversationInfo.unreadCount.unreadMention > 0 || conversationInfo.unreadCount.unreadMentionAll > 0) {
        QPen oldPen = painter->pen();
        painter->setPen(QColorConstants::Red);
        QString mentionText("[有人@我]");

        QFontMetrics fm(painter->font());
        QSize metionSize = fm.boundingRect(digestRect, Qt::AlignLeft, mentionText).size();

        painter->drawText(digestRect, Qt::AlignLeft, mentionText);

        digestRect.setX(digestRect.x() + metionSize.width() + 2);
        digestRect.setWidth(digestRect.width() - metionSize.width() + 2);
        painter->setPen(oldPen);
    }

    if(!conversationInfo.draft.empty()) {
        QPen oldPen = painter->pen();
        painter->setPen(QColorConstants::Red);
        QString draftText("[草稿]");

        QFontMetrics fm(painter->font());
        QSize draftSize = fm.boundingRect(digestRect, Qt::AlignLeft, draftText).size();

        painter->drawText(digestRect, Qt::AlignLeft, draftText);

        digestRect.setX(digestRect.x() + draftSize.width() + 2);
        digestRect.setWidth(digestRect.width() - draftSize.width() + 2);
        painter->setPen(oldPen);
    }

    if(conversationInfo.lastMessage.messageId > 0 && conversationInfo.lastMessage.status == WFCLib::Message_Status_Sending) {
        QRect sendingRect(digestRect.x(), digestRect.y(), 18, 18);
        painter->drawPixmap(sendingRect, QPixmap(":/images/conversation_msg_sending.png"));

        digestRect.setX(digestRect.x() + sendingRect.width() + 2);
        digestRect.setWidth(digestRect.width() - sendingRect.width() + 2);
    }

    if(conversationInfo.lastMessage.messageId > 0 && conversationInfo.lastMessage.status == WFCLib::Message_Status_Send_Failure) {
        QRect sendingRect(digestRect.x(), digestRect.y()+2, 12, 12);
        painter->drawPixmap(sendingRect, QPixmap(":/images/conversation_msg_sending_failure.png"));

        digestRect.setX(digestRect.x() + sendingRect.width() + 2);
        digestRect.setWidth(digestRect.width() - sendingRect.width() + 2);
    }

    if(conversationInfo.lastMessage.messageId != 0 && conversationInfo.lastMessage.messageId != -1) {
        QString digestStr = conversationInfo.lastMessage.content->digest(conversationInfo.lastMessage).c_str();
        font.setPixelSize(12);
        painter->setFont(font);
        painter->setPen(option.palette.placeholderText().color());
        painter->drawText(digestRect, Qt::AlignLeft, digestStr);
    }

    if(conversationInfo.isSilent) {
        QRect muteRect(rect.size().width() - 12 - 8, top + 22, 12, 12);
        QPixmap mutePic(":/images/mute_notification.png");
        painter->drawPixmap(muteRect, mutePic.scaled(QSize(12, 12)));
    }

    // 恢复 painter 状态
    painter->restore();
}

QSize ChatListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(option.rect.size().width(), 64);
}
