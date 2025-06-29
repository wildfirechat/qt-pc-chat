#ifndef TAGDETAILWIDGET_H
#define TAGDETAILWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QListWidget>

class TagDetailWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TagDetailWidget(QWidget *parent = nullptr);
    ~TagDetailWidget();
    
    void setTagId(const QString &tagId);

signals:
    void itemSelected(int index, const QString &itemId);

private:
    void setupUI();
    void loadTagItems(const QString &tagId);
    
    QString currentTagId;
    QLabel *tagNameLabel;
    QLabel *tagCountLabel;
    QListWidget *itemsListWidget;
};

#endif // TAGDETAILWIDGET_H    
