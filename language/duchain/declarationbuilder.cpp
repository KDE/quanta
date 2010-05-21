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

#include "declarationbuilder.h"
#include "editorintegrator.h"
#include "parsesession.h"

#include <language/duchain/classdeclaration.h>
#include "elementdeclaration.h"
#include <xmlcatalog/cataloghelper.h>

using namespace Xml;

DeclarationBuilder::DeclarationBuilder(KDevelop::EditorIntegrator* editor): DeclarationBuilderBase()
{
    setEditor(editor, false);
}


DeclarationBuilder::~DeclarationBuilder() {}

void DeclarationBuilder::closeDeclaration()
{
    eventuallyAssignInternalContext();
    DeclarationBuilderBase::closeDeclaration();
}

ElementDeclaration* DeclarationBuilder::createClassInstanceDeclaration(const QString &identifier,
        const KDevelop::SimpleRange &range,
        const QString &name,
        const QString &nameSpacePrefix)
{
    Q_UNUSED(nameSpacePrefix);
    KDevelop::QualifiedIdentifier id(KDevelop::Identifier(KDevelop::IndexedString(identifier.toUtf8())));
    KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
    ElementDeclaration* dec = openDefinition<ElementDeclaration>(id, range);
    dec->setKind(KDevelop::Declaration::Instance);
    dec->clearBaseClasses();
    dec->setClassType(KDevelop::ClassDeclarationData::Class);
    dec->setName(name);
    //dec->setNameSpacePrefix(nameSpacePrefix);
    return dec;
}

KDevelop::Declaration* DeclarationBuilder::createImportDeclaration(const QString& identifier, const KDevelop::SimpleRange& range, const KUrl& url)
{
    KDevelop::QualifiedIdentifier id(KDevelop::Identifier(KDevelop::IndexedString(identifier.toUtf8())));
    KDevelop::DUChainWriteLocker lock;
    KDevelop::TopDUContext* includedCtx = KDevelop::DUChain::self()->chainForDocument(url);
    if ( !includedCtx ) {
        // invalid include
        return NULL;
    }
    KDevelop::Declaration* dec = openDefinition<KDevelop::Declaration>(id, range/*KDevelop::SimpleRange(0, 0, 0, 0)*/);
    dec->setKind(KDevelop::Declaration::Import);
    //injectContext(editor()->smart(), includedCtx);
    //eventuallyAssignInternalContext();
    //closeInjectedContext(editor()->smart());
    closeDeclaration();
    return dec;
}

void DeclarationBuilder::visitDtdDoctype(DtdDoctypeAst* node)
{
    {
        EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
        KDevelop::SimpleRange range;
        range.start = e->findPosition(node->startToken, EditorIntegrator::FrontEdge);
        if (!node->childrenSequence)
            range.end = e->findPosition(node->endToken, EditorIntegrator::BackEdge);
        else
            range.end = e->findPosition(node->copen, EditorIntegrator::FrontEdge);
        if (node->childrenSequence) {
            KDevelop::QualifiedIdentifier id(KDevelop::Identifier(KDevelop::IndexedString(nodeText(node->name))));
            KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
            KDevelop::Declaration * dec = openDefinition<KDevelop::Declaration>(id, range);
            dec->setKind(KDevelop::Declaration::Namespace);
            DeclarationBuilderBase::visitDtdDoctype(node);
            closeDeclaration();
        } else {
            DeclarationBuilderBase::visitDtdDoctype(node);
            QString publicId = nodeText(node->publicId);
            QString systemId = nodeText(node->systemId);
            QString doctype = nodeText(node->name);
            KUrl url = CatalogHelper::resolve(publicId, systemId, QString(), doctype, KMimeType::Ptr(), this->editor()->currentUrl().toUrl());
            createImportDeclaration(url.pathOrUrl(), range, url);
            //closeDeclaration();
        }
    }
}

/* <!ELEMENT (element | element) (element | (element, element)) -(exclude | exclude)>
 * Declares to classes with a bunch of blah's excluding some blah's
 */
void DeclarationBuilder::visitDtdElement(DtdElementAst* node)
{
    EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
    KDevelop::SimpleRange range;
    range.start = e->findPosition(node->startToken, EditorIntegrator::FrontEdge);
    range.end = e->findPosition(node->endToken, EditorIntegrator::BackEdge);

    QStringList elements;
    if (node->name)
        elements << nodeText(node->name);
    else if (node->elementsSequence)
        for (int i = 0; i < node->elementsSequence->count(); i++)
            elements << nodeText(node->elementsSequence->at(i)->element->name);
    else {
        //TODO report error
        return;
    }

    // -(exclude | exclude) We will ignore these..
    m_dtdElementExclude.clear();
    if (node->excludeSequence) {
        const KDevPG::ListNode<DtdEnumItemAst*> *it = node->excludeSequence->front(), *end = it;
        do
        {
            if (it->element) {
                QString name;
                if (it->element->name) {
                    name = nodeText(it->element->name);
                } else if (it->element->type) {
                    name = QString("#%1").arg(nodeText(it->element->type));
                }
                if (!name.isEmpty())
                    m_dtdElementExclude.insert(name,name);
            }
            it = it->next;
        }
        while (it != end);
    }

    //for each <!ELEMENT (element | element)
    foreach(const QString &name, elements) {
        KDevelop::QualifiedIdentifier id(KDevelop::Identifier(KDevelop::IndexedString(name.toLatin1())));
        {
            KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
            ElementDeclaration* dec = openDefinition<ElementDeclaration>(id, range);
            dec->setKind(KDevelop::Declaration::Type);
            dec->clearBaseClasses();
            dec->clearAttributes();
            dec->setClassType(KDevelop::ClassDeclarationData::Class);
            if (node->content) {
                dec->setContentType(nodeText(node->content));
            }
            //if (e->tokenToString(node->openTag) == "O") {
            //    dec->setOpenTagRequired(false);
            //}
            if (e->tokenToString(node->closeTag) == "O") {
                dec->setCloseTagRequired(false);
            }
            m_dtdElements.insert(name, dec);
        }
        //TODO visitDtdElementList: there should be a indication if its choice or sequence for now its just a list.
        setStdElementId(id);
        DeclarationBuilderBase::visitDtdElement(node);
        closeDeclaration();
    }

}

void DeclarationBuilder::visitDtdElementList(DtdElementListAst* node)
{
    EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
    KDevelop::SimpleRange range;
    QString name;
    if (node->name) {
        name = nodeText(node->name);
        range.start = e->findPosition(node->name->startToken, EditorIntegrator::FrontEdge);
        range.end = e->findPosition(node->name->endToken, EditorIntegrator::BackEdge);
    } else if (node->type) {
        name = QString("#%1").arg(nodeText(node->type));
        range.start = e->findPosition(node->type->startToken, EditorIntegrator::FrontEdge);
        range.end = e->findPosition(node->type->endToken, EditorIntegrator::BackEdge);
    }

    if (!name.isNull() && !m_dtdElementExclude.contains(name)) {
        createClassInstanceDeclaration(name, range, name);
        closeDeclaration();
    }

    //Happens outside declaration, recursive (blah | (blah, blah) | blah ...)
    DeclarationBuilderBase::visitDtdElementList(node);
}

void DeclarationBuilder::visitDtdElementIncludeItem(DtdElementIncludeItemAst* node)
{
    EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
    KDevelop::SimpleRange range;
    QString name;
    if (node->name) {
        name = nodeText(node->name);
        range.start = e->findPosition(node->name->startToken, EditorIntegrator::FrontEdge);
        range.end = e->findPosition(node->name->endToken, EditorIntegrator::BackEdge);
    } else if (node->type) {
        name = QString("#%1").arg(nodeText(node->type));
        range.start = e->findPosition(node->type->startToken, EditorIntegrator::FrontEdge);
        range.end = e->findPosition(node->type->endToken, EditorIntegrator::BackEdge);
    }

    if (!name.isNull() && !m_dtdElementExclude.contains(name)) {
        createClassInstanceDeclaration(name, range, name);
        closeDeclaration();
    }

    DeclarationBuilderBase::visitDtdElementIncludeItem(node);
}

void DeclarationBuilder::visitDtdAttlist(DtdAttlistAst* node)
{
    QStringList elements;
    if(node->name)
        elements << nodeText(node->name);
    if(node->elementsSequence) {
        const KDevPG::ListNode<DtdEnumItemAst*> *it = node->elementsSequence->front(), *end = it;
        do
        {
            if (it->element) {
                QString name;
                if (it->element->name) {
                    name = nodeText(it->element->name);
                }
                if (!name.isEmpty())
                    elements << name;
            }
            it = it->next;
        }
        while (it != end);
    }
    foreach(QString elementName, elements ) {
        if(!m_dtdElements.contains(elementName)) {
            kDebug() << "Could not find element" << elementName;
            continue; //TODO print warning
        }
        ElementDeclaration *dec = m_dtdElements.value(elementName);
        KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
        const KDevPG::ListNode<DtdAttAst*> *it = node->attributesSequence->front(), *end = it;
        do
        {
            if (it->element) {
                QString name;
                if (it->element->name) {
                    name = nodeText(it->element->name);
                }
                if (!name.isEmpty()) {
                    dec->addAttributes(KDevelop::IndexedString(name));
                }
            }
            it = it->next;
        }
        while (it != end);
    }
}

void DeclarationBuilder::visitDtdEntity(DtdEntityAst* node)
{
    IncludeIdentifier id;
    if (node->publicId) {
        id.publicId = KDevelop::IndexedString(nodeText(node->publicId));
    }
    if (node->systemId) {
        id.systemId = KDevelop::IndexedString(nodeText(node->systemId));
    }
    if (node->name && (!id.publicId.isEmpty() || !id.systemId.isEmpty())) {
        m_entities.insert(nodeText(node->name), id);
    }
    DeclarationBuilderBase::visitDtdEntity(node);
}

void DeclarationBuilder::visitDtdEntityInclude(DtdEntityIncludeAst* node)
{
    DeclarationBuilderBase::visitDtdEntityInclude(node);
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
        KUrl url = CatalogHelper::resolve(incid.publicId.str(),
                                          incid.systemId.str(),
                                          QString(),
                                          QString(),
                                          KMimeType::Ptr(),
                                          this->editor()->currentUrl().toUrl());
        if ( url.isValid() ) {
            createImportDeclaration(url.pathOrUrl(), range, url);
            //closeDeclaration();
        }
    }
}

void DeclarationBuilder::visitElementTag(ElementTagAst* node)
{
    EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
    KDevelop::SimpleRange range;
    range.start = e->findPosition(node->startToken, EditorIntegrator::FrontEdge);
    range.end = e->findPosition(node->tclose, EditorIntegrator::BackEdge);
    createClassInstanceDeclaration(tagName(node), range, nodeText(node->name), nodeText(node->ns));
    DeclarationBuilderBase::visitElementTag(node);
    closeDeclaration();
}


void DeclarationBuilder::visitAttribute(AttributeAst* node)
{
    Xml::ContextBuilder::visitAttribute(node);
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
            createImportDeclaration(url.pathOrUrl(), range, url);
            //closeDeclaration();
        }
    }
}

void DeclarationBuilder::visitElementCDATA(ElementCDATAAst* node)
{
    EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
    KDevelop::SimpleRange range;
    range.start = e->findPosition(node->startToken, EditorIntegrator::FrontEdge);
    range.end = e->findPosition(node->endToken, EditorIntegrator::BackEdge);
    createClassInstanceDeclaration("CDATA", range);
    DeclarationBuilderBase::visitElementCDATA(node);
    closeDeclaration();
}


void DeclarationBuilder::visitElementPCDATA(ElementPCDATAAst* node)
{
    EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
    KDevelop::SimpleRange range;
    range.start = e->findPosition(node->startToken, EditorIntegrator::FrontEdge);
    range.end = e->findPosition(node->endToken, EditorIntegrator::BackEdge);
    createClassInstanceDeclaration("PCDATA", range);
    DeclarationBuilderBase::visitElementPCDATA(node);
    closeDeclaration();
}

void DeclarationBuilder::visitElementPHP(ElementPHPAst* node)
{
    EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
    KDevelop::SimpleRange range;
    range.start = e->findPosition(node->startToken, EditorIntegrator::FrontEdge);
    range.end = e->findPosition(node->endToken, EditorIntegrator::BackEdge);
    createClassInstanceDeclaration("PHP", range);
    DeclarationBuilderBase::visitElementPHP(node);
    closeDeclaration();
}


void DeclarationBuilder::visitElementProcessing(ElementProcessingAst* node)
{
    EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
    KDevelop::SimpleRange range;
    range.start = e->findPosition(node->startToken, EditorIntegrator::FrontEdge);
    range.end = e->findPosition(node->endToken, EditorIntegrator::BackEdge);
    createClassInstanceDeclaration("PROCESSING", range);
    DeclarationBuilderBase::visitElementProcessing(node);
    closeDeclaration();
}


void DeclarationBuilder::visitElementSource(ElementSourceAst* node)
{
    EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
    KDevelop::SimpleRange range;
    range.start = e->findPosition(node->startToken, EditorIntegrator::FrontEdge);
    range.end = e->findPosition(node->endToken, EditorIntegrator::BackEdge);
    createClassInstanceDeclaration("SOURCE", range);
    DeclarationBuilderBase::visitElementSource(node);
    closeDeclaration();
}


void DeclarationBuilder::visitElementText(ElementTextAst* node)
{
    EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
    KDevelop::SimpleRange range;
    range.start = e->findPosition(node->startToken, EditorIntegrator::FrontEdge);
    range.end = e->findPosition(node->endToken, EditorIntegrator::BackEdge);
    createClassInstanceDeclaration("TEXT", range);
    DeclarationBuilderBase::visitElementText(node);
    closeDeclaration();
}

KDevelop::QualifiedIdentifier DeclarationBuilder::identifierForNode(ElementTagAst* node)
{
    return KDevelop::QualifiedIdentifier(KDevelop::Identifier(KDevelop::IndexedString(tagName(node))));
}
