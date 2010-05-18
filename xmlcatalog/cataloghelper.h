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


#ifndef CATALOGHELPER_H
#define CATALOGHELPER_H

#include "kdevxmlcatalogexport.h"

#include "QtCore/QString"

#include <KDE/KMimeType>
#include <KDE/KUrl>

class KDEVXMLCATALOG_EXPORT CatalogHelper
{
public:    
    /** Resolves a document using the catalog and ensure that it is local,
     *  if it is not local it will make a local copy.
     *  If the resolved file is relative it will be maed absolute using path.
     *  @return The resolved document or null.
     */
    static KUrl resolve(const QString &publicId = QString(), 
                           const QString &systemId = QString(), 
                           const QString &URI = QString(), 
                           const QString &doctype = QString(),
                           KMimeType::Ptr mime = KMimeType::Ptr(),
                           const KUrl &path = KUrl());
                           
private:
    /** Returns the default DTD's SystemID for the doctype or null if not exist.
     *  The system id must still be resolved by the catalog afterwards.
     */
    static QString systemIdForDoctype(const QString &doctype);
    /** Returns the default DTD's SystemID for the mime type or null if not exist
     *  The system id must still be resolved by the catalog afterwards.
     */
    static QString systemIdForMime(KMimeType::Ptr mime);
};

#endif // CATALOGHELPER_H
