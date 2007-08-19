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
    KMessageBox::error(0L, i18n("<qt>The statemachine describing file <b>%1</b> is not valid.<br />The error message is: <i>%2 in line %3, column %4.</i></qt>", fileName, errorMsg, errorLine, errorColumn), i18n("Invalid StateMachine File"));
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
    if (m_stateNameMapping.contains(state->name))
      kWarning(24001) << "Duplicate state name found: " << state->name;
    else
      m_stateNameMapping.insert(state->name, state);
  }

  for (uint i = 0; i < numOfStates; i++)
  {
    QDomNode n = stateNodes.item(i);
    QDomElement e = n.toElement();
    State * state = m_stateNameMapping[e.attribute("name", QString("state:%1").arg(i))];
    readConditions(&n, state->conditions);
    state->endState = m_stateNameMapping.value(e.attribute("endOfDocument"));
  }

#ifdef DEBUG_STATEMACHINE
  foreach(State *state, m_stateNameMapping)
  {
    if (state->endState)
      kDebug(24001) << "State: name = " << state->name << " endOfDocument=" << state->endState->name;
    else
      kDebug(24001) << "State: name = " << state->name << " endOfDocument= not specified";
    Condition * condition = state->conditions[0];
    int i = 0;
    while (condition)
    {
      kDebug(24001) << "   Condition: " << " comparatorFunct = " << condition->compareFunction.name() << ", argument = \"" << condition->compareFunction.argument() << "\", nextState = (" << condition->nextState << " , " << m_stateNameMapping.key(condition->nextState) << ")";
      int j = 0;
      ActionFunction *action = condition->actionFunctions[0];
      while (action)
      {
        kDebug(24001) << "      Action: name = " << action->name() << ", argument = \"" << action->argument() << "\"";
        j++;
        action = condition->actionFunctions[j];
      }
      i++;
      condition = state->conditions[i];
    }
  }
#endif

  return true;
}

void StateMachine::readConditions(QDomNode *stateNode, Condition* conditions[])
{
  QString nodeName;
  QString conditionName;
  QString nextStateName;
#ifdef DEBUG_STATEMACHINE
  bool defaultConditionExists = false;
#endif
  for (uint i = 0; i < MAXFUNCTIONS; i++)
    conditions[i] = 0;
  int i = 0;
  for (QDomNode n = stateNode->firstChild(); !n.isNull(); n = n.nextSibling() )
  {
    nodeName = n.nodeName();
    if (nodeName == "condition")
    {
      Condition *condition = new Condition();
      for (uint j = 0; j < MAXFUNCTIONS; j++)
        condition->actionFunctions[j] = 0;
      QDomElement el = n.toElement();
      conditionName = el.attribute("name");
      CompareFunction compareFunction(Comparator::factory(conditionName), el.attribute("argument"));
#ifdef DEBUG_STATEMACHINE
      compareFunction.setName(conditionName);
      if (conditionName.toLower() == "isanychar")
        defaultConditionExists = true;
#endif
      condition->compareFunction = compareFunction;
      nextStateName = el.attribute("nextState");
      if (m_stateNameMapping.contains(nextStateName))
        condition->nextState = m_stateNameMapping.value(nextStateName);
      else
      {
#ifdef DEBUG_STATEMACHINE
        if (!nextStateName.isEmpty())
          kWarning(24001) << "State " << nextStateName << " is not known.";
#endif
        condition->nextState = 0;
      }
      QDomElement item = el.firstChild().toElement();
      QString tagName;
      QString argument;
      int j = 0;
      while (!item.isNull())
      {
        tagName = item.tagName();
        if (tagName == "action")
        {
          argument = item.attribute("argument");
          if (argument.isEmpty() && !item.attribute("message").isEmpty())
            argument = i18n(item.attribute("message").toUtf8());
          ActionFunction *action = new ActionFunction(StateActions::factory(item.attribute("name")), argument);
#ifdef DEBUG_STATEMACHINE
          action->setName(item.attribute("name"));
#endif
          condition->actionFunctions[j] = action;
          j++;
          if (j == MAXFUNCTIONS - 1)
            break;
        }
        item = item.nextSibling().toElement();
      }
      conditions[i] = condition;
      i++;
      if (i == MAXFUNCTIONS - 1)
        break;
    }
  }
#ifdef DEBUG_STATEMACHINE
  if (!defaultConditionExists)
    kWarning(24001) << "State " << stateNode->toElement().attribute("name") <<"  misses the default \"isAnyChar\" condition.";
#endif
}

//kate: space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
