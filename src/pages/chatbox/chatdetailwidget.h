#ifndef CHATDETAILWIDGET_H
#define CHATDETAILWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QDateTime>
#include <QFileDialog>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include "../../wfc/client/ChatClient.h"
#include "chatmodel.h"
#include "inputtextedit.h"


class QResizeEvent;
class Starter;
class ConversationSettingsWidget;
class QSplitter;
class EmojDialog;
class QStandardItemModel;
class AtMemberListView;
class ChatDelegate;
class IVMainWindow;
class QMediaPlayer;
class ChatDetailWidget : public QWidget, public WFCLib::GetRemoteMessageCallback, public WFCLib::GetMessagesCallback
{
    Q_OBJECT
public:
    explicit ChatDetailWidget(QWidget *parent = nullptr);
    ~ChatDetailWidget();
    
    void setChatConversation(const WFCLib::Conversation &conversation);
    bool hasContent();

signals:
    void ShotDone(Starter* starer, int code);
    void showUserDialog(const QString &userId, const QString &groupId, const QPoint &position);

private slots:
    void onSendMessage();
    void onUploadFile();
    void onMessageSendUploaded(long messageId, const std::string &remoteUrl);
    void onEmojiSelected(const QString &emoji);
    void onReceiveMessages(const std::list<WFCLib::Message> &messages, bool hasMore);

    void sendMessage(const WFCLib::MessageContent &content);
    void toggleSidePanel();
    void onMessageCleared(const WFCLib::Conversation &conversation, int64_t before);

    void onAvatarRightClicked(const QModelIndex &index);
    void onMessageContentClicked(const QModelIndex &index);


private slots:
    void onAtTriggered(const QString &prefix);
    void onMemberSelected(const QModelIndex &index);


private slots:
    void OnShotDone(Starter* starer, int code);
    void onItemClicked(const QModelIndex &index);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent* event) override;


private:
    void setupUI();
    void loadChatHistory(const WFCLib::Conversation &conversation);

    bool fetchEditorContents(QTextEdit* textEdit, QList<QImage> &images, QStringList &filePaths, QStringList &texts);
    void handlePaste();
    void loadHistoryMessages();
    void startCall(bool audioOnly);

    QSplitter *inputSplitter;

    WFCLib::Conversation currentConversation;
    QLabel *partnerNameLabel;
    QListView *messageListView;
    ChatModel *model;
    ChatDelegate *chatDelegate;

    InputTextEdit *messageEdit;
    AtMemberListView *memberListView;
    QStandardItemModel *memberListModel;

    IVMainWindow *imageViewer;

    QPushButton *sendButton;
    QPushButton *fileButton;
    QPushButton *emojiButton;
    QPushButton *capturerButton;
    QPushButton *voiceCallButton;
    QPushButton *videoCallButton;

    EmojDialog *emojDialog;

    bool isAtBottom;

    QTimer *scrollEndTimer;

    QFrame *sidePanel;
    ConversationSettingsWidget *settingWidget;
    int panelWidth;

    bool isLoading;
private:
    std::list<Starter*> m_Starer;
    bool m_Shotting;

    QMediaPlayer *audioPlayer;
    long playingMessageId;
    QTimer *playingTimer;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);

    // GetRemoteMessageCallback interface
public:
    void onSuccess(int dataType, const std::list<WFCLib::Message> &messages);
    void onFailure(int dataType, int errorCode);
};

#endif // CHATDETAILWIDGET_H    
