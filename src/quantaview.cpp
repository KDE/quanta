/***************************************************************************
                          quantaview.cpp  -  description
                             -------------------
    begin                : ���� 9 13:29:57 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com>
                           (C) 2001-2004 Andras Mantia <amantia@kde.org>
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
#include <klocale.h>
#include <kstandarddirs.h>
#include <kmenubar.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kurldrag.h>
#include <kdeversion.h>
#include <kparts/partmanager.h>
#include <kstatusbar.h>

#ifdef BUILD_KAFKAPART
#include "undoredo.h"
#include "wkafkapart.h"
#endif

#include <ktexteditor/document.h>
#include <ktexteditor/view.h>
#include <ktexteditor/viewcursorinterface.h>

// application specific includes
#include "document.h"
#include "resource.h"
#include "quantaview.h"
#include "quanta.h"
#include "quantacommon.h"
#include "qextfileinfo.h"

#include "tagaction.h"
#include "toolbartabwidget.h"
#include "quantaplugin.h"
#include "quantaplugininterface.h"
#include "project.h"
#include "structtreeview.h"

#include "tagdialog.h"

QuantaView::QuantaView(QWidget *parent, const char *name )
  : KMdiChildView(parent, name)
  , m_document(0L)
  , m_plugin(0L)
  , m_customWidget(0L)
#ifdef BUILD_KAFKAPART
  , m_currentFocus(SourceFocus)
#endif
{
#ifdef BUILD_KAFKAPART
//Connect the VPL update timers
  connect(&m_sourceUpdateTimer, SIGNAL(timeout()), this, SLOT(sourceUpdateTimerTimeout()));
  connect(&m_VPLUpdateTimer, SIGNAL(timeout()), this, SLOT(VPLUpdateTimerTimeout()));
#endif

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

  setAcceptDrops(TRUE); // [MB02] Accept drops on the view
}

QuantaView::~QuantaView()
{
   delete m_document;
   m_document = 0L;
}

bool QuantaView::mayRemove()
{
   if (m_plugin)
   {
       m_plugin->unload(false);
   } else
   {
     if (m_customWidget)
         m_customWidget->reparent(0L, 0, QPoint(), false);
     parser->setSAParserEnabled(false);
     if (!saveModified())
          return false;
      if (m_document)
      {
        m_currentViewsLayout  = -1;
        slotSetSourceLayout(); //set the layout to source only, otherwise it crashes...
        m_kafkaDocument->getKafkaWidget()->view()->reparent(0, 0, QPoint(), false);
          m_document->closeTempFile();
          if (!m_document->isUntitled() && m_document->url().isLocalFile())
            fileWatcher->removeFile(m_document->url().path());
          quantaApp->guiFactory()->removeClient(m_document->view());
      }
      kdDebug(24000) << "Calling reparse from close " << endl;
      parser->setSAParserEnabled(true);
      quantaApp->reparse(true);
   }
   return true;
}

void QuantaView::addDocument(Document *document)
{
   if (!document)
     return;
   m_document = document;
   connect(m_document, SIGNAL(editorGotFocus()),
                  this, SLOT(slotSourceGetFocus()));
    connect(m_document->view(), SIGNAL(cursorPositionChanged()), this, SIGNAL(cursorPositionChanged()));


#ifdef BUILD_KAFKAPART
   m_kafkaDocument =KafkaDocument::ref();

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
#ifdef BUILD_KAFKAPART
  m_currentViewsLayout = SourceOnly;//to correctly reload the timers.

  reloadUpdateTimers();
  m_kafkaDocument->readConfig(quantaApp->config());
#endif

#endif

   m_currentViewsLayout = -1; //force loading of this layout
   int currentViewsLayout = SourceOnly;
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

void QuantaView::addPlugin(QuantaPlugin *plugin)
{
   ToolbarTabWidget *m_toolbarTab = ToolbarTabWidget::ref(this);
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
      m_customWidget = 0L; //avoid infinite recursion
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
   m_customWidget = widget;
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
                setIcon(UserIcon("save_small"));
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

#ifdef BUILD_KAFKAPART
   if(m_currentViewsLayout == SourceAndVPL)
     m_splitterSizes = m_splitter->sizes();
#endif

   KToggleAction *ta = (KToggleAction *) quantaApp->actionCollection()->action( "show_quanta_editor" );
   if (ta)
      ta->setChecked(true);

#ifdef BUILD_KAFKAPART
   //hide the VPL widget, reload the source if necessary
   if ((m_currentViewsLayout == SourceAndVPL && m_kafkaDocument->getKafkaWidget()->view()->hasFocus()) ||
     m_currentViewsLayout == VPLOnly)
   {
          reloadSourceView();
   }
   if (m_kafkaDocument->isLoaded())
       m_kafkaDocument->unloadDocument();
#endif
//show the document if full size
   m_splitter->hide();
   m_document->view()->reparent(m_documentArea, 0, QPoint(), true);
   m_document->view()->resize(m_documentArea->size());
   m_viewLayout->addWidget(m_documentArea, 1, 0);
   m_document->view()->setFocus();

   m_currentViewsLayout = SourceOnly;

#ifdef BUILD_KAFKAPART
//update timers are not needed in source only mode
   m_sourceUpdateTimer.stop();
   m_VPLUpdateTimer.stop();
#endif
}


void QuantaView::slotSetSourceAndVPLLayout()
{
#ifdef BUILD_KAFKAPART
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
        quantaApp->slotDocumentProperties();
    }

   m_currentViewsLayout = SourceAndVPL;

   reloadUpdateTimers();
#endif
}

void QuantaView::slotSetVPLOnlyLayout()
{
#ifdef BUILD_KAFKAPART
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
      quantaApp->slotDocumentProperties();
   }

   m_currentViewsLayout = VPLOnly;

//update timers are not needed in VPL only mode
    m_sourceUpdateTimer.stop();
    m_VPLUpdateTimer.stop();

#endif
}

void QuantaView::reloadUpdateTimers()
{
#ifdef BUILD_KAFKAPART
    m_sourceUpdateTimer.stop();
    m_VPLUpdateTimer.stop();

   if (m_kafkaDocument->isLoaded() && m_currentViewsLayout == SourceAndVPL)
   {
       if (m_currentFocus == VPLFocus && !qConfig.quantaRefreshOnFocus)
          m_sourceUpdateTimer.start(qConfig.quantaRefreshDelay);
       if (m_currentFocus == SourceFocus && !qConfig.kafkaRefreshOnFocus)
          m_VPLUpdateTimer.start(qConfig.kafkaRefreshDelay);
   }
#endif
}

void QuantaView::slotVPLGetFocus(bool focus)
{
#ifdef BUILD_KAFKAPART
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


    m_currentFocus = VPLFocus;
    reloadUpdateTimers();
 }
#endif
}

void QuantaView::slotSourceGetFocus()
{
#ifdef BUILD_KAFKAPART
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

#endif
}

#ifdef BUILD_KAFKAPART

/** Reloads both views ONLY when changes have been made to the Node tree ONLY. */
void QuantaView::reloadBothViews(bool force)
{
  reloadSourceView(force);
  reloadVPLView(force);
}

/** reload the Kafka view from the Node Tree. Set force to true if you want to reload even if not necessary. */
void QuantaView::reloadVPLView(bool force)
{
  if (m_kafkaReloadingEnabled || force)
      m_document->docUndoRedo->reloadKafkaEditor(force);
}

/** reload the Quanta view from the Node Tree. Set force to true if you want to reload even if not necessary. */
void QuantaView::reloadSourceView(bool force)
{
  if (m_quantaReloadingEnabled || force)
      m_document->docUndoRedo->reloadQuantaEditor(force);
}

#endif

void QuantaView::VPLUpdateTimerTimeout()
{
#ifdef BUILD_KAFKAPART
  if(m_currentFocus == SourceFocus)
    reloadVPLView();
#endif
}

void QuantaView::sourceUpdateTimerTimeout()
{
#ifdef BUILD_KAFKAPART
  if(m_currentFocus == VPLFocus)
    reloadSourceView();
#endif
}

void QuantaView::slotVPLLoadingError(Node *)
{
   emit showProblemsView();
}


void QuantaView::slotSetCursorPositionInSource(int col, int line)
{
#ifdef BUILD_KAFKAPART
  m_curCol = col;
  m_curLine = line;
  if (m_currentViewsLayout == SourceAndVPL || m_currentViewsLayout == SourceOnly)
    m_document->viewCursorIf->setCursorPositionReal(line, col);
#endif
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
#ifdef BUILD_KAFKAPART
  else
  if (m_currentViewsLayout == VPLOnly)
    m_kafkaDocument->getKafkaWidget()->view()->resize(width,height);
  else
  if (m_currentViewsLayout == SourceAndVPL)
  {
    m_splitter->resize(width, height);
    m_splitterSizes = m_splitter->sizes();
  }
#endif
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
    TagDialog *dlg = new TagDialog(QuantaCommon::tagFromDTD(m_document->getDTDIdentifier(), tag), attr, baseURL());
    if (dlg->exec())
    {
      dlg->insertTag(m_document, insertInLine);
    }

    delete dlg;
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

void QuantaView::activated()
{
  if (!m_document)
  {
    parser->setSAParserEnabled(false);
    quantaApp->slotReloadStructTreeView();
    if (m_plugin)
       quantaApp->partManager()->setActivePart(m_plugin->part(), m_plugin->widget());
       resize(width(), height());
    return;
  }
  ToolbarTabWidget::ref()->reparent(this, 0, QPoint(), qConfig.enableDTDToolbar);
  m_viewLayout->addWidget(ToolbarTabWidget::ref(), 0 , 0);
  quantaApp->partManager()->setActivePart(m_document->doc(), m_document->view());
  m_document->checkDirtyStatus();
  StructTreeView::ref()->useOpenLevelSetting = true;
  quantaApp->loadToolbarForDTD(m_document->getDTDIdentifier());
  switch (m_currentViewsLayout)
  {
      case SourceOnly:
        {
           m_currentViewsLayout = -1;
           slotSetSourceLayout();
           break;
        }
      case SourceAndVPL:
        {
           m_currentViewsLayout = -1;
           slotSetSourceAndVPLLayout();
           break;
        }
      case VPLOnly:
        {
            m_currentViewsLayout = -1;
            slotSetVPLOnlyLayout();
            break;
        }
  }
  if (m_documentArea->height() + ToolbarTabWidget::ref()->height() > height())
    resize(m_documentArea->width(), m_documentArea->height() - ToolbarTabWidget::ref()->height());
 }


void QuantaView::deactivated()
{
  if (m_plugin)
  {
      KToggleAction* showStatusbarAction = (KToggleAction *) quantaApp->actionCollection()->action("view_statusbar");
      if (showStatusbarAction->isChecked())
      {
        showStatusbarAction->setChecked(true);
        quantaApp->statusBar()->changeItem("", IDS_STATUS);
        quantaApp->statusBar()->show();
      }
  }
}

bool QuantaView::saveModified()
{
  if (!m_document)
    return true;

  bool completed=true;
  QString fileName = m_document->url().fileName();

  if (m_document->isModified() )
  {
    int want_save
      = KMessageBox::warningYesNoCancel(this,
          i18n("The file \"%1\" has been modified.\nDo you want to save it?").arg(fileName),
          i18n("Warning"));

    switch (want_save)
    {
      case KMessageBox::Yes :
           if (m_document->isUntitled())
           {
             completed = quantaApp->slotFileSaveAs();
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
  }
  return completed;
}

bool QuantaView::saveDocument(const KURL& url)
{
  if (url.isEmpty())
    return false;

  m_saveResult = true;
  KURL oldURL = m_document->url();
  if (oldURL.isLocalFile())
    fileWatcher->removeFile(oldURL.path());

  if (url.isLocalFile())
  {
    if (!m_document->doc()->saveAs(url))
    {
#if KDE_VERSION < KDE_MAKE_VERSION(3,1,90)
      KMessageBox::error(this, i18n("<qt>Saving of the document <b>%1</b> failed.<br>Maybe you should try to save in another directory.</qt>").arg(url.prettyURL(0, KURL::StripFileProtocol)));
#endif
      fileWatcher->addFile(oldURL.path());
      return false; //saving to a local file failed
    } else //successful saving to a local file
    {
      m_document->closeTempFile();
      m_document->createTempFile();
      m_document->setDirtyStatus(false);
      fileWatcher->addFile(m_document->url().path());
    }
  } else //saving to a remote file
  {
    KTextEditor::Document *doc = m_document->doc();
    m_eventLoopStarted = false;
    connect(doc, SIGNAL(canceled(const QString &)), this, SLOT(slotSavingFailed(const QString &)));
    connect(doc, SIGNAL(completed()), this, SLOT(slotSavingCompleted()));
    m_saveResult = doc->saveAs(url);
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
        fileWatcher->addFile(oldURL.path());
      return false;
    }
  }
  // everything went fine
  if (oldURL != m_document->url())
  {
      setCaption(m_document->url().fileName());
  }
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
  m_document->closeTempFile();
  m_document->createTempFile();
  m_document->setDirtyStatus(false);
  if (m_eventLoopStarted)
    qApp->exit_loop();
}



#include "quantaview.moc"
