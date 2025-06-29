#ifndef FRIENDREQUESTDETAILWIDGET_H
#define FRIENDREQUESTDETAILWIDGET_H

#include <QWidget>
#include <QLabel>


class FriendRequestDetailWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FriendRequestDetailWidget(QWidget *parent = nullptr);

    void setUserId(const QString &userId);
signals:
private:
    QLabel *label;
};

#endif // FRIENDREQUESTDETAILWIDGET_H
