/***************************************************************************
                          quantanetaccess.h  -  description
                             -------------------
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


/** @short helperclass to track changes in the project

    This class be used as replacement of KIO::NetAccess (look there for details).
    Every possible impact on the project will be monitored and signaled to the project.

    If you don't want user interaction use the extra parameter confirm

    If the user don't accept the removal from the project no action happens
    and the caller gets false as result.
*/

class QuantaNetAccess
{
public:

  static bool upload(const QString& src, const KURL& target, QWidget* window, bool confirm = true);

  static bool copy( const KURL & src, const KURL & target, QWidget* window, bool confirm = true )
  {
    return file_copy( src, target, -1, false /*not overwrite*/, false, window, confirm );
  }

  static bool file_copy( const KURL& src, const KURL& target, int permissions,
                         bool overwrite, bool resume, QWidget* window, bool confirm = true );

  static bool file_move( const KURL& src, const KURL& target, int permissions,
                         bool overwrite, bool resume, QWidget* window, bool confirm = true );

  static bool dircopy( const KURL & src, const KURL & target, QWidget* window, bool confirm = true )
  {
    KURL::List srcList;
    srcList.append( src );
    return dircopy( srcList, target, window, confirm );
  };

  static bool dircopy( const KURL::List & srcList, const KURL & target, QWidget* window, bool confirm = true );

  static bool move( const KURL& src, const KURL& target, QWidget* window, bool confirm = true )
  {
    KURL::List srcList;
    srcList.append( src );
    return move( srcList, target, window, confirm );
  }

  static bool move( const KURL::List& srcList, const KURL& target, QWidget* window, bool confirm = true );

  static bool del( const KURL & url, QWidget* window, bool confirm = true );

  static bool mkdir( const KURL & url, QWidget* window, int permissions, bool confirm = true );

  static void checkProjectInsert(const KURL& target, QWidget* window, bool confirm = true);

  static bool checkProjectRemove(const KURL& target, QWidget* window, bool confirm = true, bool remove = true);

  static bool checkProjectDel(const KURL& target, QWidget* window, bool confirm = true);

  static KURL adjustURL(const KURL &url);

  static void errorMsg(QWidget* window = 0);

private:
  QuantaNetAccess() {};
  ~QuantaNetAccess() {};

};

#endif
