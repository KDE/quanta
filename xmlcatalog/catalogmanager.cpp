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

#include "catalogmanager.h"
#include "oasiscatalog.h"
#include "oasiscatalogreaderwriter.h"

#include <QtCore/QFile>

#include <KDE/KMimeType>
#include <KDE/KFile>
#include <KDE/KDebug>
#include <KDE/KLocalizedString>

#define debug() kDebug()

CatalogManager::CatalogManager() {
    m_rw.append(new OASISCatalogReaderWriter);
    load();
}

CatalogManager::~CatalogManager() {
    for ( int i = 0; i < m_ctlg.size(); i++ )
        delete m_ctlg[i];
    for ( int i = 0; i < m_rw.size(); i++ )
        delete m_rw[i];
}

const ICatalogReaderWriter* CatalogManager::writer(const ICatalog* catalog) const
{
    if(!catalog)
        return 0;
    for ( int i = 0; i < m_rw.size(); i++ ) {
        if ( m_rw[i] && m_rw[i]->accepts(catalog) )
            return m_rw[i];
    }
    return 0;
}

const ICatalogReaderWriter* CatalogManager::readerForContent ( const QString& file ) const {
    for ( int i = 0; i < m_rw.size(); i++ ) {
        if ( m_rw[i] && m_rw[i]->accepts ( file ) )
            return m_rw[i];
    }
    return 0;
}


QString CatalogManager::resolve ( const QString& publicId, const QString& systemId ) const {
    QString r;
    for ( int i = 0; i < m_ctlg.size(); i++ ) {
        if ( m_ctlg[i] )
            r = m_ctlg[i]->resolve ( publicId, systemId );
        if ( !r.isEmpty() )
            return r;
    }
    return QString::null;
}

QString CatalogManager::resolvePublicId ( const QString& publicId ) const {
    if (publicId.isEmpty())
        return QString::null;
    QString r;
    for ( int i = 0; i < m_ctlg.size(); i++ ) {
        if ( m_ctlg[i] )
            r = m_ctlg[i]->resolvePublicId ( publicId );
        if ( !r.isEmpty() )
            return r;
    }
    return QString::null;
}

QString CatalogManager::resolveSystemId ( const QString& systemId ) const {
    if (systemId.isEmpty())
        return QString::null;
    QString r;
    for ( int i = 0; i < m_ctlg.size(); i++ ) {
        if ( m_ctlg[i] )
            r = m_ctlg[i]->resolveSystemId ( systemId );
        if ( !r.isEmpty() )
            return r;
    }
    return QString::null;
}

QString CatalogManager::resolveUri(const QString& uri) const
{
    if (uri.isEmpty())
        return QString::null;
    QString r;
    for ( int i = 0; i < m_ctlg.size(); i++ ) {
        if ( m_ctlg[i] )
            r = m_ctlg[i]->resolveUri ( uri );
        if ( !r.isEmpty() )
            return r;
    }
    return QString::null;
}

bool CatalogManager::addCatalog ( const QString& file ) {
    if (file.isEmpty())
        return false;

    QFile f ( file );

    if(!f.exists()) {
        debug() << "Catalog does not exist:" << file;
        return false;
    }

    foreach(ICatalog *c, m_ctlg) {
        if(c->parameter(ParameterFile) == file)
            return true;
    }

    const ICatalogReaderWriter * rw = readerForContent ( file );
    if ( !rw ) {
        debug() << "No reader for content:" << file;
        return false;
    }
    ICatalog *c = rw->readCatalog ( file );
    if ( !c ) {
        debug() << "Unable to create catalog for:" << file;
        return false;
    }
    c->setParameter(ICatalogManager::ParameterFile, file);

    if (f.permissions() & QFile::WriteUser)
        c->setParameter(ICatalogManager::ParameterReadonly, false);
    else
        c->setParameter(ICatalogManager::ParameterReadonly, true);
    
    m_ctlg.append ( c );
    
    return true;
}

bool CatalogManager::removeCatalog ( const QString& file ) {
    if (file.isEmpty())
        return false;
    for (int i = 0; i < m_ctlg.size(); i++) {
        if (m_ctlg[i] && m_ctlg[i]->parameter(ICatalogManager::ParameterFile) == file) {
            delete m_ctlg.takeAt(i);
            return true;
        }
    }
    return false;
}


bool CatalogManager::load() {
    QFile f(catalogDirectory() + "config.xml");
    if (!f.exists()) {
        debug() << "File does not exist: " << f.fileName();
        return false;
    }
    if (!f.open(QIODevice::ReadOnly)) {
        debug() << "Unable to open file: " << f.fileName();
        return false;
    }
    QDomDocument doc;
    if (!doc.setContent(&f)) {
        debug() << "Unable to parse file: " << f.fileName();
        return false;
    }
    QDomNodeList nl = doc.elementsByTagName("catalog");
    for (int i = 0; i < nl.size(); i++) {
        QDomElement e = nl.at(i).toElement();
        if (e.isNull() || e.nodeName() != "catalog")
            continue;
        QString catalog = e.attribute("file");
        if (!addCatalog(catalog)) {
            debug() << "Unable load catalog: " << catalog;
            continue;
        }
    }
    if(m_ctlg.isEmpty()) {
        OASISCatalog * catalog = new OASISCatalog();
        catalog->setParameter(ICatalogManager::ParameterFile, catalogDirectory() + "catalog.xml");
        catalog->setParameter(ICatalogManager::ParameterReadonly, false);
        m_ctlg.append(catalog);
    }
    return true;
}

bool CatalogManager::save() const {
    QFile f(catalogDirectory() + "config.xml");
    if (!f.open(QIODevice::WriteOnly)) {
        debug() << "Unable to open file: " << f.fileName();
        return false;
    }
    QTextStream stream(&f);
    stream << "<catalogs>\n";
    foreach(ICatalog *c, m_ctlg) {
        if(!c)
            continue;
        QString catalogFile = c->parameter(ICatalogManager::ParameterFile).toString();
        if (catalogFile.isEmpty()) {
            debug() << "No file parameter for catalog";
            continue;
        }
        stream << QString("  <catalog file=\"%1\"/>\n").arg(catalogFile);
        if(!c->parameter(ICatalogManager::ParameterReadonly).toBool()) {
            writer(c)->writeCatalog(c, catalogFile);
        }
    }
    stream << "</catalogs>\n";
    f.close();
    return true;
}


QStringList CatalogManager::catalogFileList() const
{
    QStringList catalogs;
    foreach(ICatalog *catalog, m_ctlg ) {
        if(!catalog)
            continue;
        catalogs.append(catalog->parameter(ICatalogManager::ParameterFile).toString());
    }
    return catalogs;
}


#include "catalogmanager.moc"

