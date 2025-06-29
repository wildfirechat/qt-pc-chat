#ifndef CHANNELDETAILWIDGET_H
#define CHANNELDETAILWIDGET_H

#include <QWidget>


class QLabel;
class ChannelDetailWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelDetailWidget(QWidget *parent = nullptr);
    void setChannelId(const QString &channelId);
signals:
private:
    QLabel *label;
};

#endif // CHANNELDETAILWIDGET_H
