#include "emojdialog.h"
#include <QHeaderView>
#include <QDebug>

#define COUNT_IN_ONE_LINE 9
#define EMOJ_SIZE 50
EmojDialog::EmojDialog(QWidget *parent) : QDialog(parent), emojis({"😀", "😃", "😄", "😁", "😆", "😅", "😂", "🤣", "😊", "😇",
              "🙂", "🙃", "😉", "😌", "😍", "🥰", "😘", "😗", "😙", "😚",
              "😋", "😛", "😝", "😜", "🤪", "🤨", "🧐", "🤓", "😎", "🤩"}) {
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    setFixedSize(COUNT_IN_ONE_LINE*EMOJ_SIZE+2, ((emojis.size()-1)/COUNT_IN_ONE_LINE+1)*EMOJ_SIZE+2);

    // 创建表格
    tableWidget = new QTableWidget(0, COUNT_IN_ONE_LINE, this);
    tableWidget->horizontalHeader()->setVisible(false);
    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->horizontalHeader()->setDefaultSectionSize(EMOJ_SIZE);
    tableWidget->verticalHeader()->setDefaultSectionSize(EMOJ_SIZE);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tableWidget->setShowGrid(false);
    tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setFocusPolicy(Qt::NoFocus);
    QFont font;
    font.setPointSize(20);
    tableWidget->setFont(font);


    // 点击表格外区域关闭对话框
    setAttribute(Qt::WA_TranslucentBackground);

    for(int i = 0; i < emojis.size(); i++) {
        int row = i/COUNT_IN_ONE_LINE;
        int column = i%COUNT_IN_ONE_LINE;
        if(column == 0) {
            tableWidget->insertRow(row);
        }
        QTableWidgetItem *item = new QTableWidgetItem(emojis.at(i));
        item->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, column, item);
    }


    // 设置布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(tableWidget);
    layout->setContentsMargins(0, 0, 0, 0);

    // 连接信号与槽
    connect(tableWidget, &QTableWidget::cellClicked, [this](int row, int col) {
        QString text = tableWidget->item(row, col)->text();
        emit emojiSelected(text);
        this->hide();
    });
}
