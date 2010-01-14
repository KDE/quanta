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

#include "documentcachemanager.h"

#include <QtCore/QFile>

#include <KDE/KUrl>
#include <KDE/KIO/NetAccess>
#include <QApplication>


DocumentCacheUpdateJob::DocumentCacheUpdateJob(const QStringList &urls, QObject* parent):
        KJob(parent),
        m_persentage(0),
        m_numFiles(0),
        m_killed(false),
        m_urls(urls),
        m_currentJob(0) {
    setCapabilities(Killable);
    setObjectName("Document cache update");
    m_numFiles = m_urls.size();
}

DocumentCacheUpdateJob::~DocumentCacheUpdateJob() {
    m_killed = true;
}

void DocumentCacheUpdateJob::start() {
    if (!m_killed) {
        nextJob(0);
    }
}

bool DocumentCacheUpdateJob::doKill() {
    m_killed = true;
    if(m_currentJob)
        m_currentJob->kill();
    return m_killed;
}

void DocumentCacheUpdateJob::nextJob(KJob *completed)
{
    if(completed)
        m_persentage += completed->percent() / m_numFiles;
    setPercent(m_persentage);
    m_currentJob = 0;
    if(m_urls.size() == 0) {
        setError(0);
        emitResult();
        return;
    }

    if(m_urls.size() > 0 && !m_killed) {
        QString url = m_urls.takeFirst();
        KUrl u(url);
        if (!u.isValid() || u.isLocalFile())
            return nextJob(0);
        QString localUrl = IDocumentCacheManager::self()->getLocalUrl(url);
        QFile::remove(localUrl);
        KIO::CopyJob *job = KIO::copy(url, localUrl, KIO::HideProgressInfo);
        m_currentJob = job;
        connect(job, SIGNAL(result(KJob *)), this, SLOT(nextJob(KJob *)));
        job->setUiDelegate(0);
        job->start();
    }
}

KJob* DocumentCacheManager::createUpdateJob(const QStringList& urls)
{
    return new DocumentCacheUpdateJob(urls);
}



DocumentCacheManager::DocumentCacheManager() : IDocumentCacheManager()
{

}

DocumentCacheManager::~DocumentCacheManager()
{

}

QString DocumentCacheManager::cachedUrl(const QString& url, KIO::JobFlags flags)
{
    KUrl u(url);
    if (!u.isValid())
        return QString::null;
    if ( u.isLocalFile() )
        return u.toLocalFile();
    QString localUrl = getLocalUrl(url);
    if (QFile::exists(localUrl))
        return localUrl;
    //KIO::CopyJob *job = KIO::copy(url, localUrl, flags);
    //job->setUiDelegate(0);
    //if (job->exec())
    if(KIO::NetAccess::file_copy(url, localUrl, QApplication::activeWindow()))
        return localUrl;
    return QString::null;
}

QString DocumentCacheManager::getLocalUrl(const QString& url)
{
    KUrl u(url);

    if (!u.isValid())
        return QString::null;

    if ( u.isLocalFile() )
        return u.toLocalFile();
    else {
        QString local = QString ( u.toEncoded().constData() ).replace ( QRegExp ( "[/]|[:]" ), "_" );
        KUrl lu ( QString ( "%1%2" ).arg (getCacheDirectory(), local ) );
        return lu.toLocalFile();
    }
}

QString DocumentCacheManager::updateCache(const QString& url, KIO::JobFlags flags)
{
    KUrl u(url);
    if (!u.isValid())
        return QString::null;
    if ( u.isLocalFile() )
        return u.toLocalFile();

    QString localUrl = getLocalUrl(url);
    QFile::remove(localUrl);
    //KIO::CopyJob *job = KIO::copy(url, localUrl, flags);
    //job->setUiDelegate(0);
    //if (job->exec())
    if(KIO::NetAccess::file_copy(url, localUrl, QApplication::activeWindow()))
        return localUrl;
    return QString::null;
}

#include "documentcachemanager.moc"

