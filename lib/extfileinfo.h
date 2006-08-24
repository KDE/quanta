/*
    From WebMaker - KDE HTML Editor
    Copyright (C) 1998, 1999 Alexei Dets <dets@services.ru>
    Rewritten for Quanta Plus: (C) 2002 Andras Mantia <amantia@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
*/

#ifndef _EXTFILEINFO_H_
#define _EXTFILEINFO_H_

#include "quantaexport.h"

#include <kio/global.h>
#include <kio/job.h>
#include <kurl.h>
#include <kfileitem.h>

#include <QObject>
#include <QList>
#include <QHash>
#include <QRegExp>

class LIBQUANTA_EXPORT ExtFileInfo : public QObject
{
 Q_OBJECT
public:
  ExtFileInfo() {};
  ~ExtFileInfo() {};

  /** Returns the relative url of urlToConvert to baseURL.

      @deprecated  use KUrl::relativeUrl(baseURL, urlToConvert) instead
   */
  static KDE_DEPRECATED KUrl toRelative(const KUrl& urlToConvert, const KUrl& baseURL);

  /** Convert relative url to absolute, based on baseURL.

      @deprecated use baseURL.addPath(urlToConvert.path())
  */
  static KDE_DEPRECATED KUrl toAbsolute(const KUrl& urlToConvert, const KUrl& baseURL);

  /** Returns a recursive list of files under path, matching the specified file mask.
  *   Directory paths will have the trailing slash.
  *   The return will also contain the name of the subdirectories.
  */
  static KUrl::List allFiles( const KUrl& path, const QString &mask);

  /** Returns a recursive list of files under path, matching the specified file mask.
      The returned urls are relative to path.
  */
  static KUrl::List allFilesRelative( const KUrl& path, const QString &mask);

  /** Returns a recursive list of files under path, matching the specified file mask.
      The returned list contains detailed information about each url.
      The user should delete the KFileItems and clear the dict
      after they are not needed.
  */
  static QHash<QString, KFileItem*> allFilesDetailed(const KUrl& path, const QString &mask);

  /**
   * Gets a list with the urls under path. The listing is not recursive.
   * @param path The starting path (should be a directory)
   * @param mask The filtering mask
   * @return URL objects pointing to the files and directories under path. Directories end with a slash.
   */
  static KUrl::List listDir(const KUrl& path, const QString &mask);

  /**
   * Gets a list with the urls under path. The listing is not recursive and the returned strings are relative to the path.
   * @param path The starting path (should be a directory)
   * @param mask The filtering mask
   * @return relative paths pointing to the files and directories under path. Directories end with a slash.
   */
  static QStringList listDirRelative(const KUrl& path, const QString &mask);

  /** Creates a dir if don't exists.
      @deprecated use QuantaNetAccess::mkdir
  */
  static KDE_DEPRECATED bool createDir(const KUrl & path);

  /** Returns the path to the url.
      @deprecated don't store a relative path in KUrl, use url.path()!
   */
  static KDE_DEPRECATED KUrl path(const KUrl &url) {KUrl newURL(url); newURL.setPath(url.directory(KUrl::ObeyTrailingSlash)); return newURL;}

  /** A slightly better working alternative of KIO::NetAccess::exists().
      Checks for the existence of the url and if it is writeable. */
  static bool exists(const KUrl& url);

  /** Synchronous copy, like NetAccess::file_copy in KDE 3.2, just that it doesn't show a progress dialog */
  static bool copy( const KUrl& src, const KUrl& dest, int permissions=-1,
                    bool overwrite=false, bool resume=false, QWidget* window = 0L );

  /** Checks if url exists and shows a question about overwriting it.
  *  @param url the url to check
  *  @return true if the user answered yes, false otherwise.
  */
  static bool checkOverwrite(const KUrl& url);

  /** Reenters the event loop. You must call qApp->exit_loop() to exit it. */
  void enter_loop();

  /**
   * Exits the reentered event loop.
   */
  void exit_loop() { emit leaveModality();}

  /** Set's up the url correctly from urlString.
   *  If urlString does contain a relative path this is equal to
   */
  static void setUrl(KUrl &url, const QString& urlString);

Q_SIGNALS:
  void leaveModality();

private:
  /** Internal methods called by the above ones. They start their own event loop and
      exit when the even loop is exited */
  bool internalExists(const KUrl& url);
  bool internalCopy(const KUrl& src, const KUrl& target, int permissions,
                    bool overwrite, bool resume, QWidget* window);
  KUrl::List allFilesInternal(const KUrl& startURL, const QString& mask);
  QHash<QString, KFileItem*> allFilesDetailedInternal(const KUrl& startURL, const QString& mask);
  KUrl::List listDirInternal(const KUrl& startURL, const QString& mask);

  bool bJobOK;
  static QString lastErrorMsg;
  KIO::UDSEntry m_entry;
  KUrl::List dirListItems;
  QHash<QString, KFileItem*> detailedDirListItems;
  QList<QRegExp*> lstFilters;
  int m_listJobCount;
  QString m_listStartURL;

private slots:
  void slotListResult(KJob *job);
  void slotResult(KJob * job);
  void slotNewEntries(KIO::Job *job, const KIO::UDSEntryList& udsList);
  void slotNewDetailedEntries(KIO::Job *job, const KIO::UDSEntryList& udsList);
public slots:
  /** Timeout occurred while waiting for some network function to return. */
  void slotTimeout();
};


#endif

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
