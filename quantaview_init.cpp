/***************************************************************************
                          quantaview.cpp  -  description
                             -------------------
    begin                : Втр Май  9 13:29:57 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
                           (C) 2001-2003 by Andras Mantia <amantia@freemail.hu>
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
#include <qevent.h>
#include <qdragobject.h>
#include <qwidget.h>

// include files for KDE
#include <klocale.h>
#include <kstddirs.h>
#include <kmenubar.h>
#include <kiconloader.h>
#include <kmessagebox.h>

#include <ktexteditor/viewcursorinterface.h>

// application specific includes
#include "document.h"
#include "resource.h"
#include "quantaview.h"
#include "quantadoc.h"
#include "quanta.h"
#include "treeviews/templatestreeview.h"

#include <kaction.h>
#include "toolbar/tagaction.h"

QuantaView::QuantaView(QWidget *parent, const char *name )
  : QWidget( parent, name)
{
  doc = quantaApp->doc;
  initActions();
  
  writeTab = new QTabWidget(this);
  writeTab ->setTabPosition( QTabWidget::Bottom );
  connect( writeTab,SIGNAL(currentChanged(QWidget*)), quantaApp, SLOT(slotUpdateStatus(QWidget*)));
//  connect( writeTab,SIGNAL(selected(const QString &)), quantaApp, SLOT(slotReparse()));
  writeTab ->setFocusPolicy( QWidget::NoFocus );

  toolbarTab = new QTabWidget(this);
  toolbarTab ->setTabPosition( QTabWidget::Top );
  toolbarTab ->setFocusPolicy( QWidget::NoFocus );

  KToolBar *tb = new KToolBar(toolbarTab);
  tb->insertButton("aaa",1);
  toolbarTab->addTab(tb, "xxx");
//Find with this trick the correct needed size for the toolbar holding QTabWidget
  toolbarTab->setMinimumHeight(tb->minimumSizeHint().height()+toolbarTab->height());
  toolbarTab->removePage(tb);
  delete tb;
  
  QGridLayout *layout = new QGridLayout( this );
  layout->setRowStretch(0,0);
  layout->setRowStretch(1,1);
  layout->addWidget( toolbarTab     ,0,0);
  layout->addWidget( writeTab     ,1,0);


  writeTab->show();
//  toolbarTab->show();


  oldWrite = 0L;

  connect(this, SIGNAL(dragInsert(QDropEvent *)), quantaApp->tTab, SLOT(slotDragInsert(QDropEvent *)));
  setAcceptDrops(TRUE); // [MB02] Accept drops on the view
}

QuantaView::~QuantaView()
{
}

/** return current KWrite class */
Document* QuantaView::write()
{
  Document *w = dynamic_cast<Document *>(writeTab->currentPage());
  if (!w)
  {
    KMessageBox::error(this, i18n("If you see this, you are in big trouble as Quanta may crash."));
  }

  return w;
}

/** Add new kwrite class to writeStack and return id in stack */
void QuantaView::addWrite( Document* w , QString label )
{
  QIconSet emptyIcon ( UserIcon("empty1x16"));
  writeTab->addTab  ( w,  emptyIcon,  label.section("/",-1) );
  writeTab->setTabToolTip(w, label);
  writeTab->showPage( w );
  connect( w->view(),
           SIGNAL(cursorPositionChanged()), this, SLOT(slotNewCurPos()));
}

/** remove KWrite class from stack, return id of new KWrite */
Document* QuantaView::removeWrite()
{
  if (writeExists())
  {
    Document *w = write();
    writeTab->removePage( w );
    delete w;
  }
  return dynamic_cast<Document *>(writeTab->currentPage()); //don't call write() here
}

void QuantaView::initActions()
{
    KActionCollection *actionCollection = quantaApp->actionCollection();

    (void) new KAction( i18n( "Quick Start..." ), "quick_start", 0,
                        this, SLOT( slotTagQuickStart() ),
                        actionCollection, "tag_quick_start" );
                        
    (void) new KAction( i18n( "Quick Table..." ), "quick_table", 0,
                        this, SLOT( slotTagQuickTable() ),
                        actionCollection, "tag_quick_table" );
                        
    (void) new KAction( i18n( "Quick List..." ), "quick_list", 0,
                        this, SLOT( slotTagQuickList() ),
                        actionCollection, "tag_quick_list" );
                
    quantaApp->m_actions = new QDomDocument();
    QFile f( locate("appdata","actions.rc") );
    if ( !f.open( IO_ReadOnly ) )
      return;
    if ( !quantaApp->actions()->setContent( &f ) ) {
      f.close();
      return;
    }
    f.close();

    QDomElement docElem = quantaApp->actions()->documentElement();

    QDomNode n = docElem.firstChild();
    while( !n.isNull() ) {
       QDomElement e = n.toElement(); // try to convert the node to an element.
       if( !e.isNull() ) { // the node was really an element.
           new TagAction( &e, this );
       }
       n = n.nextSibling();
    }
                            
    
    (void) new KAction( i18n( "Color..." ), "color", CTRL+Key_NumberSign,
                        this, SLOT( slotTagColor() ),
                        actionCollection, "tag_color" );
                        
                        
    (void) new KAction( i18n( "Email..." ), "tag_mail", 0,
                        this, SLOT( slotTagMail() ),
                        actionCollection, "tag_mail" );

    (void) new KAction( i18n( "Misc. Tag..." ), "tag_misc", CTRL+Key_T,
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

    // special-character combo
    KSelectAction* char_action = new KSelectAction(
                        i18n( "Insert Special Character" ), 0,
                        actionCollection, "insert_char" );
    connect( char_action, SIGNAL(activated(const QString &)),
             this, SLOT(slotInsertChar(const QString &)) );
    QStringList char_list;
    QFile file( locate("appdata","chars") );
    if ( file.open(IO_ReadOnly) ) {    // file opened successfully
        QTextStream t( &file );        // use a text stream
        QString s;
        while ( !t.eof() ) {           // until end of file...
            char_list << t.readLine(); // line excluding '\n'
        }
        file.close();
    }
    char_action->setItems(char_list);
    char_action->setComboWidth(150);

//    qDebug("ctrl+enter: " + QString::number(CTRL+Key_Enter) );
}

/** No descriptions */
void QuantaView::resizeEvent (QResizeEvent *)
{
  if (writeExists())
     write()->view()->resize(writeTab->size().width()-5, writeTab->size().height()-35);
}

void QuantaView::dragEnterEvent(QDragEnterEvent *e)
{
  e->accept(QUriDrag::canDecode(e));
}

void QuantaView::dropEvent(QDropEvent *e)
{
  emit dragInsert(e);
}
/** True if a Document object exists, false otherwise. */
bool QuantaView::writeExists()
{
 return (dynamic_cast<Document *>(writeTab->currentPage()))?true:false;
}
