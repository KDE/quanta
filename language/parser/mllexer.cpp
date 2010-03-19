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

#include "mllexer.h"

#include "mlparser.h"

#include <QMetaObject>

namespace Ml {

MlLexer::MlLexer(Ml::TokenStream* tokenStream, const QString& contents, State initialState) {
    m_content = contents;
    m_tokenEnd = 0;
    m_tokenBegin = 0;
    m_curpos = 0;
    m_stateStart = 0;
    m_flags = FlagHtmlMode;
    if (initialState != None)
        m_states.push(initialState);
}

MlLexer::~MlLexer() {

}

qint64 MlLexer::tokenBegin() const {
    return m_tokenBegin;
}

qint64 MlLexer::tokenEnd() const {
    return m_tokenEnd;
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
int MlLexer::nextTokenKind() {
#define PUSH_STATE(SS) m_states.push(SS); m_stateStart = cursor;

#define POP_STATE if(m_states.size() > 0) m_states.pop();

#define READ_WHILE_ANY(CND, WW) condStart = cursor;\
    condEnd = readWhileAny(cursor, end, CND, WW, &condStart);\
    condLength = condEnd - condStart;\
    readLength = condEnd - cursor;\
    if(readLength == 0) return  Parser::Token_EOF;

#define READ_UNTIL(CND, WW) condStart = cursor;\
    condEnd = readUntill(cursor, end, CND, WW, &condStart);\
    condLength = condEnd - condStart;\
    readLength = condEnd - cursor;\
    if(readLength == 0) return  Parser::Token_EOF;

#define READ_UNTIL_ANY(CND, WW) condEnd = readUntillAny(cursor, end, CND, WW);\
    condStart = condEnd;\
    condLength = condEnd - condStart;\
    readLength = condEnd - cursor;\
    if(readLength == 0) return  Parser::Token_EOF;

//NOTE it looks like PG-QT needs the -1
#define DEFAULT_RETURN(TT, LL) m_curpos+=(LL); m_tokenEnd = m_curpos-1; return (TT);

#define DEFAULT_CLOSE(TT, CND, LL)\
    READ_UNTIL(CND, false);\
    if(cursor == condStart) {\
        POP_STATE\
        DEFAULT_RETURN(TT, LL)\
    }\
    int l = condStart - cursor;\
    DEFAULT_RETURN(Parser::Token_TEXT, l)

    const QChar *start = m_content.data();
    const QChar *end = m_content.data() + m_content.length();

    QChar *condStart = 0;
    QChar *condEnd = 0;
    int condLength = 0;
    int readLength = 0;

    QChar *cursor = m_content.data() + m_curpos;

    m_tokenBegin = m_curpos;

    if (m_curpos >= m_content.size())
        return Parser::Token_EOF;

    if (!m_states.empty()) {
        switch (m_states.top()) {

        case(Quote): {
            DEFAULT_CLOSE(Parser::Token_QUOTE, m_quoteSeperator, 1)
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
                m_states.push(Quote);
                m_quoteSeperator='"';
                DEFAULT_RETURN(Parser::Token_QUOTE, 1)
            }
            case('\''): {
                m_states.push(Quote);
                m_quoteSeperator='\'';
                DEFAULT_RETURN(Parser::Token_QUOTE, 1)
            }
            case(' '): {
                DEFAULT_RETURN(Parser::Token_WHITE, 1)
            }
            case('\r'): {
                DEFAULT_RETURN(Parser::Token_WHITE, 1)
            }
            case('\n'): {
                DEFAULT_RETURN(Parser::Token_WHITE, 1)
            }
            case('\t'): {
                DEFAULT_RETURN(Parser::Token_WHITE, 1)
            }
            case('/'): {
                if (cursor < end) {
                    if ((cursor+1)->unicode() == '>') {
                        POP_STATE
                        DEFAULT_RETURN(Parser::Token_FSGT, 2)
                    }
                }
                //TODO this is an error
                DEFAULT_RETURN(Parser::Token_TEXT, 1)
            }
            case('>'): {
                POP_STATE

                //HTML hacks
                if (m_flags & FlagHtmlMode) {
                    if (m_stateStart && (m_stateStart + 1)->unicode() != '/') {
                        QStringRef ref(&m_content, m_stateStart - start, cursor - m_stateStart + 1);
                        QString name = elementName(ref.toString());
                        if (name == "script") {
                            PUSH_STATE(Script);
                        } else if (name == "style") {
                            PUSH_STATE(Style);
                        }
                    }
                }

                DEFAULT_RETURN(Parser::Token_GT, 1)
            }

            case('<'): {
                //TODO serious problem
                //We could pop all states and start from scratch, or just pop the top.
                QStringRef seq(&m_content, m_stateStart - start, cursor - start + readLength);
                kDebug() << "Bad escape sequence! CLEARING STACK: " << seq.toString();
                m_states.clear();
                DEFAULT_RETURN(Parser::Token_ERROR, 1)
            }

            default: {
            } break;
            }
            READ_UNTIL_ANY(" \r\n\t=:\"'>/", false)
            DEFAULT_RETURN(Parser::Token_TEXT, readLength)
        }

        case(DTD): {
            switch (cursor->unicode()) {
            case('*'): {
                DEFAULT_RETURN(Parser::Token_ASTERISK, 1)
            }
            case('+'): {
                DEFAULT_RETURN(Parser::Token_PLUS, 1)
            }
            case('?'): {
                DEFAULT_RETURN(Parser::Token_QUESTION, 1)
            }
            case('['): {
                DEFAULT_RETURN(Parser::Token_OBRACKET, 1)
            }
            case(']'): {
                DEFAULT_RETURN(Parser::Token_CBRACKET, 1)
            }
            case('('): {
                DEFAULT_RETURN(Parser::Token_OPAREN, 1)
            }
            case(')'): {
                DEFAULT_RETURN(Parser::Token_CPAREN, 1)
            }
            case('|'): {
                DEFAULT_RETURN(Parser::Token_VBAR, 1)
            }
            case('#'): {
                DEFAULT_RETURN(Parser::Token_NUMBER, 1)
            }
            case(';'): {
                DEFAULT_RETURN(Parser::Token_SEMICOLON, 1)
            }
            case(','): {
                DEFAULT_RETURN(Parser::Token_COMMA, 1)
            }
            case('%'): {
                DEFAULT_RETURN(Parser::Token_PERCENT, 1)
            }
            case('"'): {
                m_states.push(Quote);
                m_quoteSeperator='"';
                DEFAULT_RETURN(Parser::Token_QUOTE, 1)
            }
            case('\''): {
                DEFAULT_RETURN(Parser::Token_SQUOTE, 1)
            }
            case(' '): {
                DEFAULT_RETURN(Parser::Token_WHITE, 1)
            }
            case('\r'): {
                DEFAULT_RETURN(Parser::Token_WHITE, 1)
            }
            case('\n'): {
                DEFAULT_RETURN(Parser::Token_WHITE, 1)
            }
            case('\t'): {
                DEFAULT_RETURN(Parser::Token_WHITE, 1)
            }
            case('>'): {
                POP_STATE
                DEFAULT_RETURN(Parser::Token_DECLARE_END, 1)
            }
            case('<'): {
                READ_WHILE_ANY("<!--", false);
                QStringRef ref(&m_content, cursor - start, readLength);
                QString str = ref.toString();

                if (str.startsWith("<!--")) {
                    PUSH_STATE(Comment)
                    DEFAULT_RETURN(Parser::Token_COMMENT_START, 4)
                }
                if (str.startsWith("<!")) {
                    PUSH_STATE(DTD)
                    DEFAULT_RETURN(Parser::Token_DECLARE_START, 2)
                }
                //TODO serious problem
                //We could pop all states and start from scratch, or just pop the top.
                QStringRef seq(&m_content, m_stateStart - start, cursor - start + readLength);
                kDebug() << "Bad escape sequence! CLEARING STACK: " << seq.toString();
                m_states.clear();
                DEFAULT_RETURN(Parser::Token_ERROR, readLength)
            }
            default: {
            } break;
            }
            /*TODO WARNING FIXME NOTE
             * <!element cdata ... will break as this causes 'element' and 'cdata' to be reserved keywords
             * which is not true for dtd because it depends on the context it is used in
             */
            READ_UNTIL_ANY(" \r\n\t[]()<>%*+?;#|'\"",false);
            QStringRef ref(&m_content, cursor - start, readLength);
            QString str = ref.toString().toLower();
            if (str == "doctype") {
                DEFAULT_RETURN(Parser::Token_DOCTYPE, readLength)
            } else if (str == "element") {
                DEFAULT_RETURN(Parser::Token_ELEMENT, readLength)
            } else if (str == "attlist") {
                DEFAULT_RETURN(Parser::Token_ATTLIST, readLength)
            } else if (str == "entity") {
                DEFAULT_RETURN(Parser::Token_ENTITY, readLength)
            } else if (str == "public") {
                DEFAULT_RETURN(Parser::Token_PUBLIC, readLength)
            } else if (str == "system") {
                DEFAULT_RETURN(Parser::Token_SYSTEM, readLength)
            } else if (str == "cdata") {
                DEFAULT_RETURN(Parser::Token_CDATA, readLength)
            } else if (str == "pcdata") {
                DEFAULT_RETURN(Parser::Token_PCDATA, readLength)
            } else if (str == "sdata") {
                DEFAULT_RETURN(Parser::Token_SDATA, readLength)
            } else if (str == "implied") {
                DEFAULT_RETURN(Parser::Token_IMPLIED, readLength)
            } else if (str == "required") {
                DEFAULT_RETURN(Parser::Token_REQUIRED, readLength)
            } else if (str == "fixed") {
                DEFAULT_RETURN(Parser::Token_FIXED, readLength)
            } else if (str == "empty") {
                DEFAULT_RETURN(Parser::Token_EMPTY, readLength)
            } else if (str == "any") {
                DEFAULT_RETURN(Parser::Token_ANY, readLength)
            } else if (str == "nmtoken") {
                DEFAULT_RETURN(Parser::Token_NMTOKEN, readLength)
            } else if (str == "id") {
                DEFAULT_RETURN(Parser::Token_ID, readLength)
            }
            DEFAULT_RETURN(Parser::Token_TEXT, readLength)
        } break;

        case(Comment): {
            DEFAULT_CLOSE(Parser::Token_COMMENT_END, "-->", 3)
        }

        case(Processing): {
            DEFAULT_CLOSE(Parser::Token_PROC_END, "?>", 2)
        }

        case(CDATA): {
            DEFAULT_CLOSE(Parser::Token_CDATA_END, "]]>", 3)
        }

        case(PHP): {
            //TODO whatch out for "... ?> ..." in php sources
            DEFAULT_CLOSE(Parser::Token_PHP_END, "?>", 2)
        }

        case(Source): {
            //TODO whatch out for "... %> ..." in sources
            DEFAULT_CLOSE(Parser::Token_SRC_END, "%>", 2)
        }

        case(Script): {
            READ_UNTIL("</script>", true);
            if (cursor == condStart) {
                POP_STATE
                cursor = condStart; //move c to < for next tag
            } else {
                int l = condStart - cursor;
                DEFAULT_RETURN(Parser::Token_TEXT, l)
            }
        } break;

        case(Style): {
            READ_UNTIL("</style>", true);
            if (cursor == condStart) {
                POP_STATE
                cursor = condStart; //move c to < for next tag
            } else {
                int l = condStart - cursor;
                DEFAULT_RETURN(Parser::Token_TEXT, l)
            }
        } break;

        default: {
            //TODO what now?
            kDebug() << "Unknown lexer state: " << m_states.top();
            m_states.clear();
        } break;
        }
    }

    //Whats next ...

    if (cursor->unicode() == '<') {
        READ_WHILE_ANY("<!-[]%?/>", false);
        QStringRef ref(&m_content, cursor - start, readLength);
        QString str = ref.toString();

        if (str.startsWith("<!--")) {
            PUSH_STATE(Comment)
            DEFAULT_RETURN(Parser::Token_COMMENT_START, 4)
        }

        if (str.startsWith("<![")) {
            READ_WHILE_ANY("<![PCDATApcdata[", false);
            QStringRef ref(&m_content, condStart - start, condLength);
            QString str = removeWhites(ref.toString().toLower());
            int l = ref.length();
            if (str == "<![cdata[") {
                PUSH_STATE(CDATA)
                DEFAULT_RETURN(Parser::Token_CDATA_START, l)
            }
            //TODO must be parsed
            if (str == "<![pcdata[") {
                READ_UNTIL("]]>", false);
                DEFAULT_RETURN(Parser::Token_TEXT, readLength)
            }

            //TODO what now?
            DEFAULT_RETURN(Parser::Token_ERROR, readLength)
            kDebug() << "Unknown content type: " << str;
        }

        if (str.startsWith("<?")) {
            READ_WHILE_ANY("<?phpPHPxmlXML=", false);
            QStringRef ref(&m_content, condStart - start, condLength);
            QString str = removeWhites(ref.toString().toLower());
            if (str.endsWith('?') && str.length() > 2)str.remove(str.size()-1, 1); //ie <?xml?> or <??>
            int l = str.length();
            if (str == "<?php") {
                PUSH_STATE(PHP)
                DEFAULT_RETURN(Parser::Token_PHP_START, l)
            }
            if (str == "<?xml") {
                PUSH_STATE(Processing)
                DEFAULT_RETURN(Parser::Token_PROC_START, l)
            }
            if (str == "<?=") {
                PUSH_STATE(PHP)
                DEFAULT_RETURN(Parser::Token_PHP_START, l)
            }
            //TODO what now?
            DEFAULT_RETURN(Parser::Token_ERROR, readLength)
            kDebug() << "Unknown processing instruction: " << str;
        }

        if (str.startsWith("<%")) {
            PUSH_STATE(Source)
            DEFAULT_RETURN(Parser::Token_SRC_START, 2)
        }

        if (str.startsWith("</")) {
            PUSH_STATE(Tag)
            DEFAULT_RETURN(Parser::Token_LTFS, 2)
        }

        if (str.startsWith("<!")) {
            PUSH_STATE(DTD)
            DEFAULT_RETURN(Parser::Token_DECLARE_START, 2)
        }

        if (str.startsWith("<") && readLength == 1) {
            PUSH_STATE(Tag)
            DEFAULT_RETURN(Parser::Token_LT, 1)
        }

        //TODO what now
        DEFAULT_RETURN(Parser::Token_ERROR, readLength)
        kDebug() << "Unknown escape: " << ref.toString();
    }

    READ_UNTIL_ANY("<", false);
    DEFAULT_RETURN(Parser::Token_TEXT, readLength)
}

QChar * MlLexer::readUntill(QChar *start, const QChar *end, const QString& condition, bool ignoreWhite, QChar** conditionStart) const {
    int matchCount = 0;
    int length = condition.size();
    bool conditionStartSet = false;
    while (start < end && start) {
        if (ignoreWhite) {
            if (start->isSpace()) {
                start++;
                continue;
            }
            if (condition[matchCount].isSpace()) {
                matchCount++;
                continue;
            }
        }
        if (start->unicode() == condition[matchCount].unicode()) {
            if (conditionStart && !conditionStartSet) {
                *conditionStart = start;
                conditionStartSet = true;
            }
            matchCount++;
        } else {
            if (conditionStart) {
                *conditionStart = 0;
                conditionStartSet = false;
            }
            matchCount = 0;
        }
        if (matchCount == length) return ++start;
        start++;
    }
    return start;
}

QChar * MlLexer::readUntillAny(QChar *start, const QChar *end, const QString& condition, bool ignoreWhite) const {
    while (start < end) {
        if (ignoreWhite) {
            if (start->isSpace()) {
                start++;
                continue;
            }
        }
        if (condition.contains(*start)) {
            return start;
        }
        start++;
    }
    return start;
}

QChar* MlLexer::readWhileAny(QChar* start, const QChar* end, const QString& condition, bool ignoreWhite, QChar** conditionStart) const {
    bool conditionStartSet = false;
    while (start < end) {
        if (ignoreWhite) {
            if (start->isSpace()) {
                start++;
                continue;
            }
        }
        if (condition.contains(*start)) {
            if (conditionStart && !conditionStartSet) {
                *conditionStart = start;
                conditionStartSet = true;
            }
            start++;
            continue;
        }
        break;
    }
    return start;
}

QString MlLexer::removeWhites(QString str) const {
    return str.remove(' ').remove('\r').remove('\n').remove('\t');
}

QString MlLexer::elementName(const QString& token) const
{
    static QRegExp exp("<[ \\t>/\\?!%&]*([^ \\t></?!%&]+[:])?([^ \\t></?!%&]+).*");
    if (exp.exactMatch(QString(token).replace(QChar('\n'), QChar(' ')).replace(QChar('\r'), QChar(' '))))
        return exp.cap(2).toLower();
    return QString::null;
}

/*
QChar * MlLexer::readAndValidateUntill(QChar *start, const QChar *end, const QString& condition, bool ignoreWhite, QChar** conditionStart) const {
    int matchCount = 0;
    int length = condition.size();
    while (start < end) {
   if (ignoreWhite) {
   if (start->isSpace()) {
   start++;
   continue;
}
if (condition[matchCount].isSpace()) {
   matchCount++;
   continue;
}
}
if (*start == condition[matchCount])
matchCount++;
else
    matchCount = 0;
if (matchCount == length)
return ++start;
start++;
}
return start;
}
*/
}

