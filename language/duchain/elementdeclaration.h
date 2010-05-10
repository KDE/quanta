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

#ifndef ELEMENTDECLARATION_H
#define ELEMENTDECLARATION_H

#include "duchainexport.h"
#include <language/duchain/classdeclaration.h>
#include <language/duchain/functiondeclaration.h>

namespace Xml {

class KDEVSGMLDUCHAIN_EXPORT ElementDeclarationData : public KDevelop::ClassDeclarationData
{
public:
    ElementDeclarationData()
            : KDevelop::ClassDeclarationData() {}

    ElementDeclarationData(const ElementDeclarationData& rhs)
            : KDevelop::ClassDeclarationData(rhs)
    {
        nameSpace = rhs.nameSpace;
        name = rhs.name;
    }

    ~ElementDeclarationData() {}

    KDevelop::IndexedString name;
    KDevelop::IndexedString nameSpace;
};


class KDEVSGMLDUCHAIN_EXPORT ElementDeclaration : public KDevelop::ClassDeclaration
{
public:
    ElementDeclaration(const ElementDeclaration &rhs);
    ElementDeclaration(const KDevelop::SimpleRange &range, KDevelop::DUContext *context);
    ElementDeclaration(ElementDeclaration &data);
    ElementDeclaration(ElementDeclaration &data, const KDevelop::SimpleRange &range, KDevelop::DUContext *context);
    ~ElementDeclaration();
    
    virtual QString toString() const;
    
    void setNameSpace(const QString &ns);
    QString nameSpace();
    
    void setName(const QString &n);
    QString name();
    
    enum {
        Identity = 86
    };
    
private:
    DUCHAIN_DECLARE_DATA(ElementDeclaration)
};

}

#endif // ELEMENTDECLARATION_H
