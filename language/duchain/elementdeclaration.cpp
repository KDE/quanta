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
#include <language/duchain/duchainregister.h>

namespace Xml {

REGISTER_DUCHAIN_ITEM(ElementDeclaration);

ElementDeclarationData::ElementDeclarationData()
{
    closeTagRequired = true;
    openTagRequired = true;
}

ElementDeclarationData::ElementDeclarationData(const Xml::ElementDeclarationData& rhs): ClassDeclarationData(rhs)
{
    closeTagRequired = rhs.closeTagRequired;
    openTagRequired = rhs.openTagRequired;
    name = rhs.name;
    contentType = rhs.contentType;
}


ElementDeclaration::ElementDeclaration(const ElementDeclaration& rhs): ClassDeclaration(rhs)
{
    setSmartRange(rhs.smartRange(), DocumentRangeObject::DontOwn);
}

ElementDeclaration::ElementDeclaration(const KDevelop::SimpleRange& range, KDevelop::DUContext* context):
        ClassDeclaration(*new ElementDeclarationData, range, context)
{
    d_func_dynamic()->setClassId(this);
    if (context)
        setContext(context);
}

ElementDeclaration::ElementDeclaration(ElementDeclarationData& data): ClassDeclaration(data)
{
    d_func_dynamic()->setClassId(this);
}

ElementDeclaration::ElementDeclaration(ElementDeclarationData& data, const KDevelop::SimpleRange& range, KDevelop::DUContext* context): ClassDeclaration(data, range, context)
{
    d_func_dynamic()->setClassId(this);
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
    d_func_dynamic()->name = KDevelop::IndexedString(n);
}


void ElementDeclaration::setCloseTagRequired(bool required)
{
    d_func_dynamic()->closeTagRequired = required;
}

bool ElementDeclaration::closeTagRequired()
{
    return d_func()->closeTagRequired;
}

void ElementDeclaration::setOpenTagRequired(bool required)
{
    d_func_dynamic()->openTagRequired = required;
}

bool ElementDeclaration::openTagRequired()
{
    return d_func()->openTagRequired;
}

void ElementDeclaration::setContentType(const QString& ct)
{
    d_func_dynamic()->contentType = KDevelop::IndexedString(ct);
}

QString ElementDeclaration::contentType()
{
    return d_func()->contentType.str();
}

QString Xml::ElementDeclaration::toString() const
{
    return KDevelop::ClassDeclaration::toString();
}

KDevelop::Declaration* ElementDeclaration::clonePrivate() const
{
    return new ElementDeclaration(*this);
}

}
