#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <QObject>

class QNetworkReply;
class QNetworkAccessManager;
class QTimer;
class ImageLoader;

class Loader : public QObject {
    Q_OBJECT

public:
    explicit Loader(const QString &url, int row, int column, const QString &prefix = "", QObject *parent = nullptr);

    ~Loader();


private slots:
    void onImageDownloaded(QNetworkReply *reply);

    void retryDownload();

private:
    void setUrl(const QString &url);
    QNetworkAccessManager *m_networkManager;
    QString m_currentUrl;
    int m_retryCount;
    QTimer *m_retryTimer;
    int m_row;
    int m_column;
    QString m_prefix;
};


class ImageLoader : public QObject
{
    Q_OBJECT
public:
    static ImageLoader* getInstance(QObject* parent = nullptr);

public:
    bool containCache(const QString &url);
    const QPixmap* cachedPixmap(const QString &url);
    bool onImageDownloaded(const QString &url, const QByteArray &data, int row, int column, const QString &prefix);
    bool loadUrl(const QString &url, int row = -1, int column = -1, const QString &prefix = "");

    bool containDiskCache(const QString &url, const QString &prefix = "");
    QString getDiskCache(const QString &url, const QString &prefix = "");
signals:
    void imageLoaded(const QString &url, int row, int column, const QString &prefix);

public:
    static QSet<QString> s_pendingUrls;

private:
    explicit ImageLoader(QObject* parent = nullptr);
    ~ImageLoader();
    Q_DISABLE_COPY(ImageLoader)

private:
    // 静态缓存，所有头像组件共享
    static QCache<QString, QPixmap> s_imageCache;


    // file cache
    static QString getCacheFilePath(const QString &url, const QString &prefix = "");
    static bool loadFromDiskCache(const QString &url, QPixmap &pixmap, const QString &prefix = "");
    static void saveToDiskCache(const QString &url, const QByteArray &data, const QString &prefix = "");
};

#endif // IMAGELOADER_H
