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

#include "sgmltokenizer.h"

#include "sgmlparser.h"

#include "idtdhelper.h"

#include "dtdtokenizer.h"

namespace Xml {

SgmlTokenizer::SgmlTokenizer(TokenStream* tokenStream, const QString& contents, State initialState) : Tokenizer(tokenStream, contents) {
    if (initialState != None)
        m_states.push(TokenizerState(initialState, m_contentData));
    m_dtdTokenizer = NULL;
}

SgmlTokenizer::~SgmlTokenizer() {
    if (m_dtdTokenizer)
        delete m_dtdTokenizer;
}

/* NOTE
 * Although it looks messy it has a structure.
 * The structure is: (the * means 0 or more, as usual)
 *
 *  states*             -if stack not empty then top
 *      escapes*        -ie <!, <![, etc: push/pop the stack and return token
 *      identifiers*    -ie doctype, public etc: return token
 *      text            -return token
 *  escapes*            -ie <!, <![, etc: push/pop the stack and return token
 *  identifiers*        -none for now
 *  text                -return token
 *
 * 1) The lexer needs to keep track of things like script, style, cdata, dtd etc. I assumed if it does not
 * funny things could happen in the parser if not handled correctly. Where would the appropriate place be to handle
 * these things? Lexer or parser?
 * 2) To simplify the parser the lexer treats things like (<!)-- (<!)doctype (<!)[CDATA separately. Correct?
 * 3) The lexer keeps a stack of states. Tokens have different meanings depending on the state. When things
 * go wrong the stack can be a problem or a friend. For now if the lexer comes across something funny it
 * clears the stack. Another option is just pop-ing once?
 */
int SgmlTokenizer::nextTokenKind() {
#define PUSH_STATE(SS, BB) m_states.push(TokenizerState(SS,BB));

#define POP_STATE if(m_states.size() > 0) m_states.pop();

#define READ_WHILE_ANY(CND, FF) if(cursor >= end) return  Parser::Token_EOF;\
    condStart = cursor;\
    condEnd = readWhileAny(cursor, end, CND, FF, &condStart);\
    condLength = condEnd - condStart;\
    readLength = condEnd - cursor;

#define READ_UNTIL(CND, FF)  if(cursor >= end) return  Parser::Token_EOF;\
    condStart = cursor;\
    condEnd = readUntill(cursor, end, CND, FF, &condStart);\
    condLength = condEnd - condStart;\
    readLength = condEnd - cursor;

#define READ_UNTIL_ANY(CND, FF) if(cursor >= end) return  Parser::Token_EOF;\
    condEnd = readUntillAny(cursor, end, CND, FF);\
    condStart = condEnd;\
    condLength = condEnd - condStart;\
    readLength = condEnd - cursor;

//NOTE it looks like PG-QT needs the -1
//Sets:
//     the tokens begin and end positions
//     the current token
//     increases the position in the stream for the next call to nextTokenKind
//     adds the token to the list of tokens for the current state, we dont store whites
#define DEFAULT_RETURN(TT, LL)\
    m_curpos+=(LL);\
    m_tokenEnd = m_curpos-1;\
    if (!m_states.empty() && TT != Parser::Token_WHITE) { m_states.top().tokens.push(TT); }\
    return (TT);

#define DEFAULT_CLOSE(TT, CND, LL)\
    READ_UNTIL(CND, 0);\
    if(cursor == condStart) {\
        POP_STATE\
        DEFAULT_RETURN(TT, LL)\
    }\
    int l = condStart - cursor;\
    DEFAULT_RETURN(Parser::Token_TEXT, l)

    const QChar *start = m_contentData;
    const QChar *end = m_contentData + m_contentLength;

    const QChar *condStart = 0, *condEnd = 0;
    int condLength = 0, readLength = 0;

    const QChar *cursor = m_contentData + m_curpos;

    m_tokenBegin = m_curpos;

    if (m_curpos >= m_contentLength)
        return Parser::Token_EOF;

    if (m_curpos < 0) {
        //This happens when the lexer expected tokens that is not in the content.
        kDebug() << "Cursor position negative!" << endl;
        m_curpos = m_contentLength-1;
        m_tokenEnd = m_contentLength-1;
        return Parser::Token_EOF;
    }

    if (!m_states.empty()) {
        switch (m_states.top().state) {
        case(Quote): {
            READ_UNTIL_ANY(m_tokenEndString, IgnoreNone);
            if(cursor == condStart) {
                POP_STATE
                DEFAULT_RETURN(Parser::Token_QUOTE, 1)
            }
            DEFAULT_RETURN(Parser::Token_TEXT, readLength)
        }
            //A special case for cdata elements like script style
        case(Cdata): {
            READ_UNTIL(m_tokenEndString, IgnoreWhites | IgnoreCase)
            POP_STATE
            int l = readLength - condLength;
            DEFAULT_RETURN(Parser::Token_TEXT, l)
        }

        case(Tag): {
            switch (cursor->unicode()) {
            case('='): {
                DEFAULT_RETURN(Parser::Token_EQUAL, 1)
            }
            case(':'): {
                DEFAULT_RETURN(Parser::Token_COLON, 1)
            }
            case('"'): {
                cursor++;
                m_tokenBegin+=1;
                READ_UNTIL_ANY("\"", IgnoreNone)
                m_curpos+=readLength+2;
                m_tokenEnd = m_curpos-2;
                if (!m_states.empty()) {
                    m_states.top().tokens.push(Parser::Token_TEXT);
                }
                return Parser::Token_TEXT;
            }
            case('\''): {
                cursor++;
                m_tokenBegin+=1;
                READ_UNTIL_ANY("'", IgnoreNone)
                m_curpos+=readLength+2;
                m_tokenEnd = m_curpos-2;
                if (!m_states.empty()) {
                    m_states.top().tokens.push(Parser::Token_TEXT);
                }
                return Parser::Token_TEXT;
            }
            case(' '): {
                DEFAULT_RETURN(Parser::Token_WHITE, 1)
            }
            case('\r'): {
                DEFAULT_RETURN(Parser::Token_WHITE, 1)
            }
            case('\n'): {
                createNewline(m_tokenBegin);
                DEFAULT_RETURN(Parser::Token_WHITE, 1)
            }
            case('\t'): {
                DEFAULT_RETURN(Parser::Token_WHITE, 1)
            }
            case('/'): {
                if (cursor < end) {
                    if ((cursor+1)->unicode() == '>') {
                        POP_STATE
                        DEFAULT_RETURN(Parser::Token_CLOSE, 2)
                    }
                }
                DEFAULT_RETURN(Parser::Token_TEXT, 1)
            }
            case('<'): {
                QStringRef seq(&m_content, m_states.top().begin - start, cursor - start + readLength);
                kDebug() << "Bad escape sequence! " << seq.toString();
                DEFAULT_RETURN(Parser::Token_EOF, 1)
            }
            case('>'): {
                //SGML hacks
                if (m_dtdHelper
                        && m_states.top().begin
                        && (m_states.top().begin + 1)->unicode() != '/') {
                    //QStringRef ref(&m_content, m_states.top().begin - start, cursor - m_states.top().begin + 1);
                    QString str(m_states.top().begin, cursor - m_states.top().begin + 1);
                    QString name = elementName(str);
                    if (m_dtdHelper->cdataElement(name)) {
                        m_tokenEndString = QString("</%1>").arg(name);
                        POP_STATE //Have to pop before push else it has no effect
                        PUSH_STATE(Cdata, cursor + 1);
                        DEFAULT_RETURN(Parser::Token_GT, 1)
                    } else if (m_dtdHelper->emptyElement(name)) {
                        POP_STATE
                        DEFAULT_RETURN(Parser::Token_CLOSE, 1)
                    }
                }
                POP_STATE
                DEFAULT_RETURN(Parser::Token_GT, 1)

            }

            default: {
            } break;
            }
            READ_UNTIL_ANY(" \r\n\t=:\"'<>/", IgnoreNone)
            DEFAULT_RETURN(Parser::Token_TEXT, readLength)
        }

        case(DTD): {
            if (!m_dtdTokenizer) {
                kDebug() << "No DTD tokenizer defined";
                POP_STATE
                return Parser::Token_EOF;
            }
            int token = m_dtdTokenizer->nextTokenKind();
            m_tokenBegin = m_states.top().begin - m_contentData + m_dtdTokenizer->tokenBegin();
            m_tokenEnd = m_states.top().begin - m_contentData + m_dtdTokenizer->tokenEnd();
            m_curpos = m_tokenEnd+1;
            if(m_dtdTokenizer->currentState() == 0 || token == Parser::Token_EOF) {
                QString str(m_states.top().begin, m_contentData + m_curpos - m_states.top().begin + 1);
                QString name, publicId, systemId;
                doctype(str, name, publicId, systemId);
                const IDtdHelper * helper = IDtdHelper::instance(publicId, systemId);
                if (!helper)
                    helper = IDtdHelper::instanceForName(name);
                if (!helper) {
                    kDebug() << "Failed to get a DTD instance for DOCTYPE:" << name
                    << "PublicId:" << publicId
                    << "SystemId:" << systemId;
                }
                if(helper)
                    setDtdHelper(helper);
                POP_STATE
                return token;
            }
            return token;
        }
        }
    }

    //Whats next ...

    if (cursor->unicode() == '<') {
        READ_WHILE_ANY("<!-[]%?/>", IgnoreNone);
        QStringRef ref(&m_content, cursor - start, readLength);
        QString str = ref.toString();

        if (str.startsWith("<!--")) {
            READ_UNTIL("-->", IgnoreNone);
            DEFAULT_RETURN(Parser::Token_COMMENT, readLength)
        }

        if (str.startsWith("<![")) {
            READ_WHILE_ANY("<![PCDATApcdata[", IgnoreNone);
            QStringRef ref(&m_content, condStart - start, condLength);
            QString str = ref.toString().toUpper();
            if (str == "<![CDATA[") {
                READ_UNTIL("]]>", IgnoreNone);
                DEFAULT_RETURN(Parser::Token_CDATA, readLength)
            }

            if (str == "<![PCDATA[") {
                READ_UNTIL("]]>", IgnoreNone);
                DEFAULT_RETURN(Parser::Token_PCDATA, readLength)
            }

            //TODO what now?
            kDebug() << "Unknown content type: " << str;
            DEFAULT_RETURN(Parser::Token_EOF, readLength)
        }

        if (str.startsWith("<?")) {
            READ_WHILE_ANY("<?phpPHPxmlXML=", IgnoreNone);
            QStringRef ref(&m_content, condStart - start, condLength);
            QString str = removeWhites(ref.toString().toLower());
            if (str.endsWith('?') && str.length() > 2)str.remove(str.size()-1, 1); //ie <?xml?> or <??>
            if (str == "<?php") {
                READ_UNTIL("?>", IgnoreNone);
                DEFAULT_RETURN(Parser::Token_PHP, readLength)
            }
            if (str == "<?xml") {
                READ_UNTIL("?>", IgnoreNone);
                DEFAULT_RETURN(Parser::Token_PROC, readLength)
            }
            if (str == "<?=") {
                READ_UNTIL("?>", IgnoreNone);
                DEFAULT_RETURN(Parser::Token_PHP, readLength)
            }

            //TODO what now?
            kDebug() << "Unknown processing instruction: " << str;
            DEFAULT_RETURN(Parser::Token_EOF, readLength)
        }

        if (str.startsWith("<%")) {
            READ_UNTIL("%>", IgnoreNone);
            DEFAULT_RETURN(Parser::Token_SRC, 2)
        }

        if (str.startsWith("</")) {
            PUSH_STATE(Tag, cursor)
            DEFAULT_RETURN(Parser::Token_CLOSE, 2)
        }

        if (str.startsWith("<!")) {
            READ_UNTIL_ANY(" \r\n\t>(]%", IgnoreNone);
            QStringRef ref(&m_content, cursor - start, readLength);
            QString str = ref.toString().toUpper();
            if (str == "<!DOCTYPE") {
                PUSH_STATE(DTD, cursor)
                if (m_dtdTokenizer)
                    delete m_dtdTokenizer;
                m_dtdTokenizer = new DTDTokenizer(m_tokenStream, cursor, m_contentLength- (cursor - m_contentData));
                return nextTokenKind();
            }
            kDebug() << "Unknown escape: " << ref.toString();
            DEFAULT_RETURN(Parser::Token_EOF, readLength)
        }

        if (str.startsWith("<") && readLength == 1) {
            PUSH_STATE(Tag, cursor)
            DEFAULT_RETURN(Parser::Token_LT, 1)
        }

        DEFAULT_RETURN(Parser::Token_LT, 1)
    }

    if (cursor->unicode() == '>') {
        DEFAULT_RETURN(Parser::Token_GT, 1)
    }

    READ_UNTIL_ANY("<", IgnoreNone);
    QStringRef ref(&m_content, cursor - start, readLength);
    if (ref.toString().trimmed().length() == 0) {
        //if(readLength == 0) {
        //    DEFAULT_RETURN(Parser::Token_EOF, readLength)
        //}
        m_curpos += readLength;
        return nextTokenKind();
    }
    DEFAULT_RETURN(Parser::Token_TEXT, readLength)
}

}

