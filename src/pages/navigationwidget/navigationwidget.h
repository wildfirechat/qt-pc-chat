#ifndef NAVIGATIONWIDGET_H
#define NAVIGATIONWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include "../settings/settingsdialog.h"

class AvatarWidget;;
class BadgeButton;
class NavigationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NavigationWidget(QWidget *parent = nullptr);
    ~NavigationWidget();

signals:
    void navItemSelected(int index);
    void showUserDialog(const QString &userId, const QString &groupId, const QPoint &position);

private slots:
    void onChatBtnClicked();
    void onContactBtnClicked();
    void onFavoriteBtnClicked();
    void onTagBtnClicked();
    void onSettingsBtnClicked();
    void onAvatarClicked(const QPoint &position);

private:
    void setupUI();
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    QPushButton* createNavButton(const QString &ic_normal, const QString &title);
    BadgeButton* createBadgeNavButton(const QString &ic_normal, const QString &title);

    void reloadBadge();

    AvatarWidget *avatarLabel;

    BadgeButton *chatBtn;
    BadgeButton *contactBtn;
    QPushButton *favoriteBtn;
    QPushButton *tagBtn;
    QPushButton *settingsBtn;
    SettingsDialog *settingsDialog;
    bool m_dragging;
    QPoint m_dragPosition;
};

#endif // NAVIGATIONWIDGET_H
