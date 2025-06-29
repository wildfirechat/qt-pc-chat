#ifndef FAVORITEDETAILWIDGET_H
#define FAVORITEDETAILWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QFileDialog>
#include <QDesktopServices>

class FavoriteDetailWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FavoriteDetailWidget(QWidget *parent = nullptr);
    ~FavoriteDetailWidget();
    
    void setFavoriteId(const QString &favoriteId);

private slots:
    void onOpenFile();
    void onRemoveFavorite();

private:
    void setupUI();
    void loadFavoriteInfo(const QString &favoriteId);
    
    QString currentFavoriteId;
    QLabel *typeLabel;
    QLabel *nameLabel;
    QLabel *timeLabel;
    QLabel *sizeLabel;
    QTextEdit *contentEdit;
    QLabel *previewLabel;
    QString filePath;
};

#endif // FAVORITEDETAILWIDGET_H    