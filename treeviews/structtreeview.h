/***************************************************************************
                          structtreeview.h  -  description
                             -------------------
    begin                : Sat Apr 29 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
                           (C) 2002, 2003 Andras Mantia
    email                : pdima@mail.univ.kiev.ua, amantia@freemail.hu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef STRUCTTREEVIEW_H
#define STRUCTTREEVIEW_H

#include <qmap.h>
#include <qwidget.h>

#include <klistview.h>

/**view class of document structure
  *@author Andras Mantia & Yacovlev Alexander & Dmitry Poplavsky
  */

class Node;
class StructTreeTag;
class Parser;
class KPopupMenu;
class QuantaApp;
class KConfig;
class QTime;

class StructTreeView : public KListView  {
   Q_OBJECT
friend class QuantaApp;
public:
  StructTreeView(KConfig *config, QWidget *parent=0, const char *name=0);
  ~StructTreeView();


  void setFollowCursor(bool);
  bool followCursor() { return followCursorFlag; }
  /** Show the element in tree according to cursor position (x,y) */
  void showTagAtPos(Node *node);
  /** Delete the items */
  void deleteList();
  /** Set the View as... menu to dtdName. */
  void setParsingDTD(const QString dtdName);

  StructTreeTag *top;
  StructTreeTag *groups[5];
  uint groupsCount;

  bool topOpened;
  bool groupOpened[5];
  bool useOpenLevelSetting;


public slots: // Public slots
  /** repaint document structure */
  void slotReparse(Document *w, Node* node, int openLevel = 3 );
  void slotMouseClicked(int button, QListViewItem*, const QPoint&, int);
  void slotDoubleClicked( QListViewItem * );

  // slots for RBM menu
  void slotReparse();
  void slotSelectTag();
  void slotGotoTag( QListViewItem *item );
  void slotGotoClosingTag();
  /** Recursively open the tree and all its subtrees */
  void slotOpenSubTree();
  /** Recursively close the tree and all its subtrees */
  void slotCloseSubTree();
  void changeFollowCursor() { setFollowCursor(!followCursorFlag); }
  /** No descriptions */
  void slotCollapsed(QListViewItem *item);
  /** No descriptions */
  void slotExpanded(QListViewItem *);
  void slotOpenFile();

signals:
  void newCursorPosition( int col, int row );
  void selectArea(int col1, int row1, int col2, int row2 );
  void needReparse();
  void onTag( const QString &tag );
  /** No descriptions */
  void parsingDTDChanged(const QString&);
  void openFile(const KURL&, const QString& );
  void openImage(const KURL&);
  void activatePreview();

private:
  /** builds the structure tree */
  void buildTree(Node *baseNode, int openLevel);
  /** Do the recursive opening or closing of the trees */
  void setOpenSubTree( QListViewItem *it, bool open);

  int followCursorId;
  bool followCursorFlag;

  KPopupMenu *popupMenu;
  KPopupMenu *dtdMenu;
  StructTreeTag *lastTag;
  KConfig *config;
  QStringList dtdList;
  int openFileMenuId;
  QMap<QString, uint> groupIds;

protected: // Protected methods
  /** Do a reparse before showing. */
  virtual void showEvent(QShowEvent*);
protected slots: // Protected slots
  /** The treeview DTD  has changed to id. */
  void slotDTDChanged(int id);
protected: // Protected attributes
  /**  */
  DTDStruct* m_parsingDTD;
  Document *write;
  QTime timer;
};

#endif
