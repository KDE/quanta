/***************************************************************************
                             debuggerui.cpp
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

#include "debuggerui.h"

#include "variableslistview.h"
#include "debuggervariable.h"

#include "quantacommon.h"
#include "quanta.h"
#include "resource.h"

#include <kiconloader.h>
#include <kdockwidget.h>
#include <klocale.h>
#include <qstring.h>
#include <kmditoolviewaccessor.h>

DebuggerUI::DebuggerUI(QObject *parent, const char *name)
    : QObject(parent, name), m_variablesListView(0)
{
  m_variablesListView = new VariablesListView(quantaApp->getMainDockWidget(), "debuggerVariables");
  m_variablesListView->setIcon(SmallIcon("math_brace"));
  m_variablesListView->setCaption(i18n("Variables"));
  m_variableListViewTVA = quantaApp->addToolWindow(m_variablesListView, KDockWidget::DockLeft, quantaApp->getMainDockWidget());
  showMenu();
  quantaApp->showToolbarFile(KURL().fromPathOrURL(qConfig.globalDataDir +resourceDir + "toolbars/debug.toolbar.tgz"));

  connect(m_variablesListView, SIGNAL(removeVariable(DebuggerVariable* )), parent, SLOT(slotRemoveVariable(DebuggerVariable* )));

}

DebuggerUI::~DebuggerUI()
{
  hideMenu();
  quantaApp->slotRemoveToolbar("debug");
  quantaApp->deleteToolWindow(m_variableListViewTVA);
  m_variableListViewTVA = 0L;
  // delete m_variablesListView;
}

void DebuggerUI::showMenu()
{
  QPopupMenu* debuggerMenu = (QPopupMenu*)(quantaApp->guiFactory())->container("debugger_menu", quantaApp);
  if(debuggerMenu)
    m_debuggerMenuID = quantaApp->menuBar()->insertItem(i18n("Deb&ug"), debuggerMenu, -1, 5);
  else
    m_debuggerMenuID  = 0;
}

void DebuggerUI::hideMenu()
{
  if(m_debuggerMenuID != 0)
    quantaApp->menuBar()->removeItem(m_debuggerMenuID);
  m_debuggerMenuID = 0;
}

void DebuggerUI::setVariables(const QPtrList<DebuggerVariable>& vars)
{
  m_variablesListView->clear();
  m_variablesListView->setVariables(vars);
}

void DebuggerUI::addVariable(DebuggerVariable* var)
{
  m_variablesListView->addVariable(var);
}

void DebuggerUI::parsePHPVariables(const QString &varstring)
{
  m_variablesListView->parsePHPVariables(varstring);
}


/*void DebuggerUI::preWatchUpdate()
{
  m_variablesListView->preWatchUpdate();
}

void DebuggerUI::postWatchUpdate()
{
  m_variablesListView->postWatchUpdate();
}*/

#include "debuggerui.moc"
