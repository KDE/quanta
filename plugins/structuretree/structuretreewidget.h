/***************************************************************************
 *   Copyright (C) 2005 by Jens Herden                                     *
 *   jens@kdewebdev.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef STRUCTURETREE_WIDGET_H
#define STRUCTURETREE_WIDGET_H

#include <k3listview.h>
// #include <kdebug.h>

#include <QList>
//#include <QString>

//namespace Koncrete { class Project; }
class KMenu;
class AreaStruct;
struct ParseResult;
namespace KDevelop 
{ 
  class IPlugin; 
}
class KAction;
class QuantaCoreIf;


/**
  @short This treeview operates on a nodetree to display the structure of the   document.
  
  It does not build up the whole tree but all the items who are flagged as visible in the nodes.
  You can force a full build with @see populate().

  It does not build the tree when it is not visible but delays the build until it gets the show event.
  
  @author Jens Herden <jens@kdewebdev.org>
*/

class StructureTreeWidget: public K3ListView
{
  Q_OBJECT
public:  
  StructureTreeWidget(KDevelop::IPlugin *plugin, QWidget *parent);
  ~StructureTreeWidget();

  /**
   * 
   * @return true if the tree is fully populated
   */
  bool populated() const {return m_populated;};
  
public slots:
  /**
   * disables user interaction with the widget
   */
  void slotBlockGUI();
  
  /**
   * if the widget is visible build up the structure tree from a parseResult
   * otherwise remember and do it when shown @see showEvent()
   * 
   * @param parseResult 
   */
  void slotBuild(const ParseResult *parseResult);

  /**
   * adjust the tree to new settings
   */
  void slotNewSettings();

  /**
   * makes the node at the cursor position visible if this feature 
   * is activated
   * @param qp qp.x() = line and qp.y() = column
   */
  void slotNewCursorPosition(const QPoint & qp);

  /**
   * fully populates the tree 
   */
  void populate();
  
protected:

  void showEvent ( QShowEvent *ev );


private slots:
  
  /** enables user interaction with the widget
   */
  void slotUnblockGUI();
  
  /** called when an item was expanded 
   *
   * @param item 
   */
  void slotExpanded(Q3ListViewItem * item);
  
  /** called when the context menu is wanted
   * 
   * @param view this view
   * @param item is the item for which the menu should be shown. May be 0L.
   * @param point the point at which the menu should be shown
   */
  void slotContextMenu(K3ListView * view, Q3ListViewItem * item, const QPoint & point);

  /** opens the entire subtree of the current item
   */
  void slotOpenSubTree();

  /** closes the entire subtree of the current item
   */
  void slotCloseSubTree();
  
  /**
   * 
   * @param item  the pointer to the executed listview item.
   */
  void slotDoubleClicked(Q3ListViewItem *item);

  /** 
   * @param button 
   * @param item 
   * @param point 
   * @param col 
   */
  void slotMouseClicked(int button, Q3ListViewItem *item, const QPoint& point, int col);

  /** emit a signal to move the cursor of the active document 
   *
   * @param item 
   */
  void gotoTag(Q3ListViewItem *item);
  
  /** select the tag of the current item in editor
   */
  void slotSelectTag();
  
  /** change the follow cursor flag
   */
  void slotChangeFollowCursor();
  
  /** moves the cursor to the ending tag
   */
  void slotGotoClosingTag();
  
  void slotPaste();
  
  void slotCopy();
  
  void slotCut();

private:
  
  /**
   * move the cursor in the document to a new position
   * @param qp qp.x() = line and qp.y() = column
   */
   void setCursorPosition(const QPoint & qp);

  /**
    * select an area in the document
    * @param area 
    */
  void selectArea(const AreaStruct &area);
  
  void build(const ParseResult *parseResult);
 
  KMenu * m_popupMenu;
  int m_followCursorId; ///< menu id 
  bool m_dirty; ///< flag to indicate that the tree must be build on show event
  bool m_populated; ///< flag to indicate that the tree is fully populated
  const ParseResult * m_parseResult;
  QuantaCoreIf * m_qcore;
  KDevelop::IPlugin *m_plugin;
  KAction * m_copyAction;
  KAction * m_cutAction;
  KAction * m_pasteAction;
};


#endif

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
