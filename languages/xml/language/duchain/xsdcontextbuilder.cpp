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

#include "xsdcontextbuilder.h"


#include <language/duchain/duchain.h>
#include <language/duchain/topducontext.h>
#include <language/duchain/duchainlock.h>
#include <language/duchain/declaration.h>
#include <language/duchain/classdeclaration.h>

#include <interfaces/icore.h>
#include <interfaces/ilanguagecontroller.h>
#include <interfaces/icompletionsettings.h>

#include <klocalizedstring.h>

#include "parsesession.h"
#include "editorintegrator.h"
#include "sgmlast.h"
#include <xmlcatalog/cataloghelper.h>

using namespace Xml;

XsdContextBuilder::XsdContextBuilder()
{
}

XsdContextBuilder::~XsdContextBuilder()
{
}

KDevelop::TopDUContext* XsdContextBuilder::newTopContext(const KDevelop::SimpleRange& range, KDevelop::ParsingEnvironmentFile* file)
{
    if (!file) {
        file = new KDevelop::ParsingEnvironmentFile(editor()->currentUrl());
        file->setLanguage(KDevelop::IndexedString("Sgml"));
    }
    return new KDevelop::TopDUContext(editor()->currentUrl(), range, file);
}


KDevelop::DUContext* XsdContextBuilder::contextFromNode(AstNode* node)
{
    Q_UNUSED(node);
    return 0;
}

KTextEditor::Range XsdContextBuilder::editorFindRange(AstNode* fromNode, AstNode* toNode)
{
    return static_cast<EditorIntegrator *>(editor())->findRange(fromNode, toNode).textRange();
}

KDevelop::QualifiedIdentifier XsdContextBuilder::identifierForNode(ElementTagAst* node)
{
    Q_UNUSED(node);
    return KDevelop::QualifiedIdentifier();
}

void XsdContextBuilder::setContextOnNode(AstNode* node, KDevelop::DUContext* context)
{
    Q_UNUSED(node);
    Q_UNUSED(context);
    //node->ducontext = ctx;
}

void XsdContextBuilder::startVisiting(AstNode* node)
{
    visitNode(node);
}

void XsdContextBuilder::reportProblem(KDevelop::ProblemData::Severity , AstNode* ast, const QString& message)
{
    EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
    KDevelop::Problem *p = new KDevelop::Problem();
    p->setSource(KDevelop::ProblemData::Parser);
    p->setSeverity(KDevelop::ProblemData::Error);
    p->setDescription(message);
    KDevelop::SimpleRange range = e->findRange(ast);
    p->setFinalLocation(KDevelop::DocumentRange(e->currentUrl().str(), KTextEditor::Range(range.start.line, range.start.column, range.end.line, range.end.column)));
    m_problems << KDevelop::ProblemPointer(p);
}
