/***************************************************************************
                          quantaview.cpp  -  description
                             -------------------
    begin                : ���� 9 13:29:57 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com>
                           (C) 2001-2003 Andras Mantia <amantia@kde.org>
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
#include <qscrollview.h>

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
#include <kconfig.h>
#include "parts/kafka/wkafkapart.h"
#include "parts/kafka/kafkacommon.h"
#include <dom/dom_node.h>
#include <dom/dom_string.h>
#include <kate/view.h>
#endif
#include <kdebug.h>

#if KDE_IS_VERSION(3,1,90)
#include <ktabwidget.h>
#endif

#include <ktexteditor/viewcursorinterface.h>

// application specific includes
#include "document.h"
#include "resource.h"
#include "quantaview.h"
#include "quantadoc.h"
#include "quanta.h"

#include "toolbar/tagaction.h"
#include "toolbar/toolbartabwidget.h"
#include "plugins/quantaplugin.h"
#include "plugins/quantaplugininterface.h"

QuantaView::QuantaView(QWidget *parent, const char *name )
  : QWidget( parent, name)
#ifdef BUILD_KAFKAPART
  , currentFocus(quantaFocus)
  , quantaUpdateTimer(-1)
  , kafkaUpdateTimer(-1)
#endif
{
  doc = quantaApp->doc();
  initActions();
#if KDE_IS_VERSION(3,1,90)
  m_writeTab = new KTabWidget(this);
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
  kafkaInterface = new KafkaDocument(0, 0, "KafkaPart");
  kafkaInterface->getKafkaWidget()->view()->setMinimumHeight(50);
  splitter = new QSplitter(Qt::Vertical, 0);

  m_kafkaReloadingEnabled = true;
  m_quantaReloadingEnabled = true;
  curCol = curLine = curOffset = 0;
  m_needKafkaReload = true;

  connect(kafkaInterface->getKafkaWidget(), SIGNAL(hasFocus(bool)),
    this, SLOT(slotKafkaGetFocus(bool)));
  connect(kafkaInterface, SIGNAL(newCursorPosition(int,int)),
    this, SLOT(slotSetQuantaCursorPosition(int, int)));
  connect(kafkaInterface, SIGNAL(loadingError(Node *)),
    this, SLOT(slotKafkaLoadingError(Node *)));
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
    KMessageBox::error(this, i18n("If you see this Quanta has encountered a problem and may crash. Please report this to the developers."));
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
    disconnect(write()->view(), SIGNAL(gotFocus(Kate::View *)),
      this, SLOT(slotQuantaGetFocus(Kate::View *)));
    kafkaInterface->getKafkaWidget()->view()->hide();
    kafkaInterface->getKafkaWidget()->view()->reparent(0, 0, QPoint(), false);
    if(kafkaInterface->isLoaded())
      kafkaInterface->unloadDocument();
    write()->view()->reparent(write(), 0, QPoint(), true);
    resize(writeTab()->size().width()-5, writeTab()->size().height()-35);
    write()->view()->show();
    if(oldLayout == QuantaView::QuantaAndKafkaViews)
    {
      splitter->reparent(0, 0, QPoint(), false);
      splitter->hide();
    }
  }
#endif
  if (qConfig.showCloseButtons)
    m_writeTab->addTab(w, SmallIcon("fileclose"), label.section("/",-1));
  else
    m_writeTab->addTab(w, SmallIcon("document"), label.section("/",-1));
  m_writeTab->setTabToolTip(w, label);
  Document *wr = dynamic_cast<Document *>(w);
#ifdef BUILD_KAFKAPART
  if (wr)
    connect(wr->view(), SIGNAL(gotFocus(Kate::View *)),
      this, SLOT(slotQuantaGetFocus(Kate::View *)));
#endif
  m_writeTab->showPage( w );
#ifdef BUILD_KAFKAPART
  if(writeExists() && write()->defaultDTD()->name.contains("HTML", false) == 0)
  {
    slotShowQuantaEditor();
  }
  if(oldLayout == QuantaView::QuantaAndKafkaViews)
  {
    slotShowKafkaAndQuanta();
  }
  else if(oldLayout == QuantaView::KafkaViewOnly)
  {
    slotShowKafkaPart();
  }
#endif
  if (wr)
  {
    connect( wr->view(),
             SIGNAL(cursorPositionChanged()), this, SLOT(slotNewCurPos()));
  }
}

/** remove KWrite class from stack, return id of new KWrite */
QWidget* QuantaView::removeWrite()
{
#ifdef BUILD_KAFKAPART
  int oldLayout;

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
      kafkaInterface->getKafkaWidget()->view()->hide();
      kafkaInterface->getKafkaWidget()->view()->reparent(0, 0, QPoint(), false);
      if(kafkaInterface->isLoaded())
        kafkaInterface->unloadDocument();
      write()->view()->reparent(write(), 0, QPoint(), true);
      resize(writeTab()->size().width()-5, writeTab()->size().height()-35);
      write()->view()->show();
      if(oldLayout == QuantaView::QuantaAndKafkaViews)
      {
        splitter->reparent(0, 0, QPoint(), false);
        splitter->hide();
      }
    }
#endif
    Document *w = write();
    m_writeTab->removePage(w);
#ifdef BUILD_KAFKAPART
    disconnect(w->view(), SIGNAL(gotFocus(Kate::View *)),
      this, SLOT(slotQuantaGetFocus(Kate::View *)));
#endif
    delete w;
  } else
  {
    QString tabTitle = m_writeTab->tabLabel(m_writeTab->currentPage());
    QuantaPlugin *plugin = quantaApp->m_pluginInterface->plugin(tabTitle);
    if (plugin)
    {
      plugin->unload();
      plugin->m_action->setChecked(false);
    }
  }
#ifdef BUILD_KAFKAPART
  if(!writeExists())
  {
  //no more view, going back to the quanta editor view
  KToggleAction *ta = (KToggleAction *) quantaApp->actionCollection()->action( "show_quanta_editor" );
  if(ta)
    ta->setChecked(true);
  }
  else
  {
    connect(write()->view(), SIGNAL(gotFocus(Kate::View *)),
      this, SLOT(slotQuantaGetFocus(Kate::View *)));
    if(write()->defaultDTD()->name.contains("HTML", false) == 0)
    {
      slotShowQuantaEditor();
    }
    else if(oldLayout == QuantaView::QuantaAndKafkaViews)
    {
      slotShowKafkaAndQuanta();
    }
    else if(oldLayout == QuantaView::KafkaViewOnly)
    {
      slotShowKafkaPart();
    }
  }
#endif
  oldTab = m_writeTab->currentPage();
  oldWrite = dynamic_cast<Document*>(oldTab); //don't call write() here
  return oldTab;
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
  if(oldLayout != QuantaView::QuantaViewOnly)
  {
    if(_doc)
      disconnect(_doc->view(), SIGNAL(gotFocus(Kate::View *)),
        this, SLOT(slotQuantaGetFocus(Kate::View *)));
    kafkaInterface->getKafkaWidget()->view()->hide();
    kafkaInterface->getKafkaWidget()->view()->reparent(0, 0, QPoint(), false);
    if(kafkaInterface->isLoaded())
      kafkaInterface->unloadDocument();
    if(_doc)
      _doc->view()->reparent(_doc, 0, QPoint(), true);
    resize(writeTab()->size().width()-5, writeTab()->size().height()-35);
    if(_doc)
      _doc->view()->show();
    if(oldLayout == QuantaView::QuantaAndKafkaViews)
    {
      splitter->reparent(0, 0, QPoint(), false);
      splitter->hide();
    }
  }

  if(!writeExists())//a plugin is loaded
  {
    currentViewsLayout = oldLayout;
    return;
   }
   else
   {
     connect(write()->view(), SIGNAL(gotFocus(Kate::View *)),
       this, SLOT(slotQuantaGetFocus(Kate::View *)));
     if(write()->defaultDTD()->name.contains("HTML", false) == 0)
     {
       slotShowQuantaEditor();
     }
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

    (void) new KAction( i18n( "Table Editor..." ), "quick_table", 0,
                        this, SLOT( slotTagEditTable() ),
                        actionCollection, "tag_edit_table" );

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

    (void) new KAction( i18n( "Frame Wizard..." ), "frame", 0,
                        this, SLOT( slotFrameWizard() ),
                        actionCollection, "tag_frame_wizard" );

    (void) new KAction( i18n( "Paste &HTML Quoted" ), "editpaste", 0,
                        this, SLOT( slotPasteHTMLQuoted() ),
                        actionCollection, "edit_paste_html_quoted" );

    (void) new KAction( i18n( "Paste &URL Encoded" ), "editpaste", 0,
                        this, SLOT( slotPasteURLEncoded() ),
                        actionCollection, "edit_paste_url_encoded" );

    (void) new KAction( i18n( "Insert CSS..." ),"css", 0,
                        this, SLOT( slotInsertCSS() ),
                        actionCollection, "insert_css" );

    // special-character combo
    KSelectAction* char_action = new KSelectAction(
                        i18n( "Insert Special Character" ), 0,
                        actionCollection, "insert_char" );
    connect( char_action, SIGNAL(activated(const QString &)),
             this, SLOT(slotInsertChar(const QString &)) );
    QFile file( locate("appdata","chars") );
    if ( file.open(IO_ReadOnly) ) {    // file opened successfully
        QTextStream t( &file );        // use a text stream
        t.setEncoding(QTextStream::UnicodeUTF8);
        QString s;
        while ( !t.eof() ) {           // until end of file...
            charList << i18n(t.readLine().utf8()); // line excluding '\n'
        }
        file.close();
    }
    char_action->setItems(charList);
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
  const DTDStruct *dtd = w->currentDTD(true);
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

#ifdef BUILD_KAFKAPART
/** Reloads both views ONLY when changes have been made to the Node tree ONLY. */
void QuantaView::reloadBothViews(bool force)
{
	reloadQuantaView(force);
	reloadKafkaView(force);
}
/** reload the Kafka view from the Node Tree. Set force to true if you want to reload even if not necessary. */
void QuantaView::reloadKafkaView(bool force)
{
	if((!qConfig.kafkaRefreshOnFocus && hadLastFocus() == QuantaView::kafkaFocus) ||
		(getViewsLayout() != QuantaView::QuantaViewOnly && m_kafkaReloadingEnabled) ||
		force)
		write()->docUndoRedo->reloadKafkaEditor(force);
}

/** reload the Quanta view from the Node Tree. Set force to true if you want to reload even if not necessary. */
void QuantaView::reloadQuantaView(bool force)
{
	if((!qConfig.quantaRefreshOnFocus && hadLastFocus() == QuantaView::quantaFocus) ||
		(getViewsLayout() != QuantaView::KafkaViewOnly && m_quantaReloadingEnabled)
		|| force)
		write()->docUndoRedo->reloadQuantaEditor(force);
}

#endif

void QuantaView::slotShowQuantaEditor()
{
#ifdef BUILD_KAFKAPART
  int oldLayout = currentViewsLayout;
  currentViewsLayout = QuantaView::QuantaViewOnly;

  KToggleAction *ta = (KToggleAction *) quantaApp->actionCollection()->action( "show_quanta_editor" );
  /**killTimer(quantaUpdateTimer);*/
  quantaApp->slotShowPreviewWidget(false);
  if(ta)
    ta->setChecked(true);
  if(!writeExists())
    return;
  int id = quantaApp->rightWidget()->id(quantaApp->rightWidget()->visibleWidget());
  if(id == 1 || id == 2)
    quantaApp->rightWidget()->raiseWidget(0);
  if(oldLayout == QuantaView::QuantaViewOnly)
    return;
  else if(oldLayout == QuantaView::KafkaViewOnly)
  {
    kafkaInterface->getKafkaWidget()->view()->hide();
    kafkaInterface->getKafkaWidget()->view()->reparent(0, 0, QPoint(), false);
    reloadQuantaView();
    /**kafkaInterface->getQuantaCursorPosition(curLine, curCol);
    write()->viewCursorIf->setCursorPositionReal((uint)curLine, (uint)curCol);*/
    if(kafkaInterface->isLoaded())
      kafkaInterface->unloadDocument();
    write()->view()->reparent(write(), 0, QPoint(), true);
    currentViewsLayout = QuantaView::QuantaViewOnly;
    resize(writeTab()->size().width()-5, writeTab()->size().height()-35);
    write()->view()->show();
    write()->view()->setFocus();
  }
  else if(oldLayout == QuantaView::QuantaAndKafkaViews)
  {
    _splittSizes = splitter->sizes();
    kafkaInterface->getKafkaWidget()->view()->hide();
    kafkaInterface->getKafkaWidget()->view()->reparent(0, 0, QPoint(), false);
    if(kafkaInterface->getKafkaWidget()->view()->hasFocus())
    {
    	reloadQuantaView();
	/**kafkaInterface->getQuantaCursorPosition(curLine, curCol);
	write()->viewCursorIf->setCursorPositionReal((uint)curLine, (uint)curCol);*/
    }
    if(kafkaInterface->isLoaded())
      kafkaInterface->unloadDocument();
    write()->view()->reparent(write(), 0, QPoint(), true);
    resize(writeTab()->size().width()-5, writeTab()->size().height()-35);
    write()->view()->show();
    //delete splitter;
    splitter->reparent(0, 0, QPoint(), false);
    splitter->hide();
  }
#endif
}

void QuantaView::slotShowKafkaPart()
{
#ifdef BUILD_KAFKAPART
  DOM::Node node;
  int id, oldViewsLayout;
  KToggleAction *ta = (KToggleAction *) quantaApp->actionCollection()->action( "show_kafka_view" );
  KToggleAction *ta2 = (KToggleAction *) quantaApp->actionCollection()->action( "show_quanta_editor" );

  /**killTimer(quantaUpdateTimer);*/
  quantaApp->slotShowPreviewWidget(false);
  if(!writeExists())
  {
    currentViewsLayout = QuantaView::KafkaViewOnly;
    if(ta)
      ta->setChecked(true);
    return;
  }
  if(write()->defaultDTD()->name.contains("HTML", false) == 0)
  {
    KMessageBox::information(this, i18n("Sorry, for the moment, the VPL Mode does not support the current DTD : %1").arg(write()->defaultDTD()->nickName));
    if(ta2)
      ta2->setChecked(true);
    return;
  }
  if(ta)
    ta->setChecked(true);
  id = quantaApp->rightWidget()->id(quantaApp->rightWidget()->visibleWidget());
  if(id == 1 || id == 2)
    quantaApp->rightWidget()->raiseWidget(0);
  if(currentViewsLayout == QuantaView::QuantaViewOnly)
  {
    write()->view()->hide();
    write()->view()->reparent(0, 0, QPoint(), false);
    if(!kafkaInterface->isLoaded())
      kafkaInterface->loadDocument(write());
    write()->docUndoRedo->syncKafkaCursorAndSelection();
    /**kafkaInterface->getKafkaCursorPosition(node, offset);
    kafkaInterface->getKafkaWidget()->setCurrentNode(node, offset);*/
    kafkaInterface->getKafkaWidget()->view()->reparent(write(), 0, QPoint(), true);
    oldViewsLayout = currentViewsLayout;
    currentViewsLayout = QuantaView::KafkaViewOnly;
    resize(writeTab()->size().width()-5, writeTab()->size().height()-35);
    kafkaInterface->getKafkaWidget()->view()->show();
    kafkaInterface->getKafkaWidget()->view()->setFocus();
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
    kafkaInterface->getKafkaWidget()->view()->reparent(write(), 0, QPoint(), true);
    currentViewsLayout = QuantaView::KafkaViewOnly;
    resize(writeTab()->size().width()-5, writeTab()->size().height()-35);
    kafkaInterface->getKafkaWidget()->view()->show();
    //delete splitter;
    splitter->reparent(0, 0, QPoint(), false);
    splitter->hide();
  }
  if(oldViewsLayout == QuantaView::QuantaViewOnly && (!baseNode || (baseNode->tag->type == Tag::Empty &&
    !baseNode->next && !baseNode->child)))
  {
    quantaApp->slotDocumentProperties();
  }
#endif
}

void QuantaView::slotShowKafkaAndQuanta()
{
#ifdef BUILD_KAFKAPART
  KToggleAction *ta = (KToggleAction *) quantaApp->actionCollection()->action( "show_kafka_and_quanta" );
  KToggleAction *ta2 = (KToggleAction *) quantaApp->actionCollection()->action( "show_quanta_editor" );
  int oldViewsLayout;
  DOM::Node node;

  quantaApp->slotShowPreviewWidget(false);
  if(!writeExists())
  {
    currentViewsLayout = QuantaView::QuantaAndKafkaViews;
    if(ta)
      ta->setChecked(true);
    return;
  }
  if(write()->defaultDTD()->name.contains("HTML", false) == 0)
  {
    KMessageBox::information(this, i18n("Sorry, for the moment, the VPL Mode doesn't support the current DTD : %1").arg(write()->defaultDTD()->nickName));
    if(ta2)
      ta2->setChecked(true);
    return;
  }
  if(ta)
    ta->setChecked(true);
  int id = quantaApp->rightWidget()->id(quantaApp->rightWidget()->visibleWidget());
  if(id == 1 || id == 2)
    quantaApp->rightWidget()->raiseWidget(0);
  if(currentViewsLayout == QuantaView::QuantaViewOnly || currentViewsLayout == QuantaView::KafkaViewOnly)
  {
    write()->view()->reparent(0, 0, QPoint(), false);
    splitter->reparent(write(), 0, QPoint(), true);
    splitter->show();
    if(!kafkaInterface->isLoaded())
      kafkaInterface->loadDocument(write());
    if(currentViewsLayout == QuantaView::KafkaViewOnly)
    {
      reloadQuantaView();
    }
    kafkaInterface->getKafkaWidget()->view()->reparent(splitter, 0, QPoint(), true);
    splitter->moveToFirst(kafkaInterface->getKafkaWidget()->view());
    kafkaInterface->getKafkaWidget()->view()->show();
    write()->view()->reparent(splitter, 0, QPoint(), true);
    write()->view()->show();
    oldViewsLayout = currentViewsLayout;
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
    splitter->setCollapsible(write()->view(), true);
    splitter->setCollapsible(kafkaInterface->getKafkaWidget()->view(), true);
#endif
    splitter->show();
    if (quantaUpdateTimer != -1) killTimer(quantaUpdateTimer);
    if (kafkaUpdateTimer != -1) killTimer(kafkaUpdateTimer);
    if(currentFocus == QuantaView::quantaFocus && !qConfig.kafkaRefreshOnFocus)
      kafkaUpdateTimer = startTimer(qConfig.kafkaRefreshDelay);
    else if(currentFocus == QuantaView::kafkaFocus && !qConfig.quantaRefreshOnFocus)
      quantaUpdateTimer = startTimer(qConfig.quantaRefreshDelay);
    if(oldViewsLayout == QuantaView::QuantaViewOnly && (!baseNode || (baseNode->tag->type == Tag::Empty &&
      !baseNode->next && !baseNode->child)))
    {
      quantaApp->slotDocumentProperties();
    }
  }
  else
  {
    if(write()->view()->hasFocus())
    {
      write()->docUndoRedo->syncKafkaCursorAndSelection();
      /**kafkaInterface->getKafkaCursorPosition(node, offset);
      kafkaInterface->getKafkaWidget()->setCurrentNode(node, offset);*/
      kafkaInterface->getKafkaWidget()->view()->setFocus();
    }
    else if(kafkaInterface->getKafkaWidget()->view()->hasFocus())
    {
      write()->docUndoRedo->syncQuantaCursorAndSelection();
      /**kafkaInterface->getQuantaCursorPosition(curLine, curCol);
      write()->viewCursorIf->setCursorPositionReal((uint)curLine, (uint)curCol);*/
      write()->view()->setFocus();
    }
  }
  currentViewsLayout = QuantaView::QuantaAndKafkaViews;
#endif
}

void QuantaView::resize(int width, int height)
{
  if (!writeExists())
      return;
  if(currentViewsLayout == QuantaView::QuantaViewOnly)
    write()->view()->resize(width,height);
#ifdef BUILD_KAFKAPART
  else if(currentViewsLayout == QuantaView::KafkaViewOnly)
    kafkaInterface->getKafkaWidget()->view()->resize(width,height);
  else if(currentViewsLayout == QuantaView::QuantaAndKafkaViews)
    splitter->resize(width,height);
#endif
}

void QuantaView::slotKafkaGetFocus(bool focus)
{
#ifdef BUILD_KAFKAPART
#ifdef LIGHT_DEBUG
  kdDebug(25001)<< "QuantaView::slotKafkaGetFocus(" << focus << ")" << endl;
#endif
  int contentsX, contentsY;
  KAction *action;

  if(focus)
  {
    //We reload the kafka part from the Node Tree
    if(currentViewsLayout == QuantaView::QuantaAndKafkaViews && currentFocus == QuantaView::quantaFocus)
    {
      if (kafkaUpdateTimer != -1) killTimer(kafkaUpdateTimer);
      if (quantaUpdateTimer != -1) killTimer(quantaUpdateTimer);
      if(!qConfig.quantaRefreshOnFocus)
        quantaUpdateTimer = startTimer(qConfig.quantaRefreshDelay);
      contentsX = kafkaInterface->getKafkaWidget()->view()->contentsX();
      contentsY = kafkaInterface->getKafkaWidget()->view()->contentsY();

      //Reload the kafka Editor only if Quanta was modified or if something has happened (e.g. a reparse)
      //and NEED a kafka reload.
      if(parser->parsingNeeded() && writeExists())
        baseNode = parser->rebuild(write());
      reloadKafkaView();
      //doesn't work!
      kafkaInterface->getKafkaWidget()->view()->setContentsPos(contentsX, contentsY);
    }

    //We disable some actions which doesn't work on kafka for the moment
    action = quantaApp->actionCollection()->action("tag_edit_table");
    if(action)
      action->setEnabled(false);
    action = 0L;
    action = quantaApp->actionCollection()->action("tag_quick_list");
    if(action)
      action->setEnabled(false);
    action = 0L;
    action = quantaApp->actionCollection()->action("tag_color");
    if(action)
      action->setEnabled(false);
    action = 0L;
    action = quantaApp->actionCollection()->action("tag_mail");
    if(action)
      action->setEnabled(false);
    action = 0L;
    action = quantaApp->actionCollection()->action("tag_misc");
    if(action)
      action->setEnabled(false);
    action = 0L;
    action = quantaApp->actionCollection()->action("tag_frame_wizard");
    if(action)
      action->setEnabled(false);
    action = 0L;
    action = quantaApp->actionCollection()->action("insert_css");
    if(action)
      action->setEnabled(false);
    action = 0L;
    action = quantaApp->actionCollection()->action("insert_char");
    if(action)
      action->setEnabled(false);


    currentFocus = QuantaView::kafkaFocus;
  }
#endif
}

void QuantaView::slotQuantaGetFocus(Kate::View *)
{
#ifdef BUILD_KAFKAPART
#ifdef LIGHT_DEBUG
  kdDebug(25001)<< "QuantaView::slotQuantaGetFocus(true)" << endl;
#endif
  KAction *action;

  //We reload the quanta view from the Node Tree.
  if(currentViewsLayout == QuantaView::QuantaAndKafkaViews && currentFocus == QuantaView::kafkaFocus)
  {
    if (quantaUpdateTimer != -1) killTimer(quantaUpdateTimer);
    if (kafkaUpdateTimer != -1) killTimer(kafkaUpdateTimer);
    if(!qConfig.kafkaRefreshOnFocus)
      kafkaUpdateTimer = startTimer(qConfig.kafkaRefreshDelay);
    reloadQuantaView();

    //FIXME: the tree (and the output)is right, the pos aren't.
    //This will reparse the whole Node tree and reload kafka.
    if(writeExists())
      baseNode = parser->parse(write());
  }

  currentFocus = QuantaView::quantaFocus;

    //We enable some actions which doesn't work on kafka for the moment
    action = quantaApp->actionCollection()->action("tag_edit_table");
    if(action)
      action->setEnabled(true);
    action = 0L;
    action = quantaApp->actionCollection()->action("tag_quick_list");
    if(action)
      action->setEnabled(true);
    action = 0L;
    action = quantaApp->actionCollection()->action("tag_color");
    if(action)
      action->setEnabled(true);
    action = 0L;
    action = quantaApp->actionCollection()->action("tag_mail");
    if(action)
      action->setEnabled(true);
    action = 0L;
    action = quantaApp->actionCollection()->action("tag_misc");
    if(action)
      action->setEnabled(true);
    action = 0L;
    action = quantaApp->actionCollection()->action("tag_frame_wizard");
    if(action)
      action->setEnabled(true);
    action = 0L;
    action = quantaApp->actionCollection()->action("insert_css");
    if(action)
      action->setEnabled(true);
    action = 0L;
    action = quantaApp->actionCollection()->action("insert_char");
    if(action)
      action->setEnabled(true);

#endif
}

#ifdef BUILD_KAFKAPART
void QuantaView::timerEvent( QTimerEvent *e )
{
#ifdef LIGHT_DEBUG
  kdDebug(25001)<< "QuantaView::timerEvent" << endl;
#endif
  DOM::Node node;

  if (kafkaInterface->isLoaded() && currentViewsLayout == QuantaView::QuantaAndKafkaViews && writeExists())
  {
    if(e->timerId() == kafkaUpdateTimer && hadLastFocus() == quantaFocus/**write()->view()->hasFocus()*/)
    {
      //Update kafka view
      //write()->docUndoRedo->syncKafkaView();
      reloadKafkaView();
      /**kafkaInterface->getKafkaCursorPosition(node, offset);
      kafkaInterface->getKafkaWidget()->setCurrentNode(node, offset);*/
    }
    else if(e->timerId() == quantaUpdateTimer &&  hadLastFocus() == kafkaFocus
      /**kafkaInterface->getKafkaWidget()->view()->hasFocus()*/)
    {
      //Update quanta view
      //write()->docUndoRedo->syncQuantaView();
      reloadQuantaView();
      /**kafkaInterface->getQuantaCursorPosition(curLine, curCol);
      write()->viewCursorIf->cursorPositionReal(&oldCurLine, &oldCurCol);
      if(oldCurCol != (uint)curCol || oldCurLine != (uint)curLine)
        write()->viewCursorIf->setCursorPositionReal((uint)curLine, (uint)curCol);*/
    }
  }
}
#endif

void QuantaView::slotSetQuantaCursorPosition(int col, int line)
{
#ifdef BUILD_KAFKAPART
  curCol = col;
  curLine = line;
  if(currentViewsLayout == QuantaView::QuantaAndKafkaViews || currentViewsLayout == QuantaView::QuantaViewOnly)
    write()->viewCursorIf->setCursorPositionReal((uint)curLine, (uint)curCol);
#endif
}


void QuantaView::slotSetKafkaCursorPosition(DOM::Node node, int offset)
{
#ifdef BUILD_KAFKAPART
  curNode = node;
  curOffset = offset;
  if(currentViewsLayout == QuantaView::QuantaAndKafkaViews || currentViewsLayout == QuantaView::KafkaViewOnly)
  {}
#endif
}

void QuantaView::slotKafkaLoadingError(Node *)
{
#ifdef BUILD_KAFKAPART
  quantaApp->slotShowProblemsDock(true);
#endif
}

void QuantaView::readConfig(KConfig *m_config)
{
#ifdef BUILD_KAFKAPART
  kdDebug(24000)<< "QuantaView::readConfig()" << endl;
  m_config->setGroup("Kafka Synchronization options");
  qConfig.quantaRefreshOnFocus = (m_config->readEntry("Source refresh", "delay") == "focus");
  qConfig.quantaRefreshDelay = m_config->readNumEntry("Source refresh delay", 500);
  qConfig.kafkaRefreshOnFocus = (m_config->readEntry("Kafka refresh", "focus") == "focus");
  qConfig.kafkaRefreshDelay = m_config->readNumEntry("Kafka refresh delay", 4000);
  /**reloadUpdateTimers();*/

  //reload the Timers.
  if (quantaUpdateTimer != -1) killTimer(quantaUpdateTimer);
  if (kafkaUpdateTimer != -1) killTimer(kafkaUpdateTimer);
  if(kafkaInterface->isLoaded() && currentViewsLayout == QuantaView::QuantaAndKafkaViews)
  {
    if(currentFocus == QuantaView::quantaFocus && !qConfig.kafkaRefreshOnFocus)
      kafkaUpdateTimer = startTimer(qConfig.kafkaRefreshDelay);
    else if(currentFocus == QuantaView::kafkaFocus && !qConfig.quantaRefreshOnFocus)
      quantaUpdateTimer = startTimer(qConfig.quantaRefreshDelay);
  }
  kafkaInterface->readConfig(m_config);
#endif
}

#include "quantaview.moc"
