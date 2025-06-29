/****************************************************************************
 * EZ Viewer
 * Copyright (C) 2013 huangezhao. CHINA.
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

#ifndef IMAGEVIEWER_CONFIG_H
#define IMAGEVIEWER_CONFIG_H

#include <QColor>
#include <QDir>
#include <QPointF>
#include <QSize>
#include <QSettings>
#include <QVariant>


class QFileSystemWatcher;

class IVConfig : public QObject
{
    Q_OBJECT
public:
signals:
    void configChanged();

public:
    enum ScaleMode {
        ScaleModeBegin = 0,

        ScaleLargeImageToFitWidget = 0,
        KeepImageSize,
        FitWidgetWidth,
        FitWidgetHeight,
        ScaleToFitWidget,
        ScaleToExpandWidget,

        ScaleModeEnd = ScaleToExpandWidget
    };

    enum AlignMode {
        AlignModeBegin = 0,

        AlignLeftTop = 0,
        AlignCenterTop,
        AlignRightTop,
        AlignLeftCenter,
        AlignCenterCenter,
        AlignRightCenter,
        AlignLeftBottom,
        AlignCenterBottom,
        AlignRightBottom,

        AlignModeEnd = AlignRightBottom
    };

    enum AntialiasMode {
        AntialiasModeBegin = 0,

        AntialiasWhenZoomIn = 0,
        AlwaysAntialias,
        NoAntialias,

        AntialiasModeEnd = NoAntialias
    };


    const static qreal ZoomInRatioNomal;
    const static qreal ZoomInRatioSlow;
    const static qreal ZoomInRatioFast;
    const static qreal ZoomOutRatioNomal;
    const static qreal ZoomOutRatioSlow;
    const static qreal ZoomOutRatioFast;

    const static int   DefaultScrollContentSpeed;

    const static qreal ScaleMaxLimit;
    const static qreal ScaleMinLimit;
    const static QPointF OriginPoint;
    const static QSize SizeAdjusted;

    const static QSize WindowMinSize;
    const static QSize WindowFitSize;
    const static QString DefaultBgColor;

    const static int   ClickInterval;
    const static int   ClickThreshold;
    const static int   AutoScrollInterval;
    const static int   FileSizePrecision;

    const static QDir::SortFlags DefaultDirSortFlag;

    const static ScaleMode DefaultScaleMode = ScaleLargeImageToFitWidget;
    const static AlignMode DefaultAlignMode = AlignCenterCenter;
    const static AntialiasMode DefaultAntialiasMode = AntialiasWhenZoomIn;


    /**
     * if config has been changed, the @method of @receiver will be invoked.
     */
    static void insertConfigWatcher(const QObject *receiver, const char *method);
    static void cancelConfigWatcher(const QObject *receiver);

    static void restoreDefaultsConfig();

    static QString supportFormats() { return instance()->mFormats; }
    static QStringList formatsList() { return instance()->mFormatsList; }


    static bool showDialog()    { return instance()->mShowDialog; }
    static ScaleMode scaleMode(){ return instance()->mScaleMode; }
    static AlignMode alignMode(){ return instance()->mAlignMode; }
    static AntialiasMode antialiasMode() { return instance()->mAntialiasMode; }
    static bool enableBgColor() { return instance()->mEnableBgColor; }
    static QColor bgColor()     { return instance()->mBgColor; }
    static int  timerInterval() { return instance()->mTimerInterval; }
    static bool autoRotateImage() { return instance()->mAutoRotateImage; }
    static bool enablePreReading() { return instance()->mEnablePreReading; }
    static int  cacheNum()    { return instance()->mCacheNum; }
    static QByteArray lastGeometry(){ return instance()->mLastGeometry; }

    static void setShowDialog(bool enabled);
    static void setScaleMode(ScaleMode mode);
    static void setAlignMode(AlignMode mode);
    static void setAntialiasMode(AntialiasMode mode);
    static void setEnableBgColor(bool enabled);
    static void setBgColor(const QColor &color);
    static void setTimerInterval(int interval);
    static void setAutoRotateImage(bool enabled);
    static void setEnablePreReading(bool enabled);
    static void setCacheValue(int value);
    static void setLastGeometry(const QByteArray &geometry);


    static void addShortcut(const QString &keySequence, const QString &actionScript);
    static void addShortcut(const QStringList &keySequences, const QString &actionScript);
    static void removeShortcut(const QString &keySequence);
    static void removeShortcut(const QStringList &keySequences);

private slots:
    void fileChanged(const QString &filePath);

private:
    static QString ConfigFilePath();
    static QString ShortcutFilePath();
    static void setValue(const QString &key, const QVariant &value);

    void initConfigValue();
    static void loadAllShortcut();
    static void saveAllShortcut();

    void changeScaleMode(ScaleMode mode); // if current mode is already @mode, then will change to default mode.
    void changeAlignMode(AlignMode mode); // if current mode is already @mode, then will change to default mode.
    void changeAntialiasMode(AntialiasMode mode); // if current mode is already @mode, then will change to default mode.
    void changeBgColorMode() { setEnableBgColor(!enableBgColor()); }
    void changePreReadingMode() { setEnablePreReading(!enablePreReading()); }

    static IVConfig *instance();


    IVConfig();

    void watchConfigFile(const QString &filePath);

    static IVConfig *sInstance;

    QFileSystemWatcher *cfgWatcher;

    bool mShowDialog;
    ScaleMode mScaleMode;
    AlignMode mAlignMode;
    AntialiasMode mAntialiasMode;
    bool mEnableBgColor;
    QColor mBgColor;
    int mTimerInterval;
    bool mAutoRotateImage;
    bool mEnablePreReading;
    int mCacheNum;
    QByteArray mLastGeometry;

    QStringList mFormatsList;
    QString mFormats;
};



inline IVConfig *IVConfig::instance()
{
    if(!sInstance)
        sInstance = new IVConfig();
    return sInstance;
}

inline void IVConfig::setValue(const QString &key, const QVariant &value)
{
    QSettings settings(ConfigFilePath(), QSettings::IniFormat);
    settings.setValue(key, value);
}

#endif // IMAGEVIEWER_CONFIG_H
