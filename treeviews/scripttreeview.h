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
  void slotAssignAction();
  void slotSendScriptInMail();

signals:
  void openFileInPreview(const KURL&);
  void assignActionToScript(const KURL&, const QString&);

private:
  KURL infoFile(const KURL& a_url);
  QString infoOptionValue(const KURL& a_infoURL, const QString& a_optionName);

  FilesTreeFolder *m_globalDir;
  FilesTreeFolder *m_localDir;
};

#endif
