#include "singleinstance.h"

#include <QApplication>
#include <QLocalServer>
#include <QLocalSocket>
#include <QMessageBox>
#include <QTimer>
#include <QWindow>

#define APP_INSTANCE_NAME "WfcPCClient"

SingleInstance::SingleInstance(QApplication* app) : app(app) {
    // 尝试连接到已存在的服务器
    QLocalSocket socket;
    socket.connectToServer(APP_INSTANCE_NAME);
    if (socket.waitForConnected(3000)) {
        // 已有实例运行，发送消息并退出
        socket.write("raise_window");
        socket.waitForBytesWritten(3000);
        socket.disconnectFromServer();
        socket.close();
        QTimer::singleShot(100, app, &QApplication::quit);
    } else {
        // 没有实例运行，创建服务器
        server = new QLocalServer(this);
        connect(server, &QLocalServer::newConnection, this, &SingleInstance::onNewConnection);
        if (!server->listen(APP_INSTANCE_NAME)) {
            // QMessageBox::critical(nullptr, "Error", "Unable to start the server.");
            // QTimer::singleShot(100, app, &QApplication::quit);
        }
    }
}

SingleInstance::~SingleInstance()
{
    if(server) {
        server->close();
        server->deleteLater();
    }
}

void SingleInstance::onNewConnection() {
    QLocalSocket* client = server->nextPendingConnection();
    connect(client, &QLocalSocket::readyRead, [this, client]() {
        QByteArray message = client->readAll();
        if (message == "raise_window") {
            // 将窗口带到前台
            foreach (QWidget* widget, QApplication::topLevelWidgets()) {
                widget->setWindowState(widget->windowState() & ~Qt::WindowMinimized);
                widget->raise();
                widget->activateWindow();
            }
        }
        client->disconnectFromServer();
    });
}
