#ifndef SINGLEINSTANCE_H
#define SINGLEINSTANCE_H

#include <QApplication>
#include <QLocalServer>
#include <QLocalSocket>
#include <QMessageBox>
#include <QTimer>
#include <QWindow>

class SingleInstance : public QObject
{
    Q_OBJECT

public:
    SingleInstance(QApplication* app);
    virtual ~SingleInstance();

private slots:
    void onNewConnection();

private:
    QApplication* app;
    QLocalServer* server = nullptr;
};


#endif // SINGLEINSTANCE_H
