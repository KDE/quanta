/***************************************************************************
                            debuggervariable.h
                         ------------------------
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


#ifndef DEBUGGERVARIABLE_H
#define DEBUGGERVARIABLE_H

#include <qstring.h>
#include <qptrlist.h>
#include <klistview.h>

namespace DebuggerVariableTypes
{

  enum VarType {
    Object,
    Resource,
    Reference,
    Array,
    String,
    Integer,
    Float,
    Boolean,
    Error,
    Undefined
  };
}

class DebuggerVariable;
typedef QPtrList<DebuggerVariable> ValueList_t;

class DebuggerVariable
{
  public:
    DebuggerVariable();
    DebuggerVariable(const QString& name);
    DebuggerVariable(const QString& name, const QString& value, int type);
    DebuggerVariable(const QString& name, const QString& value, int type, int size);
    DebuggerVariable(const QString& name, const ValueList_t& values, int type);
    virtual ~DebuggerVariable();

    virtual void setName(const QString& name);
    virtual QString name();

    virtual void setValue(const QString& value);
    virtual QString value();

    virtual void setValues(const ValueList_t& valueList);
    virtual ValueList_t values();

    virtual void setType(int type);
    virtual int type();
    virtual const QString typeName();
    virtual bool isScalar();

    virtual void setSize(long size);
    virtual long size();
    virtual QString sizeName();

    virtual void setReference(bool ref);
    virtual bool isReference();

    virtual void touch() { m_touched = true;};
    virtual bool touched() { return m_touched;};

    virtual void setItem(KListViewItem* item) { m_item = item;};
    virtual KListViewItem*  item() { return m_item;};

  private:
    ValueList_t m_valueList;

    QString m_name;
    QString m_value;
    bool    m_isReference;
    long    m_size;
    int     m_type;
    long    m_touched;

    KListViewItem* m_item;
};
#endif
