/***************************************************************************
    begin                : Thu Jul 21 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#include "browserpopupmenu.h"
#include "extfileinfo.h"

#
#include <kiconloader.h>

BrowserPopupMenu::BrowserPopupMenu(const KUrl& base,  bool relativePathInTitle, const QStringList &topList, QWidget *parent)
 : KMenu(parent)
{
  m_base = base;
  m_base.adjustPath(1);
  m_topList = topList;
  m_relativePathInTitle = relativePathInTitle;
  m_menuBuilt = false;
  connect(this, SIGNAL(aboutToShow()), SLOT(slotAboutToShow()));
  connect(this, SIGNAL(activated(int)), SLOT(slotActivated(int)));
 }


BrowserPopupMenu::~BrowserPopupMenu()
{
}

void BrowserPopupMenu::buildMenu()
{
//FIXME Rewrite with QAction:  insertTitle(m_relativePathInTitle ? m_base.fileName() : m_base.path(-1));
  QStringList::ConstIterator topEnd = m_topList.constEnd();
  int i = 1;
  for (QStringList::ConstIterator it = m_topList.constBegin(); it != topEnd; ++it)
  {
    m_topIds[insertItem(*it)] = i++;
  }
  addSeparator();
  QStringList entries = ExtFileInfo::listDirRelative(m_base, "*");
  QStringList::ConstIterator end = entries.constEnd();
  for (QStringList::ConstIterator it = entries.constBegin(); it != end; ++it)
  {
    if ((*it).endsWith("/"))
    {
      KUrl u = m_base;
      u.addPath(*it);
      BrowserPopupMenu *menu = new BrowserPopupMenu(u, m_relativePathInTitle, m_topList, this);
      connect(menu, SIGNAL(itemActivated(int, const KUrl&)), SIGNAL(itemActivated(int, const KUrl&)));
      int id = insertItem(SmallIconSet("folder"), (*it).left((*it).length()-1), menu);
      m_subMenus[id] = menu;
    }
  }
  m_menuBuilt = true;
}

void BrowserPopupMenu::slotActivated(int id)
{
  if (m_topIds.contains(id))
  {
    emit itemActivated(m_topIds[id], m_base);
  }
}

void BrowserPopupMenu::slotAboutToShow()
{
  if (!m_menuBuilt)
    buildMenu();
}

#include "browserpopupmenu.moc"
