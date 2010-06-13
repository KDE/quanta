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

#ifndef CSS_CONTENTASSISTDATA_H
#define CSS_CONTENTASSISTDATA_H

#include <QString>
#include <QHash>
#include <QMap>

class QXmlStreamReader;
namespace Css {

class ContentAssistData
{
public:
    struct Browser {
        QString platform;
        QString version;
        QString os;
        QString description;
    };
    struct Element {
        QString name;
        QString fullName;
        QString description;
        QList<Browser> browsers;
        QString example;
        QString remarks;
        QList<QString> fields;
    };
    struct Value {
        QString name;
        QString description;
    };
    struct Field {
        QString name;
        QString type;
        QString hint;
        QString description;
        QMap<QString, Value> values;
        QList<Browser> browsers;
        QString example;
        QString remarks;
    };

    static ContentAssistData *self();

    Element element(const QString &name);
    Field field(const QString &name);
    QStringList elements();
private:
    ContentAssistData();
    QList<Browser> readBrowsers(QXmlStreamReader &xml);
    QHash<QString, Element> m_elements;
    QHash<QString, Field> m_fields;
};

}

#endif // CSS_CONTENTASSISTDATA_H
