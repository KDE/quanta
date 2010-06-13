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

#ifndef SGMLCATALOG_H
#define SGMLCATALOG_H

#include "icatalog.h"
#include "icatalogentry.h"
#include "icataloggroup.h"

class SGMLCatalogEntry;
class SGMLCatalogReaderWriter;


/** An quick and dirty implementation of SGML Catalog
 *  This does not support a number of features:
 *  <ul>
 *  <li>CATALOG: Will not include other catalogs, these must be added individualy.</li>
 *  <li>OVERRIDE: Public is always prefered.</li>
 *  <li>ENTITY: Will not parse, replace with value.</li>
 *  <li>LINKTYPE: Will not parse, replace with value.</li>
 *  <li>NOTATION</li>
 *  <li>DELEGATE: Will not delegate to other catalogs.</li>
 *  </ul>
 *  @note This must be the owner of its children, as it will delete them
 *  @todo Implement all features
 */
class SGMLCatalog : public ICatalog {
friend class SGMLCatalogReaderWriter;
public:
    SGMLCatalog();
    virtual ~SGMLCatalog();
    virtual ICatalogEntry* addPublicEntry(const QString& publicId, const QString& url, const QHash< QString, QVariant >& parameters);
    virtual ICatalogEntry* addSystemEntry(const QString& systemId, const QString& url, const QHash< QString, QVariant >& parameters);
    virtual ICatalogEntry* addDoctypeEntry(const QString& doctype, const QString& url, const QHash< QString, QVariant >& parameters);
    virtual ICatalogEntry* addUriEntry(const QString& uri, const QString& url, const QHash< QString, QVariant >& parameters) {
        Q_UNUSED(uri); Q_UNUSED(url); Q_UNUSED(parameters);
        return 0;
    }
    virtual void removeEntry(ICatalogEntry* entry);
    virtual QList< ICatalogEntry* > entries() const;
    virtual QString resolve(const QString& publicId, const QString& systemId) const;
    virtual QString resolvePublicId(const QString& publicId) const;
    virtual QString resolveSystemId(const QString& systemId) const;
    virtual QString resolveUri(const QString& uri) const;
    virtual QString resolveDoctype(const QString& doctype) const;
    QString file() const {return m_file;}
protected:
    QString makeAbsolute(QString file) const;
    /** Contains the actual entries sorted by line */
    QList< SGMLCatalogEntry* > m_entries;
    QString m_file;
    QString m_base;
    //Not used but added if true the public override system
    bool m_override;
    QString m_fileContents;
};

class SGMLCatalogEntry: public ICatalogEntry {
friend class SGMLCatalog;
friend class SGMLCatalogReaderWriter;
public:
    SGMLCatalogEntry() : m_catalog(0), m_charStart(-1), m_charEnd(-1), m_deleted(false) {}
    virtual ICatalog* catalog() const {return m_catalog;}
    virtual QString publicId() const {return m_publicId;}
    virtual QString systemId() const {return m_systemId;}
    virtual QString URI() const {return m_uri;}
    virtual QString doctype() const {return m_doctype;}
    virtual QString URL() const {return m_url;}
protected:
    SGMLCatalog * m_catalog;
    QString m_publicId;
    QString m_systemId;
    QString m_doctype;
    QString m_uri;
    QString m_url;
    qint64 m_charStart;
    qint64 m_charEnd;
    //If this entry is deleted 
    bool m_deleted;
};


#endif //SGMLCATALOG_H
