/***************************************************************************
    begin                : Thu Jul 14 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#include "xmlentitycompletion.h"
#include "dtds.h"
#include "dtdstruct.h"
#include "editorsource.h"
#include "node.h"
#include "qtag.h"
#include "specialareagroupcompletion.h"
#include "tag.h"

#include <ktexteditor/codecompletioninterface.h>

XmlEntityCompletion::XmlEntityCompletion()
 : CompletionBase()
{
}


XmlEntityCompletion::~XmlEntityCompletion()
{
}

QList<KTextEditor::CompletionItem>* XmlEntityCompletion::completionEntries()
{
  QList<KTextEditor::CompletionItem> *completions = new QList<KTextEditor::CompletionItem>();

  QMap<QString, KTextEditor::CompletionItem> completionMap;
  KTextEditor::CompletionItem completion;

  //first search for entities defined in the document
   const DTDStruct *dtdDTD = DTDs::ref()->find("dtd");
   if (dtdDTD)
   {
     StructTreeGroup group;
     uint cnt = dtdDTD->structTreeGroups.count();
     for (uint j = 0; j < cnt; j++)
     {
       group = dtdDTD->structTreeGroups[j];
       if (!group.autoCompleteAfterRx.pattern().isEmpty() &&
           group.autoCompleteAfterRx.indexIn("&") != -1)
       {
         SpecialAreaGroupCompletion *inlineDTDCompletion = static_cast<SpecialAreaGroupCompletion*>(CompletionBase::getCompletionObject(m_base, m_source, m_position, m_currentNode, SpecialAreaGroups, Forced, m_plugin));
         inlineDTDCompletion->setGroup(&(dtdDTD->structTreeGroups[j]));
         QList<KTextEditor::CompletionItem> *inlineDTDCompletions = inlineDTDCompletion->completionEntries();
         uint count = inlineDTDCompletions->count();
         for (uint i = 0; i < count; i++)
         {
           completion = (*inlineDTDCompletions)[i];
           if (completion.text().startsWith(m_startsWith))
           {
//FIXME            completion.type = "entityCompletion";
            completions->append(completion);
            completionMap[completion.text()] = completion;
           }
         }
         break;
       }
     }
   }


//FIXME  completion.type = "entityCompletion";
  //add the entities from the tag files
  QHashIterator<QString, QTag*> it(*(m_currentNode->tag->dtd()->tagsList));
  while (it.hasNext())
  {
    it.next();
    QTag *tag = it.value();
    if (tag->type == "entity")
    {
      QString tagName = tag->name();
      if (m_startsWith.isEmpty() || tagName.startsWith(m_startsWith, Qt::CaseInsensitive) )
      {
//FIXME        completion.text = tagName;
        completions->append(completion);
        completionMap[tagName] = completion;
      }
    }
  }

  //add the global entities
  QHash<QString, QTag*> *globalEntities = DTDs::ref()->globalEntities();

  QHashIterator<QString, QTag*> itGlobal(*globalEntities);
  while (itGlobal.hasNext())
  {
    itGlobal.next();
    QTag *tag = itGlobal.value();
    if (tag->type == "entity")
    {
      QString tagName = tag->name();
      if (!completionMap.contains(tagName) && (m_startsWith.isEmpty() || tagName.startsWith(m_startsWith, Qt::CaseInsensitive)))
      {
//FIXME        completion.text = tagName;
        completions->append(completion);
        completionMap[tagName] = completion;
      }
    }
  }

  QList<KTextEditor::CompletionItem> *completions2 = new QList<KTextEditor::CompletionItem>();
  QMap<QString, KTextEditor::CompletionItem>::ConstIterator end(completionMap.constEnd());
  for (QMap<QString, KTextEditor::CompletionItem>::ConstIterator it = completionMap.constBegin(); it != end; ++it)
  {
    completions2->append(it.value());
  }
  delete completions;
  completions = completions2;

  return completions;
}

void XmlEntityCompletion::filterCompletion(KTextEditor::CompletionItem *completion , QString *string)
{
  CompletionBase::filterCompletion(completion, string);
  string->append(";");
}

bool XmlEntityCompletion::isValid(CompletionMode mode)
{
  switch (mode)
  {
    case Forced:
      return true;
    case Manual:
      {
        QString s = m_source->textLine(m_position.x()).left(m_position.y());
        int pos = s.lastIndexOf('&');
        if (pos != -1)
        {
          int pos2 = s.indexOf(';', pos);
          if (pos2 == -1)
          {
            s = s.mid(pos + 1);
            if (s.isEmpty() || !s.trimmed().isEmpty())
            {
                m_startsWith = s;
                return true;
            }
          }
        }
        break;
      }
   case Auto:
      return true;
  }
  return false;
}
