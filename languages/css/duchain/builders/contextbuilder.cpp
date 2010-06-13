/***************************************************************************
 *   This file is part of KDevelop                                         *
 *   Copyright 2010 Niko Sams <niko.sams@gmail.com>                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

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

#include "../../parser/parsesession.h"
#include "../../parser/editorintegrator.h"
// #include "../helper.h"
#include "cssast.h"

namespace Css
{

ContextBuilder::ContextBuilder() : m_reportErrors(true)
{
}

ContextBuilder::~ContextBuilder()
{
}

KDevelop::ReferencedTopDUContext ContextBuilder::build(const KDevelop::IndexedString& url, AstNode* node,
        KDevelop::ReferencedTopDUContext updateContext, bool useSmart)
{
    if ( KDevelop::ICore::self() ) {
        m_reportErrors = KDevelop::ICore::self()->languageController()->completionSettings()->highlightSemanticProblems();
    }
    return ContextBuilderBase::build(url, node, updateContext, useSmart);
}

void ContextBuilder::setEditor(EditorIntegrator* editor)
{
    ContextBuilderBase::setEditor(editor, false);
}

void ContextBuilder::startVisiting(AstNode* node)
{
    if (node->kind == HtmlAst::KIND) {
        foreach (AstNode *el, static_cast<HtmlAst*>(node)->elements) {
            kDebug() << el->kind;
            if (el->kind == StyleElementAst::KIND) {
                editor()->setParseSession(static_cast<StyleElementAst*>(el)->session);
                visitNode(static_cast<StyleElementAst*>(el)->start);
            } else if (el->kind == InlineStyleAst::KIND) {
                editor()->setParseSession(static_cast<InlineStyleAst*>(el)->session);
                InlineStyleAst* n = static_cast<InlineStyleAst*>(el);

                KDevelop::SimpleRange range;
                range = editor()->findRange(n->declarationList);
                kDebug() << range.textRange();
                openContext(n, range,
                            KDevelop::DUContext::Class,
                            KDevelop::QualifiedIdentifier("TODO"));
                visitNode(n->declarationList);
                closeContext();
            } else {
                Q_ASSERT(0);
            }
        }
    } else {
        qDebug()  << node->kind;
        Q_ASSERT(node->kind == StartAst::KIND);
        visitNode(node);
    }
}

KDevelop::TopDUContext* ContextBuilder::newTopContext(const KDevelop::SimpleRange& range, KDevelop::ParsingEnvironmentFile* file)
{
    if (!file) {
        file = new KDevelop::ParsingEnvironmentFile(editor()->currentUrl());
        file->setLanguage(KDevelop::IndexedString("Css"));
    }
    return new KDevelop::TopDUContext(editor()->currentUrl(), range, file);
}

void ContextBuilder::setContextOnNode(AstNode* node, KDevelop::DUContext* ctx)
{
    //node->ducontext = ctx;
}

KDevelop::DUContext* ContextBuilder::contextFromNode(AstNode* node)
{
    //return node->ducontext;
    return 0;
}

EditorIntegrator* ContextBuilder::editor() const
{
    return static_cast<EditorIntegrator*>(ContextBuilderBase::editor());
}

KTextEditor::Range ContextBuilder::editorFindRange(AstNode* fromRange, AstNode* toRange)
{
    return editor()->findRange(fromRange, toRange).textRange();
}

KDevelop::SimpleCursor ContextBuilder::startPos(AstNode* node)
{
    return editor()->findPosition(node->startToken, KDevelop::EditorIntegrator::FrontEdge);
}
KDevelop::QualifiedIdentifier ContextBuilder::identifierForNode(SpecifierAst* id)
{
    if (!id)
        return KDevelop::QualifiedIdentifier();
    if (id->className != -1) {
        return KDevelop::QualifiedIdentifier(editor()->parseSession()->symbol(id->className));
    }
    return KDevelop::QualifiedIdentifier();
}

void ContextBuilder::visitRuleset(RulesetAst* node)
{
    kDebug(1) << node << node->declarations;
    Q_ASSERT(node->declarations);
    KDevelop::SimpleRange range;
    if (node->lbrace != -1) {
        range.start = editor()->findPosition(node->lbrace, EditorIntegrator::BackEdge);
    } else {
        range.start = editor()->findPosition(node->declarations->startToken, EditorIntegrator::FrontEdge);
    }
    range.end = editor()->findPosition(node->declarations->endToken, EditorIntegrator::BackEdge);
    openContext(node, range,
                KDevelop::DUContext::Class,
                KDevelop::QualifiedIdentifier("TODO"));
    DefaultVisitor::visitRuleset(node);
    closeContext();
}

}
