#ifndef CONTENTWIDGET_H
#define CONTENTWIDGET_H

#include <QWidget>
#include <QStackedWidget>
#include "../../wfc/client/ChatClient.h"

class ChatListWidget;
class ContactListWidget;
class FavoriteListWidget;
class TagListWidget;
class QLineEdit;
class QPushButton;
class LoadingDialog;

class ContentWidget : public QWidget, public WFCLib::GeneralStringCallback
{
    Q_OBJECT
public:
    explicit ContentWidget(QWidget *parent = nullptr);
    ~ContentWidget();

signals:
    void itemSelected(int pageIndex, const QString &itemId);
    void conversationSelected(int pageIndex, const WFCLib::Conversation &itemId);
    void contactSelected(int pageIndex, int type, const QString &itemId);

public slots:
    void chatToUser(const QString &userId);
    void callToUser(const QString &userId, bool isAudio);
    void showPage(int index);

private slots:
    void performSearch();

private:
    void setupUI();
    
    QStackedWidget *stackedWidget;
    ChatListWidget *chatListWidget;
    ContactListWidget *contactListWidget;
    FavoriteListWidget *favoriteListWidget;
    TagListWidget *tagListWidget;

    QLineEdit *searchInput;
    QPushButton *addBtn;

    LoadingDialog *m_loadingDialog;

    // GeneralStringCallback interface
public:
    void onSuccess(int dataType, const std::string &value);
    void onFailure(int dataType, int errorCode);
};

#endif // CONTENTWIDGET_H    
