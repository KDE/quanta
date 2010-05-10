/*****************************************************************************
 * Copyright (c) 2010 Ruan Strydom <rm3dom@gmail.com>                        *
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

#include "elementdeclaration.h"

using namespace Xml;

ElementDeclaration::ElementDeclaration(const ElementDeclaration& rhs): ClassDeclaration(rhs)
{
    setSmartRange(rhs.smartRange(), DocumentRangeObject::DontOwn);
}

ElementDeclaration::ElementDeclaration(const KDevelop::SimpleRange& range, KDevelop::DUContext* context): ClassDeclaration(range, context)
{
    d_func_dynamic()->setClassId(this);
    if (context) {
        setContext(context);
    }
}

ElementDeclaration::ElementDeclaration(ElementDeclaration& data): ClassDeclaration(data)
{

}

ElementDeclaration::ElementDeclaration(ElementDeclaration& data, const KDevelop::SimpleRange& range, KDevelop::DUContext* context): ClassDeclaration(data, range, context)
{

}

ElementDeclaration::~ElementDeclaration()
{

}

QString ElementDeclaration::name()
{
    return d_func()->name.str();
}

void ElementDeclaration::setName(const QString& n)
{
    d_func_dynamic()->nameSpace = KDevelop::IndexedString(n);
}


QString ElementDeclaration::nameSpace()
{
    return d_func()->nameSpace.str();
}

void Xml::ElementDeclaration::setNameSpace(const QString& ns)
{
    d_func_dynamic()->nameSpace = KDevelop::IndexedString(ns);
}

QString Xml::ElementDeclaration::toString() const
{
    return KDevelop::ClassDeclaration::toString();
}
