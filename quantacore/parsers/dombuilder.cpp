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

//own includes
#include "dombuilder.h"
#include "dommodel.h"

//kde includes
#include <kdebug.h>
#include <klocale.h>
 


#define DEBUGMODE

#ifdef DEBUGMODE
#define DOMBUILDERDEBUG( S )
//#define DOMBUILDERDEBUG( S ) kDebug(24001) << S;
#else
#define DOMBUILDERDEBUG( S )
#endif


#ifdef DEBUGMODE
#include <QDialog>
#include <QBoxLayout>
#include <QTreeView>
#endif

DomBuilder::DomBuilder() 
{
  m_CDATAstarted = false;
  m_DTDstarted = false;
  m_locator = 0;
  // i am not allowed to add text nodes to the document, so make a child first
  m_startElement = new DomModelItem();
  m_startElement->setType(DomModelItem::TagStart);
  m_startElement->setRange(new KTextEditor::Range(KTextEditor::Range::invalid()));
  m_lastInserted = m_startElement;
  m_currentElement = m_startElement;
  m_model = new DomModel(m_startElement);
  m_error.clear();
}


DomBuilder::~DomBuilder()
{
}


// from QXmlContentHandler

bool DomBuilder::characters(const QString & ch)
{
  DOMBUILDERDEBUG("DomBuilder::Text: " << QString(ch).replace('\n', ' '))
  if (m_currentElement->type() != DomModelItem::Comment)
  {
    m_lastInserted = new DomModelItem(m_currentElement);
  } else
  {
    m_lastInserted = new DomModelItem(m_currentElement->parent());
  }
  m_lastInserted->setType(DomModelItem::Text);
  m_lastInserted->setName(ch);
  m_lastInserted->setRange(new KTextEditor::Range(m_elementRange));
  return true;
}


bool DomBuilder::endDocument()
{
  DOMBUILDERDEBUG("DomBuilder::End Document")
#ifdef DEBUGMODE
//   showTreeView();
#endif
  return true;
}


bool DomBuilder::endElement(const QString & /*namespaceURI*/, const QString & /*localName*/, const QString & qName)
{
  DOMBUILDERDEBUG("DomBuilder::End Element: " << qName)
      
  DomModelItem *emptyElement = 0L;
  DomModelItem *endElement = 0L;
  bool insertEmptyElement = (m_lastInserted->type() != DomModelItem::Text);
    
  DomModelItem *el = m_currentElement;
  while (el != m_startElement)
  {
    if (el->name() == qName)
    {
      if (insertEmptyElement)
      {
        emptyElement = new DomModelItem(m_lastInserted->parent());
      }
      endElement = new DomModelItem(el->parent());
      m_currentElement = el->parent();
      break;
    } else
    {
      el = el->parent();
    }    
  }
  if (!endElement)
  {
    if (insertEmptyElement)
    {
      emptyElement = new DomModelItem(m_currentElement->parent());
    } 
    endElement = new DomModelItem(m_currentElement->parent());
  }
  
  if (emptyElement)
  {
    emptyElement->setName("Empty area before endElement");
    emptyElement->setRange(new KTextEditor::Range(m_lastInserted->range()->end(), m_elementRange.start()));
  }
  
  endElement->setType(DomModelItem::TagEnd);
  endElement->setName('/' + qName);
  endElement->setRange(new KTextEditor::Range(m_elementRange));
  
  m_lastInserted = endElement;    
  return true;
}


bool DomBuilder::endPrefixMapping (const QString & /*prefix*/)
{
  return true;
}


QString DomBuilder::errorString() const
{
  return m_error;
}


bool DomBuilder::ignorableWhitespace(const QString & /*ch*/)
{
  return true;
}


bool DomBuilder::processingInstruction(const QString & /*target*/, const QString & /*data*/)
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
  
  if (m_lastInserted->type() != DomModelItem::Text)
  {
    DomModelItem *el = new DomModelItem(m_currentElement);
    el->setRange(new KTextEditor::Range(m_lastInserted->range()->end(), m_elementRange.start()));
    el->setName("Empty area before startElement");
  }
  
  m_lastInserted = new DomModelItem(m_currentElement);
  m_lastInserted->setType(DomModelItem::TagStart);
  
  if (qName.isEmpty())
  {
    DOMBUILDERDEBUG("DomBuilder::startElement: " << localName)
    m_lastInserted->setName(localName);
  }
  else 
  {
    DOMBUILDERDEBUG("DomBuilder::startElement: create XML element for " << qName << " Uri: " << namespaceURI)
    m_lastInserted->setNameSpace(namespaceURI);
    m_lastInserted->setName(qName);
  }
  
  // copy attributes
  m_lastInserted->setAttributes(atts);
  
  m_lastInserted->setRange(new KTextEditor::Range(m_elementRange));
  m_lastInserted->setAttributeRanges(m_attrRanges);
  if (localName.startsWith("DOCTYPE "))
  {
    m_lastInserted->setType(DomModelItem::DocType);
  } else
    m_currentElement = m_lastInserted;
  return true;
}


bool DomBuilder::startPrefixMapping(const QString & /*prefix*/, const QString & /*uri*/)
{
  return true;
}


// from QXmlLexicalHandler

bool DomBuilder::comment(const QString & ch)
{
  DOMBUILDERDEBUG("DomBuilder::Comment: " << QString(ch).replace('\n', ' '))
      
  if (m_lastInserted->type() != DomModelItem::Text)
  {
    DomModelItem *el = 0L;
    if (m_currentElement->type() != DomModelItem::Comment)
    {
      el = new DomModelItem(m_currentElement);    
    } else
    {
      el = new DomModelItem(m_currentElement->parent());
    }
    el->setRange(new KTextEditor::Range(m_lastInserted->range()->end(), m_elementRange.start()));
    el->setName("Empty area before comment");
  }
  
  if (m_currentElement->type() != DomModelItem::Comment)
  {
    m_lastInserted = new DomModelItem(m_currentElement);
  } else
  {
    m_lastInserted = new DomModelItem(m_currentElement->parent());
  }
  m_lastInserted->setType(DomModelItem::Comment);
  m_lastInserted->setName(ch);
  m_lastInserted->setRange(new KTextEditor::Range(m_elementRange));
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


bool DomBuilder::endEntity(const QString & /*name*/)
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


bool DomBuilder::startDTD(const QString & /*name*/, const QString & /*publicId*/, const QString & /*systemId*/)
{
  m_DTDstarted = true;
  m_startColumn = m_locator->columnNumber() - 1; // TODO check if -1 is correct
  m_startLine = m_locator->lineNumber();
  return true;
}


bool DomBuilder::startEntity(const QString & /*name*/)
{
  return true;
}


bool DomBuilder::error(const QXmlParseException & exception)
{
  kError(24001) << exception.message() << " at: " << exception.lineNumber() << ", " << exception.columnNumber() << endl;
  return true;
}


bool DomBuilder::fatalError(const QXmlParseException & /*exception*/)
{
  return true;
}


bool DomBuilder::warning(const QXmlParseException & exception)
{
  kWarning(24001) << exception.message() << " at: " << exception.lineNumber() << ", " << exception.columnNumber();
  return true;
}


// from QuantaHandler

bool DomBuilder::elementRanges(const KTextEditor::Range & elementRange, const QVector<KTextEditor::Range> & attrRanges)
{
  DOMBUILDERDEBUG("DomBuilder::Element Range: " << elementRange)
  m_elementRange = elementRange;
  m_attrRanges = attrRanges;
  return true;
}
#if 0
void coutTree (TreeElement *node, int indent)
{
  QString output;
  int bLine, bCol, eLine, eCol;
  while (node)
  {
    output = "";
    output.fill('.', indent);
    KTextEditor::Range *range = node->range();
    if (range) {
      bLine = range->start().line();
      bCol = range->start().column();
      eLine = range->end().line();
      eCol = range->end().column();
    }
          
    output += node->name().replace('\n'," ");
    kDebug(24000) << output <<" (" << node->type() << ") at pos " <<
        bLine << ":" << bCol << " - " << eLine << ":" << eCol << " This: "<< node << " Parent: " << node->parent() << " Prev: " << node->previous() << " Next: " << node->next() << " Child: " << node->firstChild();
    /*           for(j = 0; j < node->tag->attrCount(); j++)
    {
    kDebug(24000)<< " attr" << j << " " <<
    node->tag->getAttribute(j).nameLine << ":" <<
    node->tag->getAttribute(j).nameCol << " - " <<
    node->tag->getAttribute(j).valueLine << ":" <<
    node->tag->getAttribute(j).valueCol;
  }
    */
    if (node->firstChild())
      coutTree(node->firstChild(), indent + 4);
    node = node->next();
  }
}
#endif
                 
void DomBuilder::showTreeView()
{
/*  QDialog dialog;
  QBoxLayout layout(QBoxLayout::LeftToRight, &dialog);
  QTreeView view(&dialog);
  layout.addWidget(&view);
  DomModel model(m_startElement);
  view.setModel(&model);
  dialog.resize(300,300);
  dialog.exec();*/
}


//kate: space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
