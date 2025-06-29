#include "inputtextedit.h"
#include <QTimer>
#include <QKeyEvent>
#include <QTextBlock>


InputTextEdit::InputTextEdit(QWidget *parent) : QTextEdit(parent), isAtMode(false)
{
    connect(this, &QTextEdit::cursorPositionChanged, this, &InputTextEdit::onCursorPositionChanged);

    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, [this]() {
        if (isAtMode) {
            emit atTriggered(currentPrefix);
        }
    });

    connect(document(), &QTextDocument::contentsChange, [=](int position, int charsRemoved, int charsAdded) {
        // 阻止信号递归
        if (isProcessingChange) return;
        isProcessingChange = true;

        if(charsAdded > 0) {
            for(int i = 0; i < atEntrys.size(); i++) {
                if(atEntrys[i].startPos > position) {
                    atEntrys[i].startPos += charsAdded;
                }
            }
        } else {
            // 撤销上一步操作
            document()->undo();

            // 获取变更前的文本
            QString originalText = document()->toPlainText();

            // 计算变更后的文本（手动模拟）
            QString newText = originalText;
            int newPos = position;

            for(int i = atEntrys.size()-1; i >=0; i--) {
                if(position > atEntrys[i].startPos && ((position-charsRemoved) < (atEntrys[i].startPos + atEntrys[i].name.size() + 1))) {
                    if(position-charsRemoved > atEntrys[i].startPos && newPos > atEntrys[i].startPos) {
                        newPos = atEntrys[i].startPos;
                    }
                    atEntrys.removeLast();
                }
            }
            newText.remove(newPos, charsRemoved + position-newPos);
            // 应用修改后的文本
            document()->setPlainText(newText);

            QTextCursor cursor = textCursor();
            cursor.setPosition(newPos);
            setTextCursor(cursor);
            if(newText.isEmpty()) {
                atEntrys.clear();
            }
        }

        isProcessingChange = false;
    });
}

void InputTextEdit::addAtEntry(const QString &name, const QString &userId)
{
    // 获取当前光标位置
    QTextCursor cursor = textCursor();

    // 移除@前缀
    if (!cursor.atStart()) {
        cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor); // 向左移动并选中
        if(cursor.selectedText() == "@") {
            cursor.removeSelectedText(); // 删除选中的文本
            setTextCursor(cursor);
        }
    }

    atEntrys.append(AtEntry(name, userId, cursor.position()));

    // 插入@成员
    cursor.insertText("@" + name + " ");
    setTextCursor(cursor);
}

QVector<AtEntry> InputTextEdit::getAtEntrys()
{
    return atEntrys;
}

void InputTextEdit::onCursorPositionChanged() {
    QTextCursor cursor = textCursor();
    int currentPosition = cursor.position();

    // 判断移动方向
    bool isMovingForward = (currentPosition > lastCursorPosition);

    // 处理@提及
    handleAtMention(cursor, isMovingForward);

    lastCursorPosition = currentPosition;
}

void InputTextEdit::keyPressEvent(QKeyEvent *event)
{
    QTextEdit::keyPressEvent(event);

    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);
    QString word = cursor.selectedText();

    if (event->key() == Qt::Key_At) {
        isAtMode = true;
        currentPrefix.clear();
        timer->start(100);
    } else if (isAtMode) {
        if (word.startsWith('@')) {
            currentPrefix = word.mid(1); // 去掉@符号
            timer->start(100);
        } else {
            isAtMode = false;
        }
    } else if (event->key() == Qt::Key_Space || event->key() == Qt::Key_Return) {
        isAtMode = false;
    }
}

void InputTextEdit::focusOutEvent(QFocusEvent *event)
{
    isAtMode = false;
    QTextEdit::focusOutEvent(event);
}

bool InputTextEdit::isCursorInsideAtMention(const QTextCursor &cursor)
{
    int currentPos = cursor.position();

    // 检查光标是否在@和空格之间
    for(int i = 0; i < atEntrys.size(); i++) {
        if(atEntrys[i].startPos < currentPos && currentPos < atEntrys[i].startPos + atEntrys[i].name.size() + 1 + 1) {
            return true;
        }
    }
    return false;
}

void InputTextEdit::handleAtMention(QTextCursor &cursor, bool isMovingForward) {
    int currentPos = cursor.position();

    for(int i = 0; i < atEntrys.size(); i++) {

        if(atEntrys[i].startPos < currentPos && currentPos < atEntrys[i].startPos + atEntrys[i].name.size() + 1 + 1) {
            if(isMovingForward) {
                currentPos = atEntrys[i].startPos + atEntrys[i].name.size() + 1 + 1;
            } else {
                currentPos = atEntrys[i].startPos;
            }
            cursor.setPosition(currentPos);
            setTextCursor(cursor);
        }
    }
}

AtEntry::AtEntry(const QString &name, const QString &userId, int startPos) : name(name),
    userId(userId),
    startPos(startPos)
{}
