/***************************************************************************
                          scripttreeview.h  -  description
                             -------------------
    begin                : Thu Sep 16 2003
    copyright            : (C) 2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef SCRIPTTREEVIEW_H
#define SCRIPTTREEVIEW_H

//own includes
#include "filestreeview.h"

class ScriptTreeView : public FilesTreeView  {
   Q_OBJECT


public:
   ScriptTreeView(QWidget *parent = 0L, const char *name = 0L);
  ~ScriptTreeView();

public slots:
  virtual void slotMenu(KListView *listView, QListViewItem *item, const QPoint &point);
  virtual void slotSelectFile(QListViewItem *item);
  void slotEditScript();
  void slotEditInQuanta();
  void slotEditDescription();
  void slotRun();
  void slotAssignAction();
  void slotSendScriptInMail();
  void slotProperties();

signals:
  void openFileInPreview(const KURL&);
  void assignActionToScript(const KURL&, const QString&);

private:
  KURL infoFile(const KURL& url);
  QString infoOptionValue(const KURL& infoURL, const QString& optionName);

  FilesTreeBranch *m_globalDir;
  FilesTreeBranch *m_localDir;

};

#endif
