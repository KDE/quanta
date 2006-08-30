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
#include <QVector>
#include <QXmlAttributes>

#include <ktexteditor/range.h>

#include "quantahandler.h"


class QXmlInputSource;
class QXmlLocator;
class QXmlContentHandler;
class QXmlDTDHandler;
class QXmlDeclHandler;
class QXmlEntityResolver;
class QXmlLexicalHandler;
class QXmlErrorHandler;
class QuantaHandler;

class StateMachine;
struct State;

/**
 * \short Implements the QXmlReader interface and holds data needed during parsing.
 * 
 * This class does the actual parsing. How to parse is defined in the \ref StateMachine ,
 * the data is coming from the \ref QXmlInputSource and the \ref QXmlLocator is needed
 * to generate the position information for the parsed data so that Quanta knows where 
 * the tags are in the source. 
 * 
 * Every state in the state machine has a list of conditions to test. Each condition is 
 * a pointer to function in \ref Comparator . If the function return true all associated
 * actions are called. 
 * 
 * An action is also just a pointer into \ref StateActions . 
 * 
 * \note Before you can use this class you have to set the handlers of the parsing events. 
 * At least \ref setContentHandler must be used.
 * \note Features and properties are not supported yet.
 * 
 * \sa QXmlReader
 * \author Jens Herden   \<jens@kdewebdev.org\>
 */
class ParserStatus : public QXmlReader 
{
  friend class StateActions;
  friend class Comparator;
  
  public:
    /**
     * Constructor
     * 
     * \param locator the QXmlLocator to use, mandatory argument!
     * \param stateMachine the StateMachine to use, mandatory argument!
     */
    ParserStatus(QXmlLocator * locator, StateMachine * stateMachine);
    
    ~ParserStatus();
    
    /**
     * Reset the class into the start condition. Erases all member variables.
     * 
     * \note This does not change the set handler functions.
     * \note This class takes ownership of the locator and destroys it when it 
     * does not need it anymore.
     * 
     * \param locator the QXmlLocator to use, mandatory argument!
     * \param stateMachine the StateMachine to use, mandatory argument!
     */
    void reset(QXmlLocator * locator, StateMachine * stateMachine);
    
    /**
     * \name QXmlReader Interface
     *
     * The following methods implement the \ref QXmlReader interface
     * \{
     */
    QXmlDTDHandler * DTDHandler() const {return m_DTDHandler;};
    
    QXmlContentHandler * contentHandler() const {return m_contentHandler;};
    
    QXmlDeclHandler * declHandler() const {return m_declHandler;};
    
    QXmlEntityResolver * entityResolver() const {return m_entityResolver;};
    
    QXmlErrorHandler * errorHandler() const {return m_errorHandler;};
    
    bool feature(const QString & name, bool * ok = 0) const;
    
    bool hasFeature(const QString & name) const;
    
    bool hasProperty(const QString & name) const {Q_UNUSED(name); return false;};
    
    QXmlLexicalHandler * lexicalHandler () const {return m_lexicalHandler;};
    /**
     * Start the parsing process. 
     * 
     * \note You have to call at least \ref setContentHandler before you call this. 
     * 
     * \param input the source where the data comes from
     * \return \e true if parsing was successfull \e false if not
     */
    bool parse (const QXmlInputSource * input);
    
    void * property (const QString & name, bool * ok = 0) const
    { // we do not support properties yet
      Q_UNUSED(name);
      if (ok) *ok = false;
      return 0;
    }
    void setContentHandler(QXmlContentHandler * handler) {m_contentHandler = handler;};
    
    void setDTDHandler(QXmlDTDHandler * handler) {m_DTDHandler = handler;};
    
    void setDeclHandler(QXmlDeclHandler * handler) {m_declHandler = handler;};
    
    void setEntityResolver(QXmlEntityResolver * handler) {m_entityResolver = handler;};
    
    void setErrorHandler(QXmlErrorHandler * handler) {m_errorHandler = handler;};
    
    void setFeature(const QString & name, bool value); 
    
    void setLexicalHandler(QXmlLexicalHandler * handler) {m_lexicalHandler = handler;};
    
    void setQuantaHandler(QuantaHandler * handler) {m_quantaHandler = handler;};
    
    void setProperty(const QString & name, void * value)
    {
      Q_UNUSED(name); Q_UNUSED(value);
      return;
    };
    /**
     * \}
     */
  private:
    /**
     * helper function for \ref startParsing
     */
    bool loop();
    
    QXmlDTDHandler * m_DTDHandler;
    QXmlContentHandler * m_contentHandler;
    QXmlDeclHandler * m_declHandler;
    QXmlEntityResolver * m_entityResolver;
    QXmlErrorHandler * m_errorHandler;
    QXmlLexicalHandler * m_lexicalHandler;
    QuantaHandler * m_quantaHandler;
        
    QChar m_currChar;
    mutable QStack<QChar> m_sourceStack;
    mutable QString m_buffer;
    // state handling
    mutable const State * m_currState;
    mutable QStack<const State *> m_stateStack;
    // tag handling
    mutable QString m_namespace;
    mutable QString m_tagName;
    mutable KTextEditor::Range m_tagRange;
    // attribute handling
    mutable QString m_attrName;
    mutable QuantaHandler::Ranges m_attrRanges;
    mutable QXmlAttributes m_attributes;
    /**
     * for the feature http://kdewebdev.org/quanta/features/html-mode
     * \e true parsing in html mode, \e false parsing in xml mode
     */
    bool m_htmlMode; 
    QXmlInputSource * m_source;
    QXmlLocator * m_locator;
    StateMachine * m_stateMachine;
};


#endif

//kate: space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
