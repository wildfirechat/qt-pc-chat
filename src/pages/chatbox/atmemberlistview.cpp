#include "atmemberlistview.h"
#include <QKeyEvent>

AtMemberListView::AtMemberListView(QWidget *parent) : QListView(parent)
{}


void AtMemberListView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        hide(); // 按下ESC键时隐藏窗口
    } else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        // 获取选中项
        QModelIndexList selected = selectionModel()->selectedIndexes();
        if (!selected.isEmpty()) {
            emit clicked(selected.first());
        }
    } else {
        QListView::keyPressEvent(event); // 其他按键交给基类处理
    }
}
