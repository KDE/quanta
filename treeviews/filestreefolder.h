/***************************************************************************
                          filestreefolder.h  -  description
                             -------------------
    begin                : Mon Feb 21 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavski
                           (C) 2002 Andras Mantia
    email                : pdima@mail.univ.kiev.ua, amantia@freemail.hu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef FILESTREEFOLDER_H
#define FILESTREEFOLDER_H

#include <klistview.h>
#include <qstringlist.h>

#include <kurl.h>

class QMimeSource;
class QDropEvent;
class FilesTreeView;


class FilesTreeFolder:public KListViewItem{
public:
  FilesTreeFolder( QListView * parentListView, FilesTreeFolder * parent, const KURL& p_url);
  FilesTreeFolder( QListView * parentListView, const QString &p_name, const KURL& p_url);
  virtual ~FilesTreeFolder();

  /** Open a directory node and read the subdir */
  virtual void setOpen( bool );
  virtual QString fullName();


  QString text( int column ) const;

   /** used for sorting */
   virtual QString key ( int column, bool ascending ) const;

  void setup();
  /** No descriptions */
  virtual bool acceptDrop(const QMimeSource *mime) const;
  /** No descriptions */
  virtual void sortChildItems(int column, bool ascending);

  FilesTreeFolder * parentFolder;
  QString name;
  KURL url;

public slots: // Public slots
    /** reload file list */
    void reloadList();
protected: // Protected methods
  /** No descriptions */
  virtual void dropped(QDropEvent *e);
  /** No descriptions */
  void init();

  FilesTreeView* parentView;


};


#endif
