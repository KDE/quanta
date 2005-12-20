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
#include <qclipboard.h>

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
    Status,
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
  addColumn(QString::null);
  addColumn(i18n("Value"));
  addColumn(i18n("Type"));
  addColumn(i18n("Size"));
  setRootIsDecorated(true);
  setSorting(-1); // No sorting
  
  m_variablePopup = new KPopupMenu(this);
  m_variablePopup->insertItem(SmallIcon("editdelete"), i18n("&Remove"), this, SLOT(slotRemoveSelected()), 0, removeWatch);

  if(quantaApp->debugger()->client()->supports(DebuggerClientCapabilities::VariableSetValue))
    m_variablePopup->insertItem(SmallIcon("edit"), i18n("&Set Value"), this, SLOT(slotVariableSetValue()), 0, setValue);

  m_variablePopup->insertItem(SmallIcon("viewmag"), i18n("&Dump in Messages Log"), this, SLOT(slotVariableDump()), 0, dumpValue);
  
  m_variablePopup->insertItem(SmallIcon("editcopy"), i18n("&Copy to Clipboard"), this, SLOT(slotVariableCopyToClipboard()), 0, copyValue);

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

  // Find the old variable in the tree if it is there 
  for(DebuggerVariable* v = m_variablesList.first(); v; v = m_variablesList.next())
  {
    if(v->name() == variable->name())
    {
      replaceVariable(v, variable);
      return;
    }
  }

  // Insert the new variable
  DebuggerVariable *newvar = new DebuggerVariable(variable);
  m_variablesList.append(newvar);

  KListViewItem * item = new KListViewItem(this);
  insertItem(item);
  newvar->setItem(item);
  replaceVariable(newvar, variable);
}

void VariablesListView::clear()
{
  KListView::clear();
  m_variablesList.clear();
}

void VariablesListView::replaceVariable(DebuggerVariable* oldvar, DebuggerVariable* newvar)
{ 
  KListViewItem * item;
  
  // Remove children that doesen't exist anymore
  QPtrList<DebuggerVariable> oldlist = oldvar->values();
  for(DebuggerVariable* oldchild = oldlist.last(); oldchild; oldchild = oldlist.prev())
  {
    bool found = false;
    QPtrList<DebuggerVariable> newlist = newvar->values();
    for(DebuggerVariable* newchild = newlist.last(); newchild; newchild = newlist.prev())
    {
      if(newchild->name() == oldchild->name())
      {
        found = true;
        break;
      }
    }
    if(!found)
      oldvar->deleteChild(oldchild);
  }

  // Update and add children
  QPtrList<DebuggerVariable> newlist = newvar->values();
  for(DebuggerVariable* newchild = newlist.last(); newchild; newchild = newlist.prev())
  {
    bool found = false;
    QPtrList<DebuggerVariable> oldlist = oldvar->values();
    for(DebuggerVariable* oldchild = oldlist.last(); oldchild; oldchild = oldlist.prev())  
    {
      if(newchild->name() == oldchild->name())
      {
        found = true;
        replaceVariable( oldchild, newchild);
        break;
      }
    }
    if(!found)
    {
      DebuggerVariable* child = new DebuggerVariable();
      item = new KListViewItem(oldvar->item());
      child->setItem(item);
      replaceVariable( child, newchild);
      oldvar->append(child);
    }
  }
  
  item = oldvar->item();
  
  if(oldvar->value() != newvar->value())
    item->setPixmap(VariablesListViewColumns::Status, SmallIcon("ok"));
  else
    item->setPixmap(VariablesListViewColumns::Status, KPixmap());
  
  oldvar->copy(newvar, false);
  
  item->setText(VariablesListViewColumns::Name, oldvar->name());
  item->setText(VariablesListViewColumns::Type, oldvar->typeName());
  item->setText(VariablesListViewColumns::Size, oldvar->sizeName());
  item->setText(VariablesListViewColumns::Value, (newvar->isScalar() ? oldvar->value() : QString()));
  
}

void VariablesListView::slotVariableContextMenu(KListView *, QListViewItem *, const QPoint& point)
{
  if(!selectedItem())
    return;

  m_variablePopup->setItemEnabled(removeWatch, selected());
  if(quantaApp->debugger()->client()->supports(DebuggerClientCapabilities::VariableSetValue))
    m_variablePopup->setItemEnabled(setValue, selected());

  DebuggerVariable *v = selected(true);
  m_variablePopup->setItemEnabled(dumpValue, v && v->isScalar());
  m_variablePopup->setItemEnabled(copyValue, v && v->isScalar());

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

void VariablesListView::slotVariableCopyToClipboard( )
{
  DebuggerVariable *v = selected(true);
  if(!v)
    return;
  QApplication::clipboard()->setText(v->value());
}

#include "variableslistview.moc"
