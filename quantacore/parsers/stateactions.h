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
 * Use the factory to get a pointer for an action name.
 * 
 * \author Jens Herden   \<jens@kdewebdev.org\>
 **/

class StateActions
{
  public:
    /**
     * The pointer to an action function. 
     * \return \e false will abort the parsing
     */
    typedef bool (*ActionFunctPtr) (const ParserStatus &, const QString &);
  
    /**
     * This factory function translates the names used in the XML file into
     * pointer for the internal data structure.
     * The case of the name does not matter. 
     * The following names are defined and are pointing to:
     * \li rememberchar -> \ref currCharToBuffer
     * \li rememberstring -> \ref stringToBuffer
     * \li popstate -> \ref pushState
     * \li pushstate -> \ref stringToBuffer
     * \li pushbackchar -> \ref pushCurrChar
     * \li reportwarning -> \ref warning
     * \li reporterror -> \ref error
     * \li reportfatalerror -> \ref fatalError
     * \li createcomment -> \ref createComment
     * \li createtag -> \ref createTag
     * \li createendtag -> \ref createEndTag
     * \li addtoattributename -> \ref addToAttr
     * \li addtotagname -> \ref addToTag
     * \li xxx -> \ref popAttrRange
     * \li xxx -> \ref setAttrRangeEnd
     * \li starttag -> \ref setTagRangeStart
     * 
     * 
     * \param name the function name
     * \return function pointer for the name or pointer to \ref crashme if the name is unknown
     */
    static ActionFunctPtr factory (const QString & name);
    
    /**
     * Send error message to the error handler interface in parser
     * and stops parsing.
     * 
     * @param parser the used ParserStatus
     * @param argument the error message to send
     * \return always \e false 
     */
    static bool fatalError(const ParserStatus & parser, const QString & argument);
    
    /**
     * Send error message to the error handler interface in parser.
     * 
     * @param parser the used ParserStatus
     * @param argument the error message to send
     * \return the return value of error handler function 
     */
    static bool error(const ParserStatus & parser, const QString & argument);
    
    /**
     * Send warning message to the error handler interface in parser.
     * 
     * @param parser the used ParserStatus
     * @param argument the warning message to send
     * \return the return value of error handler function 
     */
    static bool warning(const ParserStatus & parser, const QString & argument);
    
    /**
     * Fallback if the name given to the factory was unknown.
     * It will crash in debug mode!
     * 
     * @param parser unused
     * @param argument unused
     * \return always \e false
     */
    static bool crashMe(const ParserStatus & parser, const QString & argument);
    
    /**
     * Add parser.m_currChar to parser.m_buffer.
     * 
     * @param parser the used ParserStatus
     * @param argument unused
     * \return always \e true
     */
    static bool currCharToBuffer(const ParserStatus & parser, const QString & argument);
    
    /**
     * Add string in argument to parser.m_buffer.
     * 
     * @param parser the used ParserStatus
     * @param argument string to add
     * \return always \e true
     */
    static bool stringToBuffer(const ParserStatus & parser, const QString & argument);
    
    /**
     * push back parser.m_currChar into parser.m_sourceStack
     * 
     * @param parser the used ParserStatus
     * @param argument unused
     * \return always \e true
     */
    static bool pushCurrChar(const ParserStatus & parser, const QString & argument);
    
    /**
     * push the current state on the stack
     * 
     * @param parser the used ParserStatus
     * @param argument the name of the state
     * \return \e true if the state is known, \e false otherwise
     */
    static bool pushState(const ParserStatus & parser, const QString & argument);
    
    /**
     * pop state from the stack
     * 
     * @param parser the used ParserStatus
     * @param argument unused
     * \return \e true if there was a state on the stack, \e false otherwise
     */
    static bool popState(const ParserStatus & parser, const QString & argument);
    
    /**
     * reports the found comment to the handler
     * 
     * @param parser the used ParserStatus
     * @param argument unused
     * \return the return value of the handler or \e true if there is no handler
     */
    static bool createComment(const ParserStatus & parser, const QString & argument);
    
    /**
     * reports the found start tag to the handler
     * 
     * @param parser the used ParserStatus
     * @param argument unused
     * \return the return value of the handler or \e true if there is no handler
     */
    static bool createTag(const ParserStatus & parser, const QString & argument);
    
    /**
     * reports the found end tag to the handler
     * 
     * @param parser the used ParserStatus
     * @param argument unused
     * \return the return value of the handler or \e true if there is no handler
     */
    static bool createEndTag(const ParserStatus & parser, const QString & argument);
    
    /**
     * reports the found text area to the handler
     *
     * @param parser the used ParserStatus
     * @param argument unused
     * \return the return value of the handler or \e true if there is no handler
     */
    static bool createText(const ParserStatus & parser, const QString & argument);
    
    /**
     * reports the found entity to the handler. Does not clear the buffer.
     *
     * @param parser the used ParserStatus
     * @param argument unused
     * \return the return value of the handler or \e true if there is no handler
     */
    static bool createEntity(const ParserStatus & parser, const QString & argument);
    
    /**
     * creates a new Range in the container with the current position set as start & end
     * 
     * @param parser the used ParserStatus
     * @param argument unused
     * \return true
     */
    static bool addAttrRange(const ParserStatus & parser, const QString & argument);
    
    /**
     * set the end of the last range in the container
     * 
     * @param parser the used ParserStatus
     * @param argument unused
     * \return true
     */
    static bool setAttrRangeEnd(const ParserStatus & parser, const QString & argument);
    
    /**
     * remove the last range from the container
     * 
     * @param parser the used ParserStatus
     * @param argument unused
     * \return true
     */
    static bool popAttrRange(const ParserStatus & parser, const QString & argument);
    
    /**
     * Append a string to the tag name. If argument is empty, the current
     * character is appended.
     * 
     * @param parser the used ParserStatus
     * @param argument the string to append
     * \return true
     */
    static bool addToTag(const ParserStatus & parser, const QString & argument);
    
    /**
     * Append a string to the attribute name. If argument is empty, the current
     * character is appended.
     * 
     * @param parser the used ParserStatus
     * @param argument the string to append
     * \return true
     */
    static bool addToAttr(const ParserStatus & parser, const QString & argument);
    
    /**
     * Add the found attribute to the container. 
     * \note This will clear the buffer and the attribute name buffer
     * 
     * @param parser the used ParserStatus
     * @param argument unused
     * \return true
     */
    static bool addAttr(const ParserStatus & parser, const QString & argument);
    
    /**
     * Move the tag name to the namespace variable 
     * \note This clears the tag name.
     * 
     * @param parser the used ParserStatus
     * @param argument unused
     * \return true
     */
    static bool tagToNamespace(const ParserStatus & parser, const QString & argument);

    /**
     * set the end of the element range 
     * 
     * @param parser the used ParserStatus
     * @param argument unused
     * \return true
     */
//     static bool setTagRangeEnd(const ParserStatus & parser, const QString & argument);
    
    /**
     * set the end of the element range 
     * 
     * @param parser the used ParserStatus
     * @param argument unused
     * \return true
     */
    static bool setTagRangeStart(const ParserStatus & parser, const QString & argument);

};

#endif

//kate: space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
