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

#include <QApplication>
#include <QTranslator>
#include <QLocale>

#include "IVMainWindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QString lang_country = QLocale::system().name();
    QTranslator qt_ts;
    if(qt_ts.load(QString(":/qt_%1").arg(lang_country)))
        app.installTranslator( &qt_ts );
    else if(qt_ts.load(QString("lang/qt_%1").arg(lang_country)))
        app.installTranslator( &qt_ts );
    QTranslator app_ts;
    if(app_ts.load(QString(":/EzViewer_%1").arg(lang_country)))
        app.installTranslator( &app_ts );
    else if(app_ts.load(QString("lang/EzViewer_%1").arg(lang_country)))
        app.installTranslator( &app_ts );

    IVMainWindow::preProcessCmd(app.arguments());
    IVMainWindow m;
    m.show();
    m.parseCmd(app.arguments());

    return app.exec();
}
