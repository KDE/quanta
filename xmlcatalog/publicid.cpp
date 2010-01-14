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

#include "publicid.h"


QString PublicId::normalize ( QString publicId ) {
    if(publicId.isNull())
        return QString::null;
    
    QString normal = publicId.replace ( '\t', ' ' );
    normal = normal.replace ( '\r', ' ' );
    normal = normal.replace ( '\n', ' ' );
    normal = normal.trimmed();

    int pos;

    while ( ( pos = normal.indexOf ( "  " ) ) >= 0 ) {
        normal = normal.left ( pos ) + normal.right ( normal.length() - pos + 1 );
    }

    return normal;
}

QString PublicId::encodeURN ( QString publicId ) {
    if(publicId.isNull())
        return QString::null;
    
    QString urn = PublicId::normalize ( publicId );

    urn = urn.replace ( "%", "%25" );
    urn = urn.replace ( ";", "%3B" );
    urn = urn.replace ( "'", "%27" );
    urn = urn.replace ( "?", "%3F" );
    urn = urn.replace ( "#", "%23" );
    urn = urn.replace ( "+", "%2B" );
    urn = urn.replace ( " ", "+" );
    urn = urn.replace ( "::", ";" );
    urn = urn.replace ( ":", "%3A" );
    urn = urn.replace ( "//", ":" );
    urn = urn.replace ( "/", "%2F" );

    return "urn:publicid:" + urn;
}

QString PublicId::decodeURN ( QString urn ) {
    if(urn.isNull())
        return QString::null;
    
    QString publicId = "";

    if ( urn.startsWith ( "urn:publicid:" ) ) {
        publicId = urn.right ( urn.length() - 13 );
    } else {
        return urn;
    }

    publicId = publicId.replace( "%2F", "/" );
    publicId = publicId.replace( ":", "//" );
    publicId = publicId.replace( "%3A", ":" );
    publicId = publicId.replace( ";", "::" );
    publicId = publicId.replace( "+", " " );
    publicId = publicId.replace( "%2B", "+" );
    publicId = publicId.replace( "%23", "#" );
    publicId = publicId.replace( "%3F", "?" );
    publicId = publicId.replace( "%27", "'" );
    publicId = publicId.replace( "%3B", ";" );
    publicId = publicId.replace( "%25", "%" );

    return publicId;
}
