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

#include "text.h"
#include "../screen/workspace.h"
#include "QFontMetrics"

Text::Text(Workspace *workspace):Shape(workspace)
{
    loadProps();
}

void Text::move(QPoint offsetPoint)
{
    m_location = QPoint(m_location.x()+offsetPoint.x(),m_location.y()+offsetPoint.y());
}

QVector<Handle *> Text::handles()
{
    QVector<Handle *> handles;

    return  handles;
}

QRect Text::boundary()
{
    QFontMetrics fm(font());
    QRect rect = fm.boundingRect(QRect(), 0, m_content);

    QSize size(
                rect.width() + Utils::TextEdit_Padding_Horizontal * 2,
                rect.height() + Utils::TextEdit_Padding_Vertical * 2);

    return  QRect(
                m_location.x(),
                m_location.y(),
                size.width(),
                size.height());
}

void Text::draw(QPainter &painter)
{
    QFont mfont = font();
    QFontMetrics fm(font());

    painter.save();

    QRectF rect = boundary();

    QRect wrapperRect(
                rect.x() + Utils::TextEdit_Padding_Horizontal,
                rect.y() + Utils::TextEdit_Padding_Vertical,
                rect.width() - Utils::TextEdit_Padding_Horizontal * 2,
                rect.height() - Utils::TextEdit_Padding_Vertical * 2 + 10
                );


    QPen pen(m_color);

    painter.setPen(pen);
    painter.setFont(mfont);
    painter.drawText(wrapperRect,m_content);

    painter.restore();

    if(m_selected == true)
    {
        drawAntIndicator(painter,boundary());
    }
}

bool Text::contain(QPoint point)
{
    QRectF rect = boundary();
    return rect.contains(point);
}

QFont Text::font()
{
    QFont font;

    font.setFamily(m_family);
    font.setPixelSize(m_size);
    font.setWeight(m_weight);
    font.setItalic(m_italic);
    font.setUnderline(m_underline);

    return font;
}

QString Text::content() const
{
    return m_content;
}

void Text::setContent(const QString &value)
{
    m_content = value;
}

QColor Text::color() const
{
    return m_color;
}

void Text::setColor(const QColor &color)
{
    m_color = color;
}

QPoint Text::location() const
{
    return m_location;
}

void Text::setLocation(const QPoint &value)
{
    m_location = value;
}

QString Text::type()
{
    return Utils::forTextKey();
}

void Text::loadPropsImpl(Store *store)
{
    m_size = store->read(Utils::forTextKey(),Utils::TextSizeName(),Utils::TextSizeDefault()).toInt();
    m_color = store->readColor(Utils::forTextKey(),Utils::TextColorName(),Utils::TextColorDefault());
    m_family = store->read(Utils::forTextKey(),Utils::TextFamilyName(),Utils::TextFamilyDefault()).toString();
    bool tmpBold = store->read(Utils::forTextKey(),Utils::TextBoldName(),Utils::TextBoldDefault()).toBool();
    m_italic = store->read(Utils::forTextKey(),Utils::TextItalicName(),Utils::TextItalicDefault()).toBool();
    m_underline = store->read(Utils::forTextKey(),Utils::TextUnderlineName(),Utils::TextUnderlineDefault()).toBool();

#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    m_weight = tmpBold == true ? 99 : QFont::Normal;
#else
    m_weight = tmpBold == true ? QFont::Bold : QFont::Normal;
#endif
}

void Text::savePropsImpl(Store *store)
{
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    bool tmpBold = m_weight == 99 ? true : false;
#else
    bool tmpBold = m_weight == QFont::Bold ? true : false;
#endif

    store->write(Utils::forTextKey(),Utils::TextSizeName(),m_size);
    store->writeColor(Utils::forTextKey(),Utils::TextColorName(),m_color);
    store->write(Utils::forTextKey(),Utils::TextFamilyName(),m_family);
    store->write(Utils::forTextKey(),Utils::TextBoldName(),tmpBold);
    store->write(Utils::forTextKey(),Utils::TextItalicName(),m_italic);
    store->write(Utils::forTextKey(),Utils::TextUnderlineName(),m_underline);
}
