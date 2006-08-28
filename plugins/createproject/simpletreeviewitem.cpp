/***************************************************************************
    begin                : Fri Jul 22 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#include "simpletreeviewitem.h"


#include <kiconloader.h>
#include <kmimetype.h>
#include <kurl.h>

SimpleTreeViewItem::SimpleTreeViewItem(const KUrl& url, const QString& treePath, K3ListView* parent)
 : K3ListViewItem(parent)
{
  init(url, treePath);
}

SimpleTreeViewItem::SimpleTreeViewItem(const KUrl& url, const QString& treePath, K3ListViewItem* parent, K3ListViewItem* after)
 : K3ListViewItem(parent, after)
{
  init(url, treePath);
}


SimpleTreeViewItem::~SimpleTreeViewItem()
{
}

void SimpleTreeViewItem::init(const KUrl& url, const QString& treePath)
{
  setText(0, url.fileName());
  m_url = url;
  m_treePath = treePath;
  if (url.path().endsWith("/"))
    setPixmap(0, SmallIcon("folder"));
  else
    setPixmap(0, SmallIcon(KMimeType::iconNameForUrl(url)));
}

QString SimpleTreeViewItem::key(int column, bool ascending) const
{
  bool isDir = m_url.path().endsWith("/");
  if (ascending)
    if (isDir)
      return text(column).prepend(" ");
    else
      return text(column);
  else
    if (isDir)
      return text(column);
    else
      return text(column).prepend(" ");
}

int SimpleTreeViewItem::compare(Q3ListViewItem *i, int col,
                              bool ascending) const
{
  return key(col, ascending).compare(i->key(col, ascending));
}
