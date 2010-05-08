/*
Copyright (C) 2010  Ruan Strydom <ruan@jcell.co.za>

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

#ifndef XML_DECLARATIONBUILDER_H
#define XML_DECLARATIONBUILDER_H

#include "duchainexport.h"

#include "contextbuilder.h"

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


typedef KDevelop::AbstractDeclarationBuilder<AstNode, ElementAst, ContextBuilder> DeclarationBuilderBase;

class KDEVSGMLDUCHAIN_EXPORT DeclarationBuilder : public DeclarationBuilderBase
{
public:
    DeclarationBuilder(KDevelop::EditorIntegrator* editor);
    ~DeclarationBuilder();
protected:
    virtual void visitElementTag(ElementTagAst* node);
    virtual void visitDtdDoctype(DtdDoctypeAst* node);
    virtual void visitElementPHP(ElementPHPAst* node);
    virtual void visitElementPCDATA(ElementPCDATAAst* node);
    virtual void visitElementCDATA(ElementCDATAAst* node);
    virtual void visitElementText(ElementTextAst* node);
    virtual void visitElementProcessing(ElementProcessingAst* node);
    virtual void visitElementSource(ElementSourceAst* node);
    virtual void closeDeclaration();
    virtual KDevelop::QualifiedIdentifier identifierForNode( ElementTagAst* node );
    QString tokenText(qint64 begin, qint64 end) const;
    QString tagName(const ElementTagAst *ast) const;
    QString tagName(const ElementCloseTagAst *ast) const;
};

}

#endif // XML_DECLARATIONBUILDER_H
