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

#ifndef CATALOGMANAGER_H
#define CATALOGMANAGER_H

#include "icatalogmanager.h"

class CatalogManager : public ICatalogManager {
  Q_OBJECT
public:
    CatalogManager();
    virtual ~CatalogManager();

    virtual QString resolveSystemId ( const QString& systemId ) const;
    virtual QString resolvePublicId ( const QString& publicId ) const;
    virtual QString resolveUri(const QString& uri) const;
    virtual QString resolve ( const QString& publicId, const QString& systemId ) const;

    virtual QList< ICatalog* > catalogs() const {return m_ctlg;}

    virtual bool addCatalog ( const QString& file );

    virtual bool removeCatalog ( const QString& file );

    virtual QStringList catalogFileList() const;

    virtual const ICatalogReaderWriter* readerForContent ( const QString& file ) const;
    virtual const ICatalogReaderWriter* writer(const ICatalog* catalog) const;
    
public slots:
    virtual bool save() const;
    virtual bool load();
protected:
    QList<ICatalogReaderWriter *> m_rw;
    QList<ICatalog *> m_ctlg;
};

#endif // CATALOGMANAGER_H
