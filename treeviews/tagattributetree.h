//
//
// C++ Interface: h
//
// Description:
//
//
// Author: Andras Mantia <amantia@freemail.hu>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TAGATTRIBUTETREE_H
#define TAGATTRIBUTETREE_H

//qt includes
#include <qwidget.h>

//kde includes
#include <klistview.h>

//app includes

class KListView;
class QListViewItem;
class ParentItem;
class Node;

class EditableTree : public KListView {
Q_OBJECT

public:
  EditableTree(QWidget *parent = 0, const char *name = 0);
  virtual ~EditableTree();

  virtual void setCurrentItem( QListViewItem *i);
  void setEditable(bool b) {m_editable = b;}
  bool isEditable() {return m_editable;}

public slots:
  virtual void editorContentChanged();


protected:
  bool m_editable;

};

class DualEditableTree : public EditableTree
{
  Q_OBJECT
public:
  DualEditableTree(QWidget *parent = 0, const char *name = 0);
  virtual ~DualEditableTree();

  virtual void setCurrentItem ( QListViewItem *i);
  int currentColumn() {return curCol;}

protected:
  virtual bool eventFilter(QObject *object, QEvent *event);
  virtual void resizeEvent(QResizeEvent *);

signals:
  void itemModified( QListViewItem *);

public slots:
  virtual void editorContentChanged();

private slots:
  void itemClicked(QListViewItem *i, const QPoint &point, int col);

private:
  int curCol;
};

class TagAttributeTree : public EditableTree  {
   Q_OBJECT

public:
  TagAttributeTree(QWidget *parent=0, const char *name=0);
  virtual ~TagAttributeTree();

  void setCurrentNode(Node *node);
  //virtual void setCurrentItem( QListViewItem *i );
  Node *node() const {return m_node;}

public slots:
  void slotDelayedSetCurrentNode();
  void slotParentSelected(int index);
  virtual void editorContentChanged();

private slots:
  void slotCollapsed(QListViewItem *item);
  void slotExpanded(QListViewItem *item);

signals:
  void newNodeSelected(Node *node);

private:
  Node *m_node;
  Node *m_newNode;
  bool rebuildEnabled;
  ParentItem *m_parentItem;
};

#endif

