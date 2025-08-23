#ifndef SINGLECALLWEBVIEWWIDGET_H
#define SINGLECALLWEBVIEWWIDGET_H

#include <QWidget>
#include <QDebug>



class SingleCallWebViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SingleCallWebViewWidget(QWidget *parent = nullptr);


signals:

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // SINGLECALLWEBVIEWWIDGET_H
