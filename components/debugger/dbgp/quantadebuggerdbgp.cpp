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
  setExecutionState(m_defaultExecutionState);
  emit updateStatus(DebuggerUI::NoSession);
  
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
  // debuggerInterface() might not be available, for example from project dialog
  if(!debuggerInterface())
    return;

  debuggerInterface()->enableAction("debug_request", active);
  debuggerInterface()->enableAction("debug_connect", !active);
  debuggerInterface()->enableAction("debug_disconnect", active);

  setExecutionState(m_defaultExecutionState);
  
  if(active)
    emit updateStatus(DebuggerUI::AwaitingConnection);
  else
    emit updateStatus(DebuggerUI::NoSession);

}

void QuantaDebuggerDBGp::slotNetworkConnected(bool connected)
{
  // debuggerInterface() might not be available, for example from project dialog
  if(!debuggerInterface())
    return;

  m_active = connected;

  debuggerInterface()->enableAction("debug_run", connected);
  debuggerInterface()->enableAction("debug_leap", connected);
  debuggerInterface()->enableAction("debug_pause", connected);
  debuggerInterface()->enableAction("debug_kill", connected);

  debuggerInterface()->enableAction("debug_stepinto", connected);
  debuggerInterface()->enableAction("debug_stepover", connected);
  debuggerInterface()->enableAction("debug_stepout", connected);

  debuggerInterface()->setActiveLine("", 0);
  if(connected)
    emit updateStatus(DebuggerUI::Connected);
  else
  {
    setExecutionState(m_defaultExecutionState);
    emit updateStatus(DebuggerUI::AwaitingConnection);
  }

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
//   setExecutionState(Starting);
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
void QuantaDebuggerDBGp::setExecutionState( const State & state, bool forcesend )
{
  if(m_executionState != state || forcesend)
  {
    if(state == Running)
      m_network.sendCommand("run");
    else if (state == Break)
      m_network.sendCommand("break");
  }
  m_executionState = state;

  if(debuggerInterface()) {
    // The run action will be active if we're started, stopped or paused
    debuggerInterface()->enableAction("debug_run", m_executionState == Break || m_executionState == Starting || m_executionState == Stopped);

    // The pause action will be enabled if we're running and either supports async or we're not connected (ie will start running)
    debuggerInterface()->enableAction("debug_pause", m_executionState == Running && (m_supportsasync || !isActive())) ;

    // Kill is active if we're paused, just started of the debugger supports async, as long as we have an active session
    debuggerInterface()->enableAction("debug_kill", isActive()  && (m_executionState == Break || (m_executionState == Running && m_supportsasync) || m_executionState == Starting ));

    // These are only activated when we have an active seesion and are paused
    debuggerInterface()->enableAction("debug_stepinto", isActive() && (m_executionState == Break || m_executionState == Starting ));
    debuggerInterface()->enableAction("debug_stepout", isActive() && (m_executionState == Break || m_executionState == Starting));
    debuggerInterface()->enableAction("debug_stepover", isActive() && (m_executionState == Break || m_executionState == Starting));
  }

}

// Change executionstate of the script
void QuantaDebuggerDBGp::setExecutionState(const QString &state)
{
  kdDebug(24002) << k_funcinfo << state << endl;
  
  if(state == "starting")
  {
    setExecutionState(Starting);
    emit updateStatus(DebuggerUI::Paused);
  }
  else if(state == "stopping")
  {
    setExecutionState(Stopping);
    emit updateStatus(DebuggerUI::Paused);
  }
  else if(state == "stopped")
  {
    setExecutionState(Stopped);
    emit updateStatus(DebuggerUI::Paused);
  }
  else if(state == "running")
  {
    setExecutionState(Running);
    emit updateStatus(DebuggerUI::Running);
  }
  else if(state == "break")
  {
    setExecutionState(Break);
    emit updateStatus(DebuggerUI::Paused);
  }

}

// Return capabilities of dbgp
const uint QuantaDebuggerDBGp::supports(DebuggerClientCapabilities::Capabilities cap)
{
  switch(cap)
  {
    case DebuggerClientCapabilities::LineBreakpoints:
//     case DebuggerClientCapabilities::ConditionalBreakpoints:
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

    // Status command
    if(command == "status")
      setExecutionState(attribute(response, "status"));

    // Callback stack
    else if(command == "stack_get")
      showStack(response);

    // Reply from a user execution action
    else if(command == "break" 
         || command == "step_over" 
         || command == "step_into" 
         || command == "step_out")
    {
      // If this is the acknoledge of a step command, request the call stack 
      m_network.sendCommand("stack_get");
      setExecutionState(attribute(response, "status"));
      sendWatches();
    }

    // Run
    else if(command == "run" )
      setExecutionState(attribute(response, "status"));

    // Feature get replu
    else if(command == "feature_get")
      checkSupport(response);

    // Reply after adding a breakpoint
    else if(command == "breakpoint_set")
      setBreakpointKey(response);

    // Unknown command...
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
//   setExecutionState(Starting);
//   m_network.sendCommand("feature_get", "-n encoding");
  m_network.sendCommand("feature_get", "-n supports_async");
  m_network.sendCommand("feature_get", "-n breakpoint_set");
  m_network.sendCommand("typemap_get");
  m_network.sendCommand("feature_get", "-n quanta_initialized");
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

  // if the debugger supports breakpoints, we have to send all current ones
  else if(feature == "breakpoint_set"/* && data.toLong()*/)
    debuggerInterface()->refreshBreakpoints();

  // Our own feature, probably not available but then we know we're done initiating
  else if(feature == "quanta_initialized" )
    if(m_executionState != Break)
      setExecutionState(m_executionState, true);

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


void QuantaDebuggerDBGp::sendWatches()
{
  for(QValueList<QString>::iterator it = m_watchlist.begin(); it != m_watchlist.end(); ++it)
    m_network.sendCommand("property_get", "-n " + (*it));
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
  setExecutionState(Running);
//   m_network.sendCommand("run");
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
  if(isActive())
    setExecutionState(Break);
  else
    setExecutionState(Starting);
//   m_network.sendCommand("break");
//   m_network.sendCommand("status");
}


// Add a breakpoint
void QuantaDebuggerDBGp::addBreakpoint (DebuggerBreakpoint* breakpoint)
{
  QString type;
  if(breakpoint->type() == DebuggerBreakpoint::LineBreakpoint)
    type = "line";
  else if(breakpoint->type() == DebuggerBreakpoint::ConditionalTrue)
    type = "conditional";
  else
    type = "watch";

  long id = m_network.sendCommand(
                        "breakpoint_set", 
                        "-t " + type +
                        " -f " + mapLocalPathToServer(breakpoint->filePath()) +
                        " -n " + QString::number(breakpoint->line() + 1)
                        , breakpoint->condition());

  breakpoint->setKey(QString("id %1").arg(id));
}

void QuantaDebuggerDBGp::setBreakpointKey( const QDomNode & response )
{
  long id;

  id = attribute(response, "transaction_id").toLong();
  if(id > 0)
  {
    QString oldkey = QString("id %1").arg(id);
    DebuggerBreakpoint *bp = debuggerInterface()->findDebuggerBreakpoint(oldkey);
    if(bp)
      debuggerInterface()->updateBreakpointKey(*bp, attribute(response, "id"));
  }
}


// Clear a breakpoint
void QuantaDebuggerDBGp::removeBreakpoint(DebuggerBreakpoint* bp)
{
  m_network.sendCommand("breakpoint_remove", "-d " + bp->key());
}

// A file was opened...
void QuantaDebuggerDBGp::fileOpened(const QString&)
{
//   sendCommand("reinitialize", 0);
}

// Watch a variable
void QuantaDebuggerDBGp::addWatch(const QString & variable)
{
  if(m_watchlist.find(variable) == m_watchlist.end())
    m_watchlist.append(variable);
  m_network.sendCommand("property_get", "-n " + variable);
}
// Remove watch
void QuantaDebuggerDBGp::removeWatch(DebuggerVariable *variable)
{
  if(m_watchlist.find(variable->name()) != m_watchlist.end())
    m_watchlist.remove(m_watchlist.find(variable->name()));
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
  {
    if(valuenode.firstChild().nodeValue() == "break")
      m_defaultExecutionState = Starting;
    else
      m_defaultExecutionState = Running;
  }

  valuenode = node.namedItem("useproxy");
  m_useproxy = valuenode.firstChild().nodeValue() == "1";

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
  set.lineStartSession->setText(m_startsession);
  if(m_defaultExecutionState == Starting)
    set.comboDefaultExecutionState->setCurrentItem(0);  
  else
    set.comboDefaultExecutionState->setCurrentItem(1);

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
    if(set.comboDefaultExecutionState->currentItem() == 0)
    {
      m_defaultExecutionState = Starting;
      el.appendChild(node.ownerDocument().createTextNode("break"));
    }
    else
    {
      m_defaultExecutionState = Running;
      el.appendChild(node.ownerDocument().createTextNode("run"));

    }

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
  return;
}



#include "quantadebuggerdbgp.moc"
