/***************************************************************************
    begin                : Mon Jul 11 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

//app includes
#include "completionbase.h"
#include "dtdcompletion.h"
#include "dtds.h"
#include "dtdstruct.h"
#include "node.h"
#include "parseresult.h"
#include "quantacommon.h"
#include "specialareaattributecompletion.h"
#include "specialareagroupcompletion.h"
#include "specialareatagcompletion.h"
#include "tag.h"
#include "xmltagcompletion.h"
#include "xmlattributecompletion.h"
#include "xmlattributevaluecompletion.h"
#include "xmlentitycompletion.h"

//qt includes

//kde includes
#include <kurl.h>

//kdevelop includes
#include <kdevplugin.h>

XmlTagCompletion *CompletionBase::m_xmlTagCompletion = new XmlTagCompletion();
XmlAttributeCompletion *CompletionBase::m_xmlAttributeCompletion = new XmlAttributeCompletion();
XmlAttributeValueCompletion *CompletionBase::m_xmlAttributeValueCompletion = new XmlAttributeValueCompletion();
XmlEntityCompletion *CompletionBase::m_xmlEntityCompletion = new XmlEntityCompletion();
DTDCompletion *CompletionBase::m_dtdCompletion= new DTDCompletion();
SpecialAreaTagCompletion *CompletionBase::m_specialAreaTagCompletion = new SpecialAreaTagCompletion();
SpecialAreaAttributeCompletion *CompletionBase::m_specialAreaAttributeCompletion = new SpecialAreaAttributeCompletion();
SpecialAreaGroupCompletion *CompletionBase::m_specialAreaGroupCompletion = new SpecialAreaGroupCompletion();

CompletionBase::CompletionBase()
{
  m_startsWith = "";
}


CompletionBase::~CompletionBase()
{
}

void CompletionBase::init(ParseResult *base, EditorSource *source, const QPoint& position, Node *currentNode, Koncrete::Plugin *plugin)
{
  m_startsWith = "";
  m_base = base;
  m_source = source;
  m_position = position;
  m_currentNode = currentNode;
  m_dtd = m_currentNode->tag->dtd();
  m_plugin = plugin;
}
  

CompletionBase* CompletionBase::getCompletionObject(ParseResult *base, EditorSource *source, const QPoint& position, Node* currentNode, CompletionType type, CompletionMode mode, Koncrete::Plugin *plugin)
{

  CompletionBase *completionObject = 0L;
  switch (type)
  {
    case XmlTag:
      completionObject = m_xmlTagCompletion;
      break;
    case XmlAttribute:
      completionObject = m_xmlAttributeCompletion;
      break;
    case XmlAttributeValue:
      completionObject = m_xmlAttributeValueCompletion;
      break;
    case XmlEntity:
      completionObject = m_xmlEntityCompletion;
      break;
    case DTDDefinition:
      completionObject = m_dtdCompletion;
      break;
    case SpecialAreaTag:
      completionObject = m_specialAreaTagCompletion;
      break;
    case SpecialAreaAttribute:
      completionObject = m_specialAreaAttributeCompletion;
      break;
    case SpecialAreaGroups:
      completionObject = m_specialAreaGroupCompletion;
      break;
  }
  if (completionObject)
  {
    completionObject->init(base, source, position, currentNode, plugin);
    if (!completionObject->isValid(mode))
      return 0L;
  }
  return completionObject;
}

void CompletionBase::filterCompletion(KTextEditor::CompletionItem *completion , QString *string)
{
  Q_UNUSED(completion);
  string->remove(0, m_startsWith.length());
}
