/***************************************************************************
                          structtreeview.h  -  description
                             -------------------
    begin                : Sat Apr 29 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
    email                : pdima@mail.univ.kiev.ua
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

#include <qwidget.h>
#include <qlistview.h>

/**view class of document structure
  *@author Yacovlev Alexander & Dmitry Poplavsky
  */

class Node;
class StructTreeTag;
class Parser;
class QPopupMenu;
class QuantaApp;
class KConfig;

class StructTreeView : public QListView  {
   Q_OBJECT
friend class QuantaApp;
public: 
	StructTreeView(Parser *parser, KConfig *config, QWidget *parent=0, const char *name=0);
	~StructTreeView();
	
  /** create items in the level */
  void createList(Node *node, StructTreeTag *parent = 0L, int openLevel = 3);	

  void setFollowCursor(bool);
  bool followCursor() { return followCursorFlag; }

	
public slots: // Public slots
  /** repaint document structure */
  void slotReparse(Node* node, int openLevel = 3 );
  void slotFollowTag( QListViewItem *item );
  void slotMouseClicked(int button, QListViewItem*, const QPoint&, int);
  void slotDoubleClicked( QListViewItem * );
  void slotOnTag( QListViewItem *);

  // slots for RBM menu
  void slotReparse();
  void slotSelectTag();
  void slotGotoEndOfTag();
  void slotOpenSubTree();
  void slotCloseSubTree();
  void showTagAtPos(int x, int y);
  void changeFollowCursor() { setFollowCursor( !followCursor() ); }

signals:
  void newCursorPosition( int col, int row );
  void selectArea(int col1, int row1, int col2, int row2 );
  void needReparse();
  void onTag( const QString &tag );

public:
	StructTreeTag *top;
	StructTreeTag *images;
	StructTreeTag *links;
	
	Parser *parser;

	bool topOpened;
	bool imagesOpened;
	bool linksOpened;
private:
  int imagesCount; // to check if there are images
  int linksCount;	
  int followCursorId;
  bool followCursorFlag;

  QPopupMenu *popupMenu;

  StructTreeTag *lastTag;

  void setOpenSubTree( QListViewItem *it, bool open);

  KConfig *config;
};

#endif
