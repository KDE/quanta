/***************************************************************************
                           quantadebuggergubed.cpp
                             -------------------
    begin                : 2004-03-12
    copyright            : (C) 2004 Linus McCabe <linus@mccabe.nu>
 ***************************************************************************/

/****************************************************************************
 *                                                                          *
 *   This program is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation; either version 2 of the License, or      *
 *   (at your option) any later version.                                    *                     
 *                                                                          *
 ***************************************************************************/

#include <kdebug.h>
#include <klocale.h>
#include <kgenericfactory.h>

#include "debuggerclient.h"
#include "quantadebuggergubed.h"
#include "debuggerinterface.h"
#include "debuggerbreakpoint.h"


K_EXPORT_COMPONENT_FACTORY( libquantadebuggergubed,
                            KGenericFactory<QuantaDebuggerGubed>("quantadebuggergubed"))

QuantaDebuggerGubed::QuantaDebuggerGubed (QObject *parent, const char* name, const QStringList&)
   : DebuggerClient (parent, name)
{
  // Create a socket object and set up its signals
  m_socket = new QSocket();
  connect(m_socket, SIGNAL(error(int)), this, SLOT(slotError(int)));
  connect(m_socket, SIGNAL(connected()), this, SLOT(slotConnected()));
  connect(m_socket, SIGNAL(connectionClosed()), this, SLOT(slotConnectionClosed()));
  connect(m_socket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));

}

// Try to make a connection to the gubed server
bool QuantaDebuggerGubed::startSession() 
{
  QString host;
  Q_UINT16 port;
  
  // Get host from project settings
  host = debuggerInterface()->debugServerHost();
  if(host.isEmpty())
    host = "localhost"; // Default to localhost
    
  // Get port from project settings
  port = debuggerInterface()->debugServerPort().toUInt();
  port = 0;
  if(port <= 0)
    port = 8026;  // Deault if not specified
    
  m_socket->connectToHost(host, port);
  
  return true;
}


bool QuantaDebuggerGubed::endSession() 
{
  // Close the socket
  m_socket->close();
  
  // Fake a connection closed signal
  slotConnectionClosed();
  
  return true;
}

// Return capabilities of gubed
const uint QuantaDebuggerGubed::getCapabilities()
{
   return CapBreakpoints
        + CapWatches
        + CapStepInto
        + CapSkip
        + CapPause
        + CapKill
        + CapRun
        + CapRunDisplay
        + CapConnect
        ;
}

// Socket errors
void QuantaDebuggerGubed::slotError(int error)
{
   kdDebug(24000) << "QuantaDebuggerGubed::slotError " << error << endl;
}

// Connection established
void QuantaDebuggerGubed::slotConnected()
{
  debuggerInterface()->enableAction("debug_connect", false);
  debuggerInterface()->enableAction("debug_disconnect", true);
  debuggerInterface()->enableAction("debug_run", true);
  debuggerInterface()->enableAction("debug_leap", true);
  debuggerInterface()->enableAction("debug_pause", true);
  
  m_active = true;
}

// Connectio closed
void QuantaDebuggerGubed::slotConnectionClosed()
{
  // Disable all session related actions and enable connection action
  debuggerInterface()->enableAction("*", false);
  debuggerInterface()->enableAction("debug_connect", true);
  debuggerInterface()->enableAction("debug_run", false);
  debuggerInterface()->enableAction("debug_leap", false);
  debuggerInterface()->enableAction("debug_pause", false);
  
  m_active = false;
}

// Data from socket
void QuantaDebuggerGubed::slotReadyRead()
{
  // Data from gubed comes in line terminated pairs
  // Loop as long as we can read a full line
  while(m_socket->canReadLine())
  {
     // If we can read a line and we didnt get the 'command' (first) line yet, use this line as command 
     if(m_command.isEmpty())
       m_command = m_socket->readLine().stripWhiteSpace();

    // If we can read a second line, its the data, if its not available yet, return
    if(!m_socket->canReadLine())
      return;

    // We can read a second line, its the data 
    QString data = m_socket->readLine().stripWhiteSpace();
    kdDebug(24000) << "QuantaDebuggerGubed::slotReadyRead " << m_command << ":" << data << endl;
  
    // See what command we got and act accordingly.. 
    // Just some status info, display on status line
    if(m_command == "status")
      debuggerInterface()->showStatus(data, false);

    // New current line
    else if(m_command == "setactiveline")
      debuggerInterface()->setActiveLine(data.left(data.find(':')), data.mid(data.find(':') + 1).toLong());

    // A debugging session is running 
    else if(m_command == "debuggingon")
    {
      debuggingState(true);
    }
    // No session is running
    else if(m_command == "debuggingoff")
    {
      debuggingState(false);
    }
    // We stumbled upon an error
    else if(m_command == "error")
    {
      QString filename, line, error;
      filename = data.left(data.find(':'));
      line = data.mid(data.find(':') + 1);
      error = line.mid(line.find(':') + 1);
      line = line.left(line.find(':'));
      
      // Put the line number first so double clicking will jump to the corrrect line
      debuggerInterface()->showStatus(i18n("Error occurred: ") + line + ", " + error + " (" + filename + ")" , true);
    }
    // We came across  a hard coded breakpoint
    else if(m_command == "forcedbreak")
    {
      debuggerInterface()->showStatus(i18n("Forced break: ") + data, true);
    }
    // There is a breakpoint set in this file/line
    else if(m_command == "breakpoint")
    {
      debuggerInterface()->haveBreakpoint(data.left(data.find(':')), data.mid(data.find(':') + 1).toLong());
    }
    // We're about to debug a file..
    else if(m_command == "initialize")
    {
      debuggerInterface()->setActiveLine(data.left(data.find(':')), 0);
      sendCommand("havesource", "");
      debuggingState(true);
    }
    else
      // Unimplemented command - log to debug output
      kdDebug(24000) << "QuantaDebuggerGubed::slotReadyRead Unknown: " << m_command << ":" << data << endl;

    // Clear command so the loop works
    m_command = "";
  }

}

// Turn on/off actions related to a debugging session
void QuantaDebuggerGubed::debuggingState(bool enable)
{
  debuggerInterface()->enableAction("debug_kill", enable);
  debuggerInterface()->enableAction("debug_stepover", enable);
  debuggerInterface()->enableAction("debug_stepinto", enable);
  debuggerInterface()->enableAction("debug_stepout", enable);
  debuggerInterface()->enableAction("debug_skip", enable);
}

// Send a command to gubed
bool QuantaDebuggerGubed::sendCommand(QString command, QString data)
{
  if(m_socket->state() != QSocket::Connected)
    return false;

  // Needs line terminatino
  command += "\n";
  data += "\n";
  
  // Write data to socket
  m_socket->writeBlock(command, command.length());
  m_socket->writeBlock(data, data.length());
  
  return true;
}

// Return name of debugger
QString QuantaDebuggerGubed::getName() {
   return i18n("Gubed");      // i18n ??
}

// Run boy, run (and show whats happening)
bool QuantaDebuggerGubed::run()
{
   sendCommand("rundisplay", "");
   return true;
}

// Go as fast as possible and dont update current line or watches
bool QuantaDebuggerGubed::leap()
{
   sendCommand("runnodisplay", "");
   return true;
}

// Step into function
bool QuantaDebuggerGubed::stepInto()
{
   sendCommand("next", "");
   return true;
}

// Skip next function
bool QuantaDebuggerGubed::skip()
{
   sendCommand("skip", "");
   return true;
}

// Kill the running script
bool QuantaDebuggerGubed::kill()
{
   sendCommand("kill", "");
   return true;
}

// Pause execution
bool QuantaDebuggerGubed::pause()
{
   sendCommand("pause", "");
   return true;
}


// Add a breakpoint
bool QuantaDebuggerGubed::addBreakpoint (DebuggerBreakpoint* breakpoint)
{
  sendCommand("breakpoint", breakpoint->filePath() + ":" + QString::number(breakpoint->line()));
  return true;
}
 
// Clear a breakpoint 
bool QuantaDebuggerGubed::removeBreakpoint(DebuggerBreakpoint* breakpoint)
{
  sendCommand("clearpoint", breakpoint->filePath() + ":" + QString::number(breakpoint->line()));
  return true;
}

// A file was opened...
void QuantaDebuggerGubed::fileOpened(QString file)
{
  sendCommand("reinitialize", "");
}

#include "quantadebuggergubed.moc"
