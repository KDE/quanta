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


#ifndef INCLUDEBUILDER_H
#define INCLUDEBUILDER_H

#include <QMap>
#include <language/duchain/indexedstring.h>

#include "duchainexport.h"
#include "sgmldebugvisitor.h"
#include "abstractbuilder.h"

namespace Xml
{
class EditorIntegrator;

/** Extracts includes and namespaces from documents.
 *  This should happen before running the rest of the builders
 *  as they need the namespsces and top contexts if there are any.
 */
class KDEVSGMLDUCHAIN_EXPORT IncludeBuilder : public AbstractBuilder
{
public:
    
    IncludeBuilder(EditorIntegrator* editor);
    
    QMap<Xml::AstNode*, IncludeIdentifier> includes();
    
    void build(const KDevelop::IndexedString &document, AstNode* ast);
    
    EditorIntegrator* editor() const {
        return m_editor;
    }
protected:
    /** Finds dtd imports. */
    virtual void visitDtdDoctype(DtdDoctypeAst* node);
    /** Find external references */
    virtual void visitDtdEntity(DtdEntityAst* node);
    /** Finds dtd entity imports. */
    virtual void visitDtdEntityInclude(DtdEntityIncludeAst* node);
    /** Finds namespace imports. */
    virtual void visitAttribute(AttributeAst* node);
private:
    QString tokenText(qint64 begin, qint64 end) const;
    QString nodeText(AstNode *node) const;
    virtual void reportProblem(KDevelop::ProblemData::Severity, AstNode* ast, const QString& message);
    EditorIntegrator* m_editor;
    QMap<Xml::AstNode*, IncludeIdentifier> m_includes;
    KDevelop::IndexedString m_document;
};

}

#endif // INCLUDEBUILDER_H
