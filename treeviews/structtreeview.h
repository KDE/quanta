/***************************************************************************
                          structtreeview.h  -  description
                             -------------------
    begin                : Sat Apr 29 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky <pdima@mail.univ.kiev.ua>
                           (C) 2002, 2003 Andras Mantia <amantia@kde.org>
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
#include <qvaluelist.h>

#include <klistview.h>

/**view class of document structure
 *@author Andras Mantia & Paulo Moura Guedes & Yacovlev Alexander & Dmitry Poplavsky
 */

class KPopupMenu;
class KConfig;
class KURL;
class QTime;

class Node;
class StructTreeTag;
class Parser;
class Document;
struct DTDStruct;

class StructTreeView : public KListView  {
   Q_OBJECT
public:

  /**
   *  since this class is a singleton you must use this function to access it
   *
   *  the parameters are only used at the first call to create the class
   *
   */
  static StructTreeView* const ref(QWidget *parent = 0L, const char *name = 0L)
  {
    static StructTreeView *m_ref;
    if (!m_ref) m_ref = new StructTreeView (parent, name);
    return m_ref;
  }

  ~StructTreeView();


  void setFollowCursor(bool);
  bool followCursor() { return followCursorFlag; }
  /** Show the element in tree according to cursor position (x,y) */
  void showTagAtPos(Node *node);
  /** Delete the items */
  void deleteList(bool groupOnly);
  void setParsingDTDs(const QStringList &parsingDTDList);
  void showMessage(const QString& message);
  
  
  bool useOpenLevelSetting;

public slots: // Public slots
  /** repaint document structure */
  void slotReparse(Document *w, Node* node, int openLevel = 3,bool groupOnly=false);
  void slotMouseClicked(int button, QListViewItem*, const QPoint&, int);
  void slotDoubleClicked( QListViewItem * );

  // slots for RBM menu
  void slotReparseMenuItem();
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
  void slotNodeTreeChanged();
  void slotRemoveTags();

signals:
  void newCursorPosition( int col, int row );
  void selectTagArea(Node* node);
  void selectArea(int col1, int row1, int col2, int row2 );
  void needReparse();
  void onTag( const QString &tag );
  void showGroupsForDTEP(const QString& dtep, bool show);
  void openFile(const KURL&);
  void openImage(const KURL&);
  void showProblemMessage(const QString&);
  void clearProblemOutput();

private:

  /** The constructor is private because we use singleton pattern.
   *  If you need the class use StructTreeView::ref() for
   *  construction and reference
   */
  StructTreeView(QWidget *parent=0, const char *name=0);
  /** builds the structure tree */
  void buildTree(Node *baseNode, int openLevel, bool groupOnly);
  /** Do the recursive opening or closing of the trees */
  void setOpenSubTree(QListViewItem *it, bool open);

  /** Make ctrl have the same behavior has shift */
  void setContiguousSelectedItems();
  
  void copySelectedItems(QListViewItem* parent, QListViewItem* after);
  void moveSelectedItems(QListViewItem* parent, QListViewItem* after);
  
private:

  int followCursorId;
  bool followCursorFlag;

  KPopupMenu *popupMenu;
  KPopupMenu *emptyAreaMenu;
  KPopupMenu *dtdMenu;
  StructTreeTag *lastTag;
  KConfig *config;
  QStringList dtdList;
  QValueList<const DTDStruct*> m_parsingDTDList;
  int openFileMenuId;
  QMap<QString, uint> groupIds;
  bool m_dirty;
  StructTreeTag *top;
  QValueList<StructTreeTag*> groups;
  uint groupsCount;

  bool topOpened;
  QValueList<bool> groupOpened;
  
  mutable QListViewItem* m_marker;       //track that has the drag/drop marker under it
  QListViewItem* m_draggedItem;
//   mutable QWidget* m_thisWidget; // non const this hack for acceptDrag

protected: // Protected methods
  /** Do a reparse before showing. */
  virtual void showEvent(QShowEvent*);
  /** Clear the problem output when hiding. */
  virtual void hideEvent(QHideEvent*);
  
  // drag functions  
  virtual bool acceptDrag(QDropEvent* e) const;
  virtual void startDrag();
  virtual void contentsMousePressEvent(QMouseEvent* e);

protected slots: // Protected slots
  /** The treeview DTD  has changed to id. */
  void slotDTDChanged(int id);
    
  void slotMouseClickedVPL(int button, QListViewItem*, const QPoint&, int);
  virtual void slotDropped(QDropEvent* e, QListViewItem* parent, QListViewItem* after);

protected: // Protected attributes
  Document *write;
  QTime *timer;
};

#endif
