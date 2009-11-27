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


#include "xmlvalidator.h"

#include "libxml2/libxml/xmlschemas.h"
#include <stdio.h>
#include <stdarg.h>

#include <QtCore/QDebug>

#include <KDE/KUrl>
#include <KDE/KMessageBox>
#include <KDE/KLocalizedString>


void XmlValidator::error ( void* ctx, const char* msg, ... ) {
    if ( ctx == NULL || msg == NULL || strlen ( msg ) < 1 )
        return;
    XmlValidator *v = static_cast<XmlValidator *> ( ctx );
    va_list vl;
    va_start ( vl, msg );
    char errorStr[2048];
    vsprintf ( errorStr, msg, vl );
    v->m_errors.append ( errorStr );
    va_end ( vl );
}

void XmlValidator::warn ( void* ctx, const char* msg, ... ) {
    if ( ctx == NULL || msg == NULL )
        return;
    XmlValidator *v = static_cast<XmlValidator *> ( ctx );
    va_list vl;
    va_start ( vl, msg );
    char errorStr[2048];
    vsprintf ( errorStr, msg, vl );
    v->m_warnings.append ( errorStr );
    va_end ( vl );
}

bool XmlValidator::validateSchema ( const QString &documentUrl, const QString &schemaUrl ) {
    if ( documentUrl.isEmpty() || schemaUrl.isEmpty() )
        return false;
    m_errors.clear();
    m_warnings.clear();

    int result = 0;

    xmlSchemaParserCtxtPtr parserCtxPtr = xmlSchemaNewParserCtxt ( KUrl ( schemaUrl ).toEncoded().constData() );
    if ( !parserCtxPtr ) {
        qDebug() << "Unable to create xmlSchemaParserCtxtPtr";
        return false;
    }
    xmlSchemaValidityErrorFunc errorFunc = &XmlValidator::error;
    xmlSchemaValidityWarningFunc warnFunc = &XmlValidator::warn;

    xmlSchemaSetParserErrors ( parserCtxPtr, errorFunc, warnFunc, this );

    xmlSchemaPtr schemaPtr = xmlSchemaParse ( parserCtxPtr );
    if ( !schemaPtr ) {
        KMessageBox::information ( 0, QString ( "%1\n%2" ).arg ( i18n ( "Failed: Errors" ) ).arg ( m_errors.join ( "\n" ) ) );
        xmlSchemaFreeParserCtxt ( parserCtxPtr );
        return false;
    }

    xmlSchemaValidCtxtPtr validContextPtr = xmlSchemaNewValidCtxt ( schemaPtr );
    if ( !validContextPtr || xmlSchemaIsValid ( validContextPtr ) != 1 ) {
        KMessageBox::information ( 0, QString ( "%1\n%2" ).arg ( i18n ( "Failed: Errors" ) ).arg ( m_errors.join ( "\n" ) ) );
        xmlSchemaFree ( schemaPtr );
        xmlSchemaFreeParserCtxt ( parserCtxPtr );
        return false;
    }

    xmlSchemaSetValidErrors ( validContextPtr, errorFunc, warnFunc, this );

    result = xmlSchemaValidateFile ( validContextPtr, KUrl ( documentUrl ).toEncoded().constData(), 0 );

    if ( result == -1 )
        KMessageBox::error ( 0, QString ( "%1\n%2" ).arg ( i18n ( "Failed: Errors:" ) ).arg ( m_errors.join ( "\n" ) ) );
    else if ( result )
        KMessageBox::error ( 0, QString ( "%1\n%2" ).arg ( i18n ( "Failed: XML is invalid" ) ).arg ( m_errors.join ( "\n" ) ) );
    else
        KMessageBox::information ( 0, i18n ( "Success: XML is valid" ) );

    xmlSchemaFree ( schemaPtr );
    xmlSchemaFreeParserCtxt ( parserCtxPtr );
    xmlSchemaFreeValidCtxt ( validContextPtr );

    return true;
}

bool XmlValidator::validateDTD ( const QString& documentUrl, const QString& dtdUrl ) {
    if ( documentUrl.isEmpty() )
        return false;
    m_errors.clear();
    m_warnings.clear();

    int result = 0;
    xmlDocPtr docPtr = 0;

    if ( !dtdUrl.isEmpty() ) {
        docPtr = xmlParseFile ( documentUrl.toLatin1().constData() );
        if ( !docPtr ) {
            return false;
        }

        xmlValidityErrorFunc errorFunc = &XmlValidator::error;
        xmlValidityWarningFunc warnFunc = &XmlValidator::warn;

        xmlValidCtxt cvp;
        cvp.error = errorFunc;
        cvp.warning  = warnFunc;
        cvp.userData = ( void * ) this;

        xmlDtdPtr dtdPtr = 0;
        dtdPtr = xmlParseDTD ( NULL, ( xmlChar * ) dtdUrl.toLatin1().constData() );
        if ( dtdPtr ) {
            result = xmlValidateDtd ( &cvp, docPtr, dtdPtr );
            xmlFreeDtd ( dtdPtr );
        }
    } else {
        xmlParserCtxtPtr ctxt;
        xmlDocPtr docPtr;
        ctxt = xmlNewParserCtxt();
        if ( !ctxt )
            return false;
        docPtr = xmlCtxtReadFile ( ctxt, documentUrl.toLatin1().constData(), NULL, XML_PARSE_DTDVALID );
        result = ctxt->valid;
        m_errors.append ( QString ( "%1:%2 %3" ).arg ( ctxt->lastError.line ).arg ( ctxt->lastError.int2 ).arg ( ctxt->lastError.message ) );
        xmlFreeParserCtxt ( ctxt );
    }

    if ( result == -1 )
        KMessageBox::error ( 0, QString ( "%1\n%2" ).arg ( i18n ( "Failed: Errors" ) ).arg ( m_errors.join ( "\n" ) ) );
    else if ( !result )
        KMessageBox::error ( 0, QString ( "%1\n%2" ).arg ( i18n ( "Failed: XML is invalid" ) ).arg ( m_errors.join ( "\n" ) ) );
    else
        KMessageBox::information ( 0, i18n ( "Success: XML is valid" ) );
    if ( docPtr )
        xmlFreeDoc ( docPtr );
    return true;
}

