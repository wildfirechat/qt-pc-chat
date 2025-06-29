#include "defaultportraitfactory.h"
#include <QtGlobal>
#include <QString>
#include <QPixmap>
#include <QPainter>

DefaultPortraitFactory::DefaultPortraitFactory() {}

QPixmap DefaultPortraitFactory::defaultPortraitOfUser(const QString &displayText, int width)
{
    // 创建随机颜色的圆形头像
    QPixmap pixmap(width, width);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    // 随机颜色
    srand(qHash(displayText));
    QColor color(rand() % 256, rand() % 256, rand() % 256);
    painter.setBrush(color);
    painter.setPen(Qt::NoPen);
    // painter.drawEllipse(0, 0, width, width);
    painter.drawRect(0, 0, width, width);

    // 添加姓名首字母
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", width/2, QFont::Bold));
    painter.drawText(pixmap.rect(), Qt::AlignCenter, displayText.left(1));
    painter.end();

    return pixmap;
}
