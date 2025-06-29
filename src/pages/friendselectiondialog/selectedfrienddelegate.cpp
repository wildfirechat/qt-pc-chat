#include "selectedfrienddelegate.h"
#include <QMessageBox>
#include <QApplication>
#include <QPainter>
#include "../../wfc/client/ChatClient.h"
#include "../utility/defaultportraitfactory.h"
#include "../../tools/imageloader.h"


SelectedFriendDelegate::SelectedFriendDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}


void SelectedFriendDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;

    // 绘制背景和文本
    QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter);


    // 获取单元格矩形区域
    QRect rect = option.rect;

    // 绘制图标（头像）
    int iconSize = rect.height() - 16; // 图标大小为单元格高度减边距
    QRect iconRect(rect.left() + 10, rect.top() + 8, iconSize, iconSize);

    QString friendId = index.data(Qt::UserRole).toString();
    WFCLib::UserInfo userInfo = WFCLib::ChatClient::Instance()->getUserInfo(friendId.toStdString(), false);

    // 绘制头像
    if (userInfo.portrait != "") {
        // 如果图像已加载，绘制它
        if(ImageLoader::getInstance()->containCache(userInfo.portrait.c_str())) {
            const QPixmap *p = ImageLoader::getInstance()->cachedPixmap(userInfo.portrait.c_str());
            painter->drawPixmap(iconRect, *p);
        } else {
            // 绘制默认图像
            QPixmap defaultPixmap(iconRect.size());
            defaultPixmap.fill(Qt::lightGray);
            painter->drawPixmap(iconRect, defaultPixmap);

            // 异步加载头像（通过AvatarWidget的静态缓存机制）
            ImageLoader::getInstance()->loadUrl(userInfo.portrait.c_str(), index.row(), index.column());
        }
    } else {
        painter->drawPixmap(iconRect, DefaultPortraitFactory::defaultPortraitOfUser(userInfo.getReadableName().c_str()));
    }

    // 绘制文本
    QRect textRect(rect.left() + 10 + iconSize + 10, rect.top() + 8, rect.width()-(10 + iconSize + 10), iconSize);
    painter->setPen(option.palette.text().color());
    QFont font = painter->font();
    font.setPixelSize(16);
    painter->setFont(font);
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, userInfo.getReadableName().c_str());
}

bool SelectedFriendDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        // 切换复选框状态
        Qt::CheckState state = static_cast<Qt::CheckState>(index.data(Qt::CheckStateRole).toInt());
        model->setData(index, state == Qt::Checked ? Qt::Unchecked : Qt::Checked, Qt::CheckStateRole);
        return true;
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

QSize SelectedFriendDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(option.rect.width(), 44);
}
