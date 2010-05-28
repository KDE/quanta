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

#include "xmlformatter.h"

#include <interfaces/isourceformatter.h>
#include <KDebug>

XmlFormatter::XmlFormatter() {
    m_options.insert ( "INDENT", QVariant ( 2 ) );
}

void XmlFormatter::loadStyle ( const QString& content ) {
    if ( content.isNull() )
        return;
    m_options = KDevelop::ISourceFormatter::stringToOptionMap ( content );
}

QString XmlFormatter::saveStyle() {
    return KDevelop::ISourceFormatter::optionMapToString ( m_options );
}

QString indentString ( int depth, int indent ) {
    if ( depth < 1 || indent < 1 )
        return "";
    int size = depth * indent + 1;
    char s[size];
    memset ( s, ' ', size );
    s[size-1] = '\0';
    return s;
}

QString XmlFormatter::formatSource ( const QString& text, const QString& leftContext, const QString& rightContext ) const {
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

    TokenType type;
    QString token;
    QString content;
    bool inElement = false;
    int depth = 0;
    QString endLine = "\n";
    //This is for a newline quirk
    int formatCount = 0;

    if ( leftContext.size()  > 0 ) {
        QString leftStr = leftContext;
        QTextStream leftIn ( &leftStr );
        while ( !leftIn.atEnd() ) {
            token = nextToken ( leftIn, &type );
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
                kDebug() << "Error in document: " << token;
                return text;
                break;
            };
        }
    }

    while ( !in.atEnd() ) {
        token = nextToken ( in, &type );

        switch ( type ) {
        case  None:
            kDebug() << "Error in document: " << token;
            return text;
            break;
        case  Error:
            kDebug() << "Error in document: " << token;
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
                for ( int i = 0; i < list.size() - 1; i++ )
                    result << list[i] << endLine;
                if ( !list.last().trimmed().isEmpty() )
                    result << list.last() << endLine;
                if ( list.size() > 0 )
                    align = list.last().size();
            } else if(formatCount)
                result << endLine;
            content = "";
            list = token.split ( endLine );
            result << indentString ( depth, indent ) << list.first();
            for ( int i = 1; i < list.size(); i++ )
                result << endLine << indentString ( depth, indent ).remove ( 0, align ) << list[i];

            formatCount++;
            inElement = false;
        }
        break;
        case  StartElement: {
            //This needs to keep parameters that break alignment.
            int align = 0;
            QStringList list;
            if ( content.size() > 0 ) {
                list = content.split ( endLine );
                for ( int i = 0; i < list.size() - 1; i++ )
                    result << list[i] << endLine;
                if ( !list.last().trimmed().isEmpty() )
                    result << list.last() << endLine;
                if ( list.size() > 0 )
                    align = list.last().size();
            } else if(formatCount)
                result << endLine;
            content = "";
            list = token.split ( endLine );
            result << indentString ( depth, indent ) << list.first();
            for ( int i = 1; i < list.size(); i++ )
                result << endLine << indentString ( depth, indent ).remove ( 0, align ) << list[i];

            depth++;
            formatCount++;
            inElement = true;
        }
        break;
        case  EndElement:
            if ( inElement ) {
                result << content << token;
            } else {
                if ( content.trimmed().size() > 0 ) {
                    QStringList list = content.split ( endLine );
                    for ( int i = 0; i < list.size() - 1; i++ )
                        result << list[i] << endLine;
                    if ( !list.last().trimmed().isEmpty() ) {
                        result << list.last() << endLine;
                    }
                } else
                    result << endLine;
                result << indentString ( depth - 1, indent ) << token;
            }
            inElement = false;
            depth--;
            content = "";
            formatCount++;
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
            content = token;
            formatCount++;
            break;
        default:
            kDebug() << "Error in document: " << token;
            return text;
        };
    }
    if ( content.length() > 0 )
        result << content;
    return strResult;
}

QString XmlFormatter::compactSource ( const QString& text ) const {
    if ( text.size() < 1 )
        return text;

    QString strResult = "";
    QTextStream result ( &strResult );
    QString strIn = text;
    QTextStream in ( &strIn );

    TokenType type;
    QString token;
    QString content;
    bool inElement = false;
    int depth = 0;
    QString endLine = "\n";

    while ( !in.atEnd() ) {
        token = nextToken ( in, &type );
        //KMessageBox::information(0, QString("%1  #%2#").arg(tokenText(type),token));
        switch ( type ) {
        case  None:
            kDebug() << "Error in document: " << token;
            return text;
        case  Error:
            kDebug() << "Error in document: " << token;
            return text;
            break;
        case  Processing:
            result << token.trimmed();
            content = "";
            break;
        case  ClosedElement: {
            QStringList list;
            result << content.trimmed();
            content = "";
            list = token.split ( endLine );
            result << list.first().trimmed();
            for ( int i = 1; i < list.size(); i++ )
                result << " " << list[i].trimmed();
            inElement = false;
        }
        break;
        case  StartElement: {
            QStringList list;
            result << content.trimmed();
            content = "";
            list = token.split ( endLine );
            result << list.first().trimmed();
            for ( int i = 1; i < list.size(); i++ )
                result << " " << list[i].trimmed();
            depth++;
            inElement = true;
        }
        break;
        case  EndElement:
            //Not a text-element
            if ( !inElement ) {
                result << content.trimmed();
                content = "";
            }
            inElement = false;
            depth--;
            result << content << token;
            content = "";
            break;
        case  CDATA:
            result << content << token;
            content = "";
            break;
        case  DTD:
            result << token;
            content = "";
            break;
        case  Comment:
            result << token;
            content = "";
            break;
        case  Text:
            content = token;
            break;
        default:
            kDebug() << "Error in document: " << token;
            return text;
        };
    }
    if ( content.length() > 0 )
        result << content;
    return strResult;
}

QString XmlFormatter::escapeSource ( const QString& text ) const {
    QString strResult;
    QTextStream result ( &strResult );
    QChar c;

    for ( int i = 0; i < text.size(); i++ ) {
        c = text[i];

        if ( ( c < 32 ) && ! ( c == 9 || c == 10 || c == 13 ) ) {
            result << QString ( "&#%1;" ).arg ( c.digitValue() );
            continue;
        }

        if ( c == '&' ) {
            int semiColonIndex = text.mid ( i, 10 ).indexOf ( ';', 0 );
            int nextAmpIndex = text.mid ( i, 10 ).indexOf ( '&', 1 );
            if ( ( semiColonIndex == -1 ) || ( ( nextAmpIndex < semiColonIndex ) && ( nextAmpIndex > 0 ) ) )
                result << "&amp;";
            else {
                result << text.mid ( i, semiColonIndex + 1 );
                i += semiColonIndex;
            }
        } else if ( c == '>' )
            result << "&gt;";
        else if ( c == '<' )
            result << "&lt;";
        else if ( c == '"' )
            result << "&quot;";
        else if ( c == '\'' )
            result << "&#39;";
        else if ( c < 160 )
            result << c;
        else
            result << escapeSequenceForChar ( c );
    }
    result.flush();
    return strResult;
}

QString XmlFormatter::unescapeSource ( const QString& text ) const {
    QString tmpStr;
    QChar tmpChar;
    QChar c;
    QString strResult;
    QTextStream result ( &strResult );
    int maxPos = text.length();
    int curPos = 0;
    bool broke = false;

    while ( curPos < maxPos ) {
        c = text[curPos++];
        if ( c == '&' ) {
            broke = true;
            int tmpPos = curPos;
            QChar nextChar = text[tmpPos];
            if ( nextChar.isLetter() ) {
                while ( tmpPos < maxPos ) {
                    nextChar = text[tmpPos++];
                    if ( !nextChar.isLetter() ) {
                        if ( nextChar == ';' ) {
                            tmpStr = text.mid ( curPos, tmpPos - curPos - 1 );
                            tmpChar = charForEscapeSequence ( tmpStr );
                            if ( !tmpChar.isNull() ) {
                                c = tmpChar;
                                curPos = tmpPos;
                                broke = false;
                            }
                        }
                        break;
                    }
                }
            } else if ( nextChar == '#' ) {
                nextChar = text[tmpPos++];
                while ( tmpPos < maxPos ) {
                    nextChar = text[tmpPos++];
                    if ( !nextChar.isNumber() ) {
                        if ( nextChar == ';' ) {
                            tmpStr = text.mid ( curPos + 1, tmpPos - ( curPos + 1 ) - 1 );
                            c = QChar ( tmpStr.toInt() );
                            curPos = tmpPos;
                            if ( !c.isNull() )
                                broke = false;
                        }
                        break;
                    }
                }
            }
        }
        if ( broke )
            return text;
        result << c;
    }
    result.flush();
    return strResult;
}

QString XmlFormatter::nextToken ( QTextStream& stream, XmlFormatter::TokenType* type ) const {
    QString ret;
    QChar cur;
    *type = Text;
    while ( !stream.atEnd() ) {
        stream >> cur;
        ret += cur;
        if ( cur == '<' ) {
            //We have text
            if ( ret.size() > 1 ) {
                stream.seek ( stream.pos() - 1 );
                *type = Text;
                return ret.remove ( ret.size() - 1, 1 );
            }
            QString test = stream.read ( 3 );
            if ( test == "?xm" ) {
                //Processing instruction
                ret += test + readUntill ( stream, "?>" );
                *type = Processing;
                return ret;
            } else if ( test == "!--" ) {
                //Comment
                ret += test + readUntill ( stream, "-->" );
                *type = Comment;
                return ret;
            } else if ( test == "!DO" ) {
                //Possible DTD
                test += stream.read ( 5 );
                if ( test == "!DOCTYPE" ) {
                    ret += test;
                    while ( !stream.atEnd() ) {
                        stream >> cur;
                        ret += cur;
                        if ( cur == '[' ) {
                            //TODO what about conditions ie <![INCLUDE[ ]>
                            ret += readUntill ( stream, "]>" );
                            break;
                        } else if ( cur == '>' ) break;
                    }
                    *type = DTD;
                    return ret;
                } else {
                    stream.seek ( stream.pos() - test.size() );
                }
            } else if ( test == "![C" ) {
                //Possible CDATA
                //TODO what about PCDATA
                //TODO what about conditions ie <![IE6[  ]>
                test += stream.read ( 5 );
                if ( test == "![CDATA[" ) {
                    ret += test + readUntill ( stream, "]]>" );
                    *type = CDATA;
                    return ret;
                } else {
                    stream.seek ( stream.pos() - test.size() );
                }
            } else {
                stream.seek ( stream.pos() - test.size() );
            }
            int error = 0;
            ret += readAndValidateUntill ( stream, ">", &error );
            if ( ret.endsWith ( "/>" ) )
                *type = ClosedElement;
            else if ( ret.startsWith ( "</" ) )
                *type = EndElement;
            else if ( ret.endsWith ( ">" ) )
                *type = StartElement;
            else
                *type = Error;
            return ret;
        }
    }
    return ret;
}

QString XmlFormatter::readUntill ( QTextStream & stream, const QString& condition ) const {
    QString ret = "";
    QChar c;
    while ( !stream.atEnd() ) {
        stream >> c;
        ret += c;
        if ( ret.endsWith ( condition ) )
            return ret;
    }
    return ret;
}

QString XmlFormatter::readAndValidateUntill ( QTextStream& stream, const QString& condition, int *error ) const {
    //TODO Not allowed to contain '<' or "'<'" or "'>'" or "'&'" or '&'
    QString ret = "";
    QChar c;
    *error = None;
    char escape = 0;
    while ( !stream.atEnd() ) {
        stream >> c;
        if(c == escape)
            escape = 0;
        else if(!escape && (c == '"' || c == '\''))
            escape = c.toAscii();
        if ( c == '<' && !escape)
            *error = '<';
        if ( *error )
            return QString();
        ret += c;
        if ( ret.endsWith ( condition ) )
            return ret;
    }
    return ret;
}

QString XmlFormatter::tokenText ( XmlFormatter::TokenType type ) const {
    switch ( type ) {
    case  None:
        return "None";
    case  Error:
        return "Error";
    case  Processing:
        return "Processing";
    case  ClosedElement:
        return "ClosedElement";
    case  StartElement:
        return "StartElement";
    case  EndElement:
        return "EndElement";
    case  CDATA:
        return "CDATA";
    case  DTD:
        return "DTD";
    case  Comment:
        return "Comment";
    case  Text:
        return "Text";
    default:
        return "Undefined";
    };
    return "Undefined";
}

QHash<QString, QChar> getEscapeTable() {
    QHash<QString, QChar> charTable;
    charTable.insert ( "quot", QChar ( 34 ) );
    charTable.insert ( "amp", QChar ( 38 ) );
    charTable.insert ( "apos", QChar ( 39 ) );
    charTable.insert ( "lt", QChar ( 60 ) );
    charTable.insert ( "gt", QChar ( 62 ) );
    charTable.insert ( "nbsp", QChar ( 160 ) );
    charTable.insert ( "iexcl", QChar ( 161 ) );
    charTable.insert ( "cent", QChar ( 162 ) );
    charTable.insert ( "pound", QChar ( 163 ) );
    charTable.insert ( "curren", QChar ( 164 ) );
    charTable.insert ( "yen", QChar ( 165 ) );
    charTable.insert ( "brvbar", QChar ( 166 ) );
    charTable.insert ( "sect", QChar ( 167 ) );
    charTable.insert ( "uml", QChar ( 168 ) );
    charTable.insert ( "copy", QChar ( 169 ) );
    charTable.insert ( "ordf", QChar ( 170 ) );
    charTable.insert ( "laquo", QChar ( 171 ) );
    charTable.insert ( "not", QChar ( 172 ) );
    charTable.insert ( "shy", QChar ( 173 ) );
    charTable.insert ( "reg", QChar ( 174 ) );
    charTable.insert ( "macr", QChar ( 175 ) );
    charTable.insert ( "deg", QChar ( 176 ) );
    charTable.insert ( "plusmn", QChar ( 177 ) );
    charTable.insert ( "sup2", QChar ( 178 ) );
    charTable.insert ( "sup3", QChar ( 179 ) );
    charTable.insert ( "acute", QChar ( 180 ) );
    charTable.insert ( "micro", QChar ( 181 ) );
    charTable.insert ( "para", QChar ( 182 ) );
    charTable.insert ( "middot", QChar ( 183 ) );
    charTable.insert ( "cedil", QChar ( 184 ) );
    charTable.insert ( "sup1", QChar ( 185 ) );
    charTable.insert ( "ordm", QChar ( 186 ) );
    charTable.insert ( "raquo", QChar ( 187 ) );
    charTable.insert ( "frac14", QChar ( 188 ) );
    charTable.insert ( "frac12", QChar ( 189 ) );
    charTable.insert ( "frac34", QChar ( 190 ) );
    charTable.insert ( "iquest", QChar ( 191 ) );
    charTable.insert ( "Agrave", QChar ( 192 ) );
    charTable.insert ( "Aacute", QChar ( 193 ) );
    charTable.insert ( "Acirc", QChar ( 194 ) );
    charTable.insert ( "Atilde", QChar ( 195 ) );
    charTable.insert ( "Auml", QChar ( 196 ) );
    charTable.insert ( "Aring", QChar ( 197 ) );
    charTable.insert ( "AElig", QChar ( 198 ) );
    charTable.insert ( "Ccedil", QChar ( 199 ) );
    charTable.insert ( "Egrave", QChar ( 200 ) );
    charTable.insert ( "Eacute", QChar ( 201 ) );
    charTable.insert ( "Ecirc", QChar ( 202 ) );
    charTable.insert ( "Euml", QChar ( 203 ) );
    charTable.insert ( "Igrave", QChar ( 204 ) );
    charTable.insert ( "Iacute", QChar ( 205 ) );
    charTable.insert ( "Icirc", QChar ( 206 ) );
    charTable.insert ( "Iuml", QChar ( 207 ) );
    charTable.insert ( "ETH", QChar ( 208 ) );
    charTable.insert ( "Ntilde", QChar ( 209 ) );
    charTable.insert ( "Ograve", QChar ( 210 ) );
    charTable.insert ( "Oacute", QChar ( 211 ) );
    charTable.insert ( "Ocirc", QChar ( 212 ) );
    charTable.insert ( "Otilde", QChar ( 213 ) );
    charTable.insert ( "Ouml", QChar ( 214 ) );
    charTable.insert ( "times", QChar ( 215 ) );
    charTable.insert ( "Oslash", QChar ( 216 ) );
    charTable.insert ( "Ugrave", QChar ( 217 ) );
    charTable.insert ( "Uacute", QChar ( 218 ) );
    charTable.insert ( "Ucirc", QChar ( 219 ) );
    charTable.insert ( "Uuml", QChar ( 220 ) );
    charTable.insert ( "Yacute", QChar ( 221 ) );
    charTable.insert ( "THORN", QChar ( 222 ) );
    charTable.insert ( "szlig", QChar ( 223 ) );
    charTable.insert ( "agrave", QChar ( 224 ) );
    charTable.insert ( "aacute", QChar ( 225 ) );
    charTable.insert ( "acirc", QChar ( 226 ) );
    charTable.insert ( "atilde", QChar ( 227 ) );
    charTable.insert ( "auml", QChar ( 228 ) );
    charTable.insert ( "aring", QChar ( 229 ) );
    charTable.insert ( "aelig", QChar ( 230 ) );
    charTable.insert ( "ccedil", QChar ( 231 ) );
    charTable.insert ( "egrave", QChar ( 232 ) );
    charTable.insert ( "eacute", QChar ( 233 ) );
    charTable.insert ( "ecirc", QChar ( 234 ) );
    charTable.insert ( "euml", QChar ( 235 ) );
    charTable.insert ( "igrave", QChar ( 236 ) );
    charTable.insert ( "iacute", QChar ( 237 ) );
    charTable.insert ( "icirc", QChar ( 238 ) );
    charTable.insert ( "iuml", QChar ( 239 ) );
    charTable.insert ( "eth", QChar ( 240 ) );
    charTable.insert ( "ntilde", QChar ( 241 ) );
    charTable.insert ( "ograve", QChar ( 242 ) );
    charTable.insert ( "oacute", QChar ( 243 ) );
    charTable.insert ( "ocirc", QChar ( 244 ) );
    charTable.insert ( "otilde", QChar ( 245 ) );
    charTable.insert ( "ouml", QChar ( 246 ) );
    charTable.insert ( "divide", QChar ( 247 ) );
    charTable.insert ( "oslash", QChar ( 248 ) );
    charTable.insert ( "ugrave", QChar ( 249 ) );
    charTable.insert ( "uacute", QChar ( 250 ) );
    charTable.insert ( "ucirc", QChar ( 251 ) );
    charTable.insert ( "uuml", QChar ( 252 ) );
    charTable.insert ( "yacute", QChar ( 253 ) );
    charTable.insert ( "thorn", QChar ( 254 ) );
    charTable.insert ( "yuml", QChar ( 255 ) );
    charTable.insert ( "OElig", QChar ( 338 ) );
    charTable.insert ( "oelig", QChar ( 339 ) );
    charTable.insert ( "Scaron", QChar ( 352 ) );
    charTable.insert ( "scaron", QChar ( 353 ) );
    charTable.insert ( "Yuml", QChar ( 376 ) );
    charTable.insert ( "fnof", QChar ( 402 ) );
    charTable.insert ( "circ", QChar ( 710 ) );
    charTable.insert ( "tilde", QChar ( 732 ) );
    charTable.insert ( "Alpha", QChar ( 913 ) );
    charTable.insert ( "Beta", QChar ( 914 ) );
    charTable.insert ( "Gamma", QChar ( 915 ) );
    charTable.insert ( "Delta", QChar ( 916 ) );
    charTable.insert ( "Epsilon", QChar ( 917 ) );
    charTable.insert ( "Zeta", QChar ( 918 ) );
    charTable.insert ( "Eta", QChar ( 919 ) );
    charTable.insert ( "Theta", QChar ( 920 ) );
    charTable.insert ( "Iota", QChar ( 921 ) );
    charTable.insert ( "Kappa", QChar ( 922 ) );
    charTable.insert ( "Lambda", QChar ( 923 ) );
    charTable.insert ( "Mu", QChar ( 924 ) );
    charTable.insert ( "Nu", QChar ( 925 ) );
    charTable.insert ( "Xi", QChar ( 926 ) );
    charTable.insert ( "Omicron", QChar ( 927 ) );
    charTable.insert ( "Pi", QChar ( 928 ) );
    charTable.insert ( "Rho", QChar ( 929 ) );
    charTable.insert ( "Sigma", QChar ( 931 ) );
    charTable.insert ( "Tau", QChar ( 932 ) );
    charTable.insert ( "Upsilon", QChar ( 933 ) );
    charTable.insert ( "Phi", QChar ( 934 ) );
    charTable.insert ( "Chi", QChar ( 935 ) );
    charTable.insert ( "Psi", QChar ( 936 ) );
    charTable.insert ( "Omega", QChar ( 937 ) );
    charTable.insert ( "alpha", QChar ( 945 ) );
    charTable.insert ( "beta", QChar ( 946 ) );
    charTable.insert ( "gamma", QChar ( 947 ) );
    charTable.insert ( "delta", QChar ( 948 ) );
    charTable.insert ( "epsilon", QChar ( 949 ) );
    charTable.insert ( "zeta", QChar ( 950 ) );
    charTable.insert ( "eta", QChar ( 951 ) );
    charTable.insert ( "theta", QChar ( 952 ) );
    charTable.insert ( "iota", QChar ( 953 ) );
    charTable.insert ( "kappa", QChar ( 954 ) );
    charTable.insert ( "lambda", QChar ( 955 ) );
    charTable.insert ( "mu", QChar ( 956 ) );
    charTable.insert ( "nu", QChar ( 957 ) );
    charTable.insert ( "xi", QChar ( 958 ) );
    charTable.insert ( "omicron", QChar ( 959 ) );
    charTable.insert ( "pi", QChar ( 960 ) );
    charTable.insert ( "rho", QChar ( 961 ) );
    charTable.insert ( "sigmaf", QChar ( 962 ) );
    charTable.insert ( "sigma", QChar ( 963 ) );
    charTable.insert ( "tau", QChar ( 964 ) );
    charTable.insert ( "upsilon", QChar ( 965 ) );
    charTable.insert ( "phi", QChar ( 966 ) );
    charTable.insert ( "chi", QChar ( 967 ) );
    charTable.insert ( "psi", QChar ( 968 ) );
    charTable.insert ( "omega", QChar ( 969 ) );
    charTable.insert ( "thetasym", QChar ( 977 ) );
    charTable.insert ( "upsih", QChar ( 978 ) );
    charTable.insert ( "piv", QChar ( 982 ) );
    charTable.insert ( "ensp", QChar ( 8194 ) );
    charTable.insert ( "emsp", QChar ( 8195 ) );
    charTable.insert ( "thinsp", QChar ( 8201 ) );
    charTable.insert ( "zwnj", QChar ( 8204 ) );
    charTable.insert ( "zwj", QChar ( 8205 ) );
    charTable.insert ( "lrm", QChar ( 8206 ) );
    charTable.insert ( "rlm", QChar ( 8207 ) );
    charTable.insert ( "ndash", QChar ( 8211 ) );
    charTable.insert ( "mdash", QChar ( 8212 ) );
    charTable.insert ( "lsquo", QChar ( 8216 ) );
    charTable.insert ( "rsquo", QChar ( 8217 ) );
    charTable.insert ( "sbquo", QChar ( 8218 ) );
    charTable.insert ( "ldquo", QChar ( 8220 ) );
    charTable.insert ( "rdquo", QChar ( 8221 ) );
    charTable.insert ( "bdquo", QChar ( 8222 ) );
    charTable.insert ( "dagger", QChar ( 8224 ) );
    charTable.insert ( "Dagger", QChar ( 8225 ) );
    charTable.insert ( "bull", QChar ( 8226 ) );
    charTable.insert ( "hellip", QChar ( 8230 ) );
    charTable.insert ( "permil", QChar ( 8240 ) );
    charTable.insert ( "prime", QChar ( 8242 ) );
    charTable.insert ( "Prime", QChar ( 8243 ) );
    charTable.insert ( "lsaquo", QChar ( 8249 ) );
    charTable.insert ( "rsaquo", QChar ( 8250 ) );
    charTable.insert ( "oline", QChar ( 8254 ) );
    charTable.insert ( "frasl", QChar ( 8260 ) );
    charTable.insert ( "euro", QChar ( 8364 ) );
    charTable.insert ( "image", QChar ( 8465 ) );
    charTable.insert ( "weierp", QChar ( 8472 ) );
    charTable.insert ( "real", QChar ( 8476 ) );
    charTable.insert ( "trade", QChar ( 8482 ) );
    charTable.insert ( "alefsym", QChar ( 8501 ) );
    charTable.insert ( "larr", QChar ( 8592 ) );
    charTable.insert ( "uarr", QChar ( 8593 ) );
    charTable.insert ( "rarr", QChar ( 8594 ) );
    charTable.insert ( "darr", QChar ( 8595 ) );
    charTable.insert ( "harr", QChar ( 8596 ) );
    charTable.insert ( "crarr", QChar ( 8629 ) );
    charTable.insert ( "lArr", QChar ( 8656 ) );
    charTable.insert ( "uArr", QChar ( 8657 ) );
    charTable.insert ( "rArr", QChar ( 8658 ) );
    charTable.insert ( "dArr", QChar ( 8659 ) );
    charTable.insert ( "hArr", QChar ( 8660 ) );
    charTable.insert ( "forall", QChar ( 8704 ) );
    charTable.insert ( "part", QChar ( 8706 ) );
    charTable.insert ( "exist", QChar ( 8707 ) );
    charTable.insert ( "empty", QChar ( 8709 ) );
    charTable.insert ( "nabla", QChar ( 8711 ) );
    charTable.insert ( "isin", QChar ( 8712 ) );
    charTable.insert ( "notin", QChar ( 8713 ) );
    charTable.insert ( "ni", QChar ( 8715 ) );
    charTable.insert ( "prod", QChar ( 8719 ) );
    charTable.insert ( "sum", QChar ( 8721 ) );
    charTable.insert ( "minus", QChar ( 8722 ) );
    charTable.insert ( "lowast", QChar ( 8727 ) );
    charTable.insert ( "radic", QChar ( 8730 ) );
    charTable.insert ( "prop", QChar ( 8733 ) );
    charTable.insert ( "infin", QChar ( 8734 ) );
    charTable.insert ( "ang", QChar ( 8736 ) );
    charTable.insert ( "and", QChar ( 8743 ) );
    charTable.insert ( "or", QChar ( 8744 ) );
    charTable.insert ( "cap", QChar ( 8745 ) );
    charTable.insert ( "cup", QChar ( 8746 ) );
    charTable.insert ( "int", QChar ( 8747 ) );
    charTable.insert ( "there4", QChar ( 8756 ) );
    charTable.insert ( "sim", QChar ( 8764 ) );
    charTable.insert ( "cong", QChar ( 8773 ) );
    charTable.insert ( "asymp", QChar ( 8776 ) );
    charTable.insert ( "ne", QChar ( 8800 ) );
    charTable.insert ( "equiv", QChar ( 8801 ) );
    charTable.insert ( "le", QChar ( 8804 ) );
    charTable.insert ( "ge", QChar ( 8805 ) );
    charTable.insert ( "sub", QChar ( 8834 ) );
    charTable.insert ( "sup", QChar ( 8835 ) );
    charTable.insert ( "nsub", QChar ( 8836 ) );
    charTable.insert ( "sube", QChar ( 8838 ) );
    charTable.insert ( "supe", QChar ( 8839 ) );
    charTable.insert ( "oplus", QChar ( 8853 ) );
    charTable.insert ( "otimes", QChar ( 8855 ) );
    charTable.insert ( "perp", QChar ( 8869 ) );
    charTable.insert ( "sdot", QChar ( 8901 ) );
    charTable.insert ( "lceil", QChar ( 8968 ) );
    charTable.insert ( "rceil", QChar ( 8969 ) );
    charTable.insert ( "lfloor", QChar ( 8970 ) );
    charTable.insert ( "rfloor", QChar ( 8971 ) );
    charTable.insert ( "lang", QChar ( 9001 ) );
    charTable.insert ( "rang", QChar ( 9002 ) );
    charTable.insert ( "loz", QChar ( 9674 ) );
    charTable.insert ( "spades", QChar ( 9824 ) );
    charTable.insert ( "clubs", QChar ( 9827 ) );
    charTable.insert ( "hearts", QChar ( 9829 ) );
    charTable.insert ( "diams", QChar ( 9830 ) );
    return charTable;
}

QString XmlFormatter::escapeSequenceForChar ( const QChar& c ) const {
    static QHash<QChar, QString> t = ( {
        QHash<QString, QChar> a = getEscapeTable();
        QHash<QChar, QString> t;
        QHash<QString, QChar>::iterator it;
        for ( it = a.begin(); it != a.end(); ++it )
            t.insert ( it.value(), it.key() );
        t;
    } );
    if ( t.contains ( c ) )
        return QString ( "&%1;" ).arg ( t[c] );
    return c;
}

QChar XmlFormatter::charForEscapeSequence ( const QString & s ) const {
    static QHash<QString, QChar> t = getEscapeTable();
    return t[s];
}
