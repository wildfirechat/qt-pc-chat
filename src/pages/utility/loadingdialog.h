#ifndef LOADINGDIALOG_H
#define LOADINGDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QMovie>

// 加载框类
class LoadingDialog : public QDialog {
    Q_OBJECT
public:
    explicit LoadingDialog(QWidget *parent = nullptr);
    ~LoadingDialog();
    void start();
    void stop();

private:
    QLabel *m_label;
    QMovie *m_movie;
};

#endif // LOADINGDIALOG_H
