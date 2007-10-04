/***************************************************************************
                          quantaview.cpp  -  description
                             -------------------
    begin                : ���� 9 13:29:57 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com>
                           (C) 2001-2005 Andras Mantia <amantia@kde.org>
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
#include <kdebug.h>
#include <kdirwatch.h>
#include <khtmlview.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kmenubar.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kurldrag.h>
#include <kdeversion.h>
#include <kparts/partmanager.h>
#include <kstatusbar.h>

#include "undoredo.h"
#include "kafkacommon.h"
#include "wkafkapart.h"

#include <ktexteditor/document.h>
#include <ktexteditor/selectioninterface.h>
#include <ktexteditor/selectioninterfaceext.h>
#include <ktexteditor/view.h>
#include <ktexteditor/viewcursorinterface.h>

// application specific includes
#include "document.h"
#include "resource.h"
#include "quantaview.h"
#include "quanta.h"
#include "quantacommon.h"
#include "qextfileinfo.h"
#include "viewmanager.h"

#include "tagaction.h"
#include "toolbartabwidget.h"
#include "quantaplugin.h"
#include "project.h"
#include "structtreeview.h"

#include "tagdialog.h"

extern int NN;
extern QValueList<Node*> nodes;

QuantaView::QuantaView(QWidget *parent, const char *name, const QString &caption )
  : KMdiChildView(parent, name)
  , m_document(0L)
  , m_plugin(0L)
  , m_customWidget(0L)
  , m_kafkaDocument(0L)
  , m_currentFocus(SourceFocus)
{
  setMDICaption(caption);
//Connect the VPL update timers
  connect(&m_sourceUpdateTimer, SIGNAL(timeout()), this, SLOT(sourceUpdateTimerTimeout()));
  connect(&m_VPLUpdateTimer, SIGNAL(timeout()), this, SLOT(VPLUpdateTimerTimeout()));

//create the source and VPL holding widgets
  m_documentArea = new QWidget(this);

//get the reference to the user toolbar holding widget
  ToolbarTabWidget *m_toolbarTab = ToolbarTabWidget::ref();
  m_toolbarTab->reparent(this, 0, QPoint(), true);
  m_toolbarTab ->setFocusPolicy( QWidget::NoFocus );

//create a splitter to separate the VPL and document area
  m_splitter = new QSplitter(Qt::Vertical, this);
//place the widgets in a grid
  m_viewLayout = new QGridLayout(this, 2, 0);
  m_viewLayout->setRowStretch(0, 0);
  m_viewLayout->setRowStretch(1,1);
  m_viewLayout->addWidget( m_toolbarTab, 0, 0);
  m_viewLayout->addWidget( m_documentArea, 1, 0);

  m_documentArea->show();

  setAcceptDrops(true); // [MB02] Accept drops on the view
}

QuantaView::~QuantaView()
{
   // quantaApp is undefined if the destructor of QuantaApp is active
   if (quantaApp)
     quantaApp->slotFileClosed(m_document);
   if (m_document)
   {
     m_document->view()->reparent(0L, 0, QPoint(), false);
     if (quantaApp)
       emit documentClosed(m_document->url());
   }
   delete m_document;
   m_document = 0L;
}

bool QuantaView::mayRemove()
{
  emit hidePreview();
  if (m_plugin)
  {
    m_plugin->unload(false);
  } else
  {
    bool unmodifiedUntitled = false;
    if (m_document && m_document->isUntitled() && !m_document->isModified())
      unmodifiedUntitled = true;
    if (m_customWidget)
        m_customWidget->reparent(0L, 0, QPoint(), false);
    if (!saveModified())
          return false;
    slotSetSourceLayout();
    if (static_cast<QuantaView *>(quantaApp->activeWindow()) == this)
    {
        parser->setSAParserEnabled(false);
        kdDebug(24000) << "Node objects before delete = " << NN << " ; list count = " << nodes.count() << endl;
        Node::deleteNode(baseNode);
        baseNode = 0L;
        kdDebug(24000) << "Node objects after delete = " << NN << " ; list count = " << nodes.count() << endl;
        QValueList<Node*> nList = nodes;
/*        for (QValueList<Node*>::ConstIterator it = nList.constBegin(); it != nList.constEnd(); ++it)
        Node::deleteNode(*it);
        kdDebug(24000) << "Node objects after cleanup = " << NN << " ; list count = " << nodes.count() << endl;*/
    }
    if (m_document)
    {
      KURL url = m_document->url();
      Project::ref()->saveBookmarks(url, dynamic_cast<KTextEditor::MarkInterface*>(m_document->doc()));
      if (!unmodifiedUntitled)
        emit eventHappened("before_close", url.url(), QString::null);
      m_currentViewsLayout = -1;
//      m_document->closeTempFile();
      if (!m_document->isUntitled() && url.isLocalFile())
      {
        fileWatcher->removeFile(url.path());
//        kdDebug(24000) << "removeFile[mayRemove]: " << url.path() << endl;
      }
      Project::ref()->saveCursorPosition(url, dynamic_cast<KTextEditor::ViewCursorInterface*>(m_document->view()));

      quantaApp->menuBar()->activateItemAt(-1);
      quantaApp->guiFactory()->removeClient(m_document->view());
      if (!unmodifiedUntitled)
        emit eventHappened("after_close", url.url(), QString::null);
    }
/*      kdDebug(24000) << "Calling reparse from close " << endl;
      parser->setSAParserEnabled(true);
      quantaApp->reparse(true);*/
  }
  return true;
}

void QuantaView::addDocument(Document *document)
{
   if (!document)
     return;
   m_document = document;
   connect(m_document, SIGNAL(editorGotFocus()), this, SLOT(slotSourceGetFocus()));
   connect(m_document->view(), SIGNAL(cursorPositionChanged()), this, SIGNAL(cursorPositionChanged()));


   m_kafkaDocument = KafkaDocument::ref();

  connect(m_kafkaDocument->getKafkaWidget(), SIGNAL(hasFocus(bool)),
    this, SLOT(slotVPLGetFocus(bool)));
  connect(m_kafkaDocument, SIGNAL(newCursorPosition(int,int)),
    this, SLOT(slotSetCursorPositionInSource(int, int)));
  connect(m_kafkaDocument, SIGNAL(loadingError(Node *)),
    this, SLOT(slotVPLLoadingError(Node *)));

   m_kafkaReloadingEnabled = true;
   m_quantaReloadingEnabled = true;
   m_curCol = m_curLine = m_curOffset = 0;

//init the VPL part
  m_currentViewsLayout = SourceOnly;//to correctly reload the timers.

  reloadUpdateTimers();

   m_currentViewsLayout = -1; //force loading of this layout
   slotSetSourceLayout();
}

void QuantaView::addPlugin(QuantaPlugin *plugin)
{
   ToolbarTabWidget *m_toolbarTab = ToolbarTabWidget::ref();
   m_toolbarTab->reparent(0, 0, QPoint(), false);
   m_plugin = plugin;
   m_splitter->hide();
   QWidget *w = m_plugin->widget();
   if (w)
   {
       w->reparent(m_documentArea, 0, QPoint(), true);
       w->resize(m_documentArea->size());
   }
   m_documentArea->reparent(this, 0, QPoint(), true);
   m_viewLayout->addWidget(m_documentArea, 1, 0);
   activated();
   updateTab();
}

void QuantaView::addCustomWidget(QWidget *widget, const QString &label)
{
   if (widget)
   {
      ToolbarTabWidget::ref()->reparent(0, 0, QPoint(), false);
      m_customWidget = widget;
      m_splitter->hide();
      widget->reparent(m_documentArea, 0, QPoint(), true);
      widget->resize(m_documentArea->size());
      if (!label.isEmpty())
      {
          widget->setCaption(label);
          updateTab();
      }
      m_viewLayout->addWidget(m_documentArea, 1, 0);
      m_documentArea->show();
   } else
   if (m_customWidget)
   {
      ToolbarTabWidget::ref()->reparent(this, 0, QPoint(), qConfig.enableDTDToolbar);
      m_viewLayout->addWidget(ToolbarTabWidget::ref(), 0 , 0);
      m_customWidget = 0L; //avoid infinite recursion
      reloadLayout();
   }
  if (m_documentArea->height() + ToolbarTabWidget::ref()->height() > height() && ToolbarTabWidget::ref()->isVisible())
    resize(m_documentArea->width(), m_documentArea->height() - ToolbarTabWidget::ref()->height());
  else if (ToolbarTabWidget::ref()->isHidden())
    resize(width(), height());
}

void QuantaView::reloadLayout()
{
  int currentViewsLayout = m_currentViewsLayout;
  m_currentViewsLayout = -1; //force loading of this layout
  switch (currentViewsLayout)
  {
        case SourceOnly:
            slotSetSourceLayout();
            break;
        case SourceAndVPL:
            slotSetSourceAndVPLLayout();
            break;
        case VPLOnly:
            slotSetVPLOnlyLayout();
            break;
  }
}

void QuantaView::updateTab()
{
    if (qConfig.showCloseButtons == "ShowAlways")
    {
        setIcon(SmallIcon("fileclose"));
    }
    if (m_document)
    {
        // try to set the icon from mimetype
        QIconSet mimeIcon (KMimeType::pixmapForURL(m_document->url(), 0, KIcon::Small));
        if (mimeIcon.isNull())
          mimeIcon = QIconSet(SmallIcon("document"));
        QString urlStr = QExtFileInfo::shortName(m_document->url().path());
        if (m_document->isModified())
        {
            if (qConfig.showCloseButtons == "ShowAlways")
            {
               setMDICaption(urlStr + " " + i18n("[modified]"));
            } else
            {
                setIcon(SmallIcon("filesave"));
                setMDICaption(urlStr);
            }
            m_szCaption = urlStr + " " + i18n("[modified]");
        } else
        {
          if (qConfig.showCloseButtons != "ShowAlways")
          {
             setIcon(mimeIcon.pixmap());
          }
          setMDICaption(urlStr);
          quantaApp->setTabToolTip(this, m_document->url().prettyURL(0, KURL::StripFileProtocol));
        }
    } else
    if (m_plugin)
    {
        if (qConfig.showCloseButtons != "ShowAlways")
        {
            setIcon(SmallIcon(m_plugin->icon()));
        }
       setMDICaption(m_plugin->name());
    } else
    if (m_customWidget)
    {
        if (qConfig.showCloseButtons != "ShowAlways")
        {
            setIcon(*(m_customWidget->icon()));
        }
        setMDICaption(m_customWidget->caption());
    }
}

QString QuantaView::tabName()
{
   if (m_document)
   {
       return m_document->url().fileName();
   } else
   if (m_plugin)
   {
       return m_plugin->name();
   } else
   if (m_customWidget)
   {
       return m_customWidget->caption();
   } else
   return "";
}

void QuantaView::slotSetSourceLayout()
{
   emit hidePreview();
   if (m_currentViewsLayout == SourceOnly || !m_document)
     return;

   if(m_currentViewsLayout == SourceAndVPL)
     m_splitterSizes = m_splitter->sizes();

   KToggleAction *ta = (KToggleAction *) quantaApp->actionCollection()->action( "show_quanta_editor" );
   if (ta)
      ta->setChecked(true);

   //hide the VPL widget, reload the source if necessary
   if ((m_currentViewsLayout == SourceAndVPL && m_kafkaDocument->getKafkaWidget()->view()->hasFocus()) ||
     m_currentViewsLayout == VPLOnly)
   {
          reloadSourceView();
   }
   if (m_kafkaDocument->isLoaded())
       m_kafkaDocument->unloadDocument();

//show the document if full size
   m_splitter->hide();
   m_kafkaDocument->getKafkaWidget()->view()->reparent(0, 0, QPoint(), false);
   m_document->view()->reparent(m_documentArea, 0, QPoint(), true);
   m_document->view()->resize(m_documentArea->size());
   m_viewLayout->addWidget(m_documentArea, 1, 0);
   m_document->view()->setFocus();

   m_currentViewsLayout = SourceOnly;

//update timers are not needed in source only mode
   m_sourceUpdateTimer.stop();
   m_VPLUpdateTimer.stop();
}


void QuantaView::slotSetSourceAndVPLLayout()
{
   emit hidePreview();
   if (m_currentViewsLayout == SourceAndVPL  || !m_document)
     return;

   KToggleAction *ta = (KToggleAction *) quantaApp->actionCollection()->action( "show_kafka_and_quanta" );

   if (m_document->defaultDTD()->name.contains("HTML", false) == 0)
   {
      KMessageBox::information(this, i18n("The VPL Mode does not support the current DTD, at the moment: %1").arg(m_document->defaultDTD()->nickName));
      KToggleAction *ta2 = (KToggleAction *) quantaApp->actionCollection()->action( "show_quanta_editor" );
      if (ta2)
          ta2->setChecked(true);
      return;
  }

   if (ta)
       ta->setChecked(true);


    if (!m_kafkaDocument->isLoaded())
        m_kafkaDocument->loadDocument(m_document);
    if (m_currentViewsLayout == VPLOnly)
    {
      reloadSourceView();
    }
    m_kafkaDocument->getKafkaWidget()->view()->reparent(m_splitter, 0, QPoint(), true);
    m_splitter->moveToFirst(m_kafkaDocument->getKafkaWidget()->view());
    m_document->view()->reparent(m_splitter, 0, QPoint(), true);
    m_viewLayout->addWidget(m_splitter, 1, 0);
    m_splitter->setSizes(m_splitterSizes);
    m_splitter->show();

    if ( m_currentViewsLayout == SourceOnly &&
         (!baseNode || (baseNode->tag->type == Tag::Empty &&
          !baseNode->next && !baseNode->child)))
    {
        quantaApp->documentProperties(true);
    }

   m_currentViewsLayout = SourceAndVPL;

   reloadUpdateTimers();
}

void QuantaView::slotSetVPLOnlyLayout()
{
   emit hidePreview();
   if (m_currentViewsLayout == VPLOnly || !m_document)
     return;

   if(m_currentViewsLayout == SourceAndVPL)
     m_splitterSizes = m_splitter->sizes();

   KToggleAction *ta = (KToggleAction *) quantaApp->actionCollection()->action( "show_kafka_view" );

   if (m_document->defaultDTD()->name.contains("HTML", false) == 0)
   {
      KMessageBox::information(this, i18n("The VPL Mode does not support the current DTD, at the moment: %1").arg(m_document->defaultDTD()->nickName));
      KToggleAction *ta2 = (KToggleAction *) quantaApp->actionCollection()->action( "show_quanta_editor" );
      if (ta2)
          ta2->setChecked(true);
      return;
  }

   if (ta)
       ta->setChecked(true);

   m_splitter->hide();
   if (!m_kafkaDocument->isLoaded())
        m_kafkaDocument->loadDocument(m_document);

   m_kafkaDocument->getKafkaWidget()->view()->reparent(m_documentArea, 0, QPoint(), true);
   m_kafkaDocument->getKafkaWidget()->view()->resize(m_documentArea->size());
   m_viewLayout->addWidget(m_documentArea, 1, 0);
   m_kafkaDocument->getKafkaWidget()->view()->setFocus();

   if ( m_currentViewsLayout == SourceOnly &&
         (!baseNode || (baseNode->tag->type == Tag::Empty &&
          !baseNode->next && !baseNode->child)))
   {
      quantaApp->documentProperties(true);
   }

   m_currentViewsLayout = VPLOnly;


//update timers are not needed in VPL only mode
    m_sourceUpdateTimer.stop();
    m_VPLUpdateTimer.stop();
}

void QuantaView::reloadUpdateTimers()
{
    QuantaView* view=ViewManager::ref()->activeView();

    m_sourceUpdateTimer.stop();
    m_VPLUpdateTimer.stop();

   if (m_kafkaDocument->isLoaded() && m_currentViewsLayout == SourceAndVPL && view && view == this)
   {
       if (m_currentFocus == VPLFocus && !qConfig.quantaRefreshOnFocus)
          m_sourceUpdateTimer.start(qConfig.quantaRefreshDelay);
       if (m_currentFocus == SourceFocus && !qConfig.kafkaRefreshOnFocus)
          m_VPLUpdateTimer.start(qConfig.kafkaRefreshDelay);
   }
}

void QuantaView::slotVPLGetFocus(bool focus)
{
  // is Quanta exiting?
  if (!quantaApp) return;
#ifdef LIGHT_DEBUG
  kdDebug(25001)<< "slotVPLGetFocus(" << focus << ")" << endl;
#endif
  int contentsX, contentsY;
  KAction *action;

  if(focus)
  {
    //We reload the kafka part from the Node Tree
    if (m_currentViewsLayout == SourceAndVPL && m_currentFocus == SourceFocus)
    {

      contentsX = m_kafkaDocument->getKafkaWidget()->view()->contentsX();
      contentsY = m_kafkaDocument->getKafkaWidget()->view()->contentsY();

      //Reload the kafka Editor only if Quanta was modified or if something has happened (e.g. a reparse)
      //and NEED a kafka reload.
      if  (parser->parsingNeeded())
          baseNode = parser->rebuild(m_document);
      reloadVPLView();
      //doesn't work!
      m_kafkaDocument->getKafkaWidget()->view()->setContentsPos(contentsX, contentsY);
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

   //TEMPORARY: Enable VPL undo/redo logging
   m_document->docUndoRedo->turnOn(true);

    m_currentFocus = VPLFocus;
    reloadUpdateTimers();
 }
}

void QuantaView::slotSourceGetFocus()
{
  // is Quanta exiting?
  if (!quantaApp) return;
#ifdef LIGHT_DEBUG
  kdDebug(25001)<< "slotSourceGetFocus(true)" << endl;
#endif
  KAction *action;

  quantaApp->partManager()->setActivePart(m_document->doc(), m_document->view());
  //We reload the quanta view from the Node Tree.
  if (m_currentViewsLayout == SourceAndVPL && m_currentFocus == VPLFocus)
  {
    reloadSourceView();

    //FIXME: the tree (and the output)is right, the pos aren't.
    //This will reparse the whole Node tree and reload kafka.
     baseNode = parser->parse(m_document);
  }

  m_currentFocus = SourceFocus;
  reloadUpdateTimers();

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

    //TEMPORARY: Disable VPL undo/redo logging
    m_document->docUndoRedo->turnOn(false);

}

/** Reloads both views ONLY when changes have been made to the Node tree ONLY. */
void QuantaView::reloadBothViews(bool force)
{
  reloadSourceView(force);
  reloadVPLView(force);
}

/** reload the Kafka view from the Node Tree. Set force to true if you want to reload even if not necessary. */
void QuantaView::reloadVPLView(bool force)
{
  if (m_document && (m_kafkaReloadingEnabled || force))
      m_document->docUndoRedo->reloadKafkaEditor(force);
}

/** reload the Quanta view from the Node Tree. Set force to true if you want to reload even if not necessary. */
void QuantaView::reloadSourceView(bool force)
{
  if (m_quantaReloadingEnabled || force)
      m_document->docUndoRedo->reloadQuantaEditor(force);
}


void QuantaView::VPLUpdateTimerTimeout()
{
  if(quantaApp && m_currentFocus == SourceFocus)
    reloadVPLView();
}

void QuantaView::sourceUpdateTimerTimeout()
{
  if(quantaApp && m_currentFocus == VPLFocus)
    reloadSourceView();
}

void QuantaView::slotVPLLoadingError(Node *)
{
   emit showProblemsView();
}


void QuantaView::slotSetCursorPositionInSource(int col, int line)
{
  m_curCol = col;
  m_curLine = line;
  if (m_currentViewsLayout == SourceAndVPL || m_currentViewsLayout == SourceOnly)
    m_document->viewCursorIf->setCursorPositionReal(line, col);
}

void QuantaView::dragEnterEvent(QDragEnterEvent *e)
{
  e->accept(KURLDrag::canDecode(e));
}

void QuantaView::dropEvent(QDropEvent *e)
{
   emit dragInsert(e);
}

void QuantaView::resizeEvent(QResizeEvent *e)
{
  QWidget::resizeEvent(e);
  resize(m_documentArea->width(), m_documentArea->height());
}

void QuantaView::resize(int width, int height)
{
  if (m_plugin && m_plugin->widget())
  {
      m_plugin->widget()->resize(width, height);
      return;
  } else
  if (m_customWidget)
  {
      m_customWidget->resize(width, height);
      return;
  } else
  if (!m_document)
      return;
  if (m_currentViewsLayout == SourceOnly)
    m_document->view()->resize(width, height);
  else
  if (m_currentViewsLayout == VPLOnly)
    m_kafkaDocument->getKafkaWidget()->view()->resize(width,height);
  else
  if (m_currentViewsLayout == SourceAndVPL)
  {
    m_splitter->resize(width, height);
    m_splitterSizes = m_splitter->sizes();
  }
}

void QuantaView::insertTag(const char *tag)
{
   if (!m_document )
      return;
  QString tagStr = QuantaCommon::tagCase(tag);
  const DTDStruct *dtd = m_document->currentDTD(true);
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
     m_document->insertTag( startTag, QString("</")+tagStr+">");
  }
  else
  {
     m_document->insertTag(startTag);
  }
}

//FIXME: Move out from here??
/** Insert a new tag by bringing up the TagDialog. */
void QuantaView::insertNewTag(const QString &tag, const QString &attr, bool insertInLine)
{
  if (m_document)
  {
    if (m_currentFocus == QuantaView::VPLFocus ||
        (m_currentFocus == QuantaView::SourceFocus && qConfig.smartTagInsertion))
      insertOutputInTheNodeTree("", "", quantaApp->showTagDialogAndReturnNode(tag, attr));
    else
    {
      QString selection;
      if (m_document->selectionIf)
        selection = m_document->selectionIf->selection();
      TagDialog *dlg = new TagDialog(QuantaCommon::tagFromDTD(m_document->getDTDIdentifier(), tag), selection, attr, baseURL());
      if (dlg->exec())
      {
        dlg->insertTag(m_document, insertInLine);
      }

      delete dlg;
    }
  }
}

void QuantaView::insertOutputInTheNodeTree(const QString &str1, const QString &str2, Node *node)
{
  if (!m_document)
    return;
#ifdef LIGHT_DEBUG
  if (node)
    kdDebug(25001)<< "QuantaView::insertOutputInTheNodeTree() - node : " << node->tag->name <<
      " - type : " << node->tag->type << endl;
  else
    kdDebug(25001)<< "QuantaView::insertOutputInTheNodeTree() - str1 : " << str1 <<
                " - str2 : " << str2 << endl;
#endif
  KafkaWidget *kafkaPart = m_kafkaDocument->getKafkaWidget();
  NodeModifsSet *modifs;
  DOM::Node domNode, domStartContainer, domEndContainer;
  QString tagName;
  QTag *nodeQTag, *qTag, *nodeParentQTag;
  Node *nodeCursor, *startContainer, *endContainer, *nodeParent, *dummy;
  QPtrList<QTag> qTagList;
  int startCol, startLine, endCol, endLine;
  bool specialTagInsertion = false;
  long nodeOffset, startOffset, endOffset, domNodeOffset;
  QValueList<int> loc;
  uint line, col;
  bool smartTagInsertion, hasSelection, nodeTreeModified;

  if (!node && str1.isEmpty() || node && !str1.isEmpty())
      return;

        //Three cases :
        //- Tag insertion in VPL
        //- Normal tag insertion in kate
        //- Smart tag insertion in kate
  smartTagInsertion = (m_currentFocus == QuantaView::SourceFocus && qConfig.smartTagInsertion);

  if (m_currentFocus == QuantaView::VPLFocus || smartTagInsertion)
  {
    modifs = new NodeModifsSet();
    if (!node && !str1.isEmpty())
    {
      //We build the node from the str1
      node = kafkaCommon::createNode("", "", Tag::XmlTag, m_document);
      node->tag->parse(str1, m_document);
      node->tag->name = QuantaCommon::tagCase(node->tag->name);
      node->tag->single = QuantaCommon::isSingleTag(m_document->defaultDTD()->name,
              node->tag->name);
    }
    if (m_currentFocus == QuantaView::VPLFocus)
    {
      kafkaPart->getCurrentNode(domNode, domNodeOffset);
      nodeCursor = m_kafkaDocument->getNode(domNode);
    }
    else
    {
      m_document->viewCursorIf->cursorPositionReal(&line, &col);
      nodeCursor = parser->nodeAt(line, col, false);
    }

    if (!nodeCursor)
        return;

    nodeParent = nodeCursor;
    if (nodeParent->tag->type == Tag::Text)
        nodeParent = nodeParent->parent;

  //Checking if at least one parent of node can have a Text Node as child, otherwise
  //it is impossible for the
  //user to add this node. In that case, try to insert the Node in the closest parent accepting it.
  //e.g. TR : a normal insertion would require to have the caret in the TABLE Node, but it is
              //impossible
    nodeQTag = QuantaCommon::tagFromDTD(m_document->defaultDTD(),
                                        node->tag->name);

    if (!nodeQTag)
        return;

    qTagList = nodeQTag->parents();
#ifdef HEAVY_DEBUG
    kdDebug(25001)<< "nodeQTag name : " << nodeQTag->name() << endl;
    /**kdDebug(25001)<< nodeQTag->isChild("#text", false) << endl;
    kdDebug(25001)<< nodeQTag->isChild("#text", true) << endl;*/
#endif
    for (qTag = qTagList.first(); qTag; qTag = qTagList.next())
    {
      if (qTag->isChild("#text", false))
          break;
      if (qTag == qTagList.getLast())
          specialTagInsertion = true;
    }

    if (m_currentFocus == QuantaView::VPLFocus)
    {
      m_kafkaDocument->translateKafkaIntoNodeCursorPosition(domNode, domNodeOffset, &dummy, nodeOffset);
      kafkaPart->selection(domStartContainer, startOffset, domEndContainer, endOffset);
      m_kafkaDocument->translateKafkaIntoNodeCursorPosition(domStartContainer, startOffset,
              &startContainer, startOffset);
      m_kafkaDocument->translateKafkaIntoNodeCursorPosition(domEndContainer, endOffset,
              &endContainer,endOffset);
      hasSelection = kafkaPart->hasSelection();
    }
      else
      if (m_document->selectionIfExt)
      {
        m_kafkaDocument->translateQuantaIntoNodeCursorPosition(line, col, &dummy, nodeOffset);
        startCol = m_document->selectionIfExt->selStartCol();
        startLine = m_document->selectionIfExt->selStartLine();
        endCol = m_document->selectionIfExt->selEndCol();
        endLine = m_document->selectionIfExt->selEndLine();
        m_kafkaDocument->translateQuantaIntoNodeCursorPosition((unsigned)startLine, (unsigned)startCol,
                &startContainer, startOffset);
        m_kafkaDocument->translateQuantaIntoNodeCursorPosition((unsigned)endLine, (unsigned)endCol,
                &endContainer, endOffset);
        hasSelection = m_document->selectionIf->hasSelection();
        if (startContainer == endContainer && startContainer->tag->type == Tag::Empty)
        {
          hasSelection = false;
        }
        if (endContainer && endContainer->tag->type == Tag::XmlTag && endOffset < (signed)endContainer->tag->tagStr().length())
        {
          endContainer = endContainer->previousSibling();
          endOffset = (endContainer)?endContainer->tag->tagStr().length():0;
        }
     /**else
        {
          if (startContainer && startContainer->tag->type == Tag::Empty)
            startContainer = startContainer->nextNE();
          if (endContainer && endContainer->tag->type == Tag::Empty)
            endContainer = endContainer->prevNE();
        }*/
      }

      nodeTreeModified = false;
      if (specialTagInsertion)
      {
        //let's try to insert this node in the closest parent accepting it.
        while (nodeParent)
        {
          nodeParentQTag =
                  QuantaCommon::tagFromDTD(m_document->defaultDTD(),
                  nodeParent->tag->name);
          if (nodeParentQTag && nodeParentQTag->isChild(node))
          {
            nodeCursor = kafkaCommon::createMandatoryNodeSubtree(node,
                    m_document);
            nodeOffset = 0;
            kafkaCommon::insertNodeSubtree(node, nodeParent, 0L, 0L, modifs);
            nodeTreeModified = true;
            break;
          }
          nodeParent = nodeParent->parent;
        }
      }
      else if (hasSelection && !nodeQTag->isSingle())
      {
        //If some text is selected in kafka, surround the selection with the new Node.
        if(!startContainer || !endContainer)
            return;
        nodeTreeModified = kafkaCommon::DTDinsertRemoveNode(node, startContainer, (int)startOffset,
                endContainer, (int)endOffset, m_document, &nodeCursor,
                nodeOffset, modifs);
      }
      else
      {
        //Nothing is selected, simply inserting the Node if it is not an inline.
/*            if(!kafkaCommon::isInline(node->tag->name) || nodeQTag->isSingle())
          {*/
        nodeTreeModified = kafkaCommon::DTDinsertRemoveNode(node, nodeCursor, (int)nodeOffset, nodeCursor,
                (int)nodeOffset, m_document, &nodeCursor, nodeOffset, modifs);
//             }
      }

      m_document->docUndoRedo->addNewModifsSet(modifs, undoRedo::NodeTreeModif);
      if (m_currentFocus == QuantaView::VPLFocus)
      {
                //view->reloadVPLView();
                //Now update the VPL cursor position
        m_kafkaDocument->translateNodeIntoKafkaCursorPosition(nodeCursor, nodeOffset, domNode,
                domNodeOffset);
        if (!domNode.isNull() && domNode.nodeType() != DOM::Node::TEXT_NODE &&
            !domNode.firstChild().isNull() && domNode.firstChild().nodeType() ==
            DOM::Node::TEXT_NODE)
            domNode = domNode.firstChild();
        if (!domNode.isNull())
            kafkaPart->setCurrentNode(domNode, domNodeOffset);
      }
      else
      {
        //view->reloadSourceView();
        //Now update the source cursor position
        m_kafkaDocument->translateNodeIntoQuantaCursorPosition(nodeCursor, nodeOffset, line, col);
        m_document->viewCursorIf->setCursorPositionReal(line, col);
      }
      if (!nodeTreeModified)
          quantaApp->slotStatusMsg(i18n("Cannot insert the tag: invalid location."));

    }
    else
    {
      m_document->insertTag(str1, str2);
    }
}


/** Returns the baseURL of the document. */
KURL QuantaView::baseURL()
{
  KURL base;
  if (m_document && !m_document->isUntitled() )
  {
    base = QuantaCommon::convertToPath(m_document->url());
  } else
  {
    base = Project::ref()->projectBaseURL();
  }
  return base;
}

void QuantaView::refreshWindow()
{
  if (!m_document)
  {
    if (m_plugin)
       quantaApp->partManager()->setActivePart(m_plugin->part(), m_plugin->widget());
       resize(width(), height());
  } else
  {
/*
    kdDebug(24000) << "m_documentArea->height(): " << m_documentArea->height() << endl;
    kdDebug(24000) << "ToolbarTabWidget::ref()->height(): " << ToolbarTabWidget::ref()->height() << " hidden: " << ToolbarTabWidget::ref()->isHidden() << " visible: " << ToolbarTabWidget::ref()->isVisible() << endl;
    kdDebug(24000) <<"sum: " << m_documentArea->height() + ToolbarTabWidget::ref()->height() << endl;
    kdDebug(24000) << "height(): " << height() << endl;
    */
    if (m_documentArea->height() + ToolbarTabWidget::ref()->height() - 1 > height() && !ToolbarTabWidget::ref()->isHidden()) //don't use isVisible alone instead of isHidden!
        resize(m_documentArea->width(), m_documentArea->height() - ToolbarTabWidget::ref()->height());
      else if (ToolbarTabWidget::ref()->isHidden())
        resize(width(), height());
  }
}

void QuantaView::activated()
{
  if (!m_document)
  {
    parser->setSAParserEnabled(false);
    quantaApp->slotReloadStructTreeView();
    refreshWindow();
    return;
  }
  ToolbarTabWidget::ref()->reparent(this, 0, QPoint(), qConfig.enableDTDToolbar);
  m_viewLayout->addWidget(ToolbarTabWidget::ref(), 0 , 0);
  quantaApp->partManager()->setActivePart(m_document->doc(), m_document->view());
  m_document->checkDirtyStatus();
  StructTreeView::ref()->useOpenLevelSetting = true;
  quantaApp->slotLoadToolbarForDTD(m_document->getDTDIdentifier());

  //TEMP : If the activated document is not a (X)HTML document, disable smartTagInsertion
  //Will be removed when VPL will support every DTD
  KAction *action = quantaApp->actionCollection()->action("smart_tag_insertion");
  if(action && m_document->defaultDTD()->name.contains("HTML", false) == 0)
  {
    qConfig.smartTagInsertion = false;
    (static_cast<KToggleAction* >(action))->setChecked(false);
  }

  reloadLayout();
  refreshWindow();
 }


void QuantaView::deactivated()
{
  if (m_plugin)
  {
    quantaApp->statusBar()->changeItem("", IDS_STATUS);
  }
  m_sourceUpdateTimer.stop();
  m_VPLUpdateTimer.stop();
}

bool QuantaView::saveModified(bool ask)
{
  if (!m_document)
    return true;

  bool completed=true;
  QString fileName = m_document->url().fileName();

  if (m_document->isModified() )
  {
    if (m_currentFocus == VPLFocus)
      reloadSourceView();
    int want_save;
    if (ask)
      want_save = KMessageBox::warningYesNoCancel(this,
          i18n("The file \"%1\" has been modified.\nDo you want to save it?").arg(fileName),
          i18n("Warning"), KStdGuiItem::save(), KStdGuiItem::discard());
    else
      want_save = KMessageBox::Yes;

    switch (want_save)
    {
      case KMessageBox::Yes :
           if (m_document->isUntitled())
           {
             completed = quantaApp->slotFileSaveAs(this);
           }
           else
           {
             completed = saveDocument(m_document->url());
           };

           break;

      case KMessageBox::No :
           {
      	     m_document->removeBackup(quantaApp->config());
      	     completed=true;
	   }
           break;

      case KMessageBox::Cancel :
           completed=false;
           break;

      default:
           completed=false;
           break;
    }
  } else
    m_document->removeBackup(quantaApp->config());
  return completed;
}

bool QuantaView::saveDocument(const KURL& url)
{
  if (url.isEmpty())
    return false;

  emit eventHappened("before_save", url.url(), QString::null);
  m_saveResult = true;
  KURL oldURL = m_document->url();
  if (!m_document->isUntitled() && oldURL.isLocalFile())
  {
    fileWatcher->removeFile(oldURL.path());
//    kdDebug(24000) << "removeFile[saveDocument]: " << oldURL.path() << endl;
  }
  if (url.isLocalFile())
  {
    if (!m_document->saveAs(url))
    {
      fileWatcher->addFile(oldURL.path());
//      kdDebug(24000) << "addFile[saveDocument]: " << oldURL.path() << endl;
      return false; //saving to a local file failed
    } else //successful saving to a local file
    {
      m_document->setDirtyStatus(false);
      m_document->removeBackup(quantaApp->config());
      fileWatcher->addFile(m_document->url().path());
//      kdDebug(24000) << "addFile[saveDocument, 2]: " << m_document->url().path() << endl;
    }
  } else //saving to a remote file
  {
    KTextEditor::Document *doc = m_document->doc();
    m_eventLoopStarted = false;
    connect(doc, SIGNAL(canceled(const QString &)), this, SLOT(slotSavingFailed(const QString &)));
    connect(doc, SIGNAL(completed()), this, SLOT(slotSavingCompleted()));
    m_saveResult = m_document->saveAs(url);
    if (m_saveResult)
    {
      //start an event loop and wait until the saving finished
      QExtFileInfo internalFileInfo;
      m_eventLoopStarted = true;
      internalFileInfo.enter_loop();
    }
    disconnect(doc, SIGNAL(canceled(const QString &)), this, SLOT(slotSavingFailed(const QString &)));
    disconnect(doc, SIGNAL(completed()), this, SLOT(slotSavingCompleted()));
    if (!m_saveResult) //there was an error while saving
    {
      if (oldURL.isLocalFile())
      {
        fileWatcher->addFile(oldURL.path());
//        kdDebug(24000) << "addFile[saveDocument, 3]: " << oldURL.path() << endl;
      }
      return false;
    }
  }
  // everything went fine
  if (oldURL != m_document->url())
  {
      setCaption(m_document->url().fileName());
  }
  emit eventHappened("after_save", m_document->url().url(), QString::null);
  return true;
}

void QuantaView::slotSavingFailed(const QString &error)
{
  Q_UNUSED(error);
  m_saveResult = false;
  if (m_eventLoopStarted)
    qApp->exit_loop();
}

void QuantaView::slotSavingCompleted()
{
  m_saveResult = true;
  m_document->setDirtyStatus(false);
  m_document->removeBackup(quantaApp->config());
  if (m_eventLoopStarted)
    qApp->exit_loop();
}

#include "quantaview.moc"
