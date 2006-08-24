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

#ifndef STATEACTIONS_H
#define STATEACTIONS_H


class QString;
class ParserStatus;

class StateActions
{
  public:
    /**
    * the pointer to an action function 
    * \return true if parsing can continue
    **/
    typedef bool (*ActionFunctPtr) (const ParserStatus &, const QString &);
  
    /**
    * \param name the name of the wanted action fuction
    * \return The function pointer. This will never return 0 but a default 
    *         function that stops parsing and will crash in debug mode
    **/
    static ActionFunctPtr factory (const QString &name);
    
    static bool always(const ParserStatus &parser, const QString &argument);
    
    static bool crashMe(const ParserStatus &parser, const QString &argument);

};


#endif

//kate: indent-mode cstyle; space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
