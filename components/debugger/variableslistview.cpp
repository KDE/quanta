/***************************************************************************
                          variableslistview.cpp
                         -----------------------
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

// KDE Includes
#include <klocale.h>

// Quanta includes
#include "variableslistview.h"
#include "debuggervariable.h"

VariablesListView::VariablesListView(QWidget *parent, const char *name)
 : KListView(parent, name)//QListView(parent, name)
{
  addColumn(i18n("Name"));
  addColumn(i18n("Value"));
  setRootIsDecorated(true);
}


VariablesListView::~VariablesListView()
{
}

void VariablesListView::addVariable(DebuggerVariable* variable)
{
  DebuggerVariable* v;
  
  for( v = m_variablesList.first(); v; v = m_variablesList.next())
  {
    if(v->name() == variable->name())
    {
      v->setValue(variable->value());
      v->setValues(variable->values());
      v->setType(variable->type());
      v->setReference(variable->isReference());
      
      variable = NULL;
      break;
    }
  }
  
  QPtrList<DebuggerVariable> newlist;
  newlist = m_variablesList;
  if(variable)
    newlist.append(variable);
  
  setVariables(newlist);
}
  
void VariablesListView::clear()
{
  KListView::clear();  
  m_variablesList.clear();
}

void VariablesListView::setVariables(const QPtrList<DebuggerVariable>& vars)
{
  clear();
  m_variablesList = vars;
  
  DebuggerVariable* v;
  KListViewItem* item;
  for( v = m_variablesList.first(); v; v = m_variablesList.next())
  {    
    item = new KListViewItem(this);
    item->setText(0, v->name());
    
    switch(v->type())
    {
      case DebuggerVariableTypes::Array:
      case DebuggerVariableTypes::Object:
        addChild(item, v);
        break;
      case DebuggerVariableTypes::Reference:
      case DebuggerVariableTypes::Resource:
      case DebuggerVariableTypes::Scalar:
        item->setText(1, v->value());
        break;
      default:
        item->setText(0, i18n("Unhandled variable type"));          
    }
    
    insertItem(item);
  }
}

void VariablesListView::addChild(KListViewItem* parent, DebuggerVariable* var)
{
  //QListViewItem* item;
  KListViewItem* item;
  DebuggerVariable* child;  
  QPtrList<DebuggerVariable> list = var->values();
  for(child = list.first(); child; child = list.next())
  {
    item = new KListViewItem(parent);
    item->setText(0, child->name());
    
    switch(child->type())
    {
      case DebuggerVariableTypes::Array:
      case DebuggerVariableTypes::Object:
        addChild(item, child);
        break;
      case DebuggerVariableTypes::Reference:
      case DebuggerVariableTypes::Resource:
      case DebuggerVariableTypes::Scalar:
        item->setText(1, child->value());
        break;
      default:
        item->setText(0, i18n("Unhandled variable type"));          
    }    
  }
}

void VariablesListView::parsePHPVariables(const QString &varstring)
{
  int pos = 0;
  setVariables(parsePHPVariables(varstring, &pos, 0));
}

QPtrList<DebuggerVariable>  VariablesListView::parsePHPVariables(const QString &varstring, int* pos, int level) {

  QPtrList<DebuggerVariable> newlist;
  return newlist;

}

#include "variableslistview.moc"
