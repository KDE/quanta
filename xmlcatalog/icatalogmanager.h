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


#ifndef ICATALOGMANAGER_H
#define ICATALOGMANAGER_H

#include "icatalog.h"
#include "icatalogreaderwriter.h"

/** Manages the catalogs, loads, saves and resolves.
 *  This should have fair support for Xml Catalogs and SGML Catalogs.
 *  The ICatalog implementation should contain all the logic to resolve an entry.
 *  ICatalogManager will iterate over all catalogs register when resolving.
 *  The first matching entry will be returend. When reading an ICatalog ICatalogManager
 *  will first try and find a ICatalogReaderWriter which in return will create an
 *  an instance of the ICatalog.<br>
 */
class KDEVXMLCATALOG_EXPORT ICatalogManager : public QObject {
  Q_OBJECT
public:
    /** Added to the catalog to keep track of its location. */
    static const QString ParameterFile;
    
    /** Added to the catalog - true or false. */
    static const QString ParameterReadonly;

    /** Returns the ICatalogManager implemented singleton.*/
    static ICatalogManager * self();
    
    virtual ~ICatalogManager();
	
    /** Resolves an entry.
     *  The method used to resolve will depend on the catalog implementation.
     *  This should iterate over all the catalogs loaded and return the first best match.
     */
    virtual QString resolvePublicId ( const QString & publicId ) const = 0;
    
    /** Resolves an entry.
    *  The method used to resolve will depend on the catalog implementation.
    *  This should iterate over all the catalogs loaded and return the first best match.
    */
    virtual QString resolveSystemId ( const QString & systemId ) const = 0;
    
    /** Resolves an entry.
    *  The method used to resolve will depend on the catalog implementation.
    *  This should iterate over all the catalogs loaded and return the first best match.
    */
    virtual QString resolveUri ( const QString & uri ) const = 0;
    
    /** Resolves an entry.
    *  The method used to resolve will depend on the catalog implementation.
    *  This should iterate over all the catalogs loaded and return the first best match.
    */
    virtual QString resolve ( const QString & publicId, const QString & systemId ) const = 0;
    
    virtual QList<ICatalog *> catalogs() const = 0;
    virtual bool addCatalog(const QString &file) = 0;
    virtual bool removeCatalog(const QString &file) = 0;
    virtual QStringList catalogFileList() const = 0;
    
    virtual const ICatalogReaderWriter * readerForContent ( const QString & file ) const = 0;
    virtual const ICatalogReaderWriter * writer ( const ICatalog * catalog ) const = 0;

    virtual QString catalogDirectory() const;

public slots:
    virtual bool save() const = 0;
    virtual bool load() = 0;
protected:
    explicit ICatalogManager();
    static ICatalogManager * instance;
};

#endif // ICATALOGMANAGER_H
