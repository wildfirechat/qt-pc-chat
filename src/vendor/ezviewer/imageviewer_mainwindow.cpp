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

#include "imageviewer_mainwindow.h"

#include "imageviewer_config.h"
#include "global.h"
#include "contralbar.h"
#include "floatframe.h"
#include "osrelated.h"
#include "toolkit.h"
#include "tooltip.h"
#include "actionmanager.h"
#include "settingdialog.h"
#include "../../wfc/client/ChatClient.h"

#include <QtGui>

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QtWidgets>
#endif // QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)


#define GET_SCRIPT(x) #x
#define SPLIT_FUNCTION(x) &x, #x
#define SPLIT_PARAM(x) #x, x

const int SWITCH_FRAME_WIDTH = 90;
const int BUTTOM_FRAME_HEIGHT = 60;
const int ATTRIBUTE_RECT_WIDTH = 100;
const int ATTRIBUTE_RECT_HEIGHT = 100;


IVMainWindow::IVMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    viewer = new PicManager(this);
    setCentralWidget(viewer);
    viewer->installEventFilter(this);

    wasMaximized = false;
    slideInterval = 4000;
    slideTimer = new QTimer(this);
    slideTimer->setInterval(slideInterval);
    //! if file numbers is 0 or 1, stop timer???......................
    connect(slideTimer, SIGNAL(timeout()), viewer, SLOT(nextPic()));

    connect(viewer, SIGNAL(imageChanged(QString)), SLOT(imageChanged(QString)));

    initContextMenu();
    initButtomBar();
    initSwitchFrame();

    registerAllFunction();

    resize(IVConfig::WindowFitSize);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QRect rect = QApplication::desktop()->availableGeometry();
#else
    QRect rect = QGuiApplication::primaryScreen()->availableGeometry();
#endif

    QPoint pos = QPoint((rect.width() - IVConfig::WindowFitSize.width()) / 2 + rect.left(),
                        (rect.height() - IVConfig::WindowFitSize.height()) / 2 + rect.top());
    move(pos);  // show this window in the center of Desktop
    readSettings();
    IVConfig::insertConfigWatcher(this, SLOT(applyConfig()));

    imageChanged();
    setWindowIcon(QIcon(":/appIcon"));
    setAcceptDrops(true);   //! !!
}

void IVMainWindow::closeEvent(QCloseEvent *event)
{
    IVConfig::cancelConfigWatcher(this);
    if(!isFullScreen())
        writeSettings();
    event->accept();
}

void IVMainWindow::about()
{
    QMessageBox::about(this, tr("About %1").arg(Global::ProjectName()),
                       Global::AboutInfo());
}

void IVMainWindow::setting()
{
    SettingDialog dlg(this);
    dlg.exec();
}

void IVMainWindow::imageChanged(const QString &fileName)
{
    bool hasFile = !fileName.isEmpty();
    bool hasPicture = viewer->hasPicture();

    setWindowTitle(hasFile
                   ? QString("%1 - %2").arg(fileName).arg(Global::ProjectName())
                   : Global::ProjectName());
    ToolTip::hideText();
    QWhatsThis::hideText();

    preButton->setEnabled(hasFile);
    playButton->setEnabled(hasFile);
    nextButton->setEnabled(hasFile);
    leftFrame->set_enabled(hasFile);    ///
    rightFrame->set_enabled(hasFile);   ///
    rotateLeftButton->setEnabled(hasPicture);
    rotateRightButton->setEnabled(hasPicture);
    deleteButton->setEnabled(!slideTimer->isActive() && hasFile);   ////

    // TODO: if hasFile() , show tooltip ?

    if(!hasFile && slideTimer->isActive())  // TODO: if only one file, stop slide show.
        switchSlideShow();    ///
}

void IVMainWindow::openFile()
{
    QString currentFile(viewer->filePath());
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    const QString systemPicturesPath = QDesktopServices::storageLocation(QDesktopServices::PicturesLocation);
#else
    const QStringList systemPaths = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    const QString systemPicturesPath = systemPaths.empty() ? QString() : systemPaths.first();
#endif // QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QString defaultDir(currentFile.isEmpty() ? systemPicturesPath
                       : QFileInfo(currentFile).absolutePath());
    QString fileName =
            QFileDialog::getOpenFileName(
                this, tr("Open File"), defaultDir,
                tr("Images (%1);;All Files (*)").arg(IVConfig::supportFormats()));
    if (!fileName.isEmpty())
        viewer->openFile(fileName);
}

void IVMainWindow::showInExplorer()
{
    if(viewer->hasFile()){
        OSRelated::showFileInExplorer(viewer->filePath());
    }
}

void IVMainWindow::readSettings()
{
    applyConfig();    ///
    if (!IVConfig::lastGeometry().isEmpty())
        restoreGeometry(IVConfig::lastGeometry());
}

void IVMainWindow::applyConfig()
{
    viewer->changeScaleMode(IVConfig::scaleMode());
    viewer->changeAlignMode(IVConfig::alignMode());
    viewer->changeAntialiasMode(IVConfig::antialiasMode());
    if(IVConfig::enableBgColor())
        viewer->changeBgColor(IVConfig::bgColor());
    else
        viewer->changeBgColor(QColor());
    changeTimerInterval(IVConfig::timerInterval());
    viewer->setCacheNumber(IVConfig::cacheNum());
    viewer->setPreReadingEnabled(IVConfig::enablePreReading());
}

void IVMainWindow::writeSettings()
{
    IVConfig::setLastGeometry(saveGeometry());
}

void IVMainWindow::dragEnterEvent(QDragEnterEvent *event)
{
//    if (event->mimeData()->hasFormat("text/uri-list")) {
//        event->acceptProposedAction();
//    }

    const QMimeData *mimeData = event->mimeData();
    if (event->mimeData()->hasUrls()){
        QList<QUrl> urlList(mimeData->urls());
        QFileInfo fileInfo;
        for (int i = 0; i < urlList.size(); ++i) {
            fileInfo.setFile(urlList.at(i).toLocalFile());
            if(fileInfo.isFile()){
//                    && FORMAT_LIST.contains(fileInfo.suffix().toLower())){
                event->acceptProposedAction();
                break;
            }
        }
    }
}

void IVMainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        QList<QUrl> urlList(mimeData->urls());
        QStringList fileList;
        for(int size = urlList.size(), i=0; i < size; ++i)
            fileList.append(urlList.at(i).toLocalFile());
        fileList = ToolKit::getFilesExist(fileList);   ///
        if(!fileList.empty())
            viewer->openFiles(fileList);
    }

    event->acceptProposedAction();
}

void IVMainWindow::changeFullScreen()
{
//    setWindowState( windowState() ^ Qt::WindowFullScreen );
    if(isFullScreen()){
        if(wasMaximized)
            showMaximized();
        else
            showNormal();
    }else{
        wasMaximized = isMaximized();    //! only for windows?
        showFullScreen();
    }
}

void IVMainWindow::showAttribute()
{
    if(viewer->hasPicture() || viewer->hasFile()){
#ifndef QT_NO_WHATSTHIS
        QWhatsThis::showText(QCursor::pos(), viewer->attribute(), this);
#else
        QMessageBox::information(this, tr("Property"), viewer->attribute());
#endif
    }
}

void IVMainWindow::switchSlideShow()// if other commend when slide show??
{
    if(!viewer->hasFile() && !slideTimer->isActive())  ////for setMyWindowTitle() call
        return;

    if(slideTimer->isActive()){
        slideTimer->stop();
        slideAction->setIcon(QIcon(":/Play.png"));
        slideAction->setText(tr("Auto Play"));
        playButton->setIcon(QIcon(":/Play.png"));
        playButton->setToolTip(tr("Auto Play"));
        openButton->setEnabled(true);
        deleteButton->setEnabled(viewer->hasFile());    ///
    }else{
        slideTimer->start();
        slideAction->setIcon(QIcon(":/Stop.png"));
        slideAction->setText(tr("Stop Play"));
        playButton->setIcon(QIcon(":/Stop.png"));
        playButton->setToolTip(tr("Stop Play"));
        openButton->setEnabled(false);
        deleteButton->setEnabled(false);
    }
}

void IVMainWindow::resizeEvent(QResizeEvent *event)
{
    bottomFrame->resize(width(), BUTTOM_FRAME_HEIGHT);
    bottomFrame->move(0, height() - BUTTOM_FRAME_HEIGHT);
    leftFrame->resize(SWITCH_FRAME_WIDTH, height() - BUTTOM_FRAME_HEIGHT - ATTRIBUTE_RECT_HEIGHT);
    leftFrame->move(0, ATTRIBUTE_RECT_HEIGHT);
    rightFrame->resize(SWITCH_FRAME_WIDTH, height() - BUTTOM_FRAME_HEIGHT - ATTRIBUTE_RECT_HEIGHT);
    rightFrame->move(width() - SWITCH_FRAME_WIDTH, ATTRIBUTE_RECT_HEIGHT);

    attributeRect = QRect(width() - ATTRIBUTE_RECT_WIDTH, 0, ATTRIBUTE_RECT_WIDTH, ATTRIBUTE_RECT_HEIGHT);

    QWidget::resizeEvent(event);
}

void IVMainWindow::initButtomBar()
{
    bottomFrame = new FloatFrame(this);
    ///set all the button's focous policy to Qt::NoFocous in 'ui' file.

    contralBar = new ContralBar(bottomFrame);
    bottomFrame->addWidget(contralBar);

    QHBoxLayout *hlayout = new QHBoxLayout(bottomFrame);
    hlayout->setContentsMargins(0,0,0,0);   ///qframe's layout margis default is not 0.
    hlayout->setAlignment(Qt::AlignCenter);
    hlayout->addWidget(contralBar);
    bottomFrame->setLayout(hlayout);

    settingButton = contralBar->settingButton;
    openButton = contralBar->openButton;
    preButton = contralBar->preButton;
    playButton = contralBar->playButton;
    nextButton = contralBar->nextButton;
    rotateLeftButton = contralBar->rotateLeftButton;
    rotateRightButton = contralBar->rotateRightButton;
    deleteButton = contralBar->deleteButton;

    connect(settingButton, SIGNAL(clicked()), SLOT(setting()));
    connect(openButton, SIGNAL(clicked()), SLOT(openFile()));
    connect(preButton, SIGNAL(clicked()), viewer, SLOT(prePic()));
    connect(playButton, SIGNAL(clicked()), SLOT(switchSlideShow()));
    connect(nextButton, SIGNAL(clicked()), viewer, SLOT(nextPic()));
    connect(rotateLeftButton, SIGNAL(clicked()), viewer, SLOT(rotateLeft()));
    connect(rotateRightButton, SIGNAL(clicked()), viewer, SLOT(rotateRight()));
    connect(deleteButton, SIGNAL(clicked()), viewer, SLOT(deleteFileAsk()));

    bottomFrame->installEventFilter(this);
}

void IVMainWindow::initSwitchFrame()
{
    leftFrame = new FloatFrame(this);
    leftFrame->setFillBackground(false);
    leftFrame->setHideInterval(400);
    connect(leftFrame, SIGNAL(mouseClicked()), viewer, SLOT(prePic()));

    QLabel *lb = new QLabel(leftFrame);
    lb->setPixmap(QPixmap(":/Left2.png"));
    leftFrame->addWidget(lb);

    QHBoxLayout *hlayout = new QHBoxLayout(leftFrame);
    hlayout->setAlignment(Qt::AlignCenter);
    hlayout->addWidget(lb);
    leftFrame->setLayout(hlayout);


    rightFrame = new FloatFrame(this);
    rightFrame->setFillBackground(false);
    rightFrame->setHideInterval(400);
    connect(rightFrame, SIGNAL(mouseClicked()), viewer, SLOT(nextPic()));

    lb = new QLabel(rightFrame);
    lb->setPixmap(QPixmap(":/Right2.png"));
    rightFrame->addWidget(lb);

    hlayout = new QHBoxLayout(rightFrame);
    hlayout->setAlignment(Qt::AlignCenter);
    hlayout->addWidget(lb);
    rightFrame->setLayout(hlayout);


    leftFrame->installEventFilter(this);
    rightFrame->installEventFilter(this);
}

void IVMainWindow::initContextMenu()
{
    QAction *settingAction = new QAction(QIcon(":/Setting.png"),
                                         tr("&Setting"), this);
    connect(settingAction, SIGNAL(triggered()), SLOT(setting()));

    QAction *aboutAction = new QAction(tr("&About"), this);
    connect(aboutAction, SIGNAL(triggered()), SLOT(about()));

    QAction *closeAction = new QAction(tr("&Quit"), this);
    connect(closeAction, SIGNAL(triggered()), SLOT(close()));

    openAction = new QAction(QIcon(":/Open.png"),
//                style()->standardIcon(QStyle::SP_DialogOpenButton),
                tr("&Open"), this);
    connect(openAction, SIGNAL(triggered()), SLOT(openFile()));

    if (OSRelated::supportShowFileInExplorer()) {
        locateAction = new QAction(tr("Show in &Explorer"), this);
        connect(locateAction, SIGNAL(triggered()), SLOT(showInExplorer()));
    } else {
        locateAction = NULL;
    }

    slideAction = new QAction(QIcon(":/Play.png"), tr("Auto Play"), this);
    connect(slideAction, SIGNAL(triggered()), SLOT(switchSlideShow()));

    rotateLeftAction = new QAction(QIcon(":/Undo.png"), tr("Rotate &Left"), this);
    connect(rotateLeftAction, SIGNAL(triggered()), viewer, SLOT(rotateLeft()));

    rotateRightAction = new QAction(QIcon(":/Redo.png"),
                                    tr("Rotate &Right"), this);
    connect(rotateRightAction, SIGNAL(triggered()), viewer, SLOT(rotateRight()));

    mirrorHAction = new QAction(tr("Mirrored &Horizontal"), this);
    connect(mirrorHAction, SIGNAL(triggered()), viewer, SLOT(mirrorHorizontal()));

    mirrorVAction = new QAction(tr("Mirrored &Vertical"), this);
    connect(mirrorVAction, SIGNAL(triggered()), viewer, SLOT(mirrorVertical()));

    copyAction = new QAction(tr("&Copy to clipboard"), this);
    connect(copyAction, SIGNAL(triggered()), viewer, SLOT(copyToClipboard()));

    attributeAction = new QAction(QIcon(":/Info.png"), tr("&Property"), this);
    connect(attributeAction, SIGNAL(triggered()), SLOT(showAttribute()));

    deleteAction = new QAction(QIcon(":/Delete.png"),
//                style()->standardIcon(QStyle::SP_DialogCloseButton),
                tr("&Delete"), this);
    connect(deleteAction, SIGNAL(triggered()), viewer, SLOT(deleteFileAsk()));

    contextMenu = new QMenu(this);
    contextMenu->addAction(openAction);
    if (locateAction)
        contextMenu->addAction(locateAction);
    contextMenu->addAction(slideAction);
    contextMenu->addSeparator();
    contextMenu->addAction(rotateLeftAction);
    contextMenu->addAction(rotateRightAction);
    contextMenu->addAction(mirrorHAction);
    contextMenu->addAction(mirrorVAction);
    contextMenu->addSeparator();
    contextMenu->addAction(copyAction);
    contextMenu->addAction(deleteAction);
    contextMenu->addAction(attributeAction);
    contextMenu->addSeparator();
    contextMenu->addAction(settingAction);
    contextMenu->addAction(aboutAction);
    contextMenu->addSeparator();
    contextMenu->addAction(closeAction);
}

void IVMainWindow::showContextMenu(const QPoint &pos)
{
    bool hasPixmap = viewer->hasPicture();
    bool has_file = viewer->hasFile();
    //! QMenu is top-level window, no matter hidden or shown.
    bool notSliding = (!slideTimer->isActive());

    openAction->setEnabled(notSliding);
    if (locateAction)
        locateAction->setEnabled(has_file);
    slideAction->setEnabled(has_file);
    rotateLeftAction->setEnabled(hasPixmap);
    rotateRightAction->setEnabled(hasPixmap);
    mirrorHAction->setEnabled(hasPixmap);
    mirrorVAction->setEnabled(hasPixmap);
    copyAction->setEnabled(hasPixmap);
    deleteAction->setEnabled(has_file && notSliding);
    attributeAction->setEnabled(hasPixmap || has_file); //

    contextMenu->popup(pos);
}

void IVMainWindow::parseCmd(QStringList args)
{
    args.removeFirst(); // remove name of executable
    if(!args.empty()){
        viewer->openFiles(args);
    }else{
        if(IVConfig::showDialog()) // show dialog while launch.
            openFile();
    }
}

void IVMainWindow::preProcessCmd(const QStringList &args)
{
    if(args.size() > 1){    // first one is name of executable path
        QFileInfo fi(args.at(1));
        if (fi.exists() && fi.isFile())
            ImageFactory::preReading(fi.absoluteFilePath()); // TODO: if the computer is one-core and user sets pre-reading enabled?
    }
}

void IVMainWindow::showImageMessage(const WFCLib::Message &imageMessage)
{
    viewer->openImageMessage(imageMessage);
    show();
}

void IVMainWindow::registerAllFunction()
{
    ActionManager::registerFunction(tr("Open"),
                   this, SPLIT_FUNCTION(IVMainWindow::openFile));
    ActionManager::registerFunction(tr("Full Screen"),
                   this, SPLIT_FUNCTION(IVMainWindow::changeFullScreen));
    ActionManager::registerFunction(tr("Auto Play/Stop"),
                   this, SPLIT_FUNCTION(IVMainWindow::switchSlideShow));
    ActionManager::registerFunction(tr("Property"),
                   this, SPLIT_FUNCTION(IVMainWindow::showAttribute));
    ActionManager::registerFunction(tr("Setting"),
                   this, SPLIT_FUNCTION(IVMainWindow::setting));
    ActionManager::registerFunction(tr("About"),
                   this, SPLIT_FUNCTION(IVMainWindow::about));
    ActionManager::registerFunction(tr("Quit"),
                   this, SPLIT_FUNCTION(IVMainWindow::close));
    if (OSRelated::supportShowFileInExplorer()) {
        ActionManager::registerFunction(tr("Show in Explorer"),
                       this, SPLIT_FUNCTION(IVMainWindow::showInExplorer));
    }

    ActionManager::registerFunction(tr("Next Picture"),
                   viewer, SPLIT_FUNCTION(PicManager::nextPic));
    ActionManager::registerFunction(tr("Previous Picture"),
                   viewer, SPLIT_FUNCTION(PicManager::prePic));
    ActionManager::registerFunction(tr("Rotate Left"),
                   viewer, SPLIT_FUNCTION(PicManager::rotateLeft));
    ActionManager::registerFunction(tr("Rotate Right"),
                   viewer, SPLIT_FUNCTION(PicManager::rotateRight));
    ActionManager::registerFunction(tr("Mirrored Horizontal"),
                   viewer, SPLIT_FUNCTION(PicManager::mirrorHorizontal));
    ActionManager::registerFunction(tr("Mirrored Vertical"),
                   viewer, SPLIT_FUNCTION(PicManager::mirrorVertical));
    ActionManager::registerFunction(tr("Animation Play/Pause"),
                   viewer, SPLIT_FUNCTION(PicManager::switchAnimationState));
    ActionManager::registerFunction(tr("Frame Step"),
                   viewer, SPLIT_FUNCTION(PicManager::nextAnimationFrame));
    ActionManager::registerFunction(tr("Copy to clipboard"),
                   viewer, SPLIT_FUNCTION(PicManager::copyToClipboard));
    ActionManager::registerFunction(tr("Delete"),
                   viewer, SPLIT_FUNCTION(PicManager::deleteFileAsk));
    ActionManager::registerFunction(tr("Delete Without Notification"),
                   viewer, SPLIT_FUNCTION(PicManager::deleteFileNoAsk));

    ActionManager::registerFunction(tr("Zoom In"), viewer,
                   &PicManager::zoomIn, SPLIT_PARAM(IVConfig::ZoomInRatioNomal));
    ActionManager::registerFunction(tr("Zoom In (Slow)"), viewer,
                   &PicManager::zoomIn, SPLIT_PARAM(IVConfig::ZoomInRatioSlow));
    ActionManager::registerFunction(tr("Zoom in (Fast)"), viewer,
                   &PicManager::zoomIn, SPLIT_PARAM(IVConfig::ZoomInRatioFast));
    ActionManager::registerFunction(tr("Zoom Out"), viewer,
                   &PicManager::zoomIn, SPLIT_PARAM(IVConfig::ZoomOutRatioNomal));
    ActionManager::registerFunction(tr("Zoom Out (Slow)"), viewer,
                   &PicManager::zoomIn, SPLIT_PARAM(IVConfig::ZoomOutRatioSlow));
    ActionManager::registerFunction(tr("Zoom Out (Fast)"), viewer,
                   &PicManager::zoomIn, SPLIT_PARAM(IVConfig::ZoomOutRatioFast));

    ActionManager::registerFunction(tr("Scroll Content Up"),
                   viewer, &PicManager::scrollContent, "scrollContentUp",
                   0, IVConfig::DefaultScrollContentSpeed);
    ActionManager::registerFunction(tr("Scroll Content Left"),
                   viewer, &PicManager::scrollContent, "scrollContentLeft",
                   IVConfig::DefaultScrollContentSpeed, 0);
    ActionManager::registerFunction(tr("Scroll Content Right"),
                   viewer, &PicManager::scrollContent, "scrollContentRight",
                   -IVConfig::DefaultScrollContentSpeed, 0);
    ActionManager::registerFunction(tr("Scroll Content Down"),
                   viewer, &PicManager::scrollContent, "scrollContentDown",
                   0, -IVConfig::DefaultScrollContentSpeed);
}

void IVMainWindow::keyPressEvent(QKeyEvent *e)
{
    static bool locked = false; // this function will be called only in main thread, so doesn't need using mutex.
    if (locked)
        return;
    else
        locked = true;

    QKeySequence keys((int)e->modifiers() + e->key());

    if (slideTimer->isActive()) {
        QString action = ActionManager::getMatchAction(keys.toString());
        if (action == GET_SCRIPT(IVMainWindow::openFile)
                || action == GET_SCRIPT(PicManager::deleteFileNoAsk)
                || action == GET_SCRIPT(PicManager::deleteFileAsk) ) {
            locked = false;
            return;
        }
    }

    if(ActionManager::run(keys.toString())) {
        e->accept();
    } else {
        QWidget::keyPressEvent(e);
    }

    locked = false;
}

bool IVMainWindow::eventFilter(QObject *obj, QEvent *event)
{
    switch(event->type()) {
    case QEvent::ToolTip:
    {
        if (obj != viewer) break;

        QHelpEvent* e = static_cast<QHelpEvent*>(event);
        if (attributeRect.contains(e->pos())){
            QString attribute = viewer->attribute();
            if(!attribute.isEmpty())
                ToolTip::showText(e->globalPos(),
                                  attribute.prepend("<b>").append("</b>"),
                                  false, 0.8);
        }
        return true;
    }
    case QEvent::MouseButtonDblClick:
    {
        if (obj != viewer && obj != bottomFrame) break;

        QMouseEvent *e = static_cast<QMouseEvent*>(event);
        if(e->button() & Qt::LeftButton)
            changeFullScreen();
        return true;
    }
    case QEvent::ContextMenu:
    {
        QContextMenuEvent *e = static_cast<QContextMenuEvent*>(event);
        showContextMenu(e->globalPos());
        return true;
    }
    case QEvent::Wheel:
    {
        QWheelEvent *e = static_cast<QWheelEvent *>(event);
        qreal factor = 0.1;
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
        int delta = e->angleDelta().y();
#else
        int delta = e->delta();
#endif

        switch(e->modifiers()){
        case Qt::ShiftModifier:
            factor = delta / qreal(2400); // e->delta() is +120 or -120
            break;
        case Qt::ControlModifier:
            factor = delta / qreal(600);
            break;
        default:
            factor = delta / qreal(1200);
            break;
        }
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
        QPointF globalPos = e->position().toPoint();
        viewer->zoomIn(factor, viewer->mapFromGlobal(globalPos.toPoint()));
#else
        viewer->zoomIn(factor, viewer->mapFromGlobal(e->globalPos()));
#endif
        break;
    }
    default:
        break;
    }

    return false;
}
