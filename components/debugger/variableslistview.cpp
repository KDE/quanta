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
#include <kiconloader.h>
#include <kdeversion.h>
#include <kinputdialog.h>

// Quanta includes
#include "variableslistview.h"
#include "debuggervariable.h"
#include "resource.h"
#include "quanta.h"
#include "debuggerclient.h"
#include "debuggermanager.h"

namespace VariablesListViewColumns
{
  // The enums must correspond to the order of the columns
  // If you change here, change the column adding
  enum Columns
  {
    Name = 0,
    Value,
    Type,
    Size

  };
}

VariablesListView::VariablesListView(QWidget *parent, const char *name)
    : KListView(parent, name)
{
  // If you change here, change the VariablesListViewColumns enums above
  addColumn(i18n("Name"));
  addColumn(i18n("Value"));
  addColumn(i18n("Type"));
  addColumn(i18n("Size"));
  setRootIsDecorated(true);

  m_variablePopup = new KPopupMenu(this);
  m_variablePopup->insertItem(SmallIcon("editdelete"), i18n("&Remove"), this, SLOT(slotRemoveSelected()));

  if(quantaApp->debugger()->client()->supports(DebuggerClientCapabilities::VariableSetValue))
    m_variablePopup->insertItem(SmallIcon("edit"), i18n("&Set Value"), this, SLOT(slotVariableSetValue()));

  connect(this, SIGNAL( contextMenu( KListView *, QListViewItem *, const QPoint & ) ), this, SLOT(slotVariableContextMenu(KListView *, QListViewItem *, const QPoint &)));
}


VariablesListView::~VariablesListView()
{}

DebuggerVariable* VariablesListView::selected()
{
  if(!selectedItem())
    return NULL;

  DebuggerVariable* v;
  for( v = m_variablesList.first(); v; v = m_variablesList.next())
  {
    if(v->item() == selectedItem())
    {
      return v;
    }
  }
  return NULL;
}

void VariablesListView::slotRemoveSelected()
{
  DebuggerVariable* v = selected();

  if(!v)
    return;

  emit removeVariable(v);
  m_variablesList.remove(v);
  delete v;
}

void VariablesListView::keyPressEvent(QKeyEvent *e)
{
  if(e->key() != Qt::Key_Delete)
  {
    e->ignore();
    return;
  }

  DebuggerVariable* v = selected();

  if(!v)
    return;

  emit removeVariable(v);
  m_variablesList.remove(v);
  delete v;
}

void VariablesListView::addVariable(DebuggerVariable* variable)
{

  // Remove the old varible if its there
  DebuggerVariable* v;
  for( v = m_variablesList.first(); v; v = m_variablesList.next())
  {
    if(v->name() == variable->name())
    {
      m_variablesList.remove(v);
      delete v;

      break;
    }
  }

  // Insert the new variable
  m_variablesList.append(variable);

  KListViewItem * item = new KListViewItem(this);
  item->setText(VariablesListViewColumns::Name, variable->name());
  item->setText(VariablesListViewColumns::Type, variable->typeName());
  item->setText(VariablesListViewColumns::Size, variable->sizeName());
  variable->setItem(item);
  if(variable->isScalar())
    item->setText(VariablesListViewColumns::Value, variable->value());
  else
    addChild(item, variable);

  insertItem(item);

  //setVariables(newlist);
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
    item->setText(VariablesListViewColumns::Name, v->name());
    item->setText(VariablesListViewColumns::Type, v->typeName());
    item->setText(VariablesListViewColumns::Size, v->sizeName());
    v->setItem(item);
    if(v->isScalar())
      item->setText(VariablesListViewColumns::Value, v->value());
    else
      addChild(item, v);

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
    child->setItem(item);
    item->setText(VariablesListViewColumns::Name, child->name());
    item->setText(VariablesListViewColumns::Type, child->typeName());
    item->setText(VariablesListViewColumns::Size, child->sizeName());

    if(child->isScalar())
      item->setText(VariablesListViewColumns::Value, child->value());
    else
      addChild(item, child);
  }
}

void VariablesListView::parsePHPVariables(const QString &varstring)
{
  QString str = varstring;
  DebuggerVariable* var = parsePHPVariables(str);
  if(var)
    addVariable(var);

}
DebuggerVariable* VariablesListView::parsePHPVariables(QString &str)
{

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
    debuggervar = new DebuggerVariable(key, data, DebuggerVariableTypes::Integer);

  }
  else if(type == "b")
  {
    /* Example:
      s:8:"$boolvar";b:1;
    */
    data = str.left(str.find(';'));
    data = (data == "0" ? i18n("False"): i18n("True"));
    str.remove(0, str.find(';') + 1);
    debuggervar = new DebuggerVariable(key, data, DebuggerVariableTypes::Boolean);
  }
  else if(type == "N")
  {
    /* Example:
      s:6:"return";N;
    */
    debuggervar = new DebuggerVariable(key, i18n("<Undefined>"), DebuggerVariableTypes::Undefined);
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
    debuggervar = new DebuggerVariable(key, data, DebuggerVariableTypes::String, length);
  }
  else if(type == "a" || type == "O")
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
    debuggervar = new DebuggerVariable(key, data, DebuggerVariableTypes::Float);

  }
  else if(type == "-")
  {
    debuggervar = new DebuggerVariable(key,  i18n("<Undefined>"), DebuggerVariableTypes::Undefined);
  }
  else if(type == "!")
  {
    debuggervar = new DebuggerVariable(key,  i18n("<Error>"), DebuggerVariableTypes::Error);
  }
  else
  {
    kdDebug(24000) << "VariablesListView::parsePHPVariables: Unknown variable type " << type << endl;
    debuggervar = new DebuggerVariable(key, i18n("<Unimplemented type>"), DebuggerVariableTypes::Error);
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

void VariablesListView::slotVariableContextMenu(KListView *, QListViewItem *, const QPoint& point)
{
  if(!selectedItem())
    return;

  m_variablePopup->exec(point);
}

void VariablesListView::slotVariableSetValue()
{
  DebuggerVariable* v = selected();

  if(!v)
    return;

  QString newvalue;
  switch(v->type())
  {
    case DebuggerVariableTypes::String:
      newvalue = "\"" + v->value() + "\"";
      break;

    case DebuggerVariableTypes::Float:
    case DebuggerVariableTypes::Boolean:
    case DebuggerVariableTypes::Integer:
      newvalue = v->value();
      break;

    default:
      newvalue = "";
  }
  newvalue = KInputDialog::getText(i18n("Set variable"), i18n("New value"), newvalue, 0, this);
  if(newvalue.isNull())
    return;

  v->setValue(newvalue);
  quantaApp->debugger()->client()->variableSetValue(v);

}

#include "variableslistview.moc"
