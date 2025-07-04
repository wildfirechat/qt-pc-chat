/* Copyright (C) 2022 Nature Easy Soft Network Technology Co., LTD
 *
 * This file is part of Zenshot (https://github.com/easysoft/zenshot/).
 *
 * Zenshot is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Zenshot is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Zenshot. If not, see <https://www.gnu.org/licenses/>.
 */

#include "gparams.h"
#include "../core/utils.h"

#include <QMutex>
#include <QMutexLocker>

GParams* GParams::m_instance = nullptr;

GParams* GParams::instance()
{
    if(m_instance == nullptr)
    {
        static QMutex mutex;
        QMutexLocker locker(&mutex);

        if(m_instance == nullptr)
        {
            m_instance = new GParams();
            m_instance->setScale(1.0);
            m_instance->CleanUpArgs();
        }
    }

    return m_instance;
}

QString GParams::mark() const
{
    return m_mark;
}

QString GParams::save() const
{
    return m_save;
}

void GParams::SetArgMark(const QString& m)
{
    if (m_mark != "yes" && m_mark != "no")
        return;

	m_mark = m;
}

void GParams::SetArgSave(const QString& s)
{
	m_save = s;
}

void GParams::SetArgClipboard(const QString& c)
{
    if (m_clipboard != "auto" && m_clipboard != "yes" && m_clipboard != "no")
        return;

	m_clipboard = c;
}

void GParams::CleanUpArgs()
{
	m_mark = "yes";
	m_clipboard = "auto";
	m_save = nullptr;
}

int GParams::fromArgs(int argc, char *argv[])
{
    m_mark = "yes";
    m_clipboard = "auto";
    m_save = nullptr;

    for(int i=0;i<argc;i++)
    {
        char *c = argv[i];
        QString str(c);

        if(str.startsWith("--mark="))
            m_mark = str.mid(7);
        else if(str.startsWith("-m="))
            m_mark = str.mid(3);
        else if(str.startsWith("--save="))
            m_save = str.mid(7);
        else if(str.startsWith("-s="))
            m_save = str.mid(3);
        else if(str.startsWith("--clipboard="))
            m_clipboard = str.mid(12);
        else if(str.startsWith("-c="))
            m_clipboard = str.mid(3);
    }

    if(m_mark != "yes" && m_mark != "no")
        return Utils::ERROR_MARK_VALUE;

    if(m_clipboard != "auto" && m_clipboard != "yes" && m_clipboard != "no")
        return Utils::ERROR_CLIPBOARD_VALUE;

    return 0;
}

QString GParams::clipboard() const
{
    return m_clipboard;
}

void GParams::setClipboard(const QString &clipboard)
{
    m_clipboard = clipboard;
}

float GParams::scale() const
{
    return m_scale;
}

void GParams::setScale(float scale)
{
    m_scale = scale;
}
