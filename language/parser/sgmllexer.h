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

namespace Xml {

class Token : public KDevPG::Token {};

class TokenStream : public KDevPG::TokenStreamBase<Token> {};

class IDtdHelper;

/** Markup language lexer.
 *  Tokenizes SGML/XML like languages. Because of SGML's dependency on DTD, SGML must
 *  specify a doctype, when a doctype is found, a facade IDtdHelper is used to help tokenize SGML.
 *  Until a - or if no doctype is found the source is treated as plain XML.<br/>
 *  Here is an example of the complexity of SGML:<br/>
 *  <pre>
 *  <![CDATA[
 *  <!doctype root>
 *  <root>
 *      <child1> I am a child where do I belong, where do I end? I also have mixed content.
 *          <child1.1>I am a <child> with cdata content.</child1.1>
 *      <child2>
 *  ]]>
 *  <pre><br/>
 *  It is clear that there is absolutely no way to determine where an element starts and end without DTD.
 *  It is also not possible to parse 'child1.1' without knowing that its content is cdata.<br/>
 *  This will correct CDATA and empty elements, the parser has to correct optional close elements.
 *  The parser may use the helper found by this lexer.
 *  @see IDtdHelper
 *  @todo Implement a DTD parser
 */
class KDEVXMLPARSER_EXPORT SgmlLexer {
public:
    enum State {
        None, //Switch on first occurance of <
        Quote, //untill m_tokenEndString
        Tag, //all tags opening and closing, etc
        DTD, //ended by >
        Processing, //ended by ?>
        PHP, //ended by ?>
        Source, //ended by %>
        Cdata //untill m_tokenEndString
    };

    SgmlLexer(TokenStream *tokenStream, const QString& contents, State initialState = None );

    virtual ~SgmlLexer();

    int nextTokenKind();

    qint64 tokenBegin() const;

    qint64 tokenEnd() const;

public:

    enum ReadFlag {IgnoreWhites = 1, IgnoreCase = 2};
    
    /** A pattern similar to the expression .*blah
     *  (but the first occurrence blah; not the last)
     *  @return The character after condition.
     */
    QChar * readUntill(QChar *start, const QChar *end, const QString& condition , int flag = 0, QChar** conditionStart = 0) const;

    /** A pattern similar to the expression [^blah]*
     *  @return The first occurrence of condition.
     */
    QChar * readUntillAny(QChar *start, const QChar *end, const QString& condition, int flag = 0) const;

    /** A pattern similar to the expression [blah]*
     *  @return The character after condition.
     */
    QChar * readWhileAny(QChar *start, const QChar *end, const QString& condition, int flag = 0, QChar** conditionStart = 0) const;

    QString removeWhites(QString str) const;

    QString elementName(const QString& token) const;
    
    void doctype(const QString& token, QString &name, QString &publicId, QString &systemId) const;
    
    /** If the source has a doctype this will return the helper found/used.
     * @return Null if none or no supported doctype found.
     */
    const IDtdHelper * dtdHelper() {return m_dtdHelper;}
private:

    struct LexerState {
        LexerState() : state(None), begin(0) {}
        LexerState(State s, const QChar * b) : state(s), begin(b) {}
        State state;
        const QChar *begin;
    };

    int tagNextTokenKind();

    /** The content to lex. */
    QString m_content;

    /** The current position in the content. */
    qint64 m_curpos;
    
    /** The start of the current token. */
    qint64 m_tokenBegin;
    
    /** The end of the current token. */
    qint64 m_tokenEnd;

    /** The current token */
    int m_token;
    
    /** The state the lexer is in. */
    QStack<LexerState> m_states;

    /** ie "\"" or "&lt;/script&gt;" */
    QString m_tokenEndString;

    /** m_content start position **/
    const QChar *m_start;

    /** m_content end position **/
    const QChar *m_end;

    const IDtdHelper *m_dtdHelper;
};

}

#endif

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on; auto-insert-doxygen on
