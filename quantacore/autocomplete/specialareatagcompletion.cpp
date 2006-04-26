/***************************************************************************
    begin                : Sun Jul 17 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#include "specialareatagcompletion.h"
#include "dtdstruct.h"
#include "editorsource.h"
#include "groupelement.h"
#include "groupstorage.h"
#include "node.h"
#include "parseresult.h"
#include "sagroupparser.h"
#include "quantacommon.h"
#include "qtag.h"
#include "tag.h"

#include <kdebug.h>
#include <ktexteditor/codecompletioninterface.h>

SpecialAreaTagCompletion::SpecialAreaTagCompletion()
 : CompletionBase()
{
}


SpecialAreaTagCompletion::~SpecialAreaTagCompletion()
{
}


QList<KTextEditor::CompletionItem> *SpecialAreaTagCompletion::completionEntries()
{
  QList<KTextEditor::CompletionItem> *completions = new QList<KTextEditor::CompletionItem>();
  KTextEditor::CompletionItem completion;
//FIXME  completion.type = "specialAreaTagCompletion";

  QTag *parentQTag= 0L;
  if (m_currentNode && m_currentNode->parent)
      parentQTag = QTag::tagFromDTD(m_currentNode->parent);
  QString textLine = m_source->textLine(m_position.x()).left(m_position.y() + 1);

//if the DTEP supports the notion of classes and objects, use autocompletion for them as well
  QString classStr = "";
  QString objStr;
  if (m_dtd->classGroupIndex != -1 && m_dtd->objectGroupIndex != -1)
  {
    QString word = QuantaCommon::findWordRev(textLine, m_dtd->minusAllowedInWord).toUpper();
    textLine = textLine.left(textLine.length() - word.length());
    int pos = m_dtd->memberAutoCompleteAfter.lastIndexIn(textLine);
    if (pos != -1)
    {
      textLine = textLine.left(pos);
      QRegExp r = m_dtd->structTreeGroups[m_dtd->classGroupIndex].usageRx;
      pos = r.lastIndexIn(textLine);
      if (pos != -1)
      {
        objStr = r.cap(1);
        if (objStr == "this")
        {
          QString parentGroupStr = "";
          bool classFound = false;
          Node *n = m_currentNode;
          while (n && !classFound)
          {
            //Need to parser for groups, as the node tree is rebuilt before
            //autocompletion and none of the node has links to group elements
            //at this position.
            SAGroupParser *gParser = new SAGroupParser(0L, m_base, m_source, n, n->nextSibling(), true, false, false);
            gParser->slotParseForScriptGroup();
            GroupElementList nodeElements = m_base->groupStorage->elementsForNode(n);
            GroupElementList::ConstIterator it = nodeElements.constBegin();
            GroupElementList::ConstIterator end = nodeElements.constEnd();
            while (it != end)
            {
              GroupElement *e = *it;
              if (parentGroupStr.isEmpty() && e->group->appendToTags)
              {
                parentGroupStr = e->group->parentGroup;
              }
              if (!parentGroupStr.isEmpty() && e->group->name == parentGroupStr)
              {
                classStr = e->tag->name();
                classFound = true;
              }
              ++it;
            }
            //detach the created groupelements
            m_base->groupStorage->detachNode(n);
            delete gParser;
            n = n->parent;
          }
        } else
        {
          GroupElementList groupElementList =  m_base->groupStorage->elementsFromGroup( m_dtd->structTreeGroups[m_dtd->objectGroupIndex].name, objStr);
          GroupElementList::ConstIterator end = groupElementList.constEnd();
          for (GroupElementList::ConstIterator it = groupElementList.constBegin(); it != end; ++it)
          {
            if (!(*it)->tag)
              continue;
#ifdef DEBUG_PARSER
            kDebug(24000) << "GroupElement: " << (*it) << " " << (*it)->tag->area().bLine() << " " << (*it)->tag->area().bCol() << " "<< (*it)->tag->area().eLine() << " "<< (*it)->tag->area().eCol() << " " << (*it)->tag->tagStr() << " " << (*it)->type << endl;
#endif
            if (!(*it)->type.isEmpty())
            {
              classStr = (*it)->type;
              break;
            }
          }
        }
      }
    }
    if (!objStr.isEmpty()  && classStr.isEmpty()) //the class cannot be identified for the object or there is no object.
      return completions;
  }
  QStringList tagNameList;
  QMap<QString, QString> comments;
  QString tagName;
  QHashIterator<QString, QTag*> it(*(m_dtd->tagsList));
  int i = 0;
  while (it.hasNext())
  {
    it.next();
    QTag *tag = it.value();
    if (tag->className == classStr || isDerivatedFrom(classStr, tag->className))
    {
      tagName = tag->name();
      if (!tagName.isEmpty() && tagName.startsWith(m_startsWith, Qt::CaseInsensitive))
      {
          tagName = tag->name() + QString("%1").arg(i, 10);
          tagNameList += tagName;
          comments.insert(tagName, tag->comment);
          i++;
      }
    }
  }

  QHashIterator<QString, QTag*> it2(m_base->userTags);
  while (it2.hasNext())
  {
    it2.next();
    QTag *tag = it2.value();
    if ((tag->className == classStr ||
         isDerivatedFrom(classStr, tag->className)) &&
         tag->name().startsWith(m_startsWith, Qt::CaseInsensitive))
    {
      tagName = tag->name() + QString("%1").arg(i, 10);
      tagNameList += tagName;
      comments.insert(tagName, tag->comment);
      i++;
    }
  }

  tagNameList.sort();
  for (int i = 0; i < tagNameList.count(); i++)
  {
//FIXME    completion.text = tagNameList[i];
//FIXME    completion.text = completion.text.left(completion.text.length() - 10);
//FIXME    completion.comment = comments[tagNameList[i]];
    completions->append( completion );
  }

  return completions;
}

void SpecialAreaTagCompletion::filterCompletion(KTextEditor::CompletionItem *completion , QString *string)
{
  Q_UNUSED(completion);
  string->append(m_dtd->attrAutoCompleteAfter);
}

CompletionBase::CompletionRequest SpecialAreaTagCompletion::completionDone(const KTextEditor::CompletionItem& completion)
{
  Q_UNUSED(completion);
  return RequestHint;
}

bool SpecialAreaTagCompletion::isValid(CompletionMode mode)
{
  switch (mode)
  {
    case Forced:
      return true;
    case Manual:
    case Auto:
      {
        if (m_dtd->tagAutoCompleteAfter == '\1')
          return true;
        int bl, bc;
        m_currentNode->tag->beginPos(bl, bc);
        QString s = m_source->text(bl, bc, m_position.x(), m_position.y());
        kDebug(24000) << "s=|" << s << "|" << endl;
        if (QuantaCommon::insideCommentsOrQuotes(s.length() -1, s, m_dtd))
          return false; //again, nothing to do
        QString s2 = s;
        int i = s.length() - 1;
        while (i > 0 && s[i].isSpace()) //skip the spaces
          i--;
        s = s.left(i + 1);
        if (!m_dtd->memberAutoCompleteAfter.pattern().isEmpty() &&
             m_dtd->memberAutoCompleteAfter.lastIndexIn(s))
          return true;
        if (s[i] == m_dtd->tagSeparator || s[i] == m_dtd->tagAutoCompleteAfter)
        {
          while (i > 0 && s[i] != m_dtd->tagAutoCompleteAfter)
            i--;
          s = s.left(i + 1);
          return true;
        }
        break;
      }
  }
  return false;
}

bool SpecialAreaTagCompletion::isDerivatedFrom(const QString& className, const QString &baseClass)
{
  if (className.isEmpty() || !m_dtd->classInheritance.contains(className))
    return false;

  QString parentClass = m_dtd->classInheritance[className];
  int result = 0;
  do {
    if (parentClass == baseClass)
      result = 1; //className extends baseClass
    else
    {
      if (m_dtd->classInheritance.contains(parentClass))
        parentClass = m_dtd->classInheritance[parentClass];
      else
        result = -1;//nothing was found in the inheritance list
    }
  } while (result == 0);

  return (result == 1);
}


// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
