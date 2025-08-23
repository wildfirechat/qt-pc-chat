#ifndef FILEUPLOADER_H
#define FILEUPLOADER_H

#include <string>
#include <functional>
#include <iostream>

namespace WFCLib {
class FileUploader {
public:
    static FileUploader* getLoader(std::function<void(int uploaded, int total)> progress, std::function<void()> success, std::function<void(int errorCode)> error);

    // 上传文件的函数
    virtual void uploadFile(const std::string &url, const std::string &filePath) = 0;
};
}
#endif // FILEUPLOADER_H
