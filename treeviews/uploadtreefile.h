/***************************************************************************
                          uploadtreefile.h  -  description
                             -------------------
    begin                : Sun Aug 25 2002
    copyright            : (C) 2002, 2003 by Andras Mantia
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

#ifndef UPLOADTREEFILE_H
#define UPLOADTREEFILE_H

#include <klistview.h>

#include "uploadtreefolder.h"

/**
  *@author George Vilches & Andras Mantia
  */

class KFileItem;

class UploadTreeFile : public KListViewItem  {
public:
  UploadTreeFile( UploadTreeFolder *parent, const KURL &a_url, const KFileItem &a_fileItem);
  UploadTreeFile( QListView *parent, const KURL &a_url, const KFileItem &a_fileItem);
  ~UploadTreeFile();

  void setWhichPixmap(const QString& pixmap);
  mode_t permissions();

  /** used for sorting */
  virtual QString key ( int column, bool ascending ) const;
  /** No descriptions */
  KURL url();

public:
  UploadTreeFolder * parentFolder;
  KURL m_url;
  bool isDir;
  KFileItem *m_fileItem;
};

#endif
