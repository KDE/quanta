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

#ifndef XMLFORMATTER_H_
#define XMLFORMATTER_H_

#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <QtCore/QMap>

class XmlFormatter {
public:
    XmlFormatter();

    /** Adds whites to make the xml more readable
    */
    QString formatSource ( const QString& text, const QString& leftContext = QString(), const QString& rightContext = QString() ) const;
    
    /** Removes all whites
     */
    QString compactSource ( const QString& text ) const;

    /** Escapes unsafe characters.
    */
    QString escapeSource ( const QString& text ) const;

    /** Removes escape sequences.
     */
    QString unescapeSource ( const QString& text ) const;

    /** Returns characters for strings like &(amp);
     */
    QChar charForEscapeSequence ( const QString & s ) const;

    /** Returns a string like &(amp); for '&'
     */
    QString escapeSequenceForChar ( const QChar & c ) const;

    /** Loads a formatting style.
     */
    void loadStyle ( const QString &content );

    const QMap<QString, QVariant> & options() const {
        return m_options;
    }
    QMap<QString, QVariant> & options() {
        return m_options;
    }
    QString saveStyle();
private:
    enum TokenType {None, Error, Processing, ClosedElement, StartElement, EndElement, CDATA, DTD, Comment, Text};
    QString nextToken ( QTextStream &stream, TokenType *type) const;
    QString readAndValidateUntill ( QTextStream & stream, const QString& condition, int *error ) const;
    QString readUntill ( QTextStream & stream, const QString& condition ) const;
    QString tokenText ( TokenType type ) const;
    QMap<QString, QVariant> m_options;
};

#endif //XMLFORMATTER_H_

