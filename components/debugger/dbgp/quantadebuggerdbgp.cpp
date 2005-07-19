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
  m_supportsasync = false;
  m_defaultExecutionState = Starting;
//  setExecutionState(m_defaultExecutionState);

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
  debuggerInterface()->enableAction("debug_kill", connected);

  debuggerInterface()->setActiveLine("", 0);

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
  setExecutionState("starting");
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
void QuantaDebuggerDBGp::setExecutionState(const QString &state)
{
  if(state == "starting")
  {
    m_executionState = Starting;
  }
  else if(state == "stopping")
  {
    m_executionState = Stopping;
  }
  else if(state == "stopped")
  {
    m_executionState = Stopped;
  }
  else if(state == "running")
  {
    m_executionState = Running;
  }
  else if(state == "break")
  {
    m_executionState = Break;
  }

  if(debuggerInterface()) {
    debuggerInterface()->enableAction("debug_run", m_executionState == Break || m_executionState == Starting);
    debuggerInterface()->enableAction("debug_pause", m_executionState == Running && m_supportsasync);
    debuggerInterface()->enableAction("debug_kill", m_executionState == Break || (m_executionState == Running && m_supportsasync) || m_executionState == Starting );
    debuggerInterface()->enableAction("debug_stepinto", m_executionState == Break || m_executionState == Starting );
    debuggerInterface()->enableAction("debug_stepout", m_executionState == Break || m_executionState == Starting);
    debuggerInterface()->enableAction("debug_stepover", m_executionState == Break || m_executionState == Starting);
  }

//   kdDebug(24002) << k_funcinfo << ", " << m_executionState << endl;

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

  QDomDocument data;
  data.setContent(datas);
  kdDebug(24002) << datas << endl;

  // Did we get a normal response?
  if(data.elementsByTagName("response").count() > 0)
  {
    QDomNode response = data.elementsByTagName("response").item(0);
    QString command = attribute(response, "command");

    if(command == "status")
      setExecutionState(attribute(response, "status"));
    else if(command == "stack_get")
    {
      showStack(response);
    }
    else if(command == "run" 
         || command == "step_over" 
         || command == "step_into" 
         || command == "step_out")
    {
      // If this is the acknoledge of a step command, request the call stack 
      m_network.sendCommand("stack_get");
      setExecutionState(attribute(response, "status"));
    }
    else if(command == "feature_get")
    {
      checkSupport(response);
    }
    else
    {
      kdDebug(24002) << " * Unknown command: " << command << endl;
    }
  }
  else if(data.elementsByTagName("init").count() > 0)
  {
    QDomNode init = data.elementsByTagName("init").item(0);
    initiateSession(init);
    return;
  }
  else
  {
    debuggerInterface()->showStatus(i18n("Unrecognized package: '%1%2'").arg(datas.left(50)).arg(datas.length() > 50 ? "..." : ""), true);

    kdDebug(24002) << datas << endl;
  }

}

void QuantaDebuggerDBGp::initiateSession(const QDomNode& initpacket)
{
  if(attribute(initpacket, "protocol_version") != protocolversion)
  {
    debuggerInterface()->showStatus(
      i18n("The debugger for %1 uses an unsupported protocol version (%1)")
            .arg(attribute(initpacket, "language"))
            .arg(attribute(initpacket, "protocol_version"))
          , true);

    endSession();
    return;
  }

  debuggerInterface()->setActiveLine(mapServerPathToLocal(attribute(initpacket, "fileuri")), 0);
  setExecutionState("starting");
//   m_network.sendCommand("feature_get", "-n encoding");
  m_network.sendCommand("feature_get", "-n supports_async");
}

void QuantaDebuggerDBGp::showStack(const QDomNode&node)
{

//  node.elementsByTagName("stack");
  for(QDomNode child = node.firstChild(); !child.isNull(); child = child.nextSibling())
  {
    if(attribute(child, "level") == "0")
    { 
       kdDebug(24002) << " * Stck 0: " << attribute(child, "filename") << ", " << attribute(child, "lineno") << endl;
      debuggerInterface()->setActiveLine(mapServerPathToLocal(attribute(child, "filename")), attribute(child, "lineno").toLong() -  1);
    }
  }
}

void QuantaDebuggerDBGp::checkSupport( const QDomNode & node )
{
  QString feature = attribute(node, "feature_name");
  QString data = node.nodeValue();
  if(feature == "supports_async")
    m_supportsasync = data.toLong();

}

QString QuantaDebuggerDBGp::attribute(const QDomNode&node, const QString &attribute)
{
  return node.attributes().namedItem(attribute).nodeValue();
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
//   for(QValueList<QString>::iterator it = m_watchlist.begin(); it != m_watchlist.end(); ++it)
//     sendCommand("getwatch", "variable", (*it).ascii(), 0);
//   sendCommand("sentwatches", "key", 0, 0);
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
  m_network.sendCommand("run");
//   m_network.sendCommand("status");
}

// Step into function
void QuantaDebuggerDBGp::stepInto()
{
  m_network.sendCommand("step_into");
}

// Step over function
void QuantaDebuggerDBGp::stepOver()
{
  // If we're in starting mode, we must step into, otherwise xdebug starts to run
  if(m_executionState == Starting)
    m_network.sendCommand("step_into");
  else
    m_network.sendCommand("step_over");
}

// Step out of function
void QuantaDebuggerDBGp::stepOut()
{
  m_network.sendCommand("step_out");
}


// Kill the running script
void QuantaDebuggerDBGp::kill()
{
  m_network.sendCommand("stop");
}

// Pause execution
void QuantaDebuggerDBGp::pause()
{
  m_network.sendCommand("break");
//   m_network.sendCommand("status");
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

void QuantaDebuggerDBGp::variableSetValue(const DebuggerVariable &)
{
//   sendCommand("setvariable", 
//               "variable", variable.name().ascii(),
//               "value", variable.value().ascii(),
//               0);
}

#include "quantadebuggerdbgp.moc"
