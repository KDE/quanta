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

#ifndef IDOCUMENTCACHEMANAGER_H
#define IDOCUMENTCACHEMANAGER_H
#include <QString>
#include <QObject>

#include "kdevxmlcatalogexport.h"

#include <KIO/CopyInfo>

class KDEVXMLCATALOG_EXPORT IDocumentCacheManager : public QObject
{
    Q_OBJECT
public:
    virtual ~IDocumentCacheManager();

    static IDocumentCacheManager * self();

    /** Returns the directory where the cache's live.*/
    virtual QString getCacheDirectory() const;

    /** Returns a local url for the remote url.
     *  If the url is already local it will return it as is. */
    virtual QString getLocalUrl(const QString &url) = 0;

    /** Returns a local URL for the remote url.
    *  If the URL is already local it will return it as is.
    *  If it does not exist locally it will make a local copy
    *  synchronously.
    *  @return QString:null if failed.
    */
    virtual QString cachedUrl(const QString &url, KIO::JobFlags flags = KIO::HideProgressInfo) = 0;

    /** This always copies the remote url and deletes the current copy if
     *  exists.
     *  If the URL is already local it will return it as is.
     * @return QString:null if failed.
     */
    virtual QString updateCache(const QString &url, KIO::JobFlags flags = KIO::HideProgressInfo) = 0;

    /** Creates a job to asynchronously download the URL's.*/
    virtual KJob * createUpdateJob(const QStringList &urls) = 0;

protected:
    IDocumentCacheManager();
    static IDocumentCacheManager *instance;
};

#endif // IDOCUMENTCACHEMANAGER_H
