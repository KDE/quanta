/* This file is part of the KDE libraries
   Copyright (C) 2002, 2003 Anders Lund <anders.lund@lund.tdcadsl.dk>
   Copyright (C) 2002 John Firebaugh <jfirebaugh@kde.org>
   Copyright (C) 2005 Andras Mantia <amantia@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

/* Extended bookmark manager. Code taken from the Kate part and adapted to
Quanta, so it works over multiple document */

#ifndef QUANTABOOKMARKS_H
#define QUANTABOOKMARKS_H

#include <qobject.h>
#include <qptrlist.h>

namespace KTextEditor { class Mark; }

class ViewManager;
class Document;

class KAction;
class KToggleAction;
class KActionCollection;
class QPopupMenu;
class QMenuData;



class QuantaBookmarks : public QObject
{
  Q_OBJECT

  public:
    enum Sorting { Position, Creation };
    QuantaBookmarks(ViewManager *parent, Sorting sort=Position, bool onlyFromActualDocument = false );
    virtual ~QuantaBookmarks();

    void createActions( KActionCollection* );
    void setBookmarksMenu(QPopupMenu* bookmarksMenu);

    QuantaBookmarks::Sorting sorting() { return m_sorting; };
    void setSorting( Sorting s ) { m_sorting = s; };
    void setDocument(Document *doc) {m_doc = doc;}

  protected:
    int insertBookmarks(QPopupMenu& menu, Document *doc, bool insertNavigationItems = true);
    void init(KActionCollection* ac = 0L);

  private slots:
    void toggleBookmark();
    void clearBookmarks();

    void bookmarkMenuAboutToShow();
    void bookmarkMenuAboutToHide();

    void goNext();
    void goPrevious();
    void gotoLineNumber(int line);

    void marksChanged ();
    
  signals:
    void gotoFileAndLine(const QString&, int, int);

  private:
    KToggleAction*               m_bookmarkToggle;
    KAction*                     m_bookmarkClear;
    KAction*                     m_goNext;
    KAction*                     m_goPrevious;

    Sorting                      m_sorting;
    QPopupMenu*          m_bookmarksMenu;
    QValueList<QPopupMenu*> m_othersMenuList;
    QValueList<Document*> m_others;
    ViewManager*  m_viewManager;
    Document *m_doc;
    bool m_onlyFromActualDocument;

    uint _tries;
};

#endif

// kate: space-indent on; indent-width 2; replace-tabs on;
// vim: noet ts=2
