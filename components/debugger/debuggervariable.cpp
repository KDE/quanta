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

DebuggerVariable::DebuggerVariable()
  : m_type(DebuggerVariableTypes::Scalar), m_isReference(false), m_item(NULL)
{
}

DebuggerVariable::DebuggerVariable(const QString& name)  
  : m_type(DebuggerVariableTypes::Scalar), m_isReference(false), m_item(NULL)
{
  m_name = name;
}

DebuggerVariable::DebuggerVariable(const QString& name, const QString& value, int type)
  : m_isReference(false), m_item(NULL)
{
  m_name  = name;
  m_value = value;
  m_type  = type;
}

DebuggerVariable::DebuggerVariable(const QString& name, const ValueList_t& values, int type)
  : m_isReference(false), m_item(NULL)
{
  m_name = name;
  m_valueList = values;
  m_type = type;
}

void DebuggerVariable::setName(const QString& name)
{
  m_name = name;
}
QString DebuggerVariable::name()
{
  return m_name;
}
void DebuggerVariable::setValue(const QString& value)
{
  m_value = value;
}
QString DebuggerVariable::value()
{
  switch(m_type)
  {
    case DebuggerVariableTypes::Scalar:    return m_value;      
    case DebuggerVariableTypes::Object:    return "object";
    case DebuggerVariableTypes::Resource:  return "resource";
    case DebuggerVariableTypes::Array:     return "array";
    case DebuggerVariableTypes::Reference: return "reference";
    default:          return "null";
  }  
}

void DebuggerVariable::setValues(const ValueList_t& valueList)
{
  m_valueList = valueList;
}

ValueList_t DebuggerVariable::values()
{
  return m_valueList;
}
  
void DebuggerVariable::setType(int type)
{
  m_type = type;
}

int DebuggerVariable::type()
{
  return m_type;
}

void DebuggerVariable::setReference(bool ref)
{
  m_isReference = ref;
}
bool DebuggerVariable::isReference()
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
  if(m_item)
    delete m_item;
  else
    kdDebug(24000) << "DebuggerVariable::~DebuggerVariable: m_item is NULL" << endl;
    
}
