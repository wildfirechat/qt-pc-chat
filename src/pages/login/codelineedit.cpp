#include "codelineedit.h"
#include <QTimerEvent>
CodeLineEdit::CodeLineEdit(QWidget *parent) : QLineEdit(parent), timerId(0) {
    // 创建清除按钮
    clearButton = new QToolButton(this);
    clearButton->setText("获取验证码");
    clearButton->setCursor(Qt::ArrowCursor);
    clearButton->setStyleSheet("QToolButton { border: none; padding: 0px; font-size: 12px; background-color: rgb(220, 220, 220);}");
    clearButton->setFixedWidth(64);

    // 创建布局
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addStretch();
    layout->addWidget(clearButton);
    layout->setContentsMargins(5, 0, 5, 0);

    // 连接信号和槽
    connect(clearButton, &QToolButton::clicked, this, &CodeLineEdit::clear);
}

void CodeLineEdit::enableSendCode(bool enable) {
    isSendEnable = enable;
    if(timerId == 0) {
        clearButton->setEnabled(enable);
    }
}

void CodeLineEdit::startCountDown(int count) {
    if(timerId == 0) {
        this->count = count;
        timerId = startTimer(1000);
        clearButton->setText(QString::number(count));
        clearButton->setEnabled(false);
    }
}

void CodeLineEdit::timerEvent(QTimerEvent *event) {
    if(event->timerId() == timerId) {
        clearButton->setText(QString::number(--count));
        if(count == 0) {
            killTimer(timerId);
            timerId = 0;
            clearButton->setText("获取验证码");
            clearButton->setEnabled(isSendEnable);
        }
    }
}

void CodeLineEdit::stopCountDown() {
    if(timerId > 0) {
        killTimer(timerId);
        timerId = 0;
    }
    clearButton->setText("获取验证码");
    clearButton->setEnabled(isSendEnable);
}
