//
//  WFClient.h
//  IMClient
//
//  Created by Heavyrain.Lee on 2020/1/7.
//  Copyright © 2020 Wildfirechat. All rights reserved.
//

#ifndef WFClient_h
#define WFClient_h

#ifdef WIN32
#ifdef PROTOWRAPPER_EXPORTS
#define PROTOWRAPPER_API __declspec(dllexport)
#else
#define PROTOWRAPPER_API __declspec(dllimport)
#endif

#define WFCAPI __stdcall
#else
#define PROTOWRAPPER_API
#define WFCAPI
#endif


#include <string>
#include <list>

namespace WFClient {

extern "C" PROTOWRAPPER_API void WFCAPI releaseDllString(const char *dllString);

//Global callback
typedef void (WFCAPI *fun_global_string_callback)(const char *cstr, size_t str_len);
typedef void (WFCAPI *fun_global_int_int_callback)(int value1, int value2);
typedef void (WFCAPI *fun_global_int_sring_callback)(int intValue, const char *cstr, size_t str_len);
typedef void (WFCAPI *fun_global_void_callback)();

typedef void (WFCAPI *fun_connection_callback)(int connectionStatus);
typedef void (WFCAPI *fun_receive_message_callback)(const char *cmessages, size_t messages_len, bool moreMsg);
typedef void (WFCAPI *fun_recall_message_callback)(const char *coperatorId, size_t operatorId_len, int64_t messageUid);
typedef void (WFCAPI *fun_delete_message_callback)(int64_t messageUid);

typedef void (WFCAPI *fun_connecting_to_server_callback)(const char *chost, size_t host_len, const char *cip, size_t ip_len);
typedef void (WFCAPI *fun_connected_to_server_callback)(const char *chost, size_t host_len, const char *cip, size_t ip_len, int nwType);

//general function callback
typedef void (WFCAPI *fun_general_void_success_callback)(void *pObject, int objectDataType);
typedef void (WFCAPI *fun_general_string_success_callback)(void *pObject, int objectDataType, const char *cval, size_t val_len);
typedef void (WFCAPI *fun_general_error_callback)(void *pObject, int objectDataType, int errorCode);

//send message callback
typedef void (WFCAPI *fun_sendMessage_success_callback)(void *pObject, int objectDataType, long messageId, int64_t messageUid, int64_t timestamp);
typedef void (WFCAPI *fun_sendMessage_prepared_callback)(void *pObject, int objectDataType, long messageId, int64_t saveTime);
typedef void (WFCAPI *fun_sendMessage_progress_callback)(void *pObject, int objectDataType, long messageId, int uploaded, int total);
typedef void(WFCAPI *fun_sendMessage_media_uploaded_callback)(void *pObject, int objectDataType, long messageId, const char *cremoteUrl, size_t remoteUrl_len);
typedef void (WFCAPI *fun_sendMessage_error_callback)(void *pObject, int objectDataType, long messageId, int errorCode);

extern "C" PROTOWRAPPER_API void WFCAPI setAppName(const char *cappName, size_t appName_len);

extern "C" PROTOWRAPPER_API void WFCAPI setHeartBeatInterval(int second);

extern "C" PROTOWRAPPER_API void WFCAPI setAppDataPath(const char *cpath, size_t path_len);

extern "C" PROTOWRAPPER_API void WFCAPI setPackageName(const char *cpackagename, size_t packagename_len);


extern "C" PROTOWRAPPER_API void WFCAPI useSM4();

extern "C" PROTOWRAPPER_API void WFCAPI useAES256();

extern "C" PROTOWRAPPER_API void WFCAPI useTcpShortLink();

extern "C" PROTOWRAPPER_API bool WFCAPI isTcpShortLink();

extern "C" PROTOWRAPPER_API void WFCAPI setLiteMode(bool liteMode);

extern "C" PROTOWRAPPER_API int WFCAPI getRoutePort();

extern "C" PROTOWRAPPER_API void WFCAPI setDBPath(const char *cpath, size_t path_len);

extern "C" PROTOWRAPPER_API void WFCAPI setConnectionStatusListener(fun_connection_callback callback);

extern "C" PROTOWRAPPER_API void WFCAPI setReceiveMessageListener(fun_receive_message_callback receiveCallback, fun_recall_message_callback recallCallback, fun_delete_message_callback deleteCallback, fun_global_string_callback deliveryCallback, fun_global_string_callback readedCallback);

extern "C" PROTOWRAPPER_API void WFCAPI setUserInfoUpdateListener(fun_global_string_callback callback);

extern "C" PROTOWRAPPER_API void WFCAPI setGroupInfoUpdateListener(fun_global_string_callback callback);

extern "C" PROTOWRAPPER_API void WFCAPI setGroupMemberUpdateListener(fun_global_string_callback callback);

extern "C" PROTOWRAPPER_API void WFCAPI setFriendUpdateListener(fun_global_string_callback callback);

extern "C" PROTOWRAPPER_API void WFCAPI setFriendRequestListener(fun_global_string_callback callback);

extern "C" PROTOWRAPPER_API void WFCAPI setSettingUpdateListener(fun_global_void_callback callback);

extern "C" PROTOWRAPPER_API void WFCAPI setChannelInfoUpdateListener(fun_global_string_callback callback);

extern "C" PROTOWRAPPER_API void WFCAPI setConnectToServerListener(fun_connecting_to_server_callback connectingCB, fun_connected_to_server_callback connectedCB);

extern "C" PROTOWRAPPER_API void WFCAPI setTrafficDataListener(fun_global_int_int_callback callback);

extern "C" PROTOWRAPPER_API void WFCAPI setErrorEventListener(fun_global_int_sring_callback callback);

extern "C" PROTOWRAPPER_API void WFCAPI setConferenceEventListener(fun_global_string_callback callback);

extern "C" PROTOWRAPPER_API void WFCAPI setOnlineEventListener(fun_global_string_callback callback);

extern "C" PROTOWRAPPER_API void WFCAPI setSecretChatStateListener(fun_global_int_sring_callback callback);

extern "C" PROTOWRAPPER_API void WFCAPI setSecretMessageBurnStateListener(fun_global_int_sring_callback startBurnCB, fun_global_string_callback burnedCB);

extern "C" PROTOWRAPPER_API const char* WFCAPI getClientId(size_t *retlen);

extern "C" PROTOWRAPPER_API const char* WFCAPI getProtoRevision(size_t *retlen);

extern "C" PROTOWRAPPER_API int64_t WFCAPI connect2Server(const char *cuserId, size_t userId_len, const char *ctoken, size_t token_len);

extern "C" PROTOWRAPPER_API void WFCAPI setProxyInfo(const char *chost, size_t host_len, const char *cip, size_t ip_len, int port, const char *cname, size_t name_len, const char *cpassword, size_t password_len);

extern "C" PROTOWRAPPER_API int WFCAPI WFCAPI getConnectionStatus();

extern "C" PROTOWRAPPER_API bool WFCAPI isLogin();

extern "C" PROTOWRAPPER_API void WFCAPI disconnect(int flag);

extern "C" PROTOWRAPPER_API bool WFCAPI beginTransaction();
extern "C" PROTOWRAPPER_API bool WFCAPI commitTransaction();
extern "C" PROTOWRAPPER_API bool WFCAPI rollbackTransaction();

extern "C" PROTOWRAPPER_API void WFCAPI setBackupAddressStrategy(int strategy);
extern "C" PROTOWRAPPER_API void WFCAPI setBackupAddress(const char *caddress, size_t address_len, int port);
extern "C" PROTOWRAPPER_API int WFCAPI getConnectedNetworkType();
extern "C" PROTOWRAPPER_API void WFCAPI setUserAgent(const char *cagent, size_t agent_len);
extern "C" PROTOWRAPPER_API void WFCAPI addHttpHeader(const char *cheader, size_t header_len, const char *cvalue, size_t value_len);

extern "C" PROTOWRAPPER_API void WFCAPI onAppResume();
extern "C" PROTOWRAPPER_API void WFCAPI onAppSuspend();

extern "C" PROTOWRAPPER_API int64_t WFCAPI getServerDeltaTime();

extern "C" PROTOWRAPPER_API const char* WFCAPI getCurrentUserId(size_t *retlen);


extern "C" PROTOWRAPPER_API const char* WFCAPI getConversationInfos(const int atypes[], int types_len, const int als[], int ls_len, size_t *retlen);

extern "C" PROTOWRAPPER_API void WFCAPI registerMessageFlag(int type, int flag);

extern "C" PROTOWRAPPER_API const char* WFCAPI getConversationInfo(int conversationType, const char *ctarget, size_t target_len, int line, size_t *retlen);

extern "C" PROTOWRAPPER_API const char* WFCAPI searchConversation(const int atypes[], int types_len, const int als[], int ls_len, const char *ckeyword, size_t keyword_len, size_t *retlen);

extern "C" PROTOWRAPPER_API const char* WFCAPI searchConversationEx(const int atypes[], int types_len, const int als[], int ls_len, const char *ckeyword, size_t keyword_len, int64_t start_time, int64_t end_time, bool desc, int limit, int offset, size_t *retlen);

extern "C" PROTOWRAPPER_API const char* WFCAPI searchConversationEx2(const int atypes[], int types_len, const int als[], int ls_len, const int acnttypes[], int cnttypes_len, const char *ckeyword, size_t keyword_len, int64_t start_time, int64_t end_time, bool desc, int limit, int offset, bool onlyMentionedMsg, size_t *retlen);

extern "C" PROTOWRAPPER_API void WFCAPI removeConversation(int conversationType, const char *ctarget, size_t target_len, int line, bool clearMsg);

extern "C" PROTOWRAPPER_API void WFCAPI setConversationTop(int conversationType, const char *ctarget, size_t target_len, int line, int isTop, fun_general_void_success_callback succCallback, fun_general_error_callback errCallback, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API void WFCAPI setConversationSlient(int conversationType, const char *ctarget, size_t target_len, int line, bool isSlient, fun_general_void_success_callback succCallback, fun_general_error_callback errCallback, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API void WFCAPI setConversationDraft(int conversationType, const char *ctarget, size_t target_len, int line, const char *cdraft, size_t draft_len);

extern "C" PROTOWRAPPER_API void WFCAPI setConversationTimestamp(int conversationType, const char *ctarget, size_t target_len, int line, int64_t timestamp);

extern "C" PROTOWRAPPER_API const char* WFCAPI getUnreadCount(const int atypes[], int types_len, const int als[], int ls_len, size_t *retlen);

extern "C" PROTOWRAPPER_API const char* WFCAPI getConversationUnreadCount(int conversationType, const char *ctarget, size_t target_len, int line, size_t *retlen);

extern "C" PROTOWRAPPER_API bool WFCAPI clearUnreadStatus(int conversationType, const char *ctarget, size_t target_len, int line);

extern "C" PROTOWRAPPER_API bool WFCAPI clearUnreadStatusEx(const int atypes[], int types_len, const int alines[], int lines_len);

extern "C" PROTOWRAPPER_API bool WFCAPI clearAllUnreadStatus();

extern "C" PROTOWRAPPER_API bool WFCAPI clearMessageUnreadStatus(int messageId);

extern "C" PROTOWRAPPER_API bool WFCAPI clearMessageUnreadStatusBefore(int conversationType, const char *ctarget, size_t target_len, int line, int messageId);

extern "C" PROTOWRAPPER_API int64_t WFCAPI setLastReceivedMessageUnRead(int conversationType, const char *ctarget, size_t target_len, int line, int64_t messageUid, int64_t timestamp);

extern "C" PROTOWRAPPER_API long WFCAPI getConversationFirstUnreadMessageId(int conversationType, const char *ctarget, size_t target_len, int line);

extern "C" PROTOWRAPPER_API void WFCAPI setMediaMessagePlayed(long messageId);

extern "C" PROTOWRAPPER_API bool WFCAPI setMessageLocalExtra(long messageId, const char *clocalExtra, size_t localExtra_len);

extern "C" PROTOWRAPPER_API const char* WFCAPI getMessages(int conversationType, const char *ctarget, size_t target_len, int line, const int acontTypes[], int contTypes_len, int64_t fromIndex, bool direction, int count, const char *cwithUser, size_t withUser_len, size_t *retlen);

extern "C" PROTOWRAPPER_API void WFCAPI getMessagesV2(int conversationType, const char *ctarget, size_t target_len, int line, const int acontTypes[], int contTypes_len, int64_t fromIndex, bool direction, int count, const char *cwithUser, size_t withUser_len, fun_general_string_success_callback successCallback, fun_general_error_callback errorCallback, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API void WFCAPI getMentionedMessages(int conversationType, const char *ctarget, size_t target_len, int line, int64_t fromIndex, bool direction, int count, fun_general_string_success_callback successCallback, fun_general_error_callback errorCallback, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API const char* WFCAPI getMessagesEx(const int aconversationTypes[], int conversationTypes_len, const int alines[], int lines_len, const int acontTypes[], int contTypes_len, int64_t fromIndex, bool direction, int count, const char *cwithUser, size_t withUser_len, size_t *retlen);

extern "C" PROTOWRAPPER_API void WFCAPI getMessagesExV2(const int aconversationTypes[], int conversationTypes_len, const int alines[], int lines_len, const int acontTypes[], int contTypes_len, int64_t fromIndex, bool direction, int count, const char *cwithUser, size_t withUser_len, fun_general_string_success_callback successCallback, fun_general_error_callback errorCallback, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API const char* WFCAPI getMessagesByMessageStatus(const int aconversationTypes[], int conversationTypes_len, const int alines[], int lines_len, const int amessageStatuses[], int messageStatuses_len, int64_t fromIndex, bool direction, int count, const char *cwithUser, size_t withUser_len, size_t *retlen);

extern "C" PROTOWRAPPER_API void WFCAPI getMessagesByMessageStatusV2(int conversationType, const char *ctarget, size_t target_len, int line, const int amessageStatuses[], int messageStatuses_len, int64_t fromIndex, bool direction, int count, const char *cwithUser, size_t withUser_len, fun_general_string_success_callback successCallback, fun_general_error_callback errorCallback, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API void WFCAPI getMessagesByTimestampV2(int conversationType, const char *ctarget, size_t target_len, int line, const int acontTypes[], int contTypes_len, int64_t timestamp, bool direction, int count, const char *cwithUser, size_t withUser_len, fun_general_string_success_callback successCallback, fun_general_error_callback errorCallback, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API void WFCAPI getRemoteMessages(int conversationType, const char *ctarget, size_t target_len, int line, const int acontentTypes[], int contentTypes_len, int64_t beforeUid, int count, fun_general_string_success_callback successCallback, fun_general_error_callback errorCallback, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API const char* WFCAPI getMessage(long messageId, size_t *retlen);

extern "C" PROTOWRAPPER_API const char* WFCAPI getMessageByUid(int64_t messageUid, size_t *retlen);

extern "C" PROTOWRAPPER_API long WFCAPI getFirstUnreadMessageId(int conversationType, const char *ctarget, size_t target_len, int line);

extern "C" PROTOWRAPPER_API const char* WFCAPI searchMessage(int conversationType, const char *ctarget, size_t target_len, int line, const char *ckeyword, size_t keyword_len, bool desc, int limit, int offset, const char *cwithUser, size_t withUser_len, size_t *retlen);

extern "C" PROTOWRAPPER_API const char* WFCAPI sendMessage(int conversationType, const char *ctarget, size_t target_len, int line, const char *cstrcont, size_t strcont_len, const char** cusers, const size_t* userLengths, size_t userCount, int expireDuration, fun_sendMessage_success_callback successCallback, fun_sendMessage_error_callback errorCallback, fun_sendMessage_prepared_callback preparedCallback, fun_sendMessage_progress_callback progressCallback, fun_sendMessage_media_uploaded_callback uploadedCallback, void *pObject, int objectDataType, size_t *retlen);

extern "C" PROTOWRAPPER_API bool WFCAPI sendSavedMessage(long messageId, int expireDuration, fun_sendMessage_success_callback successCallback, fun_sendMessage_error_callback errorCallback, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API bool WFCAPI cancelSendingMessage(long messageId);

extern "C" PROTOWRAPPER_API void WFCAPI recallMessage(int64_t messageUid, fun_general_void_success_callback succCallback, fun_general_error_callback errCallback, void *pObject, int objectDataType);

typedef void (WFCAPI *fun_uploadMedia_progress_callback)(void *pObject, int objectDataType, int uploaded, int total);

extern "C" PROTOWRAPPER_API void WFCAPI uploadMedia(const char *cstrName, size_t strName_len, const char *cdata, int dataLen, int mediaType, fun_general_string_success_callback successCB, fun_general_error_callback errorCB, fun_uploadMedia_progress_callback progressCB, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API bool WFCAPI deleteMessage(long messageId);

extern "C" PROTOWRAPPER_API void WFCAPI clearMessages(int conversationType, const char *ctarget, size_t target_len, int line);

extern "C" PROTOWRAPPER_API void WFCAPI clearMessagesBefore(int conversationType, const char *ctarget, size_t target_len, int line, int64_t before);

extern "C" PROTOWRAPPER_API const char* WFCAPI insertMessage(int conversationType, const char *ctarget, size_t target_len, int line, const char *cstrfrom, size_t strfrom_len, const char *cstrcont, size_t strcont_len, int status, bool notify, int64_t serverTime, size_t *retlen);

extern "C" PROTOWRAPPER_API void WFCAPI updateMessage(long messageId, const char *cstrcont, size_t strcont_len);
extern "C" PROTOWRAPPER_API void WFCAPI updateMessageContentAndTime(long messageId, const char *cstrcont, size_t strcont_len, int64_t timestamp);
extern "C" PROTOWRAPPER_API void WFCAPI updateMessageStatus(long messageId, int messageStatus);

extern "C" PROTOWRAPPER_API const char* WFCAPI getConversationRead(int conversationType, const char *ctarget, size_t target_len, int line, size_t *retlen);
extern "C" PROTOWRAPPER_API const char* WFCAPI getMessageDelivery(int conversationType, const char *ctarget, size_t target_len, size_t *retlen);

extern "C" PROTOWRAPPER_API const char* WFCAPI getUserInfo(const char *cuserId, size_t userId_len, bool refresh, const char *cgroupId, size_t groupId_len, size_t *retlen);

extern "C" PROTOWRAPPER_API const char* WFCAPI getUserInfos(const char** cusers, const size_t* userLengths, size_t userCount, const char *cgroupId, size_t groupId_len, size_t *retlen);

extern "C" PROTOWRAPPER_API void WFCAPI searchUser(const char *ckeyword, size_t keyword_len, int searchType, int page, fun_general_string_success_callback successCB, fun_general_error_callback errorCB, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API bool WFCAPI isMyFriend(const char *cuserId, size_t userId_len);

extern "C" PROTOWRAPPER_API const char* WFCAPI getMyFriendList(bool refresh, size_t *retlen);

extern "C" PROTOWRAPPER_API const char* WFCAPI searchFriends(const char *ckeyword, size_t keyword_len, size_t *retlen);

extern "C" PROTOWRAPPER_API const char* WFCAPI searchGroups(const char *ckeyword, size_t keyword_len, size_t *retlen);

extern "C" PROTOWRAPPER_API const char* WFCAPI getIncommingFriendRequest(size_t *retlen);

extern "C" PROTOWRAPPER_API const char* WFCAPI getOutgoingFriendRequest(size_t *retlen);

extern "C" PROTOWRAPPER_API const char* WFCAPI getFriendRequest(const char *cuserId, size_t userId_len, bool incomming, size_t *retlen);

extern "C" PROTOWRAPPER_API void WFCAPI loadFriendRequestFromRemote();

extern "C" PROTOWRAPPER_API int WFCAPI getUnreadFriendRequestStatus();

extern "C" PROTOWRAPPER_API void WFCAPI clearUnreadFriendRequestStatus();

extern "C" PROTOWRAPPER_API void WFCAPI deleteFriend(const char *cuserId, size_t userId_len, fun_general_void_success_callback successCB, fun_general_error_callback errorCB, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API const char* WFCAPI getFriendAlias(const char *cuserId, size_t userId_len, size_t *retlen);

extern "C" PROTOWRAPPER_API void WFCAPI setFriendAlias(const char *cuserId, size_t userId_len, const char *calias, size_t alias_len, fun_general_void_success_callback successCB, fun_general_error_callback errorCB, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API const char* WFCAPI getFriendExtra(const char *cuserId, size_t userId_len, size_t *retlen);

extern "C" PROTOWRAPPER_API void WFCAPI sendFriendRequest(const char *cuserId, size_t userId_len, const char *creason, size_t reason_len, const char *cextra, size_t extra_len, fun_general_void_success_callback successCB, fun_general_error_callback errorCB, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API void WFCAPI handleFriendRequest(const char *cuserId, size_t userId_len, bool accept, const char *cextra, size_t extra_len, fun_general_void_success_callback successCB, fun_general_error_callback errorCB, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API bool WFCAPI isBlackListed(const char *cuserId, size_t userId_len);

extern "C" PROTOWRAPPER_API const char* WFCAPI getBlackList(bool refresh, size_t *retlen);

extern "C" PROTOWRAPPER_API void WFCAPI setBlackList(const char *cuserId, size_t userId_len, bool isBlackListed, fun_general_void_success_callback successCB, fun_general_error_callback errorCB, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API const char* WFCAPI getGroupMembers(const char *cgroupId, size_t groupId_len, bool refresh, size_t *retlen);

extern "C" PROTOWRAPPER_API const char* WFCAPI getGroupMembersByType(const char *cgroupId, size_t groupId_len, int type, size_t *retlen);

extern "C" PROTOWRAPPER_API const char* WFCAPI getGroupMembersByCount(const char *cgroupId, size_t groupId_len, int count, size_t *retlen);

extern "C" PROTOWRAPPER_API const char* WFCAPI getGroupInfo(const char *cgroupId, size_t groupId_len, bool refresh, size_t *retlen);

extern "C" PROTOWRAPPER_API const char* WFCAPI getGroupMember(const char *cgroupId, size_t groupId_len, const char *cmemberId, size_t memberId_len, size_t *retlen);

extern "C" PROTOWRAPPER_API void WFCAPI createGroup(const char *cgroupId, size_t groupId_len, int groupType, const char *cgroupName, size_t groupName_len, const char *cgroupPortrait, size_t groupPortrait_len, const char *cgroupExtra, size_t groupExtra_len, const char** cusers, const size_t* userLengths, size_t userCount, const char *cmemberExtra, size_t memberExtra_len, const int alines[], int lines_len, const char *cstrCont, size_t strCont_len, fun_general_string_success_callback successCB, fun_general_error_callback errorCB, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API void WFCAPI addMembers(const char *cgroupId, size_t groupId_len, const char** cusers, const size_t* userLengths, size_t userCount, const char *cmemberExtra, size_t memberExtra_len, const int alines[], int lines_len, const char *cstrCont, size_t strCont_len, fun_general_void_success_callback successBlock, fun_general_error_callback errorBlock, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API void WFCAPI kickoffMembers(const char *cgroupId, size_t groupId_len, const char** cusers, const size_t* userLengths, size_t userCount, const int alines[], int lines_len, const char *cstrCont, size_t strCont_len, fun_general_void_success_callback successBlock, fun_general_error_callback errorBlock, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API void WFCAPI quitGroup(const char *cgroupId, size_t groupId_len, const int alines[], int lines_len, const char *cstrCont, size_t strCont_len, fun_general_void_success_callback successBlock, fun_general_error_callback errorBlock, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API void WFCAPI dismissGroup(const char *cgroupId, size_t groupId_len, const int alines[], int lines_len, const char *cstrCont, size_t strCont_len, fun_general_void_success_callback successBlock, fun_general_error_callback errorBlock, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API void WFCAPI modifyGroupInfo(const char *cgroupId, size_t groupId_len, int type, const char *cnewValue, size_t newValue_len, const int alines[], int lines_len, const char *cstrCont, size_t strCont_len, fun_general_void_success_callback successBlock, fun_general_error_callback errorBlock, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API void WFCAPI modifyGroupAlias(const char *cgroupId, size_t groupId_len, const char *cnewAlias, size_t newAlias_len, const int alines[], int lines_len, const char *cstrCont, size_t strCont_len, fun_general_void_success_callback successBlock, fun_general_error_callback errorBlock, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API void WFCAPI modifyGroupMemberAlias(const char *cgroupId, size_t groupId_len, const char *cmemberId, size_t memberId_len, const char *cnewAlias, size_t newAlias_len, const int alines[], int lines_len, const char *cstrCont, size_t strCont_len, fun_general_void_success_callback successBlock, fun_general_error_callback errorBlock, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API void WFCAPI modifyGroupMemberExtra(const char *cgroupId, size_t groupId_len, const char *cmemberId, size_t memberId_len, const char *cnewExtra, size_t newExtra_len, const int alines[], int lines_len, const char *cstrCont, size_t strCont_len, fun_general_void_success_callback successBlock, fun_general_error_callback errorBlock, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API void WFCAPI transferGroup(const char *cgroupId, size_t groupId_len, const char *cnewOwner, size_t newOwner_len, const int alines[], int lines_len, const char *cstrCont, size_t strCont_len, fun_general_void_success_callback successBlock, fun_general_error_callback errorBlock, void *pObject, int objectDataType);


extern "C" PROTOWRAPPER_API void WFCAPI setGroupManager(const char *cgroupId, size_t groupId_len, bool isSet, const char** cusers, const size_t* userLengths, size_t userCount, const int alines[], int lines_len, const char *cstrCont, size_t strCont_len, fun_general_void_success_callback successBlock, fun_general_error_callback errorBlock, void *pObject, int objectDataType);
extern "C" PROTOWRAPPER_API void WFCAPI muteGroupMember(const char *cgroupId, size_t groupId_len, bool isSet, const char** cusers, const size_t* userLengths, size_t userCount, const int alines[], int lines_len, const char *cstrCont, size_t strCont_len, fun_general_void_success_callback successBlock, fun_general_error_callback errorBlock, void *pObject, int objectDataType);
extern "C" PROTOWRAPPER_API void WFCAPI allowGroupMember(const char *cgroupId, size_t groupId_len, bool isSet, const char** cusers, const size_t* userLengths, size_t userCount, const int alines[], int lines_len, const char *cstrCont, size_t strCont_len, fun_general_void_success_callback successBlock, fun_general_error_callback errorBlock, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API const char* WFCAPI getFavGroups(size_t *retlen);
extern "C" PROTOWRAPPER_API const char* WFCAPI getUserSetting(int scope, const char *ckey, size_t key_len, size_t *retlen);

extern "C" PROTOWRAPPER_API const char* WFCAPI getUserSettings(int scope, size_t *retlen);

extern "C" PROTOWRAPPER_API void WFCAPI setUserSetting(int scope, const char *ckey, size_t key_len, const char *cvalue, size_t value_len, fun_general_void_success_callback successBlock, fun_general_error_callback errorBlock, void *pObject, int objectDataType);


extern "C" PROTOWRAPPER_API bool WFCAPI isFavGroup(const char *cgroupId, size_t groupId_len);

extern "C" PROTOWRAPPER_API void WFCAPI setFavGroup(const char *cgroupId, size_t groupId_len, bool fav, fun_general_void_success_callback successBlock, fun_general_error_callback errorBlock, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API const char* WFCAPI getFavUsers(size_t *retlen);
extern "C" PROTOWRAPPER_API bool WFCAPI isFavUser(const char *cuserId, size_t userId_len);
extern "C" PROTOWRAPPER_API void WFCAPI setFavUser(const char *cuserId, size_t userId_len, bool fav, fun_general_void_success_callback successBlock, fun_general_error_callback errorBlock, void *pObj);

extern "C" PROTOWRAPPER_API void WFCAPI modifyMyInfo(int type, const char *value, size_t len, fun_general_void_success_callback successBlock, fun_general_error_callback errorBlock, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API bool WFCAPI isGlobalSilent();
extern "C" PROTOWRAPPER_API void WFCAPI setGlobalSilent(bool slient, fun_general_void_success_callback successBlock, fun_general_error_callback errorBlock, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API bool WFCAPI isHiddenNotificationDetail();

extern "C" PROTOWRAPPER_API void WFCAPI setHiddenNotificationDetail(bool hidden, fun_general_void_success_callback successBlock, fun_general_error_callback errorBlock, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API bool WFCAPI isHiddenGroupMemberName(const char *cgroupId, size_t groupId_len);

extern "C" PROTOWRAPPER_API void WFCAPI setHiddenGroupMemberName(const char *cgroupId, size_t groupId_len, bool hidden, fun_general_void_success_callback successBlock, fun_general_error_callback errorBlock, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API void WFCAPI joinChatroom(const char *cchatroomId, size_t chatroomId_len, fun_general_void_success_callback successBlock, fun_general_error_callback errorBlock, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API void WFCAPI quitChatroom(const char *cchatroomId, size_t chatroomId_len, fun_general_void_success_callback successBlock, fun_general_error_callback errorBlock, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API void WFCAPI getChatroomInfo(const char *cchatroomId, size_t chatroomId_len, int64_t updateDt, fun_general_string_success_callback successBlock, fun_general_error_callback errorBlock, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API void WFCAPI getChatroomMemberInfo(const char *cchatroomId, size_t chatroomId_len, int maxCount, fun_general_string_success_callback successBlock, fun_general_error_callback errorBlock, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API void WFCAPI createChannel(const char *cchannelName, size_t channelName_len, const char *cchannelPortrait, size_t channelPortrait_len, const char *cdesc, size_t desc_len, const char *cextra, size_t extra_len, fun_general_string_success_callback successBlock, fun_general_error_callback errorBlock, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API const char* WFCAPI getChannelInfo(const char *cchannelId, size_t channelId_len, bool refresh, size_t *retlen);

extern "C" PROTOWRAPPER_API void WFCAPI modifyChannelInfo(const char *cchannelId, size_t channelId_len, int type, const char *cnewValue, size_t newValue_len, fun_general_void_success_callback successBlock, fun_general_error_callback errorBlock, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API void WFCAPI searchChannel(const char *ckeyword, size_t keyword_len, fun_general_string_success_callback successBlock, fun_general_error_callback errorBlock, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API bool WFCAPI isListenedChannel(const char *cchannelId, size_t channelId_len);

extern "C" PROTOWRAPPER_API void WFCAPI listenChannel(const char *cchannelId, size_t channelId_len, bool listen, fun_general_void_success_callback successBlock, fun_general_error_callback errorBlock, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API const char* WFCAPI getMyChannels(size_t *retlen);

extern "C" PROTOWRAPPER_API const char* WFCAPI getListenedChannels(size_t *retlen);

extern "C" PROTOWRAPPER_API void WFCAPI destoryChannel(const char *cchannelId, size_t channelId_len, fun_general_void_success_callback successBlock, fun_general_error_callback errorBlock, void *pObject, int objectDataType);

extern "C" PROTOWRAPPER_API const char* WFCAPI getAppPath(size_t *retlen);

extern "C" PROTOWRAPPER_API void WFCAPI getAuthorizedMediaUrl(long long messageUid, int mediaType, const char *cmediaPath, size_t mediaPath_len, fun_general_string_success_callback successBlock, fun_general_error_callback errorBlock, void *pObj, int objectDataType);

}

#endif /* WFClient_h */
