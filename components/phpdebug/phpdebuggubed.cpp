/***************************************************************************
                          phpdebuggubed.cpp
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

#include "quanta.h"
#include "resource.h"
#include "project.h"

#include "phpdebuggubed.h"
#include "phpdebuggerinterface.h"

PHPDebugGubed::PHPDebugGubed (PHPDebuggerInterface *interface)
   : PHPDebugSocket (interface)
{
   // Create a socket object and set up its signals
   m_socket = new QSocket();
   connect(m_socket, SIGNAL(error(int)), this, SLOT(slotError(int)));
   connect(m_socket, SIGNAL(connected()), this, SLOT(slotConnected()));
   connect(m_socket, SIGNAL(connectionClosed()), this, SLOT(slotConnectionClosed()));
   connect(m_socket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));

   // Try to connect
   tryConnect();
}

// Try to make a connection to the gubed server
bool PHPDebugGubed::tryConnect() 
{
  QString host;
  Q_UINT16 port;
  
  // Get host from project settings
  host = Project::ref()->debugServerHost;
  if(host.isEmpty())
    host = "localhost"; // Default to localhost
    
  // Get port from project settings
  port = Project::ref()->debugServerPort.toUInt();
  if(port <= 0)
    port = 8026;  // Deault if not specified
    
  kdDebug(24000) << "PHPDebugGubed::tryConnect " << host << ":" << port << endl;

  m_interface->enableAction("debug_connect", true);
  m_interface->enableAction("debug_disconnect", false);
  m_socket->connectToHost(host, port);
  
  return true;
}


bool PHPDebugGubed::disConnect() 
{
  // Close the socket
  m_socket->close();
  
  // Fake a connection closed signal
  slotConnectionClosed();
  return true;
}

// Return capabilities of gubed
const uint PHPDebugGubed::getCapabilities()
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
void PHPDebugGubed::slotError(int error)
{
   kdDebug(24000) << "PHPDebugGubed::slotError " << error << endl;
}

// Connection established
void PHPDebugGubed::slotConnected()
{
  kdDebug(24000) << "PHPDebugGubed::slotConnected" << endl;
  m_interface->enableAction("debug_connect", false);
  m_interface->enableAction("debug_disconnect", true);
  m_interface->enableAction("debug_run", true);
  m_interface->enableAction("debug_leap", true);
  m_interface->enableAction("debug_pause", true);
}

// Connectio closed
void PHPDebugGubed::slotConnectionClosed()
{
  kdDebug(24000) << "PHPDebugGubed::slotConnectionClosed" << endl;
   
  // Disable all session related actions and enable connection action
  m_interface->enableAction("*", false);
  m_interface->enableAction("debug_connect", true);
  m_interface->enableAction("debug_run", false);
  m_interface->enableAction("debug_leap", false);
  m_interface->enableAction("debug_pause", false);
}

// Data from socket
void PHPDebugGubed::slotReadyRead()
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
    kdDebug(24000) << "PHPDebugGubed::slotReadyRead " << m_command << ":" << data << endl;
  
    // See what command we got and act accordingly.. 
    // Just some status info, display on status line
    if(m_command == "status")
      m_interface->showStatus(data, false);

    // New current line
    else if(m_command == "setactiveline")
      m_interface->setActiveLine(KURL(data.left(data.find(':'))), data.mid(data.find(':') + 1).toLong());

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
      m_interface->showStatus(i18n("Error occurred: ") + line + ", " + error + " (" + filename + ")" , true);
    }
    // We came across  a hard coded breakpoint
    else if(m_command == "forcedbreak")
    {
      m_interface->showStatus(i18n("Forced break: ") + data, true);
    }
    // There is a breakpoint set in this file/line
    else if(m_command == "breakpoint")
    {
      m_interface->haveBreakpoint(data.left(data.find(':')), data.mid(data.find(':') + 1).toLong());
    }
    // We're about to debug a file..
    else if(m_command == "initialize")
    {
      m_interface->setActiveLine(KURL(data.left(data.find(':'))), 0);
      sendCommand("havesource", "");
      debuggingState(true);
    }
    else
      // Unimplemented command - log to debug output
      kdDebug(24000) << "PHPDebugGubed::slotReadyRead Unknown: " << m_command << ":" << data << endl;

    // Clear command so the loop works
    m_command = "";
  }

}

// Turn on/off actions related to a debugging session
void PHPDebugGubed::debuggingState(bool enable)
{
        m_interface->enableAction("debug_kill", enable);
        m_interface->enableAction("debug_stepover", enable);
        m_interface->enableAction("debug_stepinto", enable);
        m_interface->enableAction("debug_stepout", enable);
        m_interface->enableAction("debug_skip", enable);
}

// Send a command to gubed
bool PHPDebugGubed::sendCommand(QString command, QString data)
{
  // Needs line terminatino
  command += "\n";
  data += "\n";
  
  // Write data to socket
  m_socket->writeBlock(command, command.length());
  m_socket->writeBlock(data, data.length());
  
  return true;
}

// Return name of debugger
QString PHPDebugGubed::getName() {
   return i18n("Gubed");      // i18n ??
}

// Run boy, run (and show whats happening)
bool PHPDebugGubed::run()
{
   sendCommand("rundisplay", "");

   return true;
}

// Go as fast as possible and dont update current line or watches
bool PHPDebugGubed::leap()
{
   sendCommand("runnodisplay", "");
   return true;
}

// Step over (but execute) function call
bool PHPDebugGubed::stepOver()
{
   return false;
}

// Step into function
bool PHPDebugGubed::stepInto()
{
   sendCommand("next", "");
   return true;
}

// Step out of function
bool PHPDebugGubed::stepOut()
{
   return false;
}

// Skip next function
bool PHPDebugGubed::skip()
{
   sendCommand("skip", "");
   return true;
}

// Kill the running script
bool PHPDebugGubed::kill()
{
   sendCommand("kill", "");
   return true;
}

// Pause execution
bool PHPDebugGubed::pause()
{
   sendCommand("pause", "");
   return true;
}

// Add a breakpoint
bool PHPDebugGubed::setBreakpoint ( QString file, int line)
{
   sendCommand("breakpoint", file + ":" + QString::number(line));
   return true;
}

// Clear a breakpoint
bool PHPDebugGubed::removeBreakpoint ( QString file, int line )
{
   sendCommand("clearpoint", file + ":" + QString::number(line));
   return true;
}

// A file was opened...
void PHPDebugGubed::fileOpened(KURL file)
{
  sendCommand("reinitialize", "");
}

#include "phpdebuggubed.moc"
