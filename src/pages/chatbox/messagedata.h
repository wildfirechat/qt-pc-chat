#ifndef MESSAGEDATA_H
#define MESSAGEDATA_H
#include "../../wfc/client/ChatClient.h"


class MessageData
{
public:
    MessageData();
    MessageData(const WFCLib::Message &msg);
    MessageData(const MessageData &m);
    WFCLib::Message message;
    bool showTime;
    bool showName;
    WFCLib::Message quotedMessage;
    bool mediaDownloading;
    int mediaDownloadProgress;
    bool voicePlaying;
    bool highlighted;

    bool lastReadMessage;
};

Q_DECLARE_METATYPE(MessageData);

#endif // MESSAGEDATA_H
