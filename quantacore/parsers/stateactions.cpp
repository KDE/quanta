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
#include <QXmlContentHandler>
#include <QXmlErrorHandler>

#include <kdebug.h>
#include <ktexteditor/cursor.h>
#include <ktexteditor/range.h>

#include "parserstatus.h"
#include "quantaxmlinputsource.h"
#include "statemachine.h"
#include "quantahandler.h"

#include "stateactions.h"


#undef DEBUGMODE

#ifdef DEBUGMODE
#define ACTIONDEBUG( S )  kDebug(24001) << S << endl;
#else
#define ACTIONDEBUG( S )
#endif


// I am using a define because I want to have it as fast a possible
#define REPORTRANGES   if (parser.m_quantaHandler) \
    parser.m_quantaHandler->elementRanges(parser.m_tagRange, parser.m_attrRanges); \
\
  parser.m_tagRange.setRange(KTextEditor::Cursor::invalid(), KTextEditor::Cursor::invalid()); \
  parser.m_attrRanges.clear();


StateActions::ActionFunctPtr StateActions::factory(const QString &name)
{
  QString id = name.toLower();
  if (id == "addchartobuffer") return &currCharToBuffer;
  if (id == "addstringtobuffer") return &stringToBuffer;
  if (id == "pushbackchar") return &pushCurrChar;
  if (id == "popstate") return &popState;
  if (id == "pushstate") return &pushState;
  if (id == "reportwarning") return &warning;
  if (id == "reporterror") return &error;
  if (id == "reportfatalerror") return &fatalError;
  if (id == "createcomment") return &createComment;
  if (id == "createtag") return &createTag;
  if (id == "createendtag") return &createEndTag;
  if (id == "createtext") return &createText;
  if (id == "createentity") return &createEntity;
  if (id == "remembernamespace") return &tagToNamespace;
  if (id == "createattribute") return &addAttr;
  if (id == "addtoattributename") return &addToAttr;
  if (id == "addtotagname") return &addToTag;
  if (id == "") return &popAttrRange;
  if (id == "") return &setAttrRangeEnd;
  if (id == "starttag") return &setTagRangeStart;
  if (id == "quickparsespecial") return &popState;
  if (id == "fillbufferuntil") return &fillBufferUntil;

  kWarning(24001) << "unknown function name '" << id << "' in StateActions::factory" << endl;
  return &crashMe; // in case name is wrong
}


bool StateActions::crashMe(const ParserStatus & parser, const QString & argument)
{
  Q_UNUSED(parser);
  Q_UNUSED(argument);
/*  Q_ASSERT(false);
  return false;*/
  kWarning(24001) << "Unknown action function called." << endl;
  return true;
}


bool StateActions::warning(const ParserStatus & parser, const QString & argument)
{
  if (! parser.errorHandler())
    return true;

  QXmlParseException exception(argument, parser.m_locator->columnNumber(), parser.m_locator->lineNumber());
  return parser.errorHandler()->warning(exception);
}


bool StateActions::error(const ParserStatus & parser, const QString & argument)
{
  if (! parser.errorHandler())
    return true;

  QXmlParseException exception(argument, parser.m_locator->columnNumber(), parser.m_locator->lineNumber());
  return parser.errorHandler()->error(exception);
}


bool StateActions::fatalError(const ParserStatus & parser, const QString & argument)
{
  kError(24001) << argument << endl;
  if (! parser.errorHandler())
    return false;

  QXmlParseException exception(argument, parser.m_locator->columnNumber(), parser.m_locator->lineNumber());
  return parser.errorHandler()->fatalError(exception);
}


bool StateActions::currCharToBuffer(const ParserStatus & parser, const QString & argument)
{
  Q_UNUSED(argument);
  parser.m_buffer.append(parser.m_currChar);
  return true;
}


bool StateActions::stringToBuffer(const ParserStatus & parser, const QString & argument)
{
  parser.m_buffer.append(argument);
  return true;
}


bool StateActions::pushCurrChar(const ParserStatus & parser, const QString & argument)
{
  Q_UNUSED(argument);
  parser.m_sourceStack.push(parser.m_currChar);
  return true;
}


bool StateActions::pushState(const ParserStatus & parser, const QString & argument)
{
  const State *state = parser.m_stateMachine->state(argument);
  if (state)
  {
    parser.m_stateStack.push(state);
    return true;
  }
  return false;
}


bool StateActions::popState(const ParserStatus & parser, const QString & argument)
{
  Q_UNUSED(argument);
  if (parser.m_stateStack.isEmpty())
    return false;

  parser.m_currState = parser.m_stateStack.pop();
  ACTIONDEBUG("State changed to " << parser.m_currState->name << " using popState" )
  return true;
}


bool StateActions::createTag(const ParserStatus & parser, const QString & argument)
{
  Q_UNUSED(argument);

  REPORTRANGES

  bool result;
  if (parser.m_namespace.isEmpty())
    result = parser.contentHandler()->startElement("", parser.m_tagName, "", parser.m_attributes);
  else
    result = parser.contentHandler()->startElement(parser.m_namespace, parser.m_tagName, parser.m_namespace + ':' + parser.m_tagName, parser.m_attributes);
  
  parser.m_tagName.clear();
  parser.m_namespace.clear();
  parser.m_attributes.clear();
  return result;
}


bool StateActions::createEndTag(const ParserStatus & parser, const QString & argument)
{
  Q_UNUSED(argument);

  REPORTRANGES

  bool result = parser.contentHandler()->endElement(
                   parser.m_namespace + ':' + parser.m_tagName, "", parser.m_tagName);
  parser.m_tagName.clear();
  parser.m_namespace.clear();
  parser.m_attributes.clear(); // just in case
  return result;
}


bool StateActions::createComment(const ParserStatus & parser, const QString & argument)
{
  Q_UNUSED(argument);
  if (! parser.lexicalHandler())
    return true;

  REPORTRANGES

  bool result = parser.lexicalHandler()->comment(parser.m_buffer);
  parser.m_buffer.clear();
  parser.m_buffer.reserve(1000);
  return result;
}

bool StateActions::createText(const ParserStatus & parser, const QString & argument)
{
  Q_UNUSED(argument);
  bool result = parser.contentHandler()->characters(parser.m_buffer);
  parser.m_buffer.clear();
  parser.m_buffer.reserve(1000);
  return result;
}

bool StateActions::createEntity(const ParserStatus & parser, const QString & argument)
{
  Q_UNUSED(argument);
  bool result = parser.contentHandler()->skippedEntity(parser.m_buffer);
  return result;
}


bool StateActions::addAttrRange(const ParserStatus & parser, const QString & argument)
{
  Q_UNUSED(argument);
  parser.m_attrRanges.append(KTextEditor::Range(parser.m_locator->lineNumber(),
                                                parser.m_locator->columnNumber(),
                                                parser.m_locator->lineNumber(),
                                                parser.m_locator->columnNumber()));
  return true;
}


bool StateActions::setAttrRangeEnd(const ParserStatus & parser, const QString & argument)
{
  Q_UNUSED(argument);
  if (! parser.m_attrRanges.isEmpty())
  {
    parser.m_attrRanges.last().end().setPosition(
              parser.m_locator->lineNumber(),
              parser.m_locator->columnNumber());
  }
  return true;
}


bool StateActions::popAttrRange(const ParserStatus & parser, const QString & argument)
{
  Q_UNUSED(argument);
  if (! parser.m_attrRanges.isEmpty())
    parser.m_attrRanges.pop_back();

  return true;
}


bool StateActions::addToTag(const ParserStatus & parser, const QString & argument)
{
  if (argument.isEmpty())
    parser.m_tagName.append(parser.m_currChar);
  else
    parser.m_tagName.append(argument);
  return true;
}


bool StateActions::addToAttr(const ParserStatus & parser, const QString & argument)
{
  if (argument.isEmpty())
    parser.m_attrName.append(parser.m_currChar);
  else
    parser.m_attrName.append(argument);
  return true;
}


bool StateActions::addAttr(const ParserStatus & parser, const QString & argument)
{
  Q_UNUSED(argument);
  // TODO add namespace
  parser.m_attributes.append(parser.m_attrName, "", parser.m_attrName, parser.m_buffer);
  parser.m_attrName.clear();
  parser.m_attrName.reserve(100);
  parser.m_buffer.clear();
  parser.m_buffer.reserve(1000);
  return true;
}


bool StateActions::tagToNamespace(const ParserStatus & parser, const QString & argument)
{
  Q_UNUSED(argument);
  parser.m_namespace = parser.m_tagName;
  parser.m_tagName.clear();
  return true;
}


bool StateActions::setTagRangeStart(const ParserStatus & parser, const QString & argument)
{
  Q_UNUSED(argument);
  parser.m_tagRange.setRange(KTextEditor::Range(parser.m_locator->lineNumber(),
                                                parser.m_locator->columnNumber(),
                                                parser.m_locator->lineNumber(),
                                                parser.m_locator->columnNumber()));
  return true;
}

bool StateActions::fillBufferUntil(const ParserStatus & parser, const QString & argument)
{
  parser.m_buffer.append(parser.m_currChar);
  if (argument.isEmpty())
  {
    return true;
  }
  parser.m_buffer.append(parser.m_source->charactersUntil(argument.at(0)));
  return true;
}

//kate: space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
