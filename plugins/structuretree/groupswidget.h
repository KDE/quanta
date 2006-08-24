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

#ifndef GROUPS_WIDGET_H
#define GROUPS_WIDGET_H

#include <k3listview.h>
// #include <kdebug.h>

#include <QList>
//#include <QString>

//class KDevProject;
class KMenu;
class AreaStruct;
struct ParseResult;
struct DTDStruct;
struct GroupStorage;
class KDevPlugin;
class KDevDocumentController;
class QuantaCoreIf;
class KAction;


/** 
@short a K3ListView for the group elements found by the parser

It does not build the tree when it is not visible but delays the build until it gets the show event.

@author Jens Herden <jens@kdewebdev.org>
 */

class GroupsWidget: public K3ListView
{
  Q_OBJECT
public:  
  GroupsWidget(KDevPlugin *plugin, QWidget *parent);
  ~GroupsWidget();

public slots:
  
  /** disables user interaction with the widget
   */
  void slotBlockGUI();
  
  /** clears the tree if parseResult == 0
   * 
   * @param parseResult 
   */
  void slotBuild(const ParseResult *parseResult);

  /** Build the groups trees new from a parseResult.
   *  If the widget it not visible this will be delayed until the show event arrives.
   * @param parseResult 
   */
  void slotGroupsParsed(const ParseResult *parseResult);
  
  /** adjust the tree to new settings
   */
  void slotNewSettings();

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

  /**
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
  
  /** open the if associated to the current item, if possible
   */
  void slotOpenFile();
  
  void slotCopy();
  
  void slotCut();
  
  void slotPaste();
  
private:
  
  /** request to move the cursor in the document to a new position
   * 
   * @param qp qp.x() = line and qp.y() = column
   */
  void setCursorPosition(const QPoint & qp);

  /** request to select an area in the document
     * 
     * @param area 
   */
  void selectArea(const AreaStruct &area);

  /** reimplemented to build the tree if needed
   * 
   * @param ev 
   */
  void showEvent ( QShowEvent *ev );

  /** build the tree from the parseResult
   * 
   * @param parseResult 
   */
  void build(const ParseResult *parseResult);
  
  /** create a branch for a script group
   * 
   * @param dtd 
   * @param groupStorage 
   */
  void createScriptBranch(const DTDStruct *dtd, const GroupStorage *groupStorage);
  
  /** create a branch for a XML group
   * 
   * @param dtd 
   * @param groupStorage 
   */
  void createXMLBranch(const DTDStruct *dtd, const GroupStorage *groupStorage);
  
  KMenu * m_popupMenu;
  KMenu * m_dtdMenu;
  bool m_dirty;
  const ParseResult * m_parseResult;
  KDevPlugin * m_plugin;
  KDevDocumentController * m_partController;
  int openFileMenuId;
  QuantaCoreIf * m_qcore;
  
  KAction * m_copyAction;
  KAction * m_cutAction;
  KAction * m_pasteAction;
};


#endif

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
