// avatarwidget.h
#ifndef AVATARWIDGET_H
#define AVATARWIDGET_H


/*
// 在普通页面中直接使用
AvatarWidget *avatar = new AvatarWidget(this);
avatar->setSize(128);
avatar->setRadius(64);  // 圆形头像
avatar->setUrl("https://example.com/avatar.jpg");
 */




#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>
#include <QCache>
#include <QTimer>
#include <QCryptographicHash>

class AvatarWidget : public QWidget {
    Q_OBJECT
public:
    explicit AvatarWidget(int size, QWidget *parent = nullptr, const QString &placeHolder = ":/images/avatar_def.png");
    ~AvatarWidget();

    void setUrl(const QString &url);
    void setImage(const QPixmap &pixmap);
    void setDefaultImage(const QPixmap &pixmap);
    void setRadius(int radius);  // 圆角半径

signals:
    void clicked(const QPoint &pos); // 发送包含点击位置的信号

private slots:
    void onImageDownloaded(const QString &url, int row, int column, const QString &prefix);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    void paintEvent(QPaintEvent *event) override;

    QPixmap m_pixmap;
    QString m_currentUrl;
    QPixmap m_defaultPixmap;
    int m_radius;
    int m_size;

};

#endif // AVATARWIDGET_H
