/***************************************************************************
                           debuggervariable.cpp
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

#include "debuggervariable.h"
#include <kdebug.h>
#include <klocale.h>

DebuggerVariable::DebuggerVariable()
    : m_isReference(false)
    , m_size(0)
    , m_type(DebuggerVariableTypes::Undefined)
    , m_item(NULL)
{}

DebuggerVariable::DebuggerVariable(const QString& name)
    : m_isReference(false)
    , m_size(0)
    , m_type(DebuggerVariableTypes::Undefined)
    , m_item(NULL)
{
  m_name = name;
}

DebuggerVariable::DebuggerVariable(const QString& name, const QString& value, int type)
    : m_isReference(false)
    , m_size(0)
    , m_item(NULL)
{
  m_name  = name;
  m_value = value;
  m_type  = type;
  if(type == DebuggerVariableTypes::String)
    m_size = value.length();
}

DebuggerVariable::DebuggerVariable(const QString& name, const QString& value, int type, int size)
    : m_isReference(false)
    , m_item(NULL)
{
  m_name  = name;
  m_value = value;
  m_type  = type;
  m_size = size;
}

DebuggerVariable::DebuggerVariable(const QString& name, const ValueList_t& values, int type)
    : m_isReference(false)
    , m_item(NULL)
{
  m_name = name;
  m_valueList = values;
  m_type = type;
  m_size = values.count();
}

DebuggerVariable::DebuggerVariable(DebuggerVariable* v, bool copyitem )
{
  m_name = v->name();
  m_size = v->size();
  m_value = v->value();
  m_type = v->type();
  m_isReference = v->isReference();
  if(copyitem)
  {
    m_item = v->item();
    v->setItem(NULL);
  }
  else
    m_item = NULL;

  // We cant just assign m_valuelist to v->values(), it would make a shallow copy...
  for(DebuggerVariable * v2 = v->values().first(); v2; v2 = v->values().next())
    m_valueList.append(new DebuggerVariable(v2, copyitem));
}


void DebuggerVariable::setName(const QString& name)
{
  m_name = name;
}
QString DebuggerVariable::name() const
{
  return m_name;
}
void DebuggerVariable::setValue(const QString& value)
{
  m_value = value;
}

QString DebuggerVariable::value() const
{
  if(isScalar())
    return m_value;
  else
    return i18n("Non scalar value");
}

bool DebuggerVariable::isScalar() const
{
  switch(m_type)
  {
    case DebuggerVariableTypes::Reference:
    case DebuggerVariableTypes::Resource:
    case DebuggerVariableTypes::String:
    case DebuggerVariableTypes::Integer:
    case DebuggerVariableTypes::Float:
    case DebuggerVariableTypes::Boolean:
    case DebuggerVariableTypes::Undefined:
    case DebuggerVariableTypes::Error:
      return true;
  }
  return false;
}

void DebuggerVariable::setValues(const ValueList_t& valueList)
{
  m_valueList = valueList;
}

ValueList_t DebuggerVariable::values() const
{
  return m_valueList;
}

void DebuggerVariable::setType(int type)
{
  m_type = type;
}

int DebuggerVariable::type() const
{
  return m_type;
}

const QString DebuggerVariable::typeName() const
{
  switch(m_type)
  {
    case DebuggerVariableTypes::Array:
      return i18n("Array");
    case DebuggerVariableTypes::Object:
      return i18n("Object");
    case DebuggerVariableTypes::Reference:
      return i18n("Reference");
    case DebuggerVariableTypes::Resource:
      return i18n("Resource");
    case DebuggerVariableTypes::String:
      return i18n("String");
    case DebuggerVariableTypes::Integer:
      return i18n("Integer");
    case DebuggerVariableTypes::Float:
      return i18n("Float");
    case DebuggerVariableTypes::Boolean:
      return i18n("Boolean");
    case DebuggerVariableTypes::Undefined:
      return i18n("Undefined");
    case DebuggerVariableTypes::Error:
      return i18n("Error");
    default:
      return i18n("Unknown");
  }
}


void DebuggerVariable::setSize(long size)
{
  m_size = size;
}

long DebuggerVariable::size() const
{
  return m_size;
}

QString DebuggerVariable::sizeName() const
{
  switch(m_type)
  {
    case DebuggerVariableTypes::Reference:
    case DebuggerVariableTypes::Resource:
    case DebuggerVariableTypes::Integer:
    case DebuggerVariableTypes::Float:
    case DebuggerVariableTypes::Boolean:
    case DebuggerVariableTypes::Undefined:
    case DebuggerVariableTypes::Error:
      return "";
  }
  return QString::number(m_size);
}

void DebuggerVariable::setReference(bool ref)
{
  m_isReference = ref;
}
bool DebuggerVariable::isReference() const
{
  return m_isReference;
}


DebuggerVariable::~DebuggerVariable()
{
  DebuggerVariable * v;
  while((v = m_valueList.first()))
  {
    m_valueList.remove(v);
    delete v;
  }
  // If this variable is shown in the treeview, remove it
  if(m_item)
    delete m_item;

}


void DebuggerVariable::deleteChild( DebuggerVariable * child )
{
  
  for(DebuggerVariable *v = m_valueList.first(); v; v = m_valueList.next())
  {
    if(v->name() == child->name())
    {
      m_valueList.remove(v);
      delete v;
      return;
    }
  }
}

DebuggerVariable* DebuggerVariable::findItem( QListViewItem * item, bool traverse )
{
  if(item == m_item)
    return this;
  
  if(!traverse)
    return NULL;
  
  for(DebuggerVariable * v = m_valueList.first(); v; v = m_valueList.next())
  {
    DebuggerVariable * v2 = v->findItem(item, true);
    if(v2)
      return v2;
  }
  return NULL;
}

void DebuggerVariable::copy( DebuggerVariable * v, bool copychildren )
{
  m_name = v->name();
  m_size = (v->isScalar() || copychildren ? v->size() : m_valueList.count());
  m_value = v->value();
  m_type = v->type();
  m_isReference = v->isReference();

  // We cant just assign m_valuelist to v->values(), it would make a shallow copy...
//   
  if(copychildren)
  {
    m_valueList.clear();
    for(DebuggerVariable * v2 = v->values().first(); v2; v2 = v->values().next())
      m_valueList.append(new DebuggerVariable(v2, true));
  }
}

void DebuggerVariable::append( DebuggerVariable * v )
{
  m_valueList.append(v);
}

