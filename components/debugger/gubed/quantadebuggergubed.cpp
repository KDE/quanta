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
#include <qlineedit.h>
#include <kdeversion.h>

#include "debuggerclient.h"
#include "quantadebuggergubed.h"
#include "debuggerinterface.h"
#include "debuggerbreakpoint.h"
#include "gubedsettings.h" 
#include "debuggervariable.h"
#include "variableslistview.h"

K_EXPORT_COMPONENT_FACTORY( quantadebuggergubed,
                            KGenericFactory<QuantaDebuggerGubed>("quantadebuggergubed"))

QuantaDebuggerGubed::QuantaDebuggerGubed (QObject *parent, const char* name, const QStringList&)
   : DebuggerClient (parent, name)
{
  // Create a socket object and set up its signals
  m_socket = new QSocket(this);
  connect(m_socket, SIGNAL(error(int)), this, SLOT(slotError(int)));
  connect(m_socket, SIGNAL(connected()), this, SLOT(slotConnected()));
  connect(m_socket, SIGNAL(connectionClosed()), this, SLOT(slotConnectionClosed()));
  connect(m_socket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));

}

QuantaDebuggerGubed::~QuantaDebuggerGubed ()
{
  if(m_socket)
    delete m_socket;
}

// Try to make a connection to the gubed server
void QuantaDebuggerGubed::startSession() 
{
  QString host;
  Q_UINT16 port;
  
  // Get host from project settings
  host = serverHost;
  if(host.isEmpty())
    host = "localhost"; // Default to localhost
    
  // Get port from project settings
  port = serverPort.toUInt();
  if(port <= 0)
    port = 8026;  // Deault if not specified

  debuggerInterface()->enableAction("debug_connect", true);    
  m_socket->connectToHost(host, port);
  //kdDebug(24000) << "QuantaDebuggerGubed::startSession " << host << ", " << port << endl;

}


void QuantaDebuggerGubed::endSession() 
{
  // Close the socket
  m_socket->close();
  
  // Fake a connection closed signal
  slotConnectionClosed();
  
}

// Return capabilities of gubed
const uint QuantaDebuggerGubed::supports(DebuggerClient::Capabilities cap)
{
  switch(cap)
  {
    case DebuggerClient::LineBreakpoints:
    case DebuggerClient::StartSession:
    case DebuggerClient::EndSession:
    case DebuggerClient::Kill:
    case DebuggerClient::Pause:
    case DebuggerClient::Run:
    case DebuggerClient::RunDisplay:
    case DebuggerClient::Skip:
    case DebuggerClient::StepInto:
    case DebuggerClient::Watches:
      return true;
      
    default: 
      return false;
  }
  
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
      debuggerInterface()->setActiveLine(mapServerPathToLocal(data.left(data.find(':'))), data.mid(data.find(':') + 1).toLong());

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
      debuggerInterface()->haveBreakpoint(mapServerPathToLocal(data.left(data.find(':'))), data.mid(data.find(':') + 1).toLong());
    }
    // We're about to debug a file..
    else if(m_command == "initialize")
    {
      debuggerInterface()->setActiveLine(mapServerPathToLocal(data.left(data.find(':'))), 0);
      sendCommand("havesource", "");
      debuggingState(true);
    }
    else if(m_command == "sendingwatches")
    {
      //debuggerInterface()->preWatchUpdate();
    }
    else if(m_command == "watch")
    {
      showWatch(data);
    }
    else if(m_command == "sentwatches")
    {
      //debuggerInterface()->postWatchUpdate();
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
  debuggerInterface()->enableAction("debug_stepinto", enable);
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
   return "Gubed";      // no i18n on the name
}

void QuantaDebuggerGubed::showWatch(QString data)
{
  debuggerInterface()->parsePHPVariables(data);
  //DebuggerVariable *var = debuggerInterface()->newDebuggerVariable("test", "testvalue", DebuggerVariableTypes::Scalar);
  //debuggerInterface()->addVariable(var);
}

// Run boy, run (and show whats happening)
void QuantaDebuggerGubed::run()
{
   sendCommand("rundisplay", "");
}

// Go as fast as possible and dont update current line or watches
void QuantaDebuggerGubed::leap()
{
   sendCommand("runnodisplay", "");
}

// Step into function
void QuantaDebuggerGubed::stepInto()
{
   sendCommand("next", "");
}

// Skip next function
void QuantaDebuggerGubed::skip()
{
   sendCommand("skip", "");
}

// Kill the running script
void QuantaDebuggerGubed::kill()
{
   sendCommand("kill", "");
}

// Pause execution
void QuantaDebuggerGubed::pause()
{
   sendCommand("pause", "");
}


// Add a breakpoint
void QuantaDebuggerGubed::addBreakpoint (DebuggerBreakpoint* breakpoint)
{
  sendCommand("breakpoint", mapLocalPathToServer(breakpoint->filePath()) + ":" + QString::number(breakpoint->line()));
}
 
// Clear a breakpoint 
void QuantaDebuggerGubed::removeBreakpoint(DebuggerBreakpoint* breakpoint)
{
  sendCommand("clearpoint", mapLocalPathToServer(breakpoint->filePath()) + ":" + QString::number(breakpoint->line()));
}

// A file was opened...
void QuantaDebuggerGubed::fileOpened(QString)
{
  sendCommand("reinitialize", "");
}

// Read configuration
void QuantaDebuggerGubed::readConfig(QDomNode node)
{
  // Server
  QDomNode valuenode = node.namedItem("serverhost");
  serverHost = valuenode.firstChild().nodeValue(); 
  
  valuenode = node.namedItem("serverport");
  serverPort = valuenode.firstChild().nodeValue(); 
  
  valuenode = node.namedItem("localbasedir");
  localBasedir = valuenode.firstChild().nodeValue(); 
  
  valuenode = node.namedItem("serverbasedir");
  serverBasedir = valuenode.firstChild().nodeValue(); 
  

}


// Show configuration
void QuantaDebuggerGubed::showConfig(QDomNode node)
{
  GubedSettings set;
  
  readConfig(node);
  set.lineServerHost->setText(serverHost);
  set.lineServerPort->setText(serverPort);
  set.lineLocalBasedir->setText(localBasedir);
  set.lineServerBasedir->setText(serverBasedir);
  
  if(set.exec() == QDialog::Accepted )
  {
    QDomElement el;
    
    el = node.namedItem("serverhost").toElement();
    if (!el.isNull())
       el.parentNode().removeChild(el);
    el = node.ownerDocument().createElement("serverhost");
    node.appendChild( el );
    serverHost = set.lineServerHost->text();
    el.appendChild(node.ownerDocument().createTextNode(serverHost));
    
    el = node.namedItem("serverport").toElement();
    if (!el.isNull())
       el.parentNode().removeChild(el);
    el = node.ownerDocument().createElement("serverport");
    node.appendChild( el );
    serverPort = set.lineServerPort->text();
    el.appendChild( node.ownerDocument().createTextNode(serverPort) );
    
    el = node.namedItem("localbasedir").toElement();
    if (!el.isNull())
       el.parentNode().removeChild(el);
    el = node.ownerDocument().createElement("localbasedir");
    node.appendChild( el );
    localBasedir = set.lineLocalBasedir->text();
    el.appendChild( node.ownerDocument().createTextNode(localBasedir) );
    
    el = node.namedItem("serverbasedir").toElement();
    if (!el.isNull())
       el.parentNode().removeChild(el);
    el = node.ownerDocument().createElement("serverbasedir");
    node.appendChild( el );
    serverBasedir = set.lineServerBasedir->text();
    el.appendChild( node.ownerDocument().createTextNode(serverBasedir) );
    
  }
}


// Map a server filepath to a local one using project settings
QString QuantaDebuggerGubed::mapServerPathToLocal(QString serverpath)
{
#if KDE_IS_VERSION(3,2,0)   
   // Translate filename from server to local
   if(serverpath.startsWith(serverBasedir, false))
      serverpath.remove(0, serverBasedir.length());
#else
   QString lPath = serverpath.lower();
   QString lBaseDir = serverBasedir.lower();
   if(lPath.startsWith(lBaseDir))
      serverpath.remove(0, serverBasedir.length());
#endif

   return localBasedir + serverpath;
}

// Map a local filepath to a server one using project settings
QString QuantaDebuggerGubed::mapLocalPathToServer(QString localpath)
{
#if KDE_IS_VERSION(3,2,0)   
    if(localpath.startsWith(localBasedir, false))
       localpath.remove(0, localBasedir.length());
#else
   QString lPath = localpath.lower();
   QString lBaseDir = localBasedir.lower();
   if(lPath.startsWith(lBaseDir))
      localpath.remove(0, localBasedir.length());
#endif
   return serverBasedir + localpath;
}

#include "quantadebuggergubed.moc"
