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

#ifndef LINEBAR_H
#define LINEBAR_H

#include "../core/propsbar.h"
#include "../controls/colorwidget.h"
#include "../controls/pencombobox.h"
#include "../controls/sizeinputwidget.h"

/**
 * @class : LineBar
 * @brief : 直线属性面板定义
 * @note  : 直线铅笔属性设置
 */
class LineBar : public PropsBar
{
    Q_OBJECT
public:
    explicit LineBar(Workspace *m_workspace);

private:
    ColorWidget *m_colorWidget;           //颜色组件对象
    SizeInputWidget *m_sizeWidget;             //尺寸组件对象
    PenCombobox *m_penStyleCombobox; //线条组件对象

protected:
    void createWidget();
    void loadProps();
    void saveProps();
    void connectEvents();
};

#endif // LINEBAR_H
