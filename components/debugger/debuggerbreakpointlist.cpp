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
#include <kdebug.h>
#include "debuggerbreakpointlist.h"
#include "debuggerbreakpoint.h"

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
  m_breakpointList->push_front(bp);
}

int DebuggerBreakpointList::remove(DebuggerBreakpoint* bp)
{
  int count = 0;
  BreakpointList_t::iterator it;
  BreakpointList_t::iterator end = m_breakpointList->end();
  
  DebuggerBreakpoint* tmp;
  
  for(it = m_breakpointList->begin(); it != end; ++it)
  {
    if((bp->filePath() == (*it)->filePath()) &&
       (bp->line()     == (*it)->line()))
    {
      tmp = (*it);
      it = m_breakpointList->remove(it);
      delete tmp;
      count++;
    }
  }
  
  return count;
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

void DebuggerBreakpointList::clear()
{
  m_breakpointList->clear();
}

bool DebuggerBreakpointList::exists(DebuggerBreakpoint* bp)
{
  BreakpointList_t::iterator it;
  BreakpointList_t::iterator end = m_breakpointList->end();
  
  for(it = m_breakpointList->begin(); it != end; ++it)
  {
    if((bp->filePath() == (*it)->filePath()) &&
       (bp->line()     == (*it)->line()))
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
  } else 
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
