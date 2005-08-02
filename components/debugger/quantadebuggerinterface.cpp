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

void QuantaDebuggerInterface::haveBreakpoint (const QString& file, int line)
{
  return m_manager->haveBreakpoint(file, line);
}

void QuantaDebuggerInterface::havenoBreakpoint (const QString& file, int line)
{
  return m_manager->havenoBreakpoint(file, line);
}

// Public help functions
bool QuantaDebuggerInterface::showStatus(const QString& message, bool log)
{
  return m_manager->showStatus(message, log);
}

bool QuantaDebuggerInterface::setActiveLine(const QString& file, int line)
{
  return m_manager->setActiveLine(file, line);
}

void QuantaDebuggerInterface::enableAction(const QString& action, bool enable)
{
  m_manager->enableAction(action, enable);
}

void QuantaDebuggerInterface::fileOpened(const QString& file)
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

  kdDebug(24002) << k_funcinfo << ", BaseURL " << Project::ref()->projectBaseURL().path() << ", active doc : " << ViewManager::ref()->activeDocument()->url().path() << ", documentFolderForURL" << Project::ref()->documentFolderForURL(ViewManager::ref()->activeDocument()->url()) << ", newstr" << newstr << endl;

  return newstr;
}

void QuantaDebuggerInterface::showVariable(DebuggerVariable* var)
{
  m_manager->UI()->addVariable(var);
}

DebuggerVariable *QuantaDebuggerInterface::newDebuggerVariable(const QString& name, const QString& value, int type)
{
  return new DebuggerVariable(name, value, type);
}

DebuggerVariable *QuantaDebuggerInterface::newDebuggerVariable(const QString& name, const ValueList_t& values, int type)
{
  return new DebuggerVariable(name, values, type);
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
  return new DebuggerBreakpoint();
}

DebuggerBreakpoint *QuantaDebuggerInterface::findDebuggerBreakpoint(const QString& key)
{
  return m_manager->findDebuggerBreakpoint(key);
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

void QuantaDebuggerInterface::updateBreakpointKey( const DebuggerBreakpoint & bp, const QString & newkey )
{
  m_manager->updateBreakpointKey(bp, newkey);
}

void QuantaDebuggerInterface::backtraceClear( )
{
  m_manager->UI()->backtraceClear();
}

void QuantaDebuggerInterface::backtraceShow( long level, BacktraceType type, const QString & filename, long line, const QString & func )
{
  m_manager->UI()->backtraceShow(level, type, filename, line, func);
}

#include "quantadebuggerinterface.moc"
