/***************************************************************************
    begin                : Sat Jul 27 2002
    copyright            : (C) 2004 by Jens Herden <jhe@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#ifndef QUANTANETACCESS_H
#define QUANTANETACCESS_H

#include "quantaexport.h"

/** @short helperclass to track changes in the project

    Use this class as replacement for KIO::NetAccess (look there for details).
    Every possible impact on the project will be monitored and signaled to the project.

    If you don't want user interaction use the extra parameter confirm

    If the user don't accept the removal from the project no action happens
    and the caller gets false as result.
*/

namespace Koncrete { class Plugin; }

class KDEVQUANTA_EXPORT QuantaNetAccess
{
public:

  static bool upload(const QString& src, const KUrl& target, Koncrete::Plugin* plugin, bool confirm = true);

  static bool copy( const KUrl & src, const KUrl & target, Koncrete::Plugin* plugin, bool confirm = true )
  {
    return file_copy( src, target, -1, false /*not overwrite*/, false, plugin, confirm );
  }

  static bool file_copy( const KUrl& src, const KUrl& target, int permissions,
                         bool overwrite, bool resume, Koncrete::Plugin* plugin, bool confirm = true );

  static bool file_move( const KUrl& src, const KUrl& target, int permissions,
                         bool overwrite, bool resume, Koncrete::Plugin* plugin, bool confirm = true );

  static bool dircopy( const KUrl & src, const KUrl & target, Koncrete::Plugin* plugin, bool confirm = true )
  {
    KUrl::List srcList;
    srcList.append( src );
    return dircopy( srcList, target, plugin, confirm );
  };

  static bool dircopy( const KUrl::List & srcList, const KUrl & target, Koncrete::Plugin* plugin, bool confirm = true );

  static bool move( const KUrl& src, const KUrl& target, Koncrete::Plugin* plugin, bool confirm = true )
  {
    KUrl::List srcList;
    srcList.append( src );
    return move( srcList, target, plugin, confirm );
  }

  static bool move( const KUrl::List& srcList, const KUrl& target, Koncrete::Plugin* plugin, bool confirm = true );

  static bool del( const KUrl & url, Koncrete::Plugin* plugin, QWidget* window, bool confirm = true );

  static bool mkdir( const KUrl & url, Koncrete::Plugin* plugin, int permissions, bool confirm = true );

private:
  static void checkProjectInsert(const KUrl& source, const KUrl& target, Koncrete::Plugin* plugin, bool confirm = true);

  static bool checkProjectRemove(const KUrl& target, Koncrete::Plugin* plugin, bool confirm = true, bool remove = true);

  static bool checkProjectDel(const KUrl& target, Koncrete::Plugin* plugin, QWidget* window, bool confirm = true);

  static KUrl adjustURL(const KUrl &url);

  static void errorMsg(QWidget* window = 0);

private:
  QuantaNetAccess() {};
  ~QuantaNetAccess() {};

};

#endif
