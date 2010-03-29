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

#include "sgmllexer.h"

#include "sgmlparser.h"

#include <QMetaObject>
#include "idtdhelper.h"

namespace Xml {

SgmlLexer::SgmlLexer(TokenStream* tokenStream, const QString& contents, State initialState) {
    m_content = contents;
    m_tokenEnd = 0;
    m_tokenBegin = 0;
    m_curpos = 0;
    m_dtdHelper = IDtdHelper::instanceForMime("text/xml");
    if (initialState != None)
        m_states.push(LexerState(initialState, m_content.data()));
}

SgmlLexer::~SgmlLexer() {

}

qint64 SgmlLexer::tokenBegin() const {
    return m_tokenBegin;
}

qint64 SgmlLexer::tokenEnd() const {
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
int SgmlLexer::nextTokenKind() {
#define PUSH_STATE(SS, BB) m_states.push(LexerState(SS,BB));

#define POP_STATE if(m_states.size() > 0) m_states.pop();

#define READ_WHILE_ANY(CND, FF) condStart = cursor;\
    condEnd = readWhileAny(cursor, end, CND, FF, &condStart);\
    condLength = condEnd - condStart;\
    readLength = condEnd - cursor;\
    if(readLength == 0) return  Parser::Token_EOF;

#define READ_UNTIL(CND, FF) condStart = cursor;\
    condEnd = readUntill(cursor, end, CND, FF, &condStart);\
    condLength = condEnd - condStart;\
    readLength = condEnd - cursor;\
    if(readLength == 0) return  Parser::Token_EOF;

#define READ_UNTIL_ANY(CND, FF) condEnd = readUntillAny(cursor, end, CND, FF);\
    condStart = condEnd;\
    condLength = condEnd - condStart;\
    readLength = condEnd - cursor;\
    if(readLength == 0) return  Parser::Token_EOF;

//NOTE it looks like PG-QT needs the -1
#define DEFAULT_RETURN(TT, LL) m_token = (TT); m_curpos+=(LL); m_tokenEnd = m_curpos-1; return (TT);

#define DEFAULT_CLOSE(TT, CND, LL)\
    READ_UNTIL(CND, 0);\
    if(cursor == condStart) {\
        POP_STATE\
        DEFAULT_RETURN(TT, LL)\
    }\
    int l = condStart - cursor;\
    DEFAULT_RETURN(Parser::Token_TEXT, l)

    const QChar *start = m_content.data();
    const QChar *end = m_content.data() + m_content.length();

    QChar *condStart = 0, *condEnd = 0;
    int condLength = 0, readLength = 0;

    QChar *cursor = m_content.data() + m_curpos;

    m_tokenBegin = m_curpos;

    if (m_curpos >= m_content.size())
        return Parser::Token_EOF;
    
    if (m_curpos < 0) {
        //This happens when the lexer expected tokens that is not in the content.
        kDebug() << "Cursor position negative!" << endl;
        m_curpos = m_content.size();
        return Parser::Token_ERROR;
    }

    if (!m_states.empty()) {
        switch (m_states.top().state) {

        case(Quote): {
            DEFAULT_CLOSE(Parser::Token_QUOTE, m_tokenEndString, 1)
        }

        //A special case for cdata elements like script style
        case(Cdata): {
            READ_UNTIL(m_tokenEndString, IgnoreWhites | IgnoreCase);
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
                PUSH_STATE(Quote, cursor)
                m_tokenEndString='"';
                DEFAULT_RETURN(Parser::Token_QUOTE, 1)
            }
            case('\''): {
                PUSH_STATE(Quote, cursor)
                m_tokenEndString='\'';
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
                        DEFAULT_RETURN(Parser::Token_CLOSE, 2)
                    }
                }
                DEFAULT_RETURN(Parser::Token_TEXT, 1)
            }
            case('>'): {
                //SGML hacks
                if(m_dtdHelper 
                    && m_states.top().begin 
                    && (m_states.top().begin + 1)->unicode() != '/') {
                    QStringRef ref(&m_content, m_states.top().begin - start, cursor - m_states.top().begin + 1);
                    QString name = elementName(ref.toString());
                    if(m_dtdHelper->cdataElement(name)){
                        m_tokenEndString = QString("</%1>").arg(name);
                        POP_STATE //Have to pop before push else it has no effect
                        PUSH_STATE(Cdata, cursor + 1);
                        DEFAULT_RETURN(Parser::Token_GT, 1)
                    } else if(m_dtdHelper->emptyElement(name)) {
                        POP_STATE
                        DEFAULT_RETURN(Parser::Token_CLOSE, 1)
                    }
                }
                POP_STATE
                DEFAULT_RETURN(Parser::Token_GT, 1)
                
            }

            case('<'): {
                //TODO serious problem
                //We could pop all states and start from scratch, or just pop the top.
                QStringRef seq(&m_content, m_states.top().begin - start, cursor - start + readLength);
                kDebug() << "Bad escape sequence! CLEARING STACK: " << seq.toString();
                m_states.clear();
                DEFAULT_RETURN(Parser::Token_ERROR, 1)
            }

            default: {
            } break;
            }
            READ_UNTIL_ANY(" \r\n\t=:\"'>/", 0)
            DEFAULT_RETURN(Parser::Token_TEXT, readLength)
        }

        case(DTD): {
            switch (cursor->unicode()) {
            case('['): {
                //TODO Whatch for <!-- ] --> or -- ] --
                READ_UNTIL("]", 0)
                m_curpos += readLength;
                return nextTokenKind();
            }
            case('"'): {
                PUSH_STATE(Quote, cursor)
                m_tokenEndString='"';
                DEFAULT_RETURN(Parser::Token_QUOTE, 1)
            }
            case('\''): {
                PUSH_STATE(Quote, cursor)
                m_tokenEndString='\'';
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
            case('>'): {
                QStringRef ref(&m_content, m_states.top().begin - start, cursor - m_states.top().begin + 1);
                QString name, publicId, systemId;
                doctype(ref.toString(), name, publicId, systemId);
                m_dtdHelper = IDtdHelper::instance(publicId, systemId);
                if(!m_dtdHelper)
                    m_dtdHelper = IDtdHelper::instanceForName(name);
                if(!m_dtdHelper) {
                    kDebug() << "Failed to get a DTD instance for DOCTYPE:" << name
                    << "PublicId:" << publicId
                    << "SystemId:" << systemId;
                    m_dtdHelper = IDtdHelper::instanceForMime("text/xml");
                }
                POP_STATE
                DEFAULT_RETURN(Parser::Token_GT, 1)
            }
            case('-'): {
                //Skip comments in DTD
                if ((cursor+1)->unicode() == '-') {
                    cursor+=2;
                    READ_UNTIL("--", 0)
                    m_curpos += readLength+2;
                    return nextTokenKind();
                }
                if ((cursor+1)->isSpace()) {
                    DEFAULT_RETURN(Parser::Token_HYPHEN, 1)
                }
            }
            case('<'): {
                READ_WHILE_ANY("<![--", 0);
                QStringRef ref(&m_content, cursor - start, readLength);
                QString str = ref.toString();

                //Skip comments in DTD
                if (str.startsWith("<!--")) {
                    READ_UNTIL("-->", 0)
                    m_curpos += readLength;
                    return nextTokenKind();
                }

                //TODO serious problem
                //We could pop all states and start from scratch, or just pop the top.
                QStringRef seq(&m_content, m_states.top().begin - start, cursor - start + readLength);
                kDebug() << "Bad escape sequence! CLEARING STACK: " << seq.toString();
                m_states.clear();
                DEFAULT_RETURN(Parser::Token_ERROR, readLength)
            }
            default: {
            } break;
            }

            READ_UNTIL_ANY(" \r\n\t[]()<>%*+?;#|'\"", 0);
            QStringRef ref(&m_content, cursor - start, readLength);
            QString str = ref.toString().toUpper();
            if (str == "DOCTYPE") {
                DEFAULT_RETURN(Parser::Token_DOCTYPE, readLength)
            } else if (str == "PUBLIC") {
                DEFAULT_RETURN(Parser::Token_PUBLIC, readLength)
            } else if (str == "SYSTEM") {
                DEFAULT_RETURN(Parser::Token_SYSTEM, readLength)
            }

            DEFAULT_RETURN(Parser::Token_TEXT, readLength)
        }

        case(Processing): {
            DEFAULT_CLOSE(Parser::Token_PROC_END, "?>", 2)
        }

        case(PHP): {
            //TODO whatch out for "... ?> ..." in php sources
            DEFAULT_CLOSE(Parser::Token_PHP_END, "?>", 2)
        }

        case(Source): {
            //TODO whatch out for "... %> ..." in sources
            DEFAULT_CLOSE(Parser::Token_SRC_END, "%>", 2)
        }

        default: {
            //TODO what now?
            kDebug() << "Unknown lexer state: " << m_states.top().state;
            m_states.clear();
        } break;
        }
    }

    //Whats next ...

    if (cursor->unicode() == '<') {
        READ_WHILE_ANY("<!-[]%?/>", 0);
        QStringRef ref(&m_content, cursor - start, readLength);
        QString str = ref.toString();

        if (str.startsWith("<!--")) {
            READ_UNTIL("-->", 0);
            DEFAULT_RETURN(Parser::Token_COMMENT, readLength)
        }

        if (str.startsWith("<![")) {
            READ_WHILE_ANY("<![PCDATApcdata[", 0);
            QStringRef ref(&m_content, condStart - start, condLength);
            QString str = ref.toString().toUpper();
            if (str == "<![CDATA[") {
                READ_UNTIL("]]>", 0);
                DEFAULT_RETURN(Parser::Token_CDATA, readLength)
            }

            if (str == "<![PCDATA[") {
                READ_UNTIL("]]>", 0);
                DEFAULT_RETURN(Parser::Token_PCDATA, readLength)
            }

            //TODO what now?
            kDebug() << "Unknown content type: " << str;
            DEFAULT_RETURN(Parser::Token_ERROR, readLength)
        }

        if (str.startsWith("<?")) {
            READ_WHILE_ANY("<?phpPHPxmlXML=", 0);
            QStringRef ref(&m_content, condStart - start, condLength);
            QString str = removeWhites(ref.toString().toLower());
            if (str.endsWith('?') && str.length() > 2)str.remove(str.size()-1, 1); //ie <?xml?> or <??>
            int l = str.length();
            if (str == "<?php") {
                PUSH_STATE(PHP, cursor)
                DEFAULT_RETURN(Parser::Token_PHP_START, l)
            }
            if (str == "<?xml") {
                PUSH_STATE(Processing, cursor)
                DEFAULT_RETURN(Parser::Token_PROC_START, l)
            }
            if (str == "<?=") {
                PUSH_STATE(PHP, cursor)
                DEFAULT_RETURN(Parser::Token_PHP_START, l)
            }

            //TODO what now?
            kDebug() << "Unknown processing instruction: " << str;
            DEFAULT_RETURN(Parser::Token_ERROR, readLength)
        }

        if (str.startsWith("<%")) {
            PUSH_STATE(Source, cursor)
            DEFAULT_RETURN(Parser::Token_SRC_START, 2)
        }

        if (str.startsWith("</")) {
            PUSH_STATE(Tag, cursor)
            DEFAULT_RETURN(Parser::Token_CLOSE, 2)
        }

        if (str.startsWith("<!")) {
            READ_UNTIL_ANY(" \r\n\t>(]%", 0);
            QStringRef ref(&m_content, cursor - start, readLength);
            QString str = ref.toString().toUpper();
            if (str.endsWith("DOCTYPE")) {
                PUSH_STATE(DTD, cursor)
                DEFAULT_RETURN(Parser::Token_DECLARE_START, 2)
            }
            kDebug() << "Unknown escape: " << ref.toString();
            DEFAULT_RETURN(Parser::Token_ERROR, readLength)
        }

        if (str.startsWith("<") && readLength == 1) {
            PUSH_STATE(Tag, cursor)
            DEFAULT_RETURN(Parser::Token_LT, 1)
        }

        //TODO what now
        kDebug() << "Unknown escape: " << ref.toString();
        DEFAULT_RETURN(Parser::Token_ERROR, readLength)
    }

    READ_UNTIL_ANY("<", 0);
    DEFAULT_RETURN(Parser::Token_TEXT, readLength)
}

QChar * SgmlLexer::readUntill(QChar *start, const QChar *end, const QString& condition, int flag, QChar** conditionStart) const {
    int matchCount = 0, length = condition.size();
    bool conditionStartSet = false;
    while (start < end && start) {
        if (flag & IgnoreWhites) {
            if (start->isSpace()) {
                start++;
                continue;
            }
            if (condition[matchCount].isSpace()) {
                matchCount++;
                continue;
            }
        }
        if (start->unicode() == condition[matchCount].unicode()
            || (flag & IgnoreCase && start->toLower().unicode() == condition[matchCount].toLower().unicode())) {
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


QChar * SgmlLexer::readUntillAny(QChar *start, const QChar *end, const QString& condition, int flag) const {
    while (start < end) {
        if (flag & IgnoreWhites) {
            if (start->isSpace()) {
                start++;
                continue;
            }
        }
        //TODO slow IgnoreCase if condition holds
        if (condition.contains(*start)
            || (flag & IgnoreCase && condition.toLower().contains(start->toLower()))) {
            return start;
        }
        start++;
    }
    return start;
}

QChar* SgmlLexer::readWhileAny(QChar* start, const QChar* end, const QString& condition, int flag, QChar** conditionStart) const {
    bool conditionStartSet = false;
    while (start < end) {
        if (flag & IgnoreWhites) {
            if (start->isSpace()) {
                start++;
                continue;
            }
        }
        //TODO slow IgnoreCase if condition holds
        if (condition.contains(*start)
            || (flag & IgnoreCase && condition.toLower().contains(start->toLower()))) {
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

QString SgmlLexer::removeWhites(QString str) const {
    return str.remove(' ').remove('\r').remove('\n').remove('\t');
}

QString SgmlLexer::elementName(const QString& token) const
{
    static QRegExp exp("<[ \\t>/\\?!%&]*([^ \\t></?!%&]+[:])?([^ \\t></?!%&]+).*");
    if (exp.exactMatch(QString(token).replace(QChar('\n'), QChar(' ')).replace(QChar('\r'), QChar(' '))))
        return exp.cap(2).toUpper();
    kDebug() << "Element name match failed!";
    return QString::null;
}

void SgmlLexer::doctype(const QString& token, QString& name, QString& publicId, QString& systemId) const
{
    static QRegExp exp(".*<!\\s*doctype\\s+([\\w\\d]*)\\s*(public\\s+([\"'])([^\"']*)[\"'](\\s+[\"']([^\"']*)[\"'])?)?(system\\s*[\"']([^\"']*)[\"'])?.*",
                       Qt::CaseInsensitive);
    if(exp.exactMatch(token)) {
        name = exp.cap(1);
        publicId = exp.cap(4);
        systemId = exp.cap(6);
        if(systemId.isEmpty())
            systemId = exp.cap(8);
    } else {
        kDebug() << "Doctype match failed!";
    }
}



}

