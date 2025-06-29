#ifndef EMOJDIALOG_H
#define EMOJDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QDialog>
#include <QTableWidget>
#include <QVBoxLayout>

class EmojDialog : public QDialog {
    Q_OBJECT
public:
    EmojDialog(QWidget *parent = nullptr);

signals:
    void emojiSelected(const QString &emoj);

private:
    QTableWidget *tableWidget;
    QStringList emojis;
};

#endif // EMOJDIALOG_H
