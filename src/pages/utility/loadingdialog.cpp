#include "loadingdialog.h"
#include <QLayout>


// 加载框实现
LoadingDialog::LoadingDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("处理中...");
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setFixedSize(180, 180);
    setModal(true);

    m_label = new QLabel(this);
    m_label->setAlignment(Qt::AlignCenter);

    m_movie = new QMovie(":/images/loading.gif", QByteArray(), this);
    m_label->setMovie(m_movie);
    m_movie->start();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_label);
}

LoadingDialog::~LoadingDialog() {
    m_movie->stop();
}

void LoadingDialog::start()
{
    m_movie->start();
    show();
}

void LoadingDialog::stop()
{
    m_movie->stop();
    hide();
}
