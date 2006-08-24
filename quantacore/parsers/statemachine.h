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

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "comparator.h"
#include "stateactions.h"
#include "parserstatus.h"

#include <QList>
#include <QHash>

class QDomNode;

class ActionFunct {

public:
  ActionFunct(StateActions::ActionFunctPtr function, const QString &argument):
    m_function(function), m_argument(argument)
  {}

  ~ActionFunct() {};
  bool call(const ParserStatus &status)
  {
    return m_function(status, m_argument);
  } 

private:
  StateActions::ActionFunctPtr m_function;
  QString m_argument;
  
};

/**
 * This class reads the possible parser states from an XML file and
 * build the ParserState classes from it.
 */
class StateMachine{

  /**
   * Describes a condition in a state. Conditions are tested when a character is
   * read from an input source, and they can execute an action and they trigger
   * a state switch (might go back to the same state).
   */
  struct Condition {
    Comparator::CompareFunctPtr compareFunction; ///<pointer to the condition verification function
    QString compareArgument; ///< possible argument for the above function    
    QList<ActionFunct> actionFunctions; ///<possible list of actions with their arguments
    int nextState; ///<if the condition function returns true, switch to this state
  };

  /**
   * Describes a state, which has a list of conditions and a name.
   *
   */
  struct State {
    QList<Condition> conditions;
    QString name;
  };

  /**
   * A list of possible states in the state machine. The states are referred by
   * their index in the list.
   */
  typedef QList<State> ParsingStates;

public:
   
  StateMachine();

  ~StateMachine();

  /**
   * Build the state machine from an xml file.
   *
   * @param fileName the XML file to be used
   * @return true in case of success, false if the state machine could not be build
   */
  bool build(const QString &fileName);

  
  
private:
  /**
   * Reads the conditions from the dom tree in a list of conditions.
   * @param stateNode read the conditions from under this node
   * @param conditions store the conditions in this list
   */
  void readConditions(QDomNode *stateNode, QList<Condition> &conditions);
  
  ParsingStates m_parsingStates; ///< list with possible parsing states
  QHash<QString, int> m_stateNameMapping; ///<mapping between state name and state index in the above list

};

#endif
//kate: indent-mode cstyle; space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
