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

#ifndef PARSERSTATUS_H
#define PARSERSTATUS_H

#include <QXmlReader>
#include <QString>
#include <QChar>
#include <QStack>


class QXmlInputSource;
class QXmlLocator;
class StateMachine;
struct State;

/**
 * \short 
 * 
 * \author Jens Herden   \<jens@kdewebdev.org\>
 */
class ParserStatus : public QXmlReader 
{
  friend class StateActions;
  friend class Comparator;
  
  public:
    ParserStatus(QXmlInputSource *source, QXmlLocator *locator);
    ~ParserStatus();
    
    void startParsing(State *startState);
    
  private:
    QChar m_currChar;
    mutable QString m_buffer;
    mutable QStack<QChar> m_sourceStack;
    mutable QStack<QChar> m_stateStack;
    mutable State *m_currState;
    // int entity;
    QXmlInputSource *m_source;
    QXmlLocator *m_locator;
};


#endif

//kate: indent-mode cstyle; space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
