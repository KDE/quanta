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



#ifndef ICATALOGREADER_H
#define ICATALOGREADER_H
#include "icatalog.h"

#include <QtXml/QDomDocument>

/**
 * The catalog manager must always have ownership of this class.
 */
class KDEVXMLCATALOG_EXPORT ICatalogReaderWriter {
public:
    virtual ~ICatalogReaderWriter();

    virtual ICatalog * readCatalog ( const QString &file ) const = 0;

    virtual bool writeCatalog ( const ICatalog * catalog, const QString &file ) const = 0;

    /** Returns true if this can read the catalog <b>fully</b>. */
    virtual bool accepts ( const QString& file ) const = 0;
    
    /** Returns true if this can read the catalog <b>fully</b>. */
    virtual bool accepts ( const QDomDocument& doc ) const = 0;
    
    /** Returns true if this can write / read the catalog <b>fully</b>. */
    virtual bool accepts ( const ICatalog * catalog ) const = 0;

protected:
    explicit ICatalogReaderWriter();
};

#endif // ICATALOGREADER_H
