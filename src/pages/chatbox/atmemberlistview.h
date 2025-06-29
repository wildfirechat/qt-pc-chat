#ifndef ATMEMBERLISTVIEW_H
#define ATMEMBERLISTVIEW_H

#include <QListView>

class AtMemberListView : public QListView
{
    Q_OBJECT
public:
    explicit AtMemberListView(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // ATMEMBERLISTVIEW_H
