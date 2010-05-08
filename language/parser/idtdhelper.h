/*
Copyright (C) 2010  Ruan Strydom <ruan@jcell.co.za>

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

#ifndef IDTDHELPER_H
#define IDTDHELPER_H

#include "parserexport.h"

#include <QtCore/QString>
#include <QtCore/QStringList>

#include <KDE/KMimeType>

namespace Xml {

/** A DTD helper for the HTML lexer and HTML parser.
 *  When a DOCTYPE is found in a file this helper will
 *  be used to correctly lex and parse the file.
 *  It serves as a facade and abstraction for the lexer/parser.
 *  @todo Currently only have limited fuctionality, a DTD parser is needed.
 */
class KDEVSGMLPARSER_EXPORT IDtdHelper {
    public:
        virtual ~IDtdHelper();
        /** Factory method for dtd helpers.
         *  Returns an instance of a dtd helper for the specified id's.
         *  The helper may lookup the dtd using ICatalogManager.
         *  @return A helper or NULL if none is found.
         */
        static const IDtdHelper * instance(const QString &publicId, const QString &systemId);

        /** Return the default instance for a specific mime.
         *  @return A helper or NULL if none is found.
         */
        static const IDtdHelper * instanceForMime(KMimeType::Ptr mime);

        /** Return the default instance for a specific doctype name.
        *   @return A helper or 0 if none is found.
        */
        static const IDtdHelper * instanceForName(const QString &name);

        /** <!ELEMENT ... - O  .. >
         *  @todo Needs a the parent list to correctly identify.
         */
        virtual bool closeOptional(const QString &elementName) const = 0;

        /** <!ELEMENT ... EMPTY>
         *  @todo Needs a the parent list to correctly identify.
         */
        virtual bool emptyElement(const QString &elementName) const = 0;

        /** <!ELEMENT ... CDATA>
         *  @todo Needs a the parent list to correctly identify.
         */
        virtual bool cdataElement(const QString &elementName) const = 0;
        
        /** The children this element is allowed to have.
         *  This is used if element close is optional, and it is not empty.
         *  @todo Needs a the parent list to correctly identify.
         */
        virtual bool hasChild(const QString &elementName, const QString &child) const = 0;

    protected:
        IDtdHelper();
};

}

#endif // IDTDHELPER_H
