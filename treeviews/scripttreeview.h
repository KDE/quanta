/***************************************************************************
                          scripttreeview.h  -  description
                             -------------------
    begin                : Thu Sep 16 2003
    copyright            : (C) 2003 by Andras Mantia
    email                : amantia@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef SCRIPTTREEVIEW_H
#define SCRIPTTREEVIEW_H

#include "filestreeview.h"

class QListViewItem;
class KURL;
class FilesTreeFolder;

class ScriptTreeView : public FilesTreeView  {
   Q_OBJECT
public:
   ScriptTreeView(QWidget *parent=0, const char *name=0);
  ~ScriptTreeView();

public slots:
  virtual void slotMenu(QListViewItem *, const QPoint &, int);
  virtual void slotSelectFile(QListViewItem *);
  void slotEditScript();
  void slotEditInQuanta();
  void slotEditDescription();
  void slotRun();
  void slotSendScriptInMail();

signals:
  void openFileInPreview(const KURL&);
  void hidePreview();

private:
  KURL infoFile(const KURL& a_url);

  FilesTreeFolder *m_globalDir;
  FilesTreeFolder *m_localDir;

};

#endif
