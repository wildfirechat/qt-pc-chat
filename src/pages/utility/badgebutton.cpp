#include "badgebutton.h"
#include <QPainter>
#include <QPainterPath>


BadgeButton::BadgeButton(QWidget *parent) : QPushButton(parent), m_badgeNumber(0)
{
    m_badgeColor = Qt::red;
    m_textColor = Qt::white;
}

BadgeButton::BadgeButton(const QString &text, QWidget *parent) : QPushButton(text, parent), m_badgeNumber(0)
{
    m_badgeColor = Qt::red;
    m_textColor = Qt::white;
}

int BadgeButton::badgeNumber() const { return m_badgeNumber; }

QColor BadgeButton::badgeColor() const { return m_badgeColor; }

QColor BadgeButton::textColor() const { return m_textColor; }

void BadgeButton::setBadgeNumber(int number)
{
    if (m_badgeNumber != number) {
        m_badgeNumber = number;
        update(); // 触发重绘
        emit badgeNumberChanged(number);
    }
}

void BadgeButton::setBadgeColor(const QColor &color)
{
    if (m_badgeColor != color) {
        m_badgeColor = color;
        update();
        emit badgeColorChanged(color);
    }
}

void BadgeButton::setTextColor(const QColor &color)
{
    if (m_textColor != color) {
        m_textColor = color;
        update();
        emit textColorChanged(color);
    }
}

void BadgeButton::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event); // 先绘制原始按钮

    if (m_badgeNumber <= 0) return; // 没有徽章数字时不绘制

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 计算徽章位置和大小
    int size = 8;
    int x = width() - size / 2 - 5;
    int y = size / 2 + 5;

    // 绘制徽章背景
    QPainterPath path;
    path.addEllipse(x - size, y - size, size * 2, size * 2);
    painter.fillPath(path, m_badgeColor);

    // 绘制徽章文本
    painter.setPen(m_textColor);
    QFont font = painter.font();
    font.setBold(true);
    font.setPixelSize(size);
    painter.setFont(font);

    QString text = QString::number(m_badgeNumber);
    if (m_badgeNumber > 99) text = "99+";

    painter.drawText(QRect(x - size, y - size, size * 2, size * 2),
                     Qt::AlignCenter, text);
}
