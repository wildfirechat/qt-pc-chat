#include "qtfileuploader.h"
#if WF_QT
#include <QFileInfo>
#include <QDebug>

namespace WFCLib {

QTFileUploader::QTFileUploader(std::function<void(int uploaded, int total)> progress, std::function<void()> success, std::function<void(int errorCode)> error) : QObject(NULL), mProgress(progress), mSuccess(success), mError(error)
{
    m_manager = new QNetworkAccessManager(this);
}

void QTFileUploader::uploadFile(const std::string &curl, const std::string &cfilePath)
{
    QString filePath(cfilePath.c_str());
    QUrl url(curl.c_str());

    // 检查文件是否存在
    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists()) {
        mError(-1);
        qWarning() <<"文件不存在: " << filePath;
        delete this;
        return;
    }

    // 打开文件
    m_file = new QFile(filePath, this);
    if (!m_file->open(QIODevice::ReadOnly)) {
        qWarning() <<"无法打开文件: " << filePath;
        delete m_file;
        m_file = nullptr;
        mError(-1);
        delete this;
        return;
    }

    // 创建PUT请求
    QNetworkRequest request(url);

    // 设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
    request.setHeader(QNetworkRequest::ContentLengthHeader, m_file->size());

    // 发送PUT请求
    QNetworkReply *reply = m_manager->put(request, m_file);

    // 连接信号槽
    connect(reply, &QNetworkReply::uploadProgress, this, &QTFileUploader::onUploadProgress);
    connect(reply, &QNetworkReply::finished, this, &QTFileUploader::onReplyFinished);
    connect(reply, &QNetworkReply::errorOccurred, this, &QTFileUploader::onErrorOccurred);
}

void QTFileUploader::onUploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    mProgress(bytesSent, bytesTotal);
}

void QTFileUploader::onReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    // 获取HTTP状态码
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    QString message;
    bool success = false;

    if (statusCode >= 200 && statusCode < 300) {
        success = true;
        message = "文件上传成功，状态码: " + QString::number(statusCode);
    } else {
        message = "文件上传失败，状态码: " + QString::number(statusCode) +
                  "，响应: " + reply->readAll();
    }

    // 清理资源
    reply->deleteLater();
    if (m_file) {
        m_file->close();
        m_file->deleteLater();
        m_file = nullptr;
    }

    if (statusCode >= 200 && statusCode < 300) {
        qInfo() << "文件上传成功，状态码: " << QString::number(statusCode);
        mSuccess();
    } else {
        qInfo() << "文件上传失败，状态码: " << QString::number(statusCode) <<
                  "，响应: " << reply->readAll();
        mError(statusCode);
    }
    delete this;
}

void QTFileUploader::onErrorOccurred(QNetworkReply::NetworkError error)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    QString errorMsg = "网络错误: " + reply->errorString();

    // 清理资源
    reply->deleteLater();
    if (m_file) {
        m_file->close();
        m_file->deleteLater();
        m_file = nullptr;
    }

    qWarning() << errorMsg;
    mError(-1);
    delete this;
}
}
#endif
