/*****************************************************************************
 * Copyright (c) 2009 Ruan Strydom <rm3dom@gmail.com>                        *
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

/*
    NOTE: Does not work will rewrite
*/

#include "htmlformatter.h"

#include <interfaces/isourceformatter.h>
#include <KDebug>


HtmlFormatter::HtmlFormatter() : XmlFormatter() {
    m_options.insert ( "INDENT", QVariant ( 2 ) );
}

HtmlFormatter::~HtmlFormatter() {}

QString HtmlFormatter::formatSource ( const QString& text, const QString& leftContext, const QString& rightContext ) const {
    Q_UNUSED ( rightContext );

    if ( text.size() < 1 )
        return text;

    int indent = m_options["INDENT"].toInt();
    if ( indent < 0 || indent > 99 )
        indent = 2;

    QString strResult = "";
    QTextStream result ( &strResult );
    QString strIn = text;
    QTextStream in ( &strIn );

    //Manage unclosed tags
    QStack<QString> stack;

    TokenType type;
    QString token;
    QString content;
    bool inElement = false;
    int depth = 0;
    QString endLine = "\n";
    //This is for a newline quirk
    int formatCount = 0;
    qint64 error;
    if ( leftContext.size()  > 0 ) {
        QString leftStr = leftContext;
        QTextStream leftIn ( &leftStr );
        while ( !leftIn.atEnd() ) {
            token = nextToken ( leftIn, &type, &error );
            switch ( type ) {
            case  StartElement:
                depth++;
                inElement = true;
                break;
            case  EndElement:
                depth--;
                break;
            case  ClosedElement:
                inElement = false;
                break;
            case  Text:
                content = token;
                break;
            case  Error:
                kDebug() << "Error in document at pos:" << error;
                return text;
                break;
            };
        }
    }

    while ( !in.atEnd() ) {
        token = nextToken ( in, &type, &error );
        if (depth < 0) depth = 0;
        switch ( type ) {
        case  None:
            return text;
            break;
        case  Error:
            kDebug() << "Error in document at pos:" << error;
            return text;
            break;
        case  Processing:
            result << content << token;
            content = "";
            formatCount++;
            break;
        case  ClosedElement: {
            //NOTE Duplication of below
            int align = 0;
            QStringList list;
            if ( content.size() > 0 ) {
                list = content.split ( endLine );
                result << list[0] << endLine;
                for ( int i = 1; i < list.size() - 1; i++ )
                    result<< indentString ( depth, indent ) << trimText(list[i]) << endLine;
                if ( !list.last().trimmed().isEmpty() && list.size() > 1)
                    result<< indentString ( depth, indent ) << trimText(list.last()) << endLine;
                if ( list.size() > 0 )
                    align = list.last().size();
            }
            content = "";
            list = token.split ( endLine );
            result << list.first();
            for ( int i = 1; i < list.size(); i++ )
                result << endLine << indentString ( depth, indent ).remove ( 0, align ) << list[i];

            formatCount++;
            inElement = false;
        }
        break;
        case  StartElement: {
            //This needs to keep parameters that break alignment.
            //ie: <element id="test"
            //             name="test">
            //It must keep the alignment
            int align = 0;
            QString tag = tagName(token);
            QStringList list;

            if (isEmptyElement(tag)) {
                if ( content.size() > 0 ) {
                    list = content.split ( endLine );
                    result << list[0];
                    for ( int i = 1; i < list.size() - 1; i++ )
                        result << endLine << indentString ( depth, indent ) << trimText(list[i]);
                    if ( !list.last().trimmed().isEmpty() && list.size() > 1)
                        result << endLine << indentString ( depth, indent ) << trimText(list.last());
                    if ( list.size() > 0 )
                        align = list.last().size();
                }
                list = token.split ( endLine );
                result << list.first();
                for ( int i = 1; i < list.size(); i++ )
                    result << endLine << indentString ( depth, indent ).remove ( 0, align ) << list[i];
            } else {
                if ( content.size() > 0 ) {
                    list = content.split ( endLine );
                    result<< indentString ( depth, indent )  << list[0] << endLine;
                    for ( int i = 1; i < list.size() - 1; i++ )
                        result<< indentString ( depth, indent ) << trimText(list[i]) << endLine;
                    if ( !list.last().trimmed().isEmpty() && list.size() > 1) {
                        result<< indentString ( depth, indent ) << trimText(list.last()) << endLine;
                    }
                    if ( list.size() > 0 )
                        align = list.last().size();
                    if ( list.last().trimmed().isEmpty() && list.size() > 1)
                        result<< indentString ( depth, indent );
                }
                list = token.split ( endLine );
                result << list.first();
                for ( int i = 1; i < list.size(); i++ )
                    result << endLine << indentString ( depth, indent ).remove ( 0, align ) << list[i];
                stack.push(tag);
                depth++;
                inElement = true;
            }
            content = "";
            formatCount++;
            if (tag == "script") {
                content += readUntill(in,"</script>");
                in.seek ( in.pos() - 9 );
                content = content.left(content.size() - 9);
            } else if (tag == "style") {
                content += readUntill(in,"</style>");
                in.seek ( in.pos() - 8 );
                content = content.left(content.size() - 8);
            }
        }
        break;
        case  EndElement:
        {
            depth--;
            QString tag = tagName(token);
            int stackPos = stack.lastIndexOf(tag);
            if ( stackPos >= 0) {
                stack.remove(stackPos, stack.size() - stackPos);
                depth = stackPos;
            }

            if ( content.size() > 0 ) {
                QStringList list = content.split ( endLine );
                result << indentString ( depth + 1, indent )  << list[0] << endLine;
                for ( int i = 1; i < list.size() - 1; i++ )
                    result<< indentString ( depth + 1, indent ) << trimText(list[i]) << endLine;
                if ( !list.last().trimmed().isEmpty() && list.size() > 1)
                    result<< indentString ( depth + 1, indent ) << trimText(list.last()) << endLine;
                if ( list.last().trimmed().isEmpty() && list.size() > 1)
                    result<< indentString ( depth, indent );
            }

            result << token;

            inElement = false;
            content = "";
            formatCount++;
        }
        break;
        case  CDATA:
            result << content << token;
            content = "";
            formatCount++;
            break;
        case  DTD:
            result << content << token;
            content = "";
            formatCount++;
            break;
        case  Comment:
            result << content << token;
            content = "";
            formatCount++;
            break;
        case  Text:
            content += token;
            formatCount++;
            break;
        case  Other:
            //Normaly scripts like php and jsp etc.
            //TODO Possibly get a formatter for the mime
            content += token;
            break;
        default:
            return text;
        };
    }
    if ( content.length() > 0 )
        result << content;
    return strResult;
}

QString HtmlFormatter::trimText(const QString& token) const
{
    int trimLeft = 0;
    for (trimLeft; trimLeft < token.length() ; trimLeft++) {
        if (!token[trimLeft].isSpace())
            break;
    }
    return token.right(token.size() - trimLeft);
}

QString HtmlFormatter::tagName(const QString& token) const
{
    static QRegExp exp("<[ \\n\\t\\r>/\\?!%&]*([^ \\n\\t\\r></?!%&]+).*");
    if (exp.exactMatch(QString(token).replace(QChar('\n'), QChar(' ')).replace(QChar('\r'), QChar(' '))))
        return exp.cap(1).toLower();
    return QString::null;
}

QString HtmlFormatter::attributeValue(const QString &token, const QString& attribute) const
{
    QRegExp exp(QString(".*%1\\s*=\\s*(\"|')([^\\\\1]*)\\1.*").arg(attribute));
    exp.exactMatch(token);
    return exp.cap(2);
}

//This is from the 4.* loose dtd
bool HtmlFormatter::isEmptyElement(const QString& tag) const
{
    static QHash<QString,QString> hash = ( {
        QHash<QString,QString> hash;
        hash.insert("basefont", QString::null);
        hash.insert("br", QString::null);
        hash.insert("link", QString::null);
        hash.insert("img", QString::null);
        hash.insert("param", QString::null);
        hash.insert("hr", QString::null);
        hash.insert("input", QString::null);
        hash.insert("frame", QString::null);
        hash.insert("isindex", QString::null);
        hash.insert("base", QString::null);
        hash.insert("meta", QString::null);
        hash;
    });
    return hash.contains(tag.toLower());
}
