/*
Copyright (C) 2009  Ruan Strydom <ruan@jcell.co.za>

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

#ifndef HTMLFORMATTER_H_
#define HTMLFORMATTER_H_

#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <QtCore/QMap>
#include <QtCore/QHash>

#include "xmlformatter.h"
#include <QStack>

/*
    NOTE: Does not work will rewrite
*/

class HtmlFormatter : public XmlFormatter {
public:
    HtmlFormatter();
    virtual ~HtmlFormatter();
    /** Adds whites to make the xml more readable
    */
    QString formatSource ( const QString& text, const QString& leftContext = QString(), const QString& rightContext = QString() ) const;
    QString attributeValue(const QString &token, const QString &attribute) const;
protected:
    QString tagName(const QString &token) const;
    QString trimText(const QString &token) const;
    bool isEmptyElement(const QString &tag) const;
};

#endif //HTMLFORMATTER_H_

