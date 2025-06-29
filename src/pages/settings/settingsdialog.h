// settingsdialog.h
#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "settingswidget.h"
#include <../../wfc/client/ChatClient.h>

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

private slots:
    void onConnectionsStatusChanged(WFCLib::ConnectionStatus status);

private:
    SettingsWidget *settingsWidget;
};

#endif // SETTINGSDIALOG_H
