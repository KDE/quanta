/***************************************************************************
                          quantaview.cpp  -  description
                             -------------------
    begin                : ���� 9 13:29:57 EEST 2000
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
#include <qwidget.h>
#include <qsplitter.h>
#include <qpoint.h>

// include files for KDE
#include <kaction.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kmenubar.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kurldrag.h>
#include <kdeversion.h>
#ifdef BUILD_KAFKAPART
#include "parts/kafka/wkafkapart.h"
#include <dom/dom_node.h>
#include <dom/dom_string.h>
#endif
#include <kdebug.h>

#if KDE_VERSION >= KDE_MAKE_VERSION(3,1,90)
#include <ktabwidget.h>
#endif

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
#if KDE_VERSION >= KDE_MAKE_VERSION(3,1,90)
  m_writeTab = new KTabWidget(this);
  static_cast<KTabWidget*>(m_writeTab)->setHoverCloseButton(true);
  connect(m_writeTab, SIGNAL(closeRequest(QWidget *)), quantaApp, SLOT(slotClosePage(QWidget *)));
#else
  m_writeTab = new QTabWidget(this);
#endif
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

  currentViewsLayout = QuantaView::QuantaViewOnly;//TODO: load it from the config
    #ifdef BUILD_KAFKAPART
  kafkaInterface = new WKafkaPart(0, 0, "KafkaHTMLPart");
  kafkaInterface->getKafkaPart()->view()->setMinimumHeight(50);
  #endif

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
#ifdef BUILD_KAFKAPART

  int oldLayout;

  oldLayout = currentViewsLayout;
  currentViewsLayout = QuantaView::QuantaViewOnly;
  if(oldLayout == QuantaView::QuantaAndKafkaViews)
    _splittSizes = splitter->sizes();
  if(writeExists() && oldLayout != QuantaView::QuantaViewOnly)
  {
    kafkaInterface->getKafkaPart()->view()->hide();
    kafkaInterface->getKafkaPart()->view()->reparent(0, 0, QPoint(), false);
    if(kafkaInterface->isLoaded())
      kafkaInterface->unloadDocument();
    write()->view()->reparent(write(), 0, QPoint(), true);
    resize(writeTab()->size().width()-5, writeTab()->size().height()-35);
    write()->view()->show();
    if(oldLayout == QuantaView::QuantaAndKafkaViews)
      delete splitter;
  }
#endif
  QIconSet emptyIcon ( UserIcon("empty16x16"));
  m_writeTab->addTab  ( w,  emptyIcon,  label.section("/",-1) );
  m_writeTab->setTabToolTip(w, label);
  m_writeTab->showPage( w );
#ifdef BUILD_KAFKAPART
  if(oldLayout == QuantaView::QuantaAndKafkaViews)
  {
    slotShowKafkaAndQuanta();
  }
  else if(oldLayout == QuantaView::KafkaViewOnly)
  {
    slotShowKafkaPart();
  }
#endif
  if (dynamic_cast<Document *>(w))
  {
    connect( dynamic_cast<Document*>(w)->view(),
             SIGNAL(cursorPositionChanged()), this, SLOT(slotNewCurPos()));
  }
}

/** remove KWrite class from stack, return id of new KWrite */
QWidget* QuantaView::removeWrite()
{
#ifdef BUILD_KAFKAPART
  int oldLayout;

  oldTab = 0L;
  oldLayout = currentViewsLayout;
  currentViewsLayout = QuantaView::QuantaViewOnly;
  if(oldLayout == QuantaView::QuantaAndKafkaViews)
    _splittSizes = splitter->sizes();
#endif
  if (writeExists())
  {
#ifdef BUILD_KAFKAPART
    if(oldLayout != QuantaView::QuantaViewOnly)
    {
      kafkaInterface->getKafkaPart()->view()->hide();
      kafkaInterface->getKafkaPart()->view()->reparent(0, 0, QPoint(), false);
      if(kafkaInterface->isLoaded())
        kafkaInterface->unloadDocument();
      write()->view()->reparent(write(), 0, QPoint(), true);
      resize(writeTab()->size().width()-5, writeTab()->size().height()-35);
      write()->view()->show();
      if(oldLayout == QuantaView::QuantaAndKafkaViews)
        delete splitter;
    }
#endif
    Document *w = write();
    m_writeTab->removePage(w);
    delete w;
  }/* else
  {
    m_writeTab->removePage( m_writeTab->currentPage() );
  }*/
#ifdef BUILD_KAFKAPART
  if(!writeExists())
  {
  //no more view, going back to the quanta editor view
  KToggleAction *ta = (KToggleAction *) quantaApp->actionCollection()->action( "show_quanta_editor" );
  if(ta)
    ta->setChecked(true);
  }
  else if(oldLayout == QuantaView::QuantaAndKafkaViews)
  {
    slotShowKafkaAndQuanta();
  }
  else if(oldLayout == QuantaView::KafkaViewOnly)
  {
    slotShowKafkaPart();
  }
#endif
  oldWrite = dynamic_cast<Document*>(m_writeTab->currentPage()); //don't call write() here
  return oldWrite;
}

void QuantaView::updateViews()
{
#ifdef BUILD_KAFKAPART
  int oldLayout;
  Document *_doc;
  if(oldTab)
    _doc = dynamic_cast<Document*>(oldTab);
  else
    _doc = 0L;
  oldLayout = currentViewsLayout;
  currentViewsLayout = QuantaView::QuantaViewOnly;
  if(oldLayout == QuantaView::QuantaAndKafkaViews)
    _splittSizes = splitter->sizes();
  if(_doc && oldLayout != QuantaView::QuantaViewOnly)
  //if the previous page is not a plugin
  {
    kafkaInterface->getKafkaPart()->view()->hide();
    kafkaInterface->getKafkaPart()->view()->reparent(0, 0, QPoint(), false);
    if(kafkaInterface->isLoaded())
      kafkaInterface->unloadDocument();
    _doc->view()->reparent(_doc, 0, QPoint(), true);
    resize(writeTab()->size().width()-5, writeTab()->size().height()-35);
    _doc->view()->show();
    if(oldLayout == QuantaView::QuantaAndKafkaViews)
      delete splitter;
  }

  if(!writeExists())//a plugin is loaded
  {
    currentViewsLayout = oldLayout;
    return;
   }
   else
   {
     if(oldLayout == QuantaView::QuantaAndKafkaViews)
     {
       slotShowKafkaAndQuanta();
     }
     else if(oldLayout == QuantaView::KafkaViewOnly)
     {
       slotShowKafkaPart();
     }
   }
#endif
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

    (void) new KAction( i18n( "Misc. Tag..." ), "tag_misc", CTRL+SHIFT+Key_T,
                        this, SLOT( slotTagMisc() ),
                        actionCollection, "tag_misc" );

    (void) new KAction( i18n( "Frame wizard..." ), "frame", 0,
                        this, SLOT( slotFrameWizard() ),
                        actionCollection, "tag_frame_wizard" );

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
  e->accept(KURLDrag::canDecode(e));
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

void QuantaView::slotShowQuantaEditor()
{
#ifdef BUILD_KAFKAPART
  if(!writeExists())
    return;
  int id = quantaApp->rightWidget()->id(quantaApp->rightWidget()->visibleWidget());
  if(id == 1 || id == 2)
    quantaApp->rightWidget()->raiseWidget(0);
  if(currentViewsLayout == QuantaView::QuantaViewOnly)
    return;
  else if(currentViewsLayout == QuantaView::KafkaViewOnly)
  {
    kafkaInterface->getKafkaPart()->view()->hide();
    kafkaInterface->getKafkaPart()->view()->reparent(0, 0, QPoint(), false);
    if(kafkaInterface->isLoaded())
      kafkaInterface->unloadDocument();
    write()->view()->reparent(write(), 0, QPoint(), true);
    currentViewsLayout = QuantaView::QuantaViewOnly;
    resize(writeTab()->size().width()-5, writeTab()->size().height()-35);
    write()->view()->show();
  }
  else if(currentViewsLayout == QuantaView::QuantaAndKafkaViews)
  {
    _splittSizes = splitter->sizes();
    kafkaInterface->getKafkaPart()->view()->hide();
    kafkaInterface->getKafkaPart()->view()->reparent(0, 0, QPoint(), false);
    if(kafkaInterface->isLoaded())
      kafkaInterface->unloadDocument();
    write()->view()->reparent(write(), 0, QPoint(), true);
    currentViewsLayout = QuantaView::QuantaViewOnly;
    resize(writeTab()->size().width()-5, writeTab()->size().height()-35);
    write()->view()->show();
    delete splitter;
  }
#endif
}

void QuantaView::slotShowKafkaPart()
{
#ifdef BUILD_KAFKAPART
  if(!writeExists())
    return;
  int id = quantaApp->rightWidget()->id(quantaApp->rightWidget()->visibleWidget());
  if(id == 1 || id == 2)
    quantaApp->rightWidget()->raiseWidget(0);
  if(currentViewsLayout == QuantaView::QuantaViewOnly)
  {
    write()->view()->hide();
    write()->view()->reparent(0, 0, QPoint(), false);
    if(!kafkaInterface->isLoaded())
      kafkaInterface->loadDocument(write());
    kafkaInterface->getKafkaPart()->view()->reparent(write(), 0, QPoint(), true);
    currentViewsLayout = QuantaView::KafkaViewOnly;
    resize(writeTab()->size().width()-5, writeTab()->size().height()-35);
    kafkaInterface->getKafkaPart()->view()->show();
  }
  else if(currentViewsLayout == QuantaView::KafkaViewOnly)
    return;
  else if(currentViewsLayout == QuantaView::QuantaAndKafkaViews)
  {
  _splittSizes = splitter->sizes();
    write()->view()->hide();
    write()->view()->reparent(0, 0, QPoint(), false);
    if(!kafkaInterface->isLoaded())
      kafkaInterface->loadDocument(write());
    kafkaInterface->getKafkaPart()->view()->reparent(write(), 0, QPoint(), true);
    currentViewsLayout = QuantaView::KafkaViewOnly;
    resize(writeTab()->size().width()-5, writeTab()->size().height()-35);
    kafkaInterface->getKafkaPart()->view()->show();
    delete splitter;
  }
#endif
}

void QuantaView::slotShowKafkaAndQuanta()
{
#ifdef BUILD_KAFKAPART
  if(!writeExists())
    return;
  int id = quantaApp->rightWidget()->id(quantaApp->rightWidget()->visibleWidget());
  if(id == 1 || id == 2)
    quantaApp->rightWidget()->raiseWidget(0);
  if(currentViewsLayout == QuantaView::QuantaViewOnly || currentViewsLayout == QuantaView::KafkaViewOnly)
  {
    write()->view()->reparent(0, 0, QPoint(), false);
    splitter = new QSplitter(Qt::Vertical, write());
    if(!kafkaInterface->isLoaded())
      kafkaInterface->loadDocument(write());
    kafkaInterface->getKafkaPart()->view()->reparent(splitter, 0, QPoint(), true);
    splitter->moveToFirst(kafkaInterface->getKafkaPart()->view());
    kafkaInterface->getKafkaPart()->view()->show();
    write()->view()->reparent(splitter, 0, QPoint(), true);
    write()->view()->show();
    currentViewsLayout = QuantaView::QuantaAndKafkaViews;
    resize(writeTab()->size().width()-5, writeTab()->size().height()-35);
    if(_splittSizes.empty())
    {
      QValueList<int> sizes;
      sizes << (writeTab()->size().height()-35)/2 << (writeTab()->size().height()-35)/2;
      splitter->setSizes(sizes);
    }
    else
      splitter->setSizes(_splittSizes);
#if QT_VERSION > 0x030103      //shouldn't be a problem in the real release, as KDE 3.2 will require QT 3.2
    splitter->setCollapsible(write()->view(), false);
    splitter->setCollapsible(kafkaInterface->getKafkaPart()->view(), false);
#endif
    splitter->show();
  }
  else
    return;
  currentViewsLayout = QuantaView::QuantaAndKafkaViews;
#endif
}

void QuantaView::resize(int width, int height)
{
  if(currentViewsLayout == QuantaView::QuantaViewOnly)
    write()->view()->resize(width,height);
#ifdef BUILD_KAFKAPART
  else if(currentViewsLayout == QuantaView::KafkaViewOnly)
    kafkaInterface->getKafkaPart()->view()->resize(width,height);
  else if(currentViewsLayout == QuantaView::QuantaAndKafkaViews)
    splitter->resize(width,height);
#endif
}

#include "quantaview.moc"
