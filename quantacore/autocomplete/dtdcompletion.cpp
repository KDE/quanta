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
#include "dtdcompletion.h"
#include "dtds.h"
#include "dtdstruct.h"
#include "node.h"
#include "tag.h"
#include "tagattr.h"

#include <ktexteditor/codecompletioninterface.h>

DTDCompletion::DTDCompletion()
 : CompletionBase()
{
}


DTDCompletion::~DTDCompletion()
{
}


QList<KTextEditor::CompletionItem>* DTDCompletion::completionEntries()
{
  QList<KTextEditor::CompletionItem> *completions = new QList<KTextEditor::CompletionItem>();
  KTextEditor::CompletionItem completion;
  QList<KTextEditor::CompletionItem> tempCompletions;
  QStringList nameList;
  QPoint begin = m_currentNode->tag->beginPos();
  QString tagStr = m_currentNode->tag->tagStr();
  if (tagStr.startsWith("<!"))
    tagStr = tagStr.mid(2);
  if (tagStr.startsWith("doctype", false) && tagStr.mid(7, m_position.y() - begin.y() -7).trimmed().isEmpty()) //list all the known document types
  {
    QStringList nickNames = DTDs::ref()->nickNameList(true);
    QStringList::Iterator end(nickNames.end());
    for ( QStringList::Iterator it = nickNames.begin(); it != end; ++it )
    {
//FIXME      completion.type = "doctypeCompletion";
//FIXME      completion.text = *it;
      tempCompletions.append(completion);
      nameList.append(completion.text());
    }
  }
  nameList.sort();
  QStringList::Iterator end(nameList.end());
  for (QStringList::Iterator it = nameList.begin(); it != end; ++it)
  {
    QList<KTextEditor::CompletionItem>::Iterator endCompl(tempCompletions.end());
    for (QList<KTextEditor::CompletionItem>::Iterator compIt = tempCompletions.begin(); compIt != endCompl; ++compIt)
    {
      if ((*compIt).text() == *it)
      {
        completions->append(*compIt);
        break;
      }
    }
  }

  return completions;
}

void DTDCompletion::filterCompletion(KTextEditor::CompletionItem *completion , QString *string)
{
  const DTDStruct *dtd = DTDs::ref()->find(DTDs::ref()->getDTDNameFromNickName(completion->text()));
  if (dtd && !dtd->url.isEmpty())
  {
    string->remove(0, string->length()); //clean the string
    string->append(dtd->doctypeStr);
  }
}

bool DTDCompletion::isValid(CompletionMode mode)
{
  Q_UNUSED(mode);
  return true;
}
