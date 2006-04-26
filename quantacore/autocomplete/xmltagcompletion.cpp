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
#include "xmltagcompletion.h"
#include "dtdstruct.h"
#include "node.h"
#include "parseresult.h"
#include "qtag.h"
#include "tag.h"

#include <ktexteditor/codecompletioninterface.h>

XmlTagCompletion::XmlTagCompletion()
 : CompletionBase()
{
}


XmlTagCompletion::~XmlTagCompletion()
{
}

QList<KTextEditor::CompletionItem>* XmlTagCompletion::completionEntries()
{
  QList<KTextEditor::CompletionItem>* completions = new QList<KTextEditor::CompletionItem>();
  KTextEditor::CompletionItem completion;
//FIXME  completion.type = "tagCompletion";

  QTag *parentQTag = 0L;
  if (m_currentNode && m_currentNode->parent)
      parentQTag = QTag::tagFromDTD(m_currentNode->parent);
  QStringList tagNameList;
  QMap<QString, QString> comments;
  QString tagName;
  QHashIterator<QString, QTag*> it(*(m_currentNode->tag->dtd()->tagsList));
  int i = 0;
  while (it.hasNext())
  {
    it.next();
    QTag *tag = it.value();
    if (tag->type != "entity")
    {
      tagName = tag->name();
      if (!tagName.isEmpty() && tagName.startsWith(m_startsWith, Qt::CaseInsensitive))
      {
        if (!parentQTag || (parentQTag && parentQTag->isChild(tagName)))
        {
          tagName = tag->name() + QString("%1").arg(i, 10);
          tagNameList += tagName;
          comments.insert(tagName, tag->comment);
          i++;
        }
      }
    }
  }

  QHashIterator<QString, QTag*> it2(m_base->userTags);
  while (it2.hasNext())
  {
    it2.next();
    QTag *tag = it2.value();
    if (tag->name().startsWith(m_startsWith, Qt::CaseInsensitive))
    {
      tagName = tag->name() + QString("%1").arg(i, 10);
      tagNameList += tagName;
      comments.insert(tagName, tag->comment);
      i++;
    }
  }

  tagNameList.sort();
  uint count = tagNameList.count();
  for (uint i = 0; i < count; i++)
  {
//FIXME    completion.text = Tag::convertCase(tagNameList[i]);
//FIXME    completion.text = completion.text.left(completion.text.length() - 10);
//FIXME    completion.comment = comments[tagNameList[i]];
    completions->append( completion );
  }

  return completions;

}

bool XmlTagCompletion::isValid(CompletionMode mode)
{
  switch (mode)
  {
    case Forced:
      return true;
    case Manual:
      {
        QPoint begin = m_currentNode->tag->beginPos();
        int nameCol = begin.y() + m_currentNode->tag->nameAndNS().length() + 1;
        if (m_position.y() > begin.y() && m_position.y() <= nameCol) //we are inside a tag name, so show the possible tags
        {
          m_startsWith = m_currentNode->tag->name().left(m_position.y() - begin.y() - 1);
          return true;
        }
        break;
      }
    case Auto:
      {
       //Tag autocompletion is active if we are inside an empty node or inside a text area,
        //but insertin "<" inside a text area it means that a new tag was started, which is
        //invalid at this moment.
        if (m_currentNode->tag->isType(Tag::Empty) ||
            m_currentNode->tag->isType(Tag::Text) ||
           (m_currentNode->tag->isType(Tag::XmlTag) && !m_currentNode->tag->validXMLTag))
        {
          return true;
        }
        break;
      }
  }
  return false;
}
