#ifndef GROUPDETAILWIDGET_H
#define GROUPDETAILWIDGET_H

#include <QWidget>


class QLabel;
class GroupDetailWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GroupDetailWidget(QWidget *parent = nullptr);
    void setGroupId(const QString &groupId);
signals:
private:
    QLabel *label;
};

#endif // GROUPDETAILWIDGET_H
