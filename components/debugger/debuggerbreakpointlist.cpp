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
  quantaApp->debugger()->UI()->showBreakpoint(*bp);
  m_breakpointList->push_front(bp);
}

void DebuggerBreakpointList::remove(DebuggerBreakpoint* bp)
{
  BreakpointList_t::iterator it;
  BreakpointList_t::iterator end = m_breakpointList->end();

  DebuggerBreakpoint* tmp;

  for(it = m_breakpointList->begin(); it != end; ++it)
  {
    if((bp->filePath() == (*it)->filePath()) &&
        (bp->line()     == (*it)->line() || bp->type() != DebuggerBreakpoint::LineBreakpoint) &&
        (bp->type()     == (*it)->type()) &&
        (bp->inClass()     == (*it)->inClass()) &&
        (bp->inFunction()     == (*it)->inFunction()) &&
        (bp->condition() == (*it)->condition()))
    {
      tmp = (*it);
      // Remove it from the bp-list
      quantaApp->debugger()->UI()->deleteBreakpoint(*bp);

      // Remove editor markpoint if there is one...
      if(bp->type() == DebuggerBreakpoint::LineBreakpoint)
        quantaApp->debugger()->setMark(bp->filePath(), bp->line(), false, KTextEditor::MarkInterface::markType02);

      it = m_breakpointList->remove(it);
      delete tmp;
      break;
    }
  }
}

/*int DebuggerBreakpointList::remove(QString filePath, int line)
{
  int count = 0;
  BreakpointList_t::iterator it;
  BreakpointList_t::iterator end = m_breakpointList->end();

  DebuggerBreakpoint* tmp;

  for(it = m_breakpointList->begin(); it != end; ++it)
  {
    if((filePath == (*it)->filePath()) &&
       (line     == (*it)->line()))
    {
      tmp = (*it);
      it = m_breakpointList->remove(it);
      delete tmp;
      count++;
    }
  }

  return count;
}*/

DebuggerBreakpoint* DebuggerBreakpointList::retrieve(const QString& filePath, int line)
{
  BreakpointList_t::iterator it;
  BreakpointList_t::iterator end = m_breakpointList->end();

  for(it = m_breakpointList->begin(); it != end; ++it)
  {
    if((filePath   == (*it)->filePath()) &&
        line       == (*it)->line())
    {
      return (*it);
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
  BreakpointList_t::iterator it;
  BreakpointList_t::iterator end = m_breakpointList->end();

  for(it = m_breakpointList->begin(); it != end; ++it)
  {
    if((bp->filePath()  == (*it)->filePath()) &&
        (bp->line()     == (*it)->line() || bp->type() != DebuggerBreakpoint::LineBreakpoint) &&
        (bp->type()     == (*it)->type()) &&
        (bp->inClass()  == (*it)->inClass()) &&
        (bp->inFunction() == (*it)->inFunction()) &&
        (bp->condition() == (*it)->condition()))
    {
      return true;
    }
  }
  return false;
}

/*bool DebuggerBreakpointList::exists(QString filePath, int line)
{
  BreakpointList_t::iterator it;
  BreakpointList_t::iterator end = m_breakpointList->end();

  for(it = m_breakpointList->begin(); it != end; ++it)
  {
    if((filePath == (*it)->filePath()) &&
       (line     == (*it)->line()))
    {
      return true;
    }
  }
  return false;
}*/

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
