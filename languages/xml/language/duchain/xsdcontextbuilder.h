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

#ifndef XSDCONTEXTBUILDER_H
#define XSDCONTEXTBUILDER_H

#include "duchainexport.h"

#include "sgmldebugvisitor.h"

#include <language/duchain/builders/abstractcontextbuilder.h>
#include "abstractbuilder.h"

namespace Xml
{
class EditorIntegrator;
class ParseSession;

typedef KDevelop::AbstractContextBuilder<AstNode, ElementTagAst> XsdContextBuilderBase;

/** Builder for XSD
 *  @see ContextBuilder
 */
class KDEVSGMLDUCHAIN_EXPORT XsdContextBuilder: public XsdContextBuilderBase, public AbstractBuilder
{
public:
    XsdContextBuilder();

    virtual ~XsdContextBuilder();
    virtual KDevelop::DUContext* contextFromNode(AstNode* node);
    virtual KTextEditor::Range editorFindRange(AstNode* fromNode, AstNode* toNode);
    virtual KDevelop::QualifiedIdentifier identifierForNode(ElementTagAst* node);
    virtual void setContextOnNode(AstNode* node, KDevelop::DUContext* context);
    virtual void startVisiting(AstNode* node);
    virtual KDevelop::TopDUContext* newTopContext(const KDevelop::SimpleRange& range, KDevelop::ParsingEnvironmentFile* file = 0);
protected:
    virtual void reportProblem(KDevelop::ProblemData::Severity, AstNode* ast, const QString& message);
};

}

#endif // XSDCONTEXTBUILDER_H

