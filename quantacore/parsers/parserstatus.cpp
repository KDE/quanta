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

//own includes
#include "parserstatus.h"
#include "statemachine.h"
#include "quantaxmlinputsource.h"

//qt includes
#include <QXmlInputSource>
#include <QTime>

//kde includes
#include <kdebug.h>


#undef DEBUGMODE

#ifdef DEBUGMODE
#define PARSERSTATUSDEBUG( S )  kDebug(24001) << S << endl;
#else
#define PARSERSTATUSDEBUG( S )
#endif


ParserStatus::ParserStatus(QXmlLocator *locator, StateMachine *stateMachine) :
  QXmlReader()
{
  m_DTDHandler = 0;
  m_contentHandler = 0;
  m_declHandler = 0;
  m_entityResolver = 0;
  m_errorHandler = 0;
  m_lexicalHandler = 0;
  m_quantaHandler = 0;
  m_locator = 0;
  reset(locator, stateMachine);
}


ParserStatus::~ParserStatus()
{
  delete m_locator;
}

void ParserStatus::reset(QXmlLocator * locator, StateMachine * stateMachine)
{
  Q_ASSERT_X(locator != 0, "ParserStatus::reset", "locator undefined");
  Q_ASSERT_X(stateMachine != 0, "ParserStatus::reset", "stateMachine undefined");

  if (m_locator != locator)
    delete m_locator;

  m_locator = locator;
  m_stateMachine = stateMachine;
  m_buffer.clear();
  m_buffer.reserve(1000);
  m_tagName.clear();
  m_tagName.reserve(100);
  m_attrName.clear();
  m_attrName.reserve(100);
  m_currChar = QChar();
  m_source = 0;
  m_sourceStack.clear();
  m_stateStack.clear();
  m_attributes.clear();
  m_namespace.clear();
  m_htmlMode = true;
  m_tagRange = KTextEditor::Range(KTextEditor::Cursor::invalid(),
                                  KTextEditor::Cursor::invalid());
  m_attrRanges.clear();
}


bool ParserStatus::parse(const QXmlInputSource * input)
{
  if (! contentHandler())
    return false;

  Q_ASSERT_X(input != 0, "ParserStatus::parse", "input source is undefined");
  if (! input)
    return false;

  QTime timer;
  timer.start();

//  m_source = (QXmlInputSource *) input;
  contentHandler()->setDocumentLocator(m_locator);
  contentHandler()->startDocument();
  m_currState = m_stateMachine->startState();
  bool result = loop();
  contentHandler()->endDocument();

  kDebug(24000) << "Parsing time: " << timer.elapsed() << " ms" << endl;
  return result;
}

bool ParserStatus::parse(const QuantaXmlInputSource * input)
{
  if (! contentHandler())
    return false;

  Q_ASSERT_X(input != 0, "ParserStatus::parse", "input source is undefined");
  if (! input)
    return false;

  QTime timer;
  timer.start();

  m_source = (QuantaXmlInputSource *) input;
  contentHandler()->setDocumentLocator(m_locator);
  contentHandler()->startDocument();
  m_currState = m_stateMachine->startState();
  bool result = loop();
  contentHandler()->endDocument();

  kDebug(24000) << "Parsing time: " << timer.elapsed() << " ms" << endl;
  return result;
}


bool ParserStatus::loop()
{
  while (m_currState)
  {
      // get the next character
    if (! m_sourceStack.isEmpty())
      m_currChar = m_sourceStack.pop();
    else
    {
      m_currChar = m_source->next();
      if (m_currChar == QXmlInputSource::EndOfData)
        m_currChar = m_source->next();
      if (m_currChar == '\n')
        m_currChar = ' ';
    }
    if (m_currChar == QXmlInputSource::EndOfDocument)
    {
      if (m_currState->endState)
        m_currState = m_currState->endState;
      else
        break;
    }

      // test conditions and do the actions
    Condition *cond = m_currState->conditions[0];
    int i = 0;
    while (cond)
    {
      if (cond->compareFunction.call(*this))
      {
        const State * nextState = cond->nextState;
        ActionFunction *action = cond->actionFunctions[0];
        int j = 0;
        while (action)
        {
          if (! action->call(*this))
            return false;
          j++;
          action = cond->actionFunctions[j];
        }
        if (nextState)
        {
#ifdef DEBUGMODE
          if (m_currState != nextState)
          {
            PARSERSTATUSDEBUG("State changed to " << nextState->name << " char: " << m_currChar)
          }
#endif
          m_currState = nextState;
        }

        break;
      }
      i++;
      cond = m_currState->conditions[i];
    }
    if (m_currChar == QXmlInputSource::EndOfDocument && m_sourceStack.isEmpty())
      break;
  }
  return true;
}


bool ParserStatus::feature(const QString & name, bool * ok) const
{
  if (name == "http://kdewebdev.org/quanta/features/html-mode")
  {
    if (ok)
      *ok = true;
    return m_htmlMode;
  }
  if (ok)
    *ok = false;
  return false;
}


void ParserStatus::setFeature(const QString & name, bool value)
{
  if (name == "http://kdewebdev.org/quanta/features/html-mode")
    m_htmlMode = value;

  return;
}


bool ParserStatus::hasFeature(const QString & name) const
{
  if (name == "http://kdewebdev.org/quanta/features/html-mode")
     return true;

  return false;
}

//kate: space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
