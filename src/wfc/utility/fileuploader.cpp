#include "fileuploader.h"

#if WF_QT
#include "qt/qtfileuploader.h"
#endif

namespace WFCLib {

FileUploader* FileUploader::getLoader(std::function<void(int uploaded, int total)> progress, std::function<void()> success, std::function<void(int errorCode)> error) {
#if WF_QT
    return new QTFileUploader(progress, success, error);
#else
    ##这里添加其他平台的实现.##
#endif
}
};
