/***************************************************************************
    begin                : Wed Jul 13 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#include "xmlattributevaluecompletion.h"
#include "dtdstruct.h"
#include "editorsource.h"
#include "extfileinfo.h"
#include "groupstorage.h"
#include "node.h"
#include "parseresult.h"
#include "quantaprojectif.h"
#include "tag.h"

#include <QDir>

#include <kurl.h>
#include <ktexteditor/codecompletioninterface.h>


XmlAttributeValueCompletion::XmlAttributeValueCompletion()
 : CompletionBase()
{
}


XmlAttributeValueCompletion::~XmlAttributeValueCompletion()
{
}


QList<KTextEditor::CompletionItem>* XmlAttributeValueCompletion::completionEntries()
{
  QList<KTextEditor::CompletionItem> *completions = new QList<KTextEditor::CompletionItem>();

  KTextEditor::CompletionItem completion;
//FIXME  completion.type = "attributeValueCompletion";

  QStringList values = tagAttributeValues(m_currentNode->tag->dtd(), m_tagName, m_attributeName);
  if (m_attributeName.toLower() == "class")
  {
    if (values.isEmpty())
    {
      static const QRegExp selectorRx("\\.|\\#|\\:");
      const GroupElementMap *elementMap = m_base->groupStorage->elementMapForGroup("Selectors");
      GroupElementMap::ConstIterator it;
      for ( it = elementMap->constBegin(); it != elementMap->constEnd(); ++it )
      {
        QString selectorName = it.key();
        QString tmpStr;
        int index = selectorName.indexOf(selectorRx);
        if (index != -1)
        {
          tmpStr = selectorName.left(index).toLower();
        } else
        {
          tmpStr = selectorName;
        }
        if (tmpStr.isEmpty() || m_tagName.toLower() == tmpStr || tmpStr == "*")
        {
          values << selectorName.mid(index + 1).replace('.',' ');
        }
      }
    }
  } else
  if (m_attributeName.toLower() == "id")
  {
    if (values.isEmpty())
    {
       const GroupElementMap *elementMap = m_base->groupStorage->elementMapForGroup("Selectors");
       GroupElementMap::ConstIterator it;
       for ( it = elementMap->constBegin(); it != elementMap->constEnd(); ++it )
       {
          QString selectorName = it.key();
          if (selectorName.startsWith("#"))
          {
            values << selectorName.mid(1);
          }
       }
    }
  }
  if (!values.isEmpty())
  {
    QStringList::ConstIterator end(values.constEnd());
    for (QStringList::ConstIterator it = values.constBegin(); it != end; ++it )
    {
//FIXME      completion.text = *it;
      if (completion.text().startsWith(m_startsWith))
      {
        completions->append(completion);
      }
    }
  }
  return completions;
}

bool XmlAttributeValueCompletion::isValid(CompletionMode mode)
{
  switch (mode)
  {
    case Forced:
      return true;
    case Manual:
      {
        int index = m_currentNode->tag->valueIndexAtPos(m_position.x(), m_position.y());
        int bLine, bCol;
        if (index != -1)      //inside a value
        {
          m_currentNode->tag->attributeValuePos(index, bLine, bCol);
          m_startsWith = m_currentNode->tag->attributeValue(index).left(m_position.y() - bCol);
          m_tagName = m_currentNode->tag->name();
          m_attributeName = m_currentNode->tag->attribute(index);
          return true;
        }
        break;
      }
    case Auto:
      {
        if (m_currentNode->tag->isType(Tag::XmlTag))
        {
            int index = m_currentNode->tag->attributeIndexAtPos(m_position.x(), m_position.y() - 2);
            if (index != -1)
              return true;
        }
        break;
      }
  }
  return false;
}

QStringList XmlAttributeValueCompletion::tagAttributeValues(const DTDStruct* dtd, const QString& tag, const QString &attribute)
{
  QStringList values;
  if (dtd)
  {
    QString searchForAttr = (dtd->caseSensitive) ? attribute : attribute.toUpper();
    AttributeList* attrs = dtd->tagAttributes(tag);
    if (attrs)
    {
      Attribute *attr;
      KUrl u;
      KUrl base = m_source->url();
      base.setPath(base.directory(KUrl::ObeyTrailingSlash));
      QListIterator<Attribute*> it(*attrs);
      QString s;
      while (it.hasNext())
      {
        attr = it.next();
        QString attrName = (dtd->caseSensitive) ? attr->name : attr->name.toUpper();
        if (attrName == searchForAttr)
        {
          if (attr->type == "url") {
            QuantaProjectIf *project =dynamic_cast<QuantaProjectIf*>( KDevApi::self()->project());
            //FIXME: Port to the new KDevelop API!
            /*            
            if (project)
            {
              KUrl projectBase = project->projectBase();
              values = project->allFiles();
              QStringList::Iterator end = values.end();
              for (QStringList::Iterator it = values.begin(); it != end; ++it)
              {
                //convert the paths to absolute URLs
                u = projectBase;
                u.addPath(*it);
                //convert the paths now to be relative to the current document
                u = KUrl::relativeUrl(base, u);
                (*it) = u.path();
              }
//              values->append("mailto:" + project->email());
          } else */
            {
              QDir dir = QDir(m_source->url().directory());
              values = dir.entryList();
            }
            break;
          } else {
            values = attr->values;
            break;
          }
        }
      }
    }
  }
  return values;
}

