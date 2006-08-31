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

#include "dombuilder.h"

DomBuilder::DomBuilder()
{
  m_CDATAstarted = false;
}


DomBuilder::~DomBuilder()
{
}


// from QXmlContentHandler

bool DomBuilder::characters(const QString & ch)
{
  kDebug(24001) << "DomBuilder::Text: " << ch << endl;
  return true;
}


bool DomBuilder::endDocument()
{
  kDebug(24001) << "DomBuilder::End Document" << endl;
  return true;
}


bool DomBuilder::endElement(const QString & namespaceURI, const QString & localName, const QString & qName)
{
  kDebug(24001) << "DomBuilder::End Element: " << qName << endl;
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
  kDebug(24001) << "DomBuilder::Skipped entity" << name << endl;
  return true;
}


bool DomBuilder::startDocument()
{
  kDebug(24001) << "DomBuilder::Start Document" << endl;
  return true;
}


bool DomBuilder::startElement (const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts)
{
  kDebug(24001) << "DomBuilder::startElement: " << qName << endl;
  return true;
}


bool DomBuilder::startPrefixMapping(const QString & prefix, const QString & uri)
{
  return true;
}


// from QXmlLexicalHandler

bool DomBuilder::comment(const QString & ch)
{
  kDebug(24001) << "DomBuilder::Comment: " << ch << endl;
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
  kError(24001) << exception.message() << " at: " << exception.lineNumber() << ", " << exception.columnNumber() << endl;
  return true;
}


bool DomBuilder::fatalError(const QXmlParseException & exception)
{
  return true;
}


bool DomBuilder::warning(const QXmlParseException & exception)
{
  kWarning(24001) << exception.message() << " at: " << exception.lineNumber() << ", " << exception.columnNumber() << endl;
  return true;
}


bool DomBuilder::elementRanges(const KTextEditor::Range & elementRange, const Ranges & attrRanges)
{
  kDebug(24001) << "DomBuilder::Element Range: " << elementRange << endl;
  return true;
}


//kate: space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
