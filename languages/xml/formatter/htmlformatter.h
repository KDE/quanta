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

#ifndef HTMLFORMATTER_H_
#define HTMLFORMATTER_H_

#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <QtCore/QMap>
#include <QtCore/QHash>

#include "sgmlformatter.h"
#include <QStack>

namespace Xml
{

class HtmlFormatter : public SgmlFormatter {
public:
    HtmlFormatter();
    virtual ~HtmlFormatter();
    virtual QString formatSource(const QString& text, const QString& leftContext = QString(), const QString& rightContext = QString());
};
}

#endif //HTMLFORMATTER_H_

