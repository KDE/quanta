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

#include "contextbuilder.h"

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

using namespace Xml;

ContextBuilder::ContextBuilder()
{
}

ContextBuilder::~ContextBuilder()
{
}

KDevelop::TopDUContext* ContextBuilder::newTopContext(const KDevelop::SimpleRange& range, KDevelop::ParsingEnvironmentFile* file)
{
    if (!file) {
        file = new KDevelop::ParsingEnvironmentFile(editor()->currentUrl());
        file->setLanguage(KDevelop::IndexedString("Sgml"));
    }
    return new KDevelop::TopDUContext(editor()->currentUrl(), range, file);
}


KDevelop::DUContext* ContextBuilder::contextFromNode(AstNode* node)
{
    return 0;
}

KTextEditor::Range ContextBuilder::editorFindRange(AstNode* fromNode, AstNode* toNode)
{
    return static_cast<EditorIntegrator *>(editor())->findRange(fromNode, toNode).textRange();
}

KDevelop::QualifiedIdentifier ContextBuilder::identifierForNode(ElementAst* node)
{
    return KDevelop::QualifiedIdentifier();
}

void ContextBuilder::setContextOnNode(AstNode* node, KDevelop::DUContext* context)
{
    //node->ducontext = ctx;
}

void ContextBuilder::startVisiting(AstNode* node)
{
    visitNode(node);
}

void ContextBuilder::visitElementTag(ElementTagAst* node)
{
    //kDebug() << "Creating context..";
    KDevelop::SimpleRange range;
    EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
    range.start = e->findPosition(node->tclose, EditorIntegrator::BackEdge);
    range.end = findElementChildrenReach(node);//e->findPosition(node->endToken, EditorIntegrator::BackEdge);
    //kDebug() << "Context range" << QString("%1:%2").arg(range.start.line).arg(range.start.column) << QString("%1:%2").arg(range.end.line).arg(range.end.column);
    KDevelop::QualifiedIdentifier id(KDevelop::Identifier(KDevelop::IndexedString(tagName(node))));
    openContext(node, range,
                KDevelop::DUContext::Class,
                id);
    DefaultVisitor::visitElementTag(node);
    closeContext();
}

void ContextBuilder::visitDtdCondition(DtdConditionAst* node)
{
    KDevelop::SimpleRange range;
    EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
    range.start = e->findPosition(node->copen, EditorIntegrator::BackEdge);
    range.end = e->findPosition(node->cclose, EditorIntegrator::FrontEdge);
    KDevelop::QualifiedIdentifier id(KDevelop::Identifier(KDevelop::IndexedString("CONDITION")));
    openContext(node, range,
                KDevelop::DUContext::Other,
                id);
    DefaultVisitor::visitDtdCondition(node);
    closeContext();
}

void ContextBuilder::visitDtdDoctype(DtdDoctypeAst* node)
{
    KDevelop::SimpleRange range;
    EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
    range.start = e->findPosition(node->copen, EditorIntegrator::BackEdge);
    range.end = e->findPosition(node->cclose, EditorIntegrator::FrontEdge);
    KDevelop::QualifiedIdentifier id(KDevelop::Identifier(KDevelop::IndexedString("DOCTYPE")));
    openContext(node, range,
                KDevelop::DUContext::Other,
                id);
    DefaultVisitor::visitDtdDoctype(node);
    closeContext();
}


KDevelop::SimpleCursor ContextBuilder::findElementChildrenReach(ElementTagAst* node)
{
    EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
    if(node->childrenSequence) {
        ElementAst *ast = node->childrenSequence->back()->element;
        //if(ast->element)
        //    kDebug() << ast->element->kind;
        if(ast->element && ast->element->kind == AstNode::ElementCloseTagKind)
            return e->findPosition(ast->endToken, EditorIntegrator::BackEdge);
    }
    return e->findPosition(node->endToken, EditorIntegrator::BackEdge);
}

QString ContextBuilder::tokenText(qint64 begin, qint64 end) const
{
    EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
    return e->parseSession()->contents().mid(begin, end - begin + 1);
}

QString ContextBuilder::tagName(const ElementTagAst *ast) const
{
    EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
    if (ast->ns && ast->name)
        return tokenText(e->parseSession()->tokenStream()->token(ast->ns->startToken).begin, e->parseSession()->tokenStream()->token(ast->ns->endToken).end) +
        ":" +
        tokenText(e->parseSession()->tokenStream()->token(ast->name->startToken).begin, e->parseSession()->tokenStream()->token(ast->name->endToken).end);
    if (ast->name)
        return tokenText(e->parseSession()->tokenStream()->token(ast->name->startToken).begin, e->parseSession()->tokenStream()->token(ast->name->endToken).end);
    return QString();
}


QString ContextBuilder::tagName(const ElementCloseTagAst *ast) const
{
    EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
    if (ast->ns && ast->name)
        return tokenText(e->parseSession()->tokenStream()->token(ast->ns->startToken).begin, e->parseSession()->tokenStream()->token(ast->ns->endToken).end) +
        ":" +
        tokenText(e->parseSession()->tokenStream()->token(ast->name->startToken).begin, e->parseSession()->tokenStream()->token(ast->name->endToken).end);
    if (ast->name)
        return tokenText(e->parseSession()->tokenStream()->token(ast->name->startToken).begin, e->parseSession()->tokenStream()->token(ast->name->endToken).end);
    return QString();
}


