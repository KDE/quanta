/***************************************************************************
                          quantaview.cpp  -  description
                             -------------------
    begin                : Втр Май  9 13:29:57 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
    email                : pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// include files for Qt
#include <qprinter.h>
#include <qpainter.h>
#include <qtabbar.h>
#include <qtabwidget.h>
#include <qlayout.h>
#include <qwidgetstack.h>
#include <qdom.h>
#include <qfile.h>

// include files for KDE
#include <klocale.h>
#include <kstddirs.h>
#include <kmenubar.h>
#include <kiconloader.h>

// application specific includes
#include "document.h"
#include "resource.h"
#include "quantaview.h"
#include "quantadoc.h"
#include "quanta.h"


#include "widgets/wtoolbar.h"
#include "kwrite/kwview.h"
#include "kwrite/highlight/highlight.h"

#include <kaction.h>
#include "toolbar/tagaction.h"

QuantaView::QuantaView( QuantaApp *app, QWidget *parent, const char *name )
	: QWidget( parent, name)
{
	this->app = app;
	this->doc = app->doc;
	
	initActions();
	
	QGridLayout *layout = new QGridLayout( this );
	
  tabBar = new QTabBar( this);
  toolbarStack = new QWidgetStack(this,"ToolBar stack");

	tabBar 			-> setFocusPolicy(QWidget::NoFocus);
	toolbarStack-> setFocusPolicy(QWidget::NoFocus);
#ifdef USE_KDOCKTABGROUP
   writeTab = new KDockTabGroup(this);
   connect( writeTab,	SIGNAL(pageSelected (QWidget*)), app, SLOT(slotUpdateStatus(QWidget*)));  connect( tabBar,		SIGNAL(selected(int)), toolbarStack, SLOT(raiseWidget(int)));
   connect( writeTab,	SIGNAL(pageSelected (QWidget*)), app, SLOT(reparse()));
#else
  writeTab = new QTabWidget(this);
  writeTab ->setTabPosition( QTabWidget::Bottom );
//  connect( writeTab,	SIGNAL(selected(const QString &)), app, SLOT(slotUpdateStatus(const QString &)));
  connect( writeTab,	SIGNAL(currentChanged(QWidget*)), app, SLOT(slotUpdateStatus(QWidget*)));
  connect( writeTab,	SIGNAL(selected(const QString &)), app, SLOT(reparse()));
#endif
  writeTab ->setFocusPolicy( QWidget::NoFocus );
             
  layout->addWidget( tabBar       ,0,0);
  layout->addWidget( toolbarStack ,1,0);
  layout->addWidget( writeTab     ,2,0);
  connect( tabBar,SIGNAL(selected(int)), toolbarStack, SLOT(raiseWidget(int)));

  writeTab->show();
}

QuantaView::~QuantaView()
{
}

void QuantaView::print(QPrinter *pPrinter)
{
  QPainter printpainter;
  printpainter.begin(pPrinter);
	
  // TODO: add your printing code here

  printpainter.end();
}

/** return current KWrite class */
Document* QuantaView::write()
{
#ifdef USE_KDOCKTABGROUP
  return ( Document* )writeTab->visiblePage();
#else
  return ( Document* )writeTab->currentPage();
#endif

}

/** Add new kwrite class to writeStack and return id in stack */
void QuantaView::addWrite( Document* w , QString label )
{
#ifdef    USE_KDOCKTABGROUP
	writeTab->insertPage(w, label);
	writeTab->setVisiblePage(w);
#else
	QIconSet emptyIcon ( UserIcon("empty1x16"));
	writeTab->addTab  ( w,  emptyIcon,  label );
	writeTab->showPage( w );
#endif
	
	connect( w, SIGNAL(newCurPos()), this, SLOT(slotNewCurPos()));
}

/** remove KWrite class from stack, return id of new KWrite */
Document* QuantaView::removeWrite()
{
  Document *w = write();
  writeTab->removePage( w );
  return write();
}

void QuantaView::initActions()
{
    KActionCollection *actionCollection = app->actionCollection();

    (void) new KAction( i18n( "Quick Start..." ), "quick_start", 0,
                        this, SLOT( slotTagQuickStart() ),
                        actionCollection, "tag_quick_start" );
                        
    (void) new KAction( i18n( "Quick Table..." ), "quick_table", 0,
                        this, SLOT( slotTagQuickTable() ),
                        actionCollection, "tag_quick_table" );
                        
    (void) new KAction( i18n( "Quick List..." ), "quick_list", 0,
                        this, SLOT( slotTagQuickList() ),
                        actionCollection, "tag_quick_list" );
                
    app->m_actions = new QDomDocument();
    QFile f( locate("appdata","actions.rc") );
    if ( !f.open( IO_ReadOnly ) )
      return;
    if ( !app->actions()->setContent( &f ) ) {
      f.close();
      return;
    }
    f.close();

    QDomElement docElem = app->actions()->documentElement();

    QDomNode n = docElem.firstChild();
    while( !n.isNull() ) {
       QDomElement e = n.toElement(); // try to convert the node to an element.
       if( !e.isNull() ) { // the node was really an element.
           new TagAction( &e, this, actionCollection); 
       }
       n = n.nextSibling();
    }
                            
    
    (void) new KAction( i18n( "Color..." ), "color", CTRL+Key_NumberSign,
                        this, SLOT( slotTagColor() ),
                        actionCollection, "tag_color" );
                        
                        
    (void) new KAction( i18n( "E-Mail..." ), "tag_mail", 0,
                        this, SLOT( slotTagMail() ),
                        actionCollection, "tag_mail" );

    (void) new KAction( i18n( "Misc. tag..." ), "tag_misc", CTRL+Key_T,
                        this, SLOT( slotTagMisc() ),
                        actionCollection, "tag_misc" );

    (void) new KAction( i18n( "Paste &HTML Quoted" ), "editpaste", 0,
                        this, SLOT( slotPasteHTMLQuoted() ),
                        actionCollection, "edit_paste_html_quoted" );
                        
    (void) new KAction( i18n( "Paste &URL Encoded" ), "editpaste", 0,
                        this, SLOT( slotPasteURLEncoded() ),
                        actionCollection, "edit_paste_url_encoded" );
                        
    (void) new KAction( i18n( "Insert CSS..." ),"mini-modules", 0,
                        this, SLOT( slotInsertCSS() ),
                        actionCollection, "insert_css" );
                        
//    qDebug("ctrl+enter: " + QString::number(CTRL+Key_Enter) );
}

