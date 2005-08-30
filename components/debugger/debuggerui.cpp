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

#include <kiconloader.h>
#include <kdockwidget.h>
#include <klocale.h>
#include <kdebug.h>
#include <qstring.h>
#include <khtmlview.h>
#include <kmditoolviewaccessor.h>
#include <kstatusbar.h>

#include "variableslistview.h"
#include "debuggerbreakpointview.h"
#include "backtracelistview.h"
#include "debuggervariable.h"

#include "quantacommon.h"
#include "quanta.h"
#include "resource.h"
#include "whtmlpart.h"

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

  // Backtrace
  m_backtraceListview = new BacktraceListview(quantaApp->getMainDockWidget(), "debuggerBacktrace");
  m_backtraceListview->setIcon(SmallIcon("player_playlist"));
  m_backtraceListview->setCaption(i18n("Backtrace"));
  m_backtraceListviewTVA = quantaApp->addToolWindow(m_backtraceListview, quantaApp->prevDockPosition(m_backtraceListview, KDockWidget::DockBottom), quantaApp->getMainDockWidget());

  // Debug HTML preview
  m_preview = new WHTMLPart(quantaApp, "debug_output", true);
  //m_preview->view()->resize(0, 0);
  m_preview->view()->setIcon(UserIcon("debug_run"));
  m_preview->view()->setCaption(i18n("Debug Output"));
  m_previewTVA = quantaApp->addToolWindow(m_preview->view(), quantaApp->prevDockPosition(m_preview->view(), KDockWidget::DockBottom), quantaApp->getMainDockWidget());
  connect(m_preview, SIGNAL(openFile(const KURL&, const QString&, bool)), quantaApp, SLOT(slotFileOpen(const KURL&, const QString&, bool)));
  
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
  m_debuggerBreakpointViewTVA = 0L;

  // Remove backtrace
  quantaApp->deleteToolWindow(m_backtraceListviewTVA);
  m_backtraceListviewTVA = 0L;

  // Remove output
  quantaApp->deleteToolWindow(m_previewTVA);
  m_previewTVA = 0L;
}

void DebuggerUI::showMenu()
{
  QPopupMenu* debuggerMenu = (QPopupMenu*)(quantaApp->guiFactory())->container("debugger_menu", quantaApp);
  if(debuggerMenu)
  {
    KMenuBar *mb = quantaApp->menuBar();
    mb->activateItemAt(-1); //needed as insertItem might crash if a menu is activated
    m_debuggerMenuID = mb->insertItem(i18n("Deb&ug"), debuggerMenu, -1, 5);
  }
  else
    m_debuggerMenuID  = 0;

  // Status indicator
  quantaApp->statusBar()->insertFixedItem(i18n("Debugger Inactive"), IDS_STATUS_DEBUGGER);

}

void DebuggerUI::hideMenu()
{
  if(m_debuggerMenuID != 0)
  {
    KMenuBar *mb = quantaApp->menuBar();
    mb->activateItemAt(-1); //needed as removeItem might crash if a menu is activated
    mb->removeItem(m_debuggerMenuID);
  }
  m_debuggerMenuID = 0;

  // Status indicator
  quantaApp->statusBar()->removeItem(IDS_STATUS_DEBUGGER);

}

void DebuggerUI::addVariable(DebuggerVariable* var)
{
  m_variablesListView->addVariable(var);
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

void DebuggerUI::slotStatus( DebuggerStatus status )
{
  switch(status)
  {
    case NoSession:
      quantaApp->statusBar()->changeItem(i18n("No session"), IDS_STATUS_DEBUGGER);
      break;

    case AwaitingConnection:
      quantaApp->statusBar()->changeItem(i18n("Waiting"), IDS_STATUS_DEBUGGER);
      break;

    case Connected:
      quantaApp->statusBar()->changeItem(i18n("Connected"), IDS_STATUS_DEBUGGER);
      break;

    case Paused:
      quantaApp->statusBar()->changeItem(i18n("Paused"), IDS_STATUS_DEBUGGER);
      break;

    case Running:
      quantaApp->statusBar()->changeItem(i18n("Running"), IDS_STATUS_DEBUGGER);
      break;

    case Tracing:
      quantaApp->statusBar()->changeItem(i18n("Tracing"), IDS_STATUS_DEBUGGER);
      break;

    case HaltedOnError:
      quantaApp->statusBar()->changeItem(i18n("On error"), IDS_STATUS_DEBUGGER);
      break;

    case HaltedOnBreakpoint:
      quantaApp->statusBar()->changeItem(i18n("On breakpoint"), IDS_STATUS_DEBUGGER);
      break;

    default:
      quantaApp->statusBar()->changeItem("", IDS_STATUS_DEBUGGER);
  }
}

void DebuggerUI::backtraceClear( )
{
  if(m_backtraceListview)
    m_backtraceListview->clear();
}

void DebuggerUI::backtraceShow( long level, BacktraceType type, const QString & filename, long line, const QString & func )
{
  if(m_backtraceListview)
    m_backtraceListview->backtraceShow(level, type, filename, line, func);
}


#include "debuggerui.moc"
