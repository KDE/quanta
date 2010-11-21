/*****************************************************************************
 * Copyright (c) 2007 Andreas Pakulat <apaku@gmx.de>                         *
 * Copyright (c) 2007 Piyush verma <piyush.verma@gmail.com>                  *
 * Copyright (c) 2008-2009 Niko Sams <niko.sams@gmail.com>                   *
 *                                                                           *
 * Permission is hereby granted, free of charge, to any person obtaining     *
 * a copy of this software and associated documentation files (the           *
 * "Software"), to deal in the Software without restriction, including       *
 * without limitation the rights to use, copy, modify, merge, publish,       *
 * distribute, sublicense, and/or sell copies of the Software, and to        *
 * permit persons to whom the Software is furnished to do so, subject to     *
 * the following conditions:                                                 *
 *                                                                           *
 * The above copyright notice and this permission notice shall be            *
 * included in all copies or substantial portions of the Software.           *
 *                                                                           *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,           *
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF        *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                     *
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE    *
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION    *
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION     *
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.           *
 *****************************************************************************/
#ifndef CSS_PARSESESSION_H
#define CSS_PARSESESSION_H

#include <QtCore/QString>
#include <language/editor/rangeinrevision.h>
#include "cssparser.h"

namespace KDevPG
{
class MemoryPool;
}

namespace Css
{
class StartAst;

typedef QPair<KDevelop::DUContextPointer, KDevelop::RangeInRevision> SimpleUse;

class KDEVCSSPARSER_EXPORT ParseSession
{
public:
    ParseSession();
    ~ParseSession();

    void setContents(const QString& contents);
    void setCurrentDocument( const KDevelop::IndexedString& filename);
    bool readFile(const QString& filename, const char* charset = 0);
    void setDebug(bool);
    void setOffset(const KDevelop::CursorInRevision &offset);
    KDevPG::TokenStream* tokenStream() const;
    QString contents() const;
    KDevelop::IndexedString currentDocument() const;

    bool parse(Css::StartAst**);
    bool parse(Css::DeclarationListAst**);
    Parser* createParser();

    QString symbol(qint64 token) const;
    QString symbol(AstNode* node) const;

    /**
     * Return the position (\a line%, \a column%) of the \a offset in the file.
     *
     * \note the line starts from 0.
     */
    KDevelop::CursorInRevision positionAt(qint64 offset) const;

    QList<KDevelop::ProblemPointer> problems();

    //TODO warning implement this
    void mapAstUse(AstNode* node, const SimpleUse& use)
    {
        Q_UNUSED(node);
        Q_UNUSED(use);
    }

private:
    QString m_contents;
    bool m_debug;
    KDevelop::IndexedString m_currentDocument;
    KDevelop::CursorInRevision m_offset;
    KDevPG::MemoryPool* m_pool;
    KDevPG::TokenStream* m_tokenStream;
    QList<KDevelop::ProblemPointer> m_problems;
};

}

#endif

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on; auto-insert-doxygen on
