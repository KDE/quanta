/*
Copyright (C) 2009  Ruan Strydom <ruan@jcell.co.za>

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

#ifndef XMLVALIDATOR_H
#define XMLVALIDATOR_H

#include <QtCore/QStringList>

/** Validates a XML document with a XML schema or DTD using libxml.
 * @todo Should use QT when it supports validation.
 */
class XmlValidator {
public:
    XmlValidator() {}
    ~XmlValidator() {}

    /** Validates a XML document against a schema.
     * @return Returns true if successful
     */
    bool validateSchema ( const QString &documentUrl, const QString &schemaUrl );

    /** Validates a XML document against a DTD.
     * @param dtdUrl If left empty then the document will be validated as is.
     * @return Returns true if successful
     */
    bool validateDTD ( const QString &documentUrl, const QString &dtdUrl = QString() );
protected:
    static void error ( void * ctx, const char * msg, ... );
    static void warn ( void * ctx, const char * msg, ... );
    QStringList m_errors;
    QStringList m_warnings;
};

#endif // XMLVALIDATOR_H
