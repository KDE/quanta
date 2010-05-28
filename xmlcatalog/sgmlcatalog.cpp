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

#include "sgmlcatalog.h"
#include "publicid.h"

#include <KDE/KUrl>
#include <KDE/KDebug>

SGMLCatalog::SGMLCatalog() : ICatalog(), m_override(true)
{}

SGMLCatalog::~SGMLCatalog()
{
    foreach(ICatalogEntry *e, m_entries)
        delete e;
}


QList< ICatalogEntry* > SGMLCatalog::entries() const
{
    QList< ICatalogEntry* > list;
    foreach(SGMLCatalogEntry *e, m_entries) {
        if (e && !e->m_deleted)
            list.append(e);
    }
    return list;
}

ICatalogEntry* SGMLCatalog::addPublicEntry(const QString& publicId, const QString& url, const QHash< QString, QVariant >& parameters)
{
    if (publicId.isNull() || url.isNull())
        return 0;
    SGMLCatalogEntry *e = new SGMLCatalogEntry();
    e->m_publicId = publicId;
    e->m_url = url;
    e->m_catalog = this;
    foreach(QString key, parameters.keys()) {
        e->setParameter(key, parameters.value(key));
    }
    m_entries.append(e);
    return e;
}

ICatalogEntry* SGMLCatalog::addSystemEntry(const QString& systemId, const QString& url, const QHash< QString, QVariant >& parameters)
{
    if (systemId.isNull() || url.isNull())
        return 0;
    SGMLCatalogEntry *e = new SGMLCatalogEntry();
    e->m_systemId = systemId;
    e->m_url = url;
    e->m_catalog = this;
    foreach(QString key, parameters.keys()) {
        e->setParameter(key, parameters.value(key));
    }
    m_entries.append(e);
    return e;
}

ICatalogEntry* SGMLCatalog::addDoctypeEntry(const QString& doctype, const QString& url, const QHash< QString, QVariant >& parameters)
{
    if (doctype.isNull() || url.isNull())
        return 0;
    SGMLCatalogEntry *e = new SGMLCatalogEntry();
    e->m_doctype = doctype;
    e->m_url = url;
    e->m_catalog = this;
    foreach(QString key, parameters.keys()) {
        e->setParameter(key, parameters.value(key));
    }
    m_entries.append(e);
    return e;
}

void SGMLCatalog::removeEntry(ICatalogEntry* entry)
{
    SGMLCatalogEntry *e = dynamic_cast<SGMLCatalogEntry *>(entry);
    if (e)
        e->m_deleted = true;
}

QString SGMLCatalog::makeAbsolute(QString file) const
{
    KUrl url(file);
    if (url.isRelative()) {
        if (!m_base.isEmpty()) {
            if (m_base.endsWith('/'))
                return QString("%1%2").arg(m_base, file);
            else
                return QString("%1/%2").arg(m_base, file);
        } else {
            KUrl catUrl(m_file);
            return QString("%1/%2").arg(catUrl.directory(), file);
        }
    }
    return file;
}

QString SGMLCatalog::resolve(const QString& publicId, const QString& systemId) const
{
    QString url;
    if (!publicId.isNull())
        url = resolvePublicId(publicId);
    if (url.isNull())
        url = resolveSystemId(systemId);
    return url;
}

QString SGMLCatalog::resolvePublicId(const QString& publicId) const
{
    QString pubId = PublicId::decodeURN(publicId);
    foreach(SGMLCatalogEntry *e, m_entries) {
        if (e && !e->m_deleted && !e->publicId().isEmpty() && e->publicId() == pubId) {
            return makeAbsolute(e->URL());
        }
    }
    return QString();
}

QString SGMLCatalog::resolveSystemId(const QString& systemId) const
{
    foreach(SGMLCatalogEntry *e, m_entries) {
        if (e && !e->m_deleted && !e->systemId().isEmpty() && e->systemId() == systemId) {
            return makeAbsolute(e->URL());
        }
    }
    return QString();
}

QString SGMLCatalog::resolveDoctype(const QString& doctype) const
{
    foreach(SGMLCatalogEntry *e, m_entries) {
        if (e && !e->m_deleted && !e->doctype().isEmpty() && e->doctype().toLower() == doctype.toLower()) {
            return makeAbsolute(e->URL());
        }
    }
    return QString();
}

QString SGMLCatalog::resolveUri(const QString& uri) const
{
    foreach(SGMLCatalogEntry *e, m_entries) {
        if (e && !e->m_deleted && !e->URI().isEmpty() && e->URI() == uri) {
            return makeAbsolute(e->URL());
        }
    }
    return QString();
}


