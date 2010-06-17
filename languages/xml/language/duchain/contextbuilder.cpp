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

#include "parsesession.h"
#include "editorintegrator.h"
#include "sgmlast.h"
#include "../language_debug.h"

#include <xmlcatalog/cataloghelper.h>

#include <KDE/KLocalizedString>

#include <language/duchain/duchain.h>
#include <language/duchain/topducontext.h>
#include <language/duchain/duchainlock.h>
#include <language/duchain/declaration.h>
#include <language/duchain/classdeclaration.h>

#include <interfaces/icore.h>
#include <interfaces/ilanguagecontroller.h>
#include <interfaces/icompletionsettings.h>


using namespace Xml;

ContextBuilder::ContextBuilder() : m_mapAst(false)
{
}

ContextBuilder::~ContextBuilder()
{
}

EditorIntegrator* ContextBuilder::editor() const
{
    return static_cast<EditorIntegrator *>(ContextBuilderBase::editor());
}

void ContextBuilder::setEditor(EditorIntegrator* editor)
{
    ContextBuilderBase::setEditor(editor, false);
}


KDevelop::ReferencedTopDUContext ContextBuilder::build(const KDevelop::IndexedString& url, AstNode* node, KDevelop::ReferencedTopDUContext updateContext, bool useSmart)
{
    KDevelop::ReferencedTopDUContext top = KDevelop::AbstractContextBuilder< Xml::AstNode, Xml::IdentifierAst >::build(url, node, updateContext, useSmart);
    return top;
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
    Q_UNUSED(node);
    return 0;
}

KTextEditor::Range ContextBuilder::editorFindRange(AstNode* fromNode, AstNode* toNode)
{
    return static_cast<EditorIntegrator *>(editor())->findRange(fromNode, toNode).textRange();
}

KDevelop::QualifiedIdentifier ContextBuilder::identifierForNode(IdentifierAst* node)
{
    return KDevelop::QualifiedIdentifier(KDevelop::Identifier(nodeText(node)));
}

void ContextBuilder::setContextOnNode(AstNode* node, KDevelop::DUContext* context)
{
    Q_UNUSED(node);
    Q_UNUSED(context);
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
    range.end = findElementChildrenReach(node);
    KDevelop::QualifiedIdentifier id(KDevelop::Identifier(KDevelop::IndexedString(tagName(node).toLower())));
    openContext(node, range,
                KDevelop::DUContext::Class,
                id);
    DefaultVisitor::visitElementTag(node);
    closeContext();
}

void ContextBuilder::visitAttribute(AttributeAst* node)
{
    //TODO must create an abstract type for attributes and identifier must be case insensitive.
    Xml::DefaultVisitor::visitAttribute(node);

    KDevelop::SimpleRange range = nodeRange(node);

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
    DefaultVisitor::visitDtdCondition(node);
}

void ContextBuilder::visitDtdDoctype(DtdDoctypeAst* node)
{
    DefaultVisitor::visitDtdDoctype(node);
    if(node->publicId || node->systemId || node->name) {
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

    KDevelop::SimpleRange range = nodeRange(node);
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
                debug() << "Adding import context for entity:" << entity << publicId << systemId;
                currentContext()->topContext()->addImportedParentContext(includedCtx);
                currentContext()->topContext()->parsingEnvironmentFile()->addModificationRevisions(includedCtx->parsingEnvironmentFile()->allModificationRevisions());
            }
        } else
            debug() << "Unable to import entity:" << entity << publicId << systemId;
    } else
        debug() << "Unable to import entity:" << entity;
}

void ContextBuilder::visitDtdElement(DtdElementAst* node)
{
    //NOTE this is called for each element in the construct <!ELEMENT (element1, element2...) ...>
    KDevelop::SimpleRange range;
    EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
    if(node->name)
        range.start = e->findPosition(node->name->endToken , EditorIntegrator::BackEdge);
    else if(node->elementsSequence->count() > 0)
        range.start = e->findPosition(node->elementsSequence->back()->element->endToken, EditorIntegrator::BackEdge);
    else
        range.start = e->findPosition(node->topen, EditorIntegrator::BackEdge);
    range.end = e->findPosition(node->tclose, EditorIntegrator::FrontEdge);
    openContext(node, range,
                KDevelop::DUContext::Other,
                m_dtdElementId);
    //We do not want to visit the dtd element twice so we call visitDtdElementList directly
    DefaultVisitor::visitDtdElement(node);
    closeContext();
}


KDevelop::SimpleCursor ContextBuilder::findElementChildrenReach(ElementTagAst* node)
{
    if (node->childrenSequence) {
        ElementAst *ast = node->childrenSequence->back()->element;
        if (ast->element && ast->element->kind == AstNode::ElementCloseTagKind)
            return editor()->findPosition(ast->endToken, EditorIntegrator::BackEdge);
    }
    return editor()->findPosition(node->endToken, EditorIntegrator::BackEdge);
}

KDevelop::SimpleRange ContextBuilder::nodeRange(AstNode* node) const
{
    return editor()->findRange(node);
}

QString ContextBuilder::nodeText(AstNode *node) const
{
    return editor()->parseSession()->symbol(node);
}

QString ContextBuilder::tokenText(qint64 begin, qint64 end) const
{
    return editor()->parseSession()->contents().mid(begin, end - begin + 1);
}

QString ContextBuilder::tagName(const ElementTagAst *ast) const
{
    //NOTE: if that gets commented out, use the simpler code as shown below
    //if (ast->ns && ast->name)
    //    return tokenText(e->parseSession()->tokenStream()->token(ast->ns->startToken).begin, e->parseSession()->tokenStream()->token(ast->ns->endToken).end) +
    //           ":" +
    //           tokenText(e->parseSession()->tokenStream()->token(ast->name->startToken).begin, e->parseSession()->tokenStream()->token(ast->name->endToken).end);

    return editor()->parseSession()->symbol(ast->name);
}

QString ContextBuilder::tagName(const ElementCloseTagAst *ast) const
{
    //NOTE: if that gets commented out, use the simpler code as shown below
    //if (ast->ns && ast->name)
    //    return tokenText(e->parseSession()->tokenStream()->token(ast->ns->startToken).begin, e->parseSession()->tokenStream()->token(ast->ns->endToken).end) +
    //            ":" +
    //            tokenText(e->parseSession()->tokenStream()->token(ast->name->startToken).begin, e->parseSession()->tokenStream()->token(ast->name->endToken).end);

    return editor()->parseSession()->symbol(ast->name);
}

void ContextBuilder::reportProblem(KDevelop::ProblemData::Severity , AstNode* ast, const QString& message)
{
    KDevelop::Problem *p = new KDevelop::Problem();
    p->setSource(KDevelop::ProblemData::Parser);
    p->setSeverity(KDevelop::ProblemData::Error);
    p->setDescription(message);
    KDevelop::SimpleRange range = editor()->findRange(ast);
    p->setFinalLocation(KDevelop::DocumentRange(editor()->currentUrl().str(), KTextEditor::Range(range.start.line, range.start.column, range.end.line, range.end.column)));
    m_problems << KDevelop::ProblemPointer(p);
}
