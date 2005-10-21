/***************************************************************************
                            variablelistview.h
                         ------------------------
    begin                : 2004-04-04
    copyright            : (C) 2004 Thiago Silva

 ***************************************************************************/

/****************************************************************************
 *                                                                          *
 *   This program is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation; either version 2 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 ***************************************************************************/


#ifndef VARIABLESLISTVIEW_H
#define VARIABLESLISTVIEW_H

#include <klistview.h>
#include <kpopupmenu.h>
#include <qptrlist.h>

class DebuggerVariable;

class VariablesListView : public KListView
{
  Q_OBJECT

  enum menuitems 
  {
    setValue = 1,
    dumpValue,
    copyValue,
    removeWatch
  };
      
  public:
    VariablesListView(QWidget *parent = 0, const char *name = 0);
    ~VariablesListView();

    void addVariable(DebuggerVariable* variable);
    DebuggerVariable* selected(bool traverse = false);

    DebuggerVariable* first() { return m_variablesList.first(); }
    DebuggerVariable* next()  { return m_variablesList.next(); }
    
    void clear();

  public slots:
    void slotRemoveSelected();
    void slotVariableSetValue();
    void slotVariableDump();
    void slotVariableCopyToClipboard();
    void slotVariableContextMenu(KListView *list, QListViewItem * item, const QPoint& point);

  signals:
    void valueChanged(DebuggerVariable*);
    void removeVariable(DebuggerVariable*);

  private:
    void keyPressEvent(QKeyEvent *e);
    void replaceVariable(DebuggerVariable* oldvar, DebuggerVariable* newvar);

    QPtrList<DebuggerVariable> m_variablesList;
    KPopupMenu *m_variablePopup;
};

#endif
