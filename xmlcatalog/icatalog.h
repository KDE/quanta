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



#ifndef ICATALOG_H
#define ICATALOG_H

#include <QString>

#include "parameterized.h"

class ICatalogEntry;
class ICatalogGroup;

/** The catalog interface.
 *  The ICatalog implementation should contain all the logic to resolve an entry.
 *  ICatalogManager will iterate over all catalogs register when resolving.
 *  The first matching entry will be returend. When reading an ICatalog ICatalogManager
 *  will first try and find a ICatalogReaderWriter which in return will create an
 *  an instance of the ICatalog.<br>
 *  There will probably be two implementations of this, OASIS XML Catalog and SGML Catalog.
 *  @note The catalog manager must always have ownership of this class.
 */
class KDEVXMLCATALOG_EXPORT ICatalog : public virtual Parameterized<QString, QVariant> {
public:
    virtual ~ICatalog();

    /**The method used to resolve will depend on the catalog implementation.*/
    virtual QString resolvePublicId ( const QString & publicId ) const = 0;

    /**The method used to resolve will depend on the catalog implementation.*/
    virtual QString resolveSystemId ( const QString & systemId ) const = 0;

    /**The method used to resolve will depend on the catalog implementation.*/
    virtual QString resolveUri ( const QString & uri ) const = 0;

    /**The method used to resolve will depend on the catalog implementation.*/
    virtual QString resolve ( const QString & publicId, const QString & systemId ) const = 0;

    /**Return all the entries in the catalog regardless of the group.
     * @note Not all catalog formats have groups so this must return all the entries.*/
    virtual QList<ICatalogEntry *> entries() const = 0;

    /** Returns NULL when failed. */
    virtual ICatalogEntry * addPublicEntry(const QString &publicId, const QString &url, const QHash<QString, QVariant> &parameters) = 0;

    /** Returns NULL when failed. */
    virtual ICatalogEntry * addSystemEntry(const QString &systemId, const QString &url, const QHash<QString, QVariant> &parameters) = 0;

    /** Returns NULL when failed. */
    virtual ICatalogEntry * addUriEntry(const QString &uri, const QString &url, const QHash<QString, QVariant> &parameters) = 0;
    
    /** Removes an entry from the catalog. */
    virtual void removeEntry(const ICatalogEntry * entry) = 0;

    /**Return all the groups in the catalog*/
    virtual QList<ICatalogGroup *> groups() const {return QList<ICatalogGroup *>();}
protected:
    explicit ICatalog();
};

#endif // ICATALOG_H
