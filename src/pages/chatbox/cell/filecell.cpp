#include "filecell.h"

#include <QPixmap>
#include <QFileInfo>

FileCell::FileCell() {}

#define ICON_SIZE  40
#define ICON_PADDING  5

#define CELL_TOP_PADDING 4

void FileCell::paintClientArea(QPainter *painter, QRect rect, const MessageData &message) const
{
    const WFCLib::FileMessageContent *fileContent = static_cast<const WFCLib::FileMessageContent*>(message.message.content);

    QRect titleRect(rect.x(), rect.y()+CELL_TOP_PADDING, rect.width() - ICON_SIZE - ICON_PADDING - ICON_PADDING, 30);
    painter->drawText(titleRect, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, fileContent->name.c_str());

    QRect sizeRect(rect.x(), rect.y()+CELL_TOP_PADDING + 30 + 4, rect.width() - ICON_SIZE - ICON_PADDING - ICON_PADDING, 24);
    QString fileSize;

    qint64 size = fileContent->size;
    if (size < 1024)
        fileSize = QString("%1 B").arg(size);
    else if (size < 1024 * 1024)
        fileSize = QString("%1 KB").arg(size / 1024.0, 0, 'f', 1);
    else
        fileSize = QString("%1 MB").arg(size / (1024.0 * 1024.0), 0, 'f', 1);
    painter->drawText(sizeRect, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, fileSize);


    QRect imageRect(rect.x()+rect.width()-ICON_SIZE-ICON_PADDING, rect.y() + CELL_TOP_PADDING, ICON_SIZE, ICON_SIZE);
    painter->drawPixmap(imageRect, getFileIcon(fileContent->name.c_str()));
}

QSize FileCell::sizeClientMainHint(const QStyleOptionViewItem &option, int maxWidth, const MessageData &message) const
{
    return QSize(248, 68);
}

QColor FileCell::bubbleBackgroundColor(int messageDirection) const
{
    return Qt::white;
}

QPixmap FileCell::getFileIcon(const QString &fileName) const
{
    QFileInfo fileInfo(fileName);
    QString ext = fileInfo.suffix(); // 返回 "pdf"
    QString fileImage;

    if (ext == "doc" || ext == "docx" || ext == "pages") {
        fileImage = "file_type_word";
    } else if (ext == "xls" || ext == "xlsx" || ext == "numbers") {
        fileImage = "file_type_xls";
    } else if (ext == "ppt" || ext == "pptx" || ext == "keynote") {
        fileImage = "file_type_ppt";
    } else if (ext == "pdf") {
        fileImage = "file_type_pdf";
    } else if (ext == "html" || ext == "htm") {
        fileImage = "file_type_html";
    } else if (ext == "txt") {
        fileImage = "file_type_text";
    } else if (ext == "jpg" || ext == "png" || ext == "jpeg") {
        fileImage = "file_type_image";
    } else if (ext == "mp3" || ext == "amr" || ext == "acm" || ext == "aif") {
        fileImage = "file_type_audio";
    } else if (ext == "mp4" || ext == "avi" || ext == "mov" || ext == "asf" ||
               ext == "wmv" || ext == "mpeg" || ext == "ogg" || ext == "mkv" ||
               ext == "rmvb" || ext == "f4v") {
        fileImage = "file_type_video";
    } else if (ext == "exe") {
        fileImage = "file_type_exe";
    } else if (ext == "xml") {
        fileImage = "file_type_xml";
    } else if (ext == "zip" || ext == "rar" || ext == "gzip" || ext == "gz") {
        fileImage = "file_type_zip";
    } else {
        fileImage = "file_type_unknown";
    }

    return QPixmap(":/images/" + fileImage + ".png");
}
