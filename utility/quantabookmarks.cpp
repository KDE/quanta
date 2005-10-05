/* This file is part of the KDE libraries
   Copyright (C) 2002, 2003, 2004 Anders Lund <anders.lund@lund.tdcadsl.dk>
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

#include "quantabookmarks.h"

#include <kapplication.h>
#include <kconfig.h>
#include <klocale.h>
#include <kaction.h>
#include <kdebug.h>
#include <kpopupmenu.h>
#include <kstringhandler.h>
#include <ktexteditor/markinterface.h>
#include <ktexteditor/editinterface.h>
#include <ktexteditor/viewcursorinterface.h>

#include <qregexp.h>
#include <qmemarray.h>
#include <qevent.h>

#include "viewmanager.h"
#include "document.h"

/**
   Utility: selection sort
   sort a QMemArray<uint> in ascending order.
   max it the largest (zerobased) index to sort.
   To sort the entire array: ssort( *array, array.size() -1 );
   This is only efficient if ran only once.
*/
static void ssort( QMemArray<uint> &a, int max )
{
  uint tmp, j, maxpos;
  for ( uint h = max; h >= 1; h-- )
  {
    maxpos = 0;
    for ( j = 0; j <= h; j++ )
      maxpos = a[j] > a[maxpos] ? j : maxpos;
    tmp = a[maxpos];
    a[maxpos] = a[h];
    a[h] = tmp;
  }
}

// TODO add a insort() or bubble_sort - more efficient for aboutToShow() ?

QuantaBookmarks::QuantaBookmarks(ViewManager *parent,Sorting sort, bool onlyFromActualDocument )
  : QObject( parent, "bookmarks" )
  , m_sorting(sort)
  , m_onlyFromActualDocument(onlyFromActualDocument)
{
  m_viewManager = parent;
  _tries=0;
  m_bookmarksMenu = 0L;
  m_doc = 0L;
}

QuantaBookmarks::~QuantaBookmarks()
{
}

void QuantaBookmarks::createActions( KActionCollection* ac )
{
  m_bookmarksMenu = (new KActionMenu(i18n("&Bookmarks"), ac, "bookmarks"))->popupMenu();
  init(ac);
}

void QuantaBookmarks::init(KActionCollection* ac)
{
  m_bookmarkToggle = new KToggleAction(
      i18n("Set &Bookmark"), "bookmark", CTRL+Key_B,
  this, SLOT(toggleBookmark()),
  ac, "bookmarks_toggle" );
  m_bookmarkToggle->setWhatsThis(i18n("If a line has no bookmark then add one, otherwise remove it."));
  m_bookmarkToggle->setCheckedState( i18n("Clear &Bookmark") );

  m_bookmarkClear = new KAction(
      i18n("Clear &All Bookmarks"), 0,
  this, SLOT(clearBookmarks()),
  ac, "bookmarks_clear");
  m_bookmarkClear->setWhatsThis(i18n("Remove all bookmarks of the current document."));

  m_goNext = new KAction(
      i18n("Next Bookmark"), "next", ALT + Key_PageDown,
  this, SLOT(goNext()),
  ac, "bookmarks_next");
  m_goNext->setWhatsThis(i18n("Go to the next bookmark."));

  m_goPrevious = new KAction(
      i18n("Previous Bookmark"), "previous", ALT + Key_PageUp,
  this, SLOT(goPrevious()),
  ac, "bookmarks_previous");
  m_goPrevious->setWhatsThis(i18n("Go to the previous bookmark."));

  //connect the aboutToShow() and aboutToHide() signals with
  //the bookmarkMenuAboutToShow() and bookmarkMenuAboutToHide() slots
  connect( m_bookmarksMenu, SIGNAL(aboutToShow()), this, SLOT(bookmarkMenuAboutToShow()));
  connect( m_bookmarksMenu, SIGNAL(aboutToHide()), this, SLOT(bookmarkMenuAboutToHide()) );

  marksChanged ();
}

void QuantaBookmarks::setBookmarksMenu(QPopupMenu* bookmarksMenu)
    
{
  m_bookmarksMenu = bookmarksMenu;
  init();
}

void QuantaBookmarks::toggleBookmark ()
{
  Document *doc = m_doc;
  if (!doc)
    doc = m_viewManager->activeDocument();
  if (doc && doc->markIf)
  {
    uint mark = doc->markIf->mark(doc->viewCursorIf->cursorLine());
    if( mark & KTextEditor::MarkInterface::markType01 )
      doc->markIf->removeMark(doc->viewCursorIf->cursorLine(),
          KTextEditor::MarkInterface::markType01 );
    else
      doc->markIf->addMark(doc->viewCursorIf->cursorLine(),
          KTextEditor::MarkInterface::markType01 );
  }
  marksChanged();
}

void QuantaBookmarks::clearBookmarks ()
{
  Document *doc = m_viewManager->activeDocument();
  if (doc && doc->markIf)
  {
    QPtrList<KTextEditor::Mark> m = doc->markIf->marks();
    for (uint i=0; i < m.count(); i++)
      doc->markIf->removeMark( m.at(i)->line, KTextEditor::MarkInterface::markType01 );
  
    // just to be sure ;)
    marksChanged ();
  }
}

int QuantaBookmarks::insertBookmarks(QPopupMenu& menu, Document *doc, bool insertNavigationItems )
{
  int insertedItems = 0;
  if (doc->markIf)
  {
    uint line = doc->viewCursorIf->cursorLine();
    const QRegExp re("&(?!&)");
    int idx( -1 );
    int old_menu_count = menu.count();
    KTextEditor::Mark *next = 0;
    KTextEditor::Mark *prev = 0;
  
    QPtrList<KTextEditor::Mark> m = doc->markIf->marks();
    QMemArray<uint> sortArray( m.count() );
    QPtrListIterator<KTextEditor::Mark> it( m );
  
    if ( it.count() > 0 && insertNavigationItems)
      menu.insertSeparator();
  
    for( int i = 0; *it; ++it)
    {
      if( (*it)->type & KTextEditor::MarkInterface::markType01 )
      {
        QString bText = KStringHandler::rEmSqueeze
            ( doc->editIf->textLine( (*it)->line ),
                          menu.fontMetrics(), 32 );
        bText.replace(re, "&&"); // kill undesired accellerators!
        bText.replace('\t', ' '); // kill tabs, as they are interpreted as shortcuts
          
        if ( m_sorting == Position )
        {
          sortArray[i] = (*it)->line;
          ssort( sortArray, i );
          idx = sortArray.find( (*it)->line );
          if (insertNavigationItems)
            idx += 3;
          i++;
        }
  
        menu.insertItem(
            QString("%1 - \"%2\"").arg( (*it)->line+1 ).arg( bText ),
             0, (*it)->line, idx );
        insertedItems++;
  
        if ( (*it)->line < line )
        {
          if ( ! prev || prev->line < (*it)->line )
            prev = (*it);
        }
  
        else if ( (*it)->line > line )
        {
          if ( ! next || next->line > (*it)->line )
            next = (*it);
        }
      }
    }
 
    if (insertNavigationItems) 
    {
      idx = ++old_menu_count;
      if ( next )
      {
        m_goNext->setText( i18n("&Next: %1 - \"%2\"").arg( next->line + 1 )
            .arg( KStringHandler::rsqueeze( doc->editIf->textLine( next->line ), 24 ) ) );
        m_goNext->plug( &menu, idx );
        idx++;
      }
      if ( prev )
      {
        m_goPrevious->setText( i18n("&Previous: %1 - \"%2\"").arg(prev->line + 1 )
            .arg( KStringHandler::rsqueeze( doc->editIf->textLine( prev->line ), 24 ) ) );
        m_goPrevious->plug( &menu, idx );
        idx++;
      }
      if ( next || prev )
        menu.insertSeparator( idx );
    }
    connect(&menu, SIGNAL(activated(int)), this, SLOT(gotoLineNumber(int)));
  }
  return insertedItems;
}

void QuantaBookmarks::bookmarkMenuAboutToShow()
{
  KConfig *config = kapp->config();
  if (config->hasGroup("Kate View Defaults"))
  {
    config->setGroup("Kate View Defaults");
    m_sorting = config->readNumEntry("Bookmark Menu Sorting", 0) == 0 ? Position : Creation;
  }
  for (uint i = 0; i < m_othersMenuList.count(); i++)
  {
    delete m_othersMenuList[i];
  }
  m_othersMenuList.clear();
  m_others.clear();
  m_bookmarksMenu->clear();
  marksChanged();
  
  Document *doc = m_doc;
  if (!doc)
    doc = m_viewManager->activeDocument();
  QValueList<Document*> openedDocuments = m_viewManager->openedDocuments();
  if (doc && doc->markIf)
  {
    QPtrList<KTextEditor::Mark> m = doc->markIf->marks();
  
    if (!m_onlyFromActualDocument)
    {
      m_bookmarkToggle->setChecked( doc->markIf->mark( doc->viewCursorIf->cursorLine() )
                                    & KTextEditor::MarkInterface::markType01 );
      m_bookmarkToggle->plug( m_bookmarksMenu );
      m_bookmarkClear->plug( m_bookmarksMenu );
    }
  
    insertBookmarks(*m_bookmarksMenu, doc, !m_onlyFromActualDocument);
    if (openedDocuments.count() > 1 && !m_onlyFromActualDocument)
      m_bookmarksMenu->insertSeparator();
  } 
  if (!m_onlyFromActualDocument)
  {
    int i = 0;
    for (QValueList<Document*>::Iterator it = openedDocuments.begin(); it != openedDocuments.end(); ++it)
    {
      if (*it != doc)
      {
        QPopupMenu *menu = new QPopupMenu(m_bookmarksMenu);
        m_bookmarksMenu->insertItem((*it)->url().fileName(), menu);
        if (insertBookmarks(*menu, *it, false) > 0)
        {        
          m_othersMenuList.append(menu);
          m_others.append(*it);
          i++;
        } else
          delete menu;
      }     
    }
  }
}

/*
   Make sure next/prev actions are plugged, and have a clean text
*/
void QuantaBookmarks::bookmarkMenuAboutToHide()
{
  m_bookmarkToggle->plug( m_bookmarksMenu );
  m_bookmarkClear->plug( m_bookmarksMenu );
  m_goNext->setText( i18n("Next Bookmark") );
  m_goNext->plug( m_bookmarksMenu );
  m_goPrevious->setText( i18n("Previous Bookmark") );
  m_goPrevious->plug( m_bookmarksMenu );
}

void QuantaBookmarks::goNext()
{
  Document *doc = m_doc;
  if (!doc)
    doc = m_viewManager->activeDocument();
  if (doc && doc->markIf)
  {
    QPtrList<KTextEditor::Mark> m = doc->markIf->marks();
    if (m.isEmpty())
      return;
  
    uint line = doc->viewCursorIf->cursorLine();
    int found = -1;
  
    for (uint z=0; z < m.count(); z++)
      if ( (m.at(z)->line > line) && ((found == -1) || (uint(found) > m.at(z)->line)) )
        found = m.at(z)->line;
  
    if (found != -1)
      doc->viewCursorIf->setCursorPositionReal(found, 0);
  }
}

void QuantaBookmarks::goPrevious()
{
  Document *doc = m_doc;
  if (!doc)
    doc = m_viewManager->activeDocument();
  if (doc && doc->markIf)
  {
    QPtrList<KTextEditor::Mark> m = doc->markIf->marks();
    if (m.isEmpty())
      return;
  
    uint line = doc->viewCursorIf->cursorLine();
    int found = -1;
  
    for (uint z=0; z < m.count(); z++)
      if ((m.at(z)->line < line) && ((found == -1) || (uint(found) < m.at(z)->line)))
        found = m.at(z)->line;
  
    if (found != -1)
      doc->viewCursorIf->setCursorPositionReal(found, 0);
  }
}

void QuantaBookmarks::gotoLineNumber(int line)
{
  Document *doc = m_doc;
  if (!doc)
    doc = m_viewManager->activeDocument();
  const QObject *s = sender();
  for (uint i = 0; i < m_othersMenuList.count(); i++)
  {
    if (s == m_othersMenuList[i])
    {
      doc = m_others[i];
      break;
    }
  }
  if (doc)
  {
    if (doc->isUntitled())
    {
       emit gotoFileAndLine("file:" + doc->url().path(), line, 0);
    } else
    {
      emit gotoFileAndLine(doc->url().url(), line, 0);
    }
  }     
}


void QuantaBookmarks::marksChanged ()
{
  Document *doc = m_doc;
  if (!doc)
    doc = m_viewManager->activeDocument();
  if (doc && doc->markIf)
  {
    m_bookmarkClear->setEnabled( !doc->markIf->marks().isEmpty() );
  }
}

#include "quantabookmarks.moc"

// kate: space-indent on; indent-width 2; replace-tabs on;
