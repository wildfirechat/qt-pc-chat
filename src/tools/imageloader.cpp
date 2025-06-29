#include "imageloader.h"

#include <QCoreApplication>

#include <QPixmap>
#include <QSet>
#include <QCache>
#include <QUrl>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include <QPainterPath>
#include <QCryptographicHash>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>
#include <QCache>
#include <QTimer>
#include <QObject>



QCache<QString, QPixmap> ImageLoader::s_imageCache(100);  // 缓存100个头像
QSet<QString> ImageLoader::s_pendingUrls;

ImageLoader *ImageLoader::getInstance(QObject *parent)
{
    static ImageLoader instance(parent);
    return &instance;
}


bool ImageLoader::containCache(const QString &url)
{
    if(url.isEmpty()) {
        return false;
    }

    if(s_imageCache.contains(url)) {
        return true;
    }

    QPixmap diskPixmap;
    if (loadFromDiskCache(url, diskPixmap)) {
        s_imageCache.insert(url, new QPixmap(diskPixmap));
        return true;
    }

    return false;
}

const QPixmap *ImageLoader::cachedPixmap(const QString &url)
{
    if(s_imageCache.contains(url)) {
        return s_imageCache.object(url);
    }

    QPixmap diskPixmap;
    if (loadFromDiskCache(url, diskPixmap)) {
        s_imageCache.insert(url, new QPixmap(diskPixmap));
        return s_imageCache.object(url);
    }

    return nullptr;
}

bool ImageLoader::onImageDownloaded(const QString &url, const QByteArray &data, int row, int column, const QString &prefix)
{
    s_pendingUrls.remove(url);
    QPixmap pixmap;
    if (pixmap.loadFromData(data)) {
        // 保存到磁盘缓存
        saveToDiskCache(url, data, prefix);
        // 缩放图像
        // pixmap = pixmap.scaled(m_size, m_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // 存入缓存
        s_imageCache.insert(url, new QPixmap(pixmap));

        emit imageLoaded(url, row, column, prefix);
        return true;
    }
    return false;
}

bool ImageLoader::loadUrl(const QString &url, int row, int column, const QString &prefix)
{
    if(url.isEmpty()) {
        return false;
    }

    // 检查是否已有下载请求
    if (s_pendingUrls.contains(url)) {
        return false;
    }

    Loader *loader = new Loader(url, row, column, prefix);


    return false;
}

bool ImageLoader::containDiskCache(const QString &url, const QString &prefix)
{
    QString filePath = getCacheFilePath(url, prefix);
    QFile file(filePath);

    if (file.exists()) {
        return true;
    }

    return false;
}

QString ImageLoader::getDiskCache(const QString &url, const QString &prefix)
{
    QString filePath = getCacheFilePath(url, prefix);
    QFile file(filePath);

    if (file.exists()) {
        return filePath;
    }
    return QString();
}

ImageLoader::ImageLoader(QObject *parent) : QObject(parent) {}

ImageLoader::~ImageLoader() {}


QString ImageLoader::getCacheFilePath(const QString &url, const QString &prefix) {
    // 生成URL的哈希作为文件名
    QByteArray hash = QCryptographicHash::hash(url.toUtf8(), QCryptographicHash::Md5);
    QString fileName = hash.toHex() + ".png";

    // 获取缓存目录
    QString cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/";
    if(prefix.isEmpty()) {
        cacheDir += "avatars/";
    } else {
        cacheDir += prefix;
        cacheDir += "/";
    }
    QDir().mkpath(cacheDir);

    return cacheDir + fileName;
}

bool ImageLoader::loadFromDiskCache(const QString &url, QPixmap &pixmap, const QString &prefix) {
    QString filePath = getCacheFilePath(url, prefix);
    QFile file(filePath);

    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        bool loaded = pixmap.loadFromData(file.readAll());
        file.close();
        return loaded;
    }

    return false;
}

void ImageLoader::saveToDiskCache(const QString &url, const QByteArray &data, const QString &prefix) {
    QString filePath = getCacheFilePath(url, prefix);
    QFile file(filePath);

    if (file.open(QIODevice::WriteOnly)) {
        file.write(data);
        file.close();
    }
}


Loader::Loader(const QString &url, int row, int column, const QString &prefix, QObject *parent) : QObject(parent),
    m_networkManager(new QNetworkAccessManager(this)),
    m_retryCount(0),
    m_retryTimer(new QTimer(this)),
    m_row(row),
    m_column(column), m_prefix(prefix)
{
    setUrl(url);
    // 重试下载逻辑
    m_retryTimer->setInterval(3000);  // 3秒后重试
    connect(m_retryTimer, &QTimer::timeout, this, &Loader::retryDownload);
    connect(m_networkManager, &QNetworkAccessManager::finished,
            this, &Loader::onImageDownloaded);

}

Loader::~Loader() {
    m_retryTimer->stop();
}

void Loader::setUrl(const QString &url) {
    if (m_currentUrl == url) return;

    m_currentUrl = url;

    // 检查内存缓存
    if (ImageLoader::getInstance()->containCache(url)) {
        return;
    }

    // 检查是否已有下载请求
    if (ImageLoader::s_pendingUrls.contains(url)) {
        return;
    }

    // 开始下载
    ImageLoader::s_pendingUrls.insert(url);
    QUrl qurl(url);
    QNetworkRequest request(qurl);
    request.setHeader(QNetworkRequest::UserAgentHeader, "YourApp/1.0");
    m_networkManager->get(request);
}

void Loader::onImageDownloaded(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        if (ImageLoader::getInstance()->onImageDownloaded(m_currentUrl, data, m_row, m_column, m_prefix)) {

        } else {
            qDebug() << "Failed to load image from data:" << m_currentUrl;
            m_retryCount++;
            if (m_retryCount < 3) {
                m_retryTimer->start();
                return;
            }
        }
    } else {
        qDebug() << "Network error:" << reply->errorString() << "URL:" << m_currentUrl;
        m_retryCount++;
        if (m_retryCount < 3) {
            m_retryTimer->start();
            return;
        }
    }

    reply->deleteLater();
}

void Loader::retryDownload() {
    m_retryTimer->stop();
    QUrl url(m_currentUrl);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "YourApp/1.0");
    m_networkManager->get(request);
}
