#ifndef INPUTTEXTEDIT_H
#define INPUTTEXTEDIT_H

#include <QTextEdit>

class AtEntry {
public:
    QString name;
    QString userId;
    int startPos;

public:
    AtEntry(const QString &name, const QString &userId, int startPos);
};

class InputTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit InputTextEdit(QWidget *parent = nullptr);
    void addAtEntry(const QString &name, const QString &userId);
    QVector<AtEntry> getAtEntrys();

signals:
    void atTriggered(const QString &prefix);

private slots:
    void onCursorPositionChanged();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

private:
    QTimer *timer;
    bool isAtMode;
    QString currentPrefix;
    bool isProcessingChange = false;
    QVector<AtEntry> atEntrys;

private:
    int lastCursorPosition;

    bool isCursorInsideAtMention(const QTextCursor &cursor);
    void handleAtMention(QTextCursor &cursor, bool isMovingForward);
};
#endif // INPUTTEXTEDIT_H
