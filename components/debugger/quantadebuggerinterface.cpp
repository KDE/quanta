/***************************************************************************
                              debugmanager.cpp
                             ------------------
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

#include <ktexteditor/document.h>
#include <kdebug.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kmessagebox.h>

#include "debuggerinterface.h"
#include "quantadebuggerinterface.h"
#include "debuggermanager.h"
#include "debuggerui.h"
#include "pathmapper.h"
#include "project.h"
#include "viewmanager.h"
#include "document.h"

class DebuggerBreakpoint;

QuantaDebuggerInterface::QuantaDebuggerInterface (QObject *myparent, const char* name)
    : DebuggerInterface(myparent, name)
{
  m_manager = static_cast<DebuggerManager*>(parent());
}

QuantaDebuggerInterface::~QuantaDebuggerInterface ()
{}

void QuantaDebuggerInterface::haveBreakpoint (QString file, int line)
{
  return m_manager->haveBreakpoint(file, line);
}

// Public help functions
bool QuantaDebuggerInterface::showStatus(QString message, bool log)
{
  return m_manager->showStatus(message, log);
}

bool QuantaDebuggerInterface::setActiveLine(QString file, int line)
{
  return m_manager->setActiveLine(file, line);
}

void QuantaDebuggerInterface::enableAction(QString action, bool enable)
{
  m_manager->enableAction(action, enable);
}

void QuantaDebuggerInterface::fileOpened(QString file)
{
  m_manager->fileOpened(file);
}

void QuantaDebuggerInterface::sendRequest(const KURL &url)
{
  m_manager->UI()->sendRequest(url);
}

const QString QuantaDebuggerInterface::activeFileParts(const QString & str)
{
  QString newstr = str;

  // a/r = absolute/relative
  // f/p/d = file/project/docroot
  // n/d/p = name/dir/path

  // Filename, filedir and filepath
  newstr.replace("%afn", ViewManager::ref()->activeDocument()->url().fileName());
  newstr.replace("%afd", ViewManager::ref()->activeDocument()->url().directory());
  newstr.replace("%afp", ViewManager::ref()->activeDocument()->url().path());

  // filedir and filepath relative to project root
  newstr.replace("%rfpp", KURL::relativePath(Project::ref()->projectBaseURL().path(), ViewManager::ref()->activeDocument()->url().path()));
  newstr.replace("%rfpd", KURL::relativePath(Project::ref()->projectBaseURL().path(), ViewManager::ref()->activeDocument()->url().directory()));

  // filedir and filepath relative to document root
  newstr.replace("%rfdp", KURL::relativePath(Project::ref()->documentFolderForURL(ViewManager::ref()->activeDocument()->url()).directory(), ViewManager::ref()->activeDocument()->url().path()));
  newstr.replace("%rfdd", KURL::relativePath(Project::ref()->documentFolderForURL(ViewManager::ref()->activeDocument()->url()).directory(), ViewManager::ref()->activeDocument()->url().directory()));

  newstr.replace("%apd", Project::ref()->projectBaseURL().path());
  newstr.replace("%add", Project::ref()->documentFolderForURL(ViewManager::ref()->activeDocument()->url()).directory());

  kdDebug(24000) << k_funcinfo << ", BaseURL " << Project::ref()->projectBaseURL().path() << ", active doc : " << ViewManager::ref()->activeDocument()->url().path() << ", documentFolderForURL" << Project::ref()->documentFolderForURL(ViewManager::ref()->activeDocument()->url()) << ", newstr" << newstr << endl;

  return newstr;
}

/*DebuggerVariable* QuantaDebuggerInterface::newDebuggerVariable(const QString& name, const QString& value, int type)
{
  return new DebuggerVariable(name, value, type);
}*/

/*void QuantaDebuggerInterface::addVariable(DebuggerVariable* var)
{
  m_manager->UI()->addVariable(var);
}*/

void QuantaDebuggerInterface::parsePHPVariables(const QString &varstring)
{
  m_manager->UI()->parsePHPVariables(varstring);
}

void QuantaDebuggerInterface::showBreakpoint(const DebuggerBreakpoint &bp)
{
  m_manager->UI()->showBreakpoint(bp);
}

void QuantaDebuggerInterface::refreshBreakpoints()
{
  m_manager->refreshBreakpoints();
}

DebuggerBreakpoint *QuantaDebuggerInterface::newDebuggerBreakpoint()
{
  return m_manager->newDebuggerBreakpoint();
}

/*
void QuantaDebuggerInterface::preWatchUpdate()
{
  m_manager->UI()->preWatchUpdate();
}

void QuantaDebuggerInterface::postWatchUpdate()
{
  m_manager->UI()->postWatchUpdate();
}*/

// Path mapping
PathMapper* QuantaDebuggerInterface::Mapper()
{
  return m_manager->Mapper();
}

#include "quantadebuggerinterface.moc"
