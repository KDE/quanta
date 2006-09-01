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
#include <klocale.h>
#include <dom/html_document.h>
#include <dom/dom_doc.h>
#include <dom/dom_node.h>
#include <dom/dom_text.h>
#include <dom/dom_element.h>
#include <dom/dom_xml.h>
#include <dom/dom_exception.h>
#include <dom/html_element.h>
#include <dom/dom_string.h>


#include "dombuilder.h"

#define DEBUGMODE

#ifdef DEBUGMODE
#define DOMBUILDERDEBUG( S ) kDebug(24001) << S << endl;
#else
#define DOMBUILDERDEBUG( S )
#endif


#ifdef DEBUGMODE
#include <QDialog>
#include <QBoxLayout>
#include "domtreeview.h"
#endif

DomBuilder::DomBuilder()
{
  m_CDATAstarted = false;
  m_DTDstarted = false;
  m_locator = 0;
  // i am not allowed to add text nodes to the document, so make a child first
  m_currNode = m_document.createElement("quanta");
  m_document.appendChild(m_currNode);
  m_error.clear();
}


DomBuilder::~DomBuilder()
{
}


// from QXmlContentHandler

bool DomBuilder::characters(const QString & ch)
{
  DOMBUILDERDEBUG("DomBuilder::Text: " << QString(ch).replace('\n', ' '))
  
  DOM::Node node; 
  if (m_CDATAstarted)
  {
    node = m_document.createCDATASection(ch);
    Q_ASSERT_X(! node.isNull(), "DomBuilder::characters", "could not create a CDATA element");
    if (node.isNull())
    {
      m_error = i18n("Unable to create a CDATA element!");
      return false;
    }
  }
  else
  {
    try
    {
      node = m_document.createTextNode(ch);
      // TODO if the currNode is text we don't want to add the new node as child but as sibling
      m_currNode.appendChild(node);
    }
    catch (DOM::DOMException& e)
    {
      DOMBUILDERDEBUG("Unable to create a text element! DOM::Exception: " << e.code)
      m_error = i18n("Unable to create a text node!");
      return false;
    } 
  }
  return true;
}


bool DomBuilder::endDocument()
{
  DOMBUILDERDEBUG("DomBuilder::End Document")
#ifdef DEBUGMODE
  showTreeView();
#endif
  return true;
}


bool DomBuilder::endElement(const QString & namespaceURI, const QString & localName, const QString & qName)
{
  DOMBUILDERDEBUG("DomBuilder::End Element: " << qName)
      
  if (! m_currNode.parentNode().isNull())
    m_currNode = m_currNode.parentNode();
  
  return true;
}


bool DomBuilder::endPrefixMapping (const QString & prefix)
{
  return true;
}


QString DomBuilder::errorString() const
{
  return m_error;
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


bool DomBuilder::startElement(const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts)
{
  if (localName.isEmpty())
  {
    DOMBUILDERDEBUG("DomBuilder::startElement: localName name is empty!")
    m_error = i18n("Unable to create an empty element!");
    return true;
  }
  
  DOM::Element el;
  try
  {
    if (qName.isEmpty())
    {
      DOMBUILDERDEBUG("DomBuilder::startElement: create HTML element for " << localName)
      el = m_document.createElement(localName);
    }
    else {
  // was it not HTML?
//     if (el.isNull())
//     {
//       if (qName.isEmpty())
//       {
//         DOMBUILDERDEBUG("DomBuilder::startElement: create XML element for " << localName)
//         el = m_document.createElement(localName);
//       }
//       else
//       {
        DOMBUILDERDEBUG("DomBuilder::startElement: create XML element for " << qName << " Uri: " << namespaceURI)
        el = m_document.createElementNS(namespaceURI, qName);
//       }
    }
  }
  catch (DOM::DOMException& e) 
  {
    DOMBUILDERDEBUG("DomBuilder::startElement: Unable to create an element: DOMException " << e.code)
    m_error = i18n("Unable to create an HTML element: DOMException !");
    return true;
  }
  Q_ASSERT_X(! el.isNull(), "DomBuilder::startElement", "could not create an element");
  if (el.isNull())
  {
    m_error = i18n("Unable to create an element!");
    return true;
  }
  // copy attributes
  int i = 0;
  int max = atts.count();
  while (i < max)
  {
    el.setAttributeNS(atts.uri(i), atts.qName(i), atts.value(i));
    ++i;
  }
  try 
  {
    m_currNode.appendChild(el);
  }
  catch (DOM::DOMException& e) 
  {
    DOMBUILDERDEBUG("DomBuilder::startElement: Unable to append! DOMException: " << e.code)
  }
  m_currNode = el;
  return true;
}


bool DomBuilder::startPrefixMapping(const QString & prefix, const QString & uri)
{
  return true;
}


// from QXmlLexicalHandler

bool DomBuilder::comment(const QString & ch)
{
  DOMBUILDERDEBUG("DomBuilder::Comment: " << QString(ch).replace('\n', ' '))
  
  m_currNode.appendChild(m_document.createComment(ch));
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


// from QuantaHandler

bool DomBuilder::elementRanges(const KTextEditor::Range & elementRange, const Ranges & attrRanges)
{
  DOMBUILDERDEBUG("DomBuilder::Element Range: " << elementRange)
  m_elementRange = elementRange;
  m_attrRanges = attrRanges;
  return true;
}


void DomBuilder::showTreeView()
{
#ifdef DEBUGMODE
/*  m_HTMLdocument = m_HTMLdocument.implementation().createHTMLDocument("test");
  DOM::NodeList childs = m_HTMLdocument.childNodes();
  uint i = 0;
  while (i < childs.length())
  {
    try
    {
      m_HTMLdocument.body().appendChild(childs.item(i));
    }
    catch (DOM::DOMException& e) 
    {
      DOMBUILDERDEBUG("showTreeView failed to import: " << childs.item(i).nodeName() << " exception: " << e.code)
    };
    ++i;
  } */
  QDialog dialog;
  QBoxLayout layout(QBoxLayout::LeftToRight, &dialog);
  DOMTreeView view(&dialog);
  layout.addWidget(&view);
  view.showTree(m_document.firstChild());
  dialog.resize(300,300);
  dialog.exec();
#endif
}

//kate: space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
