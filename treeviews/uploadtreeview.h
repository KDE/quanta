/***************************************************************************
                          uploadtreeview.h  -  description
                             -------------------
    begin                : Sun Aug 25 2002
    copyright            : (C) 2002, 2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#ifndef UPLOADTREEVIEW_H
#define UPLOADTREEVIEW_H

#include <qdir.h>
#include <qpixmap.h>
#include <qheader.h>

#include "uploadtreefolder.h"
#include "uploadtreefile.h"

class KFileItem;

/**
  *@author George Vilches & Andras Mantia
  */

class UploadTreeView : public KListView {
  Q_OBJECT
public:
  UploadTreeView( QWidget *parent, const char *name=0L );
  ~UploadTreeView();
  UploadTreeFile* addItem( const KURL &a_url, const KFileItem &a_fileItem);
  QListViewItem* findItem( const QString& );
  int checkboxTree( QListViewItem *it = 0);
  void expandAll( QListViewItem * = 0 );
  void collapseAll( QListViewItem * = 0 );
  void invertAll( QListViewItem * = 0 );
  virtual void selectAll( bool select);
  /** No descriptions */
  QListViewItem* itemByUrl(const KURL& a_url);

public slots:
  void slotSelectFile( QListViewItem * );
  void slotSelectFile( );
  void selectAllUnderNode( QListViewItem *it, bool select );

private slots:
  void slotDoubleClicked(QListViewItem *, const QPoint &, int );

private:
  UploadTreeFolder* findFolder( UploadTreeFolder *, const QString&);
  UploadTreeFolder* printTree( UploadTreeFolder *, const QString&);
};

#endif
