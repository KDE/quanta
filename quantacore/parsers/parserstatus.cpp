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

#include <kdebug.h>

#include "parserstatus.h"
#include "statemachine.h"

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
  m_tagName.clear();
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
  
  m_source = (QXmlInputSource *) input;
  contentHandler()->setDocumentLocator(m_locator);
  contentHandler()->startDocument();
  m_currState = m_stateMachine->startState();
  bool result = loop();
  contentHandler()->endDocument();
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
      if (m_currChar == QXmlInputSource::EndOfDocument)
      {
        if (m_currState->endState)
          m_currState = m_currState->endState;
        else
          break;
      }
    }
      // test conditions and do the actions
    foreach (Condition condition, m_currState->conditions)
    {
      if (condition.compareFunction.call(*this))
      {
        foreach (ActionFunction af, condition.actionFunctions)
        {
          if (! af.call(*this))
            return false;
        }
        if (condition.nextState)
        {
          m_currState = condition.nextState;
          kDebug(24001) << "State changed to " << m_currState->name << endl;
        }
          
        break;
      }
    }
    if (m_currChar == QXmlInputSource::EndOfDocument)
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
};

    
bool ParserStatus::hasFeature(const QString & name) const
{
  if (name == "http://kdewebdev.org/quanta/features/html-mode")
     return true;
  
  return false;
}

//kate: space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
