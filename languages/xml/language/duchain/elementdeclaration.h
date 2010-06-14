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
#include <language/duchain/declaration.h>
#include <language/duchain/declarationdata.h>

namespace Xml {

using namespace KDevelop;

KDEVSGMLDUCHAIN_EXPORT DECLARE_LIST_MEMBER_HASH(ElementDeclarationData, m_attributes, IndexedString)

class KDEVSGMLDUCHAIN_EXPORT ElementDeclarationData : public ClassDeclarationData
{
public:
    
    enum ElementType {
        Element,
        Text,
        Doctype,
        CDATA,
        PCDATA,
        Entity,
        Processing
    };

    ElementDeclarationData();

    ElementDeclarationData(const ElementDeclarationData& rhs);

    ~ElementDeclarationData();

    IndexedString name;
    IndexedString contentType;
    IndexedString content;
    bool closeTagRequired;
    ElementType elementType;
    
    START_APPENDED_LISTS_BASE(ElementDeclarationData, ClassDeclarationData);
    APPENDED_LIST_FIRST(ElementDeclarationData, IndexedString, m_attributes);
    END_APPENDED_LISTS(ElementDeclarationData, m_attributes);
};


class KDEVSGMLDUCHAIN_EXPORT ElementDeclaration : public ClassDeclaration
{
public:
    
    ElementDeclaration(const ElementDeclaration &rhs);
    ElementDeclaration(const SimpleRange &range, DUContext *context);
    ElementDeclaration(ElementDeclarationData &data);
    ElementDeclaration(ElementDeclarationData &data, const SimpleRange &range, DUContext *context);
    ~ElementDeclaration();
    
    virtual QString toString() const;
    
    void setName(const QString &n);
    QString name() const;

    void setContentType(const QString &ct);
    QString contentType() const;

    void setContent(const QString &ct);
    QString content() const;
    
    void setElementType(ElementDeclarationData::ElementType type);
    ElementDeclarationData::ElementType elementType() const;

    bool closeTagRequired() const;
    void setCloseTagRequired(bool required);

    virtual const IndexedString* attributes() const;
    virtual unsigned int attributesSize() const;
    virtual void addAttributes(const IndexedString& str);
    virtual void clearAttributes();

    enum {
        Identity = 90
    };
    
private:
    virtual Declaration* clonePrivate () const;
    DUCHAIN_DECLARE_DATA(ElementDeclaration)
};

}

#endif // ELEMENTDECLARATION_H
