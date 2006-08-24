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

/**
 * \short This class holds the static functions that provide actions for the parser.
 * 
 * Use the factory to get a pointer for a action name.
 * 
 * \author Jens Herden   \<jens@kdewebdev.org\>
 **/

class StateActions
{
  public:
    /**
    * the pointer to an action function 
    * \return \e true if parsing should continue
    */
    typedef bool (*ActionFunctPtr) (const ParserStatus &, const QString &);
  
    /**
    * \param name the name of the wanted action function
    * \return The function pointer. This will never return 0 if the name is
    *         unknown but a default function that stops parsing and will 
    *         crash in debug mode.
    */
    static ActionFunctPtr factory (const QString &name);
    /**
     * Send error message to the error handler interface in parser
     * and stops parsing.
     * 
     * @param parser the used ParserStatus
     * @param argument the error message to send
     * \return always \e false 
     */
    static bool fatalError(const ParserStatus &parser, const QString &argument);
    /**
     * Send error message to the error handler interface in parser.
     * 
     * @param parser the used ParserStatus
     * @param argument the error message to send
     * \return the return value of error handler function 
     */
    static bool error(const ParserStatus &parser, const QString &argument);
    /**
     * Send warning message to the error handler interface in parser.
     * 
     * @param parser the used ParserStatus
     * @param argument the warning message to send
     * \return the return value of error handler function 
     */
    static bool warning(const ParserStatus &parser, const QString &argument);
    /**
     * Fallback if the name given to the factory was unknown.
     * It will crash in debug mode!
     * 
     * @param parser unused
     * @param argument unused
     * \return always \e false
     */
    static bool crashMe(const ParserStatus &parser, const QString &argument);
    /**
     * Add parser.m_currChar to parser.m_buffer.
     * 
     * @param parser the used ParserStatus
     * @param argument unused
     * \return always \e true
     */
    static bool currCharToBuffer(const ParserStatus &parser, const QString &argument);
    /**
     * Add string in argument to parser.m_buffer.
     * 
     * @param parser the used ParserStatus
     * @param argument string to add
     * \return always \e true
     */
    static bool stringToBuffer(const ParserStatus &parser, const QString &argument);
    /**
     * push back parser.m_currChar into parser.m_sourceStack
     * 
     * @param parser the used ParserStatus
     * @param argument unused
     * \return always \e true
     */
    static bool pushCurrChar(const ParserStatus &parser, const QString &argument);
};


#endif

//kate: indent-mode cstyle; space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
