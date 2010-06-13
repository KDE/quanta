/*****************************************************************************
 * Copyright (c) 2010 Ruan Strydom <rm3dom@gmail.com>                        *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 2 of the License, or         *
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

#include "dtdtokenizer.h"

#include "sgmlparser.h"

using namespace Xml;

struct DTDTokenizer::TokenizePart {
    TokenizePart() : start(0), end(0), curpos(0) {}
    TokenizePart(const QChar *s, const QChar *e, qint64 p) : start(s), end(e), curpos(p) {}
    const QChar *start;
    const QChar *end;
    qint64 curpos;
};

DTDTokenizer::DTDTokenizer(TokenStream* tokenStream, const QString& contents, DTDTokenizer::State initialState): Tokenizer(tokenStream, contents)
{
    init(initialState);
}

DTDTokenizer::DTDTokenizer(TokenStream* tokenStream, const QChar* content, qint64 contentLength, DTDTokenizer::State initialState): Tokenizer(tokenStream, content, contentLength)
{
    init(initialState);
}

void DTDTokenizer::init(DTDTokenizer::State initialState)
{
    m_entityContentStart = 0;
    m_entityContentEnd = 0;
    m_externEntity = false;
    if (initialState != None)
        m_states.push(TokenizerState(initialState, m_contentData));
    m_parts.push(TokenizePart(m_contentData, m_contentData + m_contentLength, 0));
}

DTDTokenizer::~DTDTokenizer() {}

/*
 * This uses three stacks:
 * <ul>
 * <li>states: to keep track of the current state.</li>
 * <li>tokens: a stack of tokens for the current state, excluding whites.</li>
 * <li>parts: a stack of string parts to tokenize, to manage ENTITIES.</li>
 * </ul>
 */
int DTDTokenizer::nextTokenKind()
{
#define PUSH_STATE(SS, BB) m_states.push(TokenizerState(SS,BB));

#define POP_STATE if(m_states.size() > 0) m_states.pop();

#define READ_WHILE_ANY(CND, FF)  if(cursor >= end) return  Parser::Token_EOF;\
    condStart = cursor;\
    condEnd = readWhileAny(cursor, end, CND, FF, &condStart);\
    condLength = condEnd - condStart;\
    readLength = condEnd - cursor;

#define READ_UNTIL(CND, FF)  if(cursor >= end) return  Parser::Token_EOF;\
    condStart = cursor;\
    condEnd = readUntill(cursor, end, CND, FF, &condStart);\
    condLength = condEnd - condStart;\
    readLength = condEnd - cursor;

#define READ_UNTIL_ANY(CND, FF)  if(cursor >= end) return  Parser::Token_EOF;\
    condEnd = readUntillAny(cursor, end, CND, FF);\
    condStart = condEnd;\
    condLength = condEnd - condStart;\
    readLength = condEnd - cursor;

#define DEFAULT_RETURN(TT, LL) \
    m_parts.top().curpos+=(LL);\
    m_tokenEnd = m_parts.top().start - m_contentData + m_parts.top().curpos - 1;\
    if (!m_states.empty() && TT != Parser::Token_WHITE) { m_states.top().tokens.push(TT); }\
    if (LL < 1) {\
        kDebug() << "Read length == 0" << "Token:" << TT << "Location" << m_tokenBegin;\
        return Parser::Token_EOF;\
    }\
    return (TT);

#define DEFAULT_CLOSE(TT, CND, LL)\
    READ_UNTIL(CND, 0);\
    if(cursor == condStart) {\
        POP_STATE\
        DEFAULT_RETURN(TT, LL)\
    }\
    int l = condStart - cursor;\
    DEFAULT_RETURN(Parser::Token_TEXT, l)

    if (!m_parts.isEmpty()) {
        while (!m_parts.isEmpty() && m_parts.top().curpos >= m_parts.top().end - m_parts.top().start) {
            m_parts.pop();
        }
    }

    if (m_parts.isEmpty()) {
        kDebug() << "No more content to parse";
        m_curpos = m_contentLength-1;
        m_tokenEnd = m_contentLength-1;
        return Parser::Token_EOF;
    }

    m_processEndline = m_parts.size() == 1;

    const QChar *start = m_parts.top().start;
    const QChar *end = m_parts.top().end;

    const QChar *condStart = 0, *condEnd = 0;
    int condLength = 0, readLength = 0;

    const QChar *cursor = m_parts.top().start + m_parts.top().curpos;

    m_tokenBegin = m_parts.top().start - m_contentData + m_parts.top().curpos;

    if (m_parts.top().curpos < 0) {
        //This happens when the lexer expected tokens that is not in the content.
        kDebug() << "Cursor position negative!" << endl;
        m_curpos = m_contentLength-1;
        m_tokenEnd = m_contentLength-1;
        return Parser::Token_ERROR;
    }

    if (!m_states.empty()) {
        switch (m_states.top().state) {
        case(Quote): {
            READ_UNTIL_ANY(m_tokenEndString, IgnoreNone);
            if (cursor == condStart) {
                POP_STATE
                DEFAULT_RETURN(Parser::Token_QUOTE, 1)
            }
            if (m_states.size() > 1 && m_states[m_states.size()-2].state == ENTITY) {
                m_entityContentStart = cursor;
                m_entityContentEnd = condEnd-1;
            }
            DEFAULT_RETURN(Parser::Token_TEXT, readLength)
        }
        case(SGML): {
            READ_UNTIL("<!doctype", IgnoreCase)
            m_parts.top().curpos = condStart - start;
            POP_STATE
            return nextTokenKind();
        }
        }
    }

    switch (cursor->unicode()) {
    case('('): {
        DEFAULT_RETURN(Parser::Token_OPAREN, 1)
    }
    case(')'): {
        DEFAULT_RETURN(Parser::Token_CPAREN, 1)
    }
    case('['): {
        DEFAULT_RETURN(Parser::Token_OBRACKET, 1)
    }
    case(']'): {
        if (!m_states.empty() && m_states.top().state == CONDITION && (cursor+1)->unicode() == '>') {
            POP_STATE
            DEFAULT_RETURN(Parser::Token_COND_END, 2)
        }
        DEFAULT_RETURN(Parser::Token_CBRACKET, 1)
    }
    case('|'): {
        DEFAULT_RETURN(Parser::Token_VBAR, 1)
    }
    case('+'): {
        DEFAULT_RETURN(Parser::Token_PLUS, 1)
    }
    case('O'): {
        if ((cursor+1)->isSpace() || *(cursor+1) == '"' || *(cursor+1) == '\'') {
            DEFAULT_RETURN(Parser::Token_OPT, 1)
        }
        break;
    }
    case('*'): {
        DEFAULT_RETURN(Parser::Token_ASTERISK, 1)
    }
    case('#'): {
        DEFAULT_RETURN(Parser::Token_NUMBER, 1)
    }
    case(','): {
        DEFAULT_RETURN(Parser::Token_COMMA, 1)
    }
    case(';'): {
        DEFAULT_RETURN(Parser::Token_SEMICOLON, 1)
    }
    case('&'): {
        DEFAULT_RETURN(Parser::Token_AMP, 1)
    }
    case('%'): {
        if ((cursor+1)->isSpace() || *(cursor+1) == '"' || *(cursor+1) == '\'') {
            DEFAULT_RETURN(Parser::Token_PERCENT, 1)
        }

        //NOTE Found in loose.dtd the following: <!ATTLIST TITLE %i18n>
        cursor+=1;
        READ_UNTIL_ANY(" \r\n\t|[]>()<\"'?*+,>%", IgnoreNone);
        QString str(cursor, readLength);
        QString entity = str.mid(0,readLength);
        if (str.endsWith(';'))
            entity = str.mid(0,readLength-1);
        if (!m_entities.contains(entity)) {
            kDebug() << "No entity for name:" << entity;
            DEFAULT_RETURN(Parser::Token_PERCENT, 1)
        }
        if(m_entities.value(entity).end - m_entities.value(entity).start <= 1) {
            kDebug() << "Empty entity:" << entity;
            m_parts.top().curpos += readLength+1;
            return nextTokenKind();
        }

        m_parts.top().curpos += readLength+1;
        m_parts.push(m_entities.value(entity));
        return nextTokenKind();
    }
    case('?'): {
        DEFAULT_RETURN(Parser::Token_QUESTION, 1)
    }
    case('"'): {
        m_tokenEndString = "\"";
        PUSH_STATE(Quote, cursor)
        DEFAULT_RETURN(Parser::Token_QUOTE, 1)
    }
    case('\''): {
        m_tokenEndString = "\'";
        PUSH_STATE(Quote, cursor)
        DEFAULT_RETURN(Parser::Token_QUOTE, 1)
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
    case('>'): {
        if (!m_states.isEmpty() ) {
            if (m_states.top().state == ENTITY) {
                if (!m_externEntity && !m_entityName.isEmpty()) {
                    m_entities.insert(m_entityName, TokenizePart(m_entityContentStart, m_entityContentEnd + 1, 0));
                }
            }
        }
        POP_STATE
        DEFAULT_RETURN(Parser::Token_GT, 1)
    }
    case('-'): {
        //Skip comments in DTD
        if ((cursor+1)->unicode() == '-') {
            cursor+=2;
            READ_UNTIL("--", IgnoreNone)
            m_parts.top().curpos += readLength+2;
            return nextTokenKind();
        }
        if ((cursor+1)->isSpace() || (cursor+1)->unicode() == '(') {
            DEFAULT_RETURN(Parser::Token_HYPHEN, 1)
        }
        break;
    }

    case('<'): {
        if ((cursor+1)->unicode() == '!') {
            if ((cursor+2)->unicode() == '[') {
                PUSH_STATE(CONDITION, cursor)
                DEFAULT_RETURN(Parser::Token_COND_START, 3);
            }
            //Skip comments in DTD
            if ((cursor+2)->unicode() == '-' && (cursor+3)->unicode() == '-') {
                READ_UNTIL("-->", IgnoreNone)
                m_parts.top().curpos += readLength;
                return nextTokenKind();
            }

            READ_UNTIL_ANY(" \r\n\t", IgnoreNone);
            QString str = QString(cursor, readLength).toUpper();
            if (str == "<!DOCTYPE") {
                PUSH_STATE(DOCTYPE, cursor)
                DEFAULT_RETURN(Parser::Token_DOCTYPE, readLength);
            }
            if (str == "<!ENTITY") {
                m_entityContentStart = 0;
                m_entityContentEnd = 0;
                m_entityName = "";
                m_externEntity = false;
                PUSH_STATE(ENTITY, cursor)
                DEFAULT_RETURN(Parser::Token_ENTITY, readLength);
            }
            if (str == "<!ATTLIST") {
                PUSH_STATE(ATTLIST, cursor)
                DEFAULT_RETURN(Parser::Token_ATTLIST, readLength);
            }
            if (str == "<!NOTATION") {
                PUSH_STATE(NOTATION, cursor)
                DEFAULT_RETURN(Parser::Token_NOTATION, readLength);
            }
            if (str == "<!ELEMENT") {
                PUSH_STATE(ELEMENT, cursor)
                DEFAULT_RETURN(Parser::Token_ELEMENT, readLength);
            }
            kDebug() << "Unknown escape:" << str;
            DEFAULT_RETURN(Parser::Token_ERROR, readLength)
        }
        //Skip processing in DTD
        if ((cursor+1)->unicode() == '?') {
                READ_UNTIL("?>", IgnoreNone)
                m_parts.top().curpos += readLength;
                return nextTokenKind();
        }
        break;
    }
    default: {
    } break;
    }

    READ_UNTIL_ANY(" \r\n\t|[]>()<\"'?*+,;%#\\", IgnoreNone);
    if (m_states.size() > 0) {
        if (m_states.top().state == DOCTYPE || m_states.top().state == ENTITY || m_states.top().state == NOTATION) {
            if (m_states.top().tokens.contains(Parser::Token_TEXT)) {
                QString str = QString(cursor, readLength).toUpper();
                if (str == "PUBLIC") {
                    if (m_states.top().state == ENTITY) m_externEntity=true;
                        DEFAULT_RETURN(Parser::Token_PUBLIC, readLength)
                }
                if (str == "SYSTEM") {
                    if (m_states.top().state == ENTITY) m_externEntity=true;
                    DEFAULT_RETURN(Parser::Token_SYSTEM, readLength)
                }
            }
            if (m_states.top().tokens.top() == Parser::Token_PERCENT
                    || m_states.top().tokens.top() == Parser::Token_ENTITY) {
                QString str = QString(cursor, readLength);
                m_entityName = str;
            }
        }
    }
    DEFAULT_RETURN(Parser::Token_TEXT, readLength)
}


