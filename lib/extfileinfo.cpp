/*
    From WebMaker - KDE HTML Editor
    Copyright (C) 1998, 1999 Alexei Dets <dets@services.ru>

    Rewritten for Quanta Plus: (C) 2002 Andras Mantia <amantia@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
*/


//qt includes
#include <QDir>
#include <QApplication>
#include <QList>
#include <QStringList>
#include <QRegExp>
#include <QTimer>

//kde includes
#include <kurl.h>
#include <kio/job.h>
#include <kio/netaccess.h>
#include <kio/scheduler.h>
#include <klocale.h>
#include <kdirlister.h>
#include <kfileitem.h>
#include <kglobal.h>
#include <kmessagebox.h>
#include <kprotocolinfo.h>
#include <kdebug.h>

//app includes
#include "extfileinfo.h"

QString ExtFileInfo::lastErrorMsg = "";

/** create a relative short url based in baseURL*/
KUrl ExtFileInfo::toRelative(const KUrl& urlToConvert, const KUrl& baseURL)
{
  KUrl resultURL = urlToConvert;
  if (urlToConvert.protocol() == baseURL.protocol())
  {
    QString path = urlToConvert.path();
    QString basePath = baseURL.path(KUrl::AddTrailingSlash);
    if (path.startsWith("/"))
    {
      path.remove(0, 1);
      basePath.remove(0, 1);
      if (! basePath.endsWith("/") ) basePath.append('/');

      int pos=0;
      int pos1=0;
      for (;;)
      {
        pos=path.indexOf('/');
        pos1=basePath.indexOf('/');
        if ( pos<0 || pos1<0 ) break;
        if ( path.left(pos+1 ) == basePath.left(pos1+1) )
        {
          path.remove(0, pos+1);
          basePath.remove(0, pos1+1);
        }
        else
          break;
      };

      if ( basePath == "/" ) basePath="";
      int level = basePath.count("/");
      for (int i=0; i<level; i++)
      {
        path="../"+path;
      };
    }

    resultURL.setPath(QDir::cleanPath(path));
  }

  if (urlToConvert.path().endsWith("/") && !resultURL.path().isEmpty())
    resultURL.adjustPath(KUrl::AddTrailingSlash);
  return resultURL;
}


/** convert relative filename to absolute */
KUrl ExtFileInfo::toAbsolute(const KUrl& urlToConvert, const KUrl& baseURL)
{
  KUrl resultURL = urlToConvert;
  if (urlToConvert.protocol() == baseURL.protocol() && !urlToConvert.path().startsWith("/"))
  {
    int pos;
    QString cutname = urlToConvert.path();
    QString cutdir = baseURL.path(KUrl::AddTrailingSlash);
    while ( (pos = cutname.indexOf("../")) >=0 )
    {
       cutname.remove( 0, pos+3 );
       cutdir.remove( cutdir.length()-1, 1 );
       cutdir.remove( cutdir.lastIndexOf('/')+1 , 1000);
    }
    resultURL.setPath(QDir::cleanPath(cutdir+cutname));
  }

  if (urlToConvert.path().endsWith("/")) resultURL.adjustPath(KUrl::AddTrailingSlash);
  return resultURL;
}

KUrl::List ExtFileInfo::allFiles( const KUrl& path, const QString& mask)
{
  ExtFileInfo internalFileInfo;
  return internalFileInfo.allFilesInternal(path, mask);
}

KUrl::List ExtFileInfo::allFilesRelative( const KUrl& path, const QString& mask)
{
  ExtFileInfo internalFileInfo;
  KUrl::List r = internalFileInfo.allFilesInternal( path, mask);

  KUrl::List::Iterator it;
  for ( it = r.begin(); it != r.end(); ++it )
  {
    *it = KUrl::relativeUrl(path, *it);
  }

  return r;
}

QHash<QString, KFileItem*> ExtFileInfo::allFilesDetailed( const KUrl& path, const QString& mask)
{
  ExtFileInfo internalFileInfo;
  return internalFileInfo.allFilesDetailedInternal(path, mask);
}

KUrl::List ExtFileInfo::listDir( const KUrl& path, const QString& mask)
{
  ExtFileInfo internalFileInfo;
  return internalFileInfo.listDirInternal(path, mask);
}

QStringList ExtFileInfo::listDirRelative( const KUrl& path, const QString& mask)
{
  QStringList result;
  ExtFileInfo internalFileInfo;
  KUrl::List urls = internalFileInfo.listDirInternal(path, mask);
  KUrl::List::ConstIterator it = urls.constBegin();
  KUrl::List::ConstIterator end = urls.constEnd();
  for ( ; it != end; ++it )
  {
    result += KUrl::relativeUrl(path, *it);
  }

  return result;
}

bool ExtFileInfo::createDir(const KUrl& path)
{
  KUrl url = path;
  if (exists(url))
    return true;
  url.setPath("/");
  if (!exists(url))
    return false; //the root is not accessible, possible wrong username/password supplied

  url = path;

  // move up the path until I find an existing folder
  QStringList stack;
  do {
    stack.push_front(url.fileName());
    url = url.upUrl();
  } while (!exists(url)); // we know that the root exists, so this will terminate

  // move down the path and create all folders
  do {
    url.addPath(stack.front());
    if (!KIO::NetAccess::mkdir(url, (QWidget*)0L))
      return false;
    stack.pop_front();
  } while (!stack.empty());

  return true;
}


bool ExtFileInfo::exists(const KUrl& a_url)
{
// Andras: Don't use it now, as it brings up an extra dialog and need manual
// intervention when usign fish
// return KIO::NetAccess::exists(a_url, false);

// No dialog when stating.
 if ( a_url.isLocalFile() )
 {
    return QFile::exists( a_url.path() );
 } else
 {
  ExtFileInfo internalFileInfo;
  return internalFileInfo.internalExists(a_url);
 }
}

/* Synchronous copy, like NetAccess::file_copy in KDE 3.2 */
bool ExtFileInfo::copy( const KUrl& src, const KUrl& target, int permissions,
 bool overwrite, bool resume, QWidget* window )
{
  ExtFileInfo internalFileInfo;
  return internalFileInfo.internalCopy( src, target, permissions, overwrite, resume, window );
}

bool ExtFileInfo::checkOverwrite(const KUrl& url)
{
  bool result = true;

  if (ExtFileInfo::exists(url))
  {
    if (KMessageBox::warningYesNo(0L,
        i18n( "<qt>The file <b>%1</b> already exists.<br>Do you want to overwrite it?</qt>", url.pathOrUrl() )) == KMessageBox::No)
            result = false;
  }

  return result;
}

KUrl::List ExtFileInfo::allFilesInternal(const KUrl& startURL, const QString& mask)
{
  dirListItems.clear();
  if (internalExists(startURL))
  {
    foreach (QRegExp* rx, lstFilters)
    {
      delete rx;
    }
    lstFilters.clear();
    // Split on white space
    QStringList list = mask.split(' ');
    for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it )
      lstFilters.append( new QRegExp(*it, Qt::CaseInsensitive, QRegExp::Wildcard) );

    bJobOK = true;
    KIO::ListJob *job = KIO::listRecursive(startURL, false, true);
    m_listJobCount = 1;
    connect(job, SIGNAL(entries(KIO::Job *, const KIO::UDSEntryList&)),
            this, SLOT(slotNewEntries(KIO::Job *, const KIO::UDSEntryList&)));
    connect( job, SIGNAL( result (KJob *) ),
             this, SLOT( slotListResult (KJob *) ) );
    m_listStartURL = startURL.url();

    //kDebug(24000) << "Now listing: " << startURL.url() << endl;
    enter_loop();
    //kDebug(24000) << "Listing done: " << startURL.url() << endl;
    lstFilters.clear();
    if (!bJobOK)
    {
 //     kDebug(24000) << "Error while listing "<< startURL.url() << endl;
      dirListItems.clear();
    }
  }
  return dirListItems;
}

QHash<QString, KFileItem*> ExtFileInfo::allFilesDetailedInternal(const KUrl& startURL, const QString& mask)
{
  foreach (KFileItem *item, detailedDirListItems)
  {
    delete item;
  }
  detailedDirListItems.clear();
  if (internalExists(startURL))
  {
    foreach (QRegExp* rx, lstFilters)
    {
      delete rx;
    }
    lstFilters.clear();
    // Split on white space
    QStringList list = mask.split(' ');
    for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it )
      lstFilters.append( new QRegExp(*it, Qt::CaseInsensitive, QRegExp::Wildcard) );

    bJobOK = true;
    KIO::ListJob *job = KIO::listRecursive(startURL, false, true);
    m_listJobCount = 1;
    connect(job, SIGNAL(entries(KIO::Job *, const KIO::UDSEntryList&)),
            this, SLOT(slotNewDetailedEntries(KIO::Job *, const KIO::UDSEntryList&)));
    connect( job, SIGNAL( result (KJob *) ),
             this, SLOT( slotListResult (KJob *) ) );
    m_listStartURL = startURL.url();
    //kDebug(24000) << "Now listing: " << startURL.url() << endl;
    enter_loop();
    //kDebug(24000) << "Listing done: " << startURL.url() << endl;
    lstFilters.clear();
    if (!bJobOK)
    {
 //     kDebug(24000) << "Error while listing "<< startURL.url() << endl;
      detailedDirListItems.clear();
    }
  }
  return detailedDirListItems;
}


KUrl::List ExtFileInfo::listDirInternal(const KUrl& startURL, const QString& mask)
{
  dirListItems.clear();
  if (internalExists(startURL))
  {
    foreach (QRegExp* rx, lstFilters)
    {
      delete rx;
    }
    lstFilters.clear();
    // Split on white space
    QStringList list = mask.split(' ');
    for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it )
      lstFilters.append( new QRegExp(*it, Qt::CaseInsensitive, QRegExp::Wildcard) );

    bJobOK = true;
    KIO::ListJob *job = KIO::listDir(startURL, false, true);
    m_listJobCount = 1;
    connect(job, SIGNAL(entries(KIO::Job *, const KIO::UDSEntryList&)),
            this, SLOT(slotNewEntries(KIO::Job *, const KIO::UDSEntryList&)));
    connect( job, SIGNAL( result (KJob *) ),
             this, SLOT( slotListResult (KJob *) ) );
    m_listStartURL = startURL.url();

    //kDebug(24000) << "Now listing: " << startURL.url() << endl;
    enter_loop();
    //kDebug(24000) << "Listing done: " << startURL.url() << endl;
    lstFilters.clear();
    if (!bJobOK)
    {
 //     kDebug(24000) << "Error while listing "<< startURL.url() << endl;
      dirListItems.clear();
    }
  }
  return dirListItems;
}


//Some hackery from KIO::NetAccess as they do not do exactly what we want
/* return true if the url exists*/
bool ExtFileInfo::internalExists(const KUrl& url)
{
  bJobOK = true;
  KUrl url2 = url;
  url2.adjustPath(KUrl::RemoveTrailingSlash);
 // kDebug(24000)<<"ExtFileInfo::internalExists"<<endl;
  KIO::StatJob * job = KIO::stat(url2, false);
  job->setDetails(0);
  job->setSide(false); //check the url for writing
  connect( job, SIGNAL( result (KJob *) ),
           this, SLOT( slotResult (KJob *) ) );

  //To avoid lock-ups, start a timer.
  QTimer::singleShot(60*1000, this,SLOT(slotTimeout()));
  //kDebug(24000)<<"ExtFileInfo::internalExists:before enter_loop"<<endl;
  enter_loop();
  //kDebug(24000)<<"ExtFileInfo::internalExists:after enter_loop"<<endl;
  return bJobOK;
}

bool ExtFileInfo::internalCopy(const KUrl& src, const KUrl& target, int permissions,
                                bool overwrite, bool resume, QWidget* window)
{
  bJobOK = true; // success unless further error occurs

  KIO::Scheduler::checkSlaveOnHold(true);
  KIO::Job * job = KIO::file_copy( src, target, permissions, overwrite, resume, false );
//  KIO::Job * job2 = KIO::del(target, false );
  //job2->setWindow (window);
  //connect( job2, SIGNAL( result (KIO::Job *) ),
//           this, SLOT( slotResult (KIO::Job *) ) );

  //enter_loop();
  //if (bJobOK)
  {
//    kDebug(24000) << "Copying " << src << " to " << target << endl;
   // KIO::Job *job = KIO::copy( src, target, false );
    job->setWindow (window);
    connect( job, SIGNAL( result (KJob *) ),
            this, SLOT( slotResult (KJob *) ) );
    enter_loop();
 }
  return bJobOK;
}


void qt_enter_modal( QWidget *widget );
void qt_leave_modal( QWidget *widget );

void ExtFileInfo::enter_loop()
{
  QEventLoop eventLoop;
  connect(this, SIGNAL(leaveModality()),
          &eventLoop, SLOT(quit()));
  eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
}

void ExtFileInfo::slotListResult(KJob *job)
{
  m_listJobCount--;
  if (m_listJobCount == 0)
    slotResult(job);
}

void ExtFileInfo::slotResult(KJob *job)
{
   //kDebug(24000)<<"ExtFileInfo::slotResult"<<endl;
 bJobOK = !job->error();
  if ( !bJobOK )
  {
    if ( lastErrorMsg.isEmpty() )
     lastErrorMsg = job->errorString();
  }
  if (job->metaObject()->className() == "KIO::StatJob")
    m_entry = static_cast<KIO::StatJob *>(job)->statResult();
  emit leaveModality();
}

void ExtFileInfo::slotNewEntries(KIO::Job *job, const KIO::UDSEntryList& udsList)
{
  KUrl url = static_cast<KIO::ListJob *>(job)->url();
  url.adjustPath(KUrl::RemoveTrailingSlash);
  // avoid creating these QStrings again and again
  static const QString& dot = KGlobal::staticQString(".");
  static const QString& dotdot = KGlobal::staticQString("..");

  KIO::UDSEntryList::ConstIterator it = udsList.begin();
  KIO::UDSEntryList::ConstIterator end = udsList.end();
  KUrl itemURL;
  QList<KFileItem*> linkItems;
  for ( ; it != end; ++it )
  {
    QString name = (*it).stringValue(KIO::UDS_NAME);

    if (!name.isEmpty() && name != dot && name != dotdot)
    {
      KFileItem* item = new KFileItem( *it, url, false, true );
      if (item->isDir() && item->isLink())
      {
        KUrl u = item->url();
        u.setPath(item->linkDest());
        u.adjustPath(KUrl::AddTrailingSlash);
        if (!dirListItems.contains(u) && u.url() != m_listStartURL)
        {
          linkItems.append(new KFileItem(*item));
        } else
        {
          kDebug(24000) << "Recursive link" << u.url() << endl;
          continue;
        }
      }
      itemURL = item->url();
      if (item->isDir())
        itemURL.adjustPath(KUrl::AddTrailingSlash);
      foreach (QRegExp *filter, lstFilters)
      {
        if (filter->exactMatch(item->text()))
            dirListItems.append(itemURL);
      }
      delete item;
    }
  }
  for (QList<KFileItem*>::ConstIterator it = linkItems.constBegin(); it != linkItems.constEnd(); ++it)
  {
    KIO::ListJob *ljob = KIO::listRecursive((*it)->url(), false, true);
    m_listJobCount++;
    //kDebug(24000) << "Now listing: " << (*it)->url() << endl;
    connect( ljob, SIGNAL(entries(KIO::Job *,const KIO::UDSEntryList &)),
             this,SLOT  (slotNewEntries(KIO::Job *,const KIO::UDSEntryList &)));
    connect( ljob, SIGNAL(result(KJob *)),
             this,SLOT  (slotListResult(KJob *)));
  }
  foreach(KFileItem *item, linkItems)
  {
    delete item;
  }
}

void ExtFileInfo::slotNewDetailedEntries(KIO::Job *job, const KIO::UDSEntryList& udsList)
{
  KUrl url = static_cast<KIO::ListJob *>(job)->url();
  url.adjustPath(KUrl::AddTrailingSlash);
  // avoid creating these QStrings again and again
  static const QString& dot = KGlobal::staticQString(".");
  static const QString& dotdot = KGlobal::staticQString("..");

  KIO::UDSEntryList::ConstIterator it = udsList.begin();
  KIO::UDSEntryList::ConstIterator end = udsList.end();
  KUrl itemURL;
  QList<KFileItem*> linkItems;
  for ( ; it != end; ++it )
  {
    QString name = (*it).stringValue(KIO::UDS_NAME);

    if (!name.isEmpty() && name != dot && name != dotdot)
    {
      KFileItem *item=  new KFileItem(*it, url, false, true );
      if (item->isDir() && item->isLink())
      {
        KUrl u = item->url();
        u.setPath(item->linkDest());
        QString urlStr = u.url();
        if (detailedDirListItems.contains(urlStr) == 0L &&
            (urlStr != m_listStartURL))
        {
          linkItems.append(new KFileItem(*item));
        } else
        {
          kDebug(24000) << "Recursive link" << item->url() << endl;
          continue;
        }
      }
      bool added = false;
      foreach (QRegExp *filter, lstFilters)
        if (filter->exactMatch(item->text()))
        {
          detailedDirListItems.insert(item->url().url(), item);
          added = true;
        }
      if (!added)
        delete item;
    }
  }
  foreach (KFileItem *it ,linkItems)
  {
    KIO::ListJob *ljob = KIO::listRecursive(it->url(), false, true);
    m_listJobCount++;
   // kDebug(24000) << "Now listing: " << (*it)->url() << endl;
    connect( ljob, SIGNAL(entries(KIO::Job *,const KIO::UDSEntryList &)),
             this,SLOT  (slotNewDetailedEntries(KIO::Job *,const KIO::UDSEntryList &)));
    connect( ljob, SIGNAL(result(KJob *)),
             this,SLOT  (slotListResult(KJob *)));
  }
  foreach(KFileItem* item, linkItems)
  {
    delete item;
  }
}

/** Timeout occurred while waiting for some network function to return. */
void ExtFileInfo::slotTimeout()
{
  bJobOK = false;
  emit leaveModality();
}


void ExtFileInfo::setUrl(KUrl &url, const QString &urlString)
{
  KUrl oldUrl = url;
  url = urlString;
  if (!KProtocolInfo::isKnownProtocol(url))
  {
    url = oldUrl;
    url.setPath(urlString);
    if (url.protocol().isEmpty())
      url.setProtocol("file");
  }
}


#include "extfileinfo.moc"

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
