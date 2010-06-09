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

#ifndef SGMLFORMATTER_H_
#define SGMLFORMATTER_H_

#include <dtdhelper.h>

#include "formatter.h"


namespace Xml
{

/** A simple formatter for SGML
 *  This uses the @see DtdHelper to help with indentation.
 */
class SgmlFormatter : public Formatter {
public:
    SgmlFormatter();
    virtual ~SgmlFormatter();

    virtual QString formatSource ( const QString& text, const QString& leftContext = QString(), const QString& rightContext = QString() );

    virtual void setDebugEnabled(bool enabled) {m_debugEnabled = enabled;};

    virtual QString escapeSource ( const QString& text );

    virtual QString unescapeSource ( const QString& text );

    virtual QChar charForEscapeSequence ( const QString & s ) const;

    virtual QString escapeSequenceForChar ( const QChar & c ) const;
    
    virtual QString compactSource ( const QString& text );

    virtual void loadStyle ( const QString &content );

    virtual const QMap<QString, QVariant> & options() const {
        return m_options;
    }

    virtual QMap<QString, QVariant> & options() {
        return m_options;
    }

    virtual QString saveStyle();

    virtual void setMime(KMimeType::Ptr mime) {
        m_mime = mime;
    }

protected:
    KMimeType::Ptr m_mime;
    bool m_debugEnabled;
    QMap<QString, QVariant> m_options;
    QString m_content;
    const QChar *m_contentData;
    qint64 m_contentDataLength;
    qint64 m_contentDataIndex;
    qint64 m_lineCount;
    DtdHelper m_dtdHelper;

    enum TokenType {Error, Processing, ClosedElement, StartElement, EndElement, DTD, Comment, Text, Other, EndOfTokenType};
    virtual QStringRef nextToken(int *tokenType);
    virtual QStringRef readCdataElement(const QString &name);

    enum ReadFlag {IgnoreNone, IgnoreWhites, IgnoreCase, EndOfReadFlag};
    
    /** A pattern similar to the expression .*blah
    *  (but the first occurrence blah; not the last)
    *  @return The character after condition.
    */
    const QChar * readUntill(const QChar *start, const QChar *end, const QString& condition , int flag, const QChar** conditionStart);
    
    /** A pattern similar to the expression [^blah]*
    *  @return The first occurrence of condition.
    */
    const QChar * readUntillAny(const QChar *start, const QChar *end, const QString& condition, int flag);
    
    /** A pattern similar to the expression [blah]*
    *  @return The character after condition.
    */
    const QChar * readWhileAny(const QChar *start, const QChar *end, const QString& condition, int flag, const QChar** conditionStart);
    
    QString removeWhites(QString str) const;
    
    QString elementName(const QString& token) const;

    void doctype(const QString& token, QString& name, QString& publicId, QString& systemId) const;

    void createNewline(int pos);
    
    QString tokenTypeString ( TokenType type ) const;
    
    virtual QString cdataHook(const QString &element, const QString &cdata, const QString &indent){return cdata;};

    //Expects the whole tag <blah ...>
    virtual QString formatTag(const QString &element, const QString& newLineIndent, const QString& indent) const;
    
    virtual QString formatText(const QString &text, const QString& indent, int *hasNewLine ) const;

    virtual QStringList formatInlineSource(const QString &text, const KMimeType::Ptr mime) const;
    
    QString indentString ( int depth, int indent ) const;

    QString leftTrim(const QString &str) const;
};

}
#endif //SGMLFORMATTER_H_

