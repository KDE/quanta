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



#ifndef PUBLICID_H
#define PUBLICID_H

#include "kdevxmlcatalogexport.h"

#include <QtCore/QString>

class KDEVXMLCATALOG_EXPORT PublicId {
public:
    /**
    * Normalize a public identifier.
    *
    * <p>Public identifiers must be normalized according to the following
    * rules before comparisons between them can be made:</p>
    *
    * <ul>
    * <li>Whitespace characters are normalized to spaces (e.g., line feeds,
    * tabs, etc. become spaces).</li>
    * <li>Leading and trailing whitespace is removed.</li>
    * <li>Multiple internal whitespaces are normalized to a single
    * space.</li>
    * </ul>
    *
    * <p>This method is declared static so that other classes
    * can use it directly.</p>
    *
    * @param publicId The unnormalized public identifier.
    *
    * @return The normalized identifier.
    */
    static QString normalize ( QString publicId );

    /**
    * Encode a public identifier as a "publicid" URN.
    *
    * <p>This method is declared static so that other classes
    * can use it directly.</p>
    *
    * @param publicId The unnormalized public identifier.
    *
    * @return The normalized identifier.
    */
    static QString encodeURN ( QString publicId );

    /**
    * Decode a "publicid" URN into a public identifier.
    *
    * <p>This method is declared static so that other classes
    * can use it directly.</p>
    *
    * @param urn The urn:publicid: URN
    *
    * @return The normalized identifier.
    */
    static QString decodeURN ( QString urn );
};

#endif // PUBLICID_H
