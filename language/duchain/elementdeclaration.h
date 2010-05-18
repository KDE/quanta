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

template <typename EnumT, typename BaseEnumT>
class KDEVSGMLDUCHAIN_EXPORT InheritEnum
{
public:
  InheritEnum() {}
  InheritEnum(EnumT e)
    : enum_(e)
  {}

  InheritEnum(BaseEnumT e)
    : baseEnum_(e)
  {}

  explicit InheritEnum( int val )
    : enum_(static_cast<EnumT>(val))
  {}

  operator EnumT() const { return enum_; }
private:
  // Note - the value is declared as a union mainly for as a debugging aid. If 

  // the union is undesired and you have other methods of debugging, change it

  // to either of EnumT and do a cast for the constructor that accepts BaseEnumT.

  union
  { 
    EnumT enum_;
    BaseEnumT baseEnum_;
  };
};
    
class KDEVSGMLDUCHAIN_EXPORT ElementDeclarationData : public KDevelop::ClassDeclarationData
{
public:
    
    ElementDeclarationData();

    ElementDeclarationData(const ElementDeclarationData& rhs);

    ~ElementDeclarationData() {}
    
    

    KDevelop::IndexedString name;
    KDevelop::IndexedString contentType;
    bool openTagRequired;
    bool closeTagRequired;
};


class KDEVSGMLDUCHAIN_EXPORT ElementDeclaration : public KDevelop::ClassDeclaration
{
public:
    
    ElementDeclaration(const ElementDeclaration &rhs);
    ElementDeclaration(const KDevelop::SimpleRange &range, KDevelop::DUContext *context);
    ElementDeclaration(ElementDeclarationData &data);
    ElementDeclaration(ElementDeclarationData &data, const KDevelop::SimpleRange &range, KDevelop::DUContext *context);
    ~ElementDeclaration();
    
    virtual QString toString() const;
    
    void setName(const QString &n);
    QString name();

    void setContentType(const QString &ct);
    QString contentType();

    bool openTagRequired();
    void setOpenTagRequired(bool required);

    bool closeTagRequired();
    void setCloseTagRequired(bool required);

    enum {
        Identity = 87
    };
    
private:
    virtual KDevelop::Declaration* clonePrivate () const;
    DUCHAIN_DECLARE_DATA(ElementDeclaration)
};

}

#endif // ELEMENTDECLARATION_H
