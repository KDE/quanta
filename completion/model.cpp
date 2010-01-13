/*
   Copyright (C) 2009 Niko Sams <niko.sams@gmail.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "model.h"

#include <KTextEditor/View>
#include <KTextEditor/Document>

#include <cssdefaultvisitor.h>
#include <cssdebugvisitor.h>
#include "../parser/parsesession.h"
#include "../parser/editorintegrator.h"
#include "contentassistdata.h"

namespace Css {

extern int debugArea();
#define debug() kDebug(debugArea())

CodeCompletionModel::CodeCompletionModel(QObject *parent)
    : CodeCompletionModel2(parent), m_completionContext(NoContext), m_assistData(new ContentAssistData)
{
}



class FindCurrentNodeVisitor : public DefaultVisitor
{
public:
    FindCurrentNodeVisitor(EditorIntegrator* editor, const KTextEditor::Range& range)
        : m_editor(editor), m_range(range), m_lastSelectorElement(-1), m_lastProperty(-1),
          m_context(CodeCompletionModel::SelectorContext)
    {}

    virtual void visitDeclaration(DeclarationAst* node)
    {
        {
            KDevelop::SimpleCursor pos = m_editor->findPosition(node->startToken, EditorIntegrator::FrontEdge);
            debug() << m_editor->tokenToString(node->startToken) << m_range.start() << pos.textCursor();
            if (m_range.start() >= pos.textCursor()) {
                if (node->colon != -1 && m_range.start() >= m_editor->findPosition(node->colon, EditorIntegrator::FrontEdge).textCursor()) {
                    debug() << "using ValueContext";
                    m_context = CodeCompletionModel::ValueContext;
                } else {
                    debug() << "using PropertyContext";
                    m_context = CodeCompletionModel::PropertyContext;
                }
            }
        }
        {
            if (node->semicolon != -1 && m_range.start() >= m_editor->findPosition(node->semicolon, EditorIntegrator::FrontEdge).textCursor()) {
                debug() << "using PropertyContext";
                m_context = CodeCompletionModel::PropertyContext;
            }
        }
        DefaultVisitor::visitDeclaration(node);
    }

    virtual void visitRuleset(RulesetAst* node)
    {
        if (node->lbrace != -1 && m_range.start() >= m_editor->findPosition(node->lbrace).textCursor()) {
            debug() << "using PropertyContext";
            m_context = CodeCompletionModel::PropertyContext;
        } else if (m_range.start() >= m_editor->findPosition(node->startToken, EditorIntegrator::FrontEdge).textCursor()) {
            debug() << "using SelectorContext 1";
            m_context = CodeCompletionModel::SelectorContext;
        }
        DefaultVisitor::visitRuleset(node);
        if (node->rbrace != -1 && m_range.start() >= m_editor->findPosition(node->rbrace).textCursor()) {
            debug() << "using SelectorContext 2";
            m_context = CodeCompletionModel::SelectorContext;
        }
    }

    virtual void visitSimpleSelector(SimpleSelectorAst* node)
    {
        if (node->element) {
            kDebug() << m_lastProperty << m_range.start() << m_editor->findPosition(node->endToken).textCursor();
            if (m_range.start() > m_editor->findPosition(node->endToken).textCursor()) {
                m_lastSelectorElement = node->element->ident;
                kDebug() << "set lastSelectorElement" << m_editor->tokenToString(m_lastSelectorElement);
            }
        }
        DefaultVisitor::visitSimpleSelector(node);
    }

    virtual void visitProperty(PropertyAst* node)
    {
        kDebug() << m_lastProperty << m_range.start() << m_editor->findPosition(node->endToken).textCursor();
        if (m_range.start() > m_editor->findPosition(node->endToken).textCursor()) {
            m_lastProperty = node->ident;
            kDebug() << "set lastProperty" << m_editor->tokenToString(m_lastProperty);
        }
        DefaultVisitor::visitProperty(node);
    }

    CodeCompletionModel::CompletionContext currentContext() { return m_context; }
    QString lastSelectorElement() {
        if (m_lastSelectorElement == -1) return QString();
        return m_editor->tokenToString(m_lastSelectorElement);
    }
    QString lastProperty() {
        if (m_lastProperty == -1) return QString();
        return m_editor->tokenToString(m_lastProperty);
    }
private:
    EditorIntegrator *m_editor;
    KTextEditor::Range m_range;
    qint64 m_lastSelectorElement;
    qint64 m_lastProperty;
    CodeCompletionModel::CompletionContext m_context;
};

void CodeCompletionModel::completionInvoked(KTextEditor::View* view, const KTextEditor::Range& range, InvocationType invocationType)
{
    Q_UNUSED(invocationType);

    debug() << range;
    ParseSession session;
    session.setContents(view->document()->text());
    Css::StartAst* ast = 0;
    session.parse(&ast);
    DebugVisitor vis(session.tokenStream(), session.contents());
    vis.visitNode(ast);

    if (ast) {
        EditorIntegrator editor(&session);

        FindCurrentNodeVisitor visitor(&editor, range);
        visitor.visitNode(ast);
        m_completionContext = visitor.currentContext();
        debug() << "context" << m_completionContext;
        switch (m_completionContext) {
            case PropertyContext:
            {
                debug() << "lastSelectorElement" << visitor.lastSelectorElement();
                ContentAssistData::Element element = m_assistData->element(visitor.lastSelectorElement());
                m_items = element.fields;
                setRowCount(m_items.count());
                reset();
                return;
            }
            case ValueContext:
            {
                debug() << "lastProperty" << visitor.lastProperty();
                ContentAssistData::Field field = m_assistData->field(visitor.lastProperty());
                m_items = field.values.keys();
                setRowCount(m_items.count());
                reset();
                return;
            }
            case SelectorContext:
                m_items = m_assistData->elements();
                setRowCount(m_items.count());
                reset();
                return;
            default:
                break;
        }
    }
    m_items.clear();
    setRowCount(0);
    reset();
}

QVariant CodeCompletionModel::data(const QModelIndex & index, int role) const
{
    if (index.parent().isValid()) return QVariant();
    if (role == Qt::DisplayRole && index.column() == CodeCompletionModel::Name) {
        if (m_items.count() < index.row()) return QVariant();
        return m_items.at(index.row());
    }
    return QVariant();
}

//default implementation with '-' added to reg exps
KTextEditor::Range CodeCompletionModel::completionRange(KTextEditor::View* view, const KTextEditor::Cursor &position)
{
    KTextEditor::Cursor end = position;

    QString text = " "+view->document()->line(end.line())+" ";

    static QRegExp findWordStart( "[^_\\w\\-]([_\\w\\-]+)$" );
    static QRegExp findWordEnd( "^([_\\w\\-]*)[^_\\w\\-]" );

    KTextEditor::Cursor start = end;

    //debug() << end << text.left(end.column()+1);
    if (findWordStart.lastIndexIn(text.left(end.column()+1)) >= 0)
        start.setColumn(findWordStart.pos(1)-1);
    //debug() << findWordStart.cap(0);

    if (findWordEnd.indexIn(text.mid(end.column()+1)) >= 0)
        end.setColumn(end.column()+1 + findWordEnd.cap(1).length()-1);
    //debug() << findWordEnd.cap(0);

    KTextEditor::Range ret = KTextEditor::Range(start, end);

    return ret;
}

//default implementation with '-' added to reg exp
bool CodeCompletionModel::shouldAbortCompletion(KTextEditor::View* view, const KTextEditor::SmartRange& range, const QString& currentCompletion)
{
    Q_UNUSED(view);
    Q_UNUSED(range);
    static const QRegExp allowedText("^([\\w\\-]*)");
    bool ret = !allowedText.exactMatch(currentCompletion);
    debug() << currentCompletion << "shouldAbort:" << ret;
    return ret;
}

void CodeCompletionModel::executeCompletionItem2(KTextEditor::Document* document, const KTextEditor::Range& word, const QModelIndex& index) const
{
    QString text = data(index.sibling(index.row(), Name), Qt::DisplayRole).toString();
    if (m_completionContext == PropertyContext) {
        text += ':';
    } else if (m_completionContext == ValueContext) {
        text += ';';
    }
    document->replaceText(word, text);
}


}

#include "model.moc"
