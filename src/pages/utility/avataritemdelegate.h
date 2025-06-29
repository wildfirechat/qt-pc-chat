// avataritemdelegate.h
#ifndef AVATARITEMDELEGATE_H
#define AVATARITEMDELEGATE_H


/*
// 在主窗口中使用AvatarWidget
QListWidget *listWidget = new QListWidget(this);
AvatarItemDelegate *delegate = new AvatarItemDelegate(listWidget);
listWidget->setItemDelegate(delegate);

// 添加项目
for (const QString &avatarUrl : avatarUrls) {
    QListWidgetItem *item = new QListWidgetItem(listWidget);
    item->setData(Qt::UserRole + 1, avatarUrl);
    item->setText("用户名");
    listWidget->addItem(item);
}
 */

#include <QStyledItemDelegate>
#include <QMap>
#include "avatarwidget.h"
#include <QPainter>
#include "../../tools/imageloader.h"

class AvatarItemDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit AvatarItemDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override {
        // 获取头像URL（假设模型的第0列存储头像URL）
        QString avatarUrl = index.data(Qt::UserRole + 1).toString();

        // 绘制背景和文本
        QStyledItemDelegate::paint(painter, option, index);

        // 绘制头像
        if (!avatarUrl.isEmpty()) {
            QRect avatarRect(option.rect.left() + 5, option.rect.top() + 5,
                             option.rect.height() - 10, option.rect.height() - 10);

            // 如果图像已加载，绘制它
            if (ImageLoader::getInstance()->containCache(avatarUrl)) {
                painter->drawPixmap(avatarRect, *ImageLoader::getInstance()->cachedPixmap(avatarUrl));
            } else {
                // 绘制默认图像
                QPixmap defaultPixmap(avatarRect.size());
                defaultPixmap.fill(Qt::lightGray);
                painter->drawPixmap(avatarRect, defaultPixmap);

                // 异步加载头像（通过AvatarWidget的静态缓存机制）
                AvatarWidget loader(avatarRect.width());
                loader.setUrl(avatarUrl);
            }
        }
    }
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override {
        return QSize(200, 40);  // 调整项的高度以适应头像
    }
};

#endif // AVATARITEMDELEGATE_H
