/***************************************************************************
    begin                : Tue Jul 12 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#include "xmlattributecompletion.h"
#include "editorsource.h"
#include "dtdstruct.h"
#include "node.h"
#include "parseresult.h"
#include "qtag.h"
#include "settings.h"
#include "tag.h"
#include "tagattr.h"

#include <ktexteditor/codecompletioninterface.h>

XmlAttributeCompletion::XmlAttributeCompletion()
 : CompletionBase()
{
}


XmlAttributeCompletion::~XmlAttributeCompletion()
{
}


QList<KTextEditor::CompletionItem>* XmlAttributeCompletion::completionEntries()
{
  QList<KTextEditor::CompletionItem> *completions = new QList<KTextEditor::CompletionItem>();
  KTextEditor::CompletionItem completion;
  QTag *tag = QTag::tagFromDTD(m_currentNode->tag->dtd(), m_currentNode->tag->name());
  if (!tag)
  {
    tag = m_base->userTags.value(m_currentNode->tag->name().toLower());
  }
  if (tag)
  {
//FIXME    completion.type = "attributeCompletion";

    //list specified attributes for this tag
    AttributeList *list = tag->attributes();
    QList<KTextEditor::CompletionItem> tempCompletions;
    QStringList nameList;
    uint count = list->count();
    for (uint i = 0; i < count; i++)
    {
      QString item = list->at(i)->name;
      if (item.startsWith(m_startsWith, Qt::CaseInsensitive) && (!m_startsWith.isEmpty() || !m_currentNode->tag->hasAttribute(item, false)))
      {
//FIXME        completion.text = TagAttr::convertCase(item);
//FIXME        completion.comment = list->at(i)->type;
        tempCompletions.append( completion );
        nameList.append(completion.text());
      }
    }

    //list common attributes for this tag
    QStringList::Iterator end(tag->commonGroups.end());
    for (QStringList::Iterator it = tag->commonGroups.begin(); it != end; ++it)
    {
      AttributeList *attrs = tag->parentDTD->commonAttrs->value(*it);
      uint count= attrs->count();
      for (uint j = 0; j < count; j++)
      {
        QString name = attrs->at(j)->name;
        if (name.startsWith(m_startsWith, Qt::CaseInsensitive) && (!m_startsWith.isEmpty() || !m_currentNode->tag->hasAttribute(name, false)))
        {
//FIXME          completion.text = TagAttr::convertCase(name);
//FIXME          completion.comment = attrs->at(j)->type;
          tempCompletions.append( completion );
          nameList.append(completion.text());
        }
      }
    }

    //below isn't fast, but enough here. May be better with QMap<QString, KTextEditor::CompletionItem>
    nameList.sort();
    QStringList::Iterator listEnd(nameList.end());
    for (QStringList::Iterator it = nameList.begin(); it != listEnd; ++it)
    {
     QList<KTextEditor::CompletionItem>::Iterator end(tempCompletions.end());
      for (QList<KTextEditor::CompletionItem>::Iterator compIt = tempCompletions.begin(); compIt != end; ++compIt)
      {
        if ((*compIt).text() == *it)
        {
          completions->append(*compIt);
          break;
        }
      }
    }
  } // if (tag)

  return completions;
}

CompletionBase::CompletionRequest XmlAttributeCompletion::completionDone(const KTextEditor::CompletionItem& completion)
{
  Q_UNUSED(completion);
  m_source->insertText("=" + Settings::self()->attributeQuotationChar(), true);
  m_source->insertText(Settings::self()->attributeQuotationChar(), false);
  return RequestCompletion;
}

bool XmlAttributeCompletion::isValid(CompletionMode mode)
{
  switch (mode)
  {
    case Forced:
      return true;
    case Manual:
      {
        int index = m_currentNode->tag->attributeIndexAtPos(m_position.x(), m_position.y());
        QString s = m_source->text(m_position.x(), m_position.y(), m_position.x(), m_position.y());
        if (index != -1 || s ==" " || s==">" || s == "/") //inside an attribute or between attributes
        {
          int bLine, bCol;
          if (index != -1)
          {
            m_currentNode->tag->attributeNamePos(index, bLine, bCol);
            m_startsWith = m_currentNode->tag->attribute(index).left(m_position.y() - bCol);
          } else
          {
            m_startsWith = m_source->text(m_position.x(), 0, m_position.x(), m_position.y()-1);
            m_startsWith = m_startsWith.section(' ', -1);
          }
          return true;
        }
        break;
      }
    case Auto:
      {
        if (m_currentNode->tag->isType(Tag::XmlTag))
        {
          int index = m_currentNode->tag->valueIndexAtPos(m_position.x(), m_position.y());
          if (index == -1)
            return true;
        }
        break;
      }
  }
  return false;
}
