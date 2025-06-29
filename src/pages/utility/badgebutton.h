#ifndef BADGEBUTTON_H
#define BADGEBUTTON_H

#include <QPushButton>

class BadgeButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(int badgeNumber READ badgeNumber WRITE setBadgeNumber NOTIFY badgeNumberChanged)
    Q_PROPERTY(QColor badgeColor READ badgeColor WRITE setBadgeColor NOTIFY badgeColorChanged)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY textColorChanged)

public:
    explicit BadgeButton(QWidget *parent = nullptr);

    explicit BadgeButton(const QString &text, QWidget *parent = nullptr);

    int badgeNumber() const;
    QColor badgeColor() const;
    QColor textColor() const;

public slots:
    void setBadgeNumber(int number);

    void setBadgeColor(const QColor &color);

    void setTextColor(const QColor &color);

signals:
    void badgeNumberChanged(int number);
    void badgeColorChanged(const QColor &color);
    void textColorChanged(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int m_badgeNumber;
    QColor m_badgeColor;
    QColor m_textColor;
};
#endif // BADGEBUTTON_H
