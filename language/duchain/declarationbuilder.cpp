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

KDevelop::QualifiedIdentifier DeclarationBuilder::identifierForNode(ElementTagAst* node)
{
    return KDevelop::QualifiedIdentifier(KDevelop::Identifier(KDevelop::IndexedString(tagName(node))));
}


void DeclarationBuilder::visitElementTag(ElementTagAst* node)
{
    {
        //kDebug() << "Creating declaration.. TAG";
        EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
        KDevelop::SimpleRange range;
        range.start = e->findPosition(node->startToken, EditorIntegrator::FrontEdge);
        range.end = e->findPosition(node->tclose, EditorIntegrator::BackEdge);
        KDevelop::QualifiedIdentifier id(KDevelop::Identifier(KDevelop::IndexedString(tagName(node))));
        KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
        KDevelop::ClassDeclaration* dec = openDeclaration<KDevelop::ClassDeclaration>(id, range);
        dec->setKind(KDevelop::Declaration::Instance);
        dec->clearBaseClasses();
        dec->setClassType(KDevelop::ClassDeclarationData::Class);
        //kDebug() << "Declaration range" << QString("%1:%2").arg(range.start.line).arg(range.start.column) << QString("%1:%2").arg(range.end.line).arg(range.end.column);
        //kDebug() << "Declaration: Class -> Instance TAG" << tagName(node);
    }
    DeclarationBuilderBase::visitElementTag(node);
    closeDeclaration();
}


QString DeclarationBuilder::tokenText(qint64 begin, qint64 end) const
{
    EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
    return e->parseSession()->contents().mid(begin, end - begin + 1);
}

QString DeclarationBuilder::tokenText(AstNode *node) const
{
    if (!node) return QString();
    EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
    return tokenText(e->parseSession()->tokenStream()->token(node->startToken).begin, e->parseSession()->tokenStream()->token(node->endToken).end);
}

QString DeclarationBuilder::tagName(const ElementTagAst *ast) const
{
    if (!ast) return QString();
    if (ast->ns && ast->name)
        return tokenText(ast->ns) + ":" + tokenText(ast->name);
    if (ast->name)
        return tokenText(ast->name);
    return QString();
}


QString DeclarationBuilder::tagName(const ElementCloseTagAst *ast) const
{
    if (ast->ns && ast->name)
        return tokenText(ast->ns) + ":" + tokenText(ast->name);
    if (ast->name)
        return tokenText(ast->name);
    return QString();
}


void DeclarationBuilder::visitDtdDoctype(DtdDoctypeAst* node)
{
    {
        //kDebug() << "Creating declaration DTD";
        EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
        KDevelop::SimpleRange range;
        range.start = e->findPosition(node->startToken, EditorIntegrator::FrontEdge);
        range.end = e->findPosition(node->endToken, EditorIntegrator::BackEdge);
        KDevelop::QualifiedIdentifier id(KDevelop::Identifier(KDevelop::IndexedString(tokenText(node->name))));
        KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
        KDevelop::Declaration * dec = openDeclaration<KDevelop::Declaration>(id, range);
        dec->setKind(KDevelop::Declaration::Namespace);
    }
    DeclarationBuilderBase::visitDtdDoctype(node);
    closeDeclaration();
}

void DeclarationBuilder::visitDtdElement(DtdElementAst* node)
{

    EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
    KDevelop::SimpleRange range;
    range.start = e->findPosition(node->startToken, EditorIntegrator::FrontEdge);
    range.end = e->findPosition(node->endToken, EditorIntegrator::BackEdge);

    QStringList elements;
    if (node->name)
        elements << tokenText(node->name);
    else if (node->elementsSequence)
        for (int i = 0; i < node->elementsSequence->count(); i++)
            elements << tokenText(node->elementsSequence->at(i)->element->name);
    else {
        //TODO report error
        return;
    }
    foreach(const QString &name, elements) {
        KDevelop::QualifiedIdentifier id(KDevelop::Identifier(KDevelop::IndexedString(name.toLatin1())));
        {
            KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
            KDevelop::ClassDeclaration* dec = openDefinition<KDevelop::ClassDeclaration>(id, range);
            dec->setKind(KDevelop::Declaration::Type);
            dec->clearBaseClasses();
            dec->setClassType(KDevelop::ClassDeclarationData::Class);
        }
        DeclarationBuilderBase::visitDtdElement(node);
        closeDeclaration();
    }

}

void DeclarationBuilder::visitElementCDATA(ElementCDATAAst* node)
{
    {
        //kDebug() << "Creating declaration CDATA";
        EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
        KDevelop::SimpleRange range;
        range.start = e->findPosition(node->startToken, EditorIntegrator::FrontEdge);
        range.end = e->findPosition(node->endToken, EditorIntegrator::BackEdge);
        KDevelop::QualifiedIdentifier id(KDevelop::Identifier(KDevelop::IndexedString("CDATA")));
        KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
        KDevelop::ClassDeclaration* dec = openDeclaration<KDevelop::ClassDeclaration>(id, range);
        dec->setKind(KDevelop::Declaration::Instance);
        dec->clearBaseClasses();
        dec->setClassType(KDevelop::ClassDeclarationData::Class);
        //kDebug() << "Declaration range" << QString("%1:%2").arg(range.start.line).arg(range.start.column) << QString("%1:%2").arg(range.end.line).arg(range.end.column);
        //kDebug() << "Declaration: Class -> Instance CDATA";
    }
    DeclarationBuilderBase::visitElementCDATA(node);
    closeDeclaration();
}


void DeclarationBuilder::visitElementPCDATA(ElementPCDATAAst* node)
{
    {
        //kDebug() << "Creating declaration PCDATA";
        EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
        KDevelop::SimpleRange range;
        range.start = e->findPosition(node->startToken, EditorIntegrator::FrontEdge);
        range.end = e->findPosition(node->endToken, EditorIntegrator::BackEdge);
        KDevelop::QualifiedIdentifier id(KDevelop::Identifier(KDevelop::IndexedString("PCDATA")));
        KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
        KDevelop::ClassDeclaration* dec = openDeclaration<KDevelop::ClassDeclaration>(id, range);
        dec->setKind(KDevelop::Declaration::Instance);
        dec->clearBaseClasses();
        dec->setClassType(KDevelop::ClassDeclarationData::Class);
        //kDebug() << "Declaration range" << QString("%1:%2").arg(range.start.line).arg(range.start.column) << QString("%1:%2").arg(range.end.line).arg(range.end.column);
        //kDebug() << "Declaration: Class -> Instance PCDATA";
    }
    DeclarationBuilderBase::visitElementPCDATA(node);
    closeDeclaration();
}

void DeclarationBuilder::visitElementPHP(ElementPHPAst* node)
{
    {
        //kDebug() << "Creating declaration PHP";
        EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
        KDevelop::SimpleRange range;
        range.start = e->findPosition(node->startToken, EditorIntegrator::FrontEdge);
        range.end = e->findPosition(node->endToken, EditorIntegrator::BackEdge);
        KDevelop::QualifiedIdentifier id(KDevelop::Identifier(KDevelop::IndexedString("PHP")));
        KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
        KDevelop::ClassDeclaration* dec = openDeclaration<KDevelop::ClassDeclaration>(id, range);
        dec->setKind(KDevelop::Declaration::Instance);
        dec->clearBaseClasses();
        dec->setClassType(KDevelop::ClassDeclarationData::Class);
        //kDebug() << "Declaration range" << QString("%1:%2").arg(range.start.line).arg(range.start.column) << QString("%1:%2").arg(range.end.line).arg(range.end.column);
        //kDebug() << "Declaration: Class -> Instance PHP";
    }
    DeclarationBuilderBase::visitElementPHP(node);
    closeDeclaration();
}


void DeclarationBuilder::visitElementProcessing(ElementProcessingAst* node)
{
    {
        //kDebug() << "Creating declaration PROCESSING";
        EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
        KDevelop::SimpleRange range;
        range.start = e->findPosition(node->startToken, EditorIntegrator::FrontEdge);
        range.end = e->findPosition(node->endToken, EditorIntegrator::BackEdge);
        KDevelop::QualifiedIdentifier id(KDevelop::Identifier(KDevelop::IndexedString("PROCESSING")));
        KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
        KDevelop::ClassDeclaration* dec = openDeclaration<KDevelop::ClassDeclaration>(id, range);
        dec->setKind(KDevelop::Declaration::Instance);
        dec->clearBaseClasses();
        dec->setClassType(KDevelop::ClassDeclarationData::Class);
        //kDebug() << "Declaration range" << QString("%1:%2").arg(range.start.line).arg(range.start.column) << QString("%1:%2").arg(range.end.line).arg(range.end.column);
        // kDebug() << "Declaration: Class -> Instance PROCESSING";
    }
    DeclarationBuilderBase::visitElementProcessing(node);
    closeDeclaration();
}


void DeclarationBuilder::visitElementSource(ElementSourceAst* node)
{
    {
        //kDebug() << "Creating declaration SOURCE";
        EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
        KDevelop::SimpleRange range;
        range.start = e->findPosition(node->startToken, EditorIntegrator::FrontEdge);
        range.end = e->findPosition(node->endToken, EditorIntegrator::BackEdge);
        KDevelop::QualifiedIdentifier id(KDevelop::Identifier(KDevelop::IndexedString("SOURCE")));
        KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
        KDevelop::ClassDeclaration* dec = openDeclaration<KDevelop::ClassDeclaration>(id, range);
        dec->setKind(KDevelop::Declaration::Instance);
        dec->clearBaseClasses();
        dec->setClassType(KDevelop::ClassDeclarationData::Class);
        //kDebug() << "Declaration range" << QString("%1:%2").arg(range.start.line).arg(range.start.column) << QString("%1:%2").arg(range.end.line).arg(range.end.column);
        //kDebug() << "Declaration: Class -> Instance SOURCE";
    }
    DeclarationBuilderBase::visitElementSource(node);
    closeDeclaration();
}


void DeclarationBuilder::visitElementText(ElementTextAst* node)
{
    {
        //kDebug() << "Creating declaration TEXT";
        EditorIntegrator *e = static_cast<EditorIntegrator *>(editor());
        KDevelop::SimpleRange range;
        range.start = e->findPosition(node->startToken, EditorIntegrator::FrontEdge);
        range.end = e->findPosition(node->endToken, EditorIntegrator::BackEdge);
        KDevelop::QualifiedIdentifier id(KDevelop::Identifier(KDevelop::IndexedString("TEXT")));
        KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
        KDevelop::ClassDeclaration* dec = openDeclaration<KDevelop::ClassDeclaration>(id, range);
        dec->setKind(KDevelop::Declaration::Instance);
        dec->clearBaseClasses();
        dec->setClassType(KDevelop::ClassDeclarationData::Class);
        //kDebug() << "Declaration range" << QString("%1:%2").arg(range.start.line).arg(range.start.column) << QString("%1:%2").arg(range.end.line).arg(range.end.column);
        //kDebug() << "Declaration: Class -> Instance TEXT";
    }
    DeclarationBuilderBase::visitElementText(node);
    closeDeclaration();
}

