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
    enum Types
    {
      LineBreakpoint = 0,
      ConditionalTrue,
      ConditionalChange
    };
  
    enum States
    {
      Undefined = 0,
      Unfulfilled,
      Fulfilled,
      Error
    };
    
    DebuggerBreakpoint();
    DebuggerBreakpoint(const DebuggerBreakpoint& bp);
    DebuggerBreakpoint(const DebuggerBreakpoint* bp);
    DebuggerBreakpoint(const QString& filePath, int line); // Line BP
    DebuggerBreakpoint(const DebuggerBreakpoint::Types type,                   // Any kind
                       const QString& conditionExpr, const QString& filePath = "", 
                       const QString& inClass = "", const QString& inFunction = "");

    virtual ~DebuggerBreakpoint();

    virtual void setFunction(const QString& filePath);
    virtual void setClass(const QString& filePath);
    virtual void setFilePath(const QString& filePath);
    virtual void setLine(int line);
    virtual void setCondition(const QString& expression);
    virtual void setState(int state);
    virtual void setType(Types type);
    virtual void setValue(const QString& value);
    virtual void setKey(const QString& value);

    virtual const QString& filePath() const;
    virtual const QString& inClass() const;
    virtual const QString& inFunction() const;
    virtual int line() const;
    virtual const QString& condition() const;
    virtual int state() const;
    virtual DebuggerBreakpoint::Types type() const;
    virtual const QString& value() const;
    virtual const QString& key() const;

    bool operator == (DebuggerBreakpoint) const;

  protected:
    QString m_filePath;
    QString m_class;
    QString m_function;
    int     m_line;
    QString m_conditionExpr;
    int     m_state;
    Types   m_type;
    QString m_value;
    QString m_key;
};

#endif
