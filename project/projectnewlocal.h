/***************************************************************************
                          projectnewlocal.h  -  description
                             -------------------
    begin                : Fri Oct 27 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com>
                           (C) 2001-2003 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PROJECTNEWLOCAL_H
#define PROJECTNEWLOCAL_H

#include <qwidget.h>
#include <kurl.h>
#include <projectnewlocals.h>

/**
  *@author Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon & Andras Mantia
  */

class CopyTo;

class ProjectNewLocal : public ProjectNewLocalS  {
   Q_OBJECT
public:
  ProjectNewLocal(QWidget *parent=0, const char *name=0);
  ~ProjectNewLocal();

  KURL::List files();
  KURL::List projectFiles();

public slots:
  void slotSetFiles(bool);
  void setBaseURL(const KURL& a_baseURL);
  void slotAddFolder();
  void slotAddFiles();
  void slotClearList();
  void slotInsertFolderAfterCopying(const KURL::List&);
  void slotInsertFilesAfterCopying(const KURL::List&);
  void slotDeleteCopyToDialog(CopyTo*);

  virtual void resizeEvent( QResizeEvent * );

private:
  KURL baseURL;
  KURL::List fileList;
};

#endif
