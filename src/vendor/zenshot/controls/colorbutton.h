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

#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QWidget>
#include <QAbstractButton>
#include <QColor>
#include <QPen>
#include <QBrush>

#include "../core/gscale.h"

/**
 * @class : ColorButton
 * @brief : 颜色按钮定义
 * @note  : 用来展示单一颜色
 */
class ColorButton : public QAbstractButton,GScale
{
    Q_OBJECT
public:
    explicit ColorButton(QWidget *parent = nullptr);

    int length() const;
    void setLength(int length);

    int padding() const;
    void setPadding(int padding);

    int hoverPadding() const;
    void setHoverPadding(int hoverPadding);

    QColor borderColor() const;
    void setBorderColor(const QColor &borderColor);

    QColor contentColor() const;
    void setContentColor(const QColor &contentColor);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &backgroundColor);

    int borderWidth() const;
    void setBorderWidth(int borderWidth);

    int borderRadius() const;
    void setBorderRadius(int borderRadius);

private:
    bool m_isHover;            //是否鼠标悬停

    int m_length;              //组件的尺寸(长宽)
    int m_padding;             //内边距
    int m_hoverPadding;        //悬停时的内边距
    int m_borderWidth;         //边框宽度
    int m_borderRadius;        //圆角半径

    QColor m_borderColor;      //边框颜色
    QColor m_contentColor;     //内容颜色
    QColor m_backgroundColor;  //背景色

    QPen m_borderPen;
    QBrush m_backgrondBrush;
    QBrush m_contentBrush;

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void paintEvent(QPaintEvent *event);
};

#endif // COLORBUTTON_H
