/***************************************************************************
                          filestreeview.h  -  description
                             -------------------
    begin                : Thu Jun 1 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
                           (C) 2001,2002 Andras Mantia
    email                : pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com, amantia@freemail.hu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef FILESTREEVIEW_H
#define FILESTREEVIEW_H

#include <qwidget.h>
#include <qvaluelist.h>
#include <qptrlist.h>

#include <kconfig.h>
#include <kpropertiesdialog.h>

#include "filemanage.h"

class KURL;

class FilesTreeFolder;
class QuantaPropertiesPageDlg;

/**
  *@author Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon & Andras Mantia
  */


class FilesTreeView : public FileManage  {
   Q_OBJECT

public:
  FilesTreeView(QWidget *parent=0L, const char *name=0L):FileManage(parent,name){};
  FilesTreeView(KURL::List topList, QWidget *parent=0L, const char *name=0L);
  virtual ~FilesTreeView();
  virtual KURL currentURL();

  /** No descriptions */
  void readDir(const KURL& url);
  /** Get a list of items for whom the url == p_url. */
  QPtrList<QListViewItem> getItems(const KURL& p_url);

  KURL::List topURLList;

public slots:
  virtual void slotMenu(QListViewItem *, const QPoint &, int);
  virtual void slotSelectFile(QListViewItem *);
  void slotSelectAnyFile(QListViewItem *);
  void slotSelectImage(QListViewItem *);
  void slotAddToTop();
  void slotNewTopFolder();
  /** No descriptions */
  virtual void slotInsertTag();
  virtual void slotReload();
  virtual void slotProperties();

protected:
  virtual void itemRenamed(const KURL& , const KURL& ) {};
  void addFileInfoPage(KPropertiesDialog *propDlg);

protected slots: // Private slots
  /** No descriptions */
  virtual void slotDirListNewItems(const KFileItemList& items);
  /** No descriptions */
  virtual void slotDirListDeleteItem(KFileItem *item);

signals:
  void activatePreview();
  void openFile(const KURL&, const QString& );
  void openImage(const KURL&);
};

#endif
