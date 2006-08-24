/***************************************************************************
    begin                : Aug 24 2006
    copyright            : (C) 2006 by Jens Herden <jens@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#include <QString>
#include <QXmlParseException>

#include "parserstatus.h"
#include "quantaxmlinputsource.h"

#include "stateactions.h"

StateActions::ActionFunctPtr StateActions::factory(const QString &name)
{
  QString id = name.toLower();
  if (id == "currentToBuffer") return &currCharToBuffer;
  
  return &crashMe; // in case name is wrong
}


bool StateActions::crashMe(const ParserStatus &parser, const QString &argument)
{
  Q_UNUSED(parser);
  Q_UNUSED(argument);
  Q_ASSERT(false);
  return false;
}


bool StateActions::warning(const ParserStatus &parser, const QString &argument)
{
  if (! parser.errorHandler())
    return true; 
  
  QXmlParseException exception(argument, parser.m_locator->columnNumber(), parser.m_locator->lineNumber());
  return parser.errorHandler()->warning(exception);
}


bool StateActions::error(const ParserStatus &parser, const QString &argument)
{
  if (! parser.errorHandler())
    return true; 
  
  QXmlParseException exception(argument, parser.m_locator->columnNumber(), parser.m_locator->lineNumber());
  return parser.errorHandler()->error(exception);
}


bool StateActions::fatalError(const ParserStatus &parser, const QString &argument)
{
  if (parser.errorHandler())
  {
    QXmlParseException exception(argument, parser.m_locator->columnNumber(), parser.m_locator->lineNumber());
    return parser.errorHandler()->fatalError(exception);
  }
  return false;
}


bool StateActions::currCharToBuffer(const ParserStatus &parser, const QString &argument)
{
  Q_UNUSED(argument);
  parser.m_buffer.append(parser.m_currChar);
  return true;
}


bool StateActions::stringToBuffer(const ParserStatus &parser, const QString &argument)
{
  parser.m_buffer.append(argument);
  return true;
}


bool StateActions::pushCurrChar(const ParserStatus &parser, const QString &argument)
{
  Q_UNUSED(argument);
  parser.m_sourceStack.push(parser.m_currChar);
  return true;
}

//kate: indent-mode cstyle; space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
