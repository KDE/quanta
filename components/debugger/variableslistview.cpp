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
#include <kdebug.h>

// Quanta includes
#include "variableslistview.h"
#include "debuggervariable.h"

VariablesListView::VariablesListView(QWidget *parent, const char *name)
 : KListView(parent, name)
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
    var->setItem(item);
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
  QString str = varstring;
  DebuggerVariable* var = parsePHPVariables(str);
  if(var)
    addVariable(var);

}
DebuggerVariable* VariablesListView::parsePHPVariables(QString &str) {

  QString key, data;
  QString tempstring;
  int length;
  DebuggerVariable* debuggervar = NULL;
 
  // get type of key
  QString type = str.left(1);
  str.remove(0, 2);
   
  // Strings
  if(type == "s")
  {
    // Get length of key
    tempstring = str.left(str.find(':'));
    str.remove(0, str.find(':') + 1);
    length = tempstring.toUInt();
  
    key = str.left(length + 1);
    key.remove(0, 1);        // remove starting quote
    str.remove(0, length + 3);
  }
  else if(type == "i")
  {
    key = str.left(str.find(';'));
    str.remove(0, str.find(';') + 1);
    
  }
  
  // Get type of data
  type = str.left(1);
  str.remove(0, 2);
   
  if(type == "i")
  {
    /* Example:
      s:4:"$row";i:6;
    */
    data = str.left(str.find(';'));
    str.remove(0, str.find(';') + 1);
    debuggervar = new DebuggerVariable(key, data, DebuggerVariableTypes::Scalar);
    
  } 
  else if(type == "s") 
  {
    /* Example:
      s:7:"$strvar";s:16:"This is a string";
    */
    
    // Get length of string
    tempstring = str.left(str.find(':'));
    str.remove(0, str.find(':') + 1);
    length = tempstring.toUInt();
  
    data = str.left(length + 1);
    data.remove(0, 1);        // remove starting quote
    str.remove(0, length + 3);  
    debuggervar = new DebuggerVariable(key, data, DebuggerVariableTypes::Scalar);
  }
  else if(type == "a")
  {
    /* Example:
      s:6:"$array";a:5:{s:11:"Ingredients";a:3:{i:0;s:8:"potatoes";i:1;s:4:"salt";i:2;s:6:"pepper";}s:6:"Guests";a:4:{i:0;s:5:"Fiona";i:1;s:4:"Tori";i:2;s:4:"Neil";i:3;s:4:"Nick";}s:4:"Dogs";a:4:{i:0;s:5:"Kitty";i:1;s:5:"Tessy";i:2;s:5:"Fanny";i:3;s:5:"Cosmo";}s:7:"Numbers";a:6:{i:0;i:1;i:1;i:2;i:2;i:3;i:3;i:9;i:4;i:8;i:5;i:7;}s:6:"Letter";s:1:"L";}
    */
    
    // Get length of array
    tempstring = str.left(str.find(':'));
    str.remove(0, str.find(':') + 2);
    length = tempstring.toUInt();
    
    QPtrList<DebuggerVariable> vars ;
    while(length > 0)
    {
      length --;
      DebuggerVariable* var = parsePHPVariables(str);
      if(var)
        vars.append(var);
        
    }
    str.remove(0, 1);
    debuggervar = new DebuggerVariable(key, vars, DebuggerVariableTypes::Array);
  }
  else if(type == "d")
  {
    /* Example:
      s:9:"$floatvar";d:12.5600000000000004973799150320701301097869873046875;"
    */
    data = str.left(str.find(';'));
    str.remove(0, str.find(';') + 1);
    debuggervar = new DebuggerVariable(key, data, DebuggerVariableTypes::Scalar);

  } 
  else if(type == "-")
  {
    debuggervar = new DebuggerVariable(key,  i18n("<Undefined>"), DebuggerVariableTypes::Scalar);
  } 
  else
  {
    debuggervar = new DebuggerVariable(key, i18n("<Unimplemented type>"), DebuggerVariableTypes::Scalar);
  }
 
  return debuggervar;

}

// This function should be called before watches are update (if they're updated in a batch)
// so that postWatchUpdate can remove obsolete variables from the list
/*void VariablesListView::preWatchUpdate() 
{
  DebuggerVariable* v;
  for( v = m_variablesList.first(); v; v = m_variablesList.next())  
    v->touch();
}*/

// This function removes obsolete variables from the tree
/*void VariablesListView::postWatchUpdate()
{
  DebuggerVariable* v;
  for( v = m_variablesList.last(); v; v = m_variablesList.prev())  
    if(v->touched())
    {
      if(v->item())
      {
        delete v->item();
        v->setItem(NULL);
      }
      m_variablesList.remove();
    }
}*/

#include "variableslistview.moc"
