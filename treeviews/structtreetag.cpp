/***************************************************************************
                          structtreetag.cpp  -  description
                             -------------------
    begin                : Sat Apr 29 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
                           (C) 2002, 2003 Andras Mantia
    email                : pdima@mail.univ.kiev.ua, amantia@freemail.hu
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
#include "structtreetag.h"
#include "../parser/tag.h"
#include "../parser/node.h"
#include "../resource.h"


StructTreeTag::StructTreeTag(QListView *parent, QString a_title)
  : KListViewItem(parent, a_title)
{
  node = 0L;
  hasOpenFileMenu = false;
  groupTag = 0L;
}

StructTreeTag::StructTreeTag(StructTreeTag *parent, Node *a_node, const QString a_title,
                             QListViewItem *after )
: KListViewItem(parent, after, a_title)
{
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
         default:
            {
              title = tag->tagStr().left(70).stripWhiteSpace();
            }


      }
    } else
    {
      title = a_title;
    }

    title.replace(newLineRx," ");
    setText(0, title);
  }
}

StructTreeTag::StructTreeTag(StructTreeTag *parent, QString a_title )
: KListViewItem(parent, a_title)
{
  node = 0L;
  hasOpenFileMenu = false;
  groupTag = 0L;
}


StructTreeTag::~StructTreeTag(){
}
