#include "ChatClient.h"
#include "../proto/include/WFClient.h"
#include <mutex>
#include "../utility/JsonTools.h"


#ifdef WIN32
#define WFCAPI __stdcall
#else
#define WFCAPI
#endif

namespace WFCLib {

template <typename T>
std::list<T> serializableFromJsonList(const std::string &jsonListStr);


static ConnectionStatusListener *gConnectionStatusListener = NULL;

static void WFCAPI client_connection_callback(int status) {
    ChatClient::Instance()->onConnectionStatusChanged((ConnectionStatus)status);
}

static ReceiveMessageListener *gReceiveMessageListener = NULL;

static void WFCAPI client_receive_message_callback(const char *cmessages, size_t messages_len, bool moreMsg) {
    std::list<Message> tmsgs = serializableFromJsonList<Message>(std::string(cmessages, messages_len));
    ChatClient::Instance()->onReceiveMessages(tmsgs, moreMsg);
}
static void WFCAPI client_recall_message_callback(const char *coperatorId, size_t operatorId_len, int64_t messageUid) {
    ChatClient::Instance()->onRecallMessage(std::string(coperatorId, operatorId_len), messageUid);
}
static void WFCAPI client_delete_message_callback(int64_t messageUid) {
    ChatClient::Instance()->onDeleteMessage(messageUid);
}

static void WFCAPI client_message_delivered_callback(const char *cstr, size_t str_len) {

}

static void WFCAPI client_message_readed_callback(const char *cstr, size_t str_len) {

}


static UserInfoUpdateListener *gUserInfoUpdateListener = NULL;
void WFCAPI client_userInfo_update_callback(const char *cuserInfos, size_t userInfos_len) {
    ChatClient::Instance()->onUserInfoUpdated(serializableFromJsonList<UserInfo>(std::string(cuserInfos, userInfos_len)));
}


static GroupInfoUpdateListener *gGroupInfoUpdateListener = NULL;
void WFCAPI client_groupInfo_update_callback(const char* cgroupInfos, size_t groupInfos_len) {
    ChatClient::Instance()->onGroupInfoUpdated(serializableFromJsonList<GroupInfo>(std::string(cgroupInfos, groupInfos_len)));
}


static GroupMemberUpdateListener *gGroupMemberUpdateListener = NULL;
void WFCAPI client_groupMembers_update_callback(const char *cgroupId, size_t groupId_len) {
    ChatClient::Instance()->onGroupMemberUpdated(std::string(cgroupId, groupId_len));
}


static ContactUpdateListener *gContactUpdateListener = NULL;
void WFCAPI client_friendList_update_callback(const char *cfriendList, size_t friendList_len) {
    ChatClient::Instance()->onContactUpdated(parseStringList(std::string(cfriendList, friendList_len)));
}


static FriendRequestUpdateListener *gFriendRequestUpdateListener = NULL;
void WFCAPI client_friendRequest_update_callback(const char *cfriendList, size_t friendList_len) {
    ChatClient::Instance()->onFriendRequestUpdated(parseStringList(std::string(cfriendList, friendList_len)));
}


static UserSettingUpdateListener *gUserSettingUpdateListener = NULL;
void WFCAPI client_user_setting_update_callback() {
    ChatClient::Instance()->onUserSettingUpdated();
}

static ChannelInfoUpdateListener *gChannelInfoUpdateListener = NULL;
void WFCAPI client_channelInfo_update_callback(const char *cchannelInfo, size_t channelInfo_len) {
    ChatClient::Instance()->onChannelInfoUpdated(serializableFromJsonList<ChannelInfo>(std::string(cchannelInfo, channelInfo_len)));
}


static void WFCAPI client_genernal_void_success_callback(void *pObj, int dataType) {
    if(pObj) {
        GeneralVoidCallback *callback = (GeneralVoidCallback *)pObj;
        callback->onSuccess(dataType);
    }
}


class VoidCallbackContext {
public:
    enum class Type {
        NONE,
        CONVERSATION_TOP,
        CONVERSATION_SILENT,
        RECALL_MESSAGE_UID,
        DELETE_FRIEND_USER_ID,
        SEND_FRIEND_REQUEST_USER_ID,
        HANDLE_FRIEND_REQUEST_USER_ID,
        SET_FRIEND_ALIAS_USER_ID,
        SET_FRIEND_BLACKLIST_USER_ID,
        ADD_GROUP_MEMBER_GROUP_ID,
        KICKOFF_GROUP_MEMBER_GROUP_ID,
        QUIT_GROUP_ID,
        DISMISS_GROUP_ID,
        MODIFY_GROUP_INFO_ID,
        MODIFY_GROUP_ALIAS_ID,
        TRANSFER_GROUP_ID,
        SET_MANAGER_GROUP_ID,
        SET_FAV_GROUP_ID,
        USER_SETTING,
        MODIFY_MY_INFO,
        SET_GLOBAL_SILENT,
        SET_HIDDEN_NOTIFI_DETAIL,
        SET_HIDDEN_GROUP_ALIAS,
        MODIFY_CHANNEL_ID,
        LISTEN_CHANNEL_ID,
        DESTROY_CHANNEL_ID,
    };

    static VoidCallbackContext* newConversationTopContext(const Conversation &conv, GeneralVoidCallback *callback, int callbackPara) {
        return new VoidCallbackContext(conv, callback, callbackPara, Type::CONVERSATION_TOP);
    }
    static VoidCallbackContext* newConversationSilentContext(const Conversation &conv, GeneralVoidCallback *callback, int callbackPara) {
        return new VoidCallbackContext(conv, callback, callbackPara, Type::CONVERSATION_SILENT);
    }

    static VoidCallbackContext* newRecallContext(int64_t messageUid, GeneralVoidCallback *callback, int callbackPara) {
        VoidCallbackContext* context = new VoidCallbackContext(callback, callbackPara, Type::RECALL_MESSAGE_UID);
        context->messageUid = messageUid;
        return context;
    }

    static VoidCallbackContext* newStringContext(const std::string &str, Type t, GeneralVoidCallback *callback, int callbackPara) {
        VoidCallbackContext* context = new VoidCallbackContext(callback, callbackPara, t);
        context->str = str;
        return context;
    }

    static VoidCallbackContext* newVoidContext(Type t, GeneralVoidCallback *callback, int callbackPara) {
        VoidCallbackContext* context = new VoidCallbackContext(callback, callbackPara, t);
        return context;
    }

    static VoidCallbackContext* newUserSettingContext(UserSettingScope scope, const std::string &key, const std::string &value, GeneralVoidCallback *callback, int callbackPara) {
        VoidCallbackContext* context = new VoidCallbackContext(callback, callbackPara, Type::USER_SETTING);
        context->scope = scope;
        context->key = key;
        context->value = value;
        return context;
    }


    GeneralVoidCallback *cb;
    int cbPara;

    Conversation conversation;
    int64_t messageUid;
    std::string str;
    UserSettingScope scope;
    std::string key;
    std::string value;

    Type type;
private:
    VoidCallbackContext(const Conversation &conv, GeneralVoidCallback *callback, int callbackPara, Type t): cb(callback), cbPara(callbackPara), conversation(conv), type(t) {}
    VoidCallbackContext(GeneralVoidCallback *callback, int callbackPara, Type t): cb(callback), cbPara(callbackPara), type(t) {}
};

static void WFCAPI client_context_void_success_callback(void *pObj, int dataType) {
    VoidCallbackContext *pContent = (VoidCallbackContext *)pObj;

    if(pContent->cb) {
        GeneralVoidCallback *callback = (GeneralVoidCallback *)pContent->cb;
        callback->onSuccess(dataType);
    }
    ChatClient::Instance()->onVoidConextSuccess(pObj);
    delete pContent;
}

static void WFCAPI client_genernal_void_error_callback(void *pObj, int dataType, int errorCode) {
    if(pObj) {
        GeneralVoidCallback *callback = (GeneralVoidCallback *)pObj;
        callback->onFailure(dataType, errorCode);
    }
}

static void WFCAPI client_genernal_string_success_callback(void *pObj, int dataType, const char *cvalue, size_t value_len) {
    if(pObj) {
        GeneralStringCallback *callback = (GeneralStringCallback *)pObj;
        callback->onSuccess(dataType, std::string(cvalue, value_len));
    }
}

static void WFCAPI client_genernal_string_error_callback(void *pObj, int dataType, int errorCode) {
    if(pObj) {
        GeneralStringCallback *callback = (GeneralStringCallback *)pObj;
        callback->onFailure(dataType, errorCode);
    }
}

static void WFCAPI client_get_remote_message_success_callback(void *pObj, int dataType, const char *cvalue, size_t value_len) {
    if(pObj) {
        GetRemoteMessageCallback *callback = (GetRemoteMessageCallback *)pObj;
        callback->onSuccess(dataType, serializableFromJsonList<Message>(std::string(cvalue, value_len)));
    }
}

static void WFCAPI client_get_remote_message_error_callback(void *pObj, int dataType, int errorCode) {
    if(pObj) {
        GetRemoteMessageCallback *callback = (GetRemoteMessageCallback *)pObj;
        callback->onFailure(dataType, errorCode);
    }
}

static void WFCAPI client_get_messages_success_callback(void *pObj, int dataType, const char *cvalue, size_t value_len) {
    if(pObj) {
        GetMessagesCallback *callback = (GetMessagesCallback *)pObj;
        callback->onSuccess(dataType, serializableFromJsonList<Message>(std::string(cvalue, value_len)));
    }
}

static void WFCAPI client_get_messages_error_callback(void *pObj, int dataType, int errorCode) {
    if(pObj) {
        GetMessagesCallback *callback = (GetMessagesCallback *)pObj;
        callback->onFailure(dataType, errorCode);
    }
}

ChatClient* ChatClient::Instance() {
	static ChatClient INSTANCE;
	return &INSTANCE;
}

void ChatClient::setConnectionStatusListener(ConnectionStatusListener * listener)
{
	gConnectionStatusListener = listener;
}

void ChatClient::setReceiveMessageListener(ReceiveMessageListener *listener) {
	gReceiveMessageListener = listener;
}

void ChatClient::setUserInfoUpdateListener(UserInfoUpdateListener *listener) {
	gUserInfoUpdateListener = listener;
}

void ChatClient::setGroupInfoUpdateListener(GroupInfoUpdateListener *listener) {
	gGroupInfoUpdateListener = listener;
}

void ChatClient::setGroupMemberUpdateListener(GroupMemberUpdateListener *listener) {
	gGroupMemberUpdateListener = listener;
}

void ChatClient::setContactUpdateListener(ContactUpdateListener *listener) {
	gContactUpdateListener = listener;
}

void ChatClient::setFriendRequestUpdateListener(FriendRequestUpdateListener *listener) {
	gFriendRequestUpdateListener = listener;
}

void ChatClient::setUserSettingUpdateListener(UserSettingUpdateListener *listener) {
	gUserSettingUpdateListener = listener;
}

void ChatClient::setChannelInfoUpdateListener(ChannelInfoUpdateListener *listener) {
	gChannelInfoUpdateListener = listener;
}

void ChatClient::setDefaultPortraitProvider(DefaultPortraitProvider *provider)
{
    groupPortraitProvider = provider;
}

void ChatClient::registerMessage(const MessageContentPrototype *prototype)
{
    messageContentFactorys[prototype->getType()] = prototype;
		WFClient::registerMessageFlag(prototype->getType(), prototype->getFlag());
}

void ChatClient::setAppName(const std::string &appName) {
    WFClient::setAppName(appName.c_str(), appName.length());
}

const std::string ChatClient::getClientId()
{
    size_t len = 0;
    const char* str = WFClient::getClientId(&len);
    return convertDllString(str, len);
}

int64_t ChatClient::connect(const std::string & userId, const std::string &token)
{
	WFClient::setConnectionStatusListener(client_connection_callback);
	WFClient::setReceiveMessageListener(client_receive_message_callback, client_recall_message_callback, client_delete_message_callback, client_message_delivered_callback, client_message_readed_callback);
    WFClient::setUserInfoUpdateListener(client_userInfo_update_callback);
    WFClient::setGroupInfoUpdateListener(client_groupInfo_update_callback);
    WFClient::setGroupMemberUpdateListener(client_groupMembers_update_callback);
    WFClient::setFriendUpdateListener(client_friendList_update_callback);
    WFClient::setFriendRequestListener(client_friendRequest_update_callback);
    WFClient::setSettingUpdateListener(client_user_setting_update_callback);
    WFClient::setChannelInfoUpdateListener(client_channelInfo_update_callback);
    
    return WFClient::connect2Server(userId.c_str(), userId.size(), token.c_str(), token.size());
}


#if IS_QT
    ChatClient::ChatClient(QObject *parent) : QObject(parent)
#else
    ChatClient::ChatClient()
#endif
{
	registerMessage(&TextMessageContent::sPrototype);
    registerMessage(&CallStartMessageContent::sPrototype);
    registerMessage(&AddGroupMemberNotificationContent::sPrototype);
    registerMessage(&RejectJoinGroupNotificationContent::sPrototype);
    registerMessage(&CreateGroupNotificationContent::sPrototype);
    registerMessage(&DismissGroupNotificationContent::sPrototype);
    registerMessage(&ImageMessageContent::sPrototype);
    registerMessage(&VideoMessageContent::sPrototype);
    registerMessage(&StickerMessageContent::sPrototype);
    registerMessage(&KickoffGroupMemberNotificaionContent::sPrototype);
    registerMessage(&TipNotificationContent::sPrototype);
    registerMessage(&QuitGroupNotificationContent::sPrototype);
    registerMessage(&SoundMessageContent::sPrototype);
    registerMessage(&FileMessageContent::sPrototype);
    registerMessage(&PTextMessageContent::sPrototype);
    registerMessage(&TypingMessageContent::sPrototype);
    registerMessage(&RecallMessageContent::sPrototype);
    registerMessage(&LocationMessageContent::sPrototype);
    registerMessage(&MarkUnreadMessageContent::sPrototype);
    registerMessage(&TransferGroupOwnerNotificationContent::sPrototype);
    registerMessage(&ChangeGroupNameNotificationContent::sPrototype);
    registerMessage(&ChangeGroupPortraitNotificationContent::sPrototype);
    registerMessage(&ModifyGroupAliasNotificationContent::sPrototype);
    registerMessage(&GroupMuteNotificationContent::sPrototype);
    registerMessage(&GroupPrivateChatNotificationContent::sPrototype);
    registerMessage(&GroupSetManagerNotificationContent::sPrototype);
    registerMessage(&GroupJoinTypeNotificationContent::sPrototype);


#if IS_QT
    // 在main()函数或静态初始化中注册

    qRegisterMetaType<int64_t>("int64_t");
    qRegisterMetaType<ConnectionStatus>("ConnectionStatus");
    qRegisterMetaType<UserInfo>("UserInfo");
    qRegisterMetaType<Message>("Message");
    qRegisterMetaType<GroupInfo>("GroupInfo");
    qRegisterMetaType<Conversation>("Conversation");
    qRegisterMetaType<ConversationInfo>("ConversationInfo");
    qRegisterMetaType<std::string>("std::string");
    qRegisterMetaType<std::list<std::string>>("std::list<std::string>");
    qRegisterMetaType<std::list<Message>>("std::list<Message>");
    qRegisterMetaType<std::list<UserInfo>>("std::list<UserInfo>");
    qRegisterMetaType<std::list<GroupInfo>>("std::list<GroupInfo>");
    qRegisterMetaType<std::list<ChannelInfo>>("std::list<ChannelInfo>");

#endif

    groupPortraitProvider = nullptr;
}

bool ChatClient::disconnect()
{
	WFClient::disconnect(false);
	return false;
}

bool ChatClient::isLogined() {
    return WFClient::isLogin();
}

ConnectionStatus ChatClient::getConnectionStatus() {
    return (ConnectionStatus)WFClient::getConnectionStatus();
}
const std::string ChatClient::getCurrentUserId() {
    size_t len = 0;
    const char *str = WFClient::getCurrentUserId(&len);
    return convertDllString(str, len);
}

int64_t ChatClient::getServerDeltaTime() {
    return WFClient::getServerDeltaTime();
}

ChatClient::~ChatClient()
{
}

const std::string ChatClient::convertDllString(const char * pDllString, size_t len)
{
    const std::string ret(pDllString, len);
    WFClient::releaseDllString(pDllString);
    return ret;
}


MessageContent* ChatClient::getContent(const MessagePayload &payload) {
    const MessageContentPrototype *prototype = messageContentFactorys[payload.contentType];
    MessageContent *content;
    if (prototype) {
        content = prototype->productEmptyContent();
    } else {
        content = new UnknownMessageContent();
    }
    content->decode(payload);
    return content;
}

int* toArray(const std::list<int> &conversationTypes) {
    int *p = new int[conversationTypes.size()];
    int i = 0;
    for(int t : conversationTypes) {
        *(p + i) = t;
        i++;
    }

    return p;
}

const std::list<ConversationInfo> ChatClient::getConversationInfos(const std::list<int> &conversationTypes, const std::list<int> &lines) {
    int* ts = toArray(conversationTypes);
    int* ls = toArray(lines);

    size_t len = 0;
    const char* pstr = WFClient::getConversationInfos(ts, conversationTypes.size(), ls, lines.size(), &len);
    std::string str = convertDllString(pstr, len);

    delete[] ts;
    delete[] ls;
    return serializableFromJsonList<ConversationInfo>(str);
}


const ConversationInfo ChatClient::getConversationInfo(const Conversation &conversation) {
    size_t len = 0;
    const char* pstr = WFClient::getConversationInfo(conversation.conversationType, conversation.target.c_str(), conversation.target.size(), conversation.line, &len);
    std::string str = convertDllString(pstr, len);
    ConversationInfo conversationInfo;
    conversationInfo.fromJson(str);
    if(conversationInfo.conversation.target.empty()) {
        conversationInfo.conversation = conversation;
    }
    return conversationInfo;
}

const std::list<ConversationSearchInfo> ChatClient::searchConversation(const std::string &keyword, const std::list<int> &conversationTypes, const std::list<int> &lines) {
    int* cs = toArray(conversationTypes);
    int* ls = toArray(lines);

    size_t len = 0;
    const char* pstr = WFClient::searchConversation(cs, conversationTypes.size(), ls, lines.size(), keyword.c_str(), keyword.size(), &len);
    std::string str = convertDllString(pstr, len);
    delete[] cs;
    delete[] ls;

    return serializableFromJsonList<ConversationSearchInfo>(str);
}

void ChatClient::removeConversation(const Conversation &conversation,bool clearMessage) {
    WFClient::removeConversation(conversation.conversationType, conversation.target.c_str(), conversation.target.size(), conversation.line, clearMessage);
}

void ChatClient::setConversationTop(const Conversation &conversation, int top, GeneralVoidCallback *callback, int callbackPara) {
    WFClient::setConversationTop(conversation.conversationType, conversation.target.c_str(), conversation.target.size(), conversation.line, top, client_context_void_success_callback,client_genernal_void_error_callback, VoidCallbackContext::newConversationTopContext(conversation, callback, callbackPara), 0);
}

void ChatClient::setConversationSilent(const Conversation &conversation, bool silent, GeneralVoidCallback *callback, int callbackPara) {
    WFClient::setConversationSlient(conversation.conversationType, conversation.target.c_str(), conversation.target.size(), conversation.line, silent, client_context_void_success_callback, client_genernal_void_error_callback, VoidCallbackContext::newConversationSilentContext(conversation, callback, callbackPara), 0);
}

void ChatClient::setConversationDraft(const Conversation &conversation, const std::string &draft) {
    WFClient::setConversationDraft(conversation.conversationType, conversation.target.c_str(), conversation.target.size(), conversation.line, draft.c_str(), draft.size());
#if IS_QT
    emit draftUpdated(conversation, draft);
#endif
}

UnreadCount ChatClient::getUnreadCount(const std::list<int> &conversationTypes, const std::list<int> &lines) {
    int* cs = toArray(conversationTypes);
    int* ls = toArray(lines);

    size_t len = 0;
    const char* pstr = WFClient::getUnreadCount(cs, conversationTypes.size(), ls, lines.size(), &len);
    const std::string str = convertDllString(pstr, len);
    delete[] cs;
    delete[] ls;
    UnreadCount uc;
    uc.fromJson(str);
    return uc;
}

UnreadCount ChatClient::getUnreadCount(const Conversation &conversation) {
    size_t len = 0;
    const char* pstr = WFClient::getConversationUnreadCount(conversation.conversationType, conversation.target.c_str(), conversation.target.size(), conversation.line, &len);
    const std::string str = convertDllString(pstr, len);
    UnreadCount uc;
    uc.fromJson(str);
    return uc;
}

void ChatClient::clearUnreadStatus(const Conversation &conversation) {
    bool updated = WFClient::clearUnreadStatus(conversation.conversationType, conversation.target.c_str(), conversation.target.size(), conversation.line);
#if IS_QT
    if(updated)
    emit unreadStatusCleared(conversation);
#endif
}

void ChatClient::clearUnreadStatus(const std::list<int> &conversationTypes, const std::list<int> &lines) {
    int *cs = toArray(conversationTypes);
    int *ls = toArray(lines);
    bool updated = WFClient::clearUnreadStatusEx(cs, conversationTypes.size(), ls, lines.size());
    delete[] cs;
    delete[] ls;
#if IS_QT
    if(updated)
    emit unreadStatusAllCleared();
#endif
}

void ChatClient::clearAllUnreadStatus() {
    bool updated = WFClient::clearAllUnreadStatus();
#if IS_QT
    if(updated)
    emit unreadStatusAllCleared();
#endif
}

long ChatClient::getConversationFirstUnreadMessageId(const Conversation &conversation)
{
    return WFClient::getConversationFirstUnreadMessageId(conversation.conversationType, conversation.target.c_str(), conversation.target.size(), conversation.line);
}

void ChatClient::setMediaMessagePlayed(long messageId) {
    WFClient::setMediaMessagePlayed(messageId);
}

bool ChatClient::markAsUnread(const Conversation &conversation, bool sync) {
    int64_t messageUid = WFClient::setLastReceivedMessageUnRead(conversation.conversationType, conversation.target.c_str(), conversation.target.size(), conversation.line, 0, 0);
    if(sync) {
        MarkUnreadMessageContent mcontent;
        mcontent.messageUid = messageUid;
        mcontent.timestamp = getMessageByUid(messageUid).timestamp;
        sendMessage(conversation, mcontent, {getCurrentUserId()}, 0, nullptr, 0);
    }

    return messageUid > 0;
}

const std::list<Message> ChatClient::getMessages(const Conversation &conversation, const std::list<int> &contentTypes, int64_t fromIndex,int count, const std::string &user) {
    int *cs = toArray(contentTypes);

    size_t len = 0;
    const char* pstr = WFClient::getMessages(conversation.conversationType, conversation.target.c_str(), conversation.target.size(), conversation.line, cs, contentTypes.size(), fromIndex, count>0, abs(count), user.c_str(), user.size(), &len);
    std::string str = convertDllString(pstr, len);
    delete[] cs;
    return serializableFromJsonList<Message>(str);
}

void ChatClient::getMessagesV2(const Conversation &conversation, const std::list<int> &contentTypes, int64_t fromIndex, int count, const std::string &user, GetMessagesCallback *callback, int callbackPara)
{
    int *cs = toArray(contentTypes);
    WFClient::getMessagesV2(conversation.conversationType, conversation.target.c_str(), conversation.target.size(), conversation.line, cs, contentTypes.size(), fromIndex, count>0, abs(count), user.c_str(), user.size(), client_get_messages_success_callback, client_get_messages_error_callback, callback, callbackPara);
}

const std::list<Message> ChatClient::getMessages(const std::list<int> &conversationTypes, const std::list<int> &lines, const std::list<int> &contentTypes, int64_t fromIndex, int count, const std::string &user) {
    int *cs = toArray(conversationTypes);
    int *ls = toArray(lines);
    int *ts = toArray(contentTypes);

    size_t len = 0;
    const char* pstr = WFClient::getMessagesEx(cs, conversationTypes.size(), ls, lines.size(), ts, contentTypes.size(), fromIndex, count>0, abs(count), user.c_str(), user.size(), &len);
    std::string str = convertDllString(pstr, len);
    delete[] cs;
    delete[] ls;
    delete[] ts;

    return serializableFromJsonList<Message>(str);
}

const std::list<Message> ChatClient::getMessagesByMessageStatus(const std::list<int> &conversationTypes, const std::list<int> &lines, const std::list<MessageStatus> &messageStatus, int64_t fromIndex, int count, const std::string &user) {
	std::list<int> ss;
    for(WFCLib::MessageStatus s : messageStatus)
	{
		ss.push_back((int)s);
	}

    int *cs = toArray(conversationTypes);
    int *ls = toArray(lines);
    int *ts = toArray(ss);

    size_t len = 0;
    const char* pstr = WFClient::getMessagesByMessageStatus(cs, conversationTypes.size(), ls, lines.size(), ts, ss.size(), fromIndex, count>0, abs(count), user.c_str(), user.size(), &len);
    std::string str = convertDllString(pstr, len);
    delete[] cs;
    delete[] ls;
    delete[] ts;

    return serializableFromJsonList<Message>(str);
}

void ChatClient::getRemoteMessages(const Conversation &conversation, const std::list<int> &contentTypes, int64_t beforeMessageUid, int count, GetRemoteMessageCallback *callback, int callbackPara) {
    int *ts = toArray(contentTypes);
    WFClient::getRemoteMessages(conversation.conversationType, conversation.target.c_str(), conversation.target.size(), conversation.line, ts, contentTypes.size(), beforeMessageUid, count, client_get_remote_message_success_callback, client_get_remote_message_error_callback, callback, callbackPara);
    delete[] ts;
}

const Message ChatClient::getMessage(long messageId) {
    size_t len = 0;
    const char* pstr = WFClient::getMessage(messageId, &len);
    std::string str = convertDllString(pstr, len);
    Message message;
    message.fromJson(str);
    return message;
}

const Message ChatClient::getMessageByUid(int64_t messageUid) {
    size_t len = 0;
    const char* pstr = WFClient::getMessageByUid(messageUid, &len);
    std::string str = convertDllString(pstr, len);
    Message message;
    message.fromJson(str);
    return message;
}

const std::list<Message> ChatClient::searchMessage(const Conversation &conversation, const std::string &keyword, bool desc, int limit, int offset, const std::string &withUser) {
    size_t len = 0;
    const char* pstr = WFClient::searchMessage(conversation.conversationType, conversation.target.c_str(), conversation.target.size(), conversation.line, keyword.c_str(), keyword.size(), desc, limit, offset, withUser.c_str(), withUser.size(), &len);
    std::string str = convertDllString(pstr, len);
    return serializableFromJsonList<Message>(str);
}

void WFCAPI client_sendMessage_success_callback(void *pObject, int dataType, long messageId, int64_t messageUid, int64_t timestamp) {
    ChatClient::Instance()->onMessageSendSuccess(messageId, messageUid, timestamp);
    if (pObject) {
        WFSendMessageCallback *callback = (WFSendMessageCallback *)pObject;
        callback->onSuccess(dataType, messageUid, timestamp);
    }
}

void WFCAPI client_sendMessage_prepared_callback(void *pObject, int dataType, long messageId, int64_t timestamp) {
    ChatClient::Instance()->onMessageSendPrepared(messageId, timestamp);
	if (pObject) {
		WFSendMessageCallback *callback = (WFSendMessageCallback *)pObject;
        callback->onPrepared(dataType, messageId, timestamp);
	}
}

void WFCAPI client_sendMessage_progress_callback(void *pObject, int dataType, long messageId, int uploaded, int total) {
    ChatClient::Instance()->onMessageSendProgress(messageId, uploaded, total);
    if (pObject) {
        WFSendMessageCallback *callback = (WFSendMessageCallback *)pObject;
        callback->onProgress(dataType, uploaded, total);
    }
}

void WFCAPI client_sendMessage_media_uploaded_callback(void *pObject, int dataType, long messageId, const char *cremoteUrl, size_t remoteUrl_len) {
    std::string remoteUrl(cremoteUrl, remoteUrl_len);
    ChatClient::Instance()->onMessageSendUploaded(messageId, remoteUrl);
	if (pObject) {
		WFSendMessageCallback *callback = (WFSendMessageCallback *)pObject;
        callback->onUploaded(dataType, remoteUrl);
	}
}

void WFCAPI client_sendMessage_error_callback(void *pObject, int dataType, long messageId, int errorCode) {
    ChatClient::Instance()->onMessageSendFailure(messageId, errorCode);
    if (pObject) {
        WFSendMessageCallback *callback = (WFSendMessageCallback *)pObject;
        callback->onFailure(dataType, errorCode);
    }
}

static size_t toStringArray(const std::list<std::string> &groupMembers, size_t **lengths, const char*** users) {
    if(groupMembers.empty()) {
        *users = NULL;
        *lengths = NULL;
        return 0;
    }
    *users = new const char*[groupMembers.size()];
    *lengths = new size_t[groupMembers.size()];
    int len = groupMembers.size();

    int i = 0;
    foreach (auto &user, groupMembers) {
        *((*users) + i) = new char[user.size()];
        memcpy((void *)(*((*users)+i)), user.c_str(), user.size());
        *((*lengths)+i) = user.size();
        i++;
    }
    return len;
}

static void freeStringArray(size_t *lengths, const char** users, size_t len) {
    if(len <= 0) {
        return;
    }

    for(size_t i = 0; i < len; i++) {
        delete[] users[i];
    }
    delete[] users;
    delete[] lengths;
}

const Message ChatClient::sendMessage(const Conversation &conversation, const MessageContent &content, const std::list<std::string> &toUsers, int expireDuration, WFSendMessageCallback *callback, int callbackPara) {
    std::string cntStr = content.encode().toJson();

    const char** users;
    size_t *lengths;
    int len = toStringArray(toUsers, &lengths, &users);


    size_t ln = 0;
    const char* pstr = WFClient::sendMessage(conversation.conversationType, conversation.target.c_str(), conversation.target.size(), conversation.line, cntStr.c_str(), cntStr.size(), users, lengths, len, expireDuration, client_sendMessage_success_callback, client_sendMessage_error_callback, client_sendMessage_prepared_callback, client_sendMessage_progress_callback, client_sendMessage_media_uploaded_callback, callback, callbackPara, &ln);
    std::string str = convertDllString(pstr, ln);
    freeStringArray(lengths, users, len);
    Message message;
    message.fromJson(str);
    return message;
}

void ChatClient::recallMessage(int64_t messageUid, GeneralVoidCallback *callback, int callbackPara) {
    WFClient::recallMessage(messageUid, client_context_void_success_callback, client_genernal_void_error_callback, VoidCallbackContext::newRecallContext(messageUid, callback, callbackPara), 0);
}

void WFCAPI client_uploadMedia_success_callback(void *pObject, int callbackPara, const char *cremoteUrl, size_t remoteUrl_len) {
    if (pObject) {
        UploadMediaCallback *callback = (UploadMediaCallback *)pObject;
        callback->onSuccess(callbackPara, std::string(cremoteUrl, remoteUrl_len));
    }
}

void WFCAPI client_uploadMedia_progress_callback(void *pObject, int callbackPara, int uploaded, int total) {
    if (pObject) {
        UploadMediaCallback *callback = (UploadMediaCallback *)pObject;
        callback->onProgress(callbackPara, uploaded, total);
    }
}

void WFCAPI client_uploadMedia_error_callback(void *pObject, int callbackPara, int errorCode) {
    if (pObject) {
        UploadMediaCallback *callback = (UploadMediaCallback *)pObject;
        callback->onFailure(callbackPara, errorCode);
    }
}

void ChatClient::uploadMedia(const std::string &fileName, const std::string mediaData, MediaType mediaType, UploadMediaCallback *callback, int callbackPara) {
    WFClient::uploadMedia(fileName.c_str(), fileName.size(), mediaData.c_str(), (int)mediaData.length(), mediaType, client_uploadMedia_success_callback, client_uploadMedia_error_callback, client_uploadMedia_progress_callback, callback, callbackPara);
}

bool ChatClient::deleteMessage(long messageId) {
    return WFClient::deleteMessage(messageId);
}


void ChatClient::clearMessages(const Conversation &conversation) {
    WFClient::clearMessages(conversation.conversationType, conversation.target.c_str(), conversation.target.size(), conversation.line);
#if IS_QT
    emit conversationMessageCleared(conversation, 0);
#endif
}

void ChatClient::clearMessages(const Conversation &conversation, int64_t before) {
    WFClient::clearMessagesBefore(conversation.conversationType, conversation.target.c_str(), conversation.target.size(),  conversation.line, before);
#if IS_QT
    emit conversationMessageCleared(conversation, before);
#endif
}

const Message ChatClient::insert(const Conversation &conversation, const std::string &sender, const MessageContent &content, MessageStatus status, bool notify, int64_t serverTime) {
    std::string contentStr = content.encode().toJson();

    size_t len = 0;
    const char* pstr = WFClient::insertMessage(conversation.conversationType, conversation.target.c_str(), conversation.target.size(),  conversation.line, sender.c_str(), sender.size(), contentStr.c_str(), contentStr.size(), status, notify, serverTime, &len);
    std::string str = convertDllString(pstr, len);
    Message message;
    message.fromJson(str);
    return message;
}

void ChatClient::updateMessage(long messageId, const MessageContent &content) {
    std::string str = content.encode().toJson();
    WFClient::updateMessage(messageId, str.c_str(), str.size());
}

const UserInfo ChatClient::getUserInfo(const std::string &userId, bool refresh) {
    return getUserInfo(userId, "", refresh);
}

const UserInfo ChatClient::getUserInfo(const std::string &userId, const std::string &groupId, bool refresh) {
    size_t len = 0;
    const char* pstr = WFClient::getUserInfo(userId.c_str(), userId.size(), refresh, groupId.c_str(), groupId.size(), &len);
    std::string str = convertDllString(pstr, len);
    UserInfo userInfo;
    userInfo.fromJson(str);
    return userInfo;
}

const std::list<UserInfo> ChatClient::getUserInfos(std::list<std::string> &toUsers, const std::string &groupId) {
    const char** users;
    size_t *lengths;
    int len = toStringArray(toUsers, &lengths, &users);

    size_t ln = 0;
    const char* pstr = WFClient::getUserInfos(users, lengths, len, groupId.c_str(), groupId.size(), &ln);
    std::string str = convertDllString(pstr, ln);
    freeStringArray(lengths, users, len);

    return serializableFromJsonList<UserInfo>(str);
}

static void WFCAPI client_searchUser_success_callback(void *pObj, int callbackPara, const char *cvalue, size_t value_len) {
    if (pObj) {
        SearchUserCallback *callback = (SearchUserCallback *)pObj;
        callback->onSuccess(callbackPara, serializableFromJsonList<UserInfo>(std::string(cvalue, value_len)));
    }
}

static void WFCAPI client_searchUser_error_callback(void *pObj, int callbackPara, int errorCode) {
    if (pObj) {
        SearchUserCallback *callback = (SearchUserCallback *)pObj;
        callback->onFailure(callbackPara, errorCode);
    }
}

void ChatClient::searchUser(const std::string &keyword, SearchUserType searchType, int page, SearchUserCallback *callback, int callbackPara) {
    WFClient::searchUser(keyword.c_str(), keyword.size(), searchType, page, client_searchUser_success_callback, client_searchUser_error_callback, callback, callbackPara);
}

bool ChatClient::isMyFriend(const std::string &userId) {
    return WFClient::isMyFriend(userId.c_str(), userId.size());
}

const std::list<std::string> ChatClient::getMyFriendList(bool refresh) {
    size_t len = 0;
    const char* pstr = WFClient::getMyFriendList(refresh, &len);
    std::string str = convertDllString(pstr, len);
    return parseStringList(str);
}

const std::list<UserInfo> ChatClient::searchFriends(const std::string &keyword) {
    size_t len = 0;
    const char* pstr = WFClient::searchFriends(keyword.c_str(), keyword.size(), &len);
    std::string str = convertDllString(pstr, len);
    return serializableFromJsonList<UserInfo>(str);
}


const std::list<GroupSearchInfo> ChatClient::searchGroups(const std::string &keyword) {
    size_t len = 0;
    const char* pstr = WFClient::searchGroups(keyword.c_str(), keyword.size(), &len);
    std::string str = convertDllString(pstr, len);
    return serializableFromJsonList<GroupSearchInfo>(str);
}

const std::list<FriendRequest> ChatClient::getIncommingFriendRequest() {
    size_t len = 0;
    const char* pstr = WFClient::getIncommingFriendRequest(&len);
    std::string str = convertDllString(pstr, len);
    return serializableFromJsonList<FriendRequest>(str);
}

const std::list<FriendRequest> ChatClient::getOutgoingFriendRequest() {
    size_t len = 0;
    const char* pstr = WFClient::getOutgoingFriendRequest(&len);
    std::string str = convertDllString(pstr, len);
    return serializableFromJsonList<FriendRequest>(str);
}

const FriendRequest ChatClient::getFriendRequest(const std::string &userId, bool incomming) {
    size_t len = 0;
    const char* pstr = WFClient::getFriendRequest(userId.c_str(), userId.length(), incomming, &len);
    std::string str = convertDllString(pstr, len);
    FriendRequest fr;
    fr.fromJson(str);
    return fr;
}

void ChatClient::loadFriendRequestFromRemote() {
    WFClient::loadFriendRequestFromRemote();
}

int ChatClient::getUnreadFriendRequestStatus() {
    return WFClient::getUnreadFriendRequestStatus();
}

void ChatClient::clearUnreadFriendRequestStatus() {
    WFClient::clearUnreadFriendRequestStatus();
}

void ChatClient::deleteFriend(const std::string &userId, GeneralVoidCallback *callback, int callbackPara) {
    WFClient::deleteFriend(userId.c_str(), userId.size(), client_context_void_success_callback, client_genernal_void_error_callback, VoidCallbackContext::newStringContext(userId, VoidCallbackContext::Type::DELETE_FRIEND_USER_ID, callback, callbackPara), 0);
}

void ChatClient::sendFriendRequest(const std::string &userId, const std::string &reason, const std::string &extra, GeneralVoidCallback *callback, int callbackPara) {
    WFClient::sendFriendRequest(userId.c_str(), userId.size(), reason.c_str(), reason.size(), extra.c_str(), extra.size(), client_context_void_success_callback, client_genernal_void_error_callback, VoidCallbackContext::newStringContext(userId, VoidCallbackContext::Type::SEND_FRIEND_REQUEST_USER_ID, callback, callbackPara), 0);
}

void ChatClient::handleFriendRequest(const std::string &userId, bool accpet, const std::string &friendExtra, GeneralVoidCallback *callback, int callbackPara) {
    WFClient::handleFriendRequest(userId.c_str(), userId.size(), accpet, friendExtra.c_str(), friendExtra.size(), client_context_void_success_callback, client_genernal_void_error_callback, VoidCallbackContext::newStringContext(userId, VoidCallbackContext::Type::HANDLE_FRIEND_REQUEST_USER_ID, callback, callbackPara), 0);
}

const std::string ChatClient::getFriendAlias(const std::string &userId) {
    size_t len = 0;
    const char* pstr = WFClient::getFriendAlias(userId.c_str(), userId.size(), &len);
    return convertDllString(pstr, len);
}

void ChatClient::setFriend(const std::string &userId, const std::string &alias, GeneralVoidCallback *callback, int callbackPara) {
    WFClient::setFriendAlias(userId.c_str(), userId.size(), alias.c_str(), alias.size(), client_context_void_success_callback, client_genernal_void_error_callback, VoidCallbackContext::newStringContext(userId, VoidCallbackContext::Type::SET_FRIEND_ALIAS_USER_ID, callback, callbackPara), 0);
}

bool ChatClient::isBlackListed(const std::string &userId) {
    return WFClient::isBlackListed(userId.c_str(), userId.size());
}

const std::list<std::string> ChatClient::getBlackList(bool refresh) {
    size_t len = 0;
    const char* pstr = WFClient::getBlackList(refresh, &len);
    std::string str = convertDllString(pstr, len);
    return parseStringList(str);
}

void ChatClient::setBlackList(const std::string &userId, bool isBlackListed, GeneralVoidCallback *callback, int callbackPara) {
    WFClient::setBlackList(userId.c_str(), userId.size(), isBlackListed, client_context_void_success_callback, client_genernal_void_error_callback, VoidCallbackContext::newStringContext(userId, VoidCallbackContext::Type::SET_FRIEND_BLACKLIST_USER_ID, callback, callbackPara), 0);
}

const std::list<GroupMember> ChatClient::getGroupMembers(const std::string &groupId, bool forceUpdate) {
    size_t len = 0;
    const char* pstr = WFClient::getGroupMembers(groupId.c_str(), groupId.size(), forceUpdate, &len);
    std::string str = convertDllString(pstr, len);
    return serializableFromJsonList<GroupMember>(str);
}

const std::list<GroupMember> ChatClient::getGroupMembersByType(const std::string &groupId, int memberType) {
    size_t len = 0;
    const char* pstr = WFClient::getGroupMembersByType(groupId.c_str(), groupId.size(), memberType, &len);
    std::string str = convertDllString(pstr,len);
    return serializableFromJsonList<GroupMember>(str);
}

const std::list<GroupMember> ChatClient::getGroupMembersByCount(const std::string &groupId, int count) {
    size_t len = 0;
    const char* pstr = WFClient::getGroupMembersByCount(groupId.c_str(), groupId.size(), count, &len);
    std::string str = convertDllString(pstr, len);
    return serializableFromJsonList<GroupMember>(str);
}

const GroupInfo ChatClient::getGroupInfo(const std::string &groupId, bool refresh) {
    size_t len = 0;
    const char* pstr = WFClient::getGroupInfo(groupId.c_str(), groupId.size(), refresh, &len);
    std::string str = convertDllString(pstr, len);
    GroupInfo groupInfo;
    groupInfo.fromJson(str);
    if(groupInfo.portrait.empty() && groupPortraitProvider) {
        std::list<GroupMember> members = getGroupMembersByCount(groupId, 9);
        std::list<UserInfo> userInfos;
        foreach (auto member, members) {
            auto userInfo = getUserInfo(member.memberId);
            userInfos.push_back(userInfo);
        }
        std::string portrait = groupPortraitProvider->groupDefaultPortrait(groupInfo, userInfos);
        groupInfo.portrait = portrait;
    }
    return groupInfo;
}

const GroupMember ChatClient::getGroupMember(const std::string &groupId, const std::string &memberId) {
    size_t len = 0;
    const char* pstr = WFClient::getGroupMember(groupId.c_str(), groupId.size(), memberId.c_str(), memberId.size(), &len);
   std::string str = convertDllString(pstr, len);
   GroupMember member;
   member.fromJson(str);
   return member;
}

void ChatClient::createGroup(const std::string &groupId, const std::string &groupName, const std::string &groupPortrait, GroupType type, const std::string &groupExtra, const std::list<std::string> &groupMembers, const std::string &memberExtra, const std::list<int> &notifyLines, GeneralStringCallback *callback, int callbackPara) {
    const char** users;
    size_t *lengths;
    int len = toStringArray(groupMembers, &lengths, &users);
    int *ls = toArray(notifyLines);
    std::string strCont;

    WFClient::createGroup(groupId.c_str(), groupId.size(), type, groupName.c_str(), groupName.size(), groupPortrait.c_str(), groupPortrait.size(), groupExtra.c_str(), groupExtra.size(), users, lengths, len, memberExtra.c_str(), memberExtra.size(), ls, notifyLines.size(), strCont.c_str(), strCont.size(), client_genernal_string_success_callback, client_genernal_string_error_callback, callback, callbackPara);

    freeStringArray(lengths, users, len);
    delete[] ls;
}


void ChatClient::addMembers(const std::list<std::string> &groupMembers, const std::string &groupId, const std::string &memberExtra, const std::list<int> &notifyLines, GeneralVoidCallback *callback, int callbackPara) {
    const char** users;
    size_t *lengths;
    int len = toStringArray(groupMembers, &lengths, &users);
    int *ls = toArray(notifyLines);

    std::string strCont;
    WFClient::addMembers(groupId.c_str(), groupId.size(), users, lengths, len, memberExtra.c_str(), memberExtra.size(), ls, notifyLines.size(), strCont.c_str(), strCont.size(), client_context_void_success_callback, client_genernal_void_error_callback, VoidCallbackContext::newStringContext(groupId, VoidCallbackContext::Type::ADD_GROUP_MEMBER_GROUP_ID, callback, callbackPara), 0);

    freeStringArray(lengths, users, len);
    delete[] ls;
}

void ChatClient::kickoffMembers(const std::list<std::string> &groupMembers, const std::string &groupId, const std::list<int> &notifyLines, GeneralVoidCallback *callback, int callbackPara) {
    const char** users;
    size_t *lengths;
    int len = toStringArray(groupMembers, &lengths, &users);
    int *ls = toArray(notifyLines);

    std::string strCont;
    WFClient::kickoffMembers(groupId.c_str(), groupId.size(), users, lengths, len, ls, notifyLines.size(), strCont.c_str(), strCont.size(), client_context_void_success_callback, client_genernal_void_error_callback, VoidCallbackContext::newStringContext(groupId, VoidCallbackContext::Type::KICKOFF_GROUP_MEMBER_GROUP_ID, callback, callbackPara), 0);

    freeStringArray(lengths, users, len);
    delete[] ls;
}

void ChatClient::quitGroup(const std::string &groupId, const std::list<int> &notifyLines, GeneralVoidCallback *callback, int callbackPara) {
    int *ls = toArray(notifyLines);
    std::string strCont;

    WFClient::quitGroup(groupId.c_str(), groupId.size(), ls, notifyLines.size(), strCont.c_str(), strCont.size(), client_context_void_success_callback, client_genernal_void_error_callback, VoidCallbackContext::newStringContext(groupId, VoidCallbackContext::Type::QUIT_GROUP_ID, callback, callbackPara), 0);
    delete[] ls;
}

void ChatClient::dismissGroup(const std::string &groupId, const std::list<int> &notifyLines, GeneralVoidCallback *callback, int callbackPara) {
    int *ls = toArray(notifyLines);
    std::string strCont;

    WFClient::dismissGroup(groupId.c_str(), groupId.size(), ls, notifyLines.size(), strCont.c_str(), strCont.size(), client_context_void_success_callback, client_genernal_void_error_callback, VoidCallbackContext::newStringContext(groupId, VoidCallbackContext::Type::DISMISS_GROUP_ID, callback, callbackPara), 0);
    delete[] ls;
}

void ChatClient::modifyGroupInfo(const std::string &groupId, ModifyGroupInfoType type, const std::string &newValue, const std::list<int> &notifyLines, GeneralVoidCallback *callback, int callbackPara) {
    int *ls = toArray(notifyLines);
    std::string strCont;

    WFClient::modifyGroupInfo(groupId.c_str(), groupId.size(), type, newValue.c_str(), newValue.size(), ls, notifyLines.size(), strCont.c_str(), strCont.size(), client_context_void_success_callback, client_genernal_void_error_callback, VoidCallbackContext::newStringContext(groupId, VoidCallbackContext::Type::MODIFY_GROUP_INFO_ID, callback, callbackPara), 0);
    delete[] ls;
}

void ChatClient::modifyGroupAlias(const std::string &groupId, const std::string &newAlias, const std::list<int> &notifyLines, GeneralVoidCallback *callback, int callbackPara) {
    int *ls = toArray(notifyLines);
    std::string strCont;

    WFClient::modifyGroupAlias(groupId.c_str(), groupId.size(), newAlias.c_str(), newAlias.size(), ls, notifyLines.size(), strCont.c_str(), strCont.size(), client_context_void_success_callback, client_genernal_void_error_callback, VoidCallbackContext::newStringContext(groupId, VoidCallbackContext::Type::MODIFY_GROUP_ALIAS_ID, callback, callbackPara), 0);
    delete[] ls;
}

void ChatClient::transferGroup(const std::string &groupId, const std::string &newOwner, const std::list<int> &notifyLines,  GeneralVoidCallback *callback, int callbackPara) {
    int *ls = toArray(notifyLines);
    std::string strCont;

    WFClient::transferGroup(groupId.c_str(), groupId.size(), newOwner.c_str(), newOwner.size(), ls, notifyLines.size(), strCont.c_str(), strCont.size(), client_context_void_success_callback, client_genernal_void_error_callback, VoidCallbackContext::newStringContext(groupId, VoidCallbackContext::Type::TRANSFER_GROUP_ID, callback, callbackPara), 0);
    delete[] ls;
}

void ChatClient::setGroupManager(const std::string &groupId, bool isSet, const std::list<std::string> &memberIds, const std::list<int> &notifyLines, GeneralVoidCallback *callback, int callbackPara) {
    const char** users;
    size_t *lengths;
    int len = toStringArray(memberIds, &lengths, &users);
    int *ls = toArray(notifyLines);
    std::string strCont;

    WFClient::setGroupManager(groupId.c_str(), groupId.size(), isSet, users, lengths, len, ls, notifyLines.size(), strCont.c_str(), strCont.size(), client_context_void_success_callback, client_genernal_void_error_callback, VoidCallbackContext::newStringContext(groupId, VoidCallbackContext::Type::SET_MANAGER_GROUP_ID, callback, callbackPara), 0);
    freeStringArray(lengths, users, len);
    delete[] ls;
}

const std::list<std::string> ChatClient::getFavGroups() {
    size_t len = 0;
    const char* pstr = WFClient::getFavGroups(&len);
    std::string str = convertDllString(pstr, len);
    return parseStringList(str);
}

bool ChatClient::isFavGroup(const std::string &groupId) {
    return WFClient::isFavGroup(groupId.c_str(), groupId.size());
}

void ChatClient::setFavGroup(const std::string &groupId, bool fav, GeneralVoidCallback *callback, int callbackPara) {
    WFClient::setFavGroup(groupId.c_str(), groupId.size(), fav, client_context_void_success_callback, client_genernal_void_error_callback, VoidCallbackContext::newStringContext(groupId, VoidCallbackContext::Type::SET_FAV_GROUP_ID, callback, callbackPara), 0);
}

const std::string ChatClient::getUserSetting(UserSettingScope scope, const std::string &key) {
    size_t len = 0;
    const char* pstr = WFClient::getUserSetting(scope, key.c_str(), key.size(), &len);
    return convertDllString(pstr, len);
}

std::map<std::string, std::string> convertStr2MapList(const std::string &str) {
    std::map<std::string, std::string> strMap;

    Document document;
    if (document.Parse(str).HasParseError()) {
        printf("\nParsing to document failure(%s).\n", str.c_str());
        return strMap;
    }

    if (document.IsArray()) {
        for (int i = 0; i < document.Size(); i++) {
            const Value& object = document[i];
            std::string key;
            std::string value;
            getValue(object, "key", key);
            getValue(object, "value", value);
            strMap[key] = value;
        }
    }

    return strMap;
}

const std::map<std::string, std::string> ChatClient::getUserSettings(UserSettingScope scope) {
    size_t len = 0;
    const char* pstr = WFClient::getUserSettings(scope, &len);
    std::string str = convertDllString(pstr, len);
    return convertStr2MapList(str);
}

void ChatClient::setUserSetting(UserSettingScope scope, const std::string &key, const std::string &value, GeneralVoidCallback *callback, int callbackPara) {
    WFClient::setUserSetting(scope, key.c_str(), key.size(), value.c_str(), value.size(), client_context_void_success_callback, client_genernal_void_error_callback, VoidCallbackContext::newUserSettingContext(scope, key, value, callback, callbackPara), 0);
}

void ChatClient::modifyMyInfo(int type, const std::string &value, GeneralVoidCallback *callback, int callbackPara) {
    WFClient::modifyMyInfo(type, value.c_str(), value.size(), client_context_void_success_callback, client_genernal_void_error_callback, VoidCallbackContext::newVoidContext(VoidCallbackContext::Type::MODIFY_MY_INFO, callback, callbackPara), 0);
}

bool ChatClient::isGlobalSilent() {
    return WFClient::isGlobalSilent();
}

void ChatClient::setGlobalSilent(bool slient, GeneralVoidCallback *callback, int callbackPara) {
    WFClient::setGlobalSilent(slient, client_context_void_success_callback, client_genernal_void_error_callback, VoidCallbackContext::newVoidContext(VoidCallbackContext::Type::SET_GLOBAL_SILENT, callback, callbackPara), 0);
}

bool ChatClient::isHiddenNotificationDetail() {
    return WFClient::isHiddenNotificationDetail();
}

void ChatClient::setHiddenNotificationDetail(bool hidden, GeneralVoidCallback *callback, int callbackPara) {
    WFClient::setHiddenNotificationDetail(hidden, client_context_void_success_callback, client_genernal_void_error_callback, VoidCallbackContext::newVoidContext(VoidCallbackContext::Type::SET_HIDDEN_NOTIFI_DETAIL, callback, callbackPara), 0);
}

bool ChatClient::isHiddenGroupMemberName(const std::string &groupId) {
    return WFClient::isHiddenGroupMemberName(groupId.c_str(), groupId.length());
}

void ChatClient::setHiddenGroupMemberName(bool hidden, const std::string &groupId, GeneralVoidCallback *callback, int callbackPara) {
    WFClient::setHiddenGroupMemberName(groupId.c_str(), groupId.size(), hidden, client_context_void_success_callback, client_genernal_void_error_callback, VoidCallbackContext::newStringContext(groupId, VoidCallbackContext::Type::SET_HIDDEN_GROUP_ALIAS, callback, callbackPara), 0);
}

void ChatClient::joinChatroom(const std::string &chatroomId, GeneralVoidCallback *callback, int callbackPara) {
    WFClient::joinChatroom(chatroomId.c_str(), chatroomId.size(), client_genernal_void_success_callback, client_genernal_void_error_callback, callback, callbackPara);
}

void ChatClient::quitChatroom(const std::string &chatroomId, GeneralVoidCallback *callback, int callbackPara) {
    WFClient::quitChatroom(chatroomId.c_str(), chatroomId.size(), client_genernal_void_success_callback, client_genernal_void_error_callback, callback, callbackPara);
}

static void WFCAPI client_get_chatroomInfo_success_callback(void *pObj, int callbackPara, const char *cvalue, size_t value_len) {
    if (pObj) {
        GetChatroomInfoCallback *callback = (GetChatroomInfoCallback *)pObj;
        ChatroomInfo info;
        info.fromJson(std::string(cvalue, value_len));
        callback->onSuccess(callbackPara, info);
    }
}

static void WFCAPI client_get_chatroomInfo_error_callback(void *pObj, int callbackPara, int errorCode) {
    if(pObj) {
        GetChatroomInfoCallback *callback = (GetChatroomInfoCallback *)pObj;
        callback->onFailure(callbackPara, errorCode);
    }
}

void ChatClient::getChatroomInfo(const std::string &chatroomId, int64_t updateDt, GetChatroomInfoCallback *callback, int callbackPara) {
    WFClient::getChatroomInfo(chatroomId.c_str(), chatroomId.size(), updateDt, client_get_chatroomInfo_success_callback, client_get_chatroomInfo_error_callback, callback, callbackPara);
}

static void WFCAPI client_get_chatroomMemberInfo_success_callback(void *pObj, int callbackPara, const char *cvalue, size_t value_len) {
    if (pObj) {
        GetChatroomMemberInfoCallback *callback = (GetChatroomMemberInfoCallback *)pObj;
        ChatroomMemberInfo info;
        info.fromJson(std::string(cvalue, value_len));
        callback->onSuccess(callbackPara, info);
    }
}

static void WFCAPI client_get_chatroomMemberInfo_error_callback(void *pObj, int callbackPara, int errorCode) {
    if(pObj) {
        GetChatroomMemberInfoCallback *callback = (GetChatroomMemberInfoCallback *)pObj;
        callback->onFailure(callbackPara, errorCode);
    }
}


void ChatClient::getChatroomMemberInfo(const std::string &chatroomId, int maxCount, GetChatroomMemberInfoCallback *callback, int callbackPara) {
    WFClient::getChatroomMemberInfo(chatroomId.c_str(), chatroomId.size(), maxCount, client_get_chatroomMemberInfo_success_callback, client_get_chatroomMemberInfo_error_callback, callback, callbackPara);
}


static void WFCAPI client_get_channelInfo_success_callback(void *pObj, int callbackPara, const char *cvalue, size_t value_len) {
    if (pObj) {
        GetChannelInfoCallback *callback = (GetChannelInfoCallback *)pObj;
        ChannelInfo info;
        info.fromJson(std::string(cvalue, value_len));
        callback->onSuccess(callbackPara, info);
    }
}

static void WFCAPI client_get_channelInfo_error_callback(void *pObj, int callbackPara, int errorCode) {
    if(pObj) {
        GetChannelInfoCallback *callback = (GetChannelInfoCallback *)pObj;
        callback->onFailure(callbackPara, errorCode);
    }
}

void ChatClient::createChannel(const std::string &channelName, const std::string &channelPortrait, const std::string &desc, const std::string &extra, GetChannelInfoCallback *callback, int callbackPara) {
    WFClient::createChannel(channelName.c_str(), channelName.size(), channelPortrait.c_str(), channelPortrait.size(), desc.c_str(), desc.size(), extra.c_str(), extra.size(), client_get_channelInfo_success_callback, client_get_channelInfo_error_callback, callback, callbackPara);
}

ChannelInfo ChatClient::getChannelInfo(const std::string &channelId, bool refresh) {
    size_t len = 0;
    const char* pstr = WFClient::getChannelInfo(channelId.c_str(), channelId.size(), refresh, &len);
    std::string str = convertDllString(pstr, len);
    ChannelInfo channelInfo;
    channelInfo.fromJson(str);
    return channelInfo;
}

void ChatClient::modifyChannelInfo(const std::string &channelId, ModifyChannelInfoType type, const std::string &newValue, GeneralVoidCallback *callback, int callbackPara) {
    WFClient::modifyChannelInfo(channelId.c_str(), channelId.size(), type, newValue.c_str(), newValue.size(), client_context_void_success_callback, client_genernal_void_error_callback, VoidCallbackContext::newVoidContext(VoidCallbackContext::Type::MODIFY_CHANNEL_ID, callback, callbackPara), 0);
}

static void WFCAPI client_search_channel_success_callback(void *pObj, int callbackPara, const char *cvalue, size_t value_len) {
    if (pObj) {
        SearchChannelCallback *callback = (SearchChannelCallback *)pObj;
        callback->onSuccess(callbackPara, serializableFromJsonList<ChannelInfo>(std::string(cvalue, value_len)));
    }
}

static void WFCAPI client_search_channel_error_callback(void *pObj, int callbackPara, int errorCode) {
    if(pObj) {
        SearchChannelCallback *callback = (SearchChannelCallback *)pObj;
        callback->onFailure(callbackPara, errorCode);
    }
}

void ChatClient::searchChannel(const std::string &keyword, SearchChannelCallback *callback, int callbackPara) {
    WFClient::searchChannel(keyword.c_str(), keyword.size(), client_search_channel_success_callback, client_search_channel_error_callback, callback, callbackPara);
}

bool ChatClient::isListenedChannel(const std::string &channelId) {
    return WFClient::isListenedChannel(channelId.c_str(), channelId.size());
}

void ChatClient::listenChannel(const std::string &channelId, bool listen, GeneralVoidCallback *callback, int callbackPara) {
    WFClient::listenChannel(channelId.c_str(), channelId.size(), listen, client_context_void_success_callback, client_genernal_void_error_callback, VoidCallbackContext::newStringContext(channelId, VoidCallbackContext::Type::LISTEN_CHANNEL_ID, callback, callbackPara), 0);
}

const std::list<std::string> ChatClient::getMyChannels() {
    size_t len = 0;
    const char* pstr = WFClient::getMyChannels(&len);
    std::string str = convertDllString(pstr, len);
    return parseStringList(str);
}

const std::list<std::string> ChatClient::getListenedChannels() {
    size_t len = 0;
    const char* pstr = WFClient::getListenedChannels(&len);
    std::string str = convertDllString(pstr, len);
    return parseStringList(str);
}

void ChatClient::destoryChannel(const std::string &channelId, GeneralVoidCallback *callback, int callbackPara) {
    WFClient::destoryChannel(channelId.c_str(), channelId.size(), client_context_void_success_callback, client_genernal_void_error_callback, VoidCallbackContext::newStringContext(channelId, VoidCallbackContext::Type::DESTROY_CHANNEL_ID, callback, callbackPara), 0);
}

void ChatClient::getAuthorizedMediaUrl(long long messageId, int mediaType, const std::string &mediaPath, GeneralStringCallback *callback, int callbackPara) {
    WFClient::getAuthorizedMediaUrl(messageId, mediaType, mediaPath.c_str(), mediaPath.size(), client_genernal_string_success_callback, client_genernal_string_error_callback, callback, callbackPara);
}

void ChatClient::onConnectionStatusChanged(ConnectionStatus status) {
#if IS_QT
    emit connectionStatusChanged(status);
#endif

    if (gConnectionStatusListener) {
        gConnectionStatusListener->onConnectionStatusChanged((ConnectionStatus)status);
    }
}
void ChatClient::onReceiveMessages(const std::list<Message> &messages, bool hasMore) {
#if IS_QT
    emit receiveMessages(messages, hasMore);
#endif

    foreach (auto msg, messages) {
        if(msg.content->getPrototype()->getType() == MESSAGE_CONTENT_TYPE_MARK_UNREAD_SYNC) {
            if (MarkUnreadMessageContent* mark = dynamic_cast<MarkUnreadMessageContent*>(msg.content)) {
                WFClient::setLastReceivedMessageUnRead(msg.conversation.conversationType, msg.conversation.target.c_str(), msg.conversation.target.size(),  msg.conversation.line, mark->messageUid, mark->timestamp);
            }
        }
    }

    if (gReceiveMessageListener) {
        gReceiveMessageListener->onReceiveMessages(messages, hasMore);
    }
}

void ChatClient::onRecallMessage(const std::string &operatorId, int64_t messageUid) {
#if IS_QT
    emit recalledMessage(operatorId, messageUid);
#endif
    if (gReceiveMessageListener) {
        gReceiveMessageListener->onRecallMessage(operatorId, messageUid);
    }
}
void ChatClient::onDeleteMessage(int64_t messageUid) {
#if IS_QT
    emit deletedMessage(messageUid);
#endif
    if (gReceiveMessageListener) {
        gReceiveMessageListener->onDeleteMessage(messageUid);
    }
}
void ChatClient::onUserInfoUpdated(const std::list<UserInfo> &userInfos) {
#if IS_QT
    emit userInfoUpdated(userInfos);
#endif
    if (gUserInfoUpdateListener) {
        gUserInfoUpdateListener->onUserInfoUpdated(userInfos);
    }
}
void ChatClient::onGroupInfoUpdated(const std::list<GroupInfo> &groupInfos) {
#if IS_QT
    emit groupInfoUpdated(groupInfos);
#endif
    if (gGroupInfoUpdateListener) {
        gGroupInfoUpdateListener->onGroupInfoUpdated(groupInfos);
    }
}
void ChatClient::onGroupMemberUpdated(const std::string &groupId) {
#if IS_QT
    emit groupMemberUpdated(groupId);
#endif
    if (gGroupMemberUpdateListener) {
        gGroupMemberUpdateListener->onGroupMemberUpdated(groupId);
    }
}
void ChatClient::onContactUpdated(const std::list<std::string> &friendUids) {
#if IS_QT
    emit contactUpdated(friendUids);
#endif
    if (gContactUpdateListener) {
        gContactUpdateListener->onContactUpdated(friendUids);
    }
}
void ChatClient::onFriendRequestUpdated(const std::list<std::string> &newRequests) {
#if IS_QT
    emit friendRequestUpdated(newRequests);
#endif
    if (gFriendRequestUpdateListener) {
        gFriendRequestUpdateListener->onFriendRequestUpdated(newRequests);
    }
}

void ChatClient::onUserSettingUpdated() {
#if IS_QT
    emit userSettingUpdated();
#endif
    if (gUserSettingUpdateListener) {
        gUserSettingUpdateListener->onUserSettingUpdated();
    }
}

void ChatClient::onChannelInfoUpdated(const std::list<ChannelInfo> &channelInfo) {
#if IS_QT
    emit channelInfoUpdated(channelInfo);
#endif
    if (gChannelInfoUpdateListener) {
        gChannelInfoUpdateListener->onChannelInfoUpdated(channelInfo);
    }
}

void ChatClient::onMessageSendSuccess(long messageId, int64_t messageUid, int64_t timestamp)
{
#if IS_QT
    emit messageSendSuccess(messageId, messageUid, timestamp);
#endif
}

void ChatClient::onMessageSendPrepared(long messageId, int64_t timestamp)
{
#if IS_QT
    emit messageSendPrepared(messageId, timestamp);
#endif
}

void ChatClient::onMessageSendProgress(long messageId, int uploaded, int total)
{
#if IS_QT
    emit messageSendProgress(messageId, uploaded, total);
#endif
}

void ChatClient::onMessageSendUploaded(long messageId, const std::string &remoteUrl)
{
#if IS_QT
    emit messageSendUploaded(messageId, remoteUrl);
#endif
}

void ChatClient::onMessageSendFailure(long messageId, int errorCode)
{
#if IS_QT
    emit messageSendFailure(messageId, errorCode);
#endif
}

void ChatClient::onVoidConextSuccess(void *pObj)
{
    VoidCallbackContext *pContent = (VoidCallbackContext *)pObj;
    switch (pContent->type) {
    case VoidCallbackContext::Type::CONVERSATION_TOP:
    {
#if IS_QT
        emit conversationTopUpdated(pContent->conversation);
#endif
        break;
    }
    case VoidCallbackContext::Type::CONVERSATION_SILENT:
    {
#if IS_QT
        emit conversationSilentUpdated(pContent->conversation);
#endif
        break;
    }
    case VoidCallbackContext::Type::RECALL_MESSAGE_UID:
    {
        //不需要再通知，协议栈会主动抛出onRecallMessage事件
        break;
    }
    case VoidCallbackContext::Type::DELETE_FRIEND_USER_ID:
    {
        //不需要再通知，协议栈会主动抛出onContactUpdated事件
        break;
    }
    case VoidCallbackContext::Type::SEND_FRIEND_REQUEST_USER_ID:
    {
        //不需要再通知，协议栈会主动抛出friendRequestUpdated事件
        break;
    }
    case VoidCallbackContext::Type::HANDLE_FRIEND_REQUEST_USER_ID:
    case VoidCallbackContext::Type::SET_FRIEND_ALIAS_USER_ID:
    case VoidCallbackContext::Type::SET_FRIEND_BLACKLIST_USER_ID:
    case VoidCallbackContext::Type::ADD_GROUP_MEMBER_GROUP_ID:
    case VoidCallbackContext::Type::KICKOFF_GROUP_MEMBER_GROUP_ID:
    case VoidCallbackContext::Type::QUIT_GROUP_ID:
    case VoidCallbackContext::Type::DISMISS_GROUP_ID:
    case VoidCallbackContext::Type::MODIFY_GROUP_INFO_ID:
    case VoidCallbackContext::Type::MODIFY_GROUP_ALIAS_ID:
    case VoidCallbackContext::Type::TRANSFER_GROUP_ID:
    case VoidCallbackContext::Type::SET_MANAGER_GROUP_ID:
    case VoidCallbackContext::Type::SET_FAV_GROUP_ID:
    case VoidCallbackContext::Type::USER_SETTING:
    case VoidCallbackContext::Type::MODIFY_MY_INFO:
    case VoidCallbackContext::Type::SET_GLOBAL_SILENT:
    case VoidCallbackContext::Type::SET_HIDDEN_NOTIFI_DETAIL:
    case VoidCallbackContext::Type::SET_HIDDEN_GROUP_ALIAS:
    case VoidCallbackContext::Type::MODIFY_CHANNEL_ID:
    case VoidCallbackContext::Type::LISTEN_CHANNEL_ID:
    case VoidCallbackContext::Type::DESTROY_CHANNEL_ID:
        break;
    }
}

template <typename T>
std::list<T> serializableFromJsonList(const std::string &jsonListStr) {
    Document document;
    if (document.Parse(jsonListStr).HasParseError()) {
        printf("\nParsing to document failure(%s).\n", jsonListStr.c_str());
        return std::list<T>();
    }

    std::list<T> result;

    if (document.IsArray()) {
        for (int i = 0; i < document.Size(); i++) {
            const Value& object = document[i];
            T message;
            Serializable *s = &message;
            s->Unserialize(&object);

            result.push_back(message);
        }
    }

    return result;
}

};
