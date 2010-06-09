/*****************************************************************************
 * Copyright (c) 2009 Ruan Strydom <rm3dom@gmail.com>                        *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *****************************************************************************/

#include "htmlformatter.h"

#include <interfaces/isourceformatter.h>
#include <KDE/KDebug>

using namespace Xml;

HtmlFormatter::HtmlFormatter() : SgmlFormatter() {
    m_mime = KMimeType::mimeType("text/html");
    m_dtdHelper = DtdHelper::instanceForName("html");
    m_options.insert("CASE", "LOWER");
    m_options.insert("INDENT", 4);
    m_options.insert("SOURCES", true);
}

HtmlFormatter::~HtmlFormatter() {}

QString HtmlFormatter::formatSource(const QString& text, const QString& leftContext, const QString& rightContext)
{
    kDebug();
    return Xml::SgmlFormatter::formatSource(text, leftContext, rightContext);
}

