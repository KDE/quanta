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

#ifndef DOCUMENTCACHEMANAGER_H
#define DOCUMENTCACHEMANAGER_H

#include "idocumentcachemanager.h"

class DocumentCacheUpdateJob : public KJob {
    Q_OBJECT;
public:
    DocumentCacheUpdateJob(const QStringList &urls, QObject* parent = 0);
    virtual ~DocumentCacheUpdateJob();
    virtual void start();
    virtual bool doKill();
protected slots:
    void nextJob(KJob *);
protected:
    long m_persentage;
    long m_numFiles;
    bool m_killed;
    QStringList m_urls;
    KJob * m_currentJob;
};

/**
 * Stores/Caches documents.
 * The purpose of this is so that documents do not need to download again.
 * @note This can not run in a different thread than the GUI.
 */
class DocumentCacheManager : public IDocumentCacheManager
{
    Q_OBJECT
public:
    DocumentCacheManager();
    virtual ~DocumentCacheManager();
    virtual QString cachedUrl(const QString& url, KIO::JobFlags flags = KIO::HideProgressInfo);
    virtual QString getLocalUrl(const QString& url);
    virtual QString updateCache(const QString& url, KIO::JobFlags flags = KIO::HideProgressInfo);
    virtual KJob* createUpdateJob(const QStringList& urls);
};

#endif // DOCUMENTCACHEMANAGER_H
