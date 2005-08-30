/***************************************************************************
                        debuggerbreakpointlist.cpp
                        --------------------------
    begin                : 2004-04-04
    copyright            : (C) 2004 Thiago Silva

 ***************************************************************************/

/****************************************************************************
 *                                                                          *
 *   This program is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation; either version 2 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 ***************************************************************************/

#include <qobject.h>
#include <ktexteditor/markinterfaceextension.h>
#include <kdebug.h>

#include "debuggerbreakpointlist.h"
#include "debuggerbreakpoint.h"
#include "debuggerclient.h"
#include "debuggermanager.h"
#include "debuggerui.h"
#include "resource.h"
#include "quanta.h"

DebuggerBreakpointList::DebuggerBreakpointList()
    : m_current(0)
{
  m_breakpointList = new BreakpointList_t();
}

DebuggerBreakpointList::~DebuggerBreakpointList()
{
  delete m_breakpointList;
}

void DebuggerBreakpointList::add(DebuggerBreakpoint* bp)
{
  if(quantaApp->debugger()->UI())
    quantaApp->debugger()->UI()->showBreakpoint(*bp);
  
  m_breakpointList->push_front(bp);
  if(bp->type() == DebuggerBreakpoint::LineBreakpoint)
    quantaApp->debugger()->setMark(bp->filePath(), bp->line(), true, KTextEditor::MarkInterface::markType02);
}

void DebuggerBreakpointList::remove(DebuggerBreakpoint* bp)
{
  BreakpointList_t::iterator it = find(*bp);
  if(it == m_breakpointList->end())
    return;

//   DebuggerBreakpoint bp1(bp);

  if(*bp == (*bp))
  {
    DebuggerBreakpoint* tmp;

    tmp = (*it);
    // Remove it from the bp-list
    if(quantaApp->debugger()->UI())
      quantaApp->debugger()->UI()->deleteBreakpoint(*bp);

    // Remove editor markpoint if there is one...
    if(bp->type() == DebuggerBreakpoint::LineBreakpoint)
      quantaApp->debugger()->setMark(bp->filePath(), bp->line(), false, KTextEditor::MarkInterface::markType02);

    it = m_breakpointList->remove(it);
    delete tmp;
  }
}



DebuggerBreakpoint* DebuggerBreakpointList::retrieve(const QString& filePath, int line)
{
  BreakpointList_t::iterator it;

  for(it = m_breakpointList->begin(); it != m_breakpointList->end(); ++it)
  {
    if((filePath   == (*it)->filePath()) &&
        line       == (*it)->line())
    {
      DebuggerBreakpoint* bp = new DebuggerBreakpoint(*it);
      return bp;
    }
  }
  return 0;
}

void DebuggerBreakpointList::clear()
{
  BreakpointList_t::iterator it;

  for(it = m_breakpointList->begin(); it != m_breakpointList->end(); ++it)
  {

    // Remove it from the bp-list
    quantaApp->debugger()->UI()->deleteBreakpoint(*(*it));

    // Remove editor markpoint if there is one...
    quantaApp->debugger()->setMark((*it)->filePath(), (*it)->line(), false, KTextEditor::MarkInterface::markType02);

    if(quantaApp->debugger()->client())
      quantaApp->debugger()->client()->removeBreakpoint((*it));
  }
  m_breakpointList->clear();
}

bool DebuggerBreakpointList::exists(DebuggerBreakpoint* bp)
{
  BreakpointList_t::iterator it = find(*bp);
  if(it == m_breakpointList->end())
    return false;

  if(*bp == (*it))
    return true;

  return false;
}


BreakpointList_t::iterator DebuggerBreakpointList::find(const DebuggerBreakpoint &bp)
{
  BreakpointList_t::iterator it;

  for(it = m_breakpointList->begin(); it != m_breakpointList->end(); ++it)
  {
    if(bp == (*it))
      return it;
  }
  return m_breakpointList->end();
}

void DebuggerBreakpointList::rewind()
{
  m_current = 0;
}

DebuggerBreakpoint* DebuggerBreakpointList::next()
{
  if(m_current == 0)
  {
    m_current = m_breakpointList->begin();
  }
  else
  {
    ++m_current;
  }

  if(m_current != m_breakpointList->end())
  {
    return (*m_current);
  }
  else
  {
    return NULL;
  }
}

DebuggerBreakpoint * DebuggerBreakpointList::findDebuggerBreakpoint( const QString & key )
{
  BreakpointList_t::iterator it;
  for(it = m_breakpointList->begin(); it != m_breakpointList->end(); ++it)
  {
    if((*it)->key() == key)
    {
      DebuggerBreakpoint *bp = new DebuggerBreakpoint((*it));
      return bp;
    }
  }
  return NULL;
}

void DebuggerBreakpointList::updateBreakpointKey( const DebuggerBreakpoint & bp, const QString & newkey )
{
  //DebuggerBreakpoint *bpp = new DebuggerBreakpoint(bp);
  BreakpointList_t::iterator it;
  it = find(bp);
  if(it != m_breakpointList->end())
  {
    (*it)->setKey(newkey);
  }
}

size_t DebuggerBreakpointList::count( )
{
  return m_breakpointList->count();
}
