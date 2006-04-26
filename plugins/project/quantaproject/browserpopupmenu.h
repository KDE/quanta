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
#ifndef BROWSERPOPUPMENU_H
#define BROWSERPOPUPMENU_H

#include <qmap.h>

#include <kmenu.h>
#include <kurl.h>
/**
A popup menu acting like a quick browser of a directory.

	@author Andras Mantia <amantia@kde.org>
*/
class BrowserPopupMenu : public KMenu
{
Q_OBJECT
public:
  BrowserPopupMenu(const KUrl& base, bool relativePathInTitle, const QStringList &topList, QWidget *parent=0);
  ~BrowserPopupMenu();
  void buildMenu();

private slots:
  void slotActivated(int id);
  void slotAboutToShow();

signals:
  void itemActivated(int, const KUrl&);

private:
  KUrl m_base;
  QMap<int, BrowserPopupMenu*> m_subMenus;
  bool m_menuBuilt;
  QMap<int, int> m_topIds;
  QStringList m_topList;
  bool m_relativePathInTitle;
};

#endif
