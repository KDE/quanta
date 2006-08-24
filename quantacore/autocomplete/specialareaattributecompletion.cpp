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
#include "specialareaattributecompletion.h"
#include "dtdstruct.h"
#include "editorsource.h"
#include "node.h"
#include "parseresult.h"
#include "qtag.h"
#include "quantacommon.h"
#include "xmlattributevaluecompletion.h"
#include "tag.h"

#include "kdebug.h"

#include <ktexteditor/codecompletioninterface.h>

//TODO: Make it more generic, so we don't need the CSS case

SpecialAreaAttributeCompletion::SpecialAreaAttributeCompletion()
 : CompletionBase()
{
}


SpecialAreaAttributeCompletion::~SpecialAreaAttributeCompletion()
{
}

QList<KTextEditor::CompletionItem>* SpecialAreaAttributeCompletion::completionEntries()
{
  QList<KTextEditor::CompletionItem> *completions = new QList<KTextEditor::CompletionItem>();

  QString textLine = m_tagStr.left(m_attrAutoCompleteAfterCharPosition);
  QString word = QuantaCommon::findWordRev(textLine, m_dtd->minusAllowedInWord);
  QList<QTag *> tags;
  if (!word.isEmpty())
  {
     tags.append(m_base->userTags.value(word.toLower()));
     QHashIterator<QString, QTag*> it(*(m_dtd->tagsList));
     while (it.hasNext())
     {
       it.next();
       if (it.key() == word)
         tags.append(it.value());
     }
  }
  QList<QTag*>::ConstIterator end(tags.constEnd());
  for (QList<QTag*>::ConstIterator it = tags.constBegin(); it != end; ++it)
  {
    QTag *qTag = *it;
    if (!qTag)
      continue;
    QString arguments;
    if (qTag->type == "property") //CSS specific case
    {
      delete completions;
      XmlAttributeValueCompletion *valueCompletion = static_cast<XmlAttributeValueCompletion*>(getCompletionObject(m_base, m_source, m_position, m_currentNode, XmlAttributeValue, Forced, m_plugin));
      valueCompletion->setTagName(qTag->name());
      valueCompletion->setAttributeName(qTag->attributeAt(0)->name);
      return valueCompletion->completionEntries();
      break;
    }
  }
  return completions;
}

CompletionBase::HintResult SpecialAreaAttributeCompletion::completionHint()
{
  HintResult result;

  QString textLine = m_currentNode->tag->tagStr().left(m_attrAutoCompleteAfterCharPosition);
  QString word = QuantaCommon::findWordRev(textLine, m_dtd->minusAllowedInWord);
  QList<QTag *> tags;
  if (!word.isEmpty())
  {
     tags.append(m_base->userTags.value(word.toLower()));
     QHashIterator<QString, QTag*> it(*(m_dtd->tagsList));
     while (it.hasNext())
     {
       it.next();
       if (it.key() == word)
         tags.append(it.value());
     }
  }
  QList<QTag*>::ConstIterator end(tags.constEnd());
  for (QList<QTag*>::ConstIterator it = tags.constBegin(); it != end; ++it)
  {
    QTag *qTag = *it;
    if (!qTag)
      continue;
    QString arguments;
    if (qTag->type != "property") //general case for functions and their arguments
    {
      for (int i =0; i < qTag->attributeCount(); i++)
      {
        Attribute* attr = qTag->attributeAt(i);
        if (attr->status == "optional") 
        {
          arguments = arguments + '[' + attr->type + ' ' + attr->name + "], ";
        } else
        {
          arguments = arguments + attr->type + ' ' + attr->name + ", ";
        }
      }
      arguments = qTag->returnType + ' ' + qTag->name() + '(' + arguments.left(arguments.length()-2) + ')';
      result.argList.append(arguments);
      result.wrapper = "()";
      result.delimiter = m_dtd->attributeSeparator;
     } else //CSS specific case
     {
       arguments = qTag->name() + ": " + qTag->attributeAt(0)->name + ';';
       result.argList.append(arguments);
       result.wrapper = ":;";
       result.delimiter = m_dtd->attributeSeparator;
     }
  }
  return result;
}

void SpecialAreaAttributeCompletion::filterCompletion(KTextEditor::CompletionItem *completion , QString *string)
{
  Q_UNUSED(completion);
  QString s = m_tagStr;
  int i = 0;
  while (i < s.length() && (s[i].isSpace() || s[i] == '\n' || s[i].unicode() == 10 || s[i].isLetterOrNumber() || s[i] == m_dtd->attributeSeparator || (m_dtd->minusAllowedInWord && s[i] == '-'))) //skip the spaces
    i++;
  if (s[i] != m_dtd->tagSeparator)
    string->append(m_dtd->tagSeparator);
}


bool SpecialAreaAttributeCompletion::isValid(CompletionMode mode)
{
  int bl, bc;
  m_currentNode->tag->beginPos(bl, bc);
  QString s = m_source->text(bl, bc, m_position.x(), m_position.y());
  m_attrAutoCompleteAfterCharPosition = s.length() -1;
  m_tagStr = m_currentNode->tag->tagStr();
  switch (mode)
  {
    case Manual:
    case Auto:
      {
        kDebug(24000) << "s=|" << s << "|" << endl;
        if (QuantaCommon::insideCommentsOrQuotes(s.length() -1, s, m_dtd))
          return false; //again, nothing to do
        QString s2 = s;
        int i = s.length() - 1;
        while (i > 0 && s[i].isSpace()) //skip the spaces
          i--;
        while (i > 0 && (s[i].isLetterOrNumber() || s[i] == '_' ||
              (m_dtd->minusAllowedInWord && s[i] == '-') ) ) //find the first word backwards
          i--;
        QString startStr = s.mid(i + 1).trimmed();
        s = s.left(i + 1);
        if (s[i] == m_dtd->attributeSeparator || s[i] == m_dtd->attrAutoCompleteAfter)
        {
          //find the "attrAutoCompleteAfter" character
          while (i > 0 && s[i] != m_dtd->attrAutoCompleteAfter)
            i--;
          s = s.left(i + 1);
          kDebug(24000) << "s=|" << s << "|" << endl;
          kDebug(24000) << "s[i]= " << QString(s[i]) << ";  attrAutoCompleteAfter = " << m_dtd->attrAutoCompleteAfter << "; attributeSeparator=" << m_dtd->attributeSeparator << endl;
          if ( s[i] == m_dtd->attrAutoCompleteAfter)
          {
            m_attrAutoCompleteAfterCharPosition = i;
            return true;
          }
        }
       break;
      }
    case Forced:
      return true;
  }
  return false;
}


// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
