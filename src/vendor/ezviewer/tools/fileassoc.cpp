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

#include "fileassoc.h"

#include <QSysInfo>
#include <QSettings>
#include <QCoreApplication>
#include <QFileInfo>
#include <QStringList>
#include <QTemporaryFile>
#include <QTextStream>
#include <QProcess>
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
#include <QOperatingSystemVersion>
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#   ifdef Q_OS_WIN
#   define Q_WS_WIN Q_OS_WIN
#   endif // Q_OS_WIN
#endif // QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)


#ifdef Q_WS_WIN
#include <Shlobj.h> //for SHChangeNotify(), refresh windows explorer
#endif  // Q_WS_WIN


namespace FileAssoc{

#ifdef Q_WS_WIN
const QString REG_DEFAULT("/.");
#endif  // Q_WS_WIN

bool isSupportAssociation()
{
#ifdef Q_WS_WIN
    return true;
#else
    return false;
#endif  // Q_WS_WIN
}

#ifdef Q_WS_WIN
//只能支持XP及以上系统

static bool isWindowsXP() {
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    const QOperatingSystemVersion version = QOperatingSystemVersion::current();

    // 首先检查是否为Windows系统
    if (version.type() != QOperatingSystemVersion::Windows) {
        return false;
    }

    // 检查主版本号和次版本号
    // Windows XP: 主版本5，次版本1或2 (5.1 = XP 32位, 5.2 = XP 64位)
    return (version.majorVersion() == 5) &&
           (version.minorVersion() == 1 || version.minorVersion() == 2);
#else
    return QSysInfo::WindowsVersion < QSysInfo::WV_VISTA;
#endif
}

static QString makeProgID(const QString & extension)
{
    // ProgID: like editor.txt
    // txt -> .txt  -> editor.txt
    return QFileInfo(qApp->applicationFilePath()).baseName()
            + "." + extension;
}

static bool hasProgID(const QString &ProgID)
{
    QSettings RegCR("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
    QSettings RegCU("HKEY_CURRENT_USER", QSettings::NativeFormat);

    RegCU.beginGroup("Software/Classes");

    QString key = ProgID + "/shell/open/command/.";
    QString appPath = qApp->applicationFilePath();
    appPath.replace('/', '\\');
    QString expect = QString("\"%1\" \"%2\"").arg(appPath, "%1"); // If it is set file association by this app, the value of key @key will be like this.

    if (RegCR.childGroups().contains(ProgID)) {
        return RegCR.value(key).toString() == expect;
    } else if (RegCU.childGroups().contains(ProgID)) {
        return RegCU.value(key).toString() == expect;
    } else {
        return false;
    }
}

static bool setProgID(const QString &ProgID,
               const QString &typeDescription,
               const QString &friendlyName)
{
    QString RootKeyName = "HKEY_CURRENT_USER";
    QString classId = "Software/Classes/" + ProgID;

    QSettings Reg (RootKeyName, QSettings::NativeFormat);
    if (!Reg.isWritable() || Reg.status() != QSettings::NoError)
        return false;

    QString appPath = qApp->applicationFilePath();
    appPath.replace('/', '\\'); //Explorer gives 'Access Denied' if we write the path with forward slashes to the registry

    if(!typeDescription.isNull())
        Reg.setValue(classId + REG_DEFAULT, typeDescription);/// Without this, will no able to create new text document.
    if(!friendlyName.isNull())
        Reg.setValue(classId + "/shell/open/FriendlyAppName", friendlyName);
    Reg.setValue(classId + "/shell/open/command/.", QString("\"%1\" \"%2\"").arg(appPath, "%1"));
    Reg.setValue(classId + "/DefaultIcon/.", QString("\"%1\",0").arg(appPath)); /// Use the first icon of current program for default format icon.

    return true;
}

static bool removeUserChoice(const QString & extension)
{
    QSettings RegCU("HKEY_CURRENT_USER", QSettings::NativeFormat);
    QString FileExtsKey = QString("Software/Microsoft/Windows/CurrentVersion/Explorer/FileExts/.") + extension;

    if(isWindowsXP()){ //XP
        RegCU.remove(FileExtsKey + "/Progid");
        return true;
    }

    QTemporaryFile file;
    if (file.open()) {
        QTextStream out(&file);
        // Unlock Registry, make user 'everyone' can access 'UserChoice' completely.
        out << "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\." + extension + "\\UserChoice [7]";
        out.flush();
        file.flush();

        // win7 build-in tool, for user access changing. Require path argument encoded by Unicode. Chinese may cause problem.
        // Only return after execute completely.
        QProcess::execute(QString("regini.exe %1").arg(file.fileName()));

        /// Windows 7, only can remove "/UserChoice" if we own it. remove "/UserChoice/Progid" is forbidden.
        RegCU.remove(FileExtsKey + "/UserChoice");

        return true;
    }

    return false;

    // The QTemporaryFile destructor removes the temporary file
    // as it goes out of scope.
}
#endif // Q_WS_WIN


bool checkAssociation(const QString & extension)
{
    if(!isSupportAssociation()) return false;

#ifndef Q_WS_WIN
    return false;
#else

    /*! The HKEY_CLASSES_ROOT subtree is a view formed by merging
     *  HKEY_CURRENT_USER\Software\Classes and
     *  HKEY_LOCAL_MACHINE\Software\Classes
     */
    QSettings RegCR("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
    QSettings RegCU("HKEY_CURRENT_USER", QSettings::NativeFormat);

    QString ext("." + extension);
    QString ProgID = makeProgID(extension);

    QString fileName = QFileInfo(qApp->applicationFilePath()).fileName();
    QString FileExtsKey = QString("Software/Microsoft/Windows/CurrentVersion/Explorer/FileExts/") + ext;
    /// Windows 7:"/UserChoice/Progid" ;   XP: "/Progid"
    QString CurClassId = (!isWindowsXP())
            ? RegCU.value(FileExtsKey + "/UserChoice/Progid").toString()
            : RegCU.value(FileExtsKey + "/Progid").toString();
    QString CurAppId = RegCU.value(FileExtsKey + "/Application").toString(); /// Windows XP

    bool isRegistered = false;
    if (!CurClassId.isEmpty()) {
        isRegistered = (CurClassId == ProgID) || (0 == CurClassId.compare(fileName, Qt::CaseInsensitive))
        || (0 == CurClassId.compare(QString("Applications\\%1").arg(fileName), Qt::CaseInsensitive));
    }else if (!CurAppId.isEmpty()){
        //If user uses Open With..., explorer creates it's own ClassId under Application, like "smplayer.exe"
        isRegistered = (CurAppId == ProgID) || (0 == CurAppId.compare(fileName, Qt::CaseInsensitive));
    }else{
        //No classId means that no associations exists in Default Programs or Explorer
        //Check the default per-user association
        isRegistered = (RegCU.value("Software/Classes/" + ext + REG_DEFAULT).toString() == ProgID);
    }

    //Finally, check the system-wide association
    //若CurClassId或CurAppId不为空，说明用户在“打开方式”中选择了其他程序，优先级高，会覆盖其他设置。
    if (!isRegistered && CurClassId.isEmpty() && CurAppId.isEmpty())
        isRegistered = (RegCR.value(ext + REG_DEFAULT).toString() == ProgID);

    return isRegistered && hasProgID(ProgID);

#endif  // Q_WS_WIN
}

bool setAssociation(const QString &extension,
                    const QString &typeDescription,
                    const QString &friendlyName)
{
    if(!isSupportAssociation()) return false;

#ifndef Q_WS_WIN
    return false;
#else

    QSettings RegCR ("HKEY_CLASSES_ROOT", QSettings::NativeFormat); //Read only on NT+
    QSettings RegCU ("HKEY_CURRENT_USER", QSettings::NativeFormat);

    QString ext("." + extension);
    QString ProgID = makeProgID(extension);

    //Check if ProgID exists in the registry, otherwise create it.
    if (!hasProgID(ProgID) && !setProgID(ProgID, typeDescription, friendlyName))
        return false;

    if(checkAssociation(extension))
        return true;

    //Create the associations
    RegCU.setValue("Software/Classes/" + ext + REG_DEFAULT, ProgID); //Extension class

    //Explorer FileExt association
    QString FileExtsKey = QString("Software/Microsoft/Windows/CurrentVersion/Explorer/FileExts/") + ext;
    removeUserChoice(extension);
    RegCU.remove(FileExtsKey + "/Application"); /// Windows XP


    RegCU.sync();
    RegCR.sync();
//    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);// Refresh Explorer cache.

    return (RegCU.status() == QSettings::NoError && RegCR.status() == QSettings::NoError);

#endif  // Q_WS_WIN
}

bool clearAssociation(const QString & extension)
{
    if(!isSupportAssociation()) return false;

#ifdef Q_WS_WIN

    if(!checkAssociation(extension))
        return true;

    QString ext("." + extension);
    QString ProgID = makeProgID(extension);
    QSettings RegCU("HKEY_CURRENT_USER", QSettings::NativeFormat);
    QSettings RegCR ("HKEY_CLASSES_ROOT", QSettings::NativeFormat);


    QString fileName = QFileInfo(qApp->applicationFilePath()).fileName();
    QString FileExtsKey = QString("Software/Microsoft/Windows/CurrentVersion/Explorer/FileExts/") + ext;
    /// Windows 7:"/UserChoice/Progid" ;   XP: "/Progid"
    QString CurClassId = (!isWindowsXP())
            ? RegCU.value(FileExtsKey + "/UserChoice/Progid").toString()
            : RegCU.value(FileExtsKey + "/Progid").toString();
    QString CurAppId = RegCU.value(FileExtsKey + "/Application").toString(); /// Windows XP

    if (!CurClassId.isEmpty() && (
                (CurClassId == ProgID)
                || (0 == CurClassId.compare(fileName, Qt::CaseInsensitive))
                || (0 == CurClassId.compare(QString("Applications\\%1").arg(fileName), Qt::CaseInsensitive))
                )  ){
        removeUserChoice(extension);
    }

    if (!CurAppId.isEmpty() && (
                (CurAppId == ProgID)
                || (0 == CurAppId.compare(fileName, Qt::CaseInsensitive))
                )   ){
        RegCU.remove(FileExtsKey + "/Application");
    }

    if (RegCU.value("Software/Classes/" + ext + REG_DEFAULT).toString() == ProgID) //Only remove if we own it
        /// This is no recommend to delete in MSDN, case it may cause other problems, such as cannot create new text document if delete '.txt' group.
        //  RegCU.remove("Software/Classes/" + ext);
        RegCU.remove("Software/Classes/" + ProgID);

    RegCU.sync();
    RegCR.sync();
//    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);// Refresh Explorer cache.

    return (RegCU.status() == QSettings::NoError && RegCR.status() == QSettings::NoError);
#else
    return false;
#endif  // Q_WS_WIN
}

void refreshExplorer()
{
    if(!isSupportAssociation()) return;

#ifdef Q_WS_WIN
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);// Refresh Explorer cache.
#else
#endif  // Q_WS_WIN
}
}
