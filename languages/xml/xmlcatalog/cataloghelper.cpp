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


#include "cataloghelper.h"
#include "icatalogmanager.h"
#include "idocumentcachemanager.h"
#include "publicid.h"


QString CatalogHelper::systemIdForMime(KMimeType::Ptr mime)
{
    if(mime->is("text/html"))
        return resolve("-//W3C//DTD HTML 4.0 Transitional//EN").pathOrUrl();
    if(mime->is("image/svg+xml"))
        return "http://www.w3.org/Graphics/SVG/1.1/DTD/svg10.dtd";
    if(mime->is("application/docbook+xml"))
        return resolve("-//OASIS//DTD DocBook XML V4.5//EN").pathOrUrl();
    //TODO more here
    return QString();
}

KUrl CatalogHelper::resolve(const QString& publicId, const QString& systemId, const QString& URI, const QString& doctype, KMimeType::Ptr mime, const KUrl& path)
{
    QString resolved;
    QString pubId = PublicId::decodeURN(publicId);
    //Does the user have a prefered document?
    if(!pubId.isEmpty() || !systemId.isEmpty())
        resolved = ICatalogManager::self()->resolve(pubId, systemId);
    if(resolved.isEmpty() && !URI.isEmpty())
        resolved = ICatalogManager::self()->resolveUri(URI);
    //No? Use the systemId.
    if(resolved.isEmpty() && !systemId.isEmpty()) {
        resolved = systemId;
    }

    if(resolved.isEmpty() && !doctype.isEmpty()) {
        resolved = ICatalogManager::self()->resolveDoctype(doctype);
    }
    
    if(resolved.isEmpty() && !mime.isNull()) {
        //Get a systemid from the helper
        QString systemId = CatalogHelper::systemIdForMime(mime);
        if(!systemId.isEmpty()) {
            //Does the user have a prefered document?
            resolved = ICatalogManager::self()->resolveSystemId(systemId);
            //No? Use the systemId.
            if(resolved.isEmpty())
                resolved = systemId;
        }
    }

    if(resolved.isEmpty())
        return KUrl();
    
    KUrl url = resolved;
    if(url.isRelative()) {
        QString file = QString("%1/%2").arg(path.directory(), url.fileName());
        url = file;
    }
    
    //Make local
    url = IDocumentCacheManager::self()->cachedUrl(url.pathOrUrl());
    return url;
}

