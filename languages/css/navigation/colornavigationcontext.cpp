/*
    Copyright (C) 2010 Milian Wolff <mail@milianw.de>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "colornavigationcontext.h"

using namespace Css;

ColorNavigationContext::ColorNavigationContext(KDevelop::TopDUContextPointer topContext, const QColor& color)
  : AbstractNavigationContext(topContext),
    m_color(color)
{

}

QString ColorNavigationContext::name() const
{
    return m_color.name();
}

QString ColorNavigationContext::html(bool /*shorten*/)
{
    QString ret("<html><body style='background-color:%1;text-align:center;'>"
                "<p style='background-color:white;color:black;margin-bottom:40px;'>%1</p>"
                "</body></html>");
    return ret.arg(m_color.name());
}

