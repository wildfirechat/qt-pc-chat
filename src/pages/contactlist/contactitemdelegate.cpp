#include "contactitemdelegate.h"
#include <QPainter>
#include "../utility/defaultportraitfactory.h"
#include <QDebug>
#include "../../tools/imageloader.h"
#include "../../wfc/client/ChatClient.h"


ContactItemDelegate::ContactItemDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

void ContactItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    // 保存 painter 状态
    painter->save();

    // 获取头像URL（假设模型的第0列存储头像URL）
    QString avatarUrl = index.data(Qt::UserRole + 1).toString();
    QString name = index.data(Qt::DisplayRole).toString();
    QString count = "-1";

    // 获取单元格矩形区域
    QRect rect = option.rect;

    // 绘制背景
    if (option.state & QStyle::State_Selected) {
        painter->fillRect(rect, option.palette.highlight());
    } else {
        painter->fillRect(rect, option.palette.base());
    }


    // 绘制图标（头像）
    int iconSize = rect.height() - 10; // 图标大小为单元格高度减边距
    QRect iconRect(rect.left() + 5, rect.top() + 5, iconSize, iconSize);

    if(!index.parent().isValid()) {
        iconSize = -10;
        count = index.data(Qt::UserRole + 2).toString();
    } else {
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
    }

    bool isFriendRequest = false;
    if(index.parent().isValid() && index.parent().row() == 0) {
        QString userId = index.data(Qt::UserRole).toString();
        WFCLib::FriendRequest friendRequest = WFCLib::ChatClient::Instance()->getFriendRequest(userId.toStdString(), true);
        isFriendRequest = true;

        // 绘制原因
        QRect textRect = rect;
        textRect = textRect.adjusted(iconSize + 15, 22, 0, 0);
        painter->setPen(QColor(145, 145, 145));
        QFont font = painter->font();
        font.setPixelSize(12);
        painter->setFont(font);
        painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, friendRequest.reason.c_str());

        // 绘制状态
        QRect statusRect(option.rect.left() + option.rect.width() - 80, option.rect.top(), 72, option.rect.height());
        QString statusText = "等待验证";
        if(friendRequest.status == 1) {
            statusText = "已接受";
        } else if(friendRequest.status == 2) {
            statusText = "已拒绝";
        } else if(friendRequest.status == 3) {
            statusText = "已过期";
        }
        painter->drawText(statusRect, Qt::AlignRight | Qt::AlignVCenter, statusText);
    }
    // 绘制文本
    QRect textRect = rect.adjusted(iconSize + 15, isFriendRequest?4:0, 0, isFriendRequest?18 + 4 - rect.size().height():0);
    painter->setPen(option.palette.text().color());
    QFont font = painter->font();
    font.setPixelSize(16);
    painter->setFont(font);
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, name);

    if(count != "-1") {
        QRect textRect(option.rect.width() - 40, option.rect.top(), 40, option.rect.size().height());
        painter->setPen(QColor(145, 145, 145));
        font.setPixelSize(12);
        painter->setFont(font);
        painter->drawText(textRect, Qt::AlignRight| Qt::AlignVCenter, count);
    }

    // 恢复 painter 状态
    painter->restore();
}

QSize ContactItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    return QSize(option.rect.size().width(), 42);  // 调整项的高度以适应头像
}
