/***************************************************************************
    begin                : Thu Aug 24 2006
    copyright            : (C) 2006 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#include "statemachine.h"

#include <QDomDocument>
#include <QFile>
#include <QFileInfo>

#include <kdebug.h>
#include <klocale.h>
#include <kmessagebox.h>


StateMachine::StateMachine()
{
}

StateMachine::~StateMachine()
{
  qDeleteAll(m_stateNameMapping);
  m_stateNameMapping.clear();
}

bool StateMachine::build(const QString &fileName)
{
  QFile xmlFile(fileName);
  if (!xmlFile.exists() || !xmlFile.open(QIODevice::ReadOnly))
    return false;
    
  QDomDocument dom;
  QString errorMsg;
  int errorLine;
  int errorColumn;
  if (!dom.setContent(&xmlFile, &errorMsg, &errorLine, &errorColumn))
  {
    KMessageBox::error(0L, i18n("<qt>The statemachine describing file <b>%1</b> is not valid.<br> The error message is: <i>%2 in line %3, column %4.</i></qt>", fileName, errorMsg, errorLine, errorColumn), i18n("Invalid StateMachine File"));
    xmlFile.close();
    return false;
  }
  xmlFile.close();

  QDomNodeList stateNodes = dom.elementsByTagName("state");
  uint numOfStates = stateNodes.count();
  m_startState = 0;

  //first build the state list with empty states and store the mapping
  //between the state name and the index in the list. This mapping
  //is needed to translate the nextState attribute of conditions to a number.
  for (uint i = 0; i < numOfStates; i++)
  {
    QDomNode n = stateNodes.item(i);
    QDomElement e = n.toElement();
    State * state = new State();
    m_startState = m_startState ? m_startState : state;
    state->name = e.attribute("name", QString("state:%1").arg(i));
    m_stateNameMapping.insert(state->name, state);
  }

  for (uint i = 0; i < numOfStates; i++)
  {
    QDomNode n = stateNodes.item(i);
    QDomElement e = n.toElement();
    readConditions(&n, m_stateNameMapping[e.attribute("name", QString("state:%1").arg(i))]->conditions);
  }

#ifdef DEBUG_STATEMACHINE  
  foreach(State *state, m_stateNameMapping)
  {
    kDebug(24000) << "State: name = " << state->name << endl;
    foreach (Condition condition, state->conditions)
    {
      kDebug(24000) << "   Condition: " << " comparatorFunct = " << condition.compareFunction.name() << ", argument = \"" << condition.compareFunction.argument() << "\" nextState = (" << condition.nextState << " , " << m_stateNameMapping.key(condition.nextState) << ")" << endl;
      foreach(ActionFunction action, condition.actionFunctions)
      {
        kDebug(24000) << "      Action: name = " << action.name() << ", argument = " << action.argument() << endl;
      }
    } 
  }
#endif
    
  return true;
}

void StateMachine::readConditions(QDomNode *stateNode, QList<Condition> &conditions)
{
  QString nodeName;
  for (QDomNode n = stateNode->firstChild(); !n.isNull(); n = n.nextSibling() )
  {
    nodeName = n.nodeName();
    if (nodeName == "condition")
    {
      Condition condition;
      QDomElement el = n.toElement();
      CompareFunction compareFunction(Comparator::factory(el.attribute("name")), el.attribute("argument"));
#ifdef DEBUG_STATEMACHINE
      compareFunction.setName(el.attribute("name"));
#endif      
      condition.compareFunction = compareFunction;
      if (m_stateNameMapping.contains(el.attribute("nextState")))
        condition.nextState = m_stateNameMapping.value(el.attribute("nextState"));
      else
        condition.nextState = 0;
      QDomElement item = el.firstChild().toElement();
      QString tagName;
      while (!item.isNull())
      {
        tagName = item.tagName();
        if (tagName == "action")
        {
          ActionFunction action(StateActions::factory(item.attribute("name")), item.attribute("argument"));
#ifdef DEBUG_STATEMACHINE
          action.setName(item.attribute("name"));
#endif          
          condition.actionFunctions.append(action);
        }
        item = item.nextSibling().toElement();
      }
      conditions.append(condition);
    }
  }
}

//kate: indent-mode cstyle; space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
