#include "friendselectiondialog.h"
#include <QMessageBox>
#include "friendselectabledelegate.h"
#include "selectedfrienddelegate.h"

// 好友选择对话框实现
FriendSelectionDialog::FriendSelectionDialog(QWidget *parent) : QDialog(parent)
{
    m_friendModel = new FriendSelectableModel(this);
    m_selectedModel = new QStandardItemModel(this);

    setupUI();
}

FriendSelectionDialog::~FriendSelectionDialog()
{
}

void FriendSelectionDialog::setupUI()
{
    // 设置对话框标题和大小
    setWindowTitle("选择好友");
    setMinimumSize(600, 400);

    // 创建主布局
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // 创建左侧好友列表区域
    QVBoxLayout *leftLayout = new QVBoxLayout;
    QLabel *friendsLabel = new QLabel("好友列表", this);

    m_friendListView = new QListView(this);
    m_friendListView->setModel(m_friendModel);
    m_friendListView->setItemDelegate(new FriendSelectableDelegate(this));
    m_friendListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_friendListView->setSelectionMode(QAbstractItemView::NoSelection);

    leftLayout->addWidget(friendsLabel);
    leftLayout->addWidget(m_friendListView);

    // 创建右侧已选择好友区域
    QVBoxLayout *rightLayout = new QVBoxLayout;
    QLabel *selectedLabel = new QLabel("已选择的好友", this);

    m_selectedFriendsListView = new QListView(this);
    m_selectedFriendsListView->setModel(m_selectedModel);
    m_selectedFriendsListView->setItemDelegate(new SelectedFriendDelegate(this));
    m_selectedFriendsListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_selectedFriendsListView->setSelectionMode(QAbstractItemView::NoSelection);

    // 创建底部按钮区域
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    m_cancelButton = new QPushButton("取消", this);
    m_finishButton = new QPushButton("完成", this);

    buttonLayout->addStretch();
    buttonLayout->addWidget(m_cancelButton);
    buttonLayout->addWidget(m_finishButton);

    rightLayout->addWidget(selectedLabel);
    rightLayout->addWidget(m_selectedFriendsListView);
    rightLayout->addLayout(buttonLayout);

    // 添加左右两部分到主布局
    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addLayout(rightLayout, 1);

    // 连接信号和槽
    connect(m_friendModel, &QAbstractListModel::dataChanged, this, &FriendSelectionDialog::onFriendListItemChanged);
    connect(m_selectedFriendsListView, &QListView::clicked, this, &FriendSelectionDialog::onSelectedFriendClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &FriendSelectionDialog::onCancelButtonClicked);
    connect(m_finishButton, &QPushButton::clicked, this, &FriendSelectionDialog::onFinishButtonClicked);
}

QStringList FriendSelectionDialog::selectedFriends() const
{
    QStringList selected;
    return m_friendModel->getCheckedIds();
}

void FriendSelectionDialog::onFriendListItemChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    Q_UNUSED(topLeft);
    Q_UNUSED(bottomRight);
    Q_UNUSED(roles);

    updateSelectedFriendsList();
}

void FriendSelectionDialog::onSelectedFriendClicked(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    QString friendName = index.data(Qt::UserRole).toString();

    // 在好友列表中找到对应的项并取消选择
    m_friendModel->uncheckId(friendName);
}

void FriendSelectionDialog::updateSelectedFriendsList()
{
    m_selectedModel->clear();
    foreach (auto fid, m_friendModel->getCheckedIds()) {
        QStandardItem *selectedItem = new QStandardItem(fid + "  [删除]");
        selectedItem->setData(fid, Qt::UserRole);
        m_selectedModel->appendRow(selectedItem);
    }
}

void FriendSelectionDialog::onCancelButtonClicked()
{
    reject(); // 关闭对话框并返回Rejected
}

void FriendSelectionDialog::onFinishButtonClicked()
{
    if (selectedFriends().isEmpty()) {
        QMessageBox::information(this, "提示", "请选择至少一个好友");
        return;
    }

    accept(); // 关闭对话框并返回Accepted
}
