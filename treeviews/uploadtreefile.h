/***************************************************************************
                          uploadtreefile.h  -  description
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
  int permissions();

  /** used for sorting */
  virtual QString key ( int column, bool ascending ) const;
  /** No descriptions */
  KURL url();
  void setConfirmUpload(bool confirm) {m_confirm = confirm;}
  bool confirmUpload() {return m_confirm;}

public:
  UploadTreeFolder * parentFolder;
  KURL m_url;
  bool isDir;
  bool m_confirm;
  KFileItem *m_fileItem;
};

#endif
