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
#include <xmlcatalog/cataloghelper.h>

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

void ContextBuilder::visitAttribute(AttributeAst* node)
{
    Xml::DefaultVisitor::visitAttribute(node);
    EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
    KDevelop::SimpleRange range;
    range.start = e->findPosition(node->startToken, EditorIntegrator::FrontEdge);
    range.end = e->findPosition(node->endToken, EditorIntegrator::BackEdge);
    IncludeIdentifier incid;
    if (node->ns && node->value && nodeText(node->ns) == "xmlns") {
        incid.uri = KDevelop::IndexedString(nodeText(node->value));
    } else if (node->name && node->value && nodeText(node->name) == "xmlns") {
        incid.uri = KDevelop::IndexedString(nodeText(node->value));
    } else if (node->name && node->value && nodeText(node->name) == "schemaLocation") {
        QStringList values = nodeText(node->value).split(QRegExp("\\s+"));
        for (int i = 0; i < values.length() && values.length()%2==0; i+=2) {
            incid.systemId = KDevelop::IndexedString(values[i+1]);
            incid.uri = KDevelop::IndexedString(values[i]);
        }
    } else if (node->name && node->value && nodeText(node->name) == "noNamespaceSchemaLocation") {
        incid.systemId = KDevelop::IndexedString(nodeText(node->value));
    }
    if (!incid.isNull()) {
        KUrl url = CatalogHelper::resolve(QString(),
                                          incid.systemId.str(),
                                          incid.uri.str(),
                                          QString(),
                                          KMimeType::Ptr(),
                                          this->editor()->currentUrl().toUrl());
        if ( url.isValid() ) {
            KDevelop::DUChainWriteLocker lock;
            KDevelop::TopDUContext* includedCtx = KDevelop::DUChain::self()->chainForDocument(url);
            if (includedCtx) {
                currentContext()->topContext()->addImportedParentContext(includedCtx);
                currentContext()->topContext()->parsingEnvironmentFile()->addModificationRevisions(includedCtx->parsingEnvironmentFile()->allModificationRevisions());
            }
        }
    }
}

void ContextBuilder::visitDtdCondition(DtdConditionAst* node)
{
    /*
    KDevelop::SimpleRange range;
    EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
    if (node->childrenSequence) {
        range.start = e->findPosition(node->copen, EditorIntegrator::BackEdge);
        range.end = e->findPosition(node->cclose, EditorIntegrator::FrontEdge);
        KDevelop::QualifiedIdentifier id(KDevelop::Identifier(KDevelop::IndexedString("CONDITION")));
        openContext(node, range,
                    KDevelop::DUContext::Other,
                    id);
        DefaultVisitor::visitDtdCondition(node);
        closeContext();
    } else {
        DefaultVisitor::visitDtdCondition(node);
    }
    */
    DefaultVisitor::visitDtdCondition(node);
}

void ContextBuilder::visitDtdDoctype(DtdDoctypeAst* node)
{
    KDevelop::SimpleRange range;
    EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
    if (node->childrenSequence) {
        range.start = e->findPosition(node->copen, EditorIntegrator::BackEdge);
        range.end = e->findPosition(node->cclose, EditorIntegrator::FrontEdge);
        KDevelop::QualifiedIdentifier id(KDevelop::Identifier(KDevelop::IndexedString(nodeText(node->name))));
        openContext(node, range,
                    KDevelop::DUContext::Other,
                    id);
        DefaultVisitor::visitDtdDoctype(node);
        closeContext();
    } else {
        DefaultVisitor::visitDtdDoctype(node);
        QString publicId = nodeText(node->publicId);
        QString systemId = nodeText(node->systemId);
        QString doctype = nodeText(node->name);
        KUrl url = CatalogHelper::resolve(publicId, systemId, QString(), doctype, KMimeType::Ptr(), this->editor()->currentUrl().toUrl());
        if ( url.isValid() ) {
            KDevelop::DUChainWriteLocker lock;
            KDevelop::TopDUContext* includedCtx = KDevelop::DUChain::self()->chainForDocument(url);
            if (includedCtx) {
                currentContext()->topContext()->addImportedParentContext(includedCtx);
                currentContext()->topContext()->parsingEnvironmentFile()->addModificationRevisions(includedCtx->parsingEnvironmentFile()->allModificationRevisions());
            }
        }
    }
}

void ContextBuilder::visitDtdEntityInclude(DtdEntityIncludeAst* node)
{
    DefaultVisitor::visitDtdEntityInclude(node);
    QString entity;
    if (node->name) {
        entity = nodeText(node->name);
    }
    EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
    KDevelop::SimpleRange range;
    range.start = e->findPosition(node->startToken, EditorIntegrator::FrontEdge);
    range.end = e->findPosition(node->endToken, EditorIntegrator::BackEdge);
    if (!entity.isEmpty() && m_entities.contains(entity)) {
        IncludeIdentifier incid = m_entities.value(entity);
        QString publicId = incid.publicId.str();
        QString systemId = incid.systemId.str();
        KUrl url = CatalogHelper::resolve(publicId, systemId, QString(), QString(), KMimeType::Ptr(), this->editor()->currentUrl().toUrl());
        if ( url.isValid() ) {
            KDevelop::QualifiedIdentifier id(KDevelop::Identifier(KDevelop::IndexedString(nodeText(node->name))));
            KDevelop::DUChainWriteLocker lock;
            KDevelop::TopDUContext* includedCtx = KDevelop::DUChain::self()->chainForDocument(url);
            if (includedCtx) {
                currentContext()->topContext()->addImportedParentContext(includedCtx);
                currentContext()->topContext()->parsingEnvironmentFile()->addModificationRevisions(includedCtx->parsingEnvironmentFile()->allModificationRevisions());
            }
        }
    }
}


void ContextBuilder::visitDtdElement(DtdElementAst* node)
{
    KDevelop::SimpleRange range;
    EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
    range.start = e->findPosition(node->topen , EditorIntegrator::FrontEdge);
    range.end = e->findPosition(node->tclose, EditorIntegrator::BackEdge);
    openContext(node, range,
                KDevelop::DUContext::Other,
                m_dtdElementId);
    //We do not want to visit the dtd element twice so we call visitDtdElementList directly
    DefaultVisitor::visitDtdElement(node);
    closeContext();
}


KDevelop::SimpleCursor ContextBuilder::findElementChildrenReach(ElementTagAst* node)
{
    EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
    if (node->childrenSequence) {
        ElementAst *ast = node->childrenSequence->back()->element;
        if (ast->element && ast->element->kind == AstNode::ElementCloseTagKind)
            return e->findPosition(ast->endToken, EditorIntegrator::BackEdge);
    }
    return e->findPosition(node->endToken, EditorIntegrator::BackEdge);
}

QString ContextBuilder::nodeText(AstNode *node) const
{
    if (!node) return QString();
    EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
    return tokenText(e->parseSession()->tokenStream()->token(node->startToken).begin, e->parseSession()->tokenStream()->token(node->endToken).end);
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

void ContextBuilder::reportProblem(KDevelop::ProblemData::Severity , AstNode* ast, const QString& message)
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

