/***************************************************************************
                          uploadtreefolder.h  -  description
                             -------------------
    begin                : Sun Aug 25 2002
    copyright            : (C) 2002 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#ifndef UPLOADTREEFOLDER_H
#define UPLOADTREEFOLDER_H

#include <qfile.h>
#include <qfileinfo.h>
#include <qstring.h>
#include <qstringlist.h>

#include <klistview.h>
#include <kurl.h>

/**
  *@author Andras Mantia
  */

class KURL;

class UploadTreeFolder : public KListViewItem  {
public:
  UploadTreeFolder( const KURL &a_url, QListView * parent, const char * name);
  UploadTreeFolder( const KURL &a_url, UploadTreeFolder * parent, const char * name );
  virtual ~UploadTreeFolder();

  /** used for sorting */
  virtual QString key ( int column, bool ascending ) const;
  QString fullName();
  KURL url() {return m_url;}

  virtual void setOpen( bool );
  void setup();
  void setWhichPixmap(const QString& pixmap );

public:
  UploadTreeFolder * parentFolder;
  KURL m_url;

public slots: // Public slots
  void reloadList();

};

#endif
