// settingsdialog.cpp
#include "settingsdialog.h"
#include <QVBoxLayout>

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("设置");
    setMinimumSize(600, 400);

    settingsWidget = new SettingsWidget(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(settingsWidget);

    connect(WFCLib::ChatClient::Instance(), &WFCLib::ChatClient::connectionStatusChanged, this, &SettingsDialog::onConnectionsStatusChanged);
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::onConnectionsStatusChanged(WFCLib::ConnectionStatus status) {
    if(status < -1) {
        hide();
        this->deleteLater();
    }
}
