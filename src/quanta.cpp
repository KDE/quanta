/***************************************************************************
                          quanta.cpp  -  description
                             -------------------
    begin                : ?? ???  9 13:29:57 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com>
                           (C) 2001-2004 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <time.h>

// include files for QT
#include <qaction.h>
#include <qdragobject.h>
#include <qdir.h>
#include <qprinter.h>
#include <qpainter.h>
#include <qwidgetstack.h>
#include <qtabwidget.h>
#include <qfile.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qtabbar.h>
#include <qradiobutton.h>
#include <qimage.h>
#include <qtimer.h>
#include <qtextstream.h>
#include <qtextedit.h>
#include <qiodevice.h>
#include <qcombobox.h>
#include <qdockarea.h>
#include <qdom.h>
#include <qspinbox.h>
#include <qeventloop.h>
#include <qfontmetrics.h>
#include <qclipboard.h>
#include <qptrlist.h>
#include <qbuffer.h>
#include <qdatetime.h>


// include files for KDE
#include <kapplication.h>
#include <kaboutdata.h>
#include <kbugreport.h>
#include <kcolordialog.h>
#include <kcombobox.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kconfig.h>
#include <khtmlview.h>
#include <kdialogbase.h>
#include <kdeversion.h>
#include <kkeydialog.h>
#include <kinputdialog.h>
#include <klibloader.h>
#include <kdockwidget.h>
#include <kstatusbar.h>
#include <kpopupmenu.h>
#include <kpushbutton.h>
#include <kprocess.h>
#include <kprogress.h>
#include <ktempdir.h>
#include <ktempfile.h>
#include <kdebug.h>
#include <ktar.h>
#include <kedittoolbar.h>
#include <kaction.h>
#include <kcharsets.h>
#include <kdirwatch.h>
#include <kspell.h>
#include <kstandarddirs.h>
#include <ktabwidget.h>
#include <ktip.h>
#include <kmimetype.h>
#include <kparts/partmanager.h>
#include <kparts/part.h>
#include <kstringhandler.h>
#include <kstdguiitem.h>

#include <ktexteditor/editinterface.h>
#include <ktexteditor/encodinginterface.h>
#include <ktexteditor/selectioninterface.h>
#include <ktexteditor/markinterface.h>
#include <ktexteditor/viewcursorinterface.h>
#include <ktexteditor/printinterface.h>
#include <ktexteditor/popupmenuinterface.h>
#include <ktexteditor/dynwordwrapinterface.h>
#include <ktexteditor/encodinginterface.h>
#include <ktexteditor/undointerface.h>
#include <ktexteditor/view.h>
#include <ktexteditor/clipboardinterface.h>

#include <kio/netaccess.h>

#ifdef ENABLE_CVSSERVICE
#include "cvsservice.h"
#endif


// application specific includes
#include "document.h"
#include "quanta.h"
#include "quantaview.h"
#include "quantadoc.h"
#include "qextfileinfo.h"
#include "resource.h"

#include "project.h"

// For Kafka cut/copy/paste
#include "wkafkapart.h"

#include "whtmlpart.h"

#include "abbreviation.h"
#include "filemasks.h"
#include "styleoptionss.h"
#include "previewoptions.h"
#include "parseroptions.h"
#include "dtdselectdialog.h"
#include "donationdialog.h"
#include "fourbuttonmessagebox.h"
#include "specialchardialog.h"
#include "kafkasyncoptions.h"
#include "htmldocumentproperties.h"
#include "undoredo.h"

#include "filestreeview.h"
#include "structtreeview.h"
#include "doctreeview.h"
#include "templatestreeview.h"
#include "tagattributetree.h"
#include "projecttreeview.h"
#include "scripttreeview.h"
#include "servertreeview.h"
#include "variableslistview.h"
#include "debuggerbreakpointview.h"

#include "listdlg.h"
#include "tagdialog.h"
#include "tagmaildlg.h"
#include "tagmisc.h"
#include "tagquicklistdlg.h"
#include "tableeditor.h"

#include "csseditor.h"
#include "cssselector.h"

#include "spellchecker.h"
#include "framewizard.h"

#include "debuggermanager.h"

#include "parser.h"
#include "dtdparser.h"

#include "messageoutput.h"

#include "actionconfigdialog.h"
#include "toolbarxmlgui.h"
#include "tagaction.h"
#include "toolbartabwidget.h"
#include "dcopquanta.h"

#include <kencodingfiledialog.h>

#include "quantaplugininterface.h"
#include "quantaplugin.h"
#include "dtds.h"
#include "dcopsettings.h"
#include "spellchecker.h"
#include "quanta_init.h"
#include "viewmanager.h"
#include "debuggerui.h"
#include "newstuff.h"
#include "quantanetaccess.h"

const QString resourceDir = QString(QUANTA_PACKAGE) + "/";

// from kfiledialog.cpp - avoid qt warning in STDERR (~/.xsessionerrors)
static void silenceQToolBar(QtMsgType, const char *){}

QuantaApp::QuantaApp(int mdiMode) : DCOPObject("WindowManagerIf"), KMdiMainFrm( 0, "Quanta", (KMdi::MdiMode) mdiMode)
{
  setStandardToolBarMenuEnabled(true);
  createStandardStatusBarAction();
  m_quantaInit = new QuantaInit(this);
  dcopSettings = new DCOPSettings;
  dcopQuanta = new DCOPQuanta;
  quantaStarted = true;
  tempFileList.setAutoDelete(true);
  toolbarList.setAutoDelete(true);
  userToolbarsCount = 0;
  baseNode = 0L;
  m_newDTEPStuff = 0L;
  m_newToolbarStuff = 0L;
  currentToolbarDTD = QString::null;
  m_config=kapp->config();
  idleTimer = new QTimer(this);
  connect(idleTimer, SIGNAL(timeout()), SLOT(slotIdleTimerExpired()));
  m_idleTimerEnabled = true;

  qConfig.globalDataDir = KGlobal::dirs()->findResourceDir("data",resourceDir + "toolbar/quantalogo.png");
  if (qConfig.globalDataDir.isEmpty())
  {
    quantaStarted = false;
    kdWarning() <<  "***************************************************************************" << endl;
    kdWarning() << i18n("Quanta data files were not found.") << endl;
    kdWarning() << i18n("You may have forgotten to run \"make install\", or your KDEDIR, KDEDIRS or PATH are not set correctly.") << endl;
    kdWarning() << "***************************************************************************" << endl;
    QTimer::singleShot(20, kapp, SLOT(quit()));
    return;
  }
  qConfig.enableDTDToolbar = true;

  exitingFlag = false;
  qConfig.spellConfig = new KSpellConfig();

  // connect up signals from KXXsldbgPart
  connectDCOPSignal(0, 0, "debuggerPositionChangedQString,int)", "newDebuggerPosition(QString,int)", false );
  connectDCOPSignal(0, 0, "editorPositionChanged(QString,int,int)", "newCursorPosition(QString,int,int)", false );
  connectDCOPSignal(0, 0, "openFile(QString,int,int)", "openFile(QString,int,int)", false);

  m_execCommandPS = 0L;

  m_partManager = new KParts::PartManager(this);
  // When the manager says the active part changes,
  // the builder updates (recreates) the GUI
  connect(m_partManager, SIGNAL(activePartChanged(KParts::Part * )),
          this, SLOT(slotActivePartChanged(KParts::Part * )));
   connect(this, SIGNAL(dockWidgetHasUndocked(KDockWidget *)), this, SLOT(slotDockWidgetHasUndocked(KDockWidget *)));
  connect(tabWidget(), SIGNAL(initiateDrag(QWidget *)), this, SLOT(slotTabDragged(QWidget*)));

  m_oldKTextEditor = 0L;
  m_previewToolView = 0L;
  m_documentationToolView = 0L;
  m_previewedDocument = 0L;
  m_previewVisible =  false;
  m_cvsMenuId = -1;
  emit eventHappened("quanta_start", QDateTime::currentDateTime().toString(Qt::ISODate), QString::null);
}


QuantaApp::~QuantaApp()
{
  disconnect(m_htmlPart, SIGNAL(destroyed(QObject *)));
  disconnect(m_htmlPartDoc, SIGNAL(destroyed(QObject *)));
  disconnect(this, SIGNAL(lastChildViewClosed()), ViewManager::ref(), SLOT(slotLastViewClosed()));
// kdDebug(24000) << "QuantaApp::~QuantaApp" << endl;
#ifdef ENABLE_CVSSERVICE
 delete CVSService::ref();
#endif
 delete m_debugger;
 m_debugger = 0L;
 quantaApp = 0L;
 delete m_doc;
 m_doc = 0L;
 delete parser;
 parser = 0L;
 delete idleTimer;
 idleTimer = 0L;
 tempFileList.clear();
 for (uint i = 0; i < tempDirList.count(); i++)
 {
    KIO::NetAccess::del(KURL().fromPathOrURL(tempDirList.at(i)->name()), this);
 }
 tempDirList.clear();
 QDictIterator<ToolbarEntry> iter(toolbarList);
 ToolbarEntry *p_toolbar;
 for( ; iter.current(); ++iter )
 {
   p_toolbar = iter.current();
   delete p_toolbar->dom;
   delete p_toolbar->menu;
   delete p_toolbar->guiClient;
 }

 toolbarList.clear();
 QStringList tmpDirs = KGlobal::dirs()->resourceDirs("tmp");
 tmpDir = tmpDirs[0];
 for (uint i = 0; i < tmpDirs.count(); i++)
 {
   if (tmpDirs[i].contains("kde-"))
      tmpDir = tmpDirs[i];
 }
 QString infoCss = tmpDir;
 infoCss += "quanta/info.css";
 QFile::remove(infoCss);
 QDir dir;
 dir.rmdir(tmpDir + "quanta");

 delete m_execCommandPS;
 m_execCommandPS = 0L;
 delete dcopSettings;
 delete dcopQuanta;
 delete m_partManager;
}

void QuantaApp::setTitle(const QString& title)
{
  setCaption( "  [  "+title+"  ]  " );
}

void QuantaApp::slotFileNew()
{
    m_doc->openDocument( KURL() );
}

void QuantaApp::slotFileOpen()
{
 QString myEncoding = defaultEncoding();
 QString startDir;
 Document *w = ViewManager::ref()->activeDocument();
 if (w && !w->isUntitled())
     startDir = w->url().url();
 else
     startDir = Project::ref()->projectBaseURL().url();

 KEncodingFileDialog::Result data;
 data = KEncodingFileDialog::getOpenURLsAndEncoding(myEncoding, startDir,
        "all/allfiles text/plain", this, i18n("Open File"));
 slotFileOpen(data.URLs, data.encoding);
}

void QuantaApp::slotFileOpen( const KURL::List &urls, const QString& encoding )
{
 m_doc->blockSignals(true);
 for (KURL::List::ConstIterator i=urls.begin(); i != urls.end(); ++i)
 {
   if (QuantaCommon::checkMimeGroup(*i, "text") ||
       QuantaCommon::denyBinaryInsert() == KMessageBox::Yes)
     slotFileOpen( *i , encoding);
 }
 m_doc->blockSignals(false);
 Document *w = ViewManager::ref()->activeDocument();
 if (w)
   setCaption(w->url().prettyURL() );
 //slotUpdateStatus(w);//FIXME:
}

void QuantaApp::slotFileOpen(const KURL &url)
{
  slotFileOpen(url, defaultEncoding());
}

void QuantaApp::slotFileOpen( const KURL &url, const QString& encoding )
{
  m_doc->openDocument( url, encoding );

//  slotUpdateStatus(m_view->write()); //FIXME:
}

void QuantaApp::slotFileOpenRecent(const KURL &url)
{
  if (!QExtFileInfo::exists(url))
  {
    if (KMessageBox::questionYesNo( this,
        i18n("The file %1 does not exist.\n Do you want to remove it from the list?").arg(url.prettyURL(0, KURL::StripFileProtocol)) )
        == KMessageBox::Yes)
    {
      fileRecent->removeURL(url);
    }
  } else
  if (QuantaCommon::checkMimeGroup(url, "text") ||
      QuantaCommon::denyBinaryInsert() == KMessageBox::Yes)
  {
    slotFileOpen(url);
  }
  fileRecent->setCurrentItem(-1);
  ViewManager::ref()->activeDocument()->view()->setFocus();
}

void QuantaApp::slotFileSave()
{
  QuantaView* view=ViewManager::ref()->activeView();
  Document *w = view->document();
  if (w)
  {
    w->checkDirtyStatus();
    if ( w->isUntitled() )
      slotFileSaveAs();
    else
    {
      if(ViewManager::ref()->activeView() &&
            ViewManager::ref()->activeView()->hadLastFocus() == QuantaView::VPLFocus)
        w->docUndoRedo->reloadQuantaEditor();
      view->saveDocument(w->url());
      w->docUndoRedo->fileSaved();
    }
  }
}

bool QuantaApp::slotFileSaveAs()
{
  bool result = false;
  QuantaView* view=ViewManager::ref()->activeView();
  Document *w = view->document();
  if (w)
  {
    KURL oldURL = w->url();
    w->checkDirtyStatus();
    if (!w->isUntitled() && oldURL.isLocalFile())
    {
        fileWatcher->removeFile(oldURL.path());
        kdDebug(24000) << "removeFile[slotFileSaveAs]: " << oldURL.path() << endl;
    }
    QString myEncoding =  dynamic_cast<KTextEditor::EncodingInterface*>(w->doc())->encoding();

    bool gotPath = false;

    KURL saveAsUrl;

    if(fTab->isVisible())
    {
      saveAsUrl = fTab->currentURL();
      if (fTab->currentKFileTreeViewItem() && fTab->currentKFileTreeViewItem()->isDir())
      {
        saveAsUrl.adjustPath(+1);
      }
      gotPath = true;
    } else
    if(ProjectTreeView::ref()->isVisible())
    {
      saveAsUrl = ProjectTreeView::ref()->currentURL();
      if (ProjectTreeView::ref()->currentKFileTreeViewItem() && ProjectTreeView::ref()->currentKFileTreeViewItem()->isDir())
      {
        saveAsUrl.adjustPath(+1);
      }
      gotPath = true;
    }
    if (!gotPath || saveAsUrl.isEmpty())
    {
      if (w->isUntitled())
      {
        saveAsUrl = Project::ref()->projectBaseURL();
        saveAsUrl.adjustPath(+1);
        saveAsUrl.setFileName( oldURL.fileName() );
      } else
        saveAsUrl = oldURL;
    }

    KEncodingFileDialog::Result data;
    data = KEncodingFileDialog::getSaveURLAndEncoding(myEncoding, saveAsUrl.url(),
             "all/allfiles text/plain", this, i18n("Save File"));
    KURL saveUrl = data.URLs[0];
    QString encoding = data.encoding;
    KTextEditor::EncodingInterface* encodingIf = dynamic_cast<KTextEditor::EncodingInterface*>(w->doc());
    if (encodingIf)
       encodingIf->setEncoding(encoding);

    if (QuantaCommon::checkOverwrite(saveUrl) && view->saveDocument(saveUrl))
    {
      oldURL = saveUrl;
      if (  Project::ref()->hasProject() && !Project::ref()->contains(saveUrl)  &&
           KMessageBox::Yes == KMessageBox::questionYesNo(0,i18n("<qt>Do you want to add the<br><b>%1</b><br>file to project?</qt>").arg(saveUrl.prettyURL(0, KURL::StripFileProtocol)))
        )
      {
        if (saveUrl.isLocalFile())
        {
          QDir dir(saveUrl.path());
          saveUrl.setPath(dir.canonicalPath());
        }
        Project::ref()->insertFile(saveUrl, true);
      }
      if(ViewManager::ref()->activeView() &&
            ViewManager::ref()->activeView()->hadLastFocus() == QuantaView::VPLFocus)
        w->docUndoRedo->reloadQuantaEditor();

      w->docUndoRedo->fileSaved();
//      slotUpdateStatus(w); //FIXME:
      result = true;
    }
    if (oldURL.isLocalFile())
    {
        fileWatcher->addFile(oldURL.path());
        kdDebug(24000) << "addFile[slotFileSaveAs]: " << oldURL.path() << endl;
    }
  }
  return result;
}

void QuantaApp::saveAsTemplate(bool projectTemplate,bool selectionOnly)
{
  QuantaView *view = ViewManager::ref()->activeView();
  Document *w = view->document();
  if (!w) return;

  KURL url;
  int query;
  KURL projectTemplateURL;
  w->checkDirtyStatus();
  QString localTemplateDir = locateLocal("data",resourceDir + "templates/");

  do {
    query = KMessageBox::Yes;

    if (projectTemplate)
    {
      url = KFileDialog::getSaveURL(Project::ref()->templateURL().url(), QString::null, this);
    } else
    {
      url = KFileDialog::getSaveURL(locateLocal("data", resourceDir + "templates/"), QString::null, this);
    }

    if (url.isEmpty()) return;

    if (Project::ref()->hasProject())
        projectTemplateURL = Project::ref()->templateURL();
    if ( ((projectTemplate) && (projectTemplateURL.isParentOf(url)) ) ||
          ((! projectTemplate) && (KURL(localTemplateDir).isParentOf(url))) )
    {
      if (!QuantaCommon::checkOverwrite(url))
        query = KMessageBox::No;
    } else
    {
      if (projectTemplate)
          localTemplateDir = projectTemplateURL.path(1);
      KMessageBox::sorry(this,i18n("You must save the templates in the following folder: \n\n%1").arg(localTemplateDir));
      query = KMessageBox::No;
    }
  } while (query != KMessageBox::Yes);

  if( query == KMessageBox::Cancel ) return;

  KTempFile *tempFile = new KTempFile(tmpDir);
  tempFile->setAutoDelete(true);
  if (selectionOnly)
  {
    QString content;
    content = w->selectionIf->selection();
    QTextStream stream(tempFile->file());
    stream.setEncoding(QTextStream::UnicodeUTF8);
    stream << content;
    tempFile->file()->flush();
    tempFile->close();
    if (!QExtFileInfo::copy(KURL::fromPathOrURL(tempFile->name()), url, -1, true, false,  this))
      KMessageBox::error(this, i18n("<qt>There was an error while creating the template file.<br>Check that you have write access to <i>%1</i>.</qt>").arg(url.prettyURL(0, KURL::StripFileProtocol)), i18n("Template Creation Error"));
    delete tempFile;
  } else
  {
    view->saveDocument(url);
  }

  if (projectTemplate)
      Project::ref()->insertFile(url, true);
  if(ViewManager::ref()->activeView() &&
        ViewManager::ref()->activeView()->hadLastFocus() == QuantaView::VPLFocus)
    w->docUndoRedo->reloadQuantaEditor();

  w->docUndoRedo->fileSaved();
}

void QuantaApp::slotFileSaveAsLocalTemplate()
{
  saveAsTemplate(false);
}

void QuantaApp::slotFileSaveAsProjectTemplate()
{
  saveAsTemplate(true);
}


void QuantaApp::slotFileSaveSelectionAsLocalTemplate()
{
  saveAsTemplate(false, true);
}

void QuantaApp::slotFileSaveSelectionAsProjectTemplate()
{
  saveAsTemplate(true, true);
}

void QuantaApp::slotFileSaveAll()
{
  ViewManager::ref()->saveAll();
//FIXME:
  /*
  if (m_view->writeExists())
    slotUpdateStatus(m_view->write()); */
}

void QuantaApp::slotFileReload(QuantaView *view)
{
  if (!view)
     view = ViewManager::ref()->activeView();
  Document *w = view->document();
  if (!w || !view->saveModified())
      return;
  if (!w->isUntitled() && w->isModified())
  {
    w->setModified(false);
    w->doc()->openURL(w->url());
    reparse(true);
  }
}

void QuantaApp::slotFileReloadAll()
{
//TODO: Implement it!
}

void QuantaApp::slotFileClose()
{
   ViewManager::ref()->removeActiveView();
}

void QuantaApp::slotFileClose(const KURL &url)
{
  QuantaView *view = ViewManager::ref()->isOpened(url);
  if (view)
  {
    ViewManager::ref()->removeView(view);
  }
}


void QuantaApp::slotFileCloseAll()
{

  ViewManager::ref()->closeAll();
  WHTMLPart *part = m_htmlPart;
  part->closeURL();
  part->begin(Project::ref()->projectBaseURL());
  part->write(" ");
  part->end();

  slotNewStatus();
}

void QuantaApp::slotFileQuit()
{
  close();
}


void QuantaApp::slotEditFindInFiles()
{
  QuantaPlugin *fileReplacePlugin = m_pluginInterface->plugin("KFileReplace");
  if (fileReplacePlugin)
    fileReplacePlugin->run();
}


void QuantaApp::slotHelpTip()
{
   KTipDialog::showTip(this, QString::null, true);
}

void QuantaApp::slotStatusMsg(const QString &msg)
{
  statusbarTimer->stop();
  statusBar()->changeItem(" " + KStringHandler::cPixelSqueeze(msg, statusBar()->fontMetrics(), progressBar->x() - 20), IDS_STATUS);
  statusBar()->repaint();
  kapp->processEvents(QEventLoop::ExcludeUserInput | QEventLoop::ExcludeSocketNotifiers);
  statusbarTimer->start(10000, true);
}

/** repaint preview */
void QuantaApp::slotRepaintPreview()
{
  Document *w = ViewManager::ref()->activeDocument();
  if (!w) return;
  if (!m_previewVisible) return;
  if (!m_previewToolView && qConfig.previewPosition != "Editor" ) return;

  m_previewedDocument = 0L;
  previewCopyMade = false;

  KHTMLView *html = m_htmlPart->view();
  int xOffset = html->contentsX(), yOffset = html->contentsY();

  m_htmlPart->closeURL();
  KParts::BrowserExtension *browserExtension = KParts::BrowserExtension::childObject(m_htmlPart);
  KParts::URLArgs  args(true, browserExtension->xOffset(), browserExtension->yOffset());
  browserExtension->setURLArgs( args );
  QString encoding = defaultEncoding();
  KTextEditor::EncodingInterface* encodingIf = dynamic_cast<KTextEditor::EncodingInterface*>(w->doc());
  if (encodingIf)
      encoding = encodingIf->encoding();

  KURL url;
  m_htmlPart->setEncoding(encoding, true);
  QStringList list;
  if (m_noFramesPreview)
  {
    list = w->tagAreas("frameset", true, true);
    if (list.isEmpty() || w->editIf->text().isEmpty())
      m_noFramesPreview = false;
    else
    {
      QStringList noframearea =  w->tagAreas("noframes", false, true);
      //find the frameset area
      int bl, bc, el, ec;
      QStringList l = QStringList::split('\n', list[0], true);
      QStringList coordList = QStringList::split(',', l[0], true);
      bl = coordList[0].toInt();
      bc = coordList[1].toInt();
      el = coordList[2].toInt();
      ec = coordList[3].toInt();
      QString noFramesText = w->text(0,0, bl, bc - 1);
      noFramesText += noframearea[0];
      noFramesText += w->text(el, ec + 1, w->editIf->numLines() - 1, w->editIf->lineLength(w->editIf->numLines() - 1));
      noFramesText.replace(QRegExp("</?noframes[^>]*>", false), "");
      //kdDebug(24000) << "NOFRAMES: " << noFramesText << endl;
      if (w->isUntitled())
        m_htmlPart->begin( Project::ref()->projectBaseURL(), xOffset, yOffset );
      else
        m_htmlPart->begin( w->url(), xOffset, yOffset );
      m_htmlPart->write(noFramesText);
      m_htmlPart->end();
    }
  }

  if (!m_noFramesPreview)
  {
    if (!w->isUntitled())
    {
  //if it's  not untitled, than it was loaded from somewhere. In this case show it from that place
      url = w->url();

      if ( w->isModified() )
      {
          w->saveIt(); //saves the file so the preview will show the current content
          m_previewedDocument = w;
      }
      url = Project::ref()->urlWithPrefix(url);

      m_htmlPart->openURL(url);
      m_htmlPart->addToHistory(url.url());
    } else  //the document is Untitled, preview the text from it
    {
      QString text = w->editIf->text();
      if ( text.isEmpty() )
      {
        text = i18n( "<center><h3>The current document is empty...</h3></center>" );
      }
      m_htmlPart->closeURL();
      m_htmlPart->begin( Project::ref()->projectBaseURL(), xOffset, yOffset );
      m_htmlPart->write( text );
      m_htmlPart->end();
    }
   }
// part->end();
 m_htmlPart->show();
}

void QuantaApp::slotOpenFileInPreview(const KURL& a_url)
{
  WHTMLPart *part = m_htmlPart;
  if ( !part  )
     return;
  slotShowPreviewWidget(true);
  part->openURL(a_url);
  part->show();
}

/** view image in preview */
void QuantaApp::slotImageOpen(const KURL& url)
{
  slotShowPreviewWidget(true);
  WHTMLPart *part = m_htmlPart;
  QString text = "<html>\n<body>\n<div align=\"center\">\n<img src=\"";
  text += url.fileName(); //TODO
  text += "\">\n</div>\n</body>\n</html>\n";
  part->closeURL();
  KURL docURL = url;
  docURL.setFileName("imagepreview.html");
  part->begin( docURL );
  part->write( text );
  part->end();

  part->show();
}


/** insert <img> tag for images or <a> for other */
void QuantaApp::slotInsertTag(const KURL& url, DirInfo dirInfo)
{
  Document *w = ViewManager::ref()->activeDocument();
  if (w)
  {
    KURL baseURL ;
    if  (w->isUntitled() )
    {
      baseURL = Project::ref()->projectBaseURL();
    } else
    {
      baseURL = w->url();
      baseURL.setFileName("");
    }
    KURL relURL = QExtFileInfo::toRelative( url, baseURL);
    QString urlStr = relURL.url();
    if (relURL.protocol() == baseURL.protocol())
        urlStr = relURL.path();
    bool isImage = false;

    if (!dirInfo.preText.isEmpty() || !dirInfo.postText.isEmpty())
    {
      w->insertTag(dirInfo.preText+urlStr+dirInfo.postText);
    } else
    {
      if (url.isLocalFile())
      {
        QImage img(url.path());
        if ( !img.isNull() )
        {
          QString width,height;
          width.setNum(img.width());
          height.setNum(img.height());
          QString imgTag = QuantaCommon::tagCase("<img ");
          imgTag += QuantaCommon::attrCase("src=");
          imgTag += QuantaCommon::quoteAttributeValue(urlStr);
          imgTag += QuantaCommon::attrCase(" width=");
          imgTag += QuantaCommon::quoteAttributeValue(width);
          imgTag += QuantaCommon::attrCase(" height=");
          imgTag += QuantaCommon::quoteAttributeValue(height);
          imgTag += QuantaCommon::attrCase(" border=");
          imgTag += QuantaCommon::quoteAttributeValue(QString("%1").arg(0));
          imgTag += ">";
          w->insertTag(imgTag);
          isImage = true;
        }
      }
      if (!isImage)
      {
        QString tag = QuantaCommon::tagCase("<a ");
        tag += QuantaCommon::attrCase("href=");
        tag += QuantaCommon::quoteAttributeValue(urlStr);
        tag += ">";
        w->insertTag(tag, QuantaCommon::tagCase("</a>"));
      }
    }
    w->view()->setFocus();
  }
}

void QuantaApp::slotNewStatus()
{
  fileRecent->setEnabled(true);
  actionCollection()->action("project_open_recent")->setEnabled(true);
  QuantaView *view = ViewManager::ref()->activeView();
  Document *w = ViewManager::ref()->activeDocument();
  if (w)
  {
    setTitle( w->url().prettyURL() );

    if (w->doc()->isReadWrite())
    {
      KToggleAction *a = dynamic_cast<KToggleAction*>(w->view()->actionCollection()->action("set_insert"));
      if (a)
      {
        statusBar()->changeItem(a->isChecked() ? i18n(" OVR ") : i18n(" INS "),IDS_INS_OVR);
      }
    }
    else
        statusBar()->changeItem(i18n(" R/O "),IDS_INS_OVR);
    statusBar()->changeItem(w->isModified() ? " * " : "",IDS_MODIFIED);

    saveAction   ->setEnabled(w->isModified());
    saveAllAction->setEnabled(ViewManager::ref()->isOneModified());

    bool projectExists = Project::ref()->hasProject();

    actionCollection()->action("toolbars_load_project")->setEnabled(projectExists);
    actionCollection()->action("toolbars_save_project")->setEnabled(projectExists);
 }
 if (view)
 {
     view->updateTab();
 }
}

void QuantaApp::slotOptionsConfigureKeys()
{
  Document *w = ViewManager::ref()->activeDocument();
  KKeyDialog dlg( false, this );
  QPtrList<KXMLGUIClient> toolbarGuiClients;
  QDictIterator<ToolbarEntry> iter(toolbarList);
  for( ; iter.current(); ++iter )
  {
    toolbarGuiClients.append(iter.current()->guiClient);
  }
  QPtrList<KXMLGUIClient> clients = guiFactory()->clients();
  for( QPtrListIterator<KXMLGUIClient> it( clients );
       it.current(); ++it )
  {
     if (toolbarGuiClients.contains(*it) <= 0) //no need to insert the collections of the toolbars as they are present in the main actionCollection
        dlg.insert((*it)->actionCollection());
  }
  if ( dlg.configure() == KKeyDialog::Accepted )
  {
  // this is needed for when we have multiple embedded kateparts and change one of them.
    // it also needs to be done to their views, as they too have actioncollections to update
      if( const QPtrList<KParts::Part> * partlist = m_partManager->parts() )
      {
          QPtrListIterator<KParts::Part> it( *partlist );
          while ( KParts::Part* part = it.current() )
          {
              if ( KTextEditor::Document * doc = dynamic_cast<KTextEditor::Document*>( part ) )
              {
                  KActionPtrList actionList = doc->actionCollection()->actions();
                  KActionPtrList::Iterator actionIt;
                  if (!w || w->doc() != doc)
                  {
                    for ( actionIt = actionList.begin(); actionIt != actionList.end(); ++actionIt )
                    {
                        (*actionIt)->setShortcut((*actionIt)->shortcutDefault());
                    }
                  }
                  doc->reloadXML();

                  QPtrList<KTextEditor::View> const & list = doc->views();
                  QPtrListIterator<KTextEditor::View> itt( list );
                  while( KTextEditor::View * view = itt.current() )
                  {
                     if (!w || w->view() != view)
                     {
                        actionList = view->actionCollection()->actions();
                        for ( actionIt = actionList.begin(); actionIt != actionList.end(); ++actionIt )
                        {
                            (*actionIt)->setShortcut((*actionIt)->shortcutDefault());
                        }
                     }
                     view->reloadXML();
                     ++itt;
                  }
              }
              ++it;
          }
      }

      QDomDocument doc;
      doc.setContent(KXMLGUIFactory::readConfigFile(xmlFile(), instance()));
      QDomNodeList nodeList = doc.elementsByTagName("ActionProperties");
      QDomNode node = nodeList.item(0).firstChild();
      while (!node.isNull())
      {
        if (node.nodeName() == "Action")
        {
          TagAction *action = dynamic_cast<TagAction*>(actionCollection()->action(node.toElement().attribute("name")));
          if (action)
          {
            action->setModified(true);
            QDomElement el = action->data();
            el.setAttribute("shortcut", action->shortcut().toString());
            el = node.toElement();
            node = node.nextSibling();
            el.parentNode().removeChild(el);
          } else
          {
            node = node.nextSibling();
          }
        }
      }
  }
}

void QuantaApp::slotConfigureToolbars(const QString& defaultToolbar)
{
 currentPageIndex = ToolbarTabWidget::ref()->currentPageIndex();
 QDomNodeList nodeList;
 ToolbarEntry *p_toolbar = 0L;
#if KDE_VERSION < KDE_MAKE_VERSION(3,2,90)
 QDictIterator<ToolbarEntry> iter(toolbarList);
 for( ; iter.current(); ++iter )
 {
   p_toolbar = iter.current();
   //Rename the _Separator_ tags back to Separator, so they will appear
   //in the config dialog
   nodeList = p_toolbar->guiClient->domDocument().elementsByTagName("_Separator_");
   for (uint i = 0; i < nodeList.count(); i++)
   {
     nodeList.item(i).toElement().setTagName("Separator");
   }
   KXMLGUIFactory::saveConfigFile(p_toolbar->guiClient->domDocument(),
        p_toolbar->guiClient->xmlFile(), p_toolbar->guiClient->instance());
 }
#endif
  saveMainWindowSettings(KGlobal::config(), autoSaveGroup());
  KEditToolbar *dlg;
  if (defaultToolbar)
    dlg = new KEditToolbar(defaultToolbar, factory(), this);
  else
    dlg = new KEditToolbar(factory(), this);

  KMenuBar *mb = menuBar();
  KActionCollection *ac = actionCollection();
  //remove the manually added menus BEFORE the dlg shows up
  if (m_debugger->UI())
  {
    m_debugger->UI()->hideMenu();
  }
  for (uint i = 0 ; i < mb->count(); i++)
  {
       if (mb->text(mb->idAt(i)) == i18n("&Window"))
       {
          mb->removeItem(mb->idAt(i));
          break;
       }
  }
 ToolbarTabWidget *tb = ToolbarTabWidget::ref();
 QString toolbarName;
 for (int i = 0; i < tb->count(); i++)
 {
   toolbarName = tb->label(i);
   p_toolbar = quantaApp->toolbarList[toolbarName.lower()];
   if (p_toolbar)
   {
    delete p_toolbar->menu;
    p_toolbar->menu = 0L;
   }
 }

 connect( dlg, SIGNAL( newToolbarConfig() ), SLOT( slotNewToolbarConfig() ) );
 dlg->exec();
 delete dlg;
 QPopupMenu *menu = 0L;
 m_tagsMenu = static_cast<QPopupMenu*>(factory()->container("tags", this));
 for (int i = 0; i < tb->count(); i++)
 {
   toolbarName = tb->label(i);
   p_toolbar = quantaApp->toolbarList[toolbarName.lower()];
   if (p_toolbar)
   {
    menu = new QPopupMenu(m_tagsMenu);
    nodeList = p_toolbar->guiClient->domDocument().elementsByTagName("Action");
    for (uint i = 0; i < nodeList.count(); i++)
    {
      KAction *action = ac->action(nodeList.item(i).toElement().attribute("name"));
      if (action)
          action->plug(menu);
    }

    m_tagsMenu->insertItem(toolbarName,menu);
    p_toolbar->menu = menu;
   }
 }

  //add back the menus
 m_pluginInterface->setPluginMenu(static_cast<QPopupMenu*>(factory()->container("plugins", this)));
 m_pluginInterface->buildPluginMenu();
  for (uint i = 0 ; i < mb->count(); i++)
  {
       if (mb->text(mb->idAt(i)) == i18n("&Settings"))
       {
          mb->insertItem(i18n("&Window"), windowMenu(), -1, i);
          break;
       }
  }
 if (m_debugger->UI())
 {
    m_debugger->UI()->showMenu();
 }
 tb->setCurrentPage(currentPageIndex);
}

void QuantaApp::slotOptionsConfigureToolbars()
{
  slotConfigureToolbars();
}

void QuantaApp::slotNewToolbarConfig()
{
 applyMainWindowSettings(KGlobal::config(), autoSaveGroup());
 ToolbarTabWidget::ref()->setCurrentPage(currentPageIndex);
}

void QuantaApp::slotOptionsConfigureActions()
{
  ActionConfigDialog dlg( this, "actions_config_dlg", true);
  dlg.exec();
}

void QuantaApp::slotPreviewOptions()
{
    KMessageBox::information(this, i18n("Changes made in the preview configuration dialog are global and have effect on every application using the KHTML part to display web pages, including Konqueror."), i18n("Warning"), "configure_preview_warning");
    KApplication::startServiceByDesktopName("quanta_preview_config");
}

void QuantaApp::slotOptions()
{
  KDialogBase *kd = new KDialogBase(KDialogBase::IconList,
              i18n("Configure Quanta"),
              KDialogBase::Ok | KDialogBase::Cancel,
              KDialogBase::Ok, this, "tabdialog");

  // Tag Style options
  QVBox *page=kd->addVBoxPage(i18n("Tag Style"), QString::null, BarIcon("kwrite", KIcon::SizeMedium ) );
  StyleOptionsS *styleOptionsS = new StyleOptionsS( (QWidget *)page);

  styleOptionsS->tagCase->setCurrentItem( qConfig.tagCase);
  styleOptionsS->attributeCase->setCurrentItem( qConfig.attrCase);
  styleOptionsS->attributeQuotation->setCurrentItem( (qConfig.attrValueQuotation == '"') ? 0 : 1);
  styleOptionsS->tagAutoClose->setChecked( qConfig.closeTags );
  styleOptionsS->optionalTagAutoClose->setChecked( qConfig.closeOptionalTags );
  styleOptionsS->useAutoCompletion->setChecked( qConfig.useAutoCompletion );
  styleOptionsS->tagUpdateClosing->setChecked(qConfig.updateClosingTags);
  styleOptionsS->replaceAccented->setChecked(qConfig.replaceAccented);

  // Environment options
  //TODO FileMasks name is not good anymore
  page=kd->addVBoxPage(i18n("Environment"), QString::null, UserIcon("files", KIcon::SizeMedium ) );
  FileMasks *fileMasks = new FileMasks((QWidget *)page);

  fileMasks->lineMarkup->setText( qConfig.markupMimeTypes );
  fileMasks->lineScript->setText( qConfig.scriptMimeTypes );
  fileMasks->lineImage->setText( qConfig.imageMimeTypes );
  fileMasks->lineText->setText( qConfig.textMimeTypes );
  fileMasks->showDTDSelectDialog->setChecked(qConfig.showDTDSelectDialog);
  m_config->setGroup("Notification Messages");
  fileMasks->warnBinaryOpening->setChecked(m_config->readEntry("Open Everything") != "yes");
  fileMasks->warnEventActions->setChecked((m_config->readEntry("Warn about internal actions", "yes") != "yes") && (m_config->readEntry("Warn about external actions", "yes") != "yes"));
  m_config->setGroup("General Options");
  fileMasks->showSplash->setChecked(m_config->readBoolEntry("Show Splash", true));
  fileMasks->reloadProject->setChecked(m_config->readBoolEntry("Reload Project", true));
  fileMasks->reloadFiles->setChecked(m_config->readBoolEntry("Reload Files", true));
  if(!m_config->readEntry("Autosave interval").isEmpty())
     fileMasks->sbAutoSave->setValue(m_config->readNumEntry("Autosave interval"));
  //else default value 15

  QStringList availableEncodingNames(KGlobal::charsets()->availableEncodingNames());
  fileMasks->encodingCombo->insertStringList( availableEncodingNames );
  QStringList::ConstIterator iter;
  int iIndex = -1;
  for (iter = availableEncodingNames.begin(); iter != availableEncodingNames.end(); ++iter)
  {
     ++iIndex;
     if ((*iter).lower() == qConfig.defaultEncoding.lower())
     {
       fileMasks->encodingCombo->setCurrentItem(iIndex);
       break;
     }
  }
  QStringList lst = DTDs::ref()->nickNameList(true);
  uint pos = 0;
  for (uint i = 0; i < lst.count(); i++)
  {
    fileMasks->defaultDTDCombo->insertItem(lst[i]);
    if (lst[i] == DTDs::ref()->getDTDNickNameFromName(qConfig.defaultDocType.lower()))
       pos = i;
  }
  fileMasks->defaultDTDCombo->setCurrentItem(pos);

  // Preview options
  page=kd->addVBoxPage(i18n("User Interface"), QString::null, BarIcon("kview", KIcon::SizeMedium ) );
  PreviewOptions *uiOptions = new PreviewOptions( (QWidget *)page );

  uiOptions->setPosition(qConfig.previewPosition);
  uiOptions->setDocPosition(qConfig.docPosition);
  uiOptions->setWindowLayout(qConfig.windowLayout);
  uiOptions->setCloseButtons(qConfig.showCloseButtons);
  uiOptions->setToolviewTabs(qConfig.toolviewTabs);
  uiOptions->setHiddenFiles(qConfig.showHiddenFiles);
  uiOptions->setSaveTrees(qConfig.saveTrees);

  //kafka options
  page = kd->addVBoxPage(i18n("VPL View"), QString::null, UserIcon("vpl_text", KIcon::SizeMedium));
  KafkaSyncOptions *kafkaOptions = new KafkaSyncOptions( m_config, (QWidget *)page );

  page=kd->addVBoxPage(i18n("Parser"), QString::null, BarIcon("kcmsystem", KIcon::SizeMedium ) );
  ParserOptions *parserOptions = new ParserOptions( m_config, (QWidget *)page );

  parserOptions->refreshFrequency->setValue(qConfig.refreshFrequency);
  parserOptions->instantUpdate->setChecked(qConfig.instantUpdate);
  parserOptions->showEmptyNodes->setChecked(qConfig.showEmptyNodes);
  parserOptions->showClosingTags->setChecked(qConfig.showClosingTags);
  parserOptions->spinExpand->setValue(qConfig.expandLevel);

  page = kd->addVBoxPage(i18n("Abbreviations"), QString::null, BarIcon("source", KIcon::SizeMedium));
  AbbreviationDlg *abbreviationOptions = new AbbreviationDlg((QWidget*)(page));
//Spelling options
  page=kd->addVBoxPage(i18n("Spelling"), QString::null, BarIcon("spellcheck", KIcon::SizeMedium ) );
  KSpellConfig *spellOptions = new KSpellConfig( (QWidget *)page, 0L, qConfig.spellConfig, false );

  bool reloadTrees = false;
  kd->adjustSize();
  if ( kd->exec() )
  {
    bool uiRebuildNeeded = false;
    bool previewSettingsChanged = false;

    qConfig.tagCase = styleOptionsS->tagCase->currentItem();
    qConfig.attrCase = styleOptionsS->attributeCase->currentItem();
    qConfig.attrValueQuotation = styleOptionsS->attributeQuotation->currentItem() == 0 ? '"': '\'';
    qConfig.closeTags = styleOptionsS->tagAutoClose->isChecked();
    qConfig.closeOptionalTags = styleOptionsS->optionalTagAutoClose->isChecked();
    qConfig.useAutoCompletion = styleOptionsS->useAutoCompletion->isChecked();
    qConfig.updateClosingTags = styleOptionsS->tagUpdateClosing->isChecked();
    qConfig.replaceAccented = styleOptionsS->replaceAccented->isChecked();

    qConfig.markupMimeTypes = fileMasks->lineMarkup->text();
    qConfig.scriptMimeTypes = fileMasks->lineScript->text();
    qConfig.imageMimeTypes = fileMasks->lineImage->text();
    qConfig.textMimeTypes = fileMasks->lineText->text();
    qConfig.showDTDSelectDialog = fileMasks->showDTDSelectDialog->isChecked();
    qConfig.autosaveInterval =  fileMasks->sbAutoSave->value();
    autosaveTimer->start(60000 * qConfig.autosaveInterval, false);
    m_config->setGroup("Notification Messages");
    m_config->writeEntry("Open Everything", fileMasks->warnBinaryOpening->isChecked() ? "" : "yes");
    m_config->writeEntry("Warn about internal actions", fileMasks->warnEventActions->isChecked() ? "" : "yes");
    m_config->writeEntry("Warn about external actions", fileMasks->warnEventActions->isChecked() ? "" : "yes");
    m_config->setGroup("General Options");
    m_config->writeEntry("Show Splash", fileMasks->showSplash->isChecked());
    m_config->writeEntry("Reload Project", fileMasks->reloadProject->isChecked());
    m_config->writeEntry("Reload Files", fileMasks->reloadFiles->isChecked());

    qConfig.defaultEncoding = fileMasks->encodingCombo->currentText();
    QString tmpStr = uiOptions->closeButtons();
    if (tmpStr != qConfig.showCloseButtons)
        uiRebuildNeeded = true;
    qConfig.showCloseButtons = tmpStr;
    uint tmp = uiOptions->toolviewTabs();
    if (tmp != qConfig.toolviewTabs)
        uiRebuildNeeded = true;
    qConfig.toolviewTabs = tmp;
    if (uiRebuildNeeded)
    {
        initTabWidget();
     }
    reloadTrees = (qConfig.showHiddenFiles != uiOptions->hiddenFiles());
    qConfig.showHiddenFiles = uiOptions->hiddenFiles();
    qConfig.saveTrees = uiOptions->saveTrees();

    qConfig.showEmptyNodes = parserOptions->showEmptyNodes->isChecked();
    qConfig.showClosingTags = parserOptions->showClosingTags->isChecked();
    qConfig.instantUpdate = parserOptions->instantUpdate->isChecked();
    qConfig.refreshFrequency = parserOptions->refreshFrequency->value();
    if (!qConfig.instantUpdate && qConfig.refreshFrequency > 0)
    {
      refreshTimer->changeInterval(qConfig.refreshFrequency*1000);
    } else
    {
      refreshTimer->stop();
    }
    qConfig.expandLevel = parserOptions->spinExpand->value();
    parserOptions->updateConfig();

    kafkaOptions->updateConfig();
    qConfig.quantaRefreshOnFocus = kafkaOptions->sourceFocusRefresh->isChecked();
    qConfig.quantaRefreshDelay = kafkaOptions->sourceDelay->value();
    qConfig.kafkaRefreshOnFocus = kafkaOptions->kafkaFocusRefresh->isChecked();
    qConfig.kafkaRefreshDelay = kafkaOptions->kafkaDelay->value();
    QuantaView *view = ViewManager::ref()->activeView();
    if (view && view->document())
        view->reloadUpdateTimers();
    /**(static_cast<HTMLEnhancer *>(quantaApp->view()->getKafkaInterface()->mainEnhancer))->
      showIconsForScripts(kafkaOptions->showScriptsIcon->isChecked());*/


    qConfig.defaultDocType = DTDs::ref()->getDTDNameFromNickName(fileMasks->defaultDTDCombo->currentText());

    abbreviationOptions->saveTemplates();

    qConfig.spellConfig->setDictionary(spellOptions->dictionary());
    qConfig.spellConfig->setNoRootAffix(spellOptions->noRootAffix());
    qConfig.spellConfig->setRunTogether(spellOptions->runTogether());
    qConfig.spellConfig->setDictFromList(spellOptions->dictFromList());
    qConfig.spellConfig->setEncoding(spellOptions->encoding());
    qConfig.spellConfig->setIgnoreList(spellOptions->ignoreList());
    qConfig.spellConfig->setReplaceAllList(spellOptions->replaceAllList());
    qConfig.spellConfig->setClient(spellOptions->client());

    tmpStr = uiOptions->position();
    if (tmpStr != qConfig.previewPosition)
    {
       if (m_previewVisible || m_previewToolView)
         previewSettingsChanged = true;
       slotShowPreviewWidget(false);
    }
    qConfig.previewPosition = tmpStr;
    if (previewSettingsChanged)
      slotShowPreviewWidget(true);
    qConfig.docPosition = uiOptions->docPosition();
    qConfig.windowLayout = uiOptions->layout();

    m_htmlPart->closeURL();
    m_htmlPart->begin( Project::ref()->projectBaseURL());
    m_htmlPart->write(" ");
    m_htmlPart->end();

    reparse(true);
    slotNewStatus();
  }

  m_config->sync();

  saveOptions();

  delete kd;
  if (reloadTrees) emit reloadAllTrees();

}

void QuantaApp::slotShowPreviewWidget(bool show)
{
  QuantaView *view = ViewManager::ref()->activeView();
  if (!view) return;
  if (m_previewVisible == show) return;
  if (show)
  {
    if (qConfig.previewPosition == "Editor")
    {
      delete m_previewToolView;
      m_previewToolView = 0L;
      view->addCustomWidget(m_htmlPart->view(), QString::null);
    } else
    {
      if (!m_previewToolView)
      {
        m_previewToolView= addToolWindow(m_htmlPart->view(), KDockWidget::DockBottom, getMainDockWidget());
        connect(m_previewToolView->wrapperWidget(), SIGNAL(iMBeingClosed
()), this, SLOT(slotPreviewBeingClosed()));
      }
      m_htmlPart->view()->show();
      m_previewToolView->show();
    }
    m_previewVisible = true;
    slotRepaintPreview();
  } else
  {
    m_noFramesPreview = false;
    m_previewVisible = false;
    m_htmlPart->view()->reparent(this, 0, QPoint(), false);
    m_htmlPart->view()->resize(0, 0);
    m_htmlPart->view()->hide();
    if (qConfig.previewPosition == "Editor")
    {
        view->addCustomWidget(0L, QString::null);
        delete m_previewToolView;
        m_previewToolView = 0L;
    } else
    {
       delete m_previewToolView;
       m_previewToolView = 0L;
   }
    Document *w = view->document();
    if (w)
    {
        restoreFromTempfile(w);
        w->view()->setFocus();
    }
  }


  int viewLayout = view->currentViewsLayout();

   KToggleAction *ta = 0L;

  if (viewLayout == QuantaView::SourceOnly)
    ta = (KToggleAction *) actionCollection()->action( "show_quanta_editor" );
  else if (viewLayout == QuantaView::VPLOnly)
    ta = (KToggleAction *) actionCollection()->action( "show_kafka_view" );
  else if (viewLayout == QuantaView::SourceAndVPL)
  {
    ta = (KToggleAction *) actionCollection()->action( "show_kafka_and_quanta" );
  }
  if (ta)
  {
    ta->setChecked(!show);
  }
}

void QuantaApp::slotChangePreviewStatus()
{
    if (qConfig.previewPosition == "Editor")
    {
        slotShowPreviewWidget(false);
    } else
    if (m_previewToolView && m_htmlPart->view()->isVisible())
    {
      //hiding the preview when it's in a toolview means that the current tab has changed,
      //so we just repaint the content and restore the document on the disc.
        m_previewVisible = true;
        slotRepaintPreview();
        restoreFromTempfile(m_previewedDocument);
        m_previewedDocument = 0L;
        Document *w = ViewManager::ref()->activeDocument();
        if (w)
          w->view()->setFocus();
    }
}

void QuantaApp::slotPreviewHasFocus(bool focus)
{

   if (m_previewToolView)
   {
      if (focus)
        slotRepaintPreview();
      else
      {
          restoreFromTempfile(m_previewedDocument);
          m_previewedDocument = 0L;
      }
   }
}

void QuantaApp::slotToggleShowPreview()
{
  Document *w  =ViewManager::ref()->activeDocument();
  if (!w)
  {
    m_previewVisible = false;
    return;
  }
  if (m_previewToolView)
  {
    m_previewVisible = m_htmlPart->view()->isVisible();
  }
  slotShowPreviewWidget(!m_previewVisible);
  m_noFramesPreview = false;
}

void QuantaApp::slotShowNoFramesPreview()
{
  m_noFramesPreview = true;
  slotToggleShowPreview();
}

void QuantaApp::restoreFromTempfile(Document *w)
{
  if (!w) return;
  if (w->tempFileName().isEmpty())
    return;

  KURL origUrl = w->url();
  if (!w->isUntitled() && origUrl.isLocalFile())
  {
      fileWatcher->removeFile(origUrl.path());
      kdDebug(24000) << "removeFile[restoreFromTempfile]: " << origUrl.path() << endl;
  }
  KURL tempUrl;
  tempUrl.setPath(w->tempFileName());
//  kdDebug(24000) << "Restoring tempfile " << w->tempFileName() << " for " << w->url() << endl;
  QExtFileInfo::copy(tempUrl, origUrl, -1, true, false, this);
  KIO::UDSEntry entry;
  if (KIO::NetAccess::stat(origUrl, entry, this))
  {
    KFileItem item(entry, origUrl, true);
    int modifiedTime = item.time(KIO::UDS_MODIFICATION_TIME);
    Project::ref()->updateTimeStamp(origUrl, modifiedTime);
  }
  if (origUrl.isLocalFile())
  {
      kdDebug(24000) << "addFile[restoreFromTempfile]: " << origUrl.path() << endl;
      fileWatcher->addFile(origUrl.path());
  }
}

void QuantaApp::newCursorPosition(const QString &file, int lineNumber, int columnNumber)
{
  Q_UNUSED(file);
  typingInProgress = true;
  startIdleTimer();
 // updateTreeViews();
  QString linenumber;
  linenumber = i18n("Line: %1 Col: %2").arg(lineNumber).arg(columnNumber);
  statusBar()->changeItem(linenumber, IDS_STATUS_CLM);
  statusBar()->changeItem(i18n(" R/O "),IDS_INS_OVR);
  statusBar()->changeItem("",IDS_MODIFIED);
}

void QuantaApp::newDebuggerPosition(const QString &file, int lineNumber)
{
    newCursorPosition(file, lineNumber, 0);
}

void QuantaApp::openFile(const QString &file, int lineNumber, int columnNumber)
{
  gotoFileAndLine(file, lineNumber, columnNumber);
  slotNewStatus();
}

void QuantaApp::slotNewLineColumn()
{
  typingInProgress = true;
  startIdleTimer();
 // updateTreeViews();
  QString linenumber;
  oldCursorLine = cursorLine;
  oldCursorCol = cursorCol;
  Document *w = ViewManager::ref()->activeDocument();
  if (w)
    w->viewCursorIf->cursorPositionReal(&cursorLine, &cursorCol);
  linenumber = i18n("Line: %1 Col: %2").arg(cursorLine+1).arg(cursorCol+1);
  statusBar()->changeItem(linenumber, IDS_STATUS_CLM);
}

void QuantaApp::updateTreeViews()
{
  QuantaView *view = ViewManager::ref()->activeView();
  Document *w = view->document();
  if (w)
  {
    w->viewCursorIf->cursorPositionReal(&cursorLine, &cursorCol);
    Node *node = parser->nodeAt(cursorLine, cursorCol);
    if (node)
    {
      StructTreeView::ref()->showTagAtPos(node);
    }
    if(view->hadLastFocus() == QuantaView::SourceFocus)
    aTab->setCurrentNode(node);
  }
}

void QuantaApp::slotIdleTimerExpired()
{
  if (idleTimer)
  {
    typingInProgress = false;
    updateTreeViews();
  }
}

void QuantaApp::startIdleTimer()
{
  if (m_idleTimerEnabled && idleTimer)
      idleTimer->start(500, true);
}

bool QuantaApp::enableIdleTimer(bool enable)
{
   bool status = m_idleTimerEnabled;
   if (enable)
     startIdleTimer();
   else
     idleTimer->stop();
   m_idleTimerEnabled = enable;
   return status;
}

void QuantaApp::slotReparse()
{
  reparse(false);
}

void QuantaApp::slotForceReparse()
{
  reparse(true);
}

/** reparse current document and initialize node. */
void QuantaApp::reparse(bool force)
{
  if (!parser)
    return;
  //temp
//  if (!parser->activated()) return;
  typingInProgress = false;
  Document *w = ViewManager::ref()->activeDocument();
  if (w)
  {
    if (force)
    {
      baseNode = parser->parse(w, true);
    }

    if (w->hasChanged() || force)
    {
      slotReloadStructTreeView();
    }

    if (force)
    {
      uint line, col;
      w->viewCursorIf->cursorPositionReal(&line, &col);
      Node *node = parser->nodeAt(line, col);
      if (StructTreeView::ref()->isVisible() && node)
         StructTreeView::ref()->showTagAtPos(node);
      aTab->setCurrentNode(node);
    }
  }

  return;
}

void QuantaApp::setCursorPosition( int row, int col )
{
  Document *w = ViewManager::ref()->activeDocument();
  if (w)
  {
    int numLines = w->editIf->numLines();

    if ( row < numLines )
      w->viewCursorIf->setCursorPositionReal(row, col);
    else
      w->viewCursorIf->setCursorPositionReal(numLines - 1, col);
  }
}

void QuantaApp::gotoFileAndLine(const QString& filename, int line, int column)
{
  if ( !filename.isEmpty() )
    m_doc->openDocument( KURL::fromPathOrURL( filename ) );

  Document *w = ViewManager::ref()->activeDocument();
  if (w)
  {
    int numLines = w->editIf->numLines();
    if ( numLines > line && line >= 0 )
    {
      w->viewCursorIf->setCursorPositionReal(line, column);
    }
    w->view()->setFocus();
  }
}

void QuantaApp::selectArea(int line1, int col1, int line2, int col2)
{
  Document *w = ViewManager::ref()->activeDocument();
  if (w)
  {
    int numLines = w->editIf->numLines();

    if ( line1 > numLines-1 )
      line1 = numLines-1;

    if ( line2 > numLines-1 )
      line2 = numLines-1;

    w->viewCursorIf->setCursorPositionReal(line2, col2);
    w->selectionIf->setSelection(line1, col1, line2, col2);
  }
}

void QuantaApp::openDoc(const QString& url)
{
  if (qConfig.docPosition == "Tab")
  {
      QuantaView *docView = ViewManager::ref()->documentationView();
      delete m_documentationToolView;
      m_documentationToolView = 0L;
      docView->activate();
  } else
  {
      QuantaView *docView = ViewManager::ref()->documentationView(false);
      if (docView)
          ViewManager::ref()->removeView(docView);
      if (!m_documentationToolView)
        m_documentationToolView= addToolWindow(m_htmlPartDoc->view(), KDockWidget::DockBottom, getMainDockWidget());
      m_htmlPartDoc->view()->show();
      m_documentationToolView->show();
  }
  m_htmlPartDoc->view()->setFocus();  // activates the part

  QString urlStr = url;
  if (urlStr.startsWith("/"))
    urlStr.prepend("file:");
  KURL u(urlStr);
  if (u == m_htmlPartDoc->url())
    return;

  m_htmlPartDoc->closeURL();
  m_htmlPartDoc->openURL(u);
  m_htmlPartDoc->show();
  m_htmlPartDoc->addToHistory(urlStr);
}

void QuantaApp::slotContextHelp()
{
    Document *w = ViewManager::ref()->activeDocument();
    if (w)
    {
        QString currentWord = "";
        reparse(true);
        uint line, col;
        w->viewCursorIf->cursorPositionReal(&line, &col);
        Node *node = parser->nodeAt(line, col);
        if (node && node->tag->type == Tag::XmlTag)
        {
           currentWord = node->tag->name;
        } else
        {
           currentWord = w->currentWord();
        }
        QString *url = dTab->contextHelp(currentWord);
        if (url)
            openDoc(*url);
    }
}

void QuantaApp::slotShowMessagesView()
{
  makeDockVisible(dynamic_cast<KDockWidget*>(m_messageOutputView->wrapperWidget()));
}

void QuantaApp::slotShowProblemsView()
{
  makeDockVisible(dynamic_cast<KDockWidget*>(m_problemsOutputView->wrapperWidget()));
}

QWidget* QuantaApp::createContainer( QWidget *parent, int index, const QDomElement &element, int &id )
{

  QString tabname = element.attribute( "i18ntabname", "" );

  if ( element.tagName().lower() == "toolbar" && !tabname.isEmpty())
  {
//avoid QToolBar warning in the log
    QtMsgHandler oldHandler = qInstallMsgHandler( silenceQToolBar );
    ToolbarTabWidget *toolbarTab = ToolbarTabWidget::ref();
    QWidget *w = new QWidget(toolbarTab, "ToolbarHoldingWidget" + element.attribute("name"));
    QuantaToolBar *tb = new QuantaToolBar(w, element.attribute("name"), true, true);
    tb->loadState(element);
    tb->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
#if KDE_VERSION < KDE_MAKE_VERSION(3,2,90)
    KAction *action;
    QDomNode node = element.firstChild();
    while (!node.isNull())
    {
      if (node.nodeName() == "Action")
      {
        action = actionCollection()->action(node.toElement().attribute("name") );
        if (action)
        {
          action->plug(tb);
        }
      }
//FIXME: This is the first part of the hack to disable automatic separator drawing
//If I do not rename the Separator tag, then all the seaprators will appear before
//the buttons. :-(
      if (node.nodeName() == "Separator")
          node.toElement().setTagName("_Separator_");
      if (node.nodeName() == "_Separator_")
        tb->insertLineSeparator();
      node = node.nextSibling();
    }
#endif
  //kdDebug(24000) << "tb->iconSize() " << tb->iconSize() << endl;
   if (tb->iconText() == KToolBar::IconTextBottom)
   {
      tb->setGeometry(0,0, toolbarTab->width(), tb->iconSize() + QFontMetrics(KGlobalSettings::toolBarFont()).height() + 10);
      toolbarTab->setFixedHeight(toolbarTab->tabHeight() + tb->height() + 3);
   } else
   {
      tb->setGeometry(0,0, toolbarTab->width(), tb->iconSize() + 10);
      toolbarTab->setFixedHeight(toolbarTab->tabHeight() + tb->height() + 3);
   }
/*
   kdDebug(24000) << "tb->height() " << tb->height() << endl;
   kdDebug(24000) << "toolbarTab->height() " << toolbarTab->height() << endl;
   kdDebug(24000) << "toolbarTab->tabHeight() " << toolbarTab->tabHeight() << endl;
*/
    toolbarTab->insertTab(tb, tabname);
    qInstallMsgHandler( oldHandler );

    connect(tb, SIGNAL(removeAction(const QString&, const QString&)),
                SLOT(slotRemoveAction(const QString&, const QString&)));
    connect(tb, SIGNAL(editAction(const QString&)),
                SLOT(slotEditAction(const QString&)));
    return tb;
  }

  return KMainWindow::createContainer( parent, index, element, id );

}

void QuantaApp::removeContainer( QWidget *container, QWidget *parent, QDomElement &element, int id )
{
  if (dynamic_cast<QuantaToolBar*>(container))
  {
    ToolbarTabWidget::ref()->removePage(container);
  }
  else
    KMainWindow::removeContainer( container, parent, element, id );
}

void QuantaApp::slotBack()
{
   if (ViewManager::ref()->documentationView(false)  == ViewManager::ref()->activeView())
   {
       m_htmlPartDoc->back();
   } else
   if (m_previewVisible && (!m_previewToolView || m_htmlPart->view()->hasFocus()))
   {
       m_htmlPart->back();
   } else
   {
      activatePrevWin();
   }
}

void QuantaApp::slotForward()
{
   if (ViewManager::ref()->documentationView(false)  == ViewManager::ref()->activeView())
   {
       m_htmlPartDoc->forward();
   } else
   if (m_previewVisible && (!m_previewToolView || m_htmlPart->view()->hasFocus()))
   {
       m_htmlPart->forward();
   } else
   {
      activateNextWin();
   }
}

/** No descriptions */
void QuantaApp::slotInsertFile(const KURL& url)
{
  Document *w = ViewManager::ref()->activeDocument();
  if (w)
  {
    w->insertFile(url);
  }
}

void QuantaApp::slotContextMenuAboutToShow()
{
  Document *w = ViewManager::ref()->activeDocument();
  if (w)
  {
    QPopupMenu *popup = static_cast<QPopupMenu*>(factory()->container("popup_editor",this));
    QString name;
    uint line, col;
    int bl, bc, el, ec;
    QString tagStr;
    w->viewCursorIf->cursorPositionReal(&line, &col);
    Node *node = parser->nodeAt(line, col, false);
    if (node)
    {
      if (node->tag->dtd()->family == Script)
      {
        StructTreeGroup group;
        uint count = node->tag->dtd()->structTreeGroups.count();
        for (uint i = 0; i < count; i++)
        {
          group = node->tag->dtd()->structTreeGroups[i];
          if (group.hasFileName)
          {
            if (!group.hasDefinitionRx )
              continue;
            tagStr =  node->tag->tagStr();
            int pos = 0;
            while (pos != -1)
            {
              pos = group.definitionRx.search(node->tag->cleanStr, pos);
              if (pos != -1)
              {
                QString cleanName = node->tag->cleanStr.mid(pos, group.definitionRx.matchedLength());
                name = tagStr.mid(pos, group.definitionRx.matchedLength());
                node->tag->beginPos(bl, bc);
                QString tmpStr = tagStr.left(pos);
                int newLines = tmpStr.contains('\n');
                bl += newLines;
                int l = tmpStr.findRev('\n'); //the last EOL
                bc = (l == -1) ? bc+pos : pos - l - 1;
                newLines = name.contains('\n');
                l = name.length();
                el = bl + newLines;
                ec = (newLines > 0) ? l - name.findRev('\n') : bc + l - 1;
                pos += l;
                int p = group.definitionRx.search(cleanName);
                if (p != -1)
                {
                  name = name.mid(p, group.definitionRx.matchedLength());
                } else
                  name = "";
                if (QuantaCommon::isBetween(line, col, bl, bc, el, ec) == 0)
                {
                  break;
                } else
                {
                  name = "";
                }
              }
            }
            name.remove(group.fileNameRx);
            if (!name.isEmpty())
              break;
          }
        }
      } else
      {
        QMap<QString, XMLStructGroup>::ConstIterator it = node->tag->dtd()->xmlStructTreeGroups.find(node->tag->name.lower());

        if (it != node->tag->dtd()->xmlStructTreeGroups.constEnd())
        {
          XMLStructGroup group = it.data();
          uint count = group.attributes.count();
          for (uint j = 0; j < count; j++ )
            if (node->tag->hasAttribute(group.attributes[j]))
            {
              name.append(node->tag->attributeValue(group.attributes[j]));
              name.append(" | ");
            }
          name = name.left(name.length()-3);
          name.remove('\n');
        }
      }
    }
    KAction *action = actionCollection()->action("open_file_under_cursor");
    if (action)
    {
      if (!name.isEmpty())
      {
        urlUnderCursor = KURL();
        QuantaCommon::setUrl(urlUnderCursor, name.stripWhiteSpace());
        KURL baseUrl = QExtFileInfo::path(w->url());
        urlUnderCursor = QExtFileInfo::toAbsolute(urlUnderCursor, baseUrl);
        action->setText(i18n("Open File: %1").arg(KStringHandler::lsqueeze(urlUnderCursor.prettyURL(0, KURL::StripFileProtocol), 80)));
        action->setEnabled(true);
      } else
      {
        action->setText(i18n("Open File: none"));
        action->setEnabled(false);
      }
    }
    action = actionCollection()->action("upload_file");
    if (action)
    {
      action->setEnabled(Project::ref()->contains(w->url()));
    }

    // If we have a debugger session active...
    if(debugger() && debugger()->hasClient())
    {
      int startpos;
      QString word;

      // If we have a selection made, thats what we want to use for watching, setting etc
      if(w->selectionIf->hasSelection())
      {
        word = w->selectionIf->selection();
      }
      else
      {
        // Otherwise, find the word under the cursor
        word =  w->editIf->textLine(w->viewCursorIf->cursorLine());
        startpos = word.findRev(QRegExp("$|[^a-zA-Z0-9_]"), w->viewCursorIf->cursorColumn());

        word.remove(0, startpos);
        if(word.left(1) != "$")
          word.remove(0, 1);

        word = word.left(word.find(QRegExp("[^a-zA-Z0-9_]"), 1));
      }
      // If we have a linebreak, take everything before the break
      startpos = word.find("\n");
      if(startpos > 0)
        word = word.left(startpos);

      // Trim whitespace from the beginning and end of the string
      word = word.stripWhiteSpace();

      // now we have a word, possibly the name of a variable
      popupWord = word;

      // The word we display in the popup will be cut off not to make an obeast pop up menu
      if(word.length() > 15)
      {
        word.remove(0, 12);
        word += "...";
      }

      // If we have the addwatch action...
      action = actionCollection()->action("debug_addwatch");
      if(action)
      {
        action->setText(i18n("Add Watch: %1").arg(word));
        action->setEnabled(!word.isEmpty());

        if(!action->isPlugged(popup))
          action->plug(popup);
      }

      // Dito for the set action
      action = actionCollection()->action("debug_variable_set");
      if(action)
      {
        action->setText(i18n("Set Value of '%1'").arg(word));
        action->setEnabled(!word.isEmpty());

        if(!action->isPlugged(popup))
          action->plug(popup);
      }

      // Dito for the "break when" action
      action = actionCollection()->action("debug_conditional_break");
      if(action)
      {
        action->setText(i18n("Break When %1...").arg(word));
        action->setEnabled(!word.isEmpty());

        if(!action->isPlugged(popup))
          action->plug(popup);
      }
    }
    else
    {
      action = actionCollection()->action("debug_addwatch");
      if(action && action->isPlugged(popup))
        action->unplug(popup);
      action = actionCollection()->action("debug_variable_set");
      if(action && action->isPlugged(popup))
        action->unplug(popup);
      action = actionCollection()->action("debug_conditional_break");
      if(action && action->isPlugged(popup))
        action->unplug(popup);
    }
#ifdef ENABLE_CVSSERVICE
   if (w->url().isLocalFile() && !w->isUntitled() && CVSService::ref()->exists())
   {
     if (m_cvsMenuId == -1)
     {
       popup->insertSeparator();
       m_cvsMenuId = popup->insertItem(SmallIcon("cervisia"), i18n("CVS"), CVSService::ref()->menu());
     }
     if (Project::ref()->contains(w->url()))
       CVSService::ref()->setRepository(Project::ref()->projectBaseURL().path());
     else
         CVSService::ref()->setRepository(w->url().directory());
     CVSService::ref()->setCurrentFile(w->url().path());
   } else
   if (m_cvsMenuId != -1)
   {
       int idx = popup->indexOf(m_cvsMenuId);
       popup->removeItemAt(idx-1);
       popup->removeItem(m_cvsMenuId);
       m_cvsMenuId = -1;
   }
#endif
  }

}

void QuantaApp::slotOpenFileUnderCursor()
{
  if (QExtFileInfo::exists(urlUnderCursor))
  {
    if (QuantaCommon::checkMimeGroup(urlUnderCursor, "text" ))
    {
      slotFileOpen(urlUnderCursor, defaultEncoding());
    }
    else if (QuantaCommon::checkMimeGroup(urlUnderCursor, "image" ))
    {
      slotShowPreviewWidget(true);
      slotImageOpen(urlUnderCursor);
    }
  } else
  {
    KMessageBox::error(this, i18n("<qt>The file <b>%1</b> does not exists or it is not reachable.</qt>").arg(urlUnderCursor.prettyURL(0, KURL::StripFileProtocol)));

  }
}

/** No descriptions */
void QuantaApp::slotSyntaxCheckDone()
{
  Document *w = ViewManager::ref()->activeDocument();
  if (w)
  {
    if (w->isModified())
    {
      KURL origUrl = w->url();
      if (!w->isUntitled() && origUrl.isLocalFile())
          fileWatcher->removeFile(origUrl.path());
      KURL tempUrl;
      tempUrl.setPath(w->tempFileName());
     QExtFileInfo::copy(tempUrl, origUrl, -1, true, false, this);
      if (origUrl.isLocalFile())
          fileWatcher->addFile(origUrl.path());
    }
  }
}

/** Load an user toolbar file from the disk. */
void QuantaApp::slotLoadToolbarFile(const KURL& url)
{
  QDictIterator<ToolbarEntry> it(toolbarList);
  ToolbarEntry *p_toolbar;
  while (it.current())
  {
    p_toolbar = it.current();
    ++it;
    if (url == p_toolbar->url)
        return;
  }
 QDomDocument actionDom;
 QDomDocument *toolbarDom = new QDomDocument();

 QTextStream str;
 str.setEncoding(QTextStream::UnicodeUTF8);
 QString fileName = url.path();

 if ( url.fileName().endsWith(toolbarExtension) )
 {
//extract the files from the archives
   KTar tar(fileName);
   if (tar.open(IO_ReadOnly))
   {
     QString base = QFileInfo(fileName).baseName();
     KArchiveFile* file = (KArchiveFile *) tar.directory()->entry(base+".toolbar");
     if (file)
     {
      toolbarDom->setContent(file->device());
     }
     file = (KArchiveFile *) tar.directory()->entry(base+".actions");
     if (file)
     {
      actionDom.setContent(file->device());
     }

     tar.close();
   }
   if ( (toolbarDom->toString().isEmpty()) ) //|| (actionContent.isEmpty()))
   {
     KMessageBox::error(this, i18n("Cannot load the toolbars from the archive.\nCheck that the filenames inside the archives begin with the archive name."));
     return;
   }

   QDomNodeList nodeList = toolbarDom->elementsByTagName("ToolBar");
   QString name = nodeList.item(0).cloneNode().toElement().attribute("tabname");

   //search for another toolbar with the same name
   QPtrList<KXMLGUIClient> xml_clients = guiFactory()->clients();
   QString newName = name;
   QString i18nName = i18n(name.utf8());
   QString origName = name;
   bool found = false;
   bool nameModified = false;
   int count = 2;
   do
   {
     uint index = 0;
     while (index < xml_clients.count())
     {
       name = newName;
       if (index == 0)
         found = false;
       nodeList = xml_clients.at(index)->domDocument().elementsByTagName("ToolBar");
       for (uint i = 0; i < nodeList.count(); i++)
       {
         if ((nodeList.item(i).cloneNode().toElement().attribute("name").lower() ) == name.lower())
         {
           newName = origName + QString(" (%1)").arg(count);
           i18nName = i18n(origName.utf8()) + QString(" (%1)").arg(count);
           nameModified = true;
           count++;
           found = true;
           break;
         }
       }
       if (found)
       {
         index = 0;
       } else
       {
         index++;
       }
     }
   } while (name == newName && found);
   name = newName;

   p_toolbar = new ToolbarEntry;

   QDomDocument *dom = new QDomDocument();
   dom->setContent(toolbarDom->toString());
   p_toolbar->dom = dom;
   p_toolbar->nameModified = nameModified;

   userToolbarsCount++;

   //Change the name also in the XML File -> create a temp XML file
   KTempFile* tempFile = new KTempFile(tmpDir);
   tempFile->setAutoDelete(true);

   nodeList = toolbarDom->elementsByTagName("ToolBar");
   QDomElement el = nodeList.item(0).toElement();
   el.setAttribute("name", name.lower());
   el.setAttribute("tabname", name);
   el.setAttribute("i18ntabname", i18nName);
   nodeList = toolbarDom->elementsByTagName("text");
   el.firstChild().setNodeValue(name);
   tempFile->textStream()->setEncoding(QTextStream::UnicodeUTF8);
   * (tempFile->textStream()) << toolbarDom->toString();
   tempFile->close();

   //create the new toolbar GUI from the temp file
   ToolbarXMLGUI * toolbarGUI = new ToolbarXMLGUI(tempFile->name());

   //setup the actions
   nodeList = actionDom.elementsByTagName("action");
   for (uint i = 0; i < nodeList.count(); i++)
   {
    QDomNode node = nodeList.item(i).cloneNode();
    el = node.toElement();
    QString actionName = el.attribute("name");
    //if there is no such action yet, add to the available actions
    if (!actionCollection()->action(actionName))
    {
      TagAction *tagAction = new TagAction(&el, actionCollection() );

#if KDE_IS_VERSION(3,2,90)
     //add the actions to every toolbar xmlguiclient
      QDictIterator<ToolbarEntry> it(toolbarList);
      while (it.current())
      {
        it.current()->guiClient->actionCollection()->insert(tagAction);
        ++it;
      }
#endif

    //Compatility code (read the action shortcuts from quantaui.rc)
    //TODO: Remove after upgrade from 3.1 is not supported
      if (oldShortcuts.contains(actionName))
      {
        tagAction->setModified(true);
        tagAction->data().setAttribute("shortcut", oldShortcuts[actionName]);
        tagAction->setShortcut(KShortcut(oldShortcuts[actionName]));
      }
    } else
    {
 //      kdDebug(24000) << "The action " << actionName << " is already present!" << endl;
       TagAction *tagAction = dynamic_cast<TagAction*>(actionCollection()->action(actionName));
       if (tagAction)
          tagAction->setModified(true);
    }
   }

#if KDE_IS_VERSION(3,2,90)
   //add all actions to the xmlguiclient of this toolbar
   for (uint i = 0 ; i < actionCollection()->count(); i++)
      toolbarGUI->actionCollection()->insert(actionCollection()->action(i));
#endif

   guiFactory()->addClient(toolbarGUI);

   //Plug in the actions & build the menu
   QPopupMenu *menu = new QPopupMenu;
   KAction *action;
   nodeList = toolbarGUI->domDocument().elementsByTagName("Action");
   for (uint i = 0; i < nodeList.count(); i++)
   {
     action = actionCollection()->action(nodeList.item(i).cloneNode().toElement().attribute("name") );
     if (action)
     {
       action->plug(menu);
     }
   }

   m_tagsMenu->insertItem(i18nName, menu);
   p_toolbar->menu = menu;

   tempFileList.append(tempFile);
   p_toolbar->guiClient = toolbarGUI;
   p_toolbar->name = name;
   p_toolbar->url = url;
   p_toolbar->visible = true;
   p_toolbar->user = true; //TODO
   toolbarList.insert(i18nName.lower(), p_toolbar);

   slotToggleDTDToolbar(!allToolbarsHidden());
 }
}

/** Show the toolbar which is in url. If it was not loaded yet, it loads the
    toolbar from the file */
void QuantaApp::showToolbarFile(const KURL &url)
{
  ToolbarEntry *p_toolbar = toolbarByURL(url);
  if (!p_toolbar)
  {
    slotLoadToolbarFile(url);
    p_toolbar = toolbarByURL(url);
    if (p_toolbar)
    {
      p_toolbar->user = false;
      userToolbarsCount--;
    }
  } else
  {
    QDomNodeList nodeList;
    QPopupMenu *menu = new QPopupMenu;
    KAction *action;
    KActionCollection *ac = actionCollection();
    nodeList = p_toolbar->dom->elementsByTagName("Action");
    for (uint i = 0; i < nodeList.count(); i++)
    {
      action = ac->action(nodeList.item(i).toElement().attribute("name") );
      if (action)
      {
        action->plug(menu);
      }
    }
    m_tagsMenu->insertItem(i18n(p_toolbar->name.utf8()),menu);
    p_toolbar->menu = menu;
    guiFactory()->addClient(p_toolbar->guiClient);
    p_toolbar->visible = true;
  }
}

/** Load an user toolbar from the disk. */
void QuantaApp::slotLoadToolbar()
{
 KURL::List urls = KFileDialog::getOpenURLs(locateLocal("data",resourceDir + "toolbars/"), "*"+toolbarExtension, this);
 if (!urls.isEmpty())
 {
   for (KURL::List::ConstIterator it = urls.constBegin(); it != urls.constEnd(); ++it)
      slotLoadToolbarFile(*it);
 }
}

/** Load an user toolbar from the disk. */
void QuantaApp::slotLoadGlobalToolbar()
{
 KURL::List urls = KFileDialog::getOpenURLs(qConfig.globalDataDir +resourceDir + "toolbars/", "*"+toolbarExtension+"\n*", this);
 if (!urls.isEmpty())
 {
   for (KURL::List::ConstIterator it = urls.constBegin(); it != urls.constEnd(); ++it)
      slotLoadToolbarFile(*it);
 }
}

KURL QuantaApp::saveToolbarToFile(const QString& toolbarName, const KURL& destFile)
{
  KURL tarFile = destFile;

  if (! destFile.fileName().endsWith(toolbarExtension))
  {
    tarFile.setFileName(destFile.fileName() + toolbarExtension);
  }

  QBuffer buffer;
  buffer.open(IO_ReadWrite);
  QString toolStr;
  QTextStream toolStream(&toolStr, IO_ReadWrite);
  toolStream.setEncoding(QTextStream::UnicodeUTF8);

  QBuffer buffer2;
  buffer2.open(IO_WriteOnly);
  QTextStream actStr(&buffer2);
  actStr.setEncoding(QTextStream::UnicodeUTF8);

  QDomNodeList nodeList, nodeList2;

  toolStream << "<!DOCTYPE kpartgui SYSTEM \"kpartgui.dtd\">\n<kpartgui name=\"quanta\" version=\"2\">\n";
  actStr << QString("<!DOCTYPE actionsconfig>\n<actions>\n");

//look up the clients
  QPtrList<KXMLGUIClient> xml_clients = factory()->clients();
  for (uint index = 0; index < xml_clients.count(); index++)
  {
      nodeList = xml_clients.at(index)->domDocument().elementsByTagName("ToolBar");
      for (uint i = 0; i < nodeList.count(); i++)
      {
      //find the actual toolbar in the XML GUI
        if ((nodeList.item(i).cloneNode().toElement().attribute("name") ) == toolbarName.lower())
        {

          //find the actions registered to the toolbar
          QDomNode n = nodeList.item(i).firstChild();
          while (! n.isNull())
          {
           QDomElement e = n.toElement();
           if (e.tagName() == "Action")
           {
             TagAction *action = dynamic_cast<TagAction*>(actionCollection()->action(e.attribute("name")));
             if (action)
             {
               action->data().save(actStr,1);
               action->setModified(false);
             }
           } else
           if (e.tagName() == "_Separator_")
           {
             e.setTagName("Separator");
           }
           n = n.nextSibling();
          }
          QDomElement e = nodeList.item(0).toElement();
          QString i18nName = e.attribute("i18ntabname");
          e.removeAttribute("i18ntabname");
          nodeList.item(i).save(toolStream,2);
          e.setAttribute("i18ntabname", i18nName);
        }
      }
  }
  toolStream << QString("\n</kpartgui>");
  actStr << QString("\n</actions>");
  //buffer.flush();

  ToolbarEntry *p_toolbar = toolbarList[toolbarName.lower()];
  delete p_toolbar->dom;
  QDomDocument *dom = new QDomDocument();
  QString s = toolStr;
  QString error;
  int el, ec;
  if (!dom->setContent(s, &error, &el, &ec))
      kdError(24000) << QString("Error %1 at (%2, %3)").arg(error).arg(el).arg(ec)<<endl;
  p_toolbar->dom = dom;

  QTextStream bufferStr(&buffer);
  bufferStr.setEncoding(QTextStream::UnicodeUTF8);
  bufferStr << toolStr;
  buffer.close();
  buffer2.close();

  KTempFile *tempFile = new KTempFile(tmpDir);
  tempFile->setAutoDelete(true);
  tempFile->close();
  KTar tar(tempFile->name(), "application/x-gzip");
  if (!tar.open(IO_WriteOnly))
      return KURL();
  if (!tar.writeFile(QFileInfo(tarFile.path()).baseName()+".toolbar", "user", "group", buffer.buffer().size(), buffer.buffer().data()))
      return KURL();
  if (!tar.writeFile(QFileInfo(tarFile.path()).baseName()+".actions", "user", "group", buffer2.buffer().size(), buffer2.buffer().data()))
      return KURL();
  tar.close();
  if (!QExtFileInfo::copy(KURL::fromPathOrURL(tempFile->name()), tarFile, -1, true, false, this))
      KMessageBox::error(this, i18n("<qt>There was an error while creating the toolbar file.<br>Check that you have write access to <i>%1</i>.</qt>").arg(tarFile.prettyURL(0, KURL::StripFileProtocol)), i18n("Toolbar Saving Error"));

  return tarFile;
}

/** Saves a toolbar as local or project specific. */
bool QuantaApp::saveToolbar(bool localToolbar, const QString& toolbarToSave, const KURL& destURL)
{
  int query;
  KURL url;
  KURL projectToolbarsURL;
  QString toolbarName;
  QString localToolbarsDir = locateLocal("data",resourceDir + "toolbars/");

  if (toolbarToSave.isEmpty())
  {
    ToolbarTabWidget *tb = ToolbarTabWidget::ref();

    QStringList lst;
    int current=0;
    for (int i = 0; i < tb->count(); i++)
    {
      lst << tb->label(i);
      if ( tb->tabLabel(tb->currentPage()) == tb->label(i) ) current=i;
    }

    bool ok = FALSE;
    QString res = KInputDialog::getItem(
                    i18n( "Save Toolbar" ),
                    i18n( "Please select a toolbar:" ), lst, current, FALSE, &ok, this );
    if ( !ok )
      return false;

    toolbarName = res;
  } else
  {
    toolbarName = toolbarToSave;
  }

  if (destURL.isEmpty())
  {
    do {
      query = KMessageBox::Yes;

      if (localToolbar)
      {
        url = KFileDialog::getSaveURL(localToolbarsDir, "*"+toolbarExtension, this);
      } else
      {
        url = KFileDialog::getSaveURL(Project::ref()->toolbarURL().url(), "*"+toolbarExtension, this);
      }

      if (url.isEmpty())
          return false;

      if (Project::ref()->hasProject())
          projectToolbarsURL = Project::ref()->toolbarURL();
      if ( ((!localToolbar) && (projectToolbarsURL.isParentOf(url)) ) ||
            ((localToolbar) && (KURL(localToolbarsDir).isParentOf(url))) )
      {
        if (!QuantaCommon::checkOverwrite(url))
          query = KMessageBox::No;
      } else
      {
        if (!localToolbar)
            localToolbarsDir = projectToolbarsURL.prettyURL();
        KMessageBox::sorry(0,i18n("<qt>You must save the toolbars in the following directory: <br><br><b>%1</b></qt>")
                                  .arg(localToolbarsDir));
        query = KMessageBox::No;
      }
    } while (query != KMessageBox::Yes);
  } else
  {
    url = destURL;
    query = KMessageBox::Yes;
  }
  if( query != KMessageBox::Cancel )
  {
    KURL tarName = saveToolbarToFile(toolbarName, url);
    if (tarName.isEmpty())
    {
      KMessageBox::error(this, i18n("<qt>An error happened while saving the <b>%1</b> toolbar.<br>"
      "Check that you have write permissions for<br><b>%2</b></qt>").arg(toolbarName).arg(url.prettyURL(0, KURL::StripFileProtocol)));
      return false;
    }
    if (!localToolbar)
        Project::ref()->insertFile(tarName, true);
  }
  return true;
}

/** Saves a toolbar as localspecific. */
void QuantaApp::slotSaveLocalToolbar()
{
  saveToolbar(true);
}
/** Saves a toolbar as project specific. */
void QuantaApp::slotSaveProjectToolbar()
{
  saveToolbar(false);
}

/** Adds a new, empty toolbar. */
void QuantaApp::slotAddToolbar()
{
 bool ok;
 QString name = KInputDialog::getText(i18n("New Toolbar"), i18n("Enter toolbar name:"), i18n("User_%1").arg(userToolbarsCount), &ok, this);
 if (ok)
 {
  userToolbarsCount++;


  KTempFile* tempFile = new KTempFile(tmpDir);
  tempFile->setAutoDelete(true);
  tempFile->textStream()->setEncoding(QTextStream::UnicodeUTF8);
  * (tempFile->textStream()) << QString("<!DOCTYPE kpartgui SYSTEM \"kpartgui.dtd\">\n<kpartgui name=\"quanta\" version=\"2\">\n<ToolBar name=\"%1\" tabname=\"%2\" i18ntabname=\"%3\">\n<text>%4</text>\n</ToolBar>\n</kpartgui>\n")\
               .arg(name.lower()).arg(name).arg(name).arg(name);
  tempFile->close();

  ToolbarXMLGUI * toolbarGUI = new ToolbarXMLGUI(tempFile->name());

#if KDE_IS_VERSION(3,2,90)
//add all actions to the xmlguiclient of this toolbar
  for (uint i = 0 ; i < actionCollection()->count(); i++)
     toolbarGUI->actionCollection()->insert(actionCollection()->action(i));
#endif

  guiFactory()->addClient(toolbarGUI);
  ToolbarTabWidget::ref()->setCurrentPage(ToolbarTabWidget::ref()->count()-1);
  tempFileList.append(tempFile);
  ToolbarEntry *p_toolbar = new ToolbarEntry;
  p_toolbar->guiClient = toolbarGUI;

  QDomDocument *dom = new QDomDocument(toolbarGUI->domDocument());

  p_toolbar->dom = dom;
  p_toolbar->name = name;
  p_toolbar->user = true;
  p_toolbar->visible = true;
  p_toolbar->nameModified = false;
  p_toolbar->menu = new QPopupMenu;
  m_tagsMenu->insertItem(p_toolbar->name, p_toolbar->menu);
  toolbarList.insert(name.lower(), p_toolbar);

  slotToggleDTDToolbar(!allToolbarsHidden());
 }
}


/** Removes a user toolbar from the toolbars. */
bool QuantaApp::slotRemoveToolbar()
{
 ToolbarTabWidget *tb = ToolbarTabWidget::ref();
 int i;

 QStringList lst;
 int current=0, j =0;
 for (i = 0; i < tb->count(); i++)
 {
   lst << tb->label(i);
   if ( tb->tabLabel(tb->currentPage()) == tb->label(i) ) current=j;
   j++;
 }

 bool ok = FALSE;
 QString res = KInputDialog::getItem(
                 i18n( "Remove Toolbar" ),
                 i18n( "Please select a toolbar:" ), lst, current, FALSE, &ok, this );

 if (ok)
 {
   return slotRemoveToolbar(res.lower());
 } else
   return false;

}


/** Sends a toolbar in mail. */
void QuantaApp::slotSendToolbar()
{
  ToolbarTabWidget *tb = ToolbarTabWidget::ref();

  QStringList lst;
  int current = 0;
  for (int i = 0; i < tb->count(); i++)
  {
    lst << tb->label(i);
    if ( tb->tabLabel(tb->currentPage()) == tb->label(i) ) current=i;
  }

  bool ok = FALSE;
  QString res = KInputDialog::getItem(
                  i18n( "Send Toolbar" ),
                  i18n( "Please select a toolbar:" ), lst, current, FALSE, &ok, this );

  if (!ok)
    return;

  QString toolbarName = res.lower();

  QStringList toolbarFile;

  QString prefix="quanta";
  KTempDir* tempDir = new KTempDir(tmpDir);
  tempDir->setAutoDelete(true);
  tempDirList.append(tempDir);
  QString tempFileName=tempDir->name() + toolbarName.replace(QRegExp("\\s|\\."), "_");

  KURL tempURL;
  tempURL.setPath(tempFileName);
  saveToolbarToFile(toolbarName, tempURL);

  toolbarFile += tempFileName + ".toolbar.tgz";

  TagMailDlg *mailDlg = new TagMailDlg( this, i18n("Send toolbar in email"));
  QString toStr;
  QString message = i18n("Hi,\n This is a Quanta Plus [http://quanta.sourceforge.net] toolbar.\n\nHave fun.\n");
  QString titleStr;
  QString subjectStr;

  mailDlg->TitleLabel->setText(i18n("Content:"));
  mailDlg->titleEdit->setFixedHeight(60);
  mailDlg->titleEdit->setVScrollBarMode(QTextEdit::Auto);
  mailDlg->titleEdit->setHScrollBarMode(QTextEdit::Auto);
  if ( mailDlg->exec() ) {
    if ( !mailDlg->lineEmail->text().isEmpty())
    {
      toStr = mailDlg->lineEmail->text();
      subjectStr = (mailDlg->lineSubject->text().isEmpty())?i18n("Quanta Plus toolbar"):mailDlg->lineSubject->text();
      if ( !mailDlg->titleEdit->text().isEmpty())
        message = mailDlg->titleEdit->text();
    } else
    {
      KMessageBox::error(this,i18n("No destination address was specified.\n Sending is aborted."),i18n("Error Sending Email"));
      delete mailDlg;
      return;
    }

    kapp->invokeMailer(toStr, QString::null, QString::null, subjectStr, message, QString::null, toolbarFile);
  }
  delete mailDlg;
}

void QuantaApp::slotRenameToolbar()
{
  ToolbarTabWidget *tb = ToolbarTabWidget::ref();

  QStringList lst;
  int current = 0;
  for (int i = 0; i < tb->count(); i++)
  {
    lst << tb->label(i);
    if ( tb->tabLabel(tb->currentPage()) == tb->label(i) ) current=i;
  }

  bool ok = FALSE;
  QString res = KInputDialog::getItem(
                  i18n( "Rename Toolbar" ),
                  i18n( "Please select a toolbar:" ), lst, current, FALSE, &ok, this );
  if (ok)
  {
    slotRenameToolbar(res.lower());
  }
}

void QuantaApp::slotRenameToolbar(const QString& name)
{
  ToolbarEntry *p_toolbar = quantaApp->toolbarList[name];
  if (p_toolbar)
  {
    bool ok;
    QString newName = KInputDialog::getText(i18n("Rename Toolbar"), i18n("Enter the new name:"), p_toolbar->name, &ok, this);
    if (ok && newName != p_toolbar->name)
    {
      toolbarList.take(name.lower());
      p_toolbar->name = newName;
      QDomElement el = p_toolbar->guiClient->domDocument().firstChild().firstChild().toElement();
      el.setAttribute("tabname", p_toolbar->name);
      el.removeAttribute("i18ntabname");
      el.setAttribute("name", p_toolbar->name.lower());
      QDomNodeList nodeList = p_toolbar->guiClient->domDocument().elementsByTagName("text");
      nodeList.item(0).firstChild().setNodeValue(p_toolbar->name);
    //Rename the _Separator_ tags back to Separator, so they are not treated
    //as changes
      nodeList = p_toolbar->guiClient->domDocument().elementsByTagName("_Separator_");
      for (uint i = 0; i < nodeList.count(); i++)
      {
        nodeList.item(i).toElement().setTagName("Separator");
      }
      KXMLGUIFactory::saveConfigFile(p_toolbar->guiClient->domDocument(),
          p_toolbar->guiClient->xmlFile(), p_toolbar->guiClient->instance());
      ToolbarTabWidget *tb = ToolbarTabWidget::ref();
      for (int i = 0; i < tb->count(); i++)
      {
        if (tb->label(i).lower() == name)
        {
          tb->setTabLabel(tb->page(i)->parentWidget(), i18n(p_toolbar->name.utf8()));
          m_tagsMenu->changeItem(m_tagsMenu->idAt(i + 2), i18n(p_toolbar->name.utf8()));
          break;
        }
      }
      toolbarList.insert(p_toolbar->name.lower(), p_toolbar);
    }
  }
}

/** Ask for save all the modified user toolbars. */
bool QuantaApp::removeToolbars()
{
  QDictIterator<ToolbarEntry> it(toolbarList);
  QStringList toolbarNameList;
  while (it.current())
  {
    toolbarNameList += it.current()->name.lower();
    if (!slotRemoveToolbar(i18n(it.current()->name).lower()))
        return false;
  }

  QString s = "<!DOCTYPE actionsconfig>\n<actions>\n</actions>\n";
  m_actions->setContent(s);
  TagAction *action;
  for (uint i = 0; i < actionCollection()->count(); i++)
  {
    action = dynamic_cast<TagAction *>(actionCollection()->action(i));
    if (action)
    {
      QDomElement el = action->data();
      m_actions->firstChild().appendChild(el);
    }
  }

  QFile f(KGlobal::instance()->dirs()->saveLocation("data")+resourceDir + "actions.rc" );
  if (f.open( IO_ReadWrite | IO_Truncate ))
  {
    if (!m_actions->firstChild().firstChild().isNull())
    {
      QTextStream qts(&f);
      qts.setEncoding(QTextStream::UnicodeUTF8);
      m_actions->save(qts,0);
      f.close();
    } else
    f.remove();
  }

 return true;
}

void QuantaApp::slotDeleteAction(KAction *action)
{
//remove all references to this action
  QDomElement el = static_cast<TagAction*>(action)->data();
  QString text = el.attribute("text");
  QString actionName = action->name();

  QPtrList<KXMLGUIClient> guiClients = factory()->clients();
  KXMLGUIClient *guiClient = 0;
  QDomNodeList nodeList;
  for (uint i = 0; i < guiClients.count(); i++)
  {
    guiClient = guiClients.at(i);
    guiClient->domDocument().setContent(KXMLGUIFactory::readConfigFile( guiClient->xmlFile(), guiClient->instance() ));
    nodeList = guiClient->domDocument().elementsByTagName("Action");
    for (uint j = 0; j < nodeList.count(); j++)
    {
      //we found a toolbar that contains the action
      if (nodeList.item(j).toElement().attribute("name") == actionName)
      {
        nodeList.item(j).parentNode().removeChild(nodeList.item(j));
        KXMLGUIFactory::saveConfigFile(guiClient->domDocument(), guiClient->xmlFile());
        break;
      }
    }
  }
  action->unplugAll();
  delete action;
  action = 0L;
}

void QuantaApp::slotRemoveAction(const QString& toolbarName, const QString& a_actionName)
{
  KAction *action = 0L;
  QString actionName = a_actionName;
  actionName.replace('&',"&&");
  KActionCollection *ac = actionCollection();
  uint actionCount = ac->count();
  QString str;
  for (uint i = 0; i < actionCount; i++)
  {
    str = ac->action(i)->text();
    if (str == actionName || str.remove('&') == actionName)
    {
      action = ac->action(i);
      break;
    }
  }
  if (!action) //workaround for actionnames ending with "...". It's stripped from the end
              //of the text when plugged into a toolbar.
  {
    actionName += "...";
    for (uint i = 0; i < actionCount; i++)
    {
      if (ac->action(i)->text() == actionName)
      {
        action = ac->action(i);
        break;
      }
    }
  }

  if (action)
  {
    ToolbarEntry *p_toolbar = quantaApp->toolbarList[toolbarName.lower()];
    if (p_toolbar)
    {
      QDomNode node = p_toolbar->guiClient->domDocument().firstChild().firstChild().firstChild();
      while (!node.isNull())
      {
        if (node.nodeName() == "Action" &&
            node.toElement().attribute("name") == action->name())
        {
          action->unplug(ToolbarTabWidget::ref()->page(toolbarName));
          action->unplug(p_toolbar->menu);
          node.parentNode().removeChild(node);
        }
        node = node.nextSibling();
      }
      KXMLGUIFactory::saveConfigFile(p_toolbar->guiClient->domDocument(),
        p_toolbar->guiClient->xmlFile(), p_toolbar->guiClient->instance());
    }
  }
}

void QuantaApp::slotEditAction(const QString& actionName)
{
  ActionConfigDialog dlg(this, "actions_config_dlg", true, 0, actionName);
  dlg.exec();
}

void QuantaApp::slotNewAction()
{
  ActionConfigDialog dlg(this, "actions_config_dlg");
  dlg.slotNewAction();
  dlg.exec();
}

void QuantaApp::slotAssignActionToScript(const KURL& a_scriptURL, const QString& a_interpreter)
{
  ActionConfigDialog dlg(this, "actions_config_dlg");
  QString name = a_scriptURL.fileName();
  name.truncate(name.length() - QFileInfo(name).extension().length() - 1);
  dlg.createScriptAction(name, a_interpreter + " " + a_scriptURL.path());
  dlg.exec();
}


/** No descriptions */
void QuantaApp::slotChangeDTD()
{
  Document *w = ViewManager::ref()->activeDocument();
  if (w)
  {
    KDialogBase dlg(this, 0L, true, i18n("DTD Selector"), KDialogBase::Ok | KDialogBase::Cancel);
    DTDSelectDialog *dtdWidget = new DTDSelectDialog(&dlg);
    dtdWidget->setMinimumHeight(130);
    dlg.setMainWidget(dtdWidget);
    int pos = -1;
    int defaultIndex = 0;

    Tag *tag = 0L;
    w->findDTDName(&tag);
    QString oldDtdName = w->getDTDIdentifier();
    QString defaultDocType = Project::ref()->defaultDTD();
    QStringList lst = DTDs::ref()->nickNameList(true);

    QString oldDtdNickName = DTDs::ref()->getDTDNickNameFromName(oldDtdName);
    QString defaultDtdNickName = DTDs::ref()->getDTDNickNameFromName(defaultDocType);
    for(uint i = 0; i < lst.count(); i++)
    {
      dtdWidget->dtdCombo->insertItem(lst[i]);
      if (lst[i] == oldDtdNickName) pos = i;
      if (lst[i] == defaultDtdNickName) defaultIndex = i;
    }

    if (pos == -1)
      pos = defaultIndex;
    dtdWidget->dtdCombo->setCurrentItem(pos);
    dtdWidget->messageLabel->setText(i18n("Change the current DTD."));
    dtdWidget->currentDTD->setText(DTDs::ref()->getDTDNickNameFromName(w->getDTDIdentifier()));
    //dlg->useClosestMatching->setShown(false);
    delete dtdWidget->useClosestMatching;
    dtdWidget->useClosestMatching = 0L;
    dtdWidget->adjustSize();
    if (dlg.exec())
    {
      w->setDTDIdentifier(DTDs::ref()->getDTDNameFromNickName(dtdWidget->dtdCombo->currentText()));
      const DTDStruct *dtd = DTDs::ref()->find(w->getDTDIdentifier());
      if (dtdWidget->convertDTD->isChecked() && dtd->family == Xml)
      {
        if (tag)
        {
          int bLine, bCol, eLine, eCol;
          tag->beginPos(bLine,bCol);
          tag->endPos(eLine,eCol);
          w->editIf->removeText(bLine, bCol, eLine, eCol+1);
          w->viewCursorIf->setCursorPositionReal((uint)bLine, (uint)bCol);
          w->insertText("<!DOCTYPE" + dtd->doctypeStr +">");
          delete tag;
        } else
        {
          w->viewCursorIf->setCursorPositionReal(0,0);
          w->insertText("<!DOCTYPE" + dtd->doctypeStr + ">\n");
        }
      }
    }

    loadToolbarForDTD(w->getDTDIdentifier());
    QuantaView *view = ViewManager::ref()->activeView();
    if (view)
      view->activated();
    reparse(true);
  }
}

/** No descriptions */
void QuantaApp::focusInEvent(QFocusEvent* e)
{
 KDockMainWindow::focusInEvent(e);
 Document *w = ViewManager::ref()->activeDocument();
 if (w)
 {
    w->view()->setFocus();
    w->checkDirtyStatus();
 }
}

/** No descriptions */
void QuantaApp::slotShowCompletion()
{
  Document *w = ViewManager::ref()->activeDocument();
  if (w->codeCompletionIf)
    w->codeCompletionRequested();
}

/** No descriptions */
void QuantaApp::slotShowCompletionHint()
{
  Document *w = ViewManager::ref()->activeDocument();
  if (w->codeCompletionIf)
    w->codeCompletionHintRequested();
}

/** No descriptions */
void QuantaApp::slotMakeDonation()
{
 DonationDialog *dlg = new DonationDialog(this);
 dlg->closeButton->setIconSet(SmallIconSet("fileclose"));
 connect(dlg->closeButton, SIGNAL(clicked()), dlg, SLOT(accept()));
 dlg->exec();
 delete dlg;
}

/** No descriptions */
void QuantaApp::slotHelpHomepage()
{
  kapp->invokeBrowser("http://quanta.sourceforge.net");
}

/** No descriptions */
void QuantaApp::slotHelpUserList()
{
  kapp->invokeBrowser("http://mail.kde.org/mailman/listinfo/quanta");
}

/** Loads the toolbars for dtd named dtdName and unload the ones belonging to oldDtdName. */
void QuantaApp::loadToolbarForDTD(const QString& dtdName)
{
  const DTDStruct *oldDtd = 0L;

  if (!currentToolbarDTD.isEmpty())
  {
     oldDtd = DTDs::ref()->find(currentToolbarDTD);
     if (!oldDtd)
       oldDtd = DTDs::ref()->find(Project::ref()->defaultDTD());
  }
  QString fileName;
  const DTDStruct *newDtd = DTDs::ref()->find(dtdName);
  if (!newDtd)
  {
      newDtd = DTDs::ref()->find(Project::ref()->defaultDTD());
      if (!newDtd)
          newDtd = DTDs::ref()->find(qConfig.defaultDocType); //extreme case
  }

  ToolbarEntry *p_toolbar;
  if (m_debugger->UI())
  {
    p_toolbar = toolbarList[i18n("Debug").lower()];
    if (p_toolbar)
    {
        guiFactory()->removeClient(p_toolbar->guiClient);
        p_toolbar->visible = false;
        delete p_toolbar->menu;
        p_toolbar->menu = 0L;
    }
  }
  if (newDtd != oldDtd)
  {
    KURL::List newToolbars;
    for (uint i = 0; i < newDtd->toolbars.count(); i++)
    {
      KURL url;
      //first load the local version if it exists
      fileName = locateLocal("data", resourceDir + "toolbars/"+newDtd->toolbars[i]);
      QuantaCommon::setUrl(url, fileName);
      if (QExtFileInfo::exists(url))
      {
        //showToolbarFile(url);
        newToolbars += url;
      } else
      {
        fileName = qConfig.globalDataDir + resourceDir + "toolbars/"+newDtd->toolbars[i];
        QuantaCommon::setUrl(url, fileName);
        if (QExtFileInfo::exists(url))
        {
          newToolbars += url;//  showToolbarFile(url);
        }
      }
    }
    //remove the toolbars of the oldDtdName
    if (!currentToolbarDTD.isEmpty())
    {
      for (uint i = 0; i < oldDtd->toolbars.count(); i++)
      {
        KURL url;
        QString fileName = qConfig.globalDataDir + resourceDir + "toolbars/"+oldDtd->toolbars[i];
        QuantaCommon::setUrl(url, fileName);
        KURL urlLocal;
        fileName = locateLocal("data", resourceDir + "toolbars/"+oldDtd->toolbars[i]);
        QuantaCommon::setUrl(urlLocal, fileName);
        if (newToolbars.contains(url) == 0)
        {
          QDictIterator<ToolbarEntry> iter(toolbarList);
          for( ; iter.current(); ++iter )
          {
            p_toolbar = iter.current();
            if (p_toolbar->url == url || p_toolbar->url == urlLocal)
            {
              guiFactory()->removeClient(p_toolbar->guiClient);
              p_toolbar->visible = false;
              delete p_toolbar->menu;
              p_toolbar->menu = 0L;
              break;
            }
          }
        } else
        {
          newToolbars.remove(url);
        }
      }
    }

    //Load the toolbars for dtdName
    KURL::List::Iterator it;
    for (it = newToolbars.begin(); it != newToolbars.end(); ++it)
    {
      showToolbarFile(*it);
    }
    ToolbarTabWidget::ref()->setCurrentPage(0);
 }
 currentToolbarDTD = newDtd->name;
 slotToggleDTDToolbar(!allToolbarsHidden());
}

/** Remove the toolbar named "name". */
bool QuantaApp::slotRemoveToolbar(const QString& name)
{
  ToolbarEntry *p_toolbar = toolbarList[name];
  QRegExp i18ntabnameRx("\\si18ntabname=\"[^\"]*\"");
  if (p_toolbar)
  {
    KXMLGUIClient* toolbarGUI = p_toolbar->guiClient;

    if (toolbarGUI)
    {
     KAction *action;
    //Rename the _Separator_ tags back to Separator, so they are not treated
    //as changes
     QDomNodeList nodeList = toolbarGUI->domDocument().elementsByTagName("_Separator_");
     for (uint i = 0; i < nodeList.count(); i++)
     {
       nodeList.item(i).toElement().setTagName("Separator");
     }
     //check if the actions on the toolbar were modified or not
     bool actionsModified = false;
     nodeList = toolbarGUI->domDocument().elementsByTagName("Action");
     for (uint i = 0; i < nodeList.count(); i++)
     {
       action = actionCollection()->action(nodeList.item(i).toElement().attribute("name"));
       if (dynamic_cast<TagAction*>(action) &&
           dynamic_cast<TagAction*>(action)->isModified())
       {
          actionsModified = true;
          break;
       }
     }

     //check if the toolbar's XML GUI was modified or not
     QString s1 = p_toolbar->dom->toString();
     QString s2 = toolbarGUI->domDocument().toString();
     s1.remove(i18ntabnameRx);
     s2.remove(i18ntabnameRx);
     if (p_toolbar->nameModified)
     {
       QRegExp tabnameRx("\\stabname=\"[^\"]*\"");
       tabnameRx.search(s2);
       QString name1 = tabnameRx.cap();
       name1.remove(" tab");
       QString name2 = name1;
       name2.remove(QRegExp("[\\s]\\([0-9]+\\)"));
       s2.replace(name1, name2);
       s2.replace(name1.lower(), name2.lower());
     }
     bool useToolbarGUI = true;
     if ( s1 != s2 /*|| actionsModified */)
     {
      int result;
      if (p_toolbar->url.isEmpty())
      {
         result = KMessageBox::questionYesNoCancel(this, i18n("<qt>Toolbar <b>%1/<b> is new and unsaved. Do you want to save it before it is removed?</qt>").arg(p_toolbar->name),
              i18n("Save Toolbar"));
      } else
      {
         FourButtonMessageBox dlg(this, 0, true);
         dlg.textLabel->setText(i18n("<qt>The toolbar <b>%1</b> was modified. Do you want to save it before it is removed?</qt>").arg(p_toolbar->name));
         dlg.setCaption(i18n("Save Toolbar"));
         dlg.pixmapLabel->setPixmap(BarIcon("messagebox_info", KIcon::SizeMedium));
         dlg.exec();
         result = dlg.status();
         switch (result)
         {
           case -1: result = KMessageBox::Cancel;
                    break;
           case  1: result = KMessageBox::Continue; //hack - this means Save
                    break;
           case  2: result = KMessageBox::Yes; //hack - this means Save As
                    break;
           case  3: result = KMessageBox::No; //this means Don't Save
                    break;
         }
       }
      switch (result)
      {
        case KMessageBox::Yes:
             {
                bool local = true;
                if (Project::ref()->hasProject() && p_toolbar->url.url().startsWith(Project::ref()->projectBaseURL().url())) local = false;
                if (!saveToolbar(local, p_toolbar->name))
                    return false;
                break;
             }
        case KMessageBox::Continue:
             {
                bool local = true;
                if (Project::ref()->hasProject() && p_toolbar->url.url().startsWith(Project::ref()->projectBaseURL().url())) local = false;
                if (!saveToolbar(local, p_toolbar->name, p_toolbar->url))
                    return false;
                break;
             }
        case KMessageBox::No:
             {
               useToolbarGUI = false;
               break;
             }
        case KMessageBox::Cancel: return false;

      }
     }

     guiFactory()->removeClient(toolbarGUI);
     delete p_toolbar->menu;
//unplug the actions and remove them if they are not used in other places
     if (useToolbarGUI)
       nodeList = toolbarGUI->domDocument().elementsByTagName("Action");
     else
       nodeList = p_toolbar->dom->elementsByTagName("Action");
     for (uint i = 0; i < nodeList.count(); i++)
     {
       action = actionCollection()->action(nodeList.item(i).toElement().attribute("name"));
       if (action && !action->isPlugged())
       {
         if (dynamic_cast<TagAction*>(action) &&
             !dynamic_cast<TagAction*>(action)->isModified())
         {
#if KDE_IS_VERSION(3,2,90)
             //take out the action from every toolbar's xmlguiclient
             //this avoid a crash when removing a toolbar
            QDictIterator<ToolbarEntry> it(toolbarList);
            QStringList toolbarNameList;
            while (it.current())
            {
              it.current()->guiClient->actionCollection()->take(action);
              ++it;
            }
#endif
            delete action;
         }
       }
     }
     delete p_toolbar->dom;
     delete p_toolbar->guiClient;
     toolbarList.remove(name);
    }
  }

  slotToggleDTDToolbar(!allToolbarsHidden());
  return true;
}

/** Show or hide the DTD toolbar */
void QuantaApp::slotToggleDTDToolbar(bool show)
{
  if (show)
  {
     ToolbarTabWidget::ref()->show();
  } else
  {
     ToolbarTabWidget::ref()->hide();
  }
  qConfig.enableDTDToolbar = show;
  if (ViewManager::ref()->activeView())
    ViewManager::ref()->activeView()->refreshWindow();
}


/** No descriptions */
void QuantaApp::slotShowGroupsForDTEP(const QString& dtepName, bool show)
{
  Document *w = ViewManager::ref()->activeDocument();
  if (w)
  {
    if (dtepName == "clear")
    {
      w->resetGroupsForDTEPList();
    } else
    {
      w->enableGroupsForDTEP(dtepName, show);
    }
    w->setChanged(true);
    reparse(false);
  }
}

KURL QuantaApp::projectBaseURL() const
{
  return Project::ref()->projectBaseURL();
}

/** No descriptions */
void QuantaApp::slotBuildPrjToolbarsMenu()
{
  static bool buildInProgress = false;
  if (buildInProgress)
    return;
  KURL::List toolbarList;
  if (Project::ref()->hasProject())
  {
    buildInProgress = true;
    toolbarList = QExtFileInfo::allFiles(Project::ref()->toolbarURL(), "*"+toolbarExtension);
    buildInProgress = false;
    projectToolbarFiles->setMaxItems(toolbarList.count());
    for (uint i = 0; i < toolbarList.count(); i++)
    {
        projectToolbarFiles->addURL(toolbarList[i]);
    }
  } else
  {
    projectToolbarFiles->clearURLList();
  }
}

/** Returns the project (if there is one loaded) or global default encoding. */
QString QuantaApp::defaultEncoding()
{
  QString encoding = qConfig.defaultEncoding;
  if (Project::ref()->hasProject())
  {
    encoding = Project::ref()->defaultEncoding();
  }
  return encoding;
}

void QuantaApp::slotGetUserToolbarFiles(KURL::List *list)
{
  ToolbarEntry *p_toolbar;
  QDictIterator<ToolbarEntry> iter(toolbarList);
  for( ; iter.current(); ++iter )
  {
    p_toolbar = iter.current();
    if (p_toolbar->user && p_toolbar->visible)
    {
      list->append(p_toolbar->url);
    }
  }
}

ToolbarEntry *QuantaApp::toolbarByURL(const KURL& url)
{
  ToolbarEntry *p_toolbar = 0L;
  QDictIterator<ToolbarEntry> iter(toolbarList);
  for( ; iter.current(); ++iter )
  {
    p_toolbar = iter.current();
    if (p_toolbar->url == url)
    {
       return p_toolbar;
    }
  }

  return 0L;
}


/** Returns true if all toolbars are hidden, false otherwise. */
bool QuantaApp::allToolbarsHidden() const
{
  bool result = true;
  showDTDToolbar->setEnabled(false);
  ToolbarEntry *p_toolbar = 0L;
  QDictIterator<ToolbarEntry> iter(toolbarList);
  for( ; iter.current(); ++iter )
  {
    p_toolbar = iter.current();
    if (p_toolbar->visible)
    {
       showDTDToolbar->setEnabled(true);
       result = false;
       break;
    }
  }

  if (!showDTDToolbar->isChecked())
      result = true;
  return result;
}


void QuantaApp::slotLoadDTEP()
{
  QString dirName = KFileDialog::getExistingDirectory(QString::null, 0, i18n("Select DTEP Directory"));
  if (!dirName.isEmpty())
  {
     DTDs::ref()->slotLoadDTEP(dirName, true);
  }
}

void QuantaApp::slotEmailDTEP()
{
  Document *w = ViewManager::ref()->activeDocument();
  if (w)
  {
    QStringList lst(DTDs::ref()->nickNameList());
    QString nickName = DTDs::ref()->getDTDNickNameFromName(w->getDTDIdentifier());
    bool ok = FALSE;
    QString res = KInputDialog::getItem(
                    i18n( "Send DTD" ),
                    i18n( "Please select a DTD:" ), lst, lst.findIndex(nickName), FALSE, &ok, this );

    if (!ok)
      return;

    QString dtdName = DTDs::ref()->getDTDNameFromNickName(res);

    QStringList dtdFile;

    QString prefix="quanta";
    KTempDir* tempDir = new KTempDir(tmpDir);
    tempDir->setAutoDelete(true);
    tempDirList.append(tempDir);
    QString tempFileName=tempDir->name() +"/"+ DTDs::ref()->getDTDNickNameFromName(dtdName).replace(QRegExp("\\s|\\."), "_") + ".tgz";

  //pack the .tag files and the description.rc into a .tgz file
    KTar tar(tempFileName, "application/x-gzip");
    tar.open(IO_WriteOnly);

    KURL dirURL;
    dirURL.setPath(DTDs::ref()->find(dtdName)->fileName);
    dirURL.setPath(dirURL.directory(false));

    KURL::List files = QExtFileInfo::allFilesRelative(dirURL, "*");
    for ( KURL::List::Iterator it_f = files.begin(); it_f != files.end(); ++it_f )
    {
      QString name = (*it_f).fileName();

      QFile file(dirURL.path()+name);
      file.open(IO_ReadOnly);
      QByteArray bArray = file.readAll();
      tar.writeFile(dirURL.fileName()+"/"+name, "user", "group", bArray.size(), bArray.data());
      file.close();

    }
    tar.close();
    dtdFile += tempFileName;


    TagMailDlg *mailDlg = new TagMailDlg( this, i18n("Send DTEP in Email"));
    QString toStr;
    QString message = i18n("Hi,\n This is a Quanta Plus [http://quanta.sourceforge.net] DTEP definition tarball.\n\nHave fun.\n");
    QString titleStr;
    QString subjectStr;

    mailDlg->TitleLabel->setText(i18n("Content:"));
    mailDlg->titleEdit->setFixedHeight(60);
    mailDlg->titleEdit->setVScrollBarMode(QTextEdit::Auto);
    mailDlg->titleEdit->setHScrollBarMode(QTextEdit::Auto);
    if ( mailDlg->exec() )
    {
      if ( !mailDlg->lineEmail->text().isEmpty())
      {
        toStr = mailDlg->lineEmail->text();
        subjectStr = (mailDlg->lineSubject->text().isEmpty())?i18n("Quanta Plus DTD"):mailDlg->lineSubject->text();
        if ( !mailDlg->titleEdit->text().isEmpty())
            message = mailDlg->titleEdit->text();
      } else
      {
        KMessageBox::error(this,i18n("No destination address was specified.\n Sending is aborted."),i18n("Error Sending Email"));
        delete mailDlg;
        return;
      }

      kapp->invokeMailer(toStr, QString::null, QString::null, subjectStr, message, QString::null, dtdFile);
    }
    delete mailDlg;
  }
}

void QuantaApp::slotDownloadDTEP()
{
    if (!m_newDTEPStuff)
      m_newDTEPStuff = new QNewDTEPStuff("quanta/dtep", this);
    m_newDTEPStuff->download();
}

void QuantaApp::slotUploadDTEP()
{
}

void QuantaApp::slotDownloadToolbar()
{
    if (!m_newToolbarStuff)
      m_newToolbarStuff = new QNewToolbarStuff("quanta/toolbar", this);
    m_newToolbarStuff->download();
}

void QuantaApp::slotSmartTagInsertion()
{
  KAction *action = actionCollection()->action("smart_tag_insertion");
  if(!action)
    return;
  if(!ViewManager::ref()->activeDocument() || !ViewManager::ref()->activeView() ||
    ViewManager::ref()->activeDocument()->defaultDTD()->name.contains("HTML", false) == 0)
  {
    KMessageBox::error(this, "Smart Tag Insertion is available only for (X)HTML for the moment.");
    qConfig.smartTagInsertion = false;
    (static_cast<KToggleAction* >(action))->setChecked(false);
    return;
  }
  qConfig.smartTagInsertion = (static_cast<KToggleAction* >(action))->isChecked();
}

void QuantaApp::slotDownloadTemplate()
{
    if (!m_newTemplateStuff)
      m_newTemplateStuff = new QNewTemplateStuff("quanta/template", this);
    m_newTemplateStuff->download();
}

void QuantaApp::slotDownloadScript()
{
    if (!m_newScriptStuff)
      m_newScriptStuff = new QNewScriptStuff("quanta/script", this);
    m_newScriptStuff->download();
}

void QuantaApp::slotCodeFormatting()
{
  QuantaView *view = ViewManager::ref()->activeView();
  if(!view || !view->document() || (view->currentViewsLayout() != QuantaView::SourceOnly &&
    view->hadLastFocus() == QuantaView::VPLFocus))
  {
    KMessageBox::error(this, i18n("Code formatting can only be done in the source view."));
    return;
  }
  view->document()->docUndoRedo->codeFormatting();
}

void QuantaApp::slotDocumentProperties()
{
  documentProperties(false);
}

void QuantaApp::documentProperties(bool forceInsertionOfMinimalTree)
{
  Document *w = ViewManager::ref()->activeDocument();
  if(w)
  {
    if(w->defaultDTD()->name.contains("HTML", false) == 0)
    {
      KMessageBox::information(this, i18n("The Document Properties Dialog is only for HTML and XHTML."));
      return;
    }
    htmlDocumentProperties htmlPropsDlg(this, forceInsertionOfMinimalTree);
    htmlPropsDlg.exec();
  }
}

/** Returns the interface number for the currently active editor. */
int QuantaApp::currentEditorIfNum() const
{
  Document *w = ViewManager::ref()->activeDocument();
  if (w)
  {
    return w->editIf->editInterfaceNumber();
  } else
  {
    QuantaView * view = ViewManager::ref()->lastActiveEditorView();
    if (view)
      return view->document()->editIf->editInterfaceNumber();
    else
      return 0L;
  }
}

/** Return the URL of the currently active document */
QString QuantaApp::currentURL() const
{
  Document *w = ViewManager::ref()->activeDocument();
  if (w)
  {
    return w->url().url();
  } else
  {
    QuantaView * view = ViewManager::ref()->lastActiveEditorView();
    if (view)
      return view->document()->url().url();
    else
      return "";
  }
}

QString QuantaApp::projectURL() const
{
  return Project::ref()->projectBaseURL().url();
}

QStringList QuantaApp::openedURLs() const
{
  QStringList list;
  QPtrListIterator<KMdiChildView> childIt(*m_pDocumentViews);
  KMdiChildView *view;
  QuantaView *qView;
  while ( (view = childIt.current()) != 0 )
  {
      ++childIt;
      qView = dynamic_cast<QuantaView*>(view);
      if (qView)
      {
          Document *w = qView->document();
          if ( w )
          {
            list.prepend( QString("%1:%2").arg(w->editIf->editInterfaceNumber()).arg(w->url().url()));
          }
      }
  }

  return list;
}

void QuantaApp::slotExpandAbbreviation()
{
  Document *w = ViewManager::ref()->activeDocument();
  if (w)
  {
    const DTDStruct *dtd = w->currentDTD();
    uint line, col;
    w->viewCursorIf->cursorPositionReal(&line, &col);
    QString text = w->text(line, 0, line, col - 1);
    text = w->findWordRev(text) + " ";
    QString textToInsert;
    QMap<QString, Abbreviation>::ConstIterator it;
    for (it = qConfig.abbreviations.constBegin(); it != qConfig.abbreviations.constEnd(); ++it)
    {
        bool found = false;
        Abbreviation abbrev = it.data();
        if (abbrev.dteps.contains(dtd->name))
        {
            QMap<QString, QString>::ConstIterator it2;
            for (it2 = abbrev.abbreviations.constBegin(); it2 != abbrev.abbreviations.constEnd(); ++it2)
            {
               if (it2.key().startsWith(text))
               {
                   textToInsert = it2.data();
                   found = true;
                   break;
               }
            }
        }
        if (found)
          break;
    }
    if (!textToInsert.isEmpty())
    {
      w->activateParser(false);
      w->editIf->removeText(line, col - text.length() + 1, line, col);
      w->activateParser(true);
      col -= (text.length() - 1);
      int pos = textToInsert.find('|');
      if (pos != -1)
      {
        text = textToInsert.left(pos);
        if (text.contains('\n'))
        {
          line += text.contains('\n');
          col = text.length() - text.findRev('\n') - 1;
        } else
        {
          col += pos;
        }
      }
      textToInsert.replace('|',"");
      w->insertText(textToInsert, false);
      w->viewCursorIf->setCursorPositionReal(line, col);
    }
  }
}

void QuantaApp::slotUploadFile(QuantaView *view)
{
  if (!view)
    view = ViewManager::ref()->activeView();
  Project::ref()->slotUploadURL(view->document()->url());
}


void QuantaApp::slotUploadOpenedFiles()
{
}

void QuantaApp::slotDeleteFile(QuantaView *view)
{
  if (!view)
    view = ViewManager::ref()->activeView();
  Document *w = view->document();
  KURL url = w->url();
/*  if (KMessageBox::questionYesNo(this,
                   i18n("<qt>Do you really want to delete the file <b>%1</b>?</qt>")
                   .arg(url.prettyURL(0, KURL::StripFileProtocol)),
                   i18n("Delete File")) == KMessageBox::Yes)
  {
    if (KIO::NetAccess::del(url, this))
    {
      if (Project::ref()->hasProject())
        Project::ref()->slotRemove(url);
    }*/
  if ( QuantaNetAccess::del(url, this, true) )
  {
    w->setModified(false); //don't ask for save
    slotFileClose();
  }
}


bool QuantaApp::structTreeVisible() const
{
  return StructTreeView::ref()->isVisible();
}

QStringList QuantaApp::tagAreas(const QString &tag, bool includeCoordinates, bool skipFoundContent) const
{
  Document *w = ViewManager::ref()->activeDocument();
  if (w)
    return w->tagAreas(tag, includeCoordinates, skipFoundContent);
  else
    return QStringList();
}

QString QuantaApp::documentFolderForURL(const QString &url)
{
  KURL u = KURL::fromPathOrURL(url);
  return Project::ref()->documentFolderForURL(u).url();
}

QString QuantaApp::urlWithPreviewPrefix(const QString &url)
{
  KURL u = KURL::fromPathOrURL(url);
  return Project::ref()->urlWithPrefix(u).url();
}

void QuantaApp::slotAutosaveTimer()
{
  m_config->reparseConfiguration();
  QPtrListIterator<KMdiChildView> childIt(*m_pDocumentViews);
  KMdiChildView *view;
  QuantaView *qView;
  while ( (view = childIt.current()) != 0 )
  {
      ++childIt;
      qView = dynamic_cast<QuantaView*>(view);
      if (qView)
      {
          Document *w = qView->document();
          if ( w )
          {
            w->createBackup(m_config);
          }
      }
  }

}

/** Get script output */
void QuantaApp::slotGetScriptOutput(KProcess* ,char* buf,int buflen)
{
 QCString tmp( buf, buflen + 1 );
 m_scriptOutput = QString::null;
 m_scriptOutput = QString::fromLocal8Bit(tmp).remove(" ");
}

/** Get script error*/
void QuantaApp::slotGetScriptError(KProcess* ,char* buf,int buflen)
{
//TODO: Implement some error handling?
 Q_UNUSED(buf);
 Q_UNUSED(buflen);
}
/** Notify when process exits*/
void QuantaApp::slotProcessExited(KProcess* )
{
  slotProcessTimeout();
}

/** Timeout occurred while waiting for some network function to return. */
void QuantaApp::slotProcessTimeout()
{
  if (m_loopStarted)
  {
    qApp->exit_loop();
    m_loopStarted = false;
  }
}

void QuantaApp::slotActivePartChanged(KParts::Part * part)
{
  if (m_oldKTextEditor && part) // if part == 0L the pointer m_oldKTextEditor is not useable
  {
    guiFactory()->removeClient(m_oldKTextEditor);
    m_oldKTextEditor = 0L;
  }
   createGUI(part);
  QWidget * activeWid = m_partManager->activeWidget();
  if ( activeWid && activeWid->inherits("KTextEditor::View"))
  {
    m_oldKTextEditor = dynamic_cast<KTextEditor::View *>(activeWid);
    if (m_oldKTextEditor)
      guiFactory()->addClient(m_oldKTextEditor);
  }
}

void QuantaApp::slotConvertCase()
{
  Document *w = ViewManager::ref()->activeDocument();
  if (w)
  {
     w->convertCase();
  }
}

void QuantaApp::slotReloadStructTreeView(bool groupOnly)
{
  Document *w = ViewManager::ref()->activeDocument();
   if (StructTreeView::ref()->isVisible() && w)
  {
    StructTreeView::ref()->setParsingDTDs(w->groupsForDTEPs());
    int expandLevel = qConfig.expandLevel;
    if (expandLevel == 0)
        expandLevel = 40;
    StructTreeView::ref()->slotReparse(w, baseNode, expandLevel, groupOnly);
  } else
    StructTreeView::ref()->slotReparse(0L, 0L, 0); //delete the tree
}

QString QuantaApp::saveCurrentFile()
{
  Document *w = ViewManager::ref()->activeDocument();
  if (!w)
    return QString::null;
  if (w->isModified())
  {
    if ( KMessageBox::questionYesNo(this,
                                    i18n("The file must be saved before external preview.\n"
                                         "Do you want to save and preview?"),
                                    i18n("Save Before Preview"),
                                    KStdGuiItem::yes(),KStdGuiItem::no(), "AskForSaveBeforePreview")
         == KMessageBox::Yes)
    {
      if (w->isUntitled())
      {
       quantaApp->slotFileSaveAs();
      }
      else
      {
       w->save();
      }
    } else
    {
      return QString::null;
    }
  }
  KURL url = Project::ref()->urlWithPrefix(w->url());
  return url.url();
}

void QuantaApp::slotReportBug()
{
  KAboutData aboutData( "quanta", I18N_NOOP("Quanta"), QUANTA_VERSION);
  KBugReport bugReportDlg(this, true, &aboutData);
  bugReportDlg.exec();
}

void QuantaApp::slotNewPart(KParts::Part *newPart, bool setActiv)
{
   m_partManager->addPart(newPart, setActiv);
}


bool QuantaApp::queryClose()
{
  bool canExit = true;
  if (quantaStarted)
  {
    exitingFlag = true;
    canExit = ViewManager::ref()->saveAll(false);
    if (canExit)
       canExit = Project::ref()->queryClose();
    saveOptions();
    if (canExit)
        canExit = removeToolbars();
  }
  if (canExit)
    emit eventHappened("quanta_exit", QDateTime::currentDateTime().toString(Qt::ISODate), QString::null);
  return canExit;
}

void QuantaApp::saveOptions()
{
  if (m_config)
  {
    m_config->setGroup  ("General Options");

    m_config->writeEntry("Geometry", size());

    m_config->writeEntry("Show Toolbar", toolBar("mainToolBar")->isVisible());
    m_config->writeEntry("Show DTD Toolbar", showDTDToolbar->isChecked());

    m_config->writeEntry("Markup mimetypes", qConfig.markupMimeTypes  );
    m_config->writeEntry("Script mimetypes", qConfig.scriptMimeTypes   );
    m_config->writeEntry("Image mimetypes", qConfig.imageMimeTypes );
    m_config->writeEntry("Text mimetypes", qConfig.textMimeTypes  );

    m_config->writeEntry("Capitals for tags", qConfig.tagCase);
    m_config->writeEntry("Capitals for attr", qConfig.attrCase);
    m_config->writeEntry("Attribute quotation", qConfig.attrValueQuotation=='"' ? "double":"single");
    m_config->writeEntry("Close tag if optional", qConfig.closeOptionalTags);
    m_config->writeEntry("Close tags", qConfig.closeTags);
    m_config->writeEntry("Auto completion", qConfig.useAutoCompletion);
    m_config->writeEntry("Update Closing Tags", qConfig.updateClosingTags);
    m_config->writeEntry("Replace Accented Chars", qConfig.replaceAccented);

    m_config->writeEntry("Default encoding", qConfig.defaultEncoding);
    m_config->writeEntry("Default DTD", qConfig.defaultDocType);

    m_config->writeEntry("Preview area", qConfig.previewPosition);
    m_config->writeEntry("Documentation area", qConfig.docPosition);

    m_config->writeEntry("Smart Tag Insertion", qConfig.smartTagInsertion);

    m_config->writeEntry("Window layout", qConfig.windowLayout);
    m_config->writeEntry("Follow Cursor", StructTreeView::ref()->followCursor() );
    //If user choose the timer interval, it needs to restart the timer too
    m_config->writeEntry("Autosave interval", qConfig.autosaveInterval);
    m_config->writePathEntry("Top folders", fTab->topURLList.toStringList());
    m_config->writePathEntry("List of opened files", ViewManager::ref()->openedFiles().toStringList());
    m_config->writeEntry("Version", QUANTA_VERSION); // version
    m_config->writeEntry("Close Buttons", qConfig.showCloseButtons);
    m_config->writeEntry("MDI mode", mdiMode());
    m_config->writeEntry("MDI style", qConfig.toolviewTabs);

    m_config->deleteGroup("RecentFiles");
    fileRecent->saveEntries(m_config);
    m_config->writeEntry("Show Hidden Files", qConfig.showHiddenFiles);
    m_config->writeEntry("Save Local Trees", qConfig.saveTrees);

    m_config->setGroup("Parser options");
    m_config->writeEntry("Instant Update", qConfig.instantUpdate);
    m_config->writeEntry("Show Empty Nodes", qConfig.showEmptyNodes);
    m_config->writeEntry("Show Closing Tags", qConfig.showClosingTags);
    m_config->writeEntry("Refresh frequency", qConfig.refreshFrequency);
    m_config->writeEntry("Expand Level", qConfig.expandLevel);
    m_config->writeEntry("Show DTD Select Dialog", qConfig.showDTDSelectDialog);

    manager()->writeConfig(m_config);
    saveMainWindowSettings(m_config);
    SpellChecker::ref()->writeConfig(m_config);
    writeDockConfig(m_config);
    // save settings of treeviews
    fTab->saveLayout( m_config, fTab->className() );
    ProjectTreeView::ref()->saveLayout(m_config, ProjectTreeView::ref()->className() );
    TemplatesTreeView::ref()->saveLayout(m_config, TemplatesTreeView::ref()->className() );
    scriptTab->saveLayout(m_config, scriptTab->className() );

    m_config->sync();
  }
}

void QuantaApp::statusBarTimeout()
{
  statusBar()->changeItem("", IDS_STATUS);
}

QStringList QuantaApp::selectors(const QString &tag)
{
  return dcopQuanta->selectors(tag);
}

QStringList QuantaApp::idSelectors()
{
  return dcopQuanta->idSelectors();
}

void QuantaApp::slotEditCurrentTag()
{
  Document *w = ViewManager::ref()->activeDocument();
  if (!w) return;
  if (parser->parsingNeeded())
    baseNode = parser->rebuild(w);
  //avoid reparsing while the dialog is shown
  typingInProgress = true;
  enableIdleTimer(false);
  uint line,col;
  w->viewCursorIf->cursorPositionReal(&line, &col);
  Node *node = parser->nodeAt(line, col, false);
  bool isUnknown = true;
  QString tagName;
  if (node && node->tag)
  {
    Tag *tag = node->tag;
    tagName = tag->name;
    if ( QuantaCommon::isKnownTag(tag->dtd()->name,tagName) )
    {
      isUnknown = false;
      QString selection(w->selectionIf->selection());
      TagDialog *dlg = new TagDialog( QuantaCommon::tagFromDTD(tag->dtd(),tagName), tag, selection, ViewManager::ref()->activeView()->baseURL() );
      if (dlg->exec())
      {
        w->changeTag(tag, dlg->getAttributes() );
      }

      delete dlg;
    }
  }
  typingInProgress = false;
  enableIdleTimer(true);
  if (isUnknown)
  {
    QString message = i18n("Unknown tag: %1").arg(tagName);
    slotStatusMsg( message );
  }
}

void QuantaApp::slotSelectTagArea(Node *node)
{
  int bLine, bCol, eLine, eCol;
  Tag *tag = node->tag;
  tag->beginPos(bLine, bCol);
  if (tag->single || !node->next)
  {
    tag->endPos(eLine, eCol);
  } else
  if (tag->closingMissing && node->child)
  {
    Node *childNode = node->child;
    while (childNode->child || childNode->next)
    {
      if (childNode->next)
      {
        childNode = childNode->next;
      } else
      {
        childNode = childNode->child;
      }
    }
    childNode->tag->endPos(eLine, eCol);
  } else
  {
    node->next->tag->endPos(eLine, eCol);
  }
  quantaApp->selectArea(bLine, bCol, eLine, eCol + 1);
}

void QuantaApp::slotSelectTagArea()
{
  Document *w = ViewManager::ref()->activeDocument();
  if (!w)
      return;
  uint line,col;
  w->viewCursorIf->cursorPositionReal(&line, &col);
  Node *node = parser->nodeAt(line, col);
  slotSelectTagArea(node);
}

void QuantaApp::slotFrameWizard()
{
  Document *w = ViewManager::ref()->activeDocument();
  if (!w)
      return;
  QStringList list = w->tagAreas("frameset", true, true);
  bool framesetExists = !list.isEmpty();
  int bl, bc, el, ec;
  bl = bc = el = ec = 0;
  QStringList l;
  QStringList l2;
  QuantaCommon::normalizeStructure(list[0],l2);
  if (framesetExists)
  {
    l = QStringList::split('\n',list[0],true);
    QStringList coordList = QStringList::split(',', l[0], true);
    bl = coordList[0].toInt();
    bc = coordList[1].toInt();
    el = coordList[2].toInt();
    ec = coordList[3].toInt();
    l.remove(l.begin());
  }

  FrameWizard dlg(this);

  if (!w->isUntitled())
      {
       dlg.setSaved(true);
      }
  dlg.setMarkupLanguage(w->currentDTD(true)->name);
  dlg.loadExistingFramesetStructure(l2);

  if ( dlg.exec() )
  {
    QString tag =
QString("\n")+dlg.generateFramesetStructure()+QString("\n");
    if (framesetExists)
    {
      w->activateParser(false);
      w->editIf->removeText(bl, bc, el, ec + 1);
      w->viewCursorIf->setCursorPositionReal((uint)bl, (uint)bc);
      w->activateParser(true);
    }
    w->insertTag(tag);
  }
}


/** edit/insert CSS */
void QuantaApp::slotInsertCSS()
{
  Document *w = ViewManager::ref()->activeDocument();
  if (!w) return;

  uint line, col;
  int bLine, bCol, eLine, eCol;
  bLine = bCol = eLine = eCol = 0;
  w->viewCursorIf->cursorPositionReal(&line, &col);
  if (line == 0 && col == 0)
    col++;
  parser->rebuild(w);
  Node *node = parser->nodeAt(line, col, false);
  unsigned int lastLine = w->editIf->numLines() - 1;
  unsigned int lastCol = w->editIf->lineLength(lastLine);
  Node *styleNode = node;

  if (styleNode->tag->type == Tag::XmlTagEnd && styleNode->prev)
    styleNode = styleNode->prev;

  while (styleNode && styleNode->parent && styleNode->tag->name.lower() != "style" && styleNode->tag->dtd()->name == "text/css")
    styleNode = styleNode->parent;

  Node *parentNode = node;
  if (parentNode->tag->type == Tag::XmlTagEnd && parentNode->prev)
    parentNode = parentNode->prev;
  else
    while (parentNode && parentNode->parent &&
           parentNode->tag->type != Tag::XmlTag)
      parentNode = parentNode->parent;
  QString fullDocument = w->editIf->text().stripWhiteSpace();

  if (styleNode->tag->name.lower() == "comment block" && styleNode->parent) {
    if (styleNode->parent->tag->name.lower() == "style") {
      styleNode = styleNode->parent;
    }
  }

  if (styleNode && styleNode->tag->name.lower() == "style" && styleNode->next)  //inside <style> invoke the selector editor
  {
    styleNode->tag->endPos(bLine, bCol);
    QString header(w->text(0, 0, bLine, bCol));// beginning part of the file
    styleNode->next->tag->endPos(eLine, eCol);
    QString footer("</style>" + w->text(eLine, eCol+1, lastLine, lastCol)); // ending part of the file

    styleNode->next->tag->beginPos(eLine, eCol);
    QString styleTagContent(w->text(bLine, bCol+1, eLine, eCol-1).remove("<!--").remove("-->"));// <style></style> block content
        kdDebug(24000) << "Style tag contains: " << endl << styleTagContent << endl;
    CSSSelector *dlg = new CSSSelector;

    dlg->setCallingFrom("XHTML");
    QFileInfo fi(quantaApp->currentURL());
    dlg->setFileToPreview(quantaApp->projectBaseURL().path() +  fi.baseName());


    dlg->setHeader(header);
    dlg->setFooter(footer);

    dlg->loadCSSContent(styleTagContent);
    if(!dlg->errorOnProcessingStylesheet())
    if( dlg->exec() ){
      w->activateParser(false);
      styleNode->next->tag->beginPos(eLine, eCol);
      w->editIf->removeText(bLine, bCol+1, eLine, eCol);
      w->viewCursorIf->setCursorPositionReal((uint)bLine, (uint)bCol+1);
      w->activateParser(true);
      w->insertTag( /*"\n<!--" + */ dlg->generateFormattedStyleSection()  /*+ "-->\n"*/);
    }
    delete dlg;

  } else if (!node || fullDocument.isEmpty() ||
      w->currentDTD(true)->name == "text/css") //empty document or pure CSS file, invoke the selector editor
  {
        kdDebug(24000) << "[CSS editor] This is a pure CSS document";

    CSSSelector *dlg = new CSSSelector;

    dlg->setCallingFrom("CSS");

    if (!fullDocument.isEmpty())
      dlg->loadCSSContent(fullDocument);
    dlg->enableApplyToFile();
    if(!dlg->errorOnProcessingStylesheet())
      if (dlg->exec())
      {
      w->activateParser(false);
      w->editIf->clear();
      w->activateParser(true);
      w->insertTag(dlg->generateFormattedStyleSection());
      }
     delete dlg;
  } else
  if (parentNode && parentNode->tag->type == Tag::XmlTag)
  {
        kdDebug(24000) << "[CSS editor] We will add a style attribute to: " << parentNode->tag->name << endl;
    CSSEditor *dlg = new CSSEditor(this);
    QFileInfo fi(quantaApp->currentURL());
    dlg->setFileToPreview(quantaApp->projectBaseURL().path() +  fi.baseName(),false);


    parentNode->tag->beginPos(bLine, bCol);
    parentNode->tag->endPos(eLine, eCol);
    dlg->setFooter(">" + w->text(eLine, eCol + 1, lastLine, lastCol));

    QString temp;
    if (parentNode->tag->hasAttribute("style"))
    {
      dlg->setInlineStyleContent(parentNode->tag->attributeValue("style"));
      Tag tempTag(*(parentNode->tag));
      tempTag.deleteAttribute("style");
      temp = tempTag.toString();

    } else {
      dlg->setInlineStyleContent(QString::null);
      temp = parentNode->tag->toString();
    }
    //using QString::mid sometimes generates strange results; maybe this is due to a (random) blank in temp
    temp = temp.left(temp.length()-1);//remove >
    temp = temp.right(temp.length()-1);//remove <
    dlg->setHeader(w->text(0, 0, bLine, bCol) + temp);

    dlg->initialize();
    if( dlg->exec() )
    {
      w->changeTagAttribute(parentNode->tag, "style", dlg->generateProperties());
    }
    delete dlg;
   } else
   KMessageBox::sorry(this, i18n("The CSS Editor cannot be invoked here.\nTry to invoke it on a tag or on a style section."));
}

/** for <a href=mailto> tag  */
void QuantaApp::slotTagMail()
{
  Document *w = ViewManager::ref()->activeDocument();
  if (!w) return;

  TagMailDlg *mailDlg = new TagMailDlg( this, i18n("Email Link (mailto)"));

  if ( mailDlg->exec() ) {
    QString tag = QString(QuantaCommon::tagCase("<a"));

    if ( !QString(mailDlg->lineEmail->text()).isEmpty())
    {
      tag += QuantaCommon::attrCase(" href=")+qConfig.attrValueQuotation+"mailto:"+mailDlg->lineEmail->text();
        if ( !QString(mailDlg->lineSubject->text()).isEmpty())
         tag += "?subject="+KURL::encode_string(mailDlg->lineSubject->text());
         tag += qConfig.attrValueQuotation;
    }

    if ( !QString(mailDlg->titleEdit->text()).isEmpty())
         tag += QuantaCommon::attrCase(" title=")+qConfig.attrValueQuotation+mailDlg->titleEdit->text()+qConfig.attrValueQuotation;
    tag += QString(">");
    w->insertTag(tag,QuantaCommon::tagCase("</a>"));
  }
  delete mailDlg;
}

/** Add the starting and closing text for a
user specified tag. */
void QuantaApp::slotTagMisc()
{
  Document *w = ViewManager::ref()->activeDocument();
  if (!w) return;

 static QString element = "";
 static bool addClosingTag = true;

 KDialogBase miscDlg(this, 0L, true, i18n("Misc. Tag"), KDialogBase::Ok | KDialogBase::Cancel);
 TagMisc *miscWidget = new TagMisc(&miscDlg);
 miscDlg.setMainWidget(miscWidget);
 miscWidget->addClosingTag->setChecked(addClosingTag);
 miscWidget->elementName->setText(element);

  if ( miscDlg.exec() )
  {
    QString tag;
    element = miscWidget->elementName->text();
    element.remove('<');
    element.remove('>');
    if ( !element.isEmpty())
    {
      tag += "<" + QuantaCommon::attrCase(element)+">";
        if ( (addClosingTag = miscWidget->addClosingTag->isChecked()) == true)
        {
          w->insertTag(tag,QuantaCommon::tagCase( "</"+QuantaCommon::attrCase(element)+">"));
        } else
        {
          w->insertTag(tag,QuantaCommon::tagCase(""));
        }
    }
  }
}

/** do quick list */
void QuantaApp::slotTagQuickList()
{
  QString space =" " ;
  Document *w = ViewManager::ref()->activeDocument();
  if (!w) return;

  TagQuickListDlg *listDlg = new TagQuickListDlg(this,i18n("Generate List"));
  if ( listDlg->exec() ) {
    int i;
    int n = listDlg->spinBoxRows->value();

    QString tag;
    if ( listDlg->radioOrdered->isChecked())
      tag = QString("<ol>\n")+space;
    else tag = QString("<ul>\n")+space;

    for ( i=0;i<n;i++)
      if ( qConfig.closeTags )
        tag += QString("  <li>  </li>\n")+space;
      else
        tag += QString("  <li> \n")+space;

    if ( listDlg->radioOrdered->isChecked())
      tag += QString("</ol>");
    else tag += QString("</ul>");

    w->insertTag( QuantaCommon::tagCase(tag));
  }
  delete(listDlg);
}

void QuantaApp::slotTagEditTable()
{
  Document *w = ViewManager::ref()->activeDocument();
  if (!w) return;
  baseNode = parser->rebuild(w);
  QStringList list = w->tagAreas("table", true, false);
  bool tableExists = false;
  uint line, col;
  w->viewCursorIf->cursorPositionReal(&line, &col);
  int bl, bc, el, ec;
  int bLine, bCol, eLine, eCol;
  bLine = bCol = eLine = eCol = 0;
  QStringList l;
  QStringList l2;
  for (QStringList::Iterator it = list.begin(); it != list.end(); ++it)
  {
    QuantaCommon::normalizeStructure(*it, l2);
    l = QStringList::split('\n', *it, true);
    QStringList coordList = QStringList::split(',', l[0], true);
    bl = coordList[0].toInt();
    bc = coordList[1].toInt();
    el = coordList[2].toInt();
    ec = coordList[3].toInt();
    if (QuantaCommon::isBetween(line, col, bl, bc, el, ec) == 0)
    {
      tableExists = true;
      bLine = bl;
      bCol = bc;
      eLine = el;
      eCol = ec;
    }
    l.remove(l.begin());
  }

  TableEditor editor;
  bool tableRead = true;
  if (tableExists)
  {
    editor.setBaseURL(ViewManager::ref()->activeView()->baseURL());
    tableRead = editor.setTableArea(bLine, bCol, eLine, eCol, parser);
    if (!tableRead)
    {
      KMessageBox::error(this, i18n("The table structure is invalid. Most probably you forgot to close some tags."), i18n("Cannot Read Table"));
    }
  } else
  {
    Node *node = parser->nodeAt(line, col);
    const DTDStruct *dtd = w->defaultDTD();
    if (node)
      dtd = node->tag->dtd();
    bLine = line;
    bCol = col;
    eLine = line;
    eCol = col;
    editor.createNewTable(w, dtd);
  }
  if (tableRead && editor.exec())
  {
    QString tableString = editor.readModifiedTable();
    w->activateParser(false);
//#ifdef BUILD_KAFKAPART
//          if(w->editIfExt)
//            w->editIfExt->editBegin();
//#endif
    if (eLine != bLine || (eLine == bLine && eCol != bCol))
      w->editIf->removeText(bLine, bCol, eLine, eCol + 1);
    w->viewCursorIf->setCursorPositionReal((uint)bLine, (uint)bCol);
    w->insertText(tableString, false);
//#ifdef BUILD_KAFKAPART
//          if(w->editIfExt)
//            w->editIfExt->editEnd();
//#endif
    w->viewCursorIf->setCursorPositionReal(line, col);
    quantaApp->reparse(true);
  }
}

/** Open color Dialog and insert color in the text */
void QuantaApp::slotTagColor()
{
  Document *w = ViewManager::ref()->activeDocument();
  if (!w) return;
  QColor color;

  if (KColorDialog::getColor( color )) {
    char c[8];
    sprintf(c,"#%2X%2X%2X",color.red(),color.green(),color.blue());
    for (int i=0;i<7;i++) if (c[i] == ' ') c[i] = '0';
    QString scolor = (char *)c;
    w->insertTag(scolor);
  }
}

/** insert date */
void QuantaApp::slotTagDate()
{
  Document *w = ViewManager::ref()->activeDocument();
  if (!w) return;
  time_t tektime;
  time( &tektime);
  QString stime = ctime( &tektime);

  w->insertTag( stime);
}

/** for select form */
void QuantaApp::slotTagSelect()
{
  Document *w = ViewManager::ref()->activeDocument();
  if (!w) return;
  w->insertTag(QuantaCommon::tagCase("<select")+ QuantaCommon::attrCase("name")+QuantaCommon::tagCase("=\"\"><option>"),QuantaCommon::tagCase("</select>"));
}

void QuantaApp::slotViewInKFM()
{
  Document *w = ViewManager::ref()->activeDocument();
  if (!w) return;
  if (w->isModified())
  {
    if ( KMessageBox::questionYesNo(this,
                                    i18n("The file must be saved before external preview.\n"
                                         "Do you want to save and preview?"),
                                    i18n("Save Before Preview"),
                                    KStdGuiItem::yes(),KStdGuiItem::no(), "AskForSaveBeforePreview")
         == KMessageBox::Yes)
    {
      if (w->isUntitled())
      {
       quantaApp->slotFileSaveAs();
      }
      else
      {
       w->save();
      }
    } else
    {
      return;
    }
  }
  if ( !w->isUntitled() )
  {
    KProcess *show = new KProcess();
    KURL url = Project::ref()->urlWithPrefix(w->url());
#if KDE_IS_VERSION(3,2,90)
    *show << "kfmclient" << "newTab" << url.url();
#else
    *show << "kfmclient" << "openURL" << url.url();
#endif
    show->start( KProcess::DontCare );
  }
}

void QuantaApp::slotViewInLynx()
{
  Document *w = ViewManager::ref()->activeDocument();
  if (!w) return;
  if (w->isModified())
  {
    if ( KMessageBox::questionYesNo(this,
                                    i18n("The file must be saved before external preview.\n"
                                         "Do you want to save and preview?"),
                                    i18n("Save Before Preview"),KStdGuiItem::yes(),KStdGuiItem::no(), "AskForSaveBeforePreview")
         == KMessageBox::Yes)
    {
      if (w->isUntitled())
      {
       quantaApp->slotFileSaveAs();
      }
      else
      {
       w->save();
      }
    } else
    {
      return;
    }
  }
  if ( !w->isUntitled() )
  {
    KProcess *show = new KProcess();
    KURL url = Project::ref()->urlWithPrefix(w->url());
    *show << "konsole"
          << "--nohist"
          << "--notoolbar"
          << "--caption"
          << "Lynx Preview - Quanta"
          << "-e"
          << "lynx"
          << url.url();
    show->start( KProcess::DontCare );
  }
}

/** insert clipboard contents (but quote them for HTML first) */
void QuantaApp::slotPasteHTMLQuoted()
{
  Document *w = ViewManager::ref()->activeDocument();
  if (w)
  {
    QClipboard *cb = qApp->clipboard();
    QString text = cb->text();

    if ( ( !text.isNull() ) && (!text.isEmpty() ) )
    {
      text.replace( "&", "&amp;" );
      text.replace( "<", "&lt;" );
      text.replace( "\"", "&quot;" );
      text.replace( ">", "&gt;" );
      unsigned int line, col;
      w->viewCursorIf->cursorPositionReal(&line, &col);
      w->editIf->insertText(line, col, text );
    }
  }
}

/** insert clipboard contents (but quote them as a URL first) */
void QuantaApp::slotPasteURLEncoded()
{
 Document *w = ViewManager::ref()->activeDocument();
  if (w)
  {
    QClipboard *cb = qApp->clipboard();
    QString text = cb->text();

    if ( ( !text.isNull() ) && (!text.isEmpty() ) )
    {
      text = KURL::encode_string( text );
      unsigned int line, col;
      w->viewCursorIf->cursorPositionReal(&line, &col);
      w->editIf->insertText(line, col, text );
    }
  }
}


/** Kate related slots. */

void QuantaApp::slotUndo ()
{
  Document *w = ViewManager::ref()->activeDocument();
  if(ViewManager::ref()->activeView()->hadLastFocus() == QuantaView::VPLFocus && w)
  {
    /**MessageBox::information(this, i18n("VPL does not support this functionality yet."),
      QString::null, "show undo unavailable");*/
    w->docUndoRedo->undo();
    return;
  }
  if (w)
  {
    bool updateClosing = qConfig.updateClosingTags;
    qConfig.updateClosingTags = false;
//#ifdef BUILD_KAFKAPART
//    write()->docUndoRedo.undo(false);
//#else
    KTextEditor::UndoInterface* undoIf = dynamic_cast<KTextEditor::UndoInterface*>(w->doc());
    if (undoIf)
      undoIf->undo();
//#endif
    qConfig.updateClosingTags = updateClosing;
  }
}

void QuantaApp::slotRedo ()
{
  Document *w = ViewManager::ref()->activeDocument();
  if(ViewManager::ref()->activeView()->hadLastFocus() == QuantaView::VPLFocus)
  {
    /**KMessageBox::information(this, i18n("VPL does not support this functionality yet."),
      QString::null, "show redo unavailable");*/
    w->docUndoRedo->redo();
    return;
  }
  if (w)
  {
    bool updateClosing = qConfig.updateClosingTags;
    qConfig.updateClosingTags = false;
//#ifdef BUILD_KAFKAPART
//    write()->docUndoRedo.redo(false);
//#else
    KTextEditor::UndoInterface* undoIf = dynamic_cast<KTextEditor::UndoInterface*>(w->doc());
    if (undoIf)
      undoIf->redo();
//#endif
    qConfig.updateClosingTags = updateClosing;
  }
}

void QuantaApp::slotSpellcheck ()
{
  Document *w = ViewManager::ref()->activeDocument();
  if (w)
  {
    SpellChecker::ref(this)->spellCheck(w->doc());
  }
}

/** insert special character */
void QuantaApp::slotInsertChar()
{
  Document *w = ViewManager::ref()->activeDocument();
  if (w)
  {
    SpecialCharDialog S( this, "special_char" );
    if (S.exec())
      w->insertTag(S.selection());
  }
}

void QuantaApp::slotCut()
{
  QuantaView* view = ViewManager::ref()->activeView();
  Document *w = ViewManager::ref()->activeDocument();
  if(view && view->hadLastFocus() == QuantaView::VPLFocus)
  {
    /*
    KMessageBox::information(this, i18n("Sorry, VPL does not support this functionality yet."),
      QString::null, "show cut unavailable");
    */
    KafkaDocument::ref()->slotCut();
    return;
  }
  if(w)
  {
    KTextEditor::ClipboardInterface* clip = dynamic_cast<KTextEditor::ClipboardInterface*>(w->view());
    if(clip)
      clip->cut();
  }
}

void QuantaApp::slotCopy()
{
  QuantaView* view = ViewManager::ref()->activeView();
  Document *w = ViewManager::ref()->activeDocument();
  if(view && view->hadLastFocus() == QuantaView::VPLFocus)
  {
    //KMessageBox::information(this, i18n("Sorry, VPL does not support this functionality yet."),
      //QString::null, "show copy unavailable");
    KafkaDocument::ref()->slotCopy();
    return;
  }
  if(w)
  {
    KTextEditor::ClipboardInterface* clip = dynamic_cast<KTextEditor::ClipboardInterface*>(w->view());
    if(clip)
      clip->copy();
  }
}

void QuantaApp::slotPaste()
{
  QuantaView* view = ViewManager::ref()->activeView();
  Document *w = ViewManager::ref()->activeDocument();
  if(view && view->hadLastFocus() == QuantaView::VPLFocus)
  {
    //KMessageBox::information(this, i18n("Sorry, VPL does not support this functionality yet."),
      //QString::null, "show paste unavailable");
    KafkaDocument::ref()->slotPaste();
    return;
  }
  if(w)
  {
    KTextEditor::ClipboardInterface* clip = dynamic_cast<KTextEditor::ClipboardInterface*>(w->view());
    if(clip)
      clip->paste();
  }
}

Node *QuantaApp::showTagDialogAndReturnNode(const QString &tag, const QString &attr)
{
  Node *n = 0L;
  QuantaView *view = ViewManager::ref()->activeView();
  if(view && view->document())
  {
    Document *w = view->document();

    QString selection;
    if(view->hadLastFocus() == QuantaView::VPLFocus)
        selection = KafkaDocument::ref()->getKafkaWidget()->selectedText();

    TagDialog *dlg = new TagDialog(QuantaCommon::tagFromDTD(w->getDTDIdentifier(),tag), selection, attr, ViewManager::ref()->activeView()->baseURL());
    if (dlg->exec())
    {
      n= dlg->buildNode(w);
    }

    delete dlg;
  }
  return n;
}


void QuantaApp::slotShowSourceEditor()
{
   ViewManager::ref()->activeView()->slotSetSourceLayout();
}

void QuantaApp::slotShowVPLAndSourceEditor()
{
   ViewManager::ref()->activeView()->slotSetSourceAndVPLLayout();
}

void QuantaApp::slotShowVPLOnly()
{
   ViewManager::ref()->activeView()->slotSetVPLOnlyLayout();
}

void QuantaApp::initTabWidget(bool closeButtonsOnly)
{
#if KDE_IS_VERSION(3,2,90) || defined(COMPAT_KMDI)
    KTabWidget *tab = tabWidget();
    if (tab)
    {
        if (qConfig.showCloseButtons == "ShowAlways")
        {
            tab->setHoverCloseButton(true);
            tab->setHoverCloseButtonDelayed(false);
        } else
        if (qConfig.showCloseButtons == "ShowDelayed")
        {
            tab->setHoverCloseButton(true);
            tab->setHoverCloseButtonDelayed(true);
        } else
        {
            tab->setHoverCloseButton(false);
        }
        if (!closeButtonsOnly)
        {
            tab->setTabReorderingEnabled(true);
            tab->setTabPosition(QTabWidget::Bottom);
            connect(tab, SIGNAL( contextMenu( QWidget *, const QPoint & ) ), ViewManager::ref(), SLOT(slotTabContextMenu( QWidget *, const QPoint & ) ) );
            setTabWidgetVisibility(KMdi::AlwaysShowTabs);
        }
    }
    if (!closeButtonsOnly)
      setToolviewStyle(qConfig.toolviewTabs);
#endif
}

void QuantaApp::slotFileClosed()
{
}

void QuantaApp::slotCVSCommandExecuted(const QString& command, const QStringList& files)
{
   QString file;
   for (uint i = 0; i < files.count(); i++)
   {
      file = files[i];
      if (Project::ref()->contains(KURL::fromPathOrURL(file)))
      {
         emit eventHappened("after_" + command, file, QString::null);
      }
   }
}

//overridden KMdiMainFrm slots
void QuantaApp::closeActiveView()
{
   ViewManager::ref()->removeActiveView();
}

void QuantaApp::closeAllViews()
{
   ViewManager::ref()->closeAll();
}

void QuantaApp::resetDockLayout()
{
    QStringList groupList = m_config->groupList();
    for (QStringList::Iterator it = groupList.begin(); it != groupList.end(); ++it)
    {
       if ((*it).startsWith("dock_setting_default"))
       {
        m_config->deleteGroup(*it);
       }
    }
    m_config->sync();
    QWidget *mainDockWidget = getMainDockWidget();
    addToolWindow(fTab, KDockWidget::DockLeft, mainDockWidget);
    addToolWindow(ProjectTreeView::ref(), KDockWidget::DockLeft, mainDockWidget);
    addToolWindow(TemplatesTreeView::ref(), KDockWidget::DockLeft, mainDockWidget);
    addToolWindow(StructTreeView::ref(), KDockWidget::DockLeft, mainDockWidget);
    addToolWindow(scriptTab, KDockWidget::DockLeft, mainDockWidget);
    addToolWindow(dTab, KDockWidget::DockRight, mainDockWidget);
    addToolWindow(aTab, KDockWidget::DockRight, mainDockWidget);
    addToolWindow(m_messageOutput, KDockWidget::DockBottom, mainDockWidget);
    addToolWindow(m_problemOutput, KDockWidget::DockBottom, mainDockWidget);
    if (m_previewToolView)
      m_previewToolView =  addToolWindow(m_htmlPart->view(), KDockWidget::DockBottom, mainDockWidget);
    if (m_documentationToolView)
      m_documentationToolView= addToolWindow(m_htmlPartDoc->view(), KDockWidget::DockBottom, mainDockWidget);
    for (QMap<QWidget*,KMdiToolViewAccessor*>::Iterator it = m_pToolViews->begin(); it != m_pToolViews->end(); ++it)
    {
        QWidget *widget = it.key();
        if (dynamic_cast<ServerTreeView*>(widget))
          addToolWindow(widget, KDockWidget::DockRight, mainDockWidget);
        if (dynamic_cast<VariablesListView*>(widget))
          addToolWindow(widget, KDockWidget::DockLeft, mainDockWidget);
        if (dynamic_cast<DebuggerBreakpointView*>(widget))
          addToolWindow(widget, KDockWidget::DockBottom, mainDockWidget);
    }
}

void QuantaApp::switchToToplevelMode()
{
   KMdiMainFrm::switchToToplevelMode();
   resetDockLayout();
   initTabWidget();
}

void QuantaApp::switchToChildframeMode()
{
   KMdiMainFrm::switchToChildframeMode();
   resetDockLayout();
   initTabWidget();
}

void QuantaApp::switchToIDEAlMode()
{
   KMdiMainFrm::switchToIDEAlMode();
   resetDockLayout();
   initTabWidget();
}

void QuantaApp::switchToTabPageMode()
{
   KMdiMainFrm::switchToTabPageMode();
   resetDockLayout();
   initTabWidget();
}

void QuantaApp::slotPreviewBeingClosed()
{
    m_previewVisible = false;
    m_noFramesPreview = false;
    m_previewToolView = 0L; //this automatically deleted, so set to 0L
 }

void QuantaApp::slotDockWidgetHasUndocked(KDockWidget *widget)
{
   if (m_previewToolView && m_previewToolView->wrapperWidget() == widget)
       slotPreviewBeingClosed();
}

void QuantaApp::slotTabDragged(QWidget *widget)
{
   QuantaView *view = dynamic_cast<QuantaView*>(widget);
   if (view && view->document())
   {
     QString url = view->document()->url().url();
     QDragObject *d = new QTextDrag( url, this );
     d->dragCopy();
   }
}

void QuantaApp::setTabToolTip(QWidget *w, const QString &toolTipStr)
{
  if (tabWidget())
    tabWidget()->setTabToolTip(w, toolTipStr);
}

void QuantaApp::createPreviewPart()
{
  m_htmlPart = new WHTMLPart(this, "rightHTML");
  m_htmlPart->view()->resize(0, 0);
  m_htmlPart->view()->setIcon(UserIcon("preview"));
  m_htmlPart->view()->setCaption(i18n("Preview"));
  slotNewPart(m_htmlPart, false);
  connect(m_htmlPart, SIGNAL(previewHasFocus(bool)), this, SLOT(slotPreviewHasFocus(bool)));
  connect(m_htmlPart, SIGNAL(destroyed(QObject *)), this, SLOT(slotHTMLPartDeleted(QObject *)));

}

void QuantaApp::createDocPart()
{
  m_htmlPartDoc = new WHTMLPart(this, "docHTML");
  m_htmlPartDoc->view()->resize(0, 0);
  m_htmlPartDoc->view()->setIcon(SmallIcon("contents"));
  m_htmlPartDoc->view()->setCaption(i18n("Documentation"));
  slotNewPart(m_htmlPartDoc, false);
  connect(m_htmlPartDoc, SIGNAL(destroyed(QObject *)), this, SLOT(slotHTMLPartDeleted(QObject *)));
}

void QuantaApp::slotHTMLPartDeleted(QObject *object)
{
  if (object == m_htmlPart)
  {
     createPreviewPart();
     slotShowPreviewWidget(false);
  } else
     createDocPart();
}


#include "quanta.moc"
