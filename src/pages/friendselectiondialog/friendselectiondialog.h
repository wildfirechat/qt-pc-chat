#ifndef FRIENDSELECTIONDIALOG_H
#define FRIENDSELECTIONDIALOG_H

#include <QDialog>
#include <QListView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include "friendselectablemodel.h"


class FriendSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FriendSelectionDialog(QWidget *parent = nullptr);
    ~FriendSelectionDialog();

    // 获取已选择的好友列表
    QStringList selectedFriends() const;

private slots:
    // 处理左侧列表项状态变化
    void onFriendListItemChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());
    // 处理右侧列表项被点击
    void onSelectedFriendClicked(const QModelIndex &index);
    // 处理取消按钮被点击
    void onCancelButtonClicked();
    // 处理完成按钮被点击
    void onFinishButtonClicked();

private:
    // 创建UI组件
    void setupUI();
    // 更新右侧已选择好友列表
    void updateSelectedFriendsList();

    QListView *m_friendListView;          // 左侧好友列表
    QListView *m_selectedFriendsListView; // 右侧已选择好友列表
    QPushButton *m_cancelButton;          // 取消按钮
    QPushButton *m_finishButton;          // 完成按钮
    FriendSelectableModel *m_friendModel;
    QStandardItemModel *m_selectedModel;  // 已选择好友模型
};

#endif // FRIENDSELECTIONDIALOG_H
