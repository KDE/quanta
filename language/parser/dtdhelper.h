/*****************************************************************************
 * Copyright (c) 2010 Ruan Strydom <rm3dom@gmail.com>                        *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 2 of the License, or         *
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

#ifndef IDTDHELPER_H
#define IDTDHELPER_H

#include "parserexport.h"

#include <QtCore/QString>
#include <QtCore/QStringList>

#include <KDE/KMimeType>

namespace Xml {

class KDEVSGMLPARSER_EXPORT AbstractDtdHelper {
public:
    virtual ~AbstractDtdHelper(){}
    /** <!ELEMENT ... - O  .. >
    *  @todo Needs a parent list to correctly identify.
    */
    virtual bool closeOptional(const QString &elementName) const = 0;
    
    /** <!ELEMENT ... EMPTY>
    *  @todo Needs a parent list to correctly identify.
    */
    virtual bool emptyElement(const QString &elementName) const = 0;
    
    /** <!ELEMENT ... CDATA>
    *  @todo Needs a parent list to correctly identify.
    */
    virtual bool cdataElement(const QString &elementName) const = 0;
    
    /** The children this element is allowed to have.
    *  This is used if element close is optional, and it is not empty.
    *  @todo Needs a parent list to correctly identify.
    */
    virtual bool hasChild(const QString &elementName, const QString &child) const = 0;
    
    virtual bool isNull() const = 0;
};

class KDEVSGMLPARSER_EXPORT DtdHelperData : public AbstractDtdHelper, public KShared {
    public:
        DtdHelperData() : AbstractDtdHelper(), KShared() {};
        virtual ~DtdHelperData(){}
};

/** A DTD helper for the SGML tokenizer and SGML parser.
 *  When a DOCTYPE is found in a file this helper will
 *  be used to correctly tokenize and parse the file.
 *  It serves as a facade and abstraction for the tokenizer/parser.
 */
class KDEVSGMLPARSER_EXPORT DtdHelper : public AbstractDtdHelper {
    public:
        DtdHelper();
        DtdHelper(DtdHelperData * helper);
        ~DtdHelper();
        
        /** Factory method for dtd helpers.
         *  Returns an instance of a dtd helper for the specified id's.
         *  The helper may lookup the dtd using ICatalogManager.
         *  @return A helper or NULL if none is found.
         */
        static DtdHelper instance(const QString &publicId = QString(),
                                        const QString &systemId = QString(),
                                        const QString &URI = QString(),
                                        const QString &doctype = QString(),
                                        const KMimeType::Ptr mime = KMimeType::Ptr(),
                                        const KUrl &path = KUrl());

        /** Return the default instance for a specific mime.
         *  @return A helper or null if none is found.
         */
        static DtdHelper instanceForMime(KMimeType::Ptr mime);

        /** Return the default instance for a specific doctype name.
        *   @return A helper or null if none is found.
        */
        static DtdHelper instanceForName(const QString &name);

        inline bool closeOptional(const QString &elementName) const  {
            if(d){return d->closeOptional(elementName);}
            return false;
        }

        inline bool emptyElement(const QString &elementName) const  {
            if(d){return d->emptyElement(elementName);}
            return false;
        }

        inline bool cdataElement(const QString &elementName) const  {
            if(d){return d->cdataElement(elementName);}
            return false;
        }

        inline bool hasChild(const QString &elementName, const QString &child) const  {
            if(d){return d->hasChild(elementName, child);}
            return true;
        }

        inline bool isNull() const  {return d.isNull();}
protected:
    KSharedPtr<DtdHelperData> d;
};

}

#endif // IDTDHELPER_H
