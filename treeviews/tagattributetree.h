/***************************************************************************
                             tagattributetree.h
                             -------------------
    copyright            : (C) 2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

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
class QLabel;
class KPushButton;
class QGridLayout;

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
  virtual void focusInEvent(QFocusEvent *);
  virtual void focusOutEvent(QFocusEvent *);

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

//An enhanced TagAttributeTree with the tag name and buttons to delete the tag.
class EnhancedTagAttributeTree : public QWidget
{
Q_OBJECT
public:
  EnhancedTagAttributeTree(QWidget *parent = 0, const char *name = 0);
  virtual ~EnhancedTagAttributeTree();

  void setCurrentNode(Node *node);
  Node *node() const {return attrTree->node();}

signals:
  void newNodeSelected(Node *node);

private slots:
  void NodeSelected(Node *node);
  void deleteNode();
  void deleteSubTree();

private:
  void showCaption();

  QLabel *nodeName;
  KPushButton* deleteAll;
  KPushButton* deleteTag;
  TagAttributeTree *attrTree;
  QGridLayout *widgetLayout;
  Node *curNode;
};

#endif

