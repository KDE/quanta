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

#ifndef ICATALOGENTRY_H
#define ICATALOGENTRY_H

#include "parameterized.h"

#include <QtCore/QString>

class ICatalogGroup;
class ICatalog;

/** An entry in to the catalog.
 *  ICatalogEntry only contains Public ID, System ID and URI's. Different catalog types may have several other
 *  entries, but these are the common ones.
 */
class KDEVXMLCATALOG_EXPORT ICatalogEntry : public virtual Parameterized<QString, QVariant> {
public:
    virtual ~ICatalogEntry();
    /** The DOCTYPE Public ID of this entry. QString::null if none*/
    virtual QString publicId() const = 0;

    /** The public identifier converted to a URN. QString::null if none*/
    virtual QString URN() const;

    /** The DOCTYPE System ID of this entry or schema location. QString::null if none*/
    virtual QString systemId() const = 0;

    /** The URI of this entry or namespace. QString::null if none*/
    virtual QString URI() const = 0;
    
    /** The location where to find this entries file */
    virtual QString URL() const = 0;
    
    /** Return the group this entry belongs too.
     *  Will return NULL if the catalog does not support groups. */
    virtual ICatalogGroup * group() const {return 0;}

    /** The catalog this entry belongs too. */
    virtual ICatalog * catalog() const = 0;
protected:
    explicit ICatalogEntry();
};

#endif // ICATALOGENTRY_H
