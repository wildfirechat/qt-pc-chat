#include "atmemberdelegate.h"
#include <QPainter>
#include <QApplication>
#include "../../wfc/client/ChatClient.h"
#include "../utility/defaultportraitfactory.h"
#include <QDebug>
#include "../../tools/imageloader.h"

AtMemberDelegate::AtMemberDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

void AtMemberDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    // 绘制默认的背景（包括选中状态）
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    const QWidget *widget = option.widget;
    QStyle *style = widget ? widget->style() : QApplication::style();
    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, widget);

    // 获取数据
    QString memberId = index.data(Qt::DisplayRole).toString();
    WFCLib::UserInfo userInfo = WFCLib::ChatClient::Instance()->getUserInfo(memberId.toStdString());

    // 设置边距和图标大小
    int margin = 5;
    QRect iconRect = opt.rect.adjusted(margin, margin, -margin, -margin);
    iconRect.setWidth(iconRect.height());

    QRect textRect = opt.rect.adjusted(
        iconRect.right() + margin, margin, -margin, -margin);

    // 绘制图标（头像）
    int iconSize = iconRect.height(); // 图标大小为单元格高度减边距
    if (!userInfo.portrait.empty()) {
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
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, userInfo.getReadableName().c_str());
}

QSize AtMemberDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    return QSize(150, 28); // 宽度180，高度28
}
