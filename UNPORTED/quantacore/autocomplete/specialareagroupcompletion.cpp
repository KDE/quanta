/***************************************************************************
    begin                : Fri Jul 15 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#include "specialareagroupcompletion.h"
#include "dtdstruct.h"
#include "editorsource.h"
#include "groupelement.h"
#include "groupstorage.h"
#include "node.h"
#include "parseresult.h"
#include "qtag.h"
#include "quantacommon.h"
#include "tag.h"

#include <ktexteditor/codecompletioninterface.h>

SpecialAreaGroupCompletion::SpecialAreaGroupCompletion()
 : CompletionBase()
{
  m_group = 0L;
}


SpecialAreaGroupCompletion::~SpecialAreaGroupCompletion()
{
}

QList<KTextEditor::CompletionItem>* SpecialAreaGroupCompletion::completionEntries()
{

  QList<KTextEditor::CompletionItem> *completions = new QList<KTextEditor::CompletionItem>();
  if (!m_group)
    return completions;

  KTextEditor::CompletionItem completion;

//FIXME  completion.type = "specialAreaGroupCompletion";

  const GroupElementMap *elementMap = m_base->groupStorage->elementMapForGroup(m_group->name);

  if (!elementMap)
    return completions;

  GroupElementMap::ConstIterator end(elementMap->constEnd());
  for (GroupElementMap::ConstIterator it = elementMap->constBegin(); it != end; ++it)
  {
    QString memberName = it.key();
    if (memberName.startsWith(m_startsWith))
    {
      GroupElementList elementList = it.value();
      GroupElementList::ConstIterator listEnd(elementList.constEnd());
      for (GroupElementList::ConstIterator elementIt = elementList.constBegin(); elementIt != listEnd; ++elementIt)
      {
        GroupElement *groupElement = (*elementIt);
        if (groupElement->parentNode == 0L || groupElement->global)
        {
//FIXME          completion.text = memberName;
          completions->append(completion);
          break; //found one from the list belonging to the memberName, no need to search for others
        } else
        {
          //try to see if local group elements are valid for the current position
          Node *n = m_currentNode;
          while (n && n != groupElement->parentNode)
          {
            n = n->parent;
          }
          if (n == groupElement->parentNode)
          {
//FIXME            completion.text = memberName;
            completions->append(completion);
            break;//found one from the list belonging to the memberName, no need to search for others
          }
        }
      }
    }
  }
/*FIXME
  IncludedGroupElementsMap elements = parser->includedMap;
  IncludedGroupElementsMap::Iterator it2;
  for ( it2 = elements.begin(); it2 != elements.end(); ++it2 )
  {
    QStringList list = it2.data()[group.name].keys();
    list.sort();
    for (uint i = 0; i < list.count(); i++)
    {
      if (list[i].startsWith(word) && list[i] != word)
      {
        completion.text = list[i];
        completions->append( completion );
      }
    }
  }
*/

  return completions;

}

bool SpecialAreaGroupCompletion::isValid(CompletionMode mode)
{
  m_group = 0L;
  switch (mode)
  {
    case Forced:
      return true;
    case Manual:
      {
        //check for group completions
        const DTDStruct *dtd = m_currentNode->tag->dtd();
        int bl, bc;
        m_currentNode->tag->beginPos(bl, bc);
        QString s = m_source->text(bl, bc, m_position.x(), m_position.y());
        int sLength = s.length() - 1;
        //proceed only if we are not inside quotes or comments and before
        //the current cursor position there is something that can belong
        //to a group
        if (!s[sLength].isSpace() && !QuantaCommon::insideCommentsOrQuotes(sLength, s, dtd))
        {
          //find the first separator backwards
          int i = sLength;
          while (i > 0 && (s[i].isLetterOrNumber() || s[i] == '_' ||
                (dtd->minusAllowedInWord && s[i] == '-') ) )
            i--;
          m_startsWith = s.mid(i + 1).trimmed();
          StructTreeGroup group;
          for (int j = 0; j < dtd->structTreeGroups.count(); j++)
          {
            group = dtd->structTreeGroups[j];
            if (!group.autoCompleteAfterRx.pattern().isEmpty() &&
                group.autoCompleteAfterRx.indexIn(s) != -1)
            {
              m_group = &(dtd->structTreeGroups[j]);
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


// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
