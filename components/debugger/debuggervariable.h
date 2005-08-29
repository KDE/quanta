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
    DebuggerVariable(DebuggerVariable* var, bool copyitem = false);
    DebuggerVariable(const QString& name);
    DebuggerVariable(const QString& name, const QString& value, int type);
    DebuggerVariable(const QString& name, const QString& value, int type, int size);
    DebuggerVariable(const QString& name, const ValueList_t& values, int type);
    virtual ~DebuggerVariable();

    DebuggerVariable* findItem(QListViewItem *item, bool traverse = false);
    
    virtual void setName(const QString& name);
    virtual QString name() const;

    virtual void setValue(const QString& value);
    virtual QString value() const;

    virtual void setValues(const ValueList_t& valueList);
    virtual ValueList_t values() const;

    virtual void setType(int type);
    virtual int type() const;
    virtual const QString typeName() const ;
    virtual bool isScalar() const;

    virtual void setSize(long size);
    virtual long size() const;
    virtual QString sizeName() const;

    virtual void setReference(bool ref);
    virtual bool isReference() const;

    virtual void touch() { m_touched = true;};
    virtual bool touched()  const { return m_touched;};

    virtual void setItem(KListViewItem* item) { m_item = item;};
    virtual KListViewItem*  item() const{  return m_item;};

    virtual void copy(DebuggerVariable* v, bool copychldren = true);
    virtual void append(DebuggerVariable* v);
    virtual void deleteChild(DebuggerVariable *child);
    
    
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
