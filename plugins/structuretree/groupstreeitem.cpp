/***************************************************************************
    begin                : Mon Jul 18 2005
    copyright            : (C) 2005 by Jens Herden <jens@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#include "groupstreeitem.h"
#include "groupswidget.h"
#include "areastruct.h"
#include "tag.h"
#include "groupstorage.h"
#include "groupelement.h"
#include "settings.h"
#include "extfileinfo.h"


#include <kurl.h>
#include <kdebug.h>

GroupsTreeItem::GroupsTreeItem(GroupsWidget *parent, const GroupElementMap *groupElements, GroupsTreeItem *after)
  : K3ListViewItem(parent, after), m_tag(0), m_groupElements(groupElements)
{
  setVisible(shouldBeVisible());
}

GroupsTreeItem::GroupsTreeItem(GroupsTreeItem *parent, const GroupElementMap *groupElements, const Tag *tag, GroupsTreeItem *after)
  : K3ListViewItem(parent, after), m_tag(tag), m_groupElements(groupElements)
{
}

GroupsTreeItem::~GroupsTreeItem()
{
}


void GroupsTreeItem::populate()
{
  // do we have already populated before?
  if (firstChild())
    return;

  if (!m_tag)  // I am a branch root
    addElements();
  else
    addMoreElements();
}


void GroupsTreeItem::addMoreElements()
{
  GroupElementMap::ConstIterator it = m_groupElements->find(m_tag->name());
  GroupElementList::ConstIterator listIt(it.value().constBegin());
  GroupElementList::ConstIterator end(it.value().constEnd());
  ++listIt;  // skip the first one, it is one level up
  GroupsTreeItem *item = 0;
  for ( ; listIt != end; ++listIt)
  {
    Tag *tag = (*listIt)->tag;
    if (item)
      item = new GroupsTreeItem(this, m_groupElements, tag, item);
    else
      item = new GroupsTreeItem(this, m_groupElements, tag);

    AreaStruct a = tag->area();
    item->setText(0, QString("(%1/%2)").arg(a.start.x()).arg(a.start.y()));
  }
}

void GroupsTreeItem::addElements()
{
  GroupsTreeItem *item = 0;
  GroupElementMap::ConstIterator end(m_groupElements->constEnd());
  for (GroupElementMap::ConstIterator it = m_groupElements->constBegin(); it != end; ++it)
  {
    // get the Tag of the first entry in the list, the first is always there
    Tag *tag = it.data().front()->tag;
    if (item)
      item = new GroupsTreeItem(this, m_groupElements, tag, item);
    else
      item = new GroupsTreeItem(this, m_groupElements, tag);

    AreaStruct a = tag->area();
    item->setText(0, it.key() + QString(" (%1/%2)").arg(a.start.x()).arg(a.start.y()));
    if (it.value().count() > 1)
      item->setExpandable(true);
  }
}

AreaStruct GroupsTreeItem::tagArea(bool adjustForXML) const
{
  AreaStruct area;
  if (m_tag)
  {
    area = m_tag->area();
    if (adjustForXML && (m_tag->isType(Tag::XmlTag) || m_tag->isType(Tag::XmlTagEnd)))
    {
      ++(area.start.ry()); //position the cursor inside the tag
    }
  }
  return area;
}


void GroupsTreeItem::settingsChanged()
{
  setVisible(shouldBeVisible());

  GroupsTreeItem *item = dynamic_cast<GroupsTreeItem *>(firstChild());
  while (item)
  {
    item->settingsChanged();
    item = dynamic_cast<GroupsTreeItem *>(item->nextSibling());
  }
}

bool GroupsTreeItem::shouldBeVisible() const
{
  // the root has no tag, all other should be visible
  if (m_tag)
    return true;

  bool v = (Settings::self()->showWhitespace()) || (m_groupElements && m_groupElements->count() > 0);

  return v;
}


KUrl GroupsTreeItem::getURL() const
{
  if (m_tag && m_groupElements)
  {
    QString path = m_tag->name();
    const GroupElementList *list = &(m_groupElements->constBegin().data());
    XMLStructGroup * group = list->front()->group;
    if (group && group->hasFileName)
    {
      path.remove(group->fileNameRx);
      KUrl url;
      ExtFileInfo::setUrl(url, path);
      return url;
    }
  }
  return KUrl();
}


// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
