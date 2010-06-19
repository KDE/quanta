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
#include "elementdeclaration.h"
#include "../language_debug.h"

#include <xmlcatalog/cataloghelper.h>

#include <language/duchain/classdeclaration.h>
#include <language/duchain/namespacealiasdeclaration.h>
#include <language/duchain/aliasdeclaration.h>


using namespace Xml;

DeclarationBuilder::DeclarationBuilder(EditorIntegrator* editor): DeclarationBuilderBase()
{
    setEditor(editor);
}


DeclarationBuilder::~DeclarationBuilder() {}

void DeclarationBuilder::closeDeclaration()
{
    eventuallyAssignInternalContext();
    DeclarationBuilderBase::closeDeclaration();
}

ElementDeclaration* DeclarationBuilder::createClassInstanceDeclaration(const QString &identifier,
        const KDevelop::SimpleRange &range,
        ElementDeclarationData::ElementType type,
        const QString &nsPrefix)
{

    //SGML ignore case
    KDevelop::QualifiedIdentifier id;
    if (!nsPrefix.isEmpty())
        id.push(KDevelop::Identifier(nsPrefix.toLower()));
    id.push(KDevelop::Identifier(identifier.toLower()));
    KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
    ElementDeclaration* dec = openDefinition<ElementDeclaration>(id, range);
    dec->setKind(KDevelop::Declaration::Instance);
    dec->clearBaseClasses();
    dec->setClassType(KDevelop::ClassDeclarationData::Class);
    dec->setName(identifier);
    dec->setElementType(type);
    dec->setNamespacePrefix(nsPrefix);
    return dec;
}

KDevelop::Declaration* DeclarationBuilder::createAliasDeclaration(const QString& identifier, const KDevelop::SimpleRange& range, KDevelop::Declaration* alias)
{
    debug() << "Alias created for" << identifier;
    KDevelop::QualifiedIdentifier id(KDevelop::Identifier(KDevelop::IndexedString(identifier.toUtf8())));
    KDevelop::AliasDeclaration *dec = 0;
    KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
    dec = openDefinition<KDevelop::AliasDeclaration>(id, range);
    dec->setAliasedDeclaration(KDevelop::IndexedDeclaration(alias));
    closeDeclaration();
    return dec;
}


KDevelop::Declaration* DeclarationBuilder::createImportDeclaration(const QString& identifier, const KDevelop::SimpleRange& range, const KUrl& url)
{

    KDevelop::QualifiedIdentifier id(KDevelop::Identifier(KDevelop::IndexedString(identifier.toUtf8())));
    KDevelop::DUChainWriteLocker lock;

    KDevelop::TopDUContext *includedCtx = KDevelop::DUChain::self()->chainForDocument(url);
    if ( !includedCtx ) {
        // invalid include
        return NULL;
    }

    //Create in topcontext
    injectContext(editor()->smart(),currentContext()->topContext());

    KDevelop::Declaration* dec = openDefinition<KDevelop::Declaration>(id, range);
    dec->setKind(KDevelop::Declaration::Import);
    injectContext(editor()->smart(), includedCtx);
    eventuallyAssignInternalContext();
    closeInjectedContext(editor()->smart());
    closeDeclaration();
    closeInjectedContext(editor()->smart());
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
        if (node->publicId || node->systemId || node->name) {
            DeclarationBuilderBase::visitDtdDoctype(node);
            QString publicId = nodeText(node->publicId);
            QString systemId = nodeText(node->systemId);
            QString doctype = nodeText(node->name);
            KUrl url = CatalogHelper::resolve(publicId, systemId, QString(), doctype, KMimeType::Ptr(), this->editor()->currentUrl().toUrl());
            if (url.isValid())
                createImportDeclaration(url.pathOrUrl(), range, url);
            //closeDeclaration();
        }
    }
}

/* <!ELEMENT (element1 | element2) (element3 | (element4, element5)) -(exclude6 | exclude7)>
 * Declares two classes with a bunch of blah's excluding some blah's
 */
void DeclarationBuilder::visitDtdElement(DtdElementAst* node)
{
    typedef QPair< QString, KDevelop::SimpleRange > NameRangePair;
    QList<NameRangePair> elements;
    if (node->name) {
        elements << NameRangePair(nodeText(node->name), nodeRange(node->name));

    } else if (node->elementsSequence) {
        for (int i = 0; i < node->elementsSequence->count(); i++) {
            elements << NameRangePair(nodeText(node->elementsSequence->at(i)->element->name),
                                      nodeRange(node->elementsSequence->at(i)->element->name));
        }
    } else {
        //TODO report error
        return;
    }

    // -(exclude6 | exclude7) We will ignore these..
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
                    m_dtdElementExclude.insert(name.toLower(),name.toLower());
            }
            it = it->next;
        }
        while (it != end);
    }

    //for each (element1 | element2)
    foreach(const NameRangePair &element, elements) {
        KDevelop::QualifiedIdentifier id(KDevelop::Identifier(KDevelop::IndexedString(element.first.toLower().toUtf8())));
        {
            KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
            ElementDeclaration* dec = openDefinition<ElementDeclaration>(id, element.second);
            dec->setKind(KDevelop::Declaration::Type);
            dec->clearBaseClasses();
            dec->clearAttributes();
            dec->setClassType(KDevelop::ClassDeclarationData::Class);
            dec->setElementType(ElementDeclarationData::Element);
            dec->setName(element.first);
            if (node->content) {
                dec->setContentType(nodeText(node->content));
            }
            if (node->closeTag && editor()->parseSession()->tokenStream()->token(node->closeTag).kind == Parser::Token_OPT) {
                dec->setCloseTagRequired(false);
            }
            m_dtdElements.insert(element.first.toLower(), dec);
        }
        //TODO visitDtdElementList: there should be an indication if its choice or sequence for now its just a list.
        setDtdElementId(id);
        DeclarationBuilderBase::visitDtdElement(node);
        closeDeclaration();
    }

}

// (element3 | (element4, element5))
void DeclarationBuilder::visitDtdElementList(DtdElementListAst* node)
{
    KDevelop::SimpleRange range;
    QString name;
    if (node->name) {
        name = nodeText(node->name);
        range = nodeRange(node->name);
    } else if (node->type) {
        name = QString("#%1").arg(nodeText(node->type));
        range = nodeRange(node->type);
    }

    if (!name.isNull() && !m_dtdElementExclude.contains(name.toLower())) {
        createClassInstanceDeclaration(name, range, ElementDeclarationData::Element);
        closeDeclaration();
    }

    DeclarationBuilderBase::visitDtdElementList(node);
}

// +(exclude6 | exclude7)
void DeclarationBuilder::visitDtdElementIncludeItem(DtdElementIncludeItemAst* node)
{
    KDevelop::SimpleRange range;
    QString name;
    if (node->name) {
        name = nodeText(node->name);
        range = nodeRange(node->name);
    } else if (node->type) {
        name = QString("#%1").arg(nodeText(node->type));
        range = nodeRange(node->type);
    }

    if (!name.isNull() && !m_dtdElementExclude.contains(name.toLower())) {
        createClassInstanceDeclaration(name, range, ElementDeclarationData::Element);
        closeDeclaration();
    }

    DeclarationBuilderBase::visitDtdElementIncludeItem(node);
}

void DeclarationBuilder::visitDtdAttlist(DtdAttlistAst* node)
{
    QStringList elements;
    if (node->name)
        elements << nodeText(node->name);
    if (node->elementsSequence) {
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
    if (node->attributesSequence) {
        foreach(QString elementName, elements ) {
            if (!m_dtdElements.contains(elementName.toLower())) {
                debug() << "Could not find element" << elementName;
                continue; //TODO print warning
            }
            ElementDeclaration *dec = m_dtdElements.value(elementName.toLower());
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
}

void DeclarationBuilder::visitDtdEntity(DtdEntityAst* node)
{
    if (node->name && ( node->publicId || node->systemId)) {
        //Its an import
        IncludeIdentifier id;
        if (node->publicId) {
            id.publicId = KDevelop::IndexedString(nodeText(node->publicId));
        }
        if (node->systemId) {
            id.systemId = KDevelop::IndexedString(nodeText(node->systemId));
        }
        m_entities.insert(nodeText(node->name), id);
    } else if (node->name && !node->persent) {
        //Its text
        KDevelop::SimpleRange range = nodeRange(node);
        QString identifier = nodeText(node->name);
        KDevelop::QualifiedIdentifier id(KDevelop::Identifier(KDevelop::IndexedString(identifier.toLower().toUtf8())));
        {
            KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
            ElementDeclaration* dec = openDefinition<ElementDeclaration>(id, range);
            dec->setKind(KDevelop::Declaration::Type);
            dec->clearBaseClasses();
            dec->clearAttributes();
            dec->setClassType(KDevelop::ClassDeclarationData::Class);
            dec->setName(identifier);
            dec->setElementType(ElementDeclarationData::Entity);
            dec->setContent(nodeText(node->value));
        }
        closeDeclaration();
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
    KDevelop::SimpleRange range = nodeRange(node);
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
    if (!node || !node->name)
        return;

    //Create namespace declarations defined in XSD's
    //This must happen before any other attributes/elements is processed
    KDevelop::Declaration *namespaceDeclaration = 0;

    if (node->attributesSequence && node->attributesSequence->count() > 0) {
        for (int i = 0 ; i  < node->attributesSequence->count(); i++) {

            AttributeAst * attrib = node->attributesSequence->at(i)->element;
            if (!attrib || !attrib->value || !attrib->name)
                continue;

            //Namespaces
            if (attrib && attrib->name && attrib->value && nodeText(attrib->name) == "targetNamespace") {
                KDevelop::SimpleRange range;
                EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
                range.start = e->findPosition(node->tclose, EditorIntegrator::BackEdge);
                range.end = findElementChildrenReach(node);
                KDevelop::QualifiedIdentifier id(KDevelop::Identifier(KDevelop::IndexedString(nodeText(attrib->value))));
                KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
                KDevelop::Declaration *decl = openDefinition<KDevelop::Declaration>(id, nodeRange(attrib->value));
                if (decl) {
                    decl->setKind(KDevelop::Declaration::Namespace);
                    namespaceDeclaration = decl;
                    openContext(node, range, KDevelop::DUContext::Namespace, id);
                }
            }
        }
    }

    //Create namespace alias
    if (node->ns) {
        KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
        KDevelop::Declaration *dec = findNamespaceAliasDeclaration(currentContext()->topContext(), nodeText(node->ns));
        if (dec)
            createAliasDeclaration(nodeText(node->ns), nodeRange(node->ns), dec);
    }

    //Create element declarations
    createClassInstanceDeclaration(nodeText(node->name), nodeRange(node->name), ElementDeclarationData::Element, nodeText(node->ns));
    DeclarationBuilderBase::visitElementTag(node);
    closeDeclaration();

    //Close namespace declaration and context if any
    if (namespaceDeclaration) {
        closeContext();
        closeDeclaration();
    }
}

//Marks the end of an elements context and is a child of the element
void DeclarationBuilder::visitElementCloseTag(ElementCloseTagAst* node)
{
    if (node->ns) {
        KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
        KDevelop::Declaration *dec = findNamespaceAliasDeclaration(currentContext()->topContext(), nodeText(node->ns));
        if (dec)
            createAliasDeclaration(nodeText(node->ns), nodeRange(node->ns), dec);
    }
    
    KDevelop::SimpleRange range = nodeRange(node);
    createClassInstanceDeclaration(nodeText(node->name), nodeRange(node->name), ElementDeclarationData::CloseTag, nodeText(node->ns));
    DeclarationBuilderBase::visitElementCloseTag(node);
    closeDeclaration();
}


/* Among attributes we need to keep record of namespaces and namespace prefixes so what I have so far:
   Attributes: blah="duh"
   Namespaces aliases: xmlns:ns="namespace-uri"
   Namespaces using: xmlns="namespace-uri"
   Namespace declaration: targetNamespace -> created in element
   Includes:  xmlns:ns, schemaLocation, noNamespaceSchemaLocation and xmlns
*/
void DeclarationBuilder::visitAttribute(AttributeAst* node)
{
    if (!node)
        return;
    DeclarationBuilderBase::visitAttribute(node);

    //Includes
    if (node->value) {
        KDevelop::SimpleRange range = nodeRange(node->value);
        IncludeIdentifier incid;
        if (node->ns && nodeText(node->ns) == "xmlns") {
            incid.uri = KDevelop::IndexedString(nodeText(node->value));
        } else if (node->name && nodeText(node->name) == "schemaLocation") {
            QStringList values = nodeText(node->value).split(QRegExp("\\s+"));
            for (int i = 0; i < values.length() && values.length()%2==0; i+=2) {
                incid.systemId = KDevelop::IndexedString(values[i+1]);
                incid.uri = KDevelop::IndexedString(values[i]);
            }
        } else if (node->name && nodeText(node->name) == "noNamespaceSchemaLocation") {
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
            } else {
                //TODO a propper unresolved id
                KDevelop::QualifiedIdentifier id(KDevelop::Identifier(KDevelop::IndexedString("")));
                KDevelop::DUChainWriteLocker lock;
                KDevelop::Declaration *dec = openDeclaration<KDevelop::Declaration>(id,range);
                dec->setKind(KDevelop::Declaration::Instance);
                closeDeclaration();
            }
        }
    }

    //Namespace aliases
    if (node->ns && node->name && node->value && nodeText(node->ns) == "xmlns") {
        KDevelop::SimpleRange range = nodeRange(node->name);
        KDevelop::QualifiedIdentifier id(KDevelop::Identifier(KDevelop::IndexedString(nodeText(node->name))));
        KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());

        //Create alias in topcontext
        injectContext(editor()->smart(),currentContext()->topContext());
        KDevelop::NamespaceAliasDeclaration *dec = openDefinition<KDevelop::NamespaceAliasDeclaration>(id, range);

        //Find the namespace
        KDevelop::Declaration *nsDec = findNamespaceDeclaration(currentContext()->topContext(), nodeText(node->value));
        if (nsDec)
            dec->setImportIdentifier(nsDec->qualifiedIdentifier());

        closeDeclaration();
        closeInjectedContext(editor()->smart());
    }

    //Namespace use
    if (node->name && node->value && nodeText(node->name) == "xmlns") {
        KDevelop::SimpleRange range = nodeRange(node->value);
        KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());

        //Create namespace use in topcontext
        injectContext(editor()->smart(),currentContext()->topContext());
        KDevelop::QualifiedIdentifier id(globalImportIdentifier());
        KDevelop::NamespaceAliasDeclaration *dec = openDefinition<KDevelop::NamespaceAliasDeclaration>(id, range);

        //Find the namespace
        KDevelop::Declaration *nsDec = findNamespaceDeclaration(currentContext()->topContext(), nodeText(node->value));
        if (nsDec)
            dec->setImportIdentifier(nsDec->qualifiedIdentifier());

        closeDeclaration();
        closeInjectedContext(editor()->smart());
    }

    //Create namespace alias
    if (node->ns)
    {
        KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
        KDevelop::Declaration *dec = findNamespaceAliasDeclaration(currentContext()->topContext(), nodeText(node->ns));
        if (dec)
            createAliasDeclaration(nodeText(node->ns), nodeRange(node->ns), dec);
    }
}

void DeclarationBuilder::visitElementCDATA(ElementCDATAAst* node)
{
    KDevelop::SimpleRange range = nodeRange(node);
    createClassInstanceDeclaration("CDATA", range, ElementDeclarationData::CDATA);
    DeclarationBuilderBase::visitElementCDATA(node);
    closeDeclaration();
}


void DeclarationBuilder::visitElementPCDATA(ElementPCDATAAst* node)
{
    KDevelop::SimpleRange range = nodeRange(node);
    createClassInstanceDeclaration("PCDATA", range, ElementDeclarationData::PCDATA);
    DeclarationBuilderBase::visitElementPCDATA(node);
    closeDeclaration();
}

void DeclarationBuilder::visitElementText(ElementTextAst* node)
{
    KDevelop::SimpleRange range = nodeRange(node);
    createClassInstanceDeclaration("TEXT", range, ElementDeclarationData::Text);
    DeclarationBuilderBase::visitElementText(node);
    closeDeclaration();
}

KDevelop::Declaration* DeclarationBuilder::findNamespaceDeclaration(KDevelop::TopDUContext *context, const QString& ns)
{
    QList<KDevelop::Declaration *> nsDecl = context->findDeclarations(KDevelop::Identifier(KDevelop::IndexedString(ns)));
    foreach(KDevelop::Declaration *d, nsDecl) {
        if (d && d->kind() == KDevelop::Declaration::Namespace)
            return d;
    }
    foreach(KDevelop::DUContext::Import i, context->importedParentContexts()) {
        KDevelop::Declaration *d = findNamespaceDeclaration(i.indexedContext().context()->topContext(), ns);
        if (d) return d;
    }
    debug() << "Unable to find namespace declaration:" << ns;
    return 0;
}

KDevelop::Declaration* DeclarationBuilder::findNamespaceAliasDeclaration(KDevelop::TopDUContext* context, const QString& ns)
{
    QList<KDevelop::Declaration *> nsDecl = context->findDeclarations(KDevelop::Identifier(KDevelop::IndexedString(ns)));
    foreach(KDevelop::Declaration *d, nsDecl) {
        if (d && d->kind() == KDevelop::Declaration::NamespaceAlias) {
            KDevelop::NamespaceAliasDeclaration *nsDec = dynamic_cast<KDevelop::NamespaceAliasDeclaration *>(d);
            if(!nsDec) continue;
            return nsDec;
        }
    }
    foreach(KDevelop::DUContext::Import i, context->importedParentContexts()) {
        KDevelop::Declaration *d = findNamespaceDeclaration(i.indexedContext().context()->topContext(), ns);
        if (d) return d;
    }
    debug() << "Unable to find namespace alias declaration:" << ns;
    return 0;
}


