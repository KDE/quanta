/***************************************************************************
                          phpdebuggerinterface.cpp
                             -------------------
    begin                : 2004-03-12
    copyright            : (C) 2004 Linus McCabe <linus@mccabe.nu>
    Based on work by Mathieu Kooiman
 ***************************************************************************/

/****************************************************************************
 *                                                                          *
 *   This program is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation; either version 2 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 ***************************************************************************/

#include <ktexteditor/document.h>
#include <ktexteditor/markinterfaceextension.h>
#include <ktexteditor/viewcursorinterface.h>
#include <kdebug.h>
#include <klocale.h>
#include <kparts/componentfactory.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <qextfileinfo.h>
#include <kinputdialog.h>
#include <qlineedit.h>

#include "quanta.h"
#include "quantadoc.h"
#include "document.h"
#include "resource.h"
#include "project.h"
#include "quantadebuggerinterface.h"
#include "debuggerclient.h"
#include "debuggerbreakpoint.h"
#include "debuggerbreakpointlist.h"
#include "debuggermanager.h"
#include "messageoutput.h"
#include "viewmanager.h"
#include "quantaview.h"
#include "debuggerui.h"

// dialogs
#include "debuggervariablesets.h"

DebuggerManager::DebuggerManager(QObject *myparent)
    : QObject(myparent)
{
  initActions();

  // Create objects
  m_breakpointList = new DebuggerBreakpointList();
  m_debuggerui = NULL;
  m_interface = new QuantaDebuggerInterface(this, "interface");
  m_client = NULL;
}

void DebuggerManager::slotNewProjectLoaded(const QString &projectname, const KURL &, const KURL &)
{
  if(m_client)
  {
    delete m_client;
    m_client = NULL;
  }
  enableAction("*", false);

  if(m_debuggerui)
  {
    delete m_debuggerui;
    m_debuggerui = NULL;
  }
  //kdDebug(24000) << "DebuggerManager::slotNewProjectLoaded " << projectname << ", " << Project::ref()->debuggerClient << endl;

  // Load new client
  if(!projectname.isEmpty())
  {

    KTrader::OfferList offers = KTrader::self()->query("Quanta/Debugger");
    KTrader::OfferList::ConstIterator iterDbg;
    for(iterDbg = offers.begin(); iterDbg != offers.end(); ++iterDbg)
    {
      KService::Ptr service = *iterDbg;
      if(Project::ref()->debuggerClient() == service->name())
      {
        int errCode = 0;
        m_client = KParts::ComponentFactory::createInstanceFromService<DebuggerClient::DebuggerClient>(service, this, 0, QStringList(), &errCode);

        //kdDebug(24000) << service->name() << " (" << m_client << ")" << endl;

        if(!m_client)
        {
          emit hideSplash();
          KMessageBox::error(NULL, i18n("<qt>Unable to load the debugger plugin, error code %1 was returned: <b>%2</b>.</qt>").arg(errCode).arg(KLibLoader::self()->lastErrorMessage()), i18n("Debugger Error"));
        }
        break;
      }
    }
  }

  // Tell client to load its settings
  if(m_client)
  {
    QDomNode nodeThisDbg;
    QDomNode projectNode = Project::ref()->dom()->firstChild().firstChild();
    QDomNode nodeDbg  = projectNode.namedItem("debuggers");
    if(nodeDbg.isNull())
    {
      nodeDbg = Project::ref()->dom()->createElement("debuggers");
      projectNode.appendChild(nodeDbg);
    }

    nodeThisDbg = nodeDbg.namedItem(m_client->getName());
    if(nodeThisDbg.isNull())
    {
      nodeThisDbg = Project::ref()->dom()->createElement(m_client->getName());
      nodeDbg.appendChild(nodeThisDbg);
    }

    m_client->readConfig(nodeThisDbg);

    m_debuggerui = new DebuggerUI(this, "debuggerui");
  }

  initClientActions();

  // Disable all debugactions that need a session (ie not breakpoints, etc)
  slotDebugStartSession();

}

void DebuggerManager::initActions()
{
  KActionCollection *ac = quantaApp->actionCollection();
  if(!ac)
    return;

  //Debugger, breakpoint
  new KAction(i18n("Toggle &Breakpoint"), SmallIcon("debug_breakpoint"), Qt::CTRL+Qt::SHIFT+Qt::Key_B,
              this, SLOT(toggleBreakpoint()), ac, "debug_breakpoints_toggle");
  new KAction(i18n("&Clear Breakpoints"), 0,
              this, SLOT(clearBreakpoints()), ac, "debug_breakpoints_clear");
  new KAction(i18n("Break When..."), SmallIcon("math_int"), 0,
              this, SLOT(slotConditionalBreakpoint()), ac, "debug_conditional_break");
  new KAction(i18n("Break When..."), SmallIcon("math_int"), 0,
              this, SLOT(slotConditionalBreakpoint()), ac, "debug_conditional_breakdialog");

  // Execution
  new KAction(i18n("&Run"), SmallIcon("debug_run"), 0,
              this, SLOT(slotDebugRun()), ac, "debug_run");
  new KAction(i18n("&Leap"), SmallIcon("debug_leap"), 0,
              this, SLOT(slotDebugLeap()), ac, "debug_leap");
  new KAction(i18n("&Step"), SmallIcon("debug_stepover"), 0,
              this, SLOT(slotDebugStepOver()), ac, "debug_stepover");
  new KAction(i18n("Step &Into"), SmallIcon("debug_stepinto"), 0,
              this, SLOT(slotDebugStepInto()), ac, "debug_stepinto");
  new KAction(i18n("S&kip"), SmallIcon("debug_skip"), 0,
              this, SLOT(slotDebugSkip()), ac, "debug_skip");
  new KAction(i18n("Step &Out"), SmallIcon("debug_stepout"), 0,
              this, SLOT(slotDebugStepOut()), ac, "debug_stepout");
  new KAction(i18n("&Pause"), SmallIcon("debug_pause"), 0,
              this, SLOT(slotDebugPause()), ac, "debug_pause");
  new KAction(i18n("Kill"), SmallIcon("debug_kill"), 0,
              this, SLOT(slotDebugKill()), ac, "debug_kill");
  new KAction(i18n("Start Session"), SmallIcon("debug_connect"), 0,
              this, SLOT(slotDebugStartSession()), ac, "debug_connect");
  new KAction(i18n("End Session"), SmallIcon("debug_disconnect"), 0,
              this, SLOT(slotDebugEndSession()), ac, "debug_disconnect");

  // Variables
  new KAction(i18n("Watch Variable"), SmallIcon("math_brace"), 0,
              this, SLOT(slotAddWatch()), ac, "debug_addwatch");
  new KAction(i18n("Watch Variable"), SmallIcon("math_brace"), 0,
              this, SLOT(slotAddWatch()), ac, "debug_addwatchdialog");
  new KAction(i18n("Set Value of Variable"), SmallIcon("edit"), 0,
              this, SLOT(slotVariableSet()), ac, "debug_variable_set");
  new KAction(i18n("Set Value of Variable"), SmallIcon("edit"), 0,
              this, SLOT(slotVariableSet()), ac, "debug_variable_setdialog");

}

void DebuggerManager::initClientActions()
{
  if(m_client)
  {
    // Get actioncollection and add appropriate actions depending on capabilities of the debugger
    if(!m_client->supports(DebuggerClientCapabilities::LineBreakpoints))
      enableAction("debug_breakpoints_toggle", false);
    if(!m_client->supports(DebuggerClientCapabilities::ClearAllBreakpoints))
      enableAction("debug_breakpoints_clear", false);

    enableAction("*", false);
  }
}

DebuggerManager::~DebuggerManager()
{
  delete m_breakpointList;
  m_breakpointList = 0L;
  delete m_client;
  m_client = 0L;
  delete m_debuggerui;
  m_debuggerui = 0L;
  delete m_interface;
  m_interface = 0L;
}

void DebuggerManager::enableAction(QString action, bool enable)
{
  if(action == "*")
  {
    // Enable/Disable all session related actions + connect/disconnect
    enableAction("debug_run", enable);
    enableAction("debug_leap", enable);
    enableAction("debug_pause", enable);
    enableAction("debug_kill", enable);
    enableAction("debug_stepover", enable);
    enableAction("debug_stepinto", enable);
    enableAction("debug_stepout", enable);
    enableAction("debug_skip", enable);

    enableAction("debug_connect", enable);
    enableAction("debug_disconnect", enable);
  }
  else
  {
    // The action may or may not exist, depending on capabilities of the debugger plugin
    KActionCollection *ac = quantaApp->actionCollection();
    if(ac && ac->action(action))
      ac->action(action)->setEnabled(enable);
  }
}

void DebuggerManager::slotRemoveVariable(DebuggerVariable* var)
{
  if(!m_client)
    return;

  m_client->removeWatch(var);

}

void DebuggerManager::slotRemoveBreakpoint(DebuggerBreakpoint* bp)
{
  if(!m_client)
    return;
  m_breakpointList->remove(bp);
  m_client->removeBreakpoint(bp);

}


void DebuggerManager::slotAddWatch()
{
  kdDebug(24000) << "DebuggerManager::slotAddWatch() " << endl;
  if(!m_client)
    return;


  QString watch = KInputDialog::getText(i18n("Add Watch"), i18n("Specify variable to watch:"), quantaApp->popupWord);
  quantaApp->popupWord = "";
  if(!watch.isEmpty())
  {
    m_client->addWatch(watch);
  }
}

void DebuggerManager::slotVariableSet()
{
  kdDebug(24000) << "DebuggerManager::slotVariableSet(" << quantaApp->popupWord << ") " << endl;
  if(!m_client)
    return;


  DebuggerVariableSetS dlg;
  dlg.lineVariable->setText(quantaApp->popupWord);
  quantaApp->popupWord = "";
  if(dlg.exec() == QDialog::Accepted)
  {
    DebuggerVariable var;
    var.setName(dlg.lineVariable->text());
    var.setValue(dlg.lineValue->text());
    m_client->variableSetValue(var);
  }
}

void DebuggerManager::slotConditionalBreakpoint()
{
  kdDebug(24000) << "DebuggerManager::slotConditionalBreakpoint() " << quantaApp->popupWord << endl;
  if(!m_client)
    return;

  QString condition = KInputDialog::getText(i18n("Add Conditional Breakpoint"), i18n("Specify expression to break at (when true):"), quantaApp->popupWord);
  quantaApp->popupWord = "";
  if(!condition.isEmpty())
  {
    DebuggerBreakpoint * bp = new DebuggerBreakpoint("", 0, condition);
    m_client->addBreakpoint(bp);
    m_breakpointList->add(bp);
  }
}

void DebuggerManager::slotDebugStartSession()
{
  if(!m_client)
    return;

  m_client->startSession();
}

void DebuggerManager::slotDebugEndSession()
{
  if(!m_client)
    return;

  m_client->endSession();
}

void DebuggerManager::slotDebugRun()
{
  if(!m_client)
    return;

  m_client->run();
}

void DebuggerManager::slotDebugLeap()
{
  if(!m_client)
    return;

  m_client->leap();

}
void DebuggerManager::slotDebugSkip()
{
  if(!m_client)
    return;

  m_client->skip();

}
void DebuggerManager::slotDebugStepOver()
{
  if(!m_client)
    return;

  m_client->stepOver();

}
void DebuggerManager::slotDebugStepInto()
{
  if(!m_client)
    return;

  m_client->stepInto();

}
void DebuggerManager::slotDebugPause()
{
  if(!m_client)
    return;

  m_client->pause();

}
void DebuggerManager::slotDebugKill()
{
  if(!m_client)
    return;

  m_client->kill();

}
void DebuggerManager::slotDebugStepOut()
{
  if(!m_client)
    return;

  m_client->stepOut();

}


// A new file was opened, tell the debugger so it can tell us about breakpoints etc
void DebuggerManager::fileOpened(QString file)
{

  // Set breakpoint markers if we have a bp in the file
  m_breakpointList->rewind();
  DebuggerBreakpoint* bp;
  while((bp = m_breakpointList->next()))
  {
    if(bp->filePath() == file)
    {
      setMark(bp->filePath(), bp->line(), true, KTextEditor::MarkInterface::markType02);
    }
  }

  // Also, if we have a debug-session, let the debugger know...
  if(m_client)
    m_client->fileOpened(file);
}

// Check with editors if breakpoints changed and send all breakpoint (again) to client
void DebuggerManager::refreshBreakpoints()
{
  // Update bp-list from editors
  // ...TODO

  // Resend bps
  m_breakpointList->rewind();
  DebuggerBreakpoint* bp;
  while((bp = m_breakpointList->next()))
  {
    m_client->addBreakpoint(bp);
  }

}


// The debug server told us we have a breakpoint, mark it in the file
void DebuggerManager::haveBreakpoint (QString file, int line)
{
  setMark(file, line, true, KTextEditor::MarkInterface::markType02);
}

// New current line
bool DebuggerManager::setActiveLine (QString file, int line )
{
  //Get local filename
  QString filename = file;

  // Find new position in editor
  if(ViewManager::ref()->isOpened(filename) || QExtFileInfo::exists(filename))
    quantaApp->gotoFileAndLine(filename, line, 0);
  else
  {
    kdDebug(24000) << k_lineinfo << "File does not exist: " << filename << endl;
  }

  // Remove old active line mark
  setMark(m_currentFile, m_currentLine, false, KTextEditor::MarkInterface::markType05);


  // Update vars with active line
  m_currentFile = filename;
  m_currentLine = line;

  // Add new active line mark
  setMark(filename, line, true, KTextEditor::MarkInterface::markType05);
  return true;
}

// Set/clear a mark in a document
void DebuggerManager::setMark(QString filename, long line, bool set, int mark)
{
  if((!filename.isEmpty()) && ViewManager::ref()->isOpened(filename))
  {
    ::Document* qdoc = ViewManager::ref()->isOpened(filename)->document();
    if(qdoc)
    {
      KTextEditor::Document* doc = qdoc->doc();
      if(doc)
      {
        KTextEditor::MarkInterface *markIf = dynamic_cast<KTextEditor::MarkInterface*>(doc);
        if(markIf)
        {
          if(set)
            markIf->addMark(line, mark);
          else
            markIf->removeMark(line, mark);
        }
      }
    }
  }
}

// Show a status message and optionally put it on the log
bool DebuggerManager::showStatus(QString message, bool log)
{
  quantaApp->slotStatusMsg(m_client->getName() + ": " + message);

  if(log)
  {
    if(!message.endsWith("\n"))
      message.append("\n");
    quantaApp->messageOutput()->showMessage(m_client->getName() + ": " + message);
  }
  return true;
}


void DebuggerManager::toggleBreakpoint ()
{
  Document *w = ViewManager::ref()->activeDocument();
  if (w)
  {
    uint line, col;
    w->viewCursorIf->cursorPositionReal(&line, &col);

    DebuggerBreakpoint* br = new DebuggerBreakpoint(w->url().prettyURL(0, KURL::StripFileProtocol), line);

    if(!m_breakpointList->exists(br))
    {
      m_breakpointList->add(br);
      setMark(w->url().prettyURL(0, KURL::StripFileProtocol), br->line(), true, KTextEditor::MarkInterface::markType02);
      if(m_client && m_client->isActive())
      {
        m_client->addBreakpoint(br);
      }
    }
    else
    {
      m_breakpointList->remove(br);
      setMark(w->url().prettyURL(0, KURL::StripFileProtocol), br->line(), false, KTextEditor::MarkInterface::markType02);

      if(m_client && m_client->isActive())
      {
        m_client->removeBreakpoint(br);
      }
    }
  }
}
void DebuggerManager::clearBreakpoints ()
{
  DebuggerBreakpoint *bp;

  m_breakpointList->rewind();
  while((bp = m_breakpointList->next()))
    setMark(bp->filePath(), bp->line(), false, KTextEditor::MarkInterface::markType02);

  m_breakpointList->clear();
}

DebuggerBreakpoint *DebuggerManager::newDebuggerBreakpoint()
{
  return new DebuggerBreakpoint();
}

#include "debuggermanager.moc"
