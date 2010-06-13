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

#ifndef XMLVALIDATOR_H
#define XMLVALIDATOR_H

#include <QtCore/QStringList>

/** Validates a XML document with a XML schema or DTD using libxml.
 *  @todo Should use QT when it supports validation.
 *  @todo Fix the thread problem.
 *  @note It can not run in a different thread than the GUI.
 */
class XmlValidator {
public:

    enum ValidationResult {Success, Failed, InternalError};
    
    XmlValidator();
    ~XmlValidator();

    /** Validates a XML document against a schema.
     * @return Returns Success if successful.
     */
    ValidationResult validateSchema ( const QString &documentUrl, const QString &schemaUrl );

    /** Validates a XML document against a DTD.
     * @param dtdUrl If left empty then the document will be validated as is.
     * @return Returns Success if successful.
     */
    ValidationResult validateDTD ( const QString &documentUrl, const QString &dtdUrl = QString() );

    const QStringList & errors() const {return m_errors; }
    const QStringList & warnings() const {return m_warnings; }
    
protected:
    static void errorMessage ( void * ctx, const char * msg, ... );
    static void warnMessage ( void * ctx, const char * msg, ... );
    QStringList m_errors;
    QStringList m_warnings;
    //The catalogs should only be loaded once
    static int m_refCount;
};

#endif // XMLVALIDATOR_H
