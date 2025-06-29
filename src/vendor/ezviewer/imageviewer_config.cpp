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

#include "imageviewer_config.h"
#include "osrelated.h"
#include "actionmanager.h"
#include "qxmlputget.h"

#include <QFileSystemWatcher>
#include <QImageReader>

#define SPLIT_FUNCTION(x) &x, #x
#define SPLIT_PARAM(x) #x, x


const qreal IVConfig::ZoomInRatioNomal = 0.1;
const qreal IVConfig::ZoomInRatioSlow = 0.05;
const qreal IVConfig::ZoomInRatioFast = 0.2;
const qreal IVConfig::ZoomOutRatioNomal = -0.1;
const qreal IVConfig::ZoomOutRatioSlow = -0.05;
const qreal IVConfig::ZoomOutRatioFast = -0.2;
const int   IVConfig::DefaultScrollContentSpeed = 15;

const qreal IVConfig::ScaleMaxLimit = 20.0;
const qreal IVConfig::ScaleMinLimit = 0.1;
const QPointF IVConfig::OriginPoint(0.0, 0.0);
const QSize IVConfig::SizeAdjusted(0, 1);
const QSize IVConfig::WindowMinSize(280, 200);
const QSize IVConfig::WindowFitSize(800, 500);
const QString IVConfig::DefaultBgColor = "#C7EDCC";
const QDir::SortFlags IVConfig::DefaultDirSortFlag = QDir::LocaleAware;//QDir::Name | QDir::IgnoreCase;
const int IVConfig::ClickInterval = 1500;
const int IVConfig::ClickThreshold = 30;
const int IVConfig::AutoScrollInterval = 20;
const int IVConfig::FileSizePrecision = 2;


const bool DefaultShowDialog = true;
const bool DefaultEnableBgColor = true;
const bool DefaultAutoRotateImage = true;
const int  DefaultTimerInterval = 4;
const int  TimerIntervalMinLimit = 1;
const int  TimerIntervalMaxLimit = 1000;
const int  InvalidCacheNum = -1;
const int  CacheNumMinLimit = 0;
const int  CacheNumMaxLimit = 5;


IVConfig *IVConfig::sInstance = NULL;


const QString ConfigFileName = "EzViewer.ini";
const QString ShortcutFileName = "shortcut.xml";

const QString GeometryKey = "geometry";
const QString StartupGroup = "Startup";
const QString SizeModeKey = StartupGroup + "/SizeMode";
const QString DialogKey = StartupGroup + "/ShowDialog";
const QString EffectGroup = "Effect";
const QString ScaleModeKey = EffectGroup + "/ScaleMode";
const QString AlignModeKey = EffectGroup + "/AlignMode";
const QString AntialiasModeKey = EffectGroup + "/Antialiasing";
const QString EnableBgColorKey = EffectGroup + "/EnableBgColor";
const QString BgColorKey = EffectGroup + "/BgColor";
const QString AutoPlayGroup = "AutoPlay";
const QString TimerIntervalKey = AutoPlayGroup + "/TimerInterval";
const QString AdvancedGroup = "Advanced";
const QString AutoRotateKey = AdvancedGroup + "/AutoRotate";
const QString EnablePreReadingKey = AdvancedGroup + "/PreReading";
const QString CacheNumKey = AdvancedGroup + "/CacheValue";
const QString FormGroup = "Form";
const QString UseTitleBarKey = FormGroup + "/UseTitleBar";

const QString RootKey = "ezviewer";
const QString ShortcutKey = "shortcut";
const QString KeySequenceKey = "keys";
const QString ActionKey = "action";


IVConfig::IVConfig()
    : QObject(qApp), cfgWatcher(new QFileSystemWatcher(this))
{
    initConfigValue();
    loadAllShortcut();

    ActionManager::registerFunction(tr("Setting: Enable/Disable Custom Background Color"),
                   this, SPLIT_FUNCTION(IVConfig::changeBgColorMode));
    ActionManager::registerFunction(tr("Setting: Enable/Disable Pre-reading"),
                   this, SPLIT_FUNCTION(IVConfig::changePreReadingMode));

    ActionManager::registerFunction(tr("Setting Scale Mode: ") + tr("Scale Large Image to Fit Window"),
                   this, &IVConfig::changeScaleMode, SPLIT_PARAM(ScaleLargeImageToFitWidget));
    ActionManager::registerFunction(tr("Setting Scale Mode: ") + tr("Keep Image Size"),
                   this, &IVConfig::changeScaleMode, SPLIT_PARAM(KeepImageSize));
    ActionManager::registerFunction(tr("Setting Scale Mode: ") + tr("Fit Window Width"),
                   this, &IVConfig::changeScaleMode, SPLIT_PARAM(FitWidgetWidth));
    ActionManager::registerFunction(tr("Setting Scale Mode: ") + tr("Fit Window Height"),
                   this, &IVConfig::changeScaleMode, SPLIT_PARAM(FitWidgetHeight));
    ActionManager::registerFunction(tr("Setting Scale Mode: ") + tr("Scale to Fit Window"),
                   this, &IVConfig::changeScaleMode, SPLIT_PARAM(ScaleToFitWidget));
    ActionManager::registerFunction(tr("Setting Scale Mode: ") + tr("Scale to Expand Window"),
                   this, &IVConfig::changeScaleMode, SPLIT_PARAM(ScaleToExpandWidget));

    ActionManager::registerFunction(tr("Setting Align Mode: ") + tr("Align Left Top"),
                   this, &IVConfig::changeAlignMode, SPLIT_PARAM(AlignLeftTop));
    ActionManager::registerFunction(tr("Setting Align Mode: ") + tr("Align Left Center"),
                   this, &IVConfig::changeAlignMode, SPLIT_PARAM(AlignLeftCenter));
    ActionManager::registerFunction(tr("Setting Align Mode: ") + tr("Align Left Bottom"),
                   this, &IVConfig::changeAlignMode, SPLIT_PARAM(AlignLeftBottom));
    ActionManager::registerFunction(tr("Setting Align Mode: ") + tr("Align Center Top"),
                   this, &IVConfig::changeAlignMode, SPLIT_PARAM(AlignCenterTop));
    ActionManager::registerFunction(tr("Setting Align Mode: ") + tr("Align Center"),
                   this, &IVConfig::changeAlignMode, SPLIT_PARAM(AlignCenterCenter));
    ActionManager::registerFunction(tr("Setting Align Mode: ") + tr("Align Center Bottom"),
                   this, &IVConfig::changeAlignMode, SPLIT_PARAM(AlignCenterBottom));
    ActionManager::registerFunction(tr("Setting Align Mode: ") + tr("Align Right Top"),
                   this, &IVConfig::changeAlignMode, SPLIT_PARAM(AlignRightTop));
    ActionManager::registerFunction(tr("Setting Align Mode: ") + tr("Align Right Center"),
                   this, &IVConfig::changeAlignMode, SPLIT_PARAM(AlignRightCenter));
    ActionManager::registerFunction(tr("Setting Align Mode: ") + tr("Align Right Bottom"),
                   this, &IVConfig::changeAlignMode, SPLIT_PARAM(AlignRightBottom));

    ActionManager::registerFunction(tr("Setting Antialias Mode: ") + tr("Using When Pictures Zoom In"),
                   this, &IVConfig::changeAntialiasMode, SPLIT_PARAM(AntialiasWhenZoomIn));
    ActionManager::registerFunction(tr("Setting Antialias Mode: ") + tr("Always Using"),
                   this, &IVConfig::changeAntialiasMode, SPLIT_PARAM(AlwaysAntialias));
    ActionManager::registerFunction(tr("Setting Antialias Mode: ") + tr("Never Using"),
                   this, &IVConfig::changeAntialiasMode, SPLIT_PARAM(NoAntialias));

    QList<QByteArray> list = QImageReader::supportedImageFormats();
    for(int i=0; i < list.size(); ++i)
        mFormatsList.append(list.at(i));

    mFormats = mFormatsList.join(" *.");
    mFormats.prepend("*.");

    watchConfigFile(ConfigFilePath());
    watchConfigFile(ShortcutFilePath());
}

QString IVConfig::ConfigFilePath()
{
    return qApp->applicationDirPath() + "/" + ConfigFileName;
}

QString IVConfig::ShortcutFilePath()
{
    return qApp->applicationDirPath() + "/" + ShortcutFileName;
}

void IVConfig::watchConfigFile(const QString &filePath)
{
    if(!QFile::exists(filePath))
        QFile(filePath).open(QIODevice::WriteOnly); // create config file

    cfgWatcher->addPath(filePath);
    connect(cfgWatcher, SIGNAL(fileChanged(QString)), SLOT(fileChanged(QString)));
}

void IVConfig::fileChanged(const QString &filePath)
{
    if (filePath == ConfigFilePath())
        initConfigValue();
    else if (filePath == ShortcutFilePath())
        loadAllShortcut();
}

void IVConfig::initConfigValue()
{
    qDebug("initConfigValue");
    QSettings settings(ConfigFilePath(), QSettings::IniFormat);
    mShowDialog = settings.value(DialogKey, DefaultShowDialog).toBool();
    mScaleMode = (ScaleMode)settings.value(ScaleModeKey, DefaultScaleMode).toInt();
    mAlignMode = (AlignMode)settings.value(AlignModeKey, DefaultAlignMode).toInt();
    mAntialiasMode = (AntialiasMode)settings.value(AntialiasModeKey, DefaultAntialiasMode).toInt();
    mEnableBgColor = settings.value(EnableBgColorKey, DefaultEnableBgColor).toBool();
    QString colorStr = settings.value(BgColorKey, DefaultBgColor).toString();
    mTimerInterval = settings.value(TimerIntervalKey, DefaultTimerInterval).toInt();
    mAutoRotateImage = settings.value(AutoRotateKey, DefaultAutoRotateImage).toBool();
    mEnablePreReading = settings.value(
                EnablePreReadingKey, OSRelated::preReadingSuggested()).toBool();
    mCacheNum = settings.value(CacheNumKey, InvalidCacheNum).toInt();
    mLastGeometry = settings.value(GeometryKey).toByteArray();

    if (mScaleMode < ScaleModeBegin || mScaleMode > ScaleModeEnd)
        mScaleMode = DefaultScaleMode;
    if (mAlignMode < AlignModeBegin || mAlignMode > AlignModeEnd)
        mAlignMode = DefaultAlignMode;
    if(mAntialiasMode < AntialiasModeBegin || mAntialiasMode > AntialiasModeEnd)
        mAntialiasMode = DefaultAntialiasMode;
    mBgColor.setNamedColor(colorStr);
    if(!mBgColor.isValid())
        mBgColor.setNamedColor(DefaultBgColor);
    if(mTimerInterval < TimerIntervalMinLimit || mTimerInterval > TimerIntervalMaxLimit)
        mTimerInterval = DefaultTimerInterval;
    if(mCacheNum < CacheNumMinLimit || mCacheNum > CacheNumMaxLimit)
        mCacheNum = OSRelated::cacheSizeSuggested();

    emit configChanged();
}

void IVConfig::restoreDefaultsConfig()
{
    QSettings(ConfigFilePath(), QSettings::IniFormat).clear();
}

void IVConfig::insertConfigWatcher(const QObject *receiver, const char *method)
{
    QObject::connect(instance(), SIGNAL(configChanged()), receiver, method);
}

void IVConfig::cancelConfigWatcher(const QObject *receiver)
{
    QObject::disconnect(instance(), 0, receiver, 0);
}

void IVConfig::setShowDialog(bool enabled)
{
    setValue(DialogKey, enabled);
}

void IVConfig::setScaleMode(ScaleMode mode)
{
    setValue(ScaleModeKey, mode);
}

void IVConfig::setAlignMode(AlignMode mode)
{
    setValue(AlignModeKey, mode);
}

void IVConfig::setAntialiasMode(AntialiasMode mode)
{
    setValue(AntialiasModeKey, mode);
}

void IVConfig::setEnableBgColor(bool enabled)
{
    setValue(EnableBgColorKey, enabled);
}

void IVConfig::setBgColor(const QColor &color)
{
    setValue(BgColorKey, color.name());
}

void IVConfig::setTimerInterval(int interval)
{
    setValue(TimerIntervalKey, interval);
}

void IVConfig::setAutoRotateImage(bool enabled)
{
    setValue(AutoRotateKey, enabled);
}

void IVConfig::setEnablePreReading(bool enabled)
{
    setValue(EnablePreReadingKey, enabled);
}

void IVConfig::setCacheValue(int value)
{
    setValue(CacheNumKey, value);
}

void IVConfig::setLastGeometry(const QByteArray &geometry)
{
    setValue(GeometryKey, geometry);
}

void IVConfig::changeScaleMode(ScaleMode mode)
{
    if (scaleMode() == mode)
        setValue(ScaleModeKey, DefaultScaleMode);
    else
        setValue(ScaleModeKey, mode);
}

void IVConfig::changeAlignMode(AlignMode mode)
{
    if (alignMode() == mode)
        setValue(AlignModeKey, DefaultAlignMode);
    else
        setValue(AlignModeKey, mode);
}

void IVConfig::changeAntialiasMode(AntialiasMode mode)
{
    if (antialiasMode() == mode)
        setValue(AntialiasModeKey, DefaultAntialiasMode);
    else
        setValue(AntialiasModeKey, mode);
}

void IVConfig::addShortcut(const QString &keySequence, const QString &actionScript)
{
    if (ActionManager::bindShortcut(keySequence, actionScript))
        saveAllShortcut();
}

void IVConfig::addShortcut(const QStringList &keySequences, const QString &actionScript)
{
    ActionManager::bindShortcut(keySequences, actionScript);
    saveAllShortcut();
}

void IVConfig::removeShortcut(const QString &keySequence)
{
    if (ActionManager::unbindShortcut(keySequence))
        saveAllShortcut();
}

void IVConfig::removeShortcut(const QStringList &keySequences)
{
    ActionManager::unbindShortcut(keySequences);
    saveAllShortcut();
}

void IVConfig::saveAllShortcut()
{
    QXmlPut xmlPut(RootKey);

    QMap<QString, QString> shortcuts = ActionManager::getAllShortcut();
    QMap<QString, QString>::const_iterator it = shortcuts.constBegin();
    while (it != shortcuts.constEnd()) {
        xmlPut.descend(ShortcutKey);
        xmlPut.putString(KeySequenceKey, it.key());
        xmlPut.putString(ActionKey, it.value());
        xmlPut.rise();
        ++it;
    }

    xmlPut.save(ShortcutFilePath());
}

void IVConfig::loadAllShortcut()
{
    ActionManager::unbindAllShortcut(); ///

    QXmlGet xmlGet;
    xmlGet.load(ShortcutFilePath());

    QString key, action;
    while (xmlGet.findNext(ShortcutKey)) {
        xmlGet.descend();
        if (xmlGet.find(KeySequenceKey))
            key = xmlGet.getString();
        if (xmlGet.find(ActionKey))
            action = xmlGet.getString();
        xmlGet.rise();

        if (!key.isEmpty() && !action.isEmpty())
            ActionManager::bindShortcut(key, action);

        key.clear();
        action.clear();
    }
}

