/* Copyright (C) 2021 Nature Easy Soft Network Technology Co., LTD
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

#ifndef LOCALSTORE_H
#define LOCALSTORE_H

#include "../core/store.h"

class LocalStore : public Store
{
public:
    explicit LocalStore();

    void write(const QString &group, const QString &key, const QVariant &value);
    QVariant read(const QString &group, const QString &key, const QVariant &defaultvalue);

private:
    std::string setting_ini;
};

#endif // LOCALSTORE_H
