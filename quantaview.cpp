/***************************************************************************
                          quantaview.cpp  -  description
                             -------------------
    begin                : Втр Май  9 13:29:57 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
                           (C) 2001-2003 Andras Mantia <amantia@freemail.hu>
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
#include <qtimer.h>
#include <qlayout.h>
#include <qwidgetstack.h>
#include <qdom.h>
#include <qfile.h>
#include <qevent.h>
#include <qdragobject.h>
#include <qwidget.h>

// include files for KDE
#include <kaction.h>
#include <klocale.h>
#include <kstandarddirs.h>
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

#include "toolbar/tagaction.h"
#include "toolbar/toolbartabwidget.h"

QuantaView::QuantaView(QWidget *parent, const char *name )
  : QWidget( parent, name)
{
  doc = quantaApp->doc();
  initActions();
  m_writeTab = new QTabWidget(this);
  m_writeTab ->setTabPosition( QTabWidget::Bottom );
  m_writeTab ->setFocusPolicy( QWidget::NoFocus );
  connect( m_writeTab,SIGNAL(currentChanged(QWidget*)), quantaApp, SLOT(slotUpdateStatus(QWidget*)));

  m_toolbarTab = new ToolbarTabWidget(this);
  m_toolbarTab ->setFocusPolicy( QWidget::NoFocus );

  connect(m_toolbarTab, SIGNAL(removeToolbar(const QString&)),
          quantaApp, SLOT(slotRemoveToolbar(const QString&)));
  connect(m_toolbarTab, SIGNAL(renameToolbar(const QString&)),
          quantaApp, SLOT(slotRenameToolbar(const QString&)));
  connect(m_toolbarTab, SIGNAL(editToolbar(const QString&)),
          quantaApp, SLOT(slotConfigureToolbars(const QString&)));

  QGridLayout *layout2 = new QGridLayout(this, 1, 2);
  layout2->setRowStretch(0,0);
  layout2->setRowStretch(1,1);
  layout2->addWidget( m_toolbarTab     ,0,0);
  layout2->addWidget( m_writeTab     ,1,0);
  m_writeTab->show();

  oldWrite = 0L;
  oldTab = 0L;

  setAcceptDrops(TRUE); // [MB02] Accept drops on the view
}

QuantaView::~QuantaView()
{
}


/** return current KWrite class */
Document* QuantaView::write()
{
  Document *w = dynamic_cast<Document *>(m_writeTab->currentPage());
  if (!w)
  {
    KMessageBox::error(this, i18n("If you see this, you are in big trouble as Quanta may crash."));
  }

  return w;
}

/** Add new kwrite class to writeStack and return id in stack */
void QuantaView::addWrite( QWidget* w , QString label )
{
  QIconSet emptyIcon ( UserIcon("empty1x16"));
  m_writeTab->addTab  ( w,  emptyIcon,  label.section("/",-1) );
  m_writeTab->setTabToolTip(w, label);
  m_writeTab->showPage( w );
  if (dynamic_cast<Document *>(w))
  {
    connect( dynamic_cast<Document*>(w)->view(),
             SIGNAL(cursorPositionChanged()), this, SLOT(slotNewCurPos()));
  }
}

/** remove KWrite class from stack, return id of new KWrite */
QWidget* QuantaView::removeWrite()
{
  if (writeExists())
  {
    Document *w = write();
    m_writeTab->removePage(w);
    delete w;
  } else
  {
    m_writeTab->removePage( m_writeTab->currentPage() );
  }
  oldWrite = dynamic_cast<Document*>(m_writeTab->currentPage()); //don't call write() here
  return oldWrite;
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

    (void) new KAction( i18n( "Color..." ), "color", CTRL+SHIFT+Key_C,
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
            char_list << i18n(t.readLine()); // line excluding '\n'
        }
        file.close();
    }
    char_action->setItems(char_list);
}

/** No descriptions /
void QuantaView::resizeEvent (QResizeEvent *e)
{
  m_toolbarTab->resize(QSize(width(), m_toolbarTab->height()));
  if (writeExists())
     write()->view()->resize(m_writeTab->size().width()-5, m_writeTab->size().height()-35);
  QWidget::resizeEvent(e);
}

*/

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
 return (dynamic_cast<Document *>(m_writeTab->currentPage()))?true:false;
}

void QuantaView::insertTag( const char *tag)
{

  Document *w = write();
  QString tagStr = QuantaCommon::tagCase(tag);
  DTDStruct *dtd = w->currentDTD(true);
  bool single = QuantaCommon::isSingleTag(dtd->name, tagStr);
  bool optional = QuantaCommon::isOptionalTag(dtd->name, tagStr);

  QString startTag = tagStr;
  startTag.prepend("<");
  if ( dtd->singleTagStyle == "xml" &&
       ( single || (optional && !qConfig.closeOptionalTags))
     )
  {
    startTag.append(" /");
  }
  startTag.append(">");

  if ( (qConfig.closeTags && !single && !optional) ||
       (qConfig.closeOptionalTags && optional) )
  {
    w->insertTag( startTag, QString("</")+tagStr+">");
  }
  else
  {
    w->insertTag(startTag);
  }
}
#include "quantaview.moc"
