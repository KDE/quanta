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
    : m_line(1), m_state(0), m_isTemp(false), m_hitCount(0), m_skipHits(0)
{}

DebuggerBreakpoint::DebuggerBreakpoint(const QString& filePath
                                       , int line, const QString& conditionExpr
                                       , int state, bool isTemp, int hitCount, int skipHits)
{
  m_filePath      = filePath;
  m_line          = line;
  m_conditionExpr = conditionExpr;
  m_state         = state;
  m_isTemp        = isTemp;
  m_hitCount      = hitCount;
  m_skipHits      = skipHits;
}

DebuggerBreakpoint::~DebuggerBreakpoint()
{}

void DebuggerBreakpoint::setFilePath(const QString& filePath)
{
  m_filePath = filePath;
}

void DebuggerBreakpoint::setLine(int line)
{
  m_line = line;
}

void DebuggerBreakpoint::setCondition(const QString& expression)
{
  m_conditionExpr = expression;
}

void DebuggerBreakpoint::setState(int state)
{
  m_state = state;
}

void DebuggerBreakpoint::setTemporary(bool temp)
{
  m_isTemp = temp;
}

void DebuggerBreakpoint::setHitCount(int hitCount)
{
  m_hitCount = hitCount;
}

void DebuggerBreakpoint::setSkipHits(int skipHits)
{
  m_skipHits = skipHits;
}

const QString& DebuggerBreakpoint::filePath() const
{
  return m_filePath;
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

bool DebuggerBreakpoint::isTemp() const
{
  return m_isTemp;
}

int DebuggerBreakpoint::hitCount() const
{
  return m_hitCount;
}

int DebuggerBreakpoint::skipHits() const
{
  return m_skipHits;
}
