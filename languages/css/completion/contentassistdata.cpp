/*
   Copyright (C) 2009 Niko Sams <niko.sams@gmail.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "contentassistdata.h"

#include <QtCore/QXmlStreamReader>
#include <QFile>

#include <KDE/KStandardDirs>
#include <KDebug>

namespace Css {

ContentAssistData* ContentAssistData::self()
{
    static ContentAssistData* ret = 0;
    if (!ret) ret = new ContentAssistData;
    return ret;
}

ContentAssistData::ContentAssistData()
{
    QString fileName = KStandardDirs::locate("data", "kdevcsssupport/completion/CSS.xml");
    QFile f(fileName);
    Q_ASSERT(f.exists());
    f.open(QIODevice::ReadOnly);
    QXmlStreamReader xml(&f);

    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement() && xml.name() == "elements") {
            while (!xml.atEnd()) {
                xml.readNext();
                if (xml.isEndElement() && xml.name() == "elements") break;
                if (xml.isStartElement() && xml.name() == "element") {
                    Element el;
                    el.name = xml.attributes().value("name").toString();
                    el.fullName = xml.attributes().value("full-name").toString();
                    while (!xml.atEnd()) {
                        xml.readNext();
                        if (xml.isEndElement() && xml.name() == "element") break;
                        if (xml.isStartElement() && xml.name() == "description") {
                            el.description = xml.readElementText();
                        }
                        if (xml.isStartElement() && xml.name() == "browsers") {
                            el.browsers = readBrowsers(xml);
                        }
                        if (xml.isStartElement() && xml.name() == "example") {
                            el.example = xml.readElementText();
                        }
                        if (xml.isStartElement() && xml.name() == "remarks") {
                            el.remarks = xml.readElementText();
                        }
                        if (xml.isStartElement() && xml.name() == "fields") {
                            while (!xml.atEnd()) {
                                xml.readNext();
                                if (xml.isEndElement() && xml.name() == "fields") break;
                                if (xml.isStartElement() && xml.name() == "field") {
                                    el.fields << xml.attributes().value("name").toString();
                                }
                            }
                        }
                        m_elements[el.name] = el;
                    }
                }
            }
        }

        if (xml.isStartElement() && xml.name() == "fields") {
            while (!xml.atEnd()) {
                xml.readNext();
                if (xml.isEndElement() && xml.name() == "fields") break;
                if (xml.isStartElement() && xml.name() == "field") {
                    Field field;
                    field.name = xml.attributes().value("name").toString();
                    while (!xml.atEnd()) {
                        xml.readNext();
                        if (xml.isEndElement() && xml.name() == "field") break;
                        if (xml.isStartElement() && xml.name() == "description") {
                            field.description = xml.readElementText();
                        }
                        if (xml.isStartElement() && xml.name() == "hint") {
                            field.hint = xml.readElementText();
                        }
                        if (xml.isStartElement() && xml.name() == "browsers") {
                            field.browsers = readBrowsers(xml);
                        }
                        if (xml.isStartElement() && xml.name() == "example") {
                            field.example = xml.readElementText();
                        }
                        if (xml.isStartElement() && xml.name() == "remarks") {
                            field.remarks = xml.readElementText();
                        }
                        if (xml.isStartElement() && xml.name() == "values") {
                            while (!xml.atEnd()) {
                                xml.readNext();
                                if (xml.isEndElement() && xml.name() == "values") break;
                                if (xml.isStartElement() && xml.name() == "value") {
                                    Value v;
                                    v.name = xml.attributes().value("name").toString();
                                    v.description = xml.attributes().value("description").toString();
                                    field.values[v.name] = v;
                                }
                            }
                        }
                        m_fields[field.name] = field;
                    }
                }
            }
        }
    }
}

QList<ContentAssistData::Browser> ContentAssistData::readBrowsers(QXmlStreamReader& xml)
{
    QList<Browser> ret;
    Q_ASSERT(xml.isStartElement() && xml.name() == "browsers");
    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isEndElement() && xml.name() == "browsers") break;
        if (xml.isStartElement() && xml.name() == "browser") {
            Browser br;
            br.platform = xml.attributes().value("platform").toString();
            br.version = xml.attributes().value("version").toString();
            br.os = xml.attributes().value("os").toString();
            while (!xml.atEnd()) {
                xml.readNext();
                if (xml.isEndElement() && xml.name() == "browser") break;
                if (xml.isStartElement() && xml.name() == "description") {
                    br.description = xml.readElementText();
                }
            }
            ret << br;
        }
    }
    return ret;
}

ContentAssistData::Element ContentAssistData::element(const QString& name)
{
    if (!m_elements.contains(name)) {
        return Element();
    }
    return m_elements[name];
}

ContentAssistData::Field ContentAssistData::field(const QString& name)
{
    if (!m_fields.contains(name)) {
        return Field();
    }
    return m_fields[name];
}

QStringList ContentAssistData::elements()
{
    return m_elements.keys();
}


}
