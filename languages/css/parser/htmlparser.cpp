/***************************************************************************
 *   This file is part of KDevelop                                         *
 *   Copyright 2010 Niko Sams <niko.sams@gmail.com>                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include "htmlparser.h"
#include <QXmlStreamReader>
#include <QRegExp>
#include <QBuffer>
#include <KDebug>

namespace Css {

HtmlParser::HtmlParser()
{
}

void HtmlParser::setContents(const QString& contents)
{
    m_contents = contents;
}

QList<HtmlParser::Part> HtmlParser::parse()
{
    QList<HtmlParser::Part> ret;
    QXmlStreamReader reader(m_contents);
    while (!reader.atEnd()) {
        if (reader.isStartElement() && reader.name() == "style") {
            Part p;
            p.kind = Part::StyleElement;
            p.range.start.line = reader.lineNumber()-1;
            p.range.start.column = reader.columnNumber();
            while (!reader.isEndElement() && !reader.atEnd()) {
                p.contents += reader.text();
                p.range.end.line = reader.lineNumber()-1;
                p.range.end.column = reader.columnNumber()-1;
                reader.readNext();
            }
            ret << p;
        }
        if (reader.attributes().hasAttribute("style")) {
            Part p;
            p.kind = Part::InlineStyle;
            QString c = m_contents.left(reader.characterOffset());
            static QRegExp rx("<[^<>]+\\sstyle=[\"'](([^<>]*)[\"'][^<>]*>)$");
            if (rx.indexIn(c) != -1) {
                kDebug() << rx.cap(0);
                p.contents = rx.cap(2); //don't use reader.attributes() as it doesn't contain newlines correctly
                p.range.start.line = reader.lineNumber()-1-rx.cap(1).count('\n');
                if (!rx.cap(1).contains('\n')) {
                    p.range.start.column = reader.columnNumber()-rx.cap(1).length();
                } else {
                    p.range.start.column = rx.cap(1).mid(rx.cap(1).indexOf('\n')).length();
                }
                p.range.end.line = p.range.start.line + p.contents.count('\n');
                if (!p.contents.contains('\n')) {
                    p.range.end.column = p.range.start.column+p.contents.length();
                } else {
                    p.range.end.column = p.contents.mid(p.contents.lastIndexOf('\n')+1).length();
                }
                p.tag = reader.name().toString();
                ret << p;
            } else {
                kWarning() << "failed parsing style attribute" << c;
            }
        }
        reader.readNext();
    }
    return ret;
}

}
