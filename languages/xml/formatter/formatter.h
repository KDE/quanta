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

#ifndef FORMATTER_H
#define FORMATTER_H

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QVariant>
#include <KMimeType>

namespace Xml
{

/** Common formatter interface */
class Formatter
{
public:
    virtual ~Formatter(){}
    
    virtual QString formatSource ( const QString& text, const QString& leftContext = QString(), const QString& rightContext = QString() ) = 0;
    
    virtual void setDebugEnabled(bool enabled) = 0;
    
    /** Escapes unsafe characters.
    */
    virtual QString escapeSource ( const QString& text ) = 0;
    
    /** Removes escape sequences.
    */
    virtual QString unescapeSource ( const QString& text ) = 0;
    
    /** Returns characters for strings like &(amp);
    */
    virtual QChar charForEscapeSequence ( const QString & s ) const = 0;
    
    /** Returns a string like &(amp); for '&'
    */
    virtual QString escapeSequenceForChar ( const QChar & c ) const = 0;
    
    /** Removes all whites
    */
    virtual QString compactSource ( const QString& text ) = 0;
    
    /** Loads a formatting style.
    */
    virtual void loadStyle ( const QString &content ) = 0;
    
    virtual const QMap<QString, QVariant> & options() const = 0;
    
    virtual QMap<QString, QVariant> & options() = 0;
    
    virtual QString saveStyle() = 0;

    virtual void setMime(KMimeType::Ptr mime) = 0;

    static QMap<QString, QVariant> stringToOptionMap(const QString &str) {
        QMap<QString, QVariant> map;
        if ( str.isEmpty() )
            return map;
        QStringList pairs = str.split(',', QString::SkipEmptyParts);
        QStringList::const_iterator it;
        for (it = pairs.constBegin(); it != pairs.constEnd(); ++it) {
            QStringList bits = (*it).split('=');
            if(bits.size() == 2)
                map[bits[0].trimmed()] = bits[1];
        }
        return map;
    }
};

}

#endif // FORMATTER_H
