/***************************************************************************
                          projectupload.h  -  description
                             -------------------
    begin                : Wed Nov 15 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
    email                : pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PROJECTUPLOAD_H
#define PROJECTUPLOAD_H

#include "projectuploads.h"
#include "project.h"
#include <qstringlist.h>
#include <kio/job.h>
#include "../treeviews/uploadtreeview.h"

/**
  *@author Dmitry Poplavsky & Alexander Yakovlev
  */

class ProjectUpload : public ProjectUploadS  {
Q_OBJECT
public:
  ProjectUpload(QString file, Project* p, QWidget *parent = 0, const char * name = 0, bool modal = FALSE, WFlags fl = 0 );
  ProjectUpload( Project* , QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
  ~ProjectUpload();

protected slots:
  void startUpload();
  void upload();
  void uploadFinished( KIO::Job *job );
  void uploadProgress ( KIO::Job *job, unsigned long percent );
  void uploadMessage ( KIO::Job *, const QString & msg );

  void clearSelection();
  void selectAll();
  void selectModified();
  void invertSelection();
  void expandAll();
  void collapseAll();

  virtual void resizeEvent( QResizeEvent * );
  virtual void reject();

private:	
  int selectedItemCount( QListViewItem *, int = 0 );
  void buildSelectedItemList( QListViewItem *, QString );

  QStringList files;    // list of all files
  QStringList modified; // modified files
  QStringList toUpload; // list of files , still didn't uploaded
  QString currentFile;  // file in progress of upload
  QStringList madeDirs;
  Project *p;
  KURL *baseUrl;
  bool stopUpload;
  bool uploadInProgress;
  bool suspendUpload;

  void initProjectInfo(Project *p);

private slots: // Private slots
  /** No descriptions */
  void slotUploadNext();

signals: // Signals
  /** No descriptions */
  void uploadNext();
};

#endif
