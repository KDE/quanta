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
#include <kserversocket.h>
#include <kstreamsocket.h>
#include <klocale.h>
#include <kgenericfactory.h>
#include <qlineedit.h>
#include <qslider.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <kdeversion.h>
#include <errno.h>
#include <qstring.h>
#include <qmap.h>

#include <stdarg.h>

#include "debuggerclient.h"
#include "quantadebuggergubed.h"
#include "debuggerinterface.h"
#include "debuggerbreakpoint.h"
#include "gubedsettings.h"
#include "debuggervariable.h"
#include "variableslistview.h"
#include "pathmapper.h"

#include "debuggerui.h"


K_EXPORT_COMPONENT_FACTORY( quantadebuggergubed,
                            KGenericFactory<QuantaDebuggerGubed>("quantadebuggergubed"))

const char QuantaDebuggerGubed::protocolversion[] = "0.0.12";

QuantaDebuggerGubed::QuantaDebuggerGubed (QObject *parent, const char* name, const QStringList&)
    : DebuggerClient (parent, name)
{
  // Create a socket object and set up its signals
  m_socket = NULL;
  m_server = NULL;
  m_errormask = 1794;
  m_defaultExecutionState = Pause;
  setExecutionState(m_defaultExecutionState);

  emit updateStatus(DebuggerUI::NoSession);
  m_datalen = -1;
}

QuantaDebuggerGubed::~QuantaDebuggerGubed ()
{

  kdDebug(24002) << k_funcinfo << ", m_server: " << m_server << ", m_socket" << m_socket << endl;

  if(m_socket)
  {
    sendCommand("die", (char*)0L);
    m_socket->flush();
    m_socket->close();
    delete m_socket;
    m_socket = NULL;
  }
  if(m_server)
  {
    m_server->close();
    delete m_server;
    m_server = NULL;
  }
  emit updateStatus(DebuggerUI::NoSession);
}

// Try to make a connection to the gubed server
void QuantaDebuggerGubed::startSession()
{

  kdDebug(24002) << k_funcinfo << ", m_server: " << m_server << ", m_socket" << m_socket << endl;
  
  // Set default execution state
  setExecutionState(m_defaultExecutionState);

  if(m_useproxy)
  {
    if(!m_socket)
    {
      m_socket = new KNetwork::KStreamSocket(m_serverHost, m_serverPort);

      connect(m_socket, SIGNAL(gotError(int)), this, SLOT(slotError(int)));
      connect(m_socket, SIGNAL(connected(const KResolverEntry &)), this, SLOT(slotConnected(const KResolverEntry &)));
      connect(m_socket, SIGNAL(closed()), this, SLOT(slotConnectionClosed()));
      connect(m_socket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));

      m_socket->connect();
      debuggerInterface()->enableAction("debug_connect", true);
      debuggerInterface()->enableAction("debug_disconnect", false);
      debuggerInterface()->enableAction("debug_request", false);
      kdDebug(24002) << k_funcinfo << ", proxy:" << m_serverHost << ", " << m_serverPort.toUInt() << endl;

      emit updateStatus(DebuggerUI::AwaitingConnection);
    }
  }
  else
  {
    if(!m_server)
    {
      m_server = new KNetwork::KServerSocket(m_listenPort);

      m_server->setAddressReuseable(true);
      connect(m_server, SIGNAL(readyAccept()), this, SLOT(slotReadyAccept()));

      if(m_server->listen())
      {
        emit updateStatus(DebuggerUI::AwaitingConnection);
        debuggerInterface()->enableAction("debug_connect", false);
        debuggerInterface()->enableAction("debug_disconnect", true);
        debuggerInterface()->enableAction("debug_request", true);
      }
      else
      {
        emit updateStatus(DebuggerUI::NoSession);
        delete m_server;
        m_server = NULL;
        debuggerInterface()->enableAction("debug_connect", true);
        debuggerInterface()->enableAction("debug_disconnect", false);
        debuggerInterface()->enableAction("debug_request", false);
      }
    }
  }
  
}


void QuantaDebuggerGubed::endSession()
{

  kdDebug(24002) << k_funcinfo << ", m_server: " << m_server << ", m_socket" << m_socket << endl;

  // Close the socket
  if(m_socket)
  {
    sendCommand("die", (char*)0L);
    m_socket->flush();
    m_socket->close();

    m_socket->deleteLater();
    m_socket = NULL;
  }

  // Close the server
  if(m_server)
  {
    m_server->close();
    delete m_server;
    m_server = NULL;
  }

  // Fake a connection closed signal
  slotConnectionClosed();
  debuggerInterface()->enableAction("debug_request", false);
  debuggerInterface()->enableAction("debug_run", false);
  debuggerInterface()->enableAction("debug_leap", false);
  debuggerInterface()->enableAction("debug_pause", false);

  emit updateStatus(DebuggerUI::NoSession);
}

// Change executionstate of the script
void QuantaDebuggerGubed::setExecutionState(State newstate)
{
  if(newstate == Pause)
  {
    sendCommand("pause", (char*)0L);
    sendCommand("sendactiveline", (char*)0L);
    if(isActive())
      emit updateStatus(DebuggerUI::Paused);
  }
  else if(newstate == Run)
  {
    if(m_executionState == Pause)
      sendCommand("next", (char*)0L);

    sendCommand("run", (char*)0L);
    if(isActive())
      emit updateStatus(DebuggerUI::Running);
  }
  else if(newstate == Trace)
  {
    if(m_executionState == Pause)
      sendCommand("next", (char*)0L);

    sendCommand("trace", (char*)0L);
    if(isActive())
      emit updateStatus(DebuggerUI::Tracing);
  }

  m_executionState = newstate;

  if(debuggerInterface()) {
    debuggerInterface()->enableAction("debug_trace", m_executionState != Trace);
    debuggerInterface()->enableAction("debug_run", m_executionState != Run);
    debuggerInterface()->enableAction("debug_pause", m_executionState != Pause);
  }

  kdDebug(24002) << k_funcinfo << ", " << m_executionState << endl;

}

// Return capabilities of gubed
const uint QuantaDebuggerGubed::supports(DebuggerClientCapabilities::Capabilities cap)
{
  switch(cap)
  {
    case DebuggerClientCapabilities::LineBreakpoints:
    case DebuggerClientCapabilities::ConditionalBreakpoints:
    case DebuggerClientCapabilities::StartSession:
    case DebuggerClientCapabilities::EndSession:
    case DebuggerClientCapabilities::Kill:
    case DebuggerClientCapabilities::Pause:
    case DebuggerClientCapabilities::Run:
    case DebuggerClientCapabilities::Trace:
    case DebuggerClientCapabilities::Skip:
    case DebuggerClientCapabilities::StepOut:
    case DebuggerClientCapabilities::StepInto:
    case DebuggerClientCapabilities::StepOver:
    case DebuggerClientCapabilities::Watches:
    case DebuggerClientCapabilities::VariableSetValue:
      return true;

    default:
      return false;
  }
}

// Socket errors
void QuantaDebuggerGubed::slotError(int)
{
  kdDebug(24002) << k_funcinfo << ", m_server: " << m_server << ", m_socket" << m_socket << endl;
  if(m_socket)
  {
    if(m_socket->error() == KNetwork::KSocketBase::RemotelyDisconnected)
    {
      slotConnectionClosed();
      return;
    }

    if(m_socket->error())
    {
      kdDebug(24002) << k_funcinfo << ", " << m_socket->errorString() << endl;
      debuggerInterface()->showStatus(m_socket->errorString(), false);
    }
  }

  if(m_server && m_server->error())
  {
    kdDebug(24002) << k_funcinfo << ", " << m_server->errorString() << endl;
    debuggerInterface()->showStatus(m_server->errorString(), false);
  }

}

// slotReadyAccept
void QuantaDebuggerGubed::slotReadyAccept()
{

  kdDebug(24002) << k_funcinfo << ", m_server: " << m_server << ", m_socket" << m_socket << endl;
  if(!m_socket)
  {

    // Perhaps this shouldnt be disconnected - instead check if connections are available at disconnect?
    disconnect(m_server, SIGNAL(readyAccept()), this, SLOT(slotReadyAccept()));

    m_socket = (KNetwork::KStreamSocket *)m_server->accept(); // KSocketServer returns a KStreamSocket (!)
    if(m_socket)
    {
      kdDebug(24002) << k_funcinfo << ", ready" << endl;
      m_socket->enableRead(true);

      connect(m_socket, SIGNAL(gotError(int)), this, SLOT(slotError(int)));
      connect(m_socket, SIGNAL(connected(const KResolverEntry &)), this, SLOT(slotConnected(const KResolverEntry &)));
      connect(m_socket, SIGNAL(closed()), this, SLOT(slotConnectionClosed()));
      connect(m_socket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
      connected();

      emit updateStatus(DebuggerUI::Connected);
    }
    else
    {
      kdDebug(24002) << k_funcinfo << ", " << m_server->errorString() << endl;
    }
  }

}

// Connection established
void QuantaDebuggerGubed::slotConnected(const KNetwork::KResolverEntry &)
{
  emit updateStatus(DebuggerUI::Connected);
  connected();
}

void QuantaDebuggerGubed::connected()
{
  kdDebug(24002) << k_funcinfo << endl;

  sendCommand("wait", (char*)0L);
  debuggerInterface()->enableAction("debug_connect", false);
  debuggerInterface()->enableAction("debug_disconnect", true);
  debuggerInterface()->enableAction("debug_request", false);

  m_active = true;  
}

// Connectio closed
void QuantaDebuggerGubed::slotConnectionClosed() 
{
  kdDebug(24002) << k_funcinfo << ", m_server: " << m_server << ", m_socket" << m_socket << endl;

  // Check if we have more data to read
  slotReadyRead();
  kdDebug(24002) << k_funcinfo << "buffer: " << m_buffer << endl;

  if(m_socket)
  {
    m_socket->deleteLater();
    m_socket = NULL;
  }

  if(m_server)
    connect(m_server, SIGNAL(readyAccept()), this, SLOT(slotReadyAccept()));

  // Disable all session related actions and enable connection action
  debuggerInterface()->enableAction("*", false);
  debuggerInterface()->enableAction("debug_connect", m_useproxy == 1 || m_server == NULL);
  debuggerInterface()->enableAction("debug_disconnect", m_useproxy == 0 && m_server != NULL);
  setExecutionState(m_defaultExecutionState);

  debuggerInterface()->enableAction("debug_request", true);
  debuggerInterface()->enableAction("debug_breakpoints_toggle", true);
  debuggerInterface()->enableAction("debug_breakpoints_clear", true);

  debuggerInterface()->setActiveLine("", 0);

  emit updateStatus(DebuggerUI::AwaitingConnection);
  m_active = false;
}

// Data from socket
void QuantaDebuggerGubed::slotReadyRead()
{

  // Data from gubed
  while(m_socket && (m_socket->bytesAvailable() > 0 || m_buffer.length() >= (unsigned long)m_datalen))
  {
    int bytes;
    QString data;

    if(m_socket && m_socket->bytesAvailable() > 0)
    {
      // Read all available bytes from socket and append them to the buffer
      bytes = m_socket->bytesAvailable();
      char* buffer = new char[bytes + 1];
      m_socket->readBlock(buffer, bytes);
      buffer[bytes] = 0;
      m_buffer += buffer;
      delete[] buffer;
    }

    while(1)
    {
      // If datalen == -1, we didnt read the command yet, otherwise were reading data.
      if(m_datalen == -1)
      {
        bytes = m_buffer.find(";");
        if(bytes < 0)
          break;

        data = m_buffer.left(bytes);
        m_buffer.remove(0, bytes + 1);
        bytes = data.find(":");
        m_command = data.left(bytes);
        data.remove(0, bytes + 1);
        m_datalen = data.toLong();
      }
      if(m_datalen != -1 && (long)m_buffer.length() >= m_datalen)
      {
        data = m_buffer.left(m_datalen);
        m_buffer.remove(0, m_datalen);
        m_datalen = -1;
        processCommand(data);
      }
      else
        break;
    }
  }
}

// Process a gubed command
void QuantaDebuggerGubed::processCommand(const QString& datas)
{
  kdDebug(24002) << k_funcinfo << ", " << m_command << " : " << datas.left(50) << endl;
  StringMap args = parseArgs(datas);

  // See what command we got and act accordingly..
  if(m_command == "commandme")
  {
    //sendCommand("sendactiveline", (char*)0L);
    debuggerInterface()->setActiveLine(mapServerPathToLocal(args["filename"]), args["line"].toLong());
    sendWatches();
    if(m_executionState == Trace)
      sendCommand("wait", (char*)0L);

    if(m_executionState != Pause)
      sendCommand("next", (char*)0L);
  }
  // Send run mode to script
  else if(m_command == "getrunmode")
  {
    debuggingState(true);
    sendCommand("setdisplaydelay", "newdelay", QString::number(m_displaydelay).ascii(), (char*)0L);
    if(m_executionState == Pause)
      sendCommand("pause", (char*)0L);
    else if(m_executionState == Run)
      sendCommand("run", (char*)0L);
    else if(m_executionState == Trace)
      sendCommand("trace", (char*)0L);

    sendCommand("seterrormask", "errormask", QString::number(m_errormask).ascii(), (char*)0L);
  }
  // Just some status info, display on status line
  else if(m_command == "status")
  {
    long argcnt = args["args"].toLong();
    QString msg = i18n(args["message"]);  // How will we get these messages throught to the translators?
    for(int cnt = 1; cnt <= argcnt; cnt++)
      msg.replace("%" + QString("%1").arg(cnt) + "%", args[QString("arg%1").arg(cnt)]);

    debuggerInterface()->showStatus(msg, false);
  }
  // New current line
  else if(m_command == "setactiveline")
  {
    debuggerInterface()->setActiveLine(mapServerPathToLocal(args["filename"]), args["line"].toLong());
  }
  // Script requests breakpointlist
  else if(m_command == "sendbreakpoints")
  {
    sendBreakpoints();
  }
  // Parsing failed
  else if(m_command == "parsefailed")
  {
    debuggerInterface()->showStatus(i18n("Syntax or parse error in %1)").arg(args["filenme"]), true);
    return;
  }
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
    // Put the line number first so double clicking will jump to the corrrect line
    debuggerInterface()->showStatus(i18n("Error occurred: Line %1, Code %2 (%3) in  %4").arg(args["line"]).arg(args["errnum"]).arg(args["errmsg"]).arg(args["filename"]), true);

    // Filter to get error code only and match it with out mask
    long error = args["errnum"].toLong();
    if(m_errormask & error)
      setExecutionState(Pause);
    else if(m_executionState == Trace)
      setExecutionState(Trace);
    else if(m_executionState == Run)
      setExecutionState(Run);
    else
      setExecutionState(Pause);

    emit updateStatus(DebuggerUI::HaltedOnError);
  }
  // We came across  a hard coded breakpoint
  else if(m_command == "forcebreak")
  {
    setExecutionState(Pause);
    emit updateStatus(DebuggerUI::HaltedOnBreakpoint);
    debuggerInterface()->showStatus(i18n("Breakpoint reached"), true);
  }
  // A conditional breakpoint was fulfilled
  else if(m_command == "conditionalbreak")
  {
    setExecutionState(Pause);
    emit updateStatus(DebuggerUI::HaltedOnBreakpoint);
    debuggerInterface()->showStatus(i18n("Conditional breakpoint fulfilled"), true);
  }
  // There is a breakpoint set in this file/line
  else if(m_command == "removebreakpoint")
  {
    debuggerInterface()->havenoBreakpoint(mapServerPathToLocal(args["filename"]), args["line"].toLong());
  }
  // We're about to debug a file..
  else if(m_command == "initialize")
  {
    debuggerInterface()->showStatus(i18n("Established connection to %1").arg(args["filename"]), false);
    sendCommand("sendprotocolversion", (char*)0L);

    debuggerInterface()->setActiveLine(mapServerPathToLocal(args["filename"]), 0);
    sendCommand("havesource", (char*)0L);
    debuggingState(true);
  }
  else if(m_command == "sendingwatches")
  {
    //debuggerInterface()->preWatchUpdate();
  }
  // Show the contents of a watched variable
  else if(m_command == "watch")
  {
    showWatch(args["variable"]);
  }
  // Show the contents of a variable
  else if(m_command == "variable")
  {
    showWatch(args["variable"]);
  }
  // Show the contents of a variable
  else if(m_command == "showcondition")
  {
    showCondition(args); 
  }
  else if(m_command == "sentwatches")
  {
    //debuggerInterface()->postWatchUpdate();
  }
  // Reached en of an include
  else if(m_command == "end")
  {
    //debuggerInterface()->showStatus(i18n("At end of include %1").arg(data), true);
    return;
  }
  // Check protocol version
  else if(m_command == "protocolversion")
  {
    if(args["version"] != protocolversion)
    {
      debuggerInterface()->showStatus(i18n("The script being debugged does not communicate with the correct protocol version"), true);
      sendCommand("die", (char*)0L);
    }
    return;
  }
  // Instructions we currently ignore
  else if(m_command == "sourcesent"
          || m_command == "addsourceline"
         )
  {}
  else
    // Unimplemented command - log to debug output
    kdDebug(24002) << "QuantaDebuggerGubed::slotReadyRead Unknown: " << m_command << ":" << datas << endl;
}

// Turn on/off actions related to a debugging session
void QuantaDebuggerGubed::debuggingState(bool enable)
{
  debuggerInterface()->enableAction("debug_kill", enable);
  debuggerInterface()->enableAction("debug_stepout", enable);
  debuggerInterface()->enableAction("debug_stepinto", enable);
  debuggerInterface()->enableAction("debug_stepover", enable);
  debuggerInterface()->enableAction("debug_skip", enable);
}

void QuantaDebuggerGubed::sendBreakpoints()
{
  debuggerInterface()->refreshBreakpoints();
}
void QuantaDebuggerGubed::sendWatches()
{
  for(QValueList<QString>::iterator it = m_watchlist.begin(); it != m_watchlist.end(); ++it)
    sendCommand("getwatch", "variable", (*it).ascii(), (char*)0L);
  sendCommand("sentwatches", "key", (char*)0L, (char*)0L);
}

// Send a command to gubed
bool QuantaDebuggerGubed::sendCommand(const QString& command, StringMap args)
{

  kdDebug(24002) << k_lineinfo << ", command " << command << " with data: " << phpSerialize(args) << endl;
  if(!m_socket || m_socket->state() != KNetwork::KClientSocketBase::Connected)
    return false;

  QString buffer = phpSerialize(args);

  buffer = QString(command + ":%1;" + buffer).arg(buffer.length());
  m_socket->writeBlock(buffer, buffer.length());
  return true;
}

// Send a command to gubed
bool QuantaDebuggerGubed::sendCommand(const QString& command, char * firstarg, ...)
{
  StringMap ca;
  char *next;

  va_list l_Arg;
  va_start(l_Arg, firstarg);

  next = firstarg;
  while(next)
  {
    ca[(QString)next] = (QString)va_arg(l_Arg, char*);
//     kdDebug(24002) << k_lineinfo << " Added arg/valuepair " << next << ", " << ca[next].left(30) << endl;
    next = va_arg(l_Arg, char*);
  }

  va_end(l_Arg);
  sendCommand(command, ca);
  return true;
}

// Return name of debugger
QString QuantaDebuggerGubed::getName()
{
  return "Gubed";      // no i18n on the name
}

void QuantaDebuggerGubed::showWatch(const QString& data)
{
  debuggerInterface()->showVariable(parsePHPVariables(data));
}

// Send HTTP Request
void QuantaDebuggerGubed::request()
{
  QString request;
  request = debuggerInterface()->activeFileParts(m_startsession);

  //if(request.startsWith(m_localBasedir, false))
  //  request.remove(0, m_localBasedir.length());

  //request = m_startsession + request;
  kdDebug(24002) << k_funcinfo << ", request: " << request << endl;
  debuggerInterface()->sendRequest(request);
}


// Run boy, run (and show whats happening)
void QuantaDebuggerGubed::trace()
{
  setExecutionState(Trace);
}

// Go as fast as possible and dont update current line or watches
void QuantaDebuggerGubed::run()
{
  setExecutionState(Run);
}

// Step into function
void QuantaDebuggerGubed::stepInto()
{
  setExecutionState(Pause);
  sendCommand("next", (char*)0L);
}

// Step over function
void QuantaDebuggerGubed::stepOver()
{
  setExecutionState(Pause);
  sendCommand("stepover", (char*)0L);
}

// Step out of function
void QuantaDebuggerGubed::stepOut()
{
  setExecutionState(Pause);
  sendCommand("stepout", (char*)0L);
}

// Skip next function
void QuantaDebuggerGubed::skip()
{
  sendCommand("skip", (char*)0L);
}

// Kill the running script
void QuantaDebuggerGubed::kill()
{
  sendCommand("die", (char*)0L);
}

// Pause execution
void QuantaDebuggerGubed::pause()
{
  setExecutionState(Pause);
}


// Add a breakpoint
void QuantaDebuggerGubed::addBreakpoint (DebuggerBreakpoint* breakpoint)
{
  QString type;
  if(breakpoint->type() == DebuggerBreakpoint::LineBreakpoint)
    type = "line";
  else if(breakpoint->type() == DebuggerBreakpoint::ConditionalTrue)
    type = "true";
  else
    type = "change";

  sendCommand("breakpoint", 
              "type", type.ascii(),
              "filename", mapLocalPathToServer(breakpoint->filePath()).ascii(),
              "class", breakpoint->inClass().ascii(),
              "function", breakpoint->inFunction().ascii(),
              "expression", breakpoint->condition().ascii(),
              "line", QString::number(breakpoint->line()).ascii(),
              (char *)0L);
}

// QString QuantaDebuggerGubed::bpToGubed(DebuggerBreakpoint* breakpoint)
// {
//   return QString("^" + mapLocalPathToServer(breakpoint->filePath()) +
//       "^" + breakpoint->inClass() +
//       "^" + breakpoint->inFunction() +
//       "^" + (breakpoint->type() == DebuggerBreakpoint::ConditionalTrue ? "true" : "change") +
//       "^" + breakpoint->condition());
// }

// Clear a breakpoint
void QuantaDebuggerGubed::removeBreakpoint(DebuggerBreakpoint* breakpoint)
{
  QString type;
  if(breakpoint->type() == DebuggerBreakpoint::LineBreakpoint)
    type = "line";
  else if(breakpoint->type() == DebuggerBreakpoint::ConditionalTrue)
    type = "true";
  else
    type = "change";

  sendCommand("removebreakpoint", 
              "type", type.ascii(),
              "filename", mapLocalPathToServer(breakpoint->filePath()).ascii(),
              "class", breakpoint->inClass().ascii(),
              "function", breakpoint->inFunction().ascii(),
              "expression", breakpoint->condition().ascii(),
              "line", QString::number(breakpoint->line()).ascii(),
              (char*)0L);
}

// A file was opened...
void QuantaDebuggerGubed::fileOpened(const QString&)
{
  sendCommand("reinitialize", (char*)0L);
}

// Watch a variable
void QuantaDebuggerGubed::addWatch(const QString &variable)
{
  if(m_watchlist.find(variable) == m_watchlist.end())
    m_watchlist.append(variable);
  sendCommand("getwatch", "variable", variable.ascii(), (char*)0L);
}
// Remove watch
void QuantaDebuggerGubed::removeWatch(DebuggerVariable *variable)
{
  if(m_watchlist.find(variable->name()) != m_watchlist.end())
    m_watchlist.remove(m_watchlist.find(variable->name()));
  //sendCommand("unwatchvariable", var->name());
}

// Show conditional breakpoint state
void QuantaDebuggerGubed::showCondition(const StringMap &args)
{

  DebuggerBreakpoint *bp = debuggerInterface()->newDebuggerBreakpoint();
  bp->setType(args["type"] == "true" ? DebuggerBreakpoint::ConditionalTrue : DebuggerBreakpoint::ConditionalChange);
  bp->setCondition(args["expression"]);
  bp->setFilePath(mapServerPathToLocal(args["filename"]));
  bp->setClass(args["class"]);
  bp->setFunction(args["function"]);
  bp->setValue(args["value"]);

  bp->setState(DebuggerBreakpoint::Undefined);

  debuggerInterface()->showBreakpoint(*bp);
}

// Read configuration
void QuantaDebuggerGubed::readConfig(QDomNode node)
{
  // Server
  QDomNode valuenode = node.namedItem("serverhost");
  m_serverHost = valuenode.firstChild().nodeValue();
  if(m_serverHost.isEmpty())
    m_serverHost = "localhost";

  valuenode = node.namedItem("serverport");
  m_serverPort = valuenode.firstChild().nodeValue();
  if(m_serverPort.isEmpty())
    m_serverPort = "8026";

  valuenode = node.namedItem("localbasedir");
  m_localBasedir = valuenode.firstChild().nodeValue();
  if(debuggerInterface())
    debuggerInterface()->Mapper()->setLocalBasedir(m_localBasedir);

  valuenode = node.namedItem("serverbasedir");
  m_serverBasedir = valuenode.firstChild().nodeValue();
  if(debuggerInterface())
    debuggerInterface()->Mapper()->setServerBasedir(m_serverBasedir);

  valuenode = node.namedItem("listenport");
  m_listenPort = valuenode.firstChild().nodeValue();
  if(m_listenPort.isEmpty())
    m_listenPort = "8016";

  valuenode = node.namedItem("startsession");
  m_startsession = valuenode.firstChild().nodeValue();
  if(m_startsession.isEmpty())
    m_startsession = "http://localhost/Gubed/StartSession.php?gbdScript=/%rfpp";

  valuenode = node.namedItem("defaultexecutionstate");
  if(valuenode.firstChild().nodeValue().isEmpty())
    m_defaultExecutionState = Pause;
  else
    m_defaultExecutionState = (State)valuenode.firstChild().nodeValue().toUInt();

  valuenode = node.namedItem("useproxy");
  m_useproxy = valuenode.firstChild().nodeValue() == "1";

  valuenode = node.namedItem("displaydelay");
  m_displaydelay = valuenode.firstChild().nodeValue().toLong();

  valuenode = node.namedItem("errormask");
  m_errormask = valuenode.firstChild().nodeValue().toLong();
  kdDebug(24002) << k_funcinfo << ", m_errormask = " << m_errormask << endl;
}


// Show configuration
void QuantaDebuggerGubed::showConfig(QDomNode node)
{
  GubedSettings set(protocolversion);

  readConfig(node);

  set.lineServerHost->setText(m_serverHost);
  set.lineServerPort->setText(m_serverPort);
  set.lineLocalBasedir->setText(m_localBasedir);
  set.lineServerBasedir->setText(m_serverBasedir);
  set.lineServerListenPort->setText(m_listenPort);
  set.checkUseProxy->setChecked(m_useproxy);
  set.sliderDisplayDelay->setValue(m_displaydelay);
  set.lineStartSession->setText(m_startsession);
  set.comboDefaultExecutionState->setCurrentItem((int)m_defaultExecutionState);

  set.checkBreakOnNotice->setChecked(QuantaDebuggerGubed::Notice & m_errormask);
  set.checkBreakOnWarning->setChecked(QuantaDebuggerGubed::Warning & m_errormask);
  set.checkBreakOnUserNotice->setChecked(QuantaDebuggerGubed::User_Notice & m_errormask);
  set.checkBreakOnUserWarning->setChecked(QuantaDebuggerGubed::User_Warning & m_errormask);
  set.checkBreakOnUserError->setChecked(QuantaDebuggerGubed::User_Error & m_errormask);

  if(set.exec() == QDialog::Accepted )
  {
    QDomElement el;

    el = node.namedItem("serverhost").toElement();
    if (!el.isNull())
      el.parentNode().removeChild(el);
    el = node.ownerDocument().createElement("serverhost");
    node.appendChild( el );
    m_serverHost = set.lineServerHost->text();
    el.appendChild(node.ownerDocument().createTextNode(m_serverHost));

    el = node.namedItem("serverport").toElement();
    if (!el.isNull())
      el.parentNode().removeChild(el);
    el = node.ownerDocument().createElement("serverport");
    node.appendChild( el );
    m_serverPort = set.lineServerPort->text();
    el.appendChild( node.ownerDocument().createTextNode(m_serverPort) );

    el = node.namedItem("localbasedir").toElement();
    if (!el.isNull())
      el.parentNode().removeChild(el);
    el = node.ownerDocument().createElement("localbasedir");
    node.appendChild( el );
    m_localBasedir = set.lineLocalBasedir->text();
    if(debuggerInterface())
      debuggerInterface()->Mapper()->setLocalBasedir(m_localBasedir);
    el.appendChild( node.ownerDocument().createTextNode(m_localBasedir) );

    el = node.namedItem("serverbasedir").toElement();
    if (!el.isNull())
      el.parentNode().removeChild(el);
    el = node.ownerDocument().createElement("serverbasedir");
    node.appendChild( el );
    m_serverBasedir = set.lineServerBasedir->text();
    if(debuggerInterface())
      debuggerInterface()->Mapper()->setServerBasedir(m_serverBasedir);
    el.appendChild( node.ownerDocument().createTextNode(m_serverBasedir) );

    el = node.namedItem("useproxy").toElement();
    if (!el.isNull())
      el.parentNode().removeChild(el);
    el = node.ownerDocument().createElement("useproxy");
    node.appendChild( el );
    m_useproxy = set.checkUseProxy->isChecked();
    el.appendChild( node.ownerDocument().createTextNode(m_useproxy ? "1" : "0") );

    el = node.namedItem("listenport").toElement();
    if (!el.isNull())
      el.parentNode().removeChild(el);
    el = node.ownerDocument().createElement("listenport");
    node.appendChild( el );
    m_listenPort = set.lineServerListenPort->text();
    el.appendChild( node.ownerDocument().createTextNode(m_listenPort) );

    el = node.namedItem("startsession").toElement();
    if (!el.isNull())
      el.parentNode().removeChild(el);
    el = node.ownerDocument().createElement("startsession");
    node.appendChild( el );
    m_startsession = set.lineStartSession->text();
    el.appendChild(node.ownerDocument().createTextNode(m_startsession));

    el = node.namedItem("defaultexecutionstate").toElement();
    if (!el.isNull())
      el.parentNode().removeChild(el);
    el = node.ownerDocument().createElement("defaultexecutionstate");
    node.appendChild( el );
    m_defaultExecutionState = (State)set.comboDefaultExecutionState->currentItem();
    el.appendChild(node.ownerDocument().createTextNode(QString::number(m_defaultExecutionState)));


    el = node.namedItem("displaydelay").toElement();
    if (!el.isNull())
      el.parentNode().removeChild(el);
    el = node.ownerDocument().createElement("displaydelay");
    node.appendChild( el );
    m_displaydelay = set.sliderDisplayDelay->value();
    el.appendChild( node.ownerDocument().createTextNode(QString::number(m_displaydelay)));

    el = node.namedItem("errormask").toElement();
    if (!el.isNull())
      el.parentNode().removeChild(el);
    el = node.ownerDocument().createElement("errormask");
    node.appendChild( el );
    m_errormask = (set.checkBreakOnNotice->isChecked() ? QuantaDebuggerGubed::Notice : 0)
                  + (set.checkBreakOnWarning->isChecked() ? QuantaDebuggerGubed::Warning : 0)
                  + (set.checkBreakOnUserNotice->isChecked() ? QuantaDebuggerGubed::User_Notice : 0)
                  + (set.checkBreakOnUserWarning->isChecked() ? QuantaDebuggerGubed::User_Warning : 0)
                  + (set.checkBreakOnUserError->isChecked() ? QuantaDebuggerGubed::User_Error : 0);
    kdDebug(24002) << k_funcinfo << ", m_errormask = " << m_errormask << endl;
    el.appendChild( node.ownerDocument().createTextNode(QString::number(m_errormask)));

  }
}

// Map a server filepath to a local one using project settings
QString QuantaDebuggerGubed::mapServerPathToLocal(const QString& serverpath)
{
  // Translate filename from server to local
  return debuggerInterface()->Mapper()->mapServerPathToLocal(serverpath);
}

// Map a local filepath to a server one using project settings
QString QuantaDebuggerGubed::mapLocalPathToServer(const QString& localpath)
{
  // Translate filename from local to server
  return debuggerInterface()->Mapper()->mapLocalPathToServer(localpath);
}

void QuantaDebuggerGubed::variableSetValue(const DebuggerVariable &variable)
{
  sendCommand("setvariable", 
              "variable", variable.name().ascii(),
              "value", variable.value().ascii(),
              (char*)0L);
}

QString QuantaDebuggerGubed::phpSerialize(StringMap args)
{
  StringMap::Iterator it;
  // a:2:{s:4:"name";s:7:"Jessica";s:3:"age";s:2:"26";s:4:"test";i:1;}
  QString ret = QString("a:%1:{").arg(args.size());
  for( it = args.begin(); it != args.end(); ++it )
  {
    bool isNumber;

    it.data().toInt(&isNumber);
    if(isNumber && !it.data().isEmpty())
      ret += QString("s:%1:\"%2\";i:%3;")
                    .arg(it.key().length())
                    .arg(it.key())
                    .arg(it.data());
    else
      ret += QString("s:%1:\"%2\";s:%3:\"%4\";")
                    .arg(it.key().length())
                    .arg(it.key())
                    .arg(it.data().length())
                    .arg(it.data());

  }

  ret += "}";
  return ret;
}


StringMap QuantaDebuggerGubed::parseArgs(const QString &args)
{
  StringMap ca;
  long cnt, length;

  // a:2:{s:4:"name";s:7:"Jessica";s:3:"age";s:2:"26";s:4:"test";i:1;}

  // No args
  if(args.isEmpty() || args == "a:0:{}")
    return ca;

  // Make sure we have a good string
  if(!args.startsWith("a:"))
  {
    kdDebug(24002) << k_funcinfo << "An error occurred in the communication link, data received was:" << args << endl;
    return ca;
  }

  cnt = args.mid(2, args.find("{") - 3).toLong();
  QString data = args.mid(args.find("{") + 1);

  QString tmp, func;
  while(cnt > 0)
  {
    tmp = data.left(data.find("\""));
    length = tmp.mid(2, tmp.length() - 3).toLong();

    func = data.mid(tmp.length() + 1, length);
    data = data.mid( tmp.length() + length + 3);

    if(data.left(1) == "i")
    {
      // Integer data
      tmp = data.mid(data.find(":") + 1);
      tmp = tmp.left(tmp.find(";"));
      ca[func] = tmp;
      data = data.mid(tmp.length() + 3);
//       kdDebug(24002) << k_funcinfo << "**i " << func << ": " << ca[func] << endl;
    }
    else
    {
      // String data
      tmp = data.left(data.find("\""));
      length = tmp.mid(2, tmp.length() - 3).toLong();

      ca[func] = data.mid(tmp.length() + 1, length);
      data = data.mid( tmp.length() + length + 3);
//       kdDebug(24002) << k_funcinfo << "**s " << func << ": " << ca[func] << endl; 
   }
 
    cnt--;
  }

  return ca;
}

DebuggerVariable* QuantaDebuggerGubed::parsePHPVariables(const QString &varstring)
{
  QString str = varstring;
  DebuggerVariable* var = parsePHPVariables(str);
  return var;
}

DebuggerVariable* QuantaDebuggerGubed::parsePHPVariables(QString &str)
{
  QString key, data;
  QString tempstring;
  int length;
  DebuggerVariable* debuggervar = NULL;

  // get type of key
  QString type = str.left(1);
  str.remove(0, 2);

  // Strings
  if(type == "s")
  {
    // Get length of key
    tempstring = str.left(str.find(':'));
    str.remove(0, str.find(':') + 1);
    length = tempstring.toUInt();

    key = str.left(length + 1);
    key.remove(0, 1);        // remove starting quote
    str.remove(0, length + 3);
  }
  else if(type == "i")
  {
    key = str.left(str.find(';'));
    str.remove(0, str.find(';') + 1);

  }

  // Get type of data
  type = str.left(1);
  str.remove(0, 2);

  if(type == "i")
  {
    /* Example:
    s:4:"$row";i:6;
    */
    data = str.left(str.find(';'));
    str.remove(0, str.find(';') + 1);
    debuggervar = debuggerInterface()->newDebuggerVariable(key, data, DebuggerVariableTypes::Integer);

  }
  else if(type == "b")
  {
    /* Example:
    s:8:"$boolvar";b:1;
    */
    data = str.left(str.find(';'));
    data = (data == "0" ? i18n("False"): i18n("True"));
    str.remove(0, str.find(';') + 1);
    debuggervar = debuggerInterface()->newDebuggerVariable(key, data, DebuggerVariableTypes::Boolean);
  }
  else if(type == "N")
  {
    /* Example:
    s:6:"return";N;
    */
    debuggervar = debuggerInterface()->newDebuggerVariable(key, i18n("<Undefined>"), DebuggerVariableTypes::Undefined);
  }
  else if(type == "s")
  {
    /* Example:
    s:7:"$strvar";s:16:"This is a string";
    */

    // Get length of string
    tempstring = str.left(str.find(':'));
    str.remove(0, str.find(':') + 1);
    length = tempstring.toUInt();

    data = str.left(length + 1);
    data.remove(0, 1);        // remove starting quote
    str.remove(0, length + 3);
    debuggervar = debuggerInterface()->newDebuggerVariable(key, data, DebuggerVariableTypes::String);
    debuggervar->setSize(length);
  }
  else if(type == "a")
  {
    /* Example:
    s:6:"$array";a:5:{s:11:"Ingredients";a:3:{i:0;s:8:"potatoes";i:1;s:4:"salt";i:2;s:6:"pepper";}s:6:"Guests";a:4:{i:0;s:5:"Fiona";i:1;s:4:"Tori";i:2;s:4:"Neil";i:3;s:4:"Nick";}s:4:"Dogs";a:4:{i:0;s:5:"Kitty";i:1;s:5:"Tessy";i:2;s:5:"Fanny";i:3;s:5:"Cosmo";}s:7:"Numbers";a:6:{i:0;i:1;i:1;i:2;i:2;i:3;i:3;i:9;i:4;i:8;i:5;i:7;}s:6:"Letter";s:1:"L";}
    */

    // Get length of array
    tempstring = str.left(str.find(':'));
    str.remove(0, str.find(':') + 2);
    length = tempstring.toUInt();

    QPtrList<DebuggerVariable> vars ;
    while(length > 0)
    {
     //kdDebug(24002) << "VariablesListView::parsePHPVariables: length " << length << ", \"" << str << "\"" << endl;
   
      length --;
      DebuggerVariable* var = parsePHPVariables(str);
      if(var)
        vars.append(var);

    }
    str.remove(0, 1);
    debuggervar = debuggerInterface()->newDebuggerVariable(key, vars, DebuggerVariableTypes::Array);
  }
  else if(type == "O")
  {
    /* Example:
      
    */

    // Get length of array
    tempstring = str.left(str.find(':'));
    str.remove(0, str.find(':') + 2);
    tempstring = str.mid(str.find(':') + 1);
    tempstring = tempstring.left(tempstring.find(':'));
    length = tempstring.toUInt();

    str.remove(0, str.find('{') + 1);
    
    QPtrList<DebuggerVariable> vars ;
    while(length > 0)
    {
     //kdDebug(24002) << "VariablesListView::parsePHPVariables: length " << length << ", \"" << str << "\"" << endl;
   
      length --;
      DebuggerVariable* var = parsePHPVariables(str);
      if(var)
        vars.append(var);

    }
    str.remove(0, 1);
    debuggervar = debuggerInterface()->newDebuggerVariable(key, vars, DebuggerVariableTypes::Object);
  }
  else if(type == "d")
  {
    /* Example:
    s:9:"$floatvar";d:12.5600000000000004973799150320701301097869873046875;"
    */
    data = str.left(str.find(';'));
    str.remove(0, str.find(';') + 1);
    debuggervar = debuggerInterface()->newDebuggerVariable(key, data, DebuggerVariableTypes::Float);

  }
  else if(type == "-")
  {
    debuggervar = debuggerInterface()->newDebuggerVariable(key,  i18n("<Undefined>"), DebuggerVariableTypes::Undefined);
  }
  else if(type == "!")
  {
    debuggervar = debuggerInterface()->newDebuggerVariable(key,  i18n("<Error>"), DebuggerVariableTypes::Error);
  }
  else
  {
    kdDebug(24002) << "VariablesListView::parsePHPVariables: Unknown variable type " << type << ", " << str << endl;
    debuggervar = debuggerInterface()->newDebuggerVariable(key, i18n("<Unimplemented type>"), DebuggerVariableTypes::Error);
  }

  return debuggervar;

}


#include "quantadebuggergubed.moc"
