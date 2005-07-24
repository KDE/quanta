/***************************************************************************
                          debuggerbreakpoint.cpp
                          ----------------------
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

#include "debuggerbreakpoint.h"
#include <kdebug.h>

DebuggerBreakpoint::DebuggerBreakpoint()
    : m_line(1)//, m_state(0)
{}

DebuggerBreakpoint::DebuggerBreakpoint( const DebuggerBreakpoint & bp )
{
  m_conditionExpr = bp.condition();
  m_filePath = bp.filePath();
  m_class = bp.inClass();
  m_function = bp.inFunction();
  m_line     = bp.line();
  m_state    = bp.state();
  m_key      = bp.key();
  m_type     = bp.type();
}

DebuggerBreakpoint::DebuggerBreakpoint( const DebuggerBreakpoint * bp )
{
  m_conditionExpr = bp->condition();
  m_filePath = bp->filePath();
  m_class = bp->inClass();
  m_function = bp->inFunction();
  m_line     = bp->line();
  m_state    = bp->state();
  m_key      = bp->key();
  m_type     = bp->type();
}


DebuggerBreakpoint::DebuggerBreakpoint(const QString& filePath, int line)
{
  m_filePath = filePath;
  m_line     = line;
  m_type     = DebuggerBreakpoint::LineBreakpoint;
  m_state    = DebuggerBreakpoint::Undefined;
}

DebuggerBreakpoint::DebuggerBreakpoint(const Types type,
                                       const QString& conditionExpr, const QString& filePath, 
                                       const QString& inClass, const QString& inFunction)
{
  m_conditionExpr = conditionExpr;
  m_filePath = filePath;
  m_class = inClass;
  m_function = inFunction;
  m_line     = 0;
  m_type     = type;
  m_state    = DebuggerBreakpoint::Undefined;
}

DebuggerBreakpoint::~DebuggerBreakpoint()
{}

void DebuggerBreakpoint::setFilePath(const QString& filePath)
{
  m_filePath = filePath;
}

void DebuggerBreakpoint::setClass(const QString& newclass)
{
  m_class = newclass;
}

void DebuggerBreakpoint::setFunction(const QString& function)
{
  m_function = function;
}

void DebuggerBreakpoint::setLine(int line)
{
  m_line = line;
}

void DebuggerBreakpoint::setCondition(const QString& expression)
{
  m_conditionExpr = expression;
}

void DebuggerBreakpoint::setValue(const QString& value)
{
  m_value = value;
}

void DebuggerBreakpoint::setState(int state)
{
  m_state = state;
}

void DebuggerBreakpoint::setType(DebuggerBreakpoint::Types type )
{
  m_type = type;
}

void DebuggerBreakpoint::setKey(const QString& value)
{
  m_key = value;
}

const QString& DebuggerBreakpoint::key() const
{
  return m_key;
}

const QString& DebuggerBreakpoint::filePath() const
{
  return m_filePath;
}

const QString& DebuggerBreakpoint::value() const
{
  return m_value;
}

const QString& DebuggerBreakpoint::inClass() const
{
  return m_class;
}
const QString& DebuggerBreakpoint::inFunction() const
{
  return m_function;
}

DebuggerBreakpoint::Types DebuggerBreakpoint::type() const
{
  return m_type;
}

int DebuggerBreakpoint::line() const
{
  return m_line;
}

const QString& DebuggerBreakpoint::condition() const
{
  return m_conditionExpr;
}

int DebuggerBreakpoint::state() const
{
  return m_state;
}

bool DebuggerBreakpoint::operator == (DebuggerBreakpoint bp) const
{
  // If they key matches
  if(!m_key.isEmpty() && bp.key() == m_key)
    return true;

  // Or everything else...
  if(bp.filePath() == m_filePath
  && (bp.line() == m_line || m_type != DebuggerBreakpoint::LineBreakpoint)
  && bp.type() == m_type
  && bp.inClass() == m_class
  && bp.inFunction() == m_function
  && bp.condition() == m_conditionExpr
  )
    return true;
  return false;
  
}

