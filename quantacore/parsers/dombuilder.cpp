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

#include <kdebug.h>
#include <dom/html_document.h>
#include <dom/dom_doc.h>


#include "dombuilder.h"

#define DEBUGMODE

#ifdef DEBUGMODE
  #define DOMBUILDERDEBUG( S ) kDebug(24001) << S << endl;
#else
  #define DOMBUILDERDEBUG( S )
#endif


DomBuilder::DomBuilder()
{
  m_CDATAstarted = false;
  m_DTDstarted = false;
  m_locator = 0;
  m_HTMLdocument = new DOM::HTMLDocument();
  m_document = new DOM::Document();
}


DomBuilder::~DomBuilder()
{
  delete m_HTMLdocument;
  m_HTMLdocument = 0;
  delete m_document;
  m_document = 0;
}


// from QXmlContentHandler

bool DomBuilder::characters(const QString & ch)
{
  DOMBUILDERDEBUG("DomBuilder::Text: " << ch)
  return true;
}


bool DomBuilder::endDocument()
{
  DOMBUILDERDEBUG("DomBuilder::End Document")
  return true;
}


bool DomBuilder::endElement(const QString & namespaceURI, const QString & localName, const QString & qName)
{
  DOMBUILDERDEBUG("DomBuilder::End Element: " << qName)
  return true;
}


bool DomBuilder::endPrefixMapping (const QString & prefix)
{
  return true;
}


QString DomBuilder::errorString() const
{
  return QString();
}


bool DomBuilder::ignorableWhitespace(const QString & ch)
{
  return true;
}


bool DomBuilder::processingInstruction(const QString & target, const QString & data)
{
  return true;
}


bool DomBuilder::skippedEntity(const QString & name)
{
  DOMBUILDERDEBUG("DomBuilder::Skipped entity" << name)
  return true;
}


bool DomBuilder::startDocument()
{
  DOMBUILDERDEBUG("DomBuilder::Start Document")
  return true;
}


bool DomBuilder::startElement (const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts)
{
  DOMBUILDERDEBUG("DomBuilder::startElement: ")
  return true;
}


bool DomBuilder::startPrefixMapping(const QString & prefix, const QString & uri)
{
  return true;
}


// from QXmlLexicalHandler

bool DomBuilder::comment(const QString & ch)
{
  DOMBUILDERDEBUG("DomBuilder::Comment: " << ch)
  return true;
}


bool DomBuilder::endCDATA()
{
  m_CDATAstarted = false;
  return true;
}


bool DomBuilder::endDTD()
{
  m_DTDstarted = false;
  m_startColumn = 0;
  m_startLine = 0;
  return true;
}


bool DomBuilder::endEntity(const QString & name)
{
  return true;
}


bool DomBuilder::startCDATA()
{
  m_CDATAstarted = true;
  m_startColumn = m_locator->columnNumber() - 1; // TODO check if -1 is correct
  m_startLine = m_locator->lineNumber();
  return true;
}


bool DomBuilder::startDTD(const QString & name, const QString & publicId, const QString & systemId)
{
  m_DTDstarted = true;
  m_startColumn = m_locator->columnNumber() - 1; // TODO check if -1 is correct
  m_startLine = m_locator->lineNumber();
  return true;
}


bool DomBuilder::startEntity(const QString & name)
{
  return true;
}


bool DomBuilder::error(const QXmlParseException & exception)
{
  DOMBUILDERDEBUG( exception.message() << " at: " << exception.lineNumber() << ", " << exception.columnNumber() )
  return true;
}


bool DomBuilder::fatalError(const QXmlParseException & exception)
{
  return true;
}


bool DomBuilder::warning(const QXmlParseException & exception)
{
  DOMBUILDERDEBUG( exception.message() << " at: " << exception.lineNumber() << ", " << exception.columnNumber() )
  return true;
}


// from QuantaHandler

bool DomBuilder::elementRanges(const KTextEditor::Range & elementRange, const Ranges & attrRanges)
{
  DOMBUILDERDEBUG("DomBuilder::Element Range: " << elementRange)
  m_elementRange = elementRange;
  m_attrRanges = attrRanges;
  return true;
}


//kate: space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
