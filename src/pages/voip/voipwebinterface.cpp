#include "voipwebinterface.h"
#include <QDebug>


VoipWebInterface::VoipWebInterface(QObject *parent): QObject{parent}{

}

QString VoipWebInterface::fromWeb(const QString &message) {
    // TODO
    qDebug() << "receive web:" << message;
    return QString("From C++: %1").arg(message);
}

void VoipWebInterface::onReady()
{
    // do nothing
}