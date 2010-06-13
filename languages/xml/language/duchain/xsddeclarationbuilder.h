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

#ifndef XSDDECLARATIONBUILDER_H
#define XSDDECLARATIONBUILDER_H

#include "duchainexport.h"

#include "xsdcontextbuilder.h"

#include <language/duchain/builders/abstractdeclarationbuilder.h>

namespace KDvelop
{
class Declaration;
class QualifiedIdentifier;
}

namespace Xml
{
class ParseSession;
class EditorIntegrator;
class ElementDeclaration;

typedef KDevelop::AbstractDeclarationBuilder<AstNode, ElementAst, XsdContextBuilder> XsdDeclarationBuilderBase;

/** Builder for XSD
 *  @see DeclarationBuilder
 */
class KDEVSGMLDUCHAIN_EXPORT XsdDeclarationBuilder : public XsdDeclarationBuilderBase
{
public:
    XsdDeclarationBuilder(KDevelop::EditorIntegrator* editor);
    ~XsdDeclarationBuilder();
};
}

#endif //XSDDECLARATIONBUILDER_H

