/***************************************************************************
                           debuggerbreakpoint.h
                           --------------------
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

#ifndef DEBUGGERBREAKPOINT_H
#define DEBUGGERBREAKPOINT_H
#include <qstring.h>

class DebuggerBreakpoint
{
  public:
    DebuggerBreakpoint();
    DebuggerBreakpoint(const QString& filePath, int line,
                       const QString& conditionExpr = "", int state = 0,
                       bool isTemp = false, int hitCount = 0, int skipHits = 0);

    virtual ~DebuggerBreakpoint();

    virtual void setFilePath(const QString& filePath);
    virtual void setLine(int line);
    virtual void setCondition(const QString& expression);
    virtual void setState(int state);
    virtual void setTemporary(bool temp);
    virtual void setHitCount(int hitCount);
    virtual void setSkipHits(int skipHits);

    virtual const QString& filePath();
    virtual int line();
    virtual const QString& condition();
    virtual int state();
    virtual bool isTemp();
    virtual int hitCount();
    virtual int skipHits();

  protected:
    QString m_filePath;
    int     m_line;
    QString m_conditionExpr;
    int     m_state;
    bool    m_isTemp;
    int     m_hitCount;
    int     m_skipHits;
};

#endif
