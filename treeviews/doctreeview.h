/***************************************************************************
                          doctreeview.h  -  description
                             -------------------
    begin                : Sat Mar 4 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky <pdima@mail.univ.kiev.ua>
                           (C) 2002, 2004 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef DOCTREEVIEW_H
#define DOCTREEVIEW_H

//qt includes
#include <qdict.h>
#include <qvaluelist.h>

//kde includes
#include <klistview.h>

class DocFolder;
class KPopupMenu;


/**
  *@author Yacovlev Alexander & Dmitry Poplavsky
  */

class DocTreeView : public KListView  {
   Q_OBJECT
public:
  DocTreeView(QWidget *parent=0, const char *name=0);
  ~DocTreeView();

  QString *contextHelp(const QString &keyword);

public slots:
  void slotNewProjectLoaded(const QString &, const KURL &, const KURL &);
  void slotReloadProjectDocs();
  void slotAddProjectDoc(const KURL& url);
  /** Re-reads the documentation directories. */
  void slotRefreshTree();

signals:
  void openURL(const QString& );
  void reloadProjectDocs();
  void downloadDoc();

private slots:
  void clickItem( QListViewItem *);
  void slotDoubleClicked(QListViewItem *);
  void slotMenu(KListView *, QListViewItem *item, const QPoint &point);


private:

  QDict<QString> *contextHelpDict;
  QValueList<DocFolder *> m_folderList;
  KListViewItem *projectDocFolder;
  KPopupMenu *m_contextMenu;
  int m_menuReload;
};

#endif
