/***************************************************************************
                          uploadtreeview.h  -  description
                             -------------------
    begin                : Sun Aug 25 2002
    copyright            : (C) 2002 by Andras Mantia
    email                : amantia@freemail.hu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef UPLOADTREEVIEW_H
#define UPLOADTREEVIEW_H

#include <qdir.h>
#include <qpixmap.h>
#include <qheader.h>

#include "filemanage.h"
#include "uploadtreefolder.h"
#include "uploadtreefile.h"


/**
  *@author George Vilches
  */

class UploadTreeView : public QListView {
  Q_OBJECT
public:
  UploadTreeView( QWidget *parent, const char *name=0L );
  ~UploadTreeView();
  UploadTreeFile* addItem( QString, QString, QString );
  QListViewItem* findItem( QString );
  int checkboxTree( QListViewItem *it );
  void expandAll( QListViewItem * = 0 );
  void collapseAll( QListViewItem * = 0 );
  void invertAll( QListViewItem * = 0 );
  virtual void selectAll( bool select);

public slots:
  void slotSelectFile( QListViewItem * );
  void slotSelectFile( );
  void selectAllUnderNode( QListViewItem *it, bool select );

private:
  UploadTreeFolder* findFolder( UploadTreeFolder *, QString );
  UploadTreeFolder* printTree( UploadTreeFolder *, QString );
};

#endif
