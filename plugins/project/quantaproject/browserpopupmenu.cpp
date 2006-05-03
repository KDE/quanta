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
#include "hacks.h"

#include <qaction.h>

#include <kiconloader.h>

BrowserPopupMenu::BrowserPopupMenu(const KUrl& base,  bool relativePathInTitle, const QStringList &topList, QWidget *parent)
 : KMenu(parent)
{
  m_base = base;
  m_base.adjustPath(KUrl::AddTrailingSlash);
  m_topList = topList;
  m_relativePathInTitle = relativePathInTitle;
  m_menuBuilt = false;
  connect(this, SIGNAL(aboutToShow()), SLOT(slotAboutToShow()));
  connect(this, SIGNAL(triggered(QAction*)), SLOT(slotItemTriggered(QAction*)));
 }


BrowserPopupMenu::~BrowserPopupMenu()
{
}

void BrowserPopupMenu::buildMenu()
{
  Hack::KMenuAddTitle(this, m_relativePathInTitle ? m_base.fileName() : m_base.path(KUrl::RemoveTrailingSlash));
  QStringList::ConstIterator topEnd = m_topList.constEnd();
  for (QStringList::ConstIterator it = m_topList.constBegin(); it != topEnd; ++it)
  {
    m_actions.append(addAction(*it));
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
      connect(menu, SIGNAL(triggered(QAction*, const KUrl&)), SIGNAL(triggered(QAction*, const KUrl&)));
      QAction *a = addMenu(menu);
      a->setIcon(SmallIconSet("folder"));
      a->setText((*it).left((*it).length()-1));
    }
  }
  m_menuBuilt = true;
}

void BrowserPopupMenu::slotItemTriggered(QAction *action)
{
  if (m_actions.contains(action))
  {
    emit triggered(action, m_base);
  }
}

void BrowserPopupMenu::slotAboutToShow()
{
  if (!m_menuBuilt)
    buildMenu();
}

#include "browserpopupmenu.moc"
