/*
Copyright (C) 2010  Ruan Strydom <ruan@jcell.co.za>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PHPLEXER_H
#define PHPLEXER_H

#include <QtCore/QStack>
#include <QtCore/QString>

#include "parserexport.h"
#include <kdev-pg-token-stream.h>
#include <qtextstream.h>

class QString;

namespace KDevPG {
class TokenStream;
}

namespace Ml {
class Token : public KDevPG::Token {
};

class TokenStream : public KDevPG::TokenStreamBase<Token> {

};

/** Markup language lexer.
 *  Lexes XML like languages and DTD.
 */
class KDEVMLPARSER_EXPORT MlLexer {
public:
    enum State {
        None, //Switch on first occurance of <
        Quote, //all "'
        Tag, //all tags opening and closing, etc
        DTD, //ended by >
        CDATA, //ended by ]]>
        Processing, //ended by ?>
        PHP, //ended by ?>
        Comment,
        Source, //ended by %>
        Script, //ended by </script>
        Style //ended by </style>
    };

    MlLexer(TokenStream *tokenStream, const QString& contents, State initialState = None );

    virtual ~MlLexer();

    int nextTokenKind();

    qint64 tokenBegin() const;

    qint64 tokenEnd() const;

public:
    /**  A pattern similar to the expression .*blah 
     * (but the first occurrence blah; not the last)
     * @return The character after condition.
     */
    QChar * readUntill(QChar *start, const QChar *end, const QString& condition , bool ignoreWhite = false, QChar** conditionStart = 0) const;

    /**  A pattern similar to the expression [^blah]*
    * @return The first occurrence of condition.
    */
    QChar * readUntillAny(QChar *start, const QChar *end, const QString& condition, bool ignoreWhite = false) const;

    /** A pattern similar to the expression [blah]*
    * @return The character after condition.
    */
    QChar * readWhileAny(QChar *start, const QChar *end, const QString& condition, bool ignoreWhite = false, QChar** conditionStart = 0) const;

    QString removeWhites(QString str) const;

    QString elementName(const QString& token) const;

    //QChar * readAndValidateUntill(QChar *start, const QChar *end, const QString& condition, bool ignoreWhite = false, QChar** conditionStart = 0) const;
private:
    static const int FlagHtmlMode = 1;
    QString m_content;
    qint64 m_curpos;
    qint64 m_tokenBegin;
    qint64 m_tokenEnd;
    QStack<State> m_states;
    QChar *m_stateStart;
    QString m_quoteSeperator;
    int m_flags;
};

}

#endif

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on; auto-insert-doxygen on
