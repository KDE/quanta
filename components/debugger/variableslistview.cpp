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
// #include <kpassivepopup.h>
// #include <kdatetbl.h>

// Quanta includes
#include "variableslistview.h"
#include "debuggervariable.h"
#include "resource.h"
#include "quanta.h"
#include "messageoutput.h"
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
  setSorting(-1); // No sorting
//   setTooltipColumn(0);
//   setShowToolTips(true);
  
  m_variablePopup = new KPopupMenu(this);
  m_variablePopup->insertItem(SmallIcon("editdelete"), i18n("&Remove"), this, SLOT(slotRemoveSelected()), 0, removeWatch);

  if(quantaApp->debugger()->client()->supports(DebuggerClientCapabilities::VariableSetValue))
    m_variablePopup->insertItem(SmallIcon("edit"), i18n("&Set Value"), this, SLOT(slotVariableSetValue()), 0, setValue);

  m_variablePopup->insertItem(SmallIcon("viewmag"), i18n("&Dump in messages log"), this, SLOT(slotVariableDump()), 0, dumpValue);

  connect(this, SIGNAL( contextMenu( KListView *, QListViewItem *, const QPoint & ) ), this, SLOT(slotVariableContextMenu(KListView *, QListViewItem *, const QPoint &)));
}


VariablesListView::~VariablesListView()
{}

DebuggerVariable* VariablesListView::selected(bool traverse)
{
  if(!selectedItem())
    return NULL;

  DebuggerVariable* v, *found;
  for( v = m_variablesList.first(); v; v = m_variablesList.next())
  {
    found = v->findItem(selectedItem(), traverse);
    if(found)
      return found;
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
  if(!variable)
  {
    kdDebug(24002) << k_funcinfo << " Tried to show NULL variable!" << endl;
    return;
  }

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

  m_variablePopup->setItemEnabled(removeWatch, selected());
  if(quantaApp->debugger()->client()->supports(DebuggerClientCapabilities::VariableSetValue))
    m_variablePopup->setItemEnabled(setValue, selected());

  DebuggerVariable *v = selected(true);
  m_variablePopup->setItemEnabled(dumpValue, v && v->type() == DebuggerVariableTypes::String);

  m_variablePopup->popup(point);
}

void VariablesListView::slotVariableSetValue()
{
  if(!selected())
    return;

  DebuggerVariable v(selected());

  QString newvalue;
  switch(v.type())
  {
    case DebuggerVariableTypes::String:
      newvalue = "\"" + v.value() + "\"";
      break;

    case DebuggerVariableTypes::Float:
    case DebuggerVariableTypes::Boolean:
    case DebuggerVariableTypes::Integer:
    //case DebuggerVariableTypes::Array:
      newvalue = v.value();
      break;

    default:
      newvalue = "";
  }
  newvalue = KInputDialog::getMultiLineText(i18n("Set Variable"), i18n("New value:"), newvalue, 0, this);
  if(newvalue.isNull())
    return;

  v.setValue(newvalue);
  quantaApp->debugger()->client()->variableSetValue(v);

}

void VariablesListView::slotVariableDump( )
{
  DebuggerVariable *v = selected(true);
  if(!v)
    return;

  quantaApp->messageOutput()->showMessage(i18n("Contents of variable %1:\n>>>\n").arg(v->name()));
  quantaApp->messageOutput()->showMessage(v->value());
  quantaApp->messageOutput()->showMessage("<<<\n");
  
  
}

#include "variableslistview.moc"
