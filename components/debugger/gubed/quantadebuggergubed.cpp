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
#include <kextsock.h>
#include <klocale.h>
#include <kgenericfactory.h>
#include <qlineedit.h>
#include <qslider.h>
#include <qcheckbox.h>
#include <kdeversion.h>
#include <errno.h>

#include "debuggerclient.h"
#include "quantadebuggergubed.h"
#include "debuggerinterface.h"
#include "debuggerbreakpoint.h"
#include "gubedsettings.h"
#include "debuggervariable.h"
#include "variableslistview.h"



K_EXPORT_COMPONENT_FACTORY( quantadebuggergubed,
                            KGenericFactory<QuantaDebuggerGubed>("quantadebuggergubed"))

const char QuantaDebuggerGubed::protocolversion[] = "0.0.10";

QuantaDebuggerGubed::QuantaDebuggerGubed (QObject *parent, const char* name, const QStringList&)
    : DebuggerClient (parent, name)
{
  // Create a socket object and set up its signals
  m_socket = NULL;
  m_server = NULL;
  m_errormask = 1794;
  m_executionState = Pause;
  m_datalen = -1;
}

QuantaDebuggerGubed::~QuantaDebuggerGubed ()
{

  kdDebug(24000) << k_funcinfo << ", m_server: " << m_server << ", m_socket" << m_socket << endl;

  if(m_socket)
  {
    sendCommand("die", "");
    m_socket->flush();
    m_socket->close();
  }
  if(m_server)
    delete m_server;
}

// Try to make a connection to the gubed server
void QuantaDebuggerGubed::startSession()
{

  kdDebug(24000) << k_funcinfo << ", m_server: " << m_server << ", m_socket" << m_socket << endl;
  if(m_useproxy)
  {
    if(!m_socket)
    {
      m_socket = new KExtendedSocket(m_serverHost, m_serverPort.toUInt(), KExtendedSocket::inetSocket |  KExtendedSocket::inputBufferedSocket);
      m_socket->enableRead(true);
      m_socket->setBufferSize(-1);
      connect(m_socket, SIGNAL(connectionFailed(int)), this, SLOT(slotError(int)));
      connect(m_socket, SIGNAL(connectionSuccess()), this, SLOT(slotConnected()));
      connect(m_socket, SIGNAL(closed(int)), this, SLOT(slotConnectionClosed(int)));
      connect(m_socket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));

      m_socket->startAsyncConnect();
      debuggerInterface()->enableAction("debug_connect", true);
      debuggerInterface()->enableAction("debug_disconnect", false);
      kdDebug(24000) << k_funcinfo << ", proxy:" << m_serverHost << ", " << m_serverPort.toUInt() << endl;
    }
  }
  else
  {
    if(!m_server)
    {
      m_server = new KExtendedSocket(QString::null, m_listenPort.toUInt(), KExtendedSocket::inetSocket | KExtendedSocket::passiveSocket);

      m_server->setAddressReusable(true);
      connect(m_server, SIGNAL(readyAccept()), this, SLOT(slotReadyAccept()));

      int err = m_server->listen();
      kdDebug(24000) << k_funcinfo << ", listen:" << m_listenPort.toUInt() << " " << err << " " << m_server->systemError() << " " << KExtendedSocket::strError(err, m_server->systemError()) << endl;
      if(err)
      {
        debuggerInterface()->showStatus(KExtendedSocket::strError(err, m_server->systemError()), false);
        delete  m_server;
        m_server = NULL;
        debuggerInterface()->enableAction("debug_connect", true);
        debuggerInterface()->enableAction("debug_disconnect", false);
      }
      else
      {
        debuggerInterface()->enableAction("debug_connect", false);
        debuggerInterface()->enableAction("debug_disconnect", true);
      }
    }
  }
  debuggerInterface()->enableAction("debug_run", true);
  debuggerInterface()->enableAction("debug_leap", true);
  debuggerInterface()->enableAction("debug_pause", true);

}


void QuantaDebuggerGubed::endSession()
{

  kdDebug(24000) << k_funcinfo << ", m_server: " << m_server << ", m_socket" << m_socket << endl;

  // Close the socket
  if(m_socket)
  {
    sendCommand("die", "");
    m_socket->flush();
    m_socket->close();
    KExtendedSocket * oldsocket = m_socket;
    m_socket = NULL;
    delete oldsocket;
  }
  // Close the server
  if(m_server)
  {
    delete m_server;
    m_server = NULL;
  }

  // Fake a connection closed signal
  slotConnectionClosed(0);
  debuggerInterface()->enableAction("debug_run", false);
  debuggerInterface()->enableAction("debug_leap", false);
  debuggerInterface()->enableAction("debug_pause", false);

}

// Change executionstate of the script
void QuantaDebuggerGubed::setExecutionState(State newstate)
{
  if(newstate == Pause)
  {
    sendCommand("pause", "");
    sendCommand("sendactiveline", "");
  }
  else if(newstate == RunNoDisplay)
  {
    if(m_executionState == Pause)
      sendCommand("next", "");

    sendCommand("runnodisplay", "");
  }
  else if(newstate == RunDisplay)
  {
    if(m_executionState == Pause)
      sendCommand("next", "");

    sendCommand("rundisplay", "");
  }

  m_executionState = newstate;
  kdDebug(24000) << k_funcinfo << ", " << m_executionState << endl;

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
    case DebuggerClientCapabilities::RunDisplay:
    case DebuggerClientCapabilities::Skip:
    case DebuggerClientCapabilities::StepInto:
    case DebuggerClientCapabilities::Watches:
    case DebuggerClientCapabilities::VariableSetValue:
      return true;

    default:
      return false;
  }
}

// Socket errors
void QuantaDebuggerGubed::slotError(int error)
{
  kdDebug(24000) << k_funcinfo << ", m_server: " << m_server << ", m_socket" << m_socket << endl;
  if(m_socket)
    endSession();

  if(m_server)
  {
    kdDebug(24000) << k_funcinfo << ", " << KExtendedSocket::strError(error, m_server->systemError()) << endl;
    debuggerInterface()->showStatus(KExtendedSocket::strError(error,m_server->systemError()), false);
  }
  else if(m_socket)
  {
    kdDebug(24000) << k_funcinfo << ", " << KExtendedSocket::strError(error, m_socket->systemError()) << endl;
    debuggerInterface()->showStatus(KExtendedSocket::strError(error,m_socket->systemError()), false);
  }
}

// slotReadyAccept
void QuantaDebuggerGubed::slotReadyAccept()
{

  kdDebug(24000) << k_funcinfo << ", m_server: " << m_server << ", m_socket" << m_socket << endl;
  if(!m_socket)
  {
    int error;
    disconnect(m_server, SIGNAL(readyAccept()), this, SLOT(slotReadyAccept()));

    m_socket = new KExtendedSocket();
    error = m_server->accept(m_socket);
    if(error == 0)
    {
      kdDebug(24000) << k_funcinfo << ", ready" << endl;
      m_socket->enableRead(true);
      m_socket->setSocketFlags(KExtendedSocket::inetSocket |  KExtendedSocket::inputBufferedSocket);
      m_socket->setBufferSize(-1);
      connect(m_socket, SIGNAL(connectionFailed(int)), this, SLOT(slotError(int)));
      connect(m_socket, SIGNAL(connectionSuccess()), this, SLOT(slotConnected()));
      connect(m_socket, SIGNAL(closed(int)), this, SLOT(slotConnectionClosed(int)));
      connect(m_socket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
      slotConnected();
    }
    else
    {
      kdDebug(24000) << k_funcinfo << ", " << KExtendedSocket::strError(error, m_server->systemError()) << endl;
      delete m_socket;
      m_socket = NULL;
    }
  }

}

// Connection established
void QuantaDebuggerGubed::slotConnected()
{
  kdDebug(24000) << k_funcinfo << endl;

  sendCommand("wait" ,"");
  debuggerInterface()->enableAction("debug_connect", false);
  debuggerInterface()->enableAction("debug_disconnect", true);

  m_active = true;
}

// Connectio closed
void QuantaDebuggerGubed::slotConnectionClosed(int state)
{
  kdDebug(24000) << k_funcinfo << ", state: " << state << ", m_server: " << m_server << ", m_socket" << m_socket << endl;

  if(m_socket)
  {
    delete m_socket;
    m_socket = NULL;
  }

  if(m_server)
    connect(m_server, SIGNAL(readyAccept()), this, SLOT(slotReadyAccept()));

  // Disable all session related actions and enable connection action
  debuggerInterface()->enableAction("*", false);
  debuggerInterface()->enableAction("debug_connect", m_useproxy == 1 || m_server == NULL);
  debuggerInterface()->enableAction("debug_disconnect", m_useproxy == 0 && m_server != NULL);
  debuggerInterface()->enableAction("debug_run", true);
  debuggerInterface()->enableAction("debug_leap", true);
  debuggerInterface()->enableAction("debug_pause", true);
  debuggerInterface()->enableAction("debug_breakpoints_toggle", true);
  debuggerInterface()->enableAction("debug_breakpoints_clear", true);

  m_active = false;
}

// Data from socket
void QuantaDebuggerGubed::slotReadyRead()
{

  // Data from gubed comes in line terminated pairs
  while(m_socket && m_socket->bytesAvailable() > 0)
  {
    QString data;

    // Read all available bytes from socket and append them to the buffer
    int bytes = m_socket->bytesAvailable();
    char* buffer = new char[bytes + 1];
    m_socket->readBlock(buffer, bytes);
    buffer[bytes] = 0;
    m_buffer += buffer;
    delete buffer;

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
void QuantaDebuggerGubed::processCommand(QString data)
{
  kdDebug(24000) << k_funcinfo << ", " << m_command << " : " << data << endl;

  // See what command we got and act accordingly..
  if(m_command == "commandme")
  {
    //sendCommand("sendactiveline", "");
    debuggerInterface()->setActiveLine(mapServerPathToLocal(data.left(data.find(':'))), data.mid(data.find(':') + 1).toLong());
    sendWatches();
    if(m_executionState == RunDisplay)
      sendCommand("wait", "");

    if(m_executionState != Pause)
      sendCommand("next", "");
  }
  // Send run mode to script
  else if(m_command == "getrunmode")
  {
    debuggingState(true);
    sendCommand("setdisplaydelay", QString::number(m_displaydelay));
    if(m_executionState == Pause)
      sendCommand("pause", "");
    else if(m_executionState == RunNoDisplay)
      sendCommand("runnodisplay", "");
    else if(m_executionState == RunDisplay)
      sendCommand("rundisplay", "");
  }
  // Just some status info, display on status line
  else if(m_command == "status")
  {
    debuggerInterface()->showStatus(data, false);
  }
  // New current line
  else if(m_command == "setactiveline")
  {
    debuggerInterface()->setActiveLine(mapServerPathToLocal(data.left(data.find(':'))), data.mid(data.find(':') + 1).toLong());
  }
  // Script requests breakpointlist
  else if(m_command == "sendbreakpoints")
  {
    sendBreakpoints();
  }
  // Debugger tells its about to parse a file
  else if(m_command == "unparsed")
  {
    debuggerInterface()->showStatus(i18n("About to parse %1").arg(data), true);
    return;
  }
  // Parsing ok
  else if(m_command == "parseok")
  {
    debuggerInterface()->showStatus(i18n("%1 parsed ok").arg(data), true);
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
    QString filename, line, error;
    filename = data.left(data.find(':'));
    line = data.mid(data.find(':') + 1);
    error = line.mid(line.find(':') + 1);
    line = line.left(line.find(':'));

    // Put the line number first so double clicking will jump to the corrrect line
    debuggerInterface()->showStatus(i18n("Error occurred: Line %1, Code %2, (%3)").arg(line).arg(error).arg(filename), true);

    // Filter to get error code only and match it with out mask
    error = error.left(error.find(':'));
    if(m_errormask & error.toUInt())
      setExecutionState(Pause);
    else if(m_executionState == RunDisplay)
      setExecutionState(RunDisplay);
    else if(m_executionState == RunNoDisplay)
      setExecutionState(RunNoDisplay);
    else
      setExecutionState(Pause);

  }
  // We came across  a hard coded breakpoint
  else if(m_command == "forcebreak")
  {
    setExecutionState(Pause);
    debuggerInterface()->showStatus(i18n("Breakpoint reached: %1").arg(data), true);
  }
  // A conditional breakpoint was fulfilled
  else if(m_command == "conditionalbreak")
  {
    setExecutionState(Pause);
    debuggerInterface()->showStatus(i18n("Conditional breakpoint fulfilled: %1").arg(data), true);
  }
  // There is a breakpoint set in this file/line
  else if(m_command == "breakpoint")
  {
    debuggerInterface()->haveBreakpoint(mapServerPathToLocal(data.left(data.find(':'))), data.mid(data.find(':') + 1).toLong());
  }
  // We're about to debug a file..
  else if(m_command == "initialize")
  {
    debuggerInterface()->showStatus(i18n("Established connection to %1").arg(data), false);
    sendCommand("sendprotocolversion", "");

    debuggerInterface()->setActiveLine(mapServerPathToLocal(data.left(data.find(':'))), 0);
    sendCommand("havesource", "");
    debuggingState(true);
  }
  else if(m_command == "sendingwatches")
  {
    //debuggerInterface()->preWatchUpdate();
  }
  // Show the contents of a watched variable
  else if(m_command == "watch")
  {
    showWatch(data);
  }
  // Show the contents of a variable
  else if(m_command == "variable")
  {
    showWatch(data);
  }
  // Show the contents of a variable
  else if(m_command == "showcondition")
  {
    showCondition(data);
  }
  else if(m_command == "sentwatches")
  {
    //debuggerInterface()->postWatchUpdate();
  }
  // Reached en of an include
  else if(m_command == "end")
  {
    debuggerInterface()->showStatus(i18n("At end of include %1").arg(data), true);
    return;
  }
  // Check protocol version
  else if(m_command == "protocolversion")
  {
    if(data != protocolversion)
    {
      debuggerInterface()->showStatus(i18n("The script being debugged does not communicate with the correct protocol version"), true);
      sendCommand("die", "");
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
    kdDebug(24000) << "QuantaDebuggerGubed::slotReadyRead Unknown: " << m_command << ":" << data << endl;
}

// Turn on/off actions related to a debugging session
void QuantaDebuggerGubed::debuggingState(bool enable)
{
  debuggerInterface()->enableAction("debug_kill", enable);
  debuggerInterface()->enableAction("debug_stepinto", enable);
  debuggerInterface()->enableAction("debug_skip", enable);
}

void QuantaDebuggerGubed::sendBreakpoints()
{
  debuggerInterface()->refreshBreakpoints();
}
void QuantaDebuggerGubed::sendWatches()
{
  for(QValueList<QString>::iterator it = m_watchlist.begin(); it != m_watchlist.end(); it++)
    sendCommand("getwatch", (*it));
  sendCommand("sentwatches", "");
}

// Send a command to gubed
bool QuantaDebuggerGubed::sendCommand(QString command, QString data)
{
  kdDebug(24000) << k_lineinfo << ", command: " << command << ", data " << data << endl;
  if(!m_socket || m_socket->socketStatus() != KExtendedSocket::connected)
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
QString QuantaDebuggerGubed::getName()
{
  return "Gubed";      // no i18n on the name
}

void QuantaDebuggerGubed::showWatch(QString data)
{
  debuggerInterface()->parsePHPVariables(data);
}

// Run boy, run (and show whats happening)
void QuantaDebuggerGubed::run()
{
  setExecutionState(RunDisplay);
}

// Go as fast as possible and dont update current line or watches
void QuantaDebuggerGubed::leap()
{
  setExecutionState(RunNoDisplay);
}

// Step into function
void QuantaDebuggerGubed::stepInto()
{
  setExecutionState(Pause);
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
  sendCommand("die", "");
}

// Pause execution
void QuantaDebuggerGubed::pause()
{
  setExecutionState(Pause);
}


// Add a breakpoint
void QuantaDebuggerGubed::addBreakpoint (DebuggerBreakpoint* breakpoint)
{
  if(breakpoint->condition().isEmpty())
    sendCommand("breakpoint", mapLocalPathToServer(breakpoint->filePath()) + ":" + QString::number(breakpoint->line()));
  else
  {
    sendCommand("conditionalbreakpoint", breakpoint->condition());
  }
}

// Clear a breakpoint
void QuantaDebuggerGubed::removeBreakpoint(DebuggerBreakpoint* breakpoint)
{
  if(breakpoint->condition().isEmpty())
    sendCommand("clearpoint", mapLocalPathToServer(breakpoint->filePath()) + ":" + QString::number(breakpoint->line()));
  else
    sendCommand("clearconditionalbreakpoint", breakpoint->condition());
}

// A file was opened...
void QuantaDebuggerGubed::fileOpened(QString)
{
  sendCommand("reinitialize", "");
}

// Watch a variable
void QuantaDebuggerGubed::addWatch(const QString &variable)
{
  if(m_watchlist.find(variable) == m_watchlist.end())
    m_watchlist.append(variable);
  sendCommand("getwatch", variable);
}
// Remove watch
void QuantaDebuggerGubed::removeWatch(DebuggerVariable *variable)
{
  if(m_watchlist.find(variable->name()) != m_watchlist.end())
    m_watchlist.remove(m_watchlist.find(variable->name()));
  //sendCommand("unwatchvariable", var->name());
}

// Show conditional breakpoint state
void QuantaDebuggerGubed::showCondition(const QString &expression)
{
  QString condition = expression.left(expression.find(":"));
  QString value = expression.mid(expression.find(":") + 1);

  DebuggerBreakpoint *bp = debuggerInterface()->newDebuggerBreakpoint();
  bp->setCondition(condition);
  bp->setLine(0);
  bp->setFilePath("");

  if(value == "F")
    bp->setState(DebuggerBreakpointStates::Unfulfilled);
  else if(value == "T")
    bp->setState(DebuggerBreakpointStates::Fulfilled);
  else if(value == "-")
    bp->setState(DebuggerBreakpointStates::Error);
  else
    bp->setState(DebuggerBreakpointStates::Undefined);

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

  valuenode = node.namedItem("serverbasedir");
  m_serverBasedir = valuenode.firstChild().nodeValue();

  valuenode = node.namedItem("listenport");
  m_listenPort = valuenode.firstChild().nodeValue();
  if(m_listenPort.isEmpty())
    m_listenPort = "8016";

  valuenode = node.namedItem("useproxy");
  m_useproxy = valuenode.firstChild().nodeValue() == "1";

  valuenode = node.namedItem("displaydelay");
  m_displaydelay = valuenode.firstChild().nodeValue().toLong();

  valuenode = node.namedItem("errormask");
  m_errormask = valuenode.firstChild().nodeValue().toLong();
  kdDebug(24000) << k_funcinfo << ", m_errormask = " << m_errormask << endl;
}


// Show configuration
void QuantaDebuggerGubed::showConfig(QDomNode node)
{
  GubedSettings set;

  readConfig(node);

  set.lineServerHost->setText(m_serverHost);
  set.lineServerPort->setText(m_serverPort);
  set.lineLocalBasedir->setText(m_localBasedir);
  set.lineServerBasedir->setText(m_serverBasedir);
  set.lineServerListenPort->setText(m_listenPort);
  set.checkUseProxy->setChecked(m_useproxy);
  set.sliderDisplayDelay->setValue(m_displaydelay);

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
    el.appendChild( node.ownerDocument().createTextNode(m_localBasedir) );

    el = node.namedItem("serverbasedir").toElement();
    if (!el.isNull())
      el.parentNode().removeChild(el);
    el = node.ownerDocument().createElement("serverbasedir");
    node.appendChild( el );
    m_serverBasedir = set.lineServerBasedir->text();
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
    kdDebug(24000) << k_funcinfo << ", m_errormask = " << m_errormask << endl;
    el.appendChild( node.ownerDocument().createTextNode(QString::number(m_errormask)));

  }
}

// Map a server filepath to a local one using project settings
QString QuantaDebuggerGubed::mapServerPathToLocal(QString serverpath)
{

  // Translate filename from server to local
  if(!serverpath.startsWith(m_serverBasedir, false))
  return serverpath;

  serverpath.remove(0, m_serverBasedir.length());
  return m_localBasedir + serverpath;
}

// Map a local filepath to a server one using project settings
QString QuantaDebuggerGubed::mapLocalPathToServer(QString localpath)
{
  if(!localpath.startsWith(m_localBasedir, false))
    return localpath;

  localpath.remove(0, m_localBasedir.length());
  return m_serverBasedir + localpath;
}

void QuantaDebuggerGubed::variableSetValue(const DebuggerVariable &variable)
{
  sendCommand("setvariable", variable.name() + "=" + variable.value());
}

#include "quantadebuggergubed.moc"
