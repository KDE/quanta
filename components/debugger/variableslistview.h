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
  
  public:
    VariablesListView(QWidget *parent = 0, const char *name = 0);
    ~VariablesListView();
  
    //void preWatchUpdate();
    //void postWatchUpdate();
    
    void setVariables(const QPtrList<DebuggerVariable>& vars);
    void addVariable(DebuggerVariable* variable);
    void parsePHPVariables(const QString &varstring);
    DebuggerVariable* selected();

    void clear();
    
  public slots:
    void slotRemoveSelected();
    void slotVariableSetValue();
    void slotVariableContextMenu(KListView *list, QListViewItem * item, const QPoint& point);
    
  signals:
    void valueChanged(DebuggerVariable*);
    void removeVariable(DebuggerVariable*);
    
  private:  
    void keyPressEvent(QKeyEvent *e);
    void addChild(KListViewItem* parent, DebuggerVariable* var);
    DebuggerVariable* parsePHPVariables(QString &str);
    
    QPtrList<DebuggerVariable> m_variablesList;
    KPopupMenu *m_variablePopup;
};

#endif
