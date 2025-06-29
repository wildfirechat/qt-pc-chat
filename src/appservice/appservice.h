#ifndef APPSERVICE_H
#define APPSERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSettings>
#include <QJsonObject>
#include <QJsonDocument>
#include "../wfc/client/ChatClient.h"

class AppService : public QObject, public WFCLib::DefaultPortraitProvider
{
    Q_OBJECT
public:
    static AppService *sharedAppService();


    /*
    private String token;
    private String userId;

    private int status;
    private long expired;
    private int platform;
    private String device_name;
     */
    void createPCSession(const QString& userId,
                         std::function<void(const QString &userId, const QString &appToken, int status, long expired, int platform, const QString &deviceName)> successBlock,
                         std::function<void(int errCode, const QString &message)> errorBlock);

    void loginWithPCSession(const QString &appToken,
                            std::function<void(const QString &userId, const QString &token, bool newUser, const QString &userName, const QString &portrait, const QString &resetCode)> successBlock,
                            std::function<void(int errCode, const QString &message, const QString &userName, const QString &portrait)> errorBlock);

    void loginWithMobile(const QString &mobile, const QString &verifyCode,
                         std::function<void(const QString &userId, const QString &token, bool newUser, const QString &resetCode)> successBlock,
                         std::function<void(int errCode, const QString &message)> errorBlock);

    void loginWithMobilePwd(const QString &mobile, const QString &password,
                         std::function<void(const QString &userId, const QString &token, bool newUser, const QString &resetCode)> successBlock,
                         std::function<void(int errCode, const QString &message)> errorBlock);

    void resetPassword(const QString &mobile, const QString &code, const QString &newPassword,
                       std::function<void()> successBlock,
                       std::function<void(int errCode, const QString &message)> errorBlock);

    void changePassword(const QString &oldPassword, const QString &newPassword,
                        std::function<void()> successBlock,
                        std::function<void(int errCode, const QString &message)> errorBlock);

    void sendLoginCode(const QString &mobile,
                       std::function<void()> successBlock,
                       std::function<void(int errCode, const QString &message)> errorBlock);

    void sendResetCode(const QString &mobile,
                       std::function<void()> successBlock,
                       std::function<void(int errCode, const QString &message)> errorBlock);


    void getGroupAnnouncement(const QString &groupId,
                              std::function<void(const QString &announcement)> successBlock,
                              std::function<void(int errCode, const QString &message)> errorBlock);

    void updateGroupAnnouncement(const QString &groupId, const QString &announcement,
                                 std::function<void()> successBlock,
                                 std::function<void(int errCode, const QString &message)> errorBlock);

    void getGroupMembersForPortrait(const QString &groupId,
                                    std::function<void(const QJsonArray &members)> successBlock,
                                    std::function<void(int errCode, const QString &message)> errorBlock);

    void uploadLogs(const QStringList &logFiles,
                    std::function<void()> successBlock,
                    std::function<void(int errCode, const QString &message)> errorBlock);

    void clearAppServiceAuthInfos();
    // 其他方法的声明...

private:
    explicit AppService(QObject *parent = nullptr);
    static AppService *sharedSingleton;
    QNetworkAccessManager *manager;
    QSettings settings;

    void post(const QString &path, const QJsonObject &data, bool isLogin,
              std::function<void(const QJsonObject &dict)> successBlock,
              std::function<void(const QNetworkReply::NetworkError &error)> errorBlock);

    // DefaultPortraitProvider interface
public:
    std::string groupDefaultPortrait(const WFCLib::GroupInfo &groupInfo, const std::list<WFCLib::UserInfo> &members);
    std::string userDefaultPortrait(const WFCLib::UserInfo &userInfo);
};

#endif // APPSERVICE_H
