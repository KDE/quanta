/***************************************************************************

    begin                : June 20 2005
    copyright            : (C) 2005 Andras Mantia <amantia@kde.org>
                           (C) 2005 Jens Herden <jens@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// app includes
#include "structtreeitem.h"
#include "structuretreewidget.h"
#include "node.h"
#include "tag.h"
#include "areastruct.h"
#include "settings.h"

//qt include
#include <QRegExp>
#include <QPixmap>
#include <qpoint.h>

// KDE includes
#include <kiconloader.h>
#include <klocale.h>
#include <kdebug.h>


StructTreeItem::StructTreeItem(StructTreeItem *parent, const Node *node, StructTreeItem *after)
  : K3ListViewItem(parent, after), FilterableItemIf(), m_node(node), m_tag(node ? node->tag : 0)
{
  initItem();
}

StructTreeItem::StructTreeItem(StructureTreeWidget *parent, const Node *node, StructTreeItem *after)
  : K3ListViewItem(parent, after), m_node(node), m_tag(node ? node->tag : 0)
{
  initItem();
}


void StructTreeItem::initItem()
{
  static const QString space = " ";
  static const QRegExp nbspRx("&nbsp;|\\n");
  if (m_node)
  {
    setExpandable(m_node->child);
    QString title = m_tag->name();
    if (m_tag->isClosingTag())
      title.prepend("/");
/*    if (title.isEmpty())
    {*/
      switch (m_tag->type())
      {
        case Tag::XmlTag:
            {
              QString titleUpper = title.toUpper();
              if (titleUpper == "FONT")
              {
                setPixmap( 0, UserIcon("tag_font_small") );
                if ( m_tag->attrCount() )
                  title = space + m_tag->attribute(0) + "=" + m_tag->attributeValue(0);
                else
                  title = "";
              } else
              if (titleUpper == "IMG")
              {
                setPixmap( 0, SmallIcon("image") );
                title = space + m_tag->attributeValue("src");

              } else
              if (titleUpper == "A")
              {
                setPixmap( 0, SmallIcon("www") );
                if ( m_tag->hasAttribute("href") )
                  title = " href " + m_tag->attributeValue("href");
                if ( m_tag->hasAttribute("name") )
                  title = " name " + m_tag->attributeValue("name");
              } else
              if (titleUpper == "BR")
              {
                  setPixmap( 0, UserIcon("tag_br_small") );
                  title = "";
              } else
              if (titleUpper == "HR")
              {
                setPixmap( 0, UserIcon("tag_hr_small") );
                title = "";
              } else
              if (titleUpper == "LI")
              {
                setPixmap( 0, UserIcon("ball") );
                title = "";
              } else
              if (titleUpper == "P")
              {
                setPixmap( 0, UserIcon("tag_p") );
                title = "";
              }
/*  FIXME this belongs to another place
              if (!typingInProgress)
              {
                QTag *parentQTag = 0L;
                if (m_node->parent)
                  parentQTag = QuantaCommon::tagFromDTD(m_node->parent);
                QString qTagName = m_node->tag->dtd()->caseSensitive ? m_node->tag->name : m_node->tag->name.upper();
                int line, col;
                m_node->tag->beginPos(line, col);
                if (parentQTag && !parentQTag->childTags.contains(qTagName) &&
                !parentQTag->childTags.isEmpty())
                {
                  m_node->tag->write()->setErrorMark(line);
                  QString parentTagName = m_node->tag->dtd()->caseSensitive ? m_node->parent->tag->name : m_node->parent->tag->name.upper();
                  parentTree->showMessage(i18n("Line %1: %2 is not a possible child of %3.\n").arg(line + 1).arg(qTagName).arg(parentTagName));
                }
                QString nextTagName;
                if (m_node->next)
                {
                  nextTagName = m_node->tag->dtd()->caseSensitive ? m_node->next->tag->name : m_node->next->tag->name.upper();
                }
                parentQTag = QuantaCommon::tagFromDTD(m_node);
                if (parentQTag && !parentQTag->isSingle() &&
                    !parentQTag->isOptional() &&
                    (!m_node->next || ( !m_node->getClosingNode()))  )
                {
                  m_node->tag->write()->setErrorMark(line);
                  parentTree->showMessage(i18n("Line %1, column %2: Closing tag for %3 is missing.").arg(line + 1).arg(col + 1).arg(qTagName));
                } else
                if (!parentQTag && m_node->tag->name.upper() != "!DOCTYPE")
                {
                  m_node->tag->write()->setErrorMark(line);
                  parentTree->showMessage(i18n("Line %1, column %2: %3 is not part of %4.").arg(line + 1).arg(col + 1).arg(qTagName).arg(m_node->tag->dtd()->nickName));
                }
              }*/
              break;
            }
        case Tag::Text:
            {
              title = m_tag->tagStr();
              title = title.left(70).trimmed();
              title.replace(nbspRx, space);
              break;
            }
        case Tag::ScriptTag:
            {
              if (m_tag->name() != "[COMMENT]")
              {
                title = m_tag->name();
                break;
              }
              //      fall through to comment !!
            }
        case Tag::Comment:
        {
          setPixmap( 0, UserIcon("tag_comm") );
          title = m_tag->tagStr();
          title = title.left(70).trimmed();
          title.replace(nbspRx, space);
          break;
        }
        case Tag::Empty:
            {
              title = i18n("Whitespace");
              break;
            }
         case Tag::ScriptStructureBegin:
             {
              title = m_tag->cleanStr.trimmed();
              break;
            }
         default:
            {
/*FIXME this belongs to another place
              if (!typingInProgress && m_node->tag->type == Tag::XmlTagEnd && !m_node->tag->name.isEmpty())
              {
                int line, col;
                m_node->tag->beginPos(line, col);
                QString qTagName = m_node->tag->dtd()->caseSensitive ? m_node->tag->name : m_node->tag->name.upper();
                QString qPrevTagName;
stripWhiteSpace                if (m_node->prev)
                 qPrevTagName = m_node->tag->dtd()->caseSensitive ? m_node->prev->tag->name : m_node->prev->tag->name.upper();
                if (!m_node->prev || qTagName != "/" + qPrevTagName)
                {
                  m_node->tag->write()->setErrorMark(line);
                  parentTree->showMessage(i18n("Line %1, column %2: Opening tag for %3 is missing.").arg(line + 1).arg(col + 1).arg(qTagName));
                }
              }*/
              title = m_tag->tagStr().left(70).trimmed();
            }
      }
//     }
    title.replace("\n", space);
    setText(0, title);
    setVisible(shouldBeVisible());

//     kDebug(24000) << isVisible() << m_node->opened << endl;
    if (isVisible() && m_node->opened)
    {
      setOpen(true);
    }
  } // if m_node
}


StructTreeItem::~StructTreeItem()
{
}

void StructTreeItem::populateAll()
{
  populate();
  StructTreeItem * item = static_cast<StructTreeItem *>(firstChild());
  if (item)
    item->populateAll();

  item = static_cast<StructTreeItem *>(nextSibling());
  if (item)
    item->populateAll();
}


void StructTreeItem::populate()
{
  // do we have already populated before?
  if (firstChild())
    return;

  Node *node = m_node->child;
  if (!node)
    return;

//   kDebug(24000) << "populate() " << text(0) << endl;

  StructTreeItem *item = new StructTreeItem(this, node);
  setOpen(true);
  node = node->next;
  while (node)
  {
    item = new StructTreeItem(this, node, item);
    node = node->next;
  }
}

bool StructTreeItem::shouldBeVisible() const
{
  // the root has a fake node without a tag
  if (! m_tag)
    return true;

  bool v = ((!m_tag->isType(Tag::Empty)) || Settings::self()->showWhitespace()) &&
           (((!m_tag->isType(Tag::XmlTagEnd)) && (!m_tag->isType(Tag::ScriptStructureEnd))) || Settings::self()->showClosingTag());

//   kDebug(24000) << v << endl;

  return v;
}

void StructTreeItem::settingsChanged()
{
  setVisible(shouldBeVisible());
  StructTreeItem *item = dynamic_cast<StructTreeItem *>(firstChild());
  while (item)
  {
    item->settingsChanged();
    item = dynamic_cast<StructTreeItem *>(item->nextSibling());
  }
}


void StructTreeItem::setSubTreeOpen(bool open)
{
  if (m_node->child)
  {
    setOpen(open);
    StructTreeItem * item = dynamic_cast<StructTreeItem *>(firstChild());
    if (item)
      item->openSubTree(open);
  }
}


void StructTreeItem::openSubTree(bool open)
{
  setOpen(open);

  StructTreeItem * item = dynamic_cast<StructTreeItem *>(nextSibling());
  if (item)
    item->openSubTree(open);

  item = dynamic_cast<StructTreeItem *>(firstChild());
  if (item)
    item->openSubTree(open);
}


AreaStruct StructTreeItem::tagArea(bool adjustForXML, bool excludeClosing) const
{
  AreaStruct area;
  if (m_node && m_tag)
  {
    area = m_node->tagArea(excludeClosing);
    if (adjustForXML && (m_tag->isType(Tag::XmlTag) || m_tag->isType(Tag::XmlTagEnd)))
    {
      ++(area.start.ry()); //position the cursor inside the tag
    }
  }
  return area;
}


void StructTreeItem::setOpen(bool open)
{
  K3ListViewItem::setOpen(open);
  m_node->opened = open;
//    kDebug(24000) << "============setOpen " << open << endl;
}

StructTreeItem * StructTreeItem::makeNodeVisible(const QPoint & qp)
{
 // kDebug(24000) << "Make item visible: " << text(0) << endl;
  if (! m_tag)
    return 0;

  switch (AreaStruct::isBetween(qp, m_tag->area()))
  {
    case -1: // cursor is before the Tag
      return 0;
    case 0: // cursor is inside the Tag
    {
      populate();
      StructTreeItem * item = static_cast<StructTreeItem *>(firstChild());
      if (item)
      {
        item = item->makeNodeVisible(qp);
        if (item)
          return item;
      }
      listView()->setCurrentItem(this);
      return this;
    }
    case +1: // cursor is after the Tag
    {
      populate();
      StructTreeItem * item = static_cast<StructTreeItem *>(firstChild());
      if (item)
      {
        item = item->makeNodeVisible(qp);
        if (item)
          return item;
      }
      item = static_cast<StructTreeItem *>(nextSibling());
      if (item)
        item = item->makeNodeVisible(qp);

      return item;
    }
  } // switch

  return 0; // this should never happen!
}


bool StructTreeItem::filter(const QString &s) const
{
  StructureTreeWidget * view = dynamic_cast<StructureTreeWidget *>(listView());
  if (view)
    view->populate();

  return (shouldBeVisible() && text(0).startsWith(s, Qt::CaseInsensitive));
}


// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
