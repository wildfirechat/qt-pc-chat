#ifndef QTFILEUPLOADER_H
#define QTFILEUPLOADER_H

#if WF_QT
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QUrl>
#include <functional>
#include <iostream>
#include "../fileuploader.h"

namespace WFCLib {
class QTFileUploader : public QObject, public FileUploader
{
    Q_OBJECT
public:
    // 上传文件的函数
    virtual void uploadFile(const std::string &url, const std::string &filePath);
private:
    explicit QTFileUploader(std::function<void(int uploaded, int total)> progress, std::function<void()> success, std::function<void(int errorCode)> error);
    friend FileUploader;
private slots:
    // 处理上传进度
    void onUploadProgress(qint64 bytesSent, qint64 bytesTotal);
    // 处理上传完成
    void onReplyFinished();
    // 处理错误
    void onErrorOccurred(QNetworkReply::NetworkError error);

private:
    QNetworkAccessManager *m_manager; // 网络访问管理器
    QFile *m_file; // 要上传的文件
    std::function<void(int uploaded, int total)> mProgress;
    std::function<void()> mSuccess;
    std::function<void(int errorCode)> mError;
};
};
#endif
#endif // QTFILEUPLOADER_H
