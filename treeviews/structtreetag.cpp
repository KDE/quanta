/***************************************************************************
                          structtreetag.cpp  -  description
                             -------------------
    begin                : Sat Apr 29 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky <pdima@mail.univ.kiev.ua>
                           (C) 2002, 2003 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
//qt include
#include <qregexp.h>

// KDE includes
#include <kiconloader.h>
#include <klocale.h>

// app includes
#include "structtreeview.h"
#include "structtreetag.h"
#include "messageoutput.h"
#include "tag.h"
#include "node.h"
#include "quantacommon.h"
#include "document.h"
#include "resource.h"

StructTreeTag::StructTreeTag(QListView *parent, QString a_title)
  : KListViewItem(parent, a_title)
{
  node = 0L;
  hasOpenFileMenu = false;
  groupTag = 0L;
  parentTree = static_cast<StructTreeView*>(parent);
}

StructTreeTag::StructTreeTag(StructTreeTag *parent, Node *a_node, const QString a_title,
                             QListViewItem *after )
: KListViewItem(parent, after, a_title)
{
  parentTree = parent->parentTree;
  hasOpenFileMenu = false;
  groupTag = 0L;
  static const QString space = " ";
  static const QRegExp nbspRx("&nbsp;|\\n");
  node = a_node;
  if (node)
  {
    Tag *tag = node->tag;
    QString title = tag->name.lower();
    if (a_title.isEmpty())
    {
      switch (tag->type)
      {
        case Tag::XmlTag:
            {
              if (title == "font")
              {
                setPixmap( 0, UserIcon("tag_font_small") );
                if ( tag->attrCount() )
                  title = space + tag->attribute(0) + "=" + tag->attributeValue(0);
                else
                  title = "";
              } else
              if (title == "img")
              {
                setPixmap( 0, SmallIcon("image") );
                title = space + tag->attributeValue("src");

              } else
              if (title == "a")
              {
                setPixmap( 0, SmallIcon("www") );
                if ( tag->hasAttribute("href") )
                  title = space + "href "+ tag->attributeValue("href");
                if ( tag->hasAttribute("name") )
                  title = space + "name "+ tag->attributeValue("name");
              } else
              if ( title == "br")
              {
                  setPixmap( 0, UserIcon("tag_br_small") );
                  title = "";
              } else
              if ( title == "hr")
              {
                setPixmap( 0, UserIcon("tag_hr_small") );
                title = "";
              } else
              if ( title == "li")
              {
                setPixmap( 0, UserIcon("ball") );
                title = "";
              } else
              if ( title == "p")
              {
                setPixmap( 0, UserIcon("tag_p") );
                title = "";
              }

              if (!typingInProgress)
              {
                QTag *parentQTag = 0L;
                if (node->parent)
                  parentQTag = QuantaCommon::tagFromDTD(node->parent);
                QString qTagName = node->tag->dtd()->caseSensitive ? node->tag->name : node->tag->name.upper();
                int line, col;
                node->tag->beginPos(line, col);
                if (parentQTag && !parentQTag->childTags.contains(qTagName) &&
                !parentQTag->childTags.isEmpty())
                {
                  node->tag->write()->setErrorMark(line);
                  QString parentTagName = node->tag->dtd()->caseSensitive ? node->parent->tag->name : node->parent->tag->name.upper();
                  parentTree->showMessage(i18n("Line %1: %2 is not a possible child of %3.\n").arg(line + 1).arg(qTagName).arg(parentTagName));
                }
                QString nextTagName;
                if (node->next)
                {
                  nextTagName = node->tag->dtd()->caseSensitive ? node->next->tag->name : node->next->tag->name.upper();
                }
                parentQTag = QuantaCommon::tagFromDTD(node);
                if (parentQTag && !parentQTag->isSingle() &&
                    !parentQTag->isOptional() &&
                    (!node->next || ( !node->getClosingNode()))  )
                {
                  node->tag->write()->setErrorMark(line);
                  parentTree->showMessage(i18n("Line %1, column %2: Closing tag for %3 is missing.").arg(line + 1).arg(col + 1).arg(qTagName));
                } else
                if (!parentQTag && node->tag->name.upper() != "!DOCTYPE")
                {
                  node->tag->write()->setErrorMark(line);
                  parentTree->showMessage(i18n("Line %1, column %2: %3 is not part of %4.").arg(line + 1).arg(col + 1).arg(qTagName).arg(node->tag->dtd()->nickName));
                }
              }
              break;
            }
        case Tag::Text:
            {
              title = tag->tagStr();
              title = title.left(70).stripWhiteSpace();
              title.replace( nbspRx," ");
              break;
            }
        case Tag::Comment:
            {
              setPixmap( 0, UserIcon("tag_comm") );
              title = tag->tagStr();
              title = title.left(70).stripWhiteSpace();
              title.replace( nbspRx," ");
              break;
            }
         case Tag::ScriptTag:
            {
              title = tag->name;
              break;
            }
         case Tag::Empty:
            {
              title = i18n("Empty tag");
              break;
            }
         case Tag::ScriptStructureBegin:
             {
              title = tag->cleanStr.stripWhiteSpace();
              break;
            }
         default:
            {
              if (!typingInProgress && node->tag->type == Tag::XmlTagEnd && !node->tag->name.isEmpty())
              {
                int line, col;
                node->tag->beginPos(line, col);
                QString qTagName = node->tag->dtd()->caseSensitive ? node->tag->name : node->tag->name.upper();
                QString qPrevTagName;
                if (node->prev)
                 qPrevTagName = node->tag->dtd()->caseSensitive ? node->prev->tag->name : node->prev->tag->name.upper();
                if (!node->prev || qTagName != "/" + qPrevTagName)
                {
                  node->tag->write()->setErrorMark(line);
                  parentTree->showMessage(i18n("Line %1, column %2: Opening tag for %3 is missing.").arg(line + 1).arg(col + 1).arg(qTagName));
                }
              }
              title = tag->tagStr().left(70).stripWhiteSpace();
            }


      }
    } else
    {
      title = a_title;
    }

    title.replace(newLineRx," ");
    setText(0, title);
    node->listItems.append(this);
  }
}

StructTreeTag::StructTreeTag(StructTreeTag *parent, QString a_title )
: KListViewItem(parent, a_title)
{
  node = 0L;
  hasOpenFileMenu = false;
  groupTag = 0L;
  parentTree = parent->parentTree;
}


StructTreeTag::~StructTreeTag(){
  if (node)
    node->listItems.remove(node->listItems.find(this));
}
