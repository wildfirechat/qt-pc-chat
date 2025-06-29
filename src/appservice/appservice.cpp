#include "appservice.h"
#include "../config.h"
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include "../wfc/client/ChatClient.h"

/*
    PlatformType_UNSET = 0,
    PlatformType_iOS = 1,
    PlatformType_Android = 2,
    PlatformType_Windows = 3,
    PlatformType_OSX = 4,
    PlatformType_WEB = 5,
    PlatformType_WX = 6,
    PlatformType_Linux = 7,
    PlatformType_iPad = 8,
    //Android pad
    PlatformType_APad = 9,
    PlatformType_Harmony = 10,
    PlatformType_HarmonyPad = 11,
    PlatformType_HarmonyPC = 12
*/
#ifdef Q_OS_WIN
    static int platform = 3;
#elif defined(Q_OS_MAC)
    static int platform = 4;
#elif defined(Q_OS_LINUX)
    static int platform = 7;
#else
    std::cout << "Running on an unknown operating system" << std::endl;
#endif

AppService *AppService::sharedSingleton = nullptr;

AppService::AppService(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
}

AppService *AppService::sharedAppService()
{
    if (sharedSingleton == nullptr) {
        sharedSingleton = new AppService();
    }
    return sharedSingleton;
}

void AppService::createPCSession(const QString& userId,
                     std::function<void(const QString &userId, const QString &appToken, int status, long expired, int platform, const QString &deviceName)> successBlock,
                     std::function<void(int errCode, const QString &message)> errorBlock) {
    QJsonObject data;
    data["flag"] = 1;
    data["userId"] = userId;
    data["device_name"] = QString("pc");
    data["clientId"] = QString(WFCLib::ChatClient::Instance()->getClientId().c_str());
    data["platform"] = platform;

    post("/pc_session", data, true, [successBlock, errorBlock](const QJsonObject &dict) {
        if (dict["code"].toInt() == 0) {
            QJsonObject result = dict["result"].toObject();
            QString userId = result["userId"].toString();
            QString token = result["token"].toString();
            int status = result["status"].toInt();
            long expired = result["expired"].toBool();
            int platform = result["platform"].toInt();
            QString deviceName = result["deviceName"].toString();
            if (successBlock) {
                successBlock(userId, token, status, expired, platform, deviceName);
            }
        } else {
            if (errorBlock) {
                errorBlock(dict["code"].toInt(), dict["message"].toString());
            }
        }
    }, [errorBlock](const QNetworkReply::NetworkError &error) {
        if (errorBlock) {
            errorBlock(-1, QString::number(error));
        }
    });
}

void AppService::loginWithPCSession(const QString &appToken,
                        std::function<void(const QString &userId, const QString &token, bool newUser, const QString &userName, const QString &portrait, const QString &resetCode)> successBlock,
                                    std::function<void(int errCode, const QString &message, const QString &userName, const QString &portrait)> errorBlock) {
    QJsonObject data;
    post("/session_login/" + appToken, data, true, [successBlock, errorBlock](const QJsonObject &dict) {
        int code = dict["code"].toInt();
        if (code == 0) {
            QJsonObject result = dict["result"].toObject();
            QString userId = result["userId"].toString();
            QString token = result["token"].toString();
            bool newUser = result["register"].toBool();
            QString userName = result["userName"].toString();
            QString portrait = result["portrait"].toString();
            QString resetCode = result["resetCode"].toString();
            if (successBlock) {
                successBlock(userId, token, newUser, userName, portrait, resetCode);
            }
        } else if(code == 9) {
            QJsonObject result = dict["result"].toObject();
            QString userName = result["userName"].toString();
            QString portrait = result["portrait"].toString();
            if (errorBlock) {
                errorBlock(code, nullptr, userName, portrait);
            }
        } else {
            if (errorBlock) {
                errorBlock(code, dict["message"].toString(), nullptr, nullptr);
            }
        }
    }, [errorBlock](const QNetworkReply::NetworkError &error) {
        if (errorBlock) {
            errorBlock(-1, QString::number(error), nullptr, nullptr);
        }
    });
}


void AppService::loginWithMobile(const QString &mobile, const QString &verifyCode,
                                 std::function<void(const QString &userId, const QString &token, bool newUser, const QString &resetCode)> successBlock,
                                 std::function<void(int errCode, const QString &message)> errorBlock) {
    QJsonObject data;
    data["mobile"] = mobile;
    data["code"] = verifyCode;
    data["clientId"] = QString(WFCLib::ChatClient::Instance()->getClientId().c_str());
    data["platform"] = platform;

    post("/login", data, true, [successBlock, errorBlock](const QJsonObject &dict) {
        if (dict["code"].toInt() == 0) {
            QJsonObject result = dict["result"].toObject();
            QString userId = result["userId"].toString();
            QString token = result["token"].toString();
            bool newUser = result["register"].toBool();
            QString resetCode = result["resetCode"].toString();
            if (successBlock) {
                successBlock(userId, token, newUser, resetCode);
            }
        } else {
            if (errorBlock) {
                errorBlock(dict["code"].toInt(), dict["message"].toString());
            }
        }
    }, [errorBlock](const QNetworkReply::NetworkError &error) {
        if (errorBlock) {
            errorBlock(-1, QString::number(error));
        }
    });
}

void AppService::loginWithMobilePwd(const QString &mobile, const QString &password,
                                 std::function<void(const QString &userId, const QString &token, bool newUser, const QString &resetCode)> successBlock,
                                 std::function<void(int errCode, const QString &message)> errorBlock) {
    QJsonObject data;
    data["mobile"] = mobile;
    data["password"] = password;
    data["clientId"] = QString(WFCLib::ChatClient::Instance()->getClientId().c_str());
    data["platform"] = platform;

    post("/login_pwd", data, true, [successBlock, errorBlock](const QJsonObject &dict) {
        if (dict["code"].toInt() == 0) {
            QJsonObject result = dict["result"].toObject();
            QString userId = result["userId"].toString();
            QString token = result["token"].toString();
            bool newUser = result["register"].toBool();
            QString resetCode = result["resetCode"].toString();
            if (successBlock) {
                successBlock(userId, token, newUser, resetCode);
            }
        } else {
            if (errorBlock) {
                errorBlock(dict["code"].toInt(), dict["message"].toString());
            }
        }
    }, [errorBlock](const QNetworkReply::NetworkError &error) {
             if (errorBlock) {
                 errorBlock(-1, QString::number(error));
             }
    });
}


void AppService::resetPassword(const QString &mobile, const QString &code, const QString &newPassword,
                               std::function<void()> successBlock,
                               std::function<void(int errCode, const QString &message)> errorBlock) {
    QJsonObject data;
    if (!mobile.isEmpty()) {
        data["mobile"] = mobile;
    }
    data["resetCode"] = code;
    data["newPassword"] = newPassword;

    post("/reset_pwd", data, false, [successBlock, errorBlock](const QJsonObject &dict) {
        if (dict["code"].toInt() == 0) {
            if (successBlock) {
                successBlock();
            }
        } else {
            if (errorBlock) {
                errorBlock(dict["code"].toInt(), dict["message"].toString());
            }
        }
    }, [errorBlock](const QNetworkReply::NetworkError &error) {
             if (errorBlock) {
                 errorBlock(-1, QString::number(error));
             }
    });
}

void AppService::changePassword(const QString &oldPassword, const QString &newPassword,
                                std::function<void()> successBlock,
                                std::function<void(int errCode, const QString &message)> errorBlock) {
    QJsonObject data;
    data["oldPassword"] = oldPassword;
    data["newPassword"] = newPassword;

    post("/change_pwd", data, false, [successBlock, errorBlock](const QJsonObject &dict) {
        if (dict["code"].toInt() == 0) {
            if (successBlock) {
                successBlock();
            }
        } else {
            if (errorBlock) {
                errorBlock(dict["code"].toInt(), dict["message"].toString());
            }
        }
    }, [errorBlock](const QNetworkReply::NetworkError &error) {
             if (errorBlock) {
                 errorBlock(-1, QString::number(error));
             }
    });
}


void AppService::sendLoginCode(const QString &mobile,
                               std::function<void()> successBlock,
                               std::function<void(int errCode, const QString &message)> errorBlock) {
    QJsonObject data;
    data["mobile"] = mobile;
    data["clientId"] = QString(WFCLib::ChatClient::Instance()->getClientId().c_str());

    post("/send_login_code", data, false, [successBlock, errorBlock](const QJsonObject &dict) {
        if (dict["code"].toInt() == 0) {
            if (successBlock) {
                successBlock();
            }
        } else {
            if (errorBlock) {
                errorBlock(dict["code"].toInt(), dict["message"].toString());
            }
        }
    }, [errorBlock](const QNetworkReply::NetworkError &error) {
             if (errorBlock) {
                 errorBlock(-1, QString::number(error));
             }
    });
}

void AppService::sendResetCode(const QString &mobile,
                               std::function<void()> successBlock,
                               std::function<void(int errCode, const QString &message)> errorBlock) {
    QJsonObject data;
    data["mobile"] = mobile;
    data["clientId"] = QString(WFCLib::ChatClient::Instance()->getClientId().c_str());

    post("/send_reset_code", data, false, [successBlock, errorBlock](const QJsonObject &dict) {
        if (dict["code"].toInt() == 0) {
            if (successBlock) {
                successBlock();
            }
        } else {
            if (errorBlock) {
                errorBlock(dict["code"].toInt(), dict["message"].toString());
            }
        }
    }, [errorBlock](const QNetworkReply::NetworkError &error) {
             if (errorBlock) {
                 errorBlock(-1, QString::number(error));
             }
    });
}

void AppService::getGroupAnnouncement(const QString &groupId,
                                      std::function<void(const QString &announcement)> successBlock,
                                      std::function<void(int errCode, const QString &message)> errorBlock) {
    QJsonObject data;
    data["groupId"] = groupId;

    post("/get_group_announcement", data, false, [successBlock, errorBlock](const QJsonObject &dict) {
        if (dict["code"].toInt() == 0) {
            QJsonObject result = dict["result"].toObject();
            QString announcement = result["announcement"].toString();
            if (successBlock) {
                successBlock(announcement);
            }
        } else {
            if (errorBlock) {
                errorBlock(dict["code"].toInt(), dict["message"].toString());
            }
        }
    }, [errorBlock](const QNetworkReply::NetworkError &error) {
             if (errorBlock) {
                 errorBlock(-1, QString::number(error));
             }
    });
}

void AppService::updateGroupAnnouncement(const QString &groupId, const QString &announcement,
                                         std::function<void()> successBlock,
                                         std::function<void(int errCode, const QString &message)> errorBlock) {
    QJsonObject data;
    data["groupId"] = groupId;
    data["announcement"] = announcement;

    post("/update_group_announcement", data, false, [successBlock, errorBlock](const QJsonObject &dict) {
        if (dict["code"].toInt() == 0) {
            if (successBlock) {
                successBlock();
            }
        } else {
            if (errorBlock) {
                errorBlock(dict["code"].toInt(), dict["message"].toString());
            }
        }
    }, [errorBlock](const QNetworkReply::NetworkError &error) {
             if (errorBlock) {
                 errorBlock(-1, QString::number(error));
             }
    });
}

void AppService::getGroupMembersForPortrait(const QString &groupId,
                                            std::function<void(const QJsonArray &members)> successBlock,
                                            std::function<void(int errCode, const QString &message)> errorBlock) {
    QJsonObject data;
    data["groupId"] = groupId;

    post("/get_group_members_for_portrait", data, false, [successBlock, errorBlock](const QJsonObject &dict) {
        if (dict["code"].toInt() == 0) {
            QJsonArray members = dict["result"].toArray();
            if (successBlock) {
                successBlock(members);
            }
        } else {
            if (errorBlock) {
                errorBlock(dict["code"].toInt(), dict["message"].toString());
            }
        }
    }, [errorBlock](const QNetworkReply::NetworkError &error) {
             if (errorBlock) {
                 errorBlock(-1, QString::number(error));
             }
    });
}

void AppService::uploadLogs(const QStringList &logFiles,
                            std::function<void()> successBlock,
                            std::function<void(int errCode, const QString &message)> errorBlock) {
    QNetworkRequest request(QUrl("APP_SERVER_ADDRESS/upload_logs"));
    QString boundary = "---------------------------" + QString::number(QDateTime::currentMSecsSinceEpoch());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=" + boundary);

    // 设置认证信息
    QString authToken = settings.value("WFC_APPSERVER_AUTH_TOKEN").toString();
    if (!authToken.isEmpty()) {
        request.setRawHeader("authToken", authToken.toUtf8());
    }

    QByteArray postData;

    // 添加每个日志文件
    for (const QString &filePath : logFiles) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            if (errorBlock) {
                errorBlock(-1, "无法打开文件: " + filePath);
            }
            return;
        }

        postData.append("--").append(boundary.toStdString().c_str()).append("\r\n");
        postData.append("Content-Disposition: form-data; name=\"logs\"; filename=\"").append(QFileInfo(file).fileName().toStdString().c_str()).append("\"\r\n");
        postData.append("Content-Type: application/octet-stream\r\n\r\n");
        postData.append(file.readAll());
        postData.append("\r\n");
        file.close();
    }

    // 添加结束边界
    postData.append("--").append(boundary.toStdString().c_str()).append("--\r\n");


    QNetworkReply *reply = manager->post(request, postData);

    QObject::connect(reply, &QNetworkReply::finished, [reply, successBlock, errorBlock, this]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
            QJsonObject dict = jsonDoc.object();

            if (dict["code"].toInt() == 0) {
                if (successBlock) {
                    successBlock();
                }
            } else {
                if (errorBlock) {
                    errorBlock(dict["code"].toInt(), dict["message"].toString());
                }
            }
        } else {
            if (errorBlock) {
                errorBlock(-1, reply->errorString());
            }
        }
        reply->deleteLater();
    });
}

void AppService::clearAppServiceAuthInfos() {
    // 清除保存的认证令牌
    settings.remove("WFC_APPSERVER_AUTH_TOKEN");

    // 清除其他可能的认证相关信息
    settings.remove("userId");
    settings.remove("userName");

    // 可以添加其他需要清除的认证信息...

    qDebug() << "应用服务认证信息已清除";
}

void AppService::post(const QString &path, const QJsonObject &data, bool isLogin,
                      std::function<void(const QJsonObject &dict)> successBlock,
                      std::function<void(const QNetworkReply::NetworkError &error)> errorBlock) {
    QNetworkRequest request(QUrl(APP_SERVER_ADDRESS + path));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 设置认证信息
    QString authToken = settings.value("WFC_APPSERVER_AUTH_TOKEN").toString();
    if (!authToken.isEmpty()) {
        request.setRawHeader("authToken", authToken.toUtf8());
    }

    QByteArray jsonData = QJsonDocument(data).toJson();
    QNetworkReply *reply = manager->post(request, jsonData);

    QObject::connect(reply, &QNetworkReply::finished, [reply, isLogin, successBlock, errorBlock, this]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
            QJsonObject dict = jsonDoc.object();

            if (isLogin) {
                QVariant appToken = reply->rawHeader("authToken");
                if (!appToken.toString().isEmpty()) {
                    settings.setValue("WFC_APPSERVER_AUTH_TOKEN", appToken);
                }
            }

            if (successBlock) {
                successBlock(dict);
            }
        } else {
            if (errorBlock) {
                errorBlock(reply->error());
            }
        }
        reply->deleteLater();
    });
}

std::string AppService::groupDefaultPortrait(const WFCLib::GroupInfo &groupInfo, const std::list<WFCLib::UserInfo> &members)
{
    if(!groupInfo.portrait.empty()) {
        return groupInfo.portrait;
    }

    QJsonObject data;
    QJsonArray array;
    foreach (auto u, members) {
        QJsonObject m;
        if(u.portrait.empty()) {
            m["name"] = u.displayName.c_str();
        } else {
            m["avatarUrl"] = u.portrait.c_str();
        }
        array.append(m);
    }
    data["members"] = array;

    QJsonDocument doc(data);
    return APP_SERVER_ADDRESS.toStdString() + "/avatar/group?request=" + QUrl::toPercentEncoding(doc.toJson(QJsonDocument::Compact)).toStdString();
}

std::string AppService::userDefaultPortrait(const WFCLib::UserInfo &userInfo)
{
    if(!userInfo.portrait.empty()) {
        return userInfo.portrait;
    }

    return APP_SERVER_ADDRESS.toStdString() + "/avatar?name=" + QUrl::toPercentEncoding(userInfo.getReadableName().c_str()).toStdString();
}
