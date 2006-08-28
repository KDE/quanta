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

#include <QXmlInputSource>


#include "parserstatus.h"
#include "statemachine.h"

ParserStatus::ParserStatus(QXmlInputSource *source, QXmlLocator *locator, StateMachine *stateMachine) :
  QXmlReader(), m_currState(0), m_source(source), m_locator(locator), m_stateMachine(stateMachine)
{
  Q_ASSERT(source != 0);
  Q_ASSERT(locator != 0);
  Q_ASSERT(stateMachine != 0);
}


ParserStatus::~ParserStatus()
{
}


void ParserStatus::startParsing()
{
  if (! contentHandler())
    return;
  
  contentHandler()->setDocumentLocator(m_locator);
  contentHandler()->startDocument();
  m_currState = m_stateMachine->startState();
  loop();
  contentHandler()->endDocument();
}


void ParserStatus::loop()
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
      if (m_currChar == QXmlInputSource::EndOfDocument)
        return;
    }
      // test conditions and do the actions
    foreach (Condition condition, m_currState->conditions)
    {
      if (condition.compareFunction.call(*this))
      {
        foreach (ActionFunction af, condition.actionFunctions)
        {
          if (! af.call(*this))
            return;
        }
        if (condition.nextState)
          m_currState = condition.nextState;
          
        break;
      }
    }
  }
}


//kate: indent-mode cstyle; space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
