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
#include <QXmlAttributes>

#include "parserstatus.h"
#include "quantaxmlinputsource.h"

#include "stateactions.h"
#include "statemachine.h"

StateActions::ActionFunctPtr StateActions::factory(const QString &name)
{
  QString id = name.toLower();
  if (id == "rememberChar") return &currCharToBuffer;
  if (id == "rememberString") return &stringToBuffer;
  if (id == "popState") return &popState;
  if (id == "pushState") return &pushState;
  if (id == "popState") return &popState;
  if (id == "reportWarning") return &warning;
  if (id == "reportError") return &error;
  if (id == "reportFatalError") return &fatalError;
  if (id == "createComment") return &createComment;
  if (id == "createTag") return &createTag;

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
  kWarning(24001) << argument << endl;
  if (! parser.errorHandler())
    return true; 
  
  QXmlParseException exception(argument, parser.m_locator->columnNumber(), parser.m_locator->lineNumber());
  return parser.errorHandler()->warning(exception);
}


bool StateActions::error(const ParserStatus &parser, const QString &argument)
{
  kError(24001) << argument << endl;
  if (! parser.errorHandler())
    return true; 
  
  QXmlParseException exception(argument, parser.m_locator->columnNumber(), parser.m_locator->lineNumber());
  return parser.errorHandler()->error(exception);
}


bool StateActions::fatalError(const ParserStatus &parser, const QString &argument)
{
  kError(24001) << argument << endl;
  if (! parser.errorHandler())
    return false;
  
  QXmlParseException exception(argument, parser.m_locator->columnNumber(), parser.m_locator->lineNumber());
  return parser.errorHandler()->fatalError(exception);
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


bool StateActions::pushState(const ParserStatus &parser, const QString &argument)
{
  const State *state = parser.m_stateMachine->state(argument);
  if (state)
  {
    parser.m_stateStack.push(state);
    return true;
  }
  return false;
}


bool StateActions::popState(const ParserStatus &parser, const QString &argument)
{
  Q_UNUSED(argument);
  if (parser.m_stateStack.isEmpty())
    return false;
  
  parser.m_currState = parser.m_stateStack.pop();
  return true; 
}


bool StateActions::createTag(const ParserStatus &parser, const QString &argument)
{
  Q_UNUSED(argument);
  if (! parser.contentHandler())
    return true;
  
  return parser.contentHandler()->startElement("", "", parser.m_buffer, QXmlAttributes());
}


bool StateActions::createComment(const ParserStatus &parser, const QString &argument)
{
  Q_UNUSED(argument);
  if (! parser.lexicalHandler())
    return true;
  
  return parser.lexicalHandler()->comment(parser.m_buffer);
}


//kate: indent-mode cstyle; space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
