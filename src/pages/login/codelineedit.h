#ifndef CODELINEEDIT_H
#define CODELINEEDIT_H

#include <QLineEdit>
#include <QToolButton>
#include <QHBoxLayout>

class CodeLineEdit : public QLineEdit {
    Q_OBJECT
public:
    CodeLineEdit(QWidget *parent = nullptr);

    void enableSendCode(bool enable);

    void startCountDown(int count);
    void stopCountDown();

    QToolButton* clearButton;
protected:
    virtual void timerEvent(QTimerEvent *event);

private:
    bool isSendEnable;
    int timerId;
    int count;
};

#endif // CODELINEEDIT_H
