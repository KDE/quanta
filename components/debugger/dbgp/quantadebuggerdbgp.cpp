/***************************************************************************
                           quantadebuggerdbgp.cpp
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
#include <qslider.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <kdeversion.h>
#include <errno.h>
#include <qstring.h>
#include <qmap.h>

#include <stdarg.h>

#include "debuggerclient.h"
#include "quantadebuggerdbgp.h"
#include "debuggerinterface.h"
#include "debuggerbreakpoint.h"
#include "dbgpsettings.h"
#include "debuggervariable.h"
#include "variableslistview.h"
#include "pathmapper.h"



K_EXPORT_COMPONENT_FACTORY( quantadebuggerdbgp,
                            KGenericFactory<QuantaDebuggerDBGp>("quantadebuggerdbgp"))

const char QuantaDebuggerDBGp::protocolversion[] = "1.0";

QuantaDebuggerDBGp::QuantaDebuggerDBGp (QObject *parent, const char*, const QStringList&)
    : DebuggerClient (parent, "DBGp")
{
  // Create a socket object and set up its signals
  m_errormask = 1794;
  m_defaultExecutionState = Starting;
  setExecutionState(m_defaultExecutionState);

  connect(&m_network, SIGNAL(command(const QString&)), this, SLOT(processCommand(const QString&)));
  connect(&m_network, SIGNAL(active(bool)), this, SLOT(slotNetworkActive(bool)));
  connect(&m_network, SIGNAL(connected(bool)), this, SLOT(slotNetworkConnected(bool)));
  connect(&m_network, SIGNAL(networkError(const QString &, bool)), this, SLOT(slotNetworkError(const QString &, bool)));

}


QuantaDebuggerDBGp::~QuantaDebuggerDBGp ()
{

//   kdDebug(24002) << k_funcinfo << endl;

  m_network.sessionEnd();
}

void QuantaDebuggerDBGp::slotNetworkActive(bool active)
{
  debuggerInterface()->enableAction("debug_request", active);
  debuggerInterface()->enableAction("debug_connect", !active);
  debuggerInterface()->enableAction("debug_disconnect", active);

}

void QuantaDebuggerDBGp::slotNetworkConnected(bool connected)
{
  debuggerInterface()->enableAction("debug_run", connected);
  debuggerInterface()->enableAction("debug_leap", connected);
  debuggerInterface()->enableAction("debug_pause", connected);
}

void QuantaDebuggerDBGp::slotNetworkError(const QString &errormsg, bool log)
{
  debuggerInterface()->showStatus(errormsg, log);
}


// Try to make a connection to the dbgp server
void QuantaDebuggerDBGp::startSession()
{
  kdDebug(24002) << k_funcinfo << endl;

  m_network.sessionStart(m_useproxy, m_serverHost, m_useproxy ? m_serverPort : m_listenPort);
  setExecutionState(m_defaultExecutionState);
}


void QuantaDebuggerDBGp::endSession()
{

  kdDebug(24002) << k_funcinfo << endl;

  // Close the socket
  m_network.sessionEnd();

//   debuggerInterface()->enableAction("debug_request", false);
//   debuggerInterface()->enableAction("debug_run", false);
//   debuggerInterface()->enableAction("debug_leap", false);
//   debuggerInterface()->enableAction("debug_pause", false);

}

// Change executionstate of the script
void QuantaDebuggerDBGp::setExecutionState(State)
{
  /*if(newstate == Pause)
  {
    sendCommand("pause", 0);
    sendCommand("sendactiveline", 0);
  }
  else if(newstate == Run)
  {
    if(m_executionState == Pause)
      sendCommand("next", 0);

    sendCommand("run", 0);
  }
  else if(newstate == Trace)
  {
    if(m_executionState == Pause)
      sendCommand("next", 0);

    sendCommand("trace", 0);
  }

  m_executionState = newstate;

  if(debuggerInterface()) {
    debuggerInterface()->enableAction("debug_trace", m_executionState != Trace);
    debuggerInterface()->enableAction("debug_run", m_executionState != Run);
    debuggerInterface()->enableAction("debug_pause", m_executionState != Pause);
  }

  kdDebug(24002) << k_funcinfo << ", " << m_executionState << endl;*/

}

// Return capabilities of dbgp
const uint QuantaDebuggerDBGp::supports(DebuggerClientCapabilities::Capabilities cap)
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


// Process a dbgp command
void QuantaDebuggerDBGp::processCommand(const QString& datas)
{
  kdDebug(24002) << k_lineinfo << datas.left(50) << " (" << datas.length() << " bytes)" << endl;

/*  StringMap args = parseArgs(datas);

  // See what command we got and act accordingly..
  if(m_command == "commandme")
  {

  }
  else
    // Unimplemented command - log to debug output
    kdDebug(24002) << "QuantaDebuggerDBGp::slotReadyRead Unknown: " << m_command << ":" << datas << endl;*/
}

// Turn on/off actions related to a debugging session
void QuantaDebuggerDBGp::debuggingState(bool enable)
{
  debuggerInterface()->enableAction("debug_kill", enable);
  debuggerInterface()->enableAction("debug_stepout", enable);
  debuggerInterface()->enableAction("debug_stepinto", enable);
  debuggerInterface()->enableAction("debug_stepover", enable);
  debuggerInterface()->enableAction("debug_skip", enable);
}

void QuantaDebuggerDBGp::sendBreakpoints()
{
  debuggerInterface()->refreshBreakpoints();
}
void QuantaDebuggerDBGp::sendWatches()
{
  for(QValueList<QString>::iterator it = m_watchlist.begin(); it != m_watchlist.end(); ++it)
    sendCommand("getwatch", "variable", (*it).ascii(), 0);
  sendCommand("sentwatches", "key", 0, 0);
}
/*
// Send a command to dbgp
bool QuantaDebuggerDBGp::sendCommand(const QString& a_command, const QString& a_data)
{
  kdDebug(24002) << k_lineinfo << ", command: " << a_command << ", data " << a_data << endl;
  if(!m_socket || m_socket->socketStatus() != KExtendedSocket::connected)
    return false;

  // Needs line terminatino
  QString command = a_command;
  QString data = a_data;
  command += "\n";
  data += "\n";

  // Write data to socket
  m_socket->writeBlock(command, command.length());
  m_socket->writeBlock(data, data.length());

  return true;
}*/

// Send a command to dbgp
bool QuantaDebuggerDBGp::sendCommand(const QString& command, StringMap args)
{

  kdDebug(24002) << k_lineinfo << ", command " << command << " with data: " << phpSerialize(args) << endl;
  if(!m_network.isConnected())
    return false;

  QString buffer = phpSerialize(args);

  buffer = QString(command + ":%1;" + buffer).arg(buffer.length());
//   m_socket->writeBlock(buffer, buffer.length());
  return true;
}

// Send a command to dbgp
bool QuantaDebuggerDBGp::sendCommand(const QString& command, char * firstarg, ...)
{
  StringMap ca;
  char *next;

  va_list l_Arg;
  va_start(l_Arg, firstarg);

  next = firstarg;
  while(next)
  {
    ca[(QString)next] = (QString)va_arg(l_Arg, char*) ; 
//     kdDebug(24002) << k_lineinfo << " Added arg/valuepair " << next << ", " << ca[next].left(30) << endl;

    next = va_arg(l_Arg, char*);
  }

  va_end(l_Arg);
  sendCommand(command, ca);
  return true;
}

// Return name of debugger
QString QuantaDebuggerDBGp::getName()
{
  return "DBGp";      // no i18n on the name
}

void QuantaDebuggerDBGp::showWatch(const QString& data)
{
  debuggerInterface()->parsePHPVariables(data);
}

// Send HTTP Request
void QuantaDebuggerDBGp::request()
{
  QString request;
  request = debuggerInterface()->activeFileParts(m_startsession);

  //if(request.startsWith(m_localBasedir, false))
  //  request.remove(0, m_localBasedir.length());

  //request = m_startsession + request;
  kdDebug(24002) << k_funcinfo << ", request: " << request << endl;
  debuggerInterface()->sendRequest(request);
}


// Go as fast as possible and dont update current line or watches
void QuantaDebuggerDBGp::run()
{
  //setExecutionState(Run);
}

// Step into function
void QuantaDebuggerDBGp::stepInto()
{
//   setExecutionState(Pause);
//   sendCommand("next", 0);
}

// Step over function
void QuantaDebuggerDBGp::stepOver()
{
//   setExecutionState(Pause);
//   sendCommand("stepover", 0);
}

// Step out of function
void QuantaDebuggerDBGp::stepOut()
{
//   setExecutionState(Pause);
//   sendCommand("stepout", 0);
}

// Skip next function
void QuantaDebuggerDBGp::skip()
{
//   sendCommand("skip", 0);
}

// Kill the running script
void QuantaDebuggerDBGp::kill()
{
//   sendCommand("die", 0);
}

// Pause execution
void QuantaDebuggerDBGp::pause()
{
//   setExecutionState(Pause);
}


// Add a breakpoint
void QuantaDebuggerDBGp::addBreakpoint (DebuggerBreakpoint* )
{
//   QString type;
//   if(breakpoint->type() == DebuggerBreakpoint::LineBreakpoint)
//     type = "line";
//   else if(breakpoint->type() == DebuggerBreakpoint::ConditionalTrue)
//     type = "true";
//   else
//     type = "change";
// 
//   sendCommand("breakpoint", 
//               "type", type.ascii(),
//               "filename", mapLocalPathToServer(breakpoint->filePath()).ascii(),
//               "class", breakpoint->inClass().ascii(),
//               "function", breakpoint->inFunction().ascii(),
//               "expression", breakpoint->condition().ascii(),
//               "line", QString::number(breakpoint->line()).ascii(),
//               0);
}


// Clear a breakpoint
void QuantaDebuggerDBGp::removeBreakpoint(DebuggerBreakpoint* )
{
//   QString type;
//   if(breakpoint->type() == DebuggerBreakpoint::LineBreakpoint)
//     type = "line";
//   else if(breakpoint->type() == DebuggerBreakpoint::ConditionalTrue)
//     type = "true";
//   else
//     type = "change";
// 
//   sendCommand("removebreakpoint", 
//               "type", type.ascii(),
//               "filename", mapLocalPathToServer(breakpoint->filePath()).ascii(),
//               "class", breakpoint->inClass().ascii(),
//               "function", breakpoint->inFunction().ascii(),
//               "expression", breakpoint->condition().ascii(),
//               "line", QString::number(breakpoint->line()).ascii(),
//               0);
}

// A file was opened...
void QuantaDebuggerDBGp::fileOpened(const QString&)
{
//   sendCommand("reinitialize", 0);
}

// Watch a variable
void QuantaDebuggerDBGp::addWatch(const QString &)
{
//   if(m_watchlist.find(variable) == m_watchlist.end())
//     m_watchlist.append(variable);
//   sendCommand("getwatch", "variable", variable.ascii(), 0);
}
// Remove watch
void QuantaDebuggerDBGp::removeWatch(DebuggerVariable *)
{
//   if(m_watchlist.find(variable->name()) != m_watchlist.end())
//     m_watchlist.remove(m_watchlist.find(variable->name()));
//   //sendCommand("unwatchvariable", var->name());
}

// Show conditional breakpoint state
void QuantaDebuggerDBGp::showCondition(const StringMap &)
{

//   DebuggerBreakpoint *bp = debuggerInterface()->newDebuggerBreakpoint();
//   bp->setType(args["type"] == "true" ? DebuggerBreakpoint::ConditionalTrue : DebuggerBreakpoint::ConditionalChange);
//   bp->setCondition(args["expression"]);
//   bp->setFilePath(mapServerPathToLocal(args["filename"]));
//   bp->setClass(args["class"]);
//   bp->setFunction(args["function"]);
//   bp->setValue(args["value"]);
// 
//   bp->setState(DebuggerBreakpoint::Undefined);
// 
//   debuggerInterface()->showBreakpoint(*bp);
}

// Read configuration
void QuantaDebuggerDBGp::readConfig(QDomNode node)
{
  // Server
  QDomNode valuenode = node.namedItem("serverhost");
  m_serverHost = valuenode.firstChild().nodeValue();
  if(m_serverHost.isEmpty())
    m_serverHost = "localhost";

  valuenode = node.namedItem("serverport");
  m_serverPort = valuenode.firstChild().nodeValue();
  if(m_serverPort.isEmpty())
    m_serverPort = "9000";

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
    m_listenPort = "9000";

  valuenode = node.namedItem("startsession");
  m_startsession = valuenode.firstChild().nodeValue();
  if(m_startsession.isEmpty())
    m_startsession = "http://localhost/DBGp/StartSession.php?gbdScript=/%rfpp";

  valuenode = node.namedItem("defaultexecutionstate");
  if(valuenode.firstChild().nodeValue().isEmpty())
    m_defaultExecutionState = Starting;
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
void QuantaDebuggerDBGp::showConfig(QDomNode node)
{
  DBGpSettings set(protocolversion);

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

  set.checkBreakOnNotice->setChecked(QuantaDebuggerDBGp::Notice & m_errormask);
  set.checkBreakOnWarning->setChecked(QuantaDebuggerDBGp::Warning & m_errormask);
  set.checkBreakOnUserNotice->setChecked(QuantaDebuggerDBGp::User_Notice & m_errormask);
  set.checkBreakOnUserWarning->setChecked(QuantaDebuggerDBGp::User_Warning & m_errormask);
  set.checkBreakOnUserError->setChecked(QuantaDebuggerDBGp::User_Error & m_errormask);

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
    m_errormask = (set.checkBreakOnNotice->isChecked() ? QuantaDebuggerDBGp::Notice : 0)
                  + (set.checkBreakOnWarning->isChecked() ? QuantaDebuggerDBGp::Warning : 0)
                  + (set.checkBreakOnUserNotice->isChecked() ? QuantaDebuggerDBGp::User_Notice : 0)
                  + (set.checkBreakOnUserWarning->isChecked() ? QuantaDebuggerDBGp::User_Warning : 0)
                  + (set.checkBreakOnUserError->isChecked() ? QuantaDebuggerDBGp::User_Error : 0);
    kdDebug(24002) << k_funcinfo << ", m_errormask = " << m_errormask << endl;
    el.appendChild( node.ownerDocument().createTextNode(QString::number(m_errormask)));

  }
}

// Map a server filepath to a local one using project settings
QString QuantaDebuggerDBGp::mapServerPathToLocal(const QString& serverpath)
{
  // Translate filename from server to local
  return debuggerInterface()->Mapper()->mapServerPathToLocal(serverpath);
}

// Map a local filepath to a server one using project settings
QString QuantaDebuggerDBGp::mapLocalPathToServer(const QString& localpath)
{
  // Translate filename from local to server
  return debuggerInterface()->Mapper()->mapLocalPathToServer(localpath);
}

void QuantaDebuggerDBGp::variableSetValue(const DebuggerVariable &variable)
{
  sendCommand("setvariable", 
              "variable", variable.name().ascii(),
              "value", variable.value().ascii(),
              0);
}

QString QuantaDebuggerDBGp::phpSerialize(StringMap args)
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


StringMap QuantaDebuggerDBGp::parseArgs(const QString &args)
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
#include "quantadebuggerdbgp.moc"
