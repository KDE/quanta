#include <ktexteditor/document.h>
#include <kdebug.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kmessagebox.h>

#include "quanta.h"
#include "quantadoc.h"
#include "document.h"
#include "resource.h"
#include "project.h"
#include "phpdebuggerinterface.h"
#include "phpdebugsocket.h"
#include "phpdebuggubed.h"


using namespace KTextEditor;

PHPDebuggerInterface::PHPDebuggerInterface ()
{

  // This will change when put in loadable module
  m_debugsocket = new PHPDebugGubed(this);
  uint cap = m_debugsocket->getCapabilities();
  
  // Get actioncollection and add appropriate actions depending on capabilities of the debugger
  KActionCollection *ac = quantaApp->actionCollection();
  ac->action("debug_breakpoints_toggle")->setEnabled(cap & CapBreakpoints);
  ac->action("debug_breakpoints_clear")->setEnabled(cap & CapClearAllBreakpoints);
  
  if(cap & CapRunDisplay)
    new KAction(i18n("&Run"), SmallIcon("debug_run"), 0,
          this, SLOT(slotDebugRun()), ac, "debug_run");
  if(cap & CapRun)
    new KAction(i18n("&Leap"), SmallIcon("debug_leap"), 0,
          this, SLOT(slotDebugLeap()), ac, "debug_leap");
  if(cap & CapStepOver)
    new KAction(i18n("&Step"), SmallIcon("debug_stepover"), 0,
          this, SLOT(slotDebugStepOver()), ac, "debug_stepover");
  if(cap & CapStepInto)
    new KAction(i18n("Step &into"), SmallIcon("debug_stepinto"), 0,
          this, SLOT(slotDebugStepInto()), ac, "debug_stepinto");
  if(cap & CapSkip)
    new KAction(i18n("S&kip"), SmallIcon("debug_skip"), 0,
          this, SLOT(slotDebugSkip()), ac, "debug_skip");
  if(cap & CapStepOut)
    new KAction(i18n("Step &Out"), SmallIcon("debug_stepout"), 0,
          this, SLOT(slotDebugStepOut()), ac, "debug_stepout");
  if(cap & CapPause)
    new KAction(i18n("&Pause"), SmallIcon("debug_pause"), 0,
          this, SLOT(slotDebugPause()), ac, "debug_pause");
  if(cap & CapKill)
    new KAction(i18n("Kill"), SmallIcon("debug_kill"), 0,
          this, SLOT(slotDebugKill()), ac, "debug_kill");
  if(cap & CapConnect)
  {
    new KAction(i18n("Connect"), SmallIcon("debug_connect"), 0,
          this, SLOT(slotDebugConnect()), ac, "debug_connect");
    new KAction(i18n("Disconnect"), SmallIcon("debug_disconnect"), 0,
          this, SLOT(slotDebugDisconnect()), ac, "debug_disconnect");
  }
  
  // Disable all debugactions that need a session (ie not breakpoints, etc)
  enableAction("*", false);
}

void PHPDebuggerInterface::enableAction(QString action, bool enable)
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

void PHPDebuggerInterface::slotDebugConnect()
{
   kdDebug(24000) << "PHPDebuggerInterface::slotDebugConnect" << endl;
   if(!m_debugsocket->tryConnect())
      KMessageBox::error(NULL, i18n("This debugger (%1) does not support the \"%2\" instruction.").arg(m_debugsocket->getName()).arg(i18n("Connect")), i18n("Unsupported debugger function"));
}

void PHPDebuggerInterface::slotDebugDisconnect()
{
   kdDebug(24000) << "PHPDebuggerInterface::slotDebugDisconnect" << endl;
   if(!m_debugsocket->disConnect())
      KMessageBox::error(NULL, i18n("This debugger (%1) does not support the \"%2\" instruction.").arg(m_debugsocket->getName()).arg(i18n("Disconnect")), i18n("Unsupported debugger function"));
}

void PHPDebuggerInterface::slotDebugRun()
{
   kdDebug(24000) << "PHPDebuggerInterface::slotDebugRun" << endl;
   if(!m_debugsocket->run())
      KMessageBox::error(NULL, i18n("This debugger (%1) does not support the \"%2\" instruction.").arg(m_debugsocket->getName()).arg(i18n("Run")), i18n("Unsupported debugger function"));
}

void PHPDebuggerInterface::slotDebugLeap()
{
   kdDebug(24000) << "PHPDebuggerInterface::slotDebugLeap" << endl;
   if(!m_debugsocket->leap())
      KMessageBox::error(NULL, i18n("This debugger (%1) does not support the \"%2\" instruction.").arg(m_debugsocket->getName()).arg(i18n("Leap")), i18n("Unsupported debugger function"));

}
void PHPDebuggerInterface::slotDebugSkip()
{
   kdDebug(24000) << "PHPDebuggerInterface::slotDebugSkip" << endl;
   if(!m_debugsocket->skip())
      KMessageBox::error(NULL, i18n("This debugger (%1) does not support the \"%2\" instruction.").arg(m_debugsocket->getName()).arg(i18n("Skip")), i18n("Unsupported debugger function"));

}
void PHPDebuggerInterface::slotDebugStepOver()
{
   kdDebug(24000) << "PHPDebuggerInterface::slotDebugStep" << endl;
   if(!m_debugsocket->stepOver())
      KMessageBox::error(NULL, i18n("This debugger (%1) does not support the \"%2\" instruction.").arg(m_debugsocket->getName()).arg(i18n("Step")), i18n("Unsupported debugger function"));

}
void PHPDebuggerInterface::slotDebugStepInto()
{
   kdDebug(24000) << "PHPDebuggerInterface::slotDebugStepInto" << endl;
   if(!m_debugsocket->stepInto())
      KMessageBox::error(NULL, i18n("This debugger (%1) does not support the \"%2\" instruction.").arg(m_debugsocket->getName()).arg(i18n("Step Into")), i18n("Unsupported debugger function"));

}
void PHPDebuggerInterface::slotDebugPause()
{
   kdDebug(24000) << "PHPDebuggerInterface::slotDebugPause" << endl;
   if(!m_debugsocket->pause())
      KMessageBox::error(NULL, i18n("This debugger (%1) does not support the \"%2\" instruction.").arg(m_debugsocket->getName()).arg(i18n("Pause")), i18n("Unsupported debugger function"));

}
void PHPDebuggerInterface::slotDebugKill()
{
   kdDebug(24000) << "PHPDebuggerInterface::slotDebugKill" << endl;
   if(!m_debugsocket->kill())
      KMessageBox::error(NULL, i18n("This debugger (%1) does not support the \"%2\" instruction.").arg(m_debugsocket->getName()).arg(i18n("Kill")), i18n("Unsupported debugger function"));

}
void PHPDebuggerInterface::slotDebugStepOut()
{
   kdDebug(24000) << "PHPDebuggerInterface::slotDebugStepOut" << endl;
   if(!m_debugsocket->stepOut())
      KMessageBox::error(NULL, i18n("This debugger (%1) does not support the \"%2\" instruction.").arg(m_debugsocket->getName()).arg(i18n("Step Out")), i18n("Unsupported debugger function"));

}

// The debug server told us we have a breakpoint, mark it in the file
void PHPDebuggerInterface::haveBreakpoint (QString file, int line)
{
  setMark(mapServerPathToLocal(file), line, true, KTextEditor::MarkInterface::markType02);
}

// Users added a breakpoint
bool PHPDebuggerInterface::setBreakpoint ( KURL file, int line)
{
   if(!m_debugsocket->setBreakpoint(mapLocalPathToServer(file.prettyURL(0, KURL::StripFileProtocol)), line))
   {
      KMessageBox::error(NULL, i18n("Unable to set breakpoint"), i18n("An error occured"));
      return false;
   }
   return true;
}

// User removed a breakpoint
bool PHPDebuggerInterface::removeBreakpoint ( KURL file, int line )
{
   if(!m_debugsocket->removeBreakpoint(mapLocalPathToServer(file.prettyURL(0, KURL::StripFileProtocol)), line))
   {
      KMessageBox::error(NULL, i18n("Unable to remove breakpoint"), i18n("An error occured"));
      return false;
   }
   return true;
}

// Map a server filepath to a local one using project settings
QString PHPDebuggerInterface::mapServerPathToLocal(QString serverpath)
{
   // Translate filename from server to local
   if(serverpath.startsWith(Project::ref()->debugServerBasedir, false))
      serverpath.remove(0, Project::ref()->debugServerBasedir.length());

   return Project::ref()->debugLocalBasedir + serverpath;
}

// Map a local filepath to a server one using project settings
QString PHPDebuggerInterface::mapLocalPathToServer(QString localpath)
{
   // Translate filename from local to server
   if(localpath.startsWith(Project::ref()->debugLocalBasedir, false))
      localpath.remove(0, Project::ref()->debugLocalBasedir.length());

   return Project::ref()->debugServerBasedir + localpath;
}

// New current line
bool PHPDebuggerInterface::setActiveLine ( KURL file, int line )
{
  //Get local filename
  QString filename = mapServerPathToLocal(file.prettyURL(0, KURL::StripFileProtocol));

   // Find new position in editor
   quantaApp->gotoFileAndLine(filename, line, 0);

   // Remove old active line mark
   setMark(m_currentFile, m_currentLine, false, KTextEditor::MarkInterface::markType05);

   // Update vars with active line
   m_currentFile = filename;
   m_currentLine = line;

   // Add new active line mark
   setMark(filename, line, true, KTextEditor::MarkInterface::markType05);
  return true;
}

// Set a mark in a document
void PHPDebuggerInterface::setMark(QString filename, long line, bool set, int mark)
{
   if(!filename.isEmpty())
   {
      ::Document* qdoc = quantaApp->doc()->isOpened(filename);
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
bool PHPDebuggerInterface::showStatus(QString message, bool log)
{
   quantaApp->slotStatusMsg(m_debugsocket->getName() + ": " + message);
   
   if(log)
   {
     if(!message.endsWith("\n"))
       message.append("\n");
     quantaApp->messageOutput()->showMessage(m_debugsocket->getName() + ": " + message);
   }
   return true;
}

// A new file was opened, tell the debugger so it can tell us about breakpoints etc
void PHPDebuggerInterface::fileOpened(KURL file)
{
  m_debugsocket->fileOpened(mapLocalPathToServer(file.prettyURL(0, KURL::StripFileProtocol)));
}

#include "phpdebuggerinterface.moc"
