#include "messagedata.h"

/*
    WFCLib::Message message;
    bool showTime;
    bool showName;
    WFCLib::Message quotedMessage;
    bool mediaDownloading;
    int mediaDownloadProgress;
    bool voicePlaying;
    bool highlighted;

    bool lastReadMessage;
*/

MessageData::MessageData() :
    message(),
    showTime(false),
    showName(false),
    quotedMessage(),
    mediaDownloading(false),
    mediaDownloadProgress(0),
    voicePlaying(false),
    highlighted(false),
    lastReadMessage(false)
{

}

MessageData::MessageData(const WFCLib::Message &msg) :
    message(msg),
    showTime(false),
    showName(false),
    quotedMessage(),
    mediaDownloading(false),
    mediaDownloadProgress(0),
    voicePlaying(false),
    highlighted(false),
    lastReadMessage(false)
{

}

MessageData::MessageData(const MessageData &m) :
    message(m.message),
    showTime(m.showTime),
    showName(m.showName),
    quotedMessage(m.quotedMessage),
    mediaDownloading(m.mediaDownloading),
    mediaDownloadProgress(m.mediaDownloadProgress),
    voicePlaying(m.voicePlaying),
    highlighted(m.highlighted),
    lastReadMessage(m.lastReadMessage)
{

}
