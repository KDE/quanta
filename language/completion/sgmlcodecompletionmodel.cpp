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

#include "sgmlcodecompletionmodel.h"

#include <KTextEditor/View>
#include <KTextEditor/Document>

#include <language/duchain/topducontext.h>
#include <language/duchain/duchainlock.h>
#include <language/duchain/duchain.h>
#include <language/duchain/declaration.h>
#include <language/duchain/classdeclaration.h>
#include <KMimeType>

using namespace Xml;

using namespace KDevelop;
using namespace KTextEditor;

SgmlCodeCompletionModel::SgmlCodeCompletionModel(QObject* parent): CodeCompletionModel2(parent)
{
    m_attributeCompletion = false;
}

SgmlCodeCompletionModel::~SgmlCodeCompletionModel()
{

}


QChar SgmlCodeCompletionModel::getSeperator(Document* document, const KTextEditor::Cursor& position) const
{
    static QString seps = "</:!>?";
    QString line = document->line(position.line());
    KTextEditor::Cursor pos = position;
    QChar posc;
    int posi = position.column();
    while (posi-- > 0) {
        pos.setColumn(posi);
        posc = document->character(pos);
        if (seps.contains(posc)) {
            return posc;
        }
        if (posc.isSpace())
            continue;
        return QChar('\0');
    }
    return QChar('\0');
}

QList< SgmlCodeCompletionModel::CompletionItem > SgmlCodeCompletionModel::findAll(KTextEditor::Document* document, const KTextEditor::Range& range) const
{
    DUChainReadLocker lock;
    TopDUContext * tc = 0;
    QHash < QString,  CompletionItem > items;

    tc = DUChain::self()->chainForDocument(document->url());
    if(!tc) return items.values();

    foreach(Declaration * d, tc->localDeclarations(tc)) {
        if (!d) continue;
        if (d->kind() == Declaration::Type) {
            QString name = d->identifier().toString();
            items.insert(name, CompletionItem(name,0));
        }
    }
    foreach(DUContext::Import i, tc->importedParentContexts()) {
        if (i.indexedContext().context()) {
            foreach(Declaration * d, i.indexedContext().context()->localDeclarations()) {
                if (!d) continue;
                if (d->kind() == Declaration::Type) {
                    QString name = d->identifier().toString();
                    if (!name.startsWith("#"))
                        items.insert(name, CompletionItem(name,0));
                }
            }
        }
    }
    return items.values();
}

QList< SgmlCodeCompletionModel::CompletionItem > SgmlCodeCompletionModel::findAttributes(KTextEditor::Document* document, const KTextEditor::Range& range) const
{
    DUChainReadLocker lock;
    DUContext * ctx = 0;
    Declaration *dec = 0;
    ElementDeclaration *elementDec = 0;
    QString name;
    QList<Declaration *> decs;
    TopDUContext * tc = 0;
    QHash< QString, CompletionItem > items;
    kDebug();
    tc = DUChain::self()->chainForDocument(document->url());
    if(!tc) { kDebug() << "No top context"; return items.values();}

    ctx = tc->findContextAt(SimpleCursor(range.start()));
    if(!ctx) { kDebug() << "No context"; return items.values();}

    dec = ctx->findDeclarationAt(SimpleCursor(range.start()));
    if(!dec) { kDebug() << "No declaration"; return items.values();}

    name = dec->identifier().toString();

    decs.append(tc->findDeclarations(Identifier(name), SimpleCursor::invalid(), tc));
    decs.append(tc->findDeclarations(Identifier(name.toUpper()), SimpleCursor::invalid(), tc));
    decs.append(tc->findDeclarations(Identifier(name.toLower()), SimpleCursor::invalid(), tc));

    foreach(Declaration * d, decs) {
        if (!d || d->kind() != Declaration::Type || !d->internalContext())
            continue;
        elementDec = dynamic_cast<ElementDeclaration *>(d);
        if(!elementDec) continue;
        for(int i = 0; i < elementDec->attributesSize(); i++) {
            IndexedString str = elementDec->attributes()[i];
            items.insert(str.str(), CompletionItem(str.str(), 0));
        }
    }
    
    return items.values();
}

QList< SgmlCodeCompletionModel::CompletionItem > SgmlCodeCompletionModel::findChildElements(KTextEditor::Document* document, const KTextEditor::Range& range) const
{
    DUChainReadLocker lock;
    DUContext * ctx = 0;
    Declaration *dec = 0;
    ElementDeclaration *elementDec = 0;
    QString name;
    QList<Declaration *> decs;
    TopDUContext * tc = 0;
    QHash<QString, CompletionItem > items;
    
    tc = DUChain::self()->chainForDocument(document->url());
    if(!tc) return items.values();
    
    ctx = tc->findContextAt(SimpleCursor(range.start()));
    if(!ctx) return items.values();
    
    dec = ctx->owner();
    if(!dec) return items.values();
    
    name = dec->identifier().toString();
    
    decs.append(tc->findDeclarations(Identifier(name), SimpleCursor::invalid(), tc));
    decs.append(tc->findDeclarations(Identifier(name.toUpper()), SimpleCursor::invalid(), tc));
    decs.append(tc->findDeclarations(Identifier(name.toLower()), SimpleCursor::invalid(), tc));

    foreach(Declaration * dc, decs) {
        if (!dc || dc->kind() != Declaration::Type || !dc->internalContext())
            continue;
        foreach(Declaration * d, dc->internalContext()->localDeclarations()) {
            if (!d || d->kind() != Declaration::Instance) continue;
            QString name = d->identifier().toString();
            if (!name.startsWith("#"))
                items.insert(name, CompletionItem(name,10));
        }
    }
    
    return items.values();
}


void SgmlCodeCompletionModel::completionInvoked(KTextEditor::View* view, const KTextEditor::Range& range, KTextEditor::CodeCompletionModel::InvocationType invocationType)
{
    KUrl url = view->document()->url();
    QChar seperator = getSeperator(view->document(), range.start());
    kDebug() << view->document()->text(range) << seperator.toAscii();
    setRowCount(0);
    m_items.clear();
    m_attributeCompletion = false;

    if (seperator == '!') {
        m_items.append(CompletionItem("DOCTYPE", 10));
        m_items.append(CompletionItem("ELEMENT", 10));
        m_items.append(CompletionItem("ATTLIST", 10));
        m_items.append(CompletionItem("NOTATION", 10));
        m_items.append(CompletionItem("[CDATA[", 0));
        m_items.append(CompletionItem("[PCDATA[", 0));
        setRowCount(m_items.size());
        return;
    }

    {
        DUChainReadLocker lock;
        DUContext * ctx = 0;
        Declaration *dec = 0;
        TopDUContext * tc = 0;
        tc = DUChain::self()->chainForDocument(view->document()->url());
        if(!tc) return;
        
        ctx = tc->findContextAt(SimpleCursor(range.start()));
        if(ctx) {
            dec = ctx->findDeclarationAt(SimpleCursor(range.start()));
            if(dec && seperator == '\0') {
                m_items.append(findAttributes(view->document(), range));
                m_attributeCompletion = true;
            } else {
                dec = ctx->owner();
                if(dec) {
                    if(seperator == '/' || seperator == '>') {
                        m_items.append(CompletionItem(QString("/%1").arg(dec->identifier().toString()), 10));
                    }
                }
                m_items.append(findChildElements(view->document(), range));
                m_items.append(findAll(view->document(), range));
            }
        } else {
            m_items.append(findAll(view->document(), range));
        }
    }
    setRowCount(m_items.count());
}

KTextEditor::Range SgmlCodeCompletionModel::completionRange(KTextEditor::View* view, const KTextEditor::Cursor& position)
{
    const static QString seps = "/\\!<>:= \r\n\t[]()'\"";
    KTextEditor::Cursor start = position;
    KTextEditor::Cursor end = position;
    QString text = view->document()->line(position.line());
    while (start.column() >= 0 && !seps.contains(text.at(start.column()))) {
        start.setColumn(start.column()-1);
        if (seps.contains(text.at(start.column()))) {
            start.setColumn(start.column()+1);
            break;
        }
    }
    if (start.column() < 0)
        start.setColumn(0);
    while (end.column() < text.length() && !seps.contains(text.at(end.column()))) {
        end.setColumn(end.column()+1);
        if (seps.contains(text.at(end.column()))) {
            end.setColumn(end.column());
            break;
        }
    }
    if (end.column() < 0)
        end.setColumn(0);
    return Range(start, end);
}

/*QMap< int, QVariant > SgmlCodeCompletionModel::itemData(const QModelIndex& index) const
{
    return KTextEditor::CodeCompletionModel::itemData(index);
}*/


QVariant SgmlCodeCompletionModel::data(const QModelIndex& index, int role) const
{
    if (index.parent().isValid())
        return QVariant();
    if (m_items.count() <= index.row())
        return QVariant();

    if (role == Qt::DisplayRole && index.column() == CodeCompletionModel::Name)
        return QVariant(m_items.at(index.row()).first);

    if (role == MatchQuality)
        return QVariant(m_items.at(index.row()).second);

    if (role == InheritanceDepth)
        return QVariant(10 - m_items.at(index.row()).second + 1);

    return QVariant();
}


QString SgmlCodeCompletionModel::getIndentstring(int depth) const
{
    QString ret;
    for (int i = 0; i < depth; i++)
        ret += "    ";
    return ret;
}

QString SgmlCodeCompletionModel::formatString(const QString& str) const
{
    return str;
}

Range SgmlCodeCompletionModel::growRangeLeft(Document* document, const Range& range, const QString& condition) const
{
    Range r = range;
    while (r.start().column() > 0) {
        r.start().setColumn(r.start().column() -1);
        QChar c = document->character(r.start());
        if (condition.contains(c))
            continue;
        r.start().setColumn(r.start().column() + 1);
        break;
    }
    return r;
}

Range SgmlCodeCompletionModel::growRangeRight(Document* document, const Range& range, const QString& condition) const
{
    Range r = range;
    QString line = document->line(range.end().line());
    while (r.end().column() < line.size()) {
        QChar c = document->character(r.end());
        if (condition.contains(c)) {
            r.end().setColumn(r.end().column() +1);
            continue;
        }
        break;
    }
    return r;
}

void SgmlCodeCompletionModel::executeCompletionItem2(KTextEditor::Document* document, const KTextEditor::Range& word, const QModelIndex& index) const
{
    QString text = formatString(data(index.sibling(index.row(), Name), Qt::DisplayRole).toString());
    QString line = document->line(word.start().line());
    QString trimmedLine = line.mid(0, word.start().column()).remove('/');
    trimmedLine = trimmedLine.remove('<');
    trimmedLine = trimmedLine.remove('>').trimmed();
    int depth = 0;
    QChar seperator = getSeperator(document, word.start());

    {
        DUChainReadLocker lock;
        DUContext * ctx;
        TopDUContext * tc;
        tc = DUChain::self()->chainForDocument(document->url());
        if (!tc) {
            kDebug() << "No top context";
            return;
        }
        ctx = tc->findContext(SimpleCursor(word.start()));
        if (ctx)
            depth = ctx->depth();
    }

    //After replacement move cursur to start of context ie <blah>|</blah>
    if (seperator == '/' || text.startsWith('/')) {
        if (text.startsWith('/'))
            text = text.mid(1, text.size() -1);
        Range range = word;
        range = growRangeLeft(document, range, "</");
        kDebug() << "right1" << line.at(range.end().column());
        range = growRangeRight(document, range, ">");
        kDebug() << "right2" << line.at(range.end().column());
        if (trimmedLine.isEmpty()) {
            Range r = range;
            r.start().setColumn(0);
            text = QString("%1</%2>").arg(getIndentstring(depth-1),text);
            document->replaceText(r, text);
        } else {
            text = QString("</%1>").arg(text);
            document->replaceText(range, text);
        }
        foreach(View * v, document->views()) {
            if (v->isActiveView())
                v->setCursorPosition(range.start());
        }
        return;
    }

    Range range = word;
    if ((seperator == '<' || seperator == '>') &&  !m_attributeCompletion) {
        range = growRangeLeft(document, range, "<");
        if (trimmedLine.isEmpty()) {
            range.start().setColumn(0);
            text = QString("%1<%2").arg(getIndentstring(depth),text);
            document->replaceText(range, text);
        } else {
            text = QString("<%1").arg(text);
            document->replaceText(range, text);
        }
    } else if (seperator.isNull() && !m_attributeCompletion) {
        range = growRangeLeft(document, range, "<");
        if (trimmedLine.isEmpty()) {
            range.start().setColumn(0);
            text = QString("%1<%2").arg(getIndentstring(depth),text);
            document->replaceText(range, text);
        } else {
            text = QString("<%1").arg(text);
            document->replaceText(range, text);
        }
    } else if (m_attributeCompletion) {
        range = growRangeRight(document, range, "=");
        text = QString("%1=").arg(text);
        document->replaceText(range, text);

        range.end().setColumn(range.start().column() + text.size());
        QString growSeps = "=\"'";
        range = growRangeRight(document, range, growSeps);
        if(growSeps.contains(document->character(range.end())))
            range.end().setColumn(range.end().column() + 1);
        foreach(View * v, document->views()) {
            if (v->isActiveView())
                v->setCursorPosition(range.end());
        }
        return;
    } else {
        text = QString("%1").arg(text);
        document->replaceText(range, text);
    }

    //After replacement move cursur to end of tag ie: <blah>|
    range.end().setColumn(range.start().column() + text.size());
    QString growSeps = "=\"'/>";
    range = growRangeRight(document, range, growSeps);
    if(growSeps.contains(document->character(range.end())))
        range.end().setColumn(range.end().column() + 1);
    foreach(View * v, document->views()) {
        if (v->isActiveView())
            v->setCursorPosition(range.end());
    }
}

bool SgmlCodeCompletionModel::shouldAbortCompletion(KTextEditor::View* view, const KTextEditor::SmartRange& range, const QString& currentCompletion)
{
    return false;
}

bool SgmlCodeCompletionModel::shouldStartCompletion(View* view, const QString& insertedText, bool userInsertion, const KTextEditor::Cursor& position)
{
    return !getSeperator(view->document(), position).isNull();
}



#include "sgmlcodecompletionmodel.moc"
