#ifndef EMPTYDETAILWIDGET_H
#define EMPTYDETAILWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class EmptyDetailWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EmptyDetailWidget(QWidget *parent = nullptr);
    ~EmptyDetailWidget();

private:
    void setupUI();
};

#endif // EMPTYDETAILWIDGET_H    