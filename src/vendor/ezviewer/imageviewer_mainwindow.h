/****************************************************************************
 * EZ Viewer
 * Copyright (C) 2012 huangezhao. CHINA.
 * Contact: huangezhao (huangezhao@gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 ***************************************************************************/

#ifndef IMAGEVIEWER_IVMainWindow_H
#define IMAGEVIEWER_IVMainWindow_H

#include <QMainWindow>
#include <QTimer>

#include "picmanager.h"

namespace WFCLib {
    class Message;
};

class FloatFrame;
class ContralBar;
class QLabel;
class QMenu;
class QAction;
class QPushButton;

class IVMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit IVMainWindow(QWidget *parent = 0);

    void parseCmd(QStringList args);
    static void preProcessCmd(const QStringList &args);

    void showImageMessage(const WFCLib::Message &imageMessage);
protected slots:
    void closeEvent(QCloseEvent *event);
    void dragEnterEvent(QDragEnterEvent * event);
    void dropEvent(QDropEvent * event);
    void keyPressEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent *event);

    bool eventFilter(QObject *obj, QEvent *event);

    void applyConfig();
    void imageChanged(const QString &fileName = QString());
    void showContextMenu(const QPoint &pos);

    void openFile();
    void showInExplorer();
    void changeFullScreen();
    void showAttribute();
    void switchSlideShow();
    void setting();
    void about();

private:
    void readSettings();
    void writeSettings();

    void initContextMenu();
    void initButtomBar();
    void initSwitchFrame(); // init left and right float frame

    void registerAllFunction();

    void changeTimerInterval(int sec) { slideTimer->setInterval(sec * 1000); }

private:
    PicManager *viewer;
    bool wasMaximized;
    QTimer *slideTimer;
    int slideInterval; // msec
    QRect attributeRect;

    FloatFrame *bottomFrame;
    FloatFrame *leftFrame;
    FloatFrame *rightFrame;

    ContralBar *contralBar;
    QPushButton *settingButton;
    QPushButton *openButton;
    QPushButton *preButton;
    QPushButton *playButton;
    QPushButton *nextButton;
    QPushButton *rotateLeftButton;
    QPushButton *rotateRightButton;
    QPushButton *deleteButton;

    QMenu *contextMenu;
    QAction *openAction;
    QAction *locateAction;
    QAction *slideAction;
    QAction *rotateLeftAction;
    QAction *rotateRightAction;
    QAction *mirrorHAction;
    QAction *mirrorVAction;
    QAction *copyAction;
    QAction *deleteAction;
    QAction *attributeAction;
};


#endif // IMAGEVIEWER_IVMainWindow_H
