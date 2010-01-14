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


#ifndef ICATALOGGROUP_H
#define ICATALOGGROUP_H

#include "parameterized.h"

#include <QtCore/QHash>

class ICatalogEntry;
class ICatalog;

/** An group in the catalog.
 *  If the catalog does not support groups then this will not be used and catalog and entry should return 0 for groups.
 */
class KDEVXMLCATALOG_EXPORT ICatalogGroup : public virtual Parameterized<QString, QVariant> {
public:
    virtual ~ICatalogGroup();

    /** Return all the entries in the catalog group. */
    virtual QList<ICatalogEntry *> entries() const = 0;

    /** Returns the parent catalog of this group. */
    virtual ICatalog * catalog() const = 0;

    /** Adds an entry to the group. Returns NULL if failed. */
    virtual ICatalogEntry * addPublicEntry(const QString &publicId, const QString &url, const QHash<QString, QVariant> &parameters) = 0;

    /** Adds an entry to the group. Returns NULL if failed. */
    virtual ICatalogEntry * addSystemEntry(const QString &systemId, const QString &url, const QHash<QString, QVariant> &parameters) = 0;
    
    /** Adds an entry to the group. Returns NULL if failed. */
    virtual ICatalogEntry * addUriEntry(const QString &uri, const QString &url, const QHash<QString, QVariant> &parameters) = 0;

    /** Removes an entry from the group. */
    virtual void removeEntry(const ICatalogEntry * entry) = 0;

protected:
    explicit ICatalogGroup();
};

#endif // ICATALOGGROUP_H
