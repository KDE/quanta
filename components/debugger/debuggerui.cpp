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
#include "debuggerbreakpointview.h"
#include "debuggervariable.h"

#include "quantacommon.h"
#include "quanta.h"
#include "resource.h"
#include "whtmlpart.h"

#include <kiconloader.h>
#include <kdockwidget.h>
#include <klocale.h>
#include <kdebug.h>
#include <qstring.h>
#include <khtmlview.h>
#include <kmditoolviewaccessor.h>

DebuggerUI::DebuggerUI(QObject *parent, const char *name)
    : QObject(parent, name), m_variablesListView(0)
{

  // Variable watch tree
  m_variablesListView = new VariablesListView(quantaApp->getMainDockWidget(), "debuggerVariables");
  m_variablesListView->setIcon(SmallIcon("math_brace"));
  m_variablesListView->setCaption(i18n("Variables"));
  m_variableListViewTVA = quantaApp->addToolWindow(m_variablesListView,  quantaApp->prevDockPosition(m_variablesListView, KDockWidget::DockLeft), quantaApp->getMainDockWidget());

  // Breakpointlist
  m_debuggerBreakpointView = new DebuggerBreakpointView(quantaApp->getMainDockWidget(), "debuggerBreakpoints");
  m_debuggerBreakpointView->setIcon(SmallIcon("debug_breakpoint"));
  m_debuggerBreakpointView->setCaption(i18n("Breakpoints"));
  m_debuggerBreakpointViewTVA = quantaApp->addToolWindow(m_debuggerBreakpointView, quantaApp->prevDockPosition(m_debuggerBreakpointView, KDockWidget::DockBottom), quantaApp->getMainDockWidget());

  // Debug HTML preview
  m_preview = new WHTMLPart(quantaApp, "debug_output");
  //m_preview->view()->resize(0, 0);
  m_preview->view()->setIcon(UserIcon("debug_run"));
  m_preview->view()->setCaption(i18n("Debug Output"));
  m_previewTVA = quantaApp->addToolWindow(m_preview->view(), quantaApp->prevDockPosition(m_preview->view(), KDockWidget::DockBottom), quantaApp->getMainDockWidget());

  // Show debugger toolbar
  quantaApp->toolBar("debugger_toolbar")->show();

  connect(m_variablesListView, SIGNAL(removeVariable(DebuggerVariable* )), parent, SLOT(slotRemoveVariable(DebuggerVariable* )));

  connect(m_debuggerBreakpointView, SIGNAL(removeBreakpoint(DebuggerBreakpoint* )), parent, SLOT(slotRemoveBreakpoint(DebuggerBreakpoint* )));
  showMenu();
}

DebuggerUI::~DebuggerUI()
{
  hideMenu();
  quantaApp->toolBar("debugger_toolbar")->hide();

  // Remove Variable tree
  quantaApp->deleteToolWindow(m_variableListViewTVA);
  m_variableListViewTVA = 0L;

  // Remove breakpointlist
  quantaApp->deleteToolWindow(m_debuggerBreakpointViewTVA);
  quantaApp->deleteToolWindow(m_previewTVA);
  m_debuggerBreakpointViewTVA = 0L;
  m_previewTVA = 0L;
}

void DebuggerUI::showMenu()
{
  QPopupMenu* debuggerMenu = (QPopupMenu*)(quantaApp->guiFactory())->container("debugger_menu", quantaApp);
  if(debuggerMenu)
  {
    //hiding and showing the menubar is ugly, but unfortunately
    //QMenuBar simply crashes if a new item is added while a submenu
    //is opened.
    KMenuBar *mb = quantaApp->menuBar();
    bool visible = mb->isVisible();
    if (visible)
      mb->hide();
    m_debuggerMenuID = quantaApp->menuBar()->insertItem(i18n("Deb&ug"), debuggerMenu, -1, 5);
    if (visible)
      mb->show();
  }
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

void DebuggerUI::showBreakpoint(const DebuggerBreakpoint &bp)
{
  m_debuggerBreakpointView->showBreakpoint(bp);
}
void DebuggerUI::deleteBreakpoint(const DebuggerBreakpoint &bp)
{
  m_debuggerBreakpointView->deleteBreakpoint(bp);
}

void DebuggerUI::sendRequest(const KURL &url)
{
  m_preview->openURL(url);
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
