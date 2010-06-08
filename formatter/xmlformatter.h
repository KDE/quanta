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

#ifndef XMLFORMATTER_H_
#define XMLFORMATTER_H_

#include "sgmlformatter.h"

#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <QtCore/QMap>

namespace Xml
{

/** A simple XML formatter.
 *  A formatter for XML, which is hopefully a little faster than the SgmlFormatter.
 *  Unlike SGML the content in elements is not indented.
 */
class XmlFormatter : public SgmlFormatter {
public:
    XmlFormatter();

    QString formatSource ( const QString& text, const QString& leftContext = QString(), const QString& rightContext = QString() );
    
    QString compactSource ( const QString& text );
};

}

#endif //XMLFORMATTER_H_

