// avatarwidget.cpp
#include "avatarwidget.h"
#include <QPainter>
#include <QUrl>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include <QPainterPath>
#include <QMouseEvent>
#include "../../tools/imageloader.h"

AvatarWidget::AvatarWidget(int size, QWidget *parent, const QString &placeHolder)
    : QWidget(parent),
    m_radius(16),
    m_size(size) {

    // 设置默认占位图
    m_defaultPixmap = QPixmap(placeHolder);
    if (m_defaultPixmap.isNull()) {
        m_defaultPixmap = QPixmap(size, size);
        m_defaultPixmap.fill(Qt::lightGray);
    }

    m_defaultPixmap = m_defaultPixmap.scaled(m_size, m_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setFixedSize(size, size);

    connect(ImageLoader::getInstance(), &ImageLoader::imageLoaded, this, &AvatarWidget::onImageDownloaded);
}

AvatarWidget::~AvatarWidget() {

}

void AvatarWidget::setUrl(const QString &url) {
    if (m_currentUrl == url && !url.isEmpty()) return;

    m_currentUrl = url;
    m_pixmap = QPixmap();  // 重置当前图像

    // 检查内存缓存
    if (ImageLoader::getInstance()->containCache(url)) {
        m_pixmap = *ImageLoader::getInstance()->cachedPixmap(url);
        update();
        return;
    }

    ImageLoader::getInstance()->loadUrl(url);

    update();
}

void AvatarWidget::setImage(const QPixmap &pixmap)
{
    m_pixmap = pixmap.scaled(m_size, m_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    update();
}

void AvatarWidget::setDefaultImage(const QPixmap &pixmap) {
    m_defaultPixmap = pixmap.scaled(m_size, m_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    update();
}

void AvatarWidget::setRadius(int radius) {
    m_radius = radius;
    update();
}

void AvatarWidget::onImageDownloaded(const QString &url, int row, int column, const QString &prefix)
{
    if (m_currentUrl == url && ImageLoader::getInstance()->containCache(url)) {
        m_pixmap = *ImageLoader::getInstance()->cachedPixmap(url);
        update();
        return;
    }
}

void AvatarWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) { // 判断是否为左键点击
        QPoint clickPos = event->pos(); // 获取点击位置（相对于widget）
        emit clicked(clickPos); // 发出点击信号
        qDebug() << "Widget clicked at:" << clickPos;
    }
    QWidget::mousePressEvent(event); // 确保默认事件处理继续进行
}

void AvatarWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制圆角背景
    QRectF rect(0, 0, m_size, m_size);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::lightGray);
    painter.drawRoundedRect(rect, m_radius, m_radius);

    // 创建裁剪路径
    QPainterPath clipPath;
    clipPath.addRoundedRect(rect, m_radius, m_radius);
    painter.setClipPath(clipPath);

    // 绘制图像
    QPixmap displayPixmap;
    if (!m_pixmap.isNull()) {
        displayPixmap = m_pixmap.scaled(
            m_size, m_size,           // 目标大小
            Qt::KeepAspectRatio,      // 保持宽高比
            Qt::SmoothTransformation  // 平滑缩放
            );
    } else {
        displayPixmap = m_defaultPixmap.scaled(
            m_size, m_size,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
            );
    }

    painter.drawPixmap(0, 0, displayPixmap);
}

