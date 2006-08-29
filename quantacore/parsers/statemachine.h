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

#define DEBUG_STATEMACHINE

/**
 *  @short Template class to call function by their name.
 * 
 *  Right now there are two kind of function we would like to call:
 * comparator functions (Comparator::CompareFunctPtr) used to check if a
 * condition in a state is true and action functions
 * (StateActions::ActionFunctPtr), used to execute
 * actions associated to a condition once a comparator function returns true.
 *  As of now, the functions have one QString argument.
 *  Example:
 * @code
* //create the function class from a name
 *  Comparator::CompareFunctPtr functionPtr(Comparator::factory("functionName"));
 *  Function<Comparator::CompareFunctPtr> compareFunction(functionPtr, argument);
 *  ...
 * //call the function
 *  compareFunction.call(statusObject); //statusObject is a ParserStatus object 
 * @endcode
 *
 * @author Andras Mantia <amantia@kde.org>
 */
template <class T>
class Function {

public:
  /**
   * Constructs the function.
   * @param function a function pointer
   * @param argument the argument for the function
   */
  Function(T function, const QString &argument):
    m_function(function), m_argument(argument)
    {}
  Function() {};

  ~Function() {};
  /**
   * Call the function.
   * @param status the ParserStatus object
   * @return false in case of problems
   */
  bool call(const ParserStatus & status)
  {
    return m_function(status, m_argument);
  }

#ifdef DEBUG_STATEMACHINE
  void setName(const QString &name) {m_name = name;}
  QString name() {return m_name;}
  QString argument() {return m_argument;}
#endif

private:
  T m_function;
  QString m_argument;
#ifdef DEBUG_STATEMACHINE
  QString m_name; //for debugging
#endif
};

/** Helper typedef's to ease readability.*/  
typedef Function<StateActions::ActionFunctPtr> ActionFunction;
typedef Function<Comparator::CompareFunctPtr> CompareFunction;
      
struct State;
/**
 * @short Describes a condition in a state.
 * 
 * Conditions are tested when a character is
 * read from an input source, and they can execute an action and they trigger
 * a state switch (might go back to the same state). The @ref compareFunction
 * is called and if it returns true the @ref actionFunctions are executed and
 * the state machine will switch to the @ref nextState.
 *
 * @author Andras Mantia <amantia@kde.org>
 */
struct Condition {
  CompareFunction compareFunction; ///<condition verification function
  QList< ActionFunction > actionFunctions; ///<possible list of actions with their arguments
  State * nextState; ///<if the condition function returns true, switch to this state
};

/**
 * @short Describes a state, which has a list of conditions and a name.
 *
 * @author Andras Mantia <amantia@kde.org>
 */
struct State {
  QList<Condition> conditions; ///<the list of conditions
  QString name; ///<the name of the state
};

/**
 * @short This class reads the possible parser states from an XML file and
 * builds the ParserState classes from it.
 *
 * @author Andras Mantia <amantia@kde.org>
 */
class StateMachine{


public:
   
  StateMachine();

  ~StateMachine();

  /**
   * Build the state machine from an xml file.
   *
   * @param fileName the XML file to be used
   * @return @e true in case of success, @e false if the state machine could not be build
   */
  bool build(const QString &fileName);

  /**
   * @return the start State 
   */
  const State * startState() const {return m_startState;}
  
  /**
   * @param name the name of the state in the XML file 
   * @return a pointer to a State for the given name or 0 for unknown states
   */
  const State * state(const QString &name) const {return m_stateNameMapping.value(name);}
  
private:
  /**
   * Reads the conditions from the dom tree in a list of conditions.
   * @param stateNode read the conditions from under this node
   * @param conditions store the conditions in this list
   */
  void readConditions(QDomNode *stateNode, QList<Condition> &conditions);
  
  QHash<QString, State *> m_stateNameMapping; ///<mapping between state name and state index in the above list
  State *m_startState; ///< the statemachine starts from this state. It is the first state read from the xml file

};

#endif
//kate: space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
