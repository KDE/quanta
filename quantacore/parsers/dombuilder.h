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

#ifndef DOMBUILDER_H
#define DOMBUILDER_H

#include <QXmlContentHandler>
#include <QXmlLexicalHandler>

class QXmlLocator;

/**
 * \short This class builds the dom tree from the parser events.
 * 
 * \author Jens Herden   \<jens@kdewebdev.org\>
 */
class DomBuilder : public QXmlContentHandler, public QXmlLexicalHandler, public QXmlErrorHandler 
{
  public:
    DomBuilder();
    ~DomBuilder();
    
    /**
     * \name QXmlLexicalHandler Interface
     *
     * The following methods implement the \ref QXmlLexicalHandler interface
     * \{
     */
    bool characters (const QString & ch);
    bool endDocument();
    bool endElement(const QString & namespaceURI, const QString & localName, const QString & qName);
    bool endPrefixMapping (const QString & prefix);
    QString errorString() const;
    bool ignorableWhitespace(const QString & ch);
    bool processingInstruction(const QString & target, const QString & data);
    void setDocumentLocator(QXmlLocator * locator) {m_locator = locator;}
    bool skippedEntity(const QString & name);
    bool startDocument();
    bool startElement (const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts);
    bool startPrefixMapping(const QString & prefix, const QString & uri); 
    /**
     * \}
     */
    /**
     * \name QXmlLexicalHandler Interface
     *
     * The following methods implement the \ref QXmlLexicalHandler interface
     * \{
     */
    bool comment(const QString & ch);
    bool endCDATA();
    bool endDTD();
    bool endEntity(const QString & name);
    bool startCDATA();
    bool startDTD(const QString & name, const QString & publicId, const QString & systemId);
    bool startEntity(const QString & name);
    /**
     * \}
     */
    /**
     * \name QXmlErrorHandler Interface
     *
     * The following methods implement the \ref QXmlErrorHandler interface
     * \{
     */
    bool error(const QXmlParseException & exception);
    bool fatalError(const QXmlParseException & exception);
    bool warning(const QXmlParseException & exception); 
    /**
     * \}
     */
  private:
    QXmlLocator * m_locator;
};


#endif

//kate: space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
