/***************************************************************************
                          quanta.cpp  -  description
                             -------------------
    begin                : ?? ???  9 13:29:57 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com>
                           (C) 2001-2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// include files for QT
#include <qaction.h>
#include <qdir.h>
#include <qprinter.h>
#include <qpainter.h>
#include <qwidgetstack.h>
#include <qtabwidget.h>
#include <qfile.h>
#include <qinputdialog.h>
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

// include files for KDE
#include <kcombobox.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kconfig.h>
#include <khtmlview.h>
#include <kdialogbase.h>
#include <kkeydialog.h>
#include <kstandarddirs.h>
#include <klibloader.h>
#include <klineeditdlg.h>
#include <kdockwidget.h>
#include <kstatusbar.h>
#include <kpopupmenu.h>
#include <kpushbutton.h>
#include <kprocess.h>
#include <ktempfile.h>
#include <kdebug.h>
#include <ktar.h>
#include <kedittoolbar.h>
#include <kaction.h>
#include <kcharsets.h>
#include <kdirwatch.h>
#include <kspell.h>
#include <kdeversion.h>
#include <ktip.h>
#include <kmimetype.h>
#include <kparts/componentfactory.h>

#include <ktexteditor/editinterface.h>
#include <ktexteditor/encodinginterface.h>
#include <ktexteditor/selectioninterface.h>
#include <ktexteditor/viewcursorinterface.h>
#include <ktexteditor/printinterface.h>
#include <ktexteditor/popupmenuinterface.h>
#include <ktexteditor/dynwordwrapinterface.h>
#include <ktexteditor/encodinginterface.h>

#include <kio/netaccess.h>

#include <kate/view.h>

#if KDE_IS_VERSION(3,1,90)
#include <ktabwidget.h>
#include <kinputdialog.h>
#endif

// application specific includes
#include "document.h"
#include "quanta.h"
#include "quantaview.h"
#include "quantadoc.h"
#include "qextfileinfo.h"
#include "resource.h"

#include "project/project.h"

#include "widgets/whtmlpart.h"

#include "dialogs/abbreviation.h"
#include "dialogs/filemasks.h"
#include "dialogs/styleoptionss.h"
#include "dialogs/previewoptions.h"
#include "dialogs/previewfontoptions.h"
#include "dialogs/parseroptions.h"
#include "dialogs/debuggeroptionss.h"
#include "dialogs/dtdselectdialog.h"
#include "dialogs/donationdialog.h"
#include "dialogs/fourbuttonmessagebox.h"
#ifdef BUILD_KAFKAPART
#include "parts/kafka/kafkasyncoptions.h"
#include "parts/kafka/htmldocumentproperties.h"
#endif

#include "treeviews/structtreeview.h"
#include "treeviews/doctreeview.h"
#include "treeviews/templatestreeview.h"
#include "treeviews/tagattributetree.h"
#include "treeviews/projecttreeview.h"

#include "tagdialogs/listdlg.h"
#include "tagdialogs/tagmaildlg.h"

#include "parser/parser.h"
#include "parser/dtdparser.h"

#include "messages/messageoutput.h"

#include "toolbar/actionconfigdialog.h"
#include "toolbar/toolbarxmlgui.h"
#include "toolbar/tagaction.h"
#include "toolbar/toolbartabwidget.h"

#include "dialogs/kategrepdialog.h"

#if KDE_VERSION < KDE_MAKE_VERSION(3,1,90)
#include "dialogs/katefiledialog.h"
#else
#include <kencodingfiledialog.h>
#endif

#include "plugins/quantaplugininterface.h"

// from kfiledialog.cpp - avoid qt warning in STDERR (~/.xsessionerrors)
static void silenceQToolBar(QtMsgType, const char *){}


/////////////////////////////////////////////////////////////////////
// SLOT IMPLEMENTATION
/////////////////////////////////////////////////////////////////////

void QuantaApp::setTitle(const QString& title)
{
  setCaption( "  [  "+title+"  ]  " );
}

void QuantaApp::slotFileNew()
{
  m_doc->openDocument( KURL() );
  if (!setHighlight)
     setHighlight = m_view->write()->kate_doc->hlActionMenu (i18n("Highlight &Mode"), actionCollection(), "set_highlight");
}

void QuantaApp::slotFileOpen()
{
 QString myEncoding = defaultEncoding();
 QString startDir;
 if (view()->writeExists() && !view()->write()->isUntitled())
     startDir = view()->write()->url().url();
 else
      startDir = projectBaseURL().url();

 KURL::List urls;
 QString encoding;
#if KDE_VERSION < KDE_MAKE_VERSION(3,1,90)
 KateFileDialog *dialog = new KateFileDialog (startDir, myEncoding, this, i18n ("Open File"));
 KateFileDialogData data = dialog->exec();
 urls = data.urls;
 encoding = data.encoding;
 delete dialog;
#else
 KEncodingFileDialog::Result data;
 data = KEncodingFileDialog::getOpenURLsAndEncoding(myEncoding, startDir,
        "all/allfiles text/plain", this, i18n("Open File"));
 urls = data.URLs;
 encoding = data.encoding;
#endif

 m_doc->blockSignals(true);
 m_view->writeTab()->blockSignals(true);
 for (KURL::List::Iterator i=urls.begin(); i != urls.end(); ++i)
 {
   if (QuantaCommon::checkMimeGroup(*i, "text") ||
       QuantaCommon::denyBinaryInsert() == KMessageBox::Yes)
     slotFileOpen( *i , encoding);
//   kapp->eventLoop()->processEvents( QEventLoop::ExcludeUserInput | QEventLoop::ExcludeSocketNotifiers);
 }
 m_doc->blockSignals(false);
 m_view->writeTab()->blockSignals(false);
 Document *w = m_view->write();
 setCaption(w->url().prettyURL() );
 slotUpdateStatus(w);
}

void QuantaApp::slotFileOpen( const KURL &url, const QString& encoding )
{
  m_doc->openDocument( url, encoding );
  m_view->write()->view()->setFocus();
}

void QuantaApp::slotFileOpenRecent(const KURL &url)
{
  QString fn = url.prettyURL();
  if (fn.left(5) == "file:")
    fn.remove(0,5);

  if ((url.isLocalFile()) && (!QFileInfo(fn).exists()))
  {
    if (KMessageBox::questionYesNo( this,
        i18n("The file %1 does not exist.\n Do you want to remove from the list?").arg(fn) )
        == KMessageBox::Yes)
    {
      fileRecent->removeURL(url);
    }
  } else
  if (QuantaCommon::checkMimeGroup(url, "text") ||
      QuantaCommon::denyBinaryInsert() == KMessageBox::Yes)
  {
    m_doc->openDocument(url);
  }
  fileRecent->setCurrentItem(-1);
  m_view->write()->view()->setFocus();
}

void QuantaApp::slotFileSave()
{
  if (m_view->writeExists())
  {
    Document *w = m_view->write();
    w->checkDirtyStatus();
    if ( w->isUntitled() )
      slotFileSaveAs();
    else
    {
      m_doc->saveDocument( w->url() );
#ifdef BUILD_KAFKAPART
      view()->write()->docUndoRedo->fileSaved();
#endif
    }
    w->removeBackup(m_config);
    slotUpdateStatus(w);
  }
}

bool QuantaApp::slotFileSaveAs()
{
  bool result = false;
  if (m_view->writeExists())
  {
    Document *w = m_view->write();
    KURL oldURL = w->url();
    w->checkDirtyStatus();
    if (oldURL.isLocalFile())
        fileWatcher->removeFile(oldURL.path());
    QString myEncoding =  dynamic_cast<KTextEditor::EncodingInterface*>(w->doc())->encoding();

    QString saveAsPath;
    bool gotPath = false;

    KURL saveAsUrl;
    if(ptabdock->isVisible())
    {
      saveAsUrl = pTab->currentURL();
      saveAsPath = saveAsUrl.url();
      gotPath = true;
    }
    else if(ftabdock->isVisible())
    {
      saveAsUrl = fTab->currentURL();
      saveAsPath = saveAsUrl.url();
      gotPath = true;
    }

    if(gotPath)
    {
      if(saveAsPath.isEmpty())
        saveAsPath = projectBaseURL().url();
      else
      {
        QFileInfo saveAsPathInfo = QFileInfo(saveAsPath);
        if(saveAsPathInfo.isFile())
          saveAsPath = saveAsUrl.directory();
      }
    }
    else
    {
      saveAsPath = projectBaseURL().url();
    }

    QString saveAsFileName = "";
    if (!(oldURL.fileName()).isEmpty())
      saveAsFileName = "/" + oldURL.fileName();

    KURL saveUrl;
    QString encoding;
#if KDE_VERSION < KDE_MAKE_VERSION(3,1,90)
    KateFileDialog dialog(saveAsPath+saveAsFileName, myEncoding, this, i18n ("Save File"), KateFileDialog::saveDialog);
    KateFileDialogData data = dialog.exec();
    saveUrl = data.url;
    encoding = data.encoding;
#else
    KEncodingFileDialog::Result data;
    data = KEncodingFileDialog::getSaveURLAndEncoding(myEncoding, saveAsPath+saveAsFileName,
            "all/allfiles text/plain", this, i18n("Save File"));
    saveUrl = data.URLs[0];
    encoding = data.encoding;
#endif
    dynamic_cast<KTextEditor::EncodingInterface*>(w->doc())->setEncoding(encoding);
    if (w->checkOverwrite(saveUrl) == KMessageBox::Yes && m_doc->saveDocument(saveUrl))
    {
      oldURL = saveUrl;
      if (  m_project->hasProject() && !m_project->contains(saveUrl)  &&
           KMessageBox::Yes == KMessageBox::questionYesNo(0,i18n("<qt>Do you want to add the<br><b>%1</b><br>file to project?</qt>").arg(saveUrl.prettyURL(0, KURL::StripFileProtocol)))
        )
      {
        m_project->insertFile(saveUrl, true);
      }
#ifdef BUILD_KAFKAPART
      view()->write()->docUndoRedo->fileSaved();
#endif
      slotUpdateStatus(w);
      result = true;
    }
    if (oldURL.isLocalFile())
        fileWatcher->addFile(oldURL.path());
    w->removeBackup(m_config);
  }
  return result;
}

void QuantaApp::saveAsTemplate(bool projectTemplate,bool selectionOnly)
{
//TODO: Saving is limited to local files...

  if (!m_view->writeExists()) return;

  KURL url;
  int query;
  KURL projectTemplateURL;
  Document *w = m_view->write();
  w->checkDirtyStatus();
  QString localTemplateDir = locateLocal("data","quanta/templates/");

  do {
    query = KMessageBox::Yes;

    if (projectTemplate)
    {
      url = KFileDialog::getSaveURL(m_project->templateURL.url(), QString::null, this);
    } else
    {
      url = KFileDialog::getSaveURL(locateLocal("data","quanta/templates/"), QString::null, this);
    }

    if (url.isEmpty()) return;

    if ( m_project->hasProject() )  projectTemplateURL = m_project->templateURL;
    if ( ((projectTemplate) && (projectTemplateURL.isParentOf(url)) ) ||
          ((! projectTemplate) && (KURL(localTemplateDir).isParentOf(url))) )
    {
      query = w->checkOverwrite( url );
    } else
    {
      if (projectTemplate) localTemplateDir = projectTemplateURL.path(1);
      KMessageBox::sorry(this,i18n("You must save the templates in the following directory: \n\n%1")
                                   .arg(localTemplateDir));
      query = KMessageBox::No;
    }
  } while (query != KMessageBox::Yes);

  if( query == KMessageBox::Cancel ) return;

  QString fileName;
  if (selectionOnly)
  {
    QString content;
    content = w->selectionIf->selection();
    fileName = url.directory(false)+url.fileName();
    QFile templateFile(fileName);
    templateFile.open(IO_WriteOnly);
    QTextStream stream(&templateFile);
    stream << content;
    templateFile.flush();
    templateFile.close();
  } else
  {
    m_doc->saveDocument( url );
    fileName = url.path();
  }

  if (projectTemplate) m_project->insertFile(fileName, true);
#ifdef BUILD_KAFKAPART
  view()->write()->docUndoRedo->fileSaved();
#endif
  slotUpdateStatus(w);
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
  m_doc->saveAll();
  QTabWidget *docTab = view()->writeTab();
  Document *w;
  for (int i = docTab->count() -1; i >=0; i--)
  {
    w = dynamic_cast<Document*>(docTab->page(i));
    w->removeBackup(m_config);
  }
  slotUpdateStatus(m_view->write());
}

void QuantaApp::slotFileReload()
{
  if (!m_view->writeExists() || !m_doc->saveModified())
      return;
  Document *w = m_view->write();
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
  if (m_view->writeExists())
  {
  //[MB]  QWidget *activeWidget = rightWidgetStack->visibleWidget();
  #ifdef BUILD_KAFKAPART
    //kafkaPart->unloadDocument();
  #endif
    m_doc->closeDocument();
    WHTMLPart *part = m_htmlPart;
    part->closeURL();
    part->begin(projectBaseURL());
    part->write( "" );
    part->end();

    slotUpdateStatus(m_view->writeTab()->currentPage());
  } else {
    QWidget *kietWidget;
    kietWidget = m_view->writeTab()->currentPage();

    QDict<QuantaPlugin> plugins = m_pluginInterface->plugins();
    QDictIterator<QuantaPlugin> it(plugins);

    for(;it.current() != 0;++it) {
      QuantaPlugin *curPlugin = it.current();
      QWidget *pluginWidget = curPlugin->widget();
      if (pluginWidget && pluginWidget == kietWidget) {
        curPlugin->unload();
      }
   }
 }

}

void QuantaApp::slotFileCloseAll()
{
#ifdef BUILD_KAFKAPART
  //kafkaPart->unloadDocument();
#endif
  m_doc->closeAll();

  WHTMLPart *part = m_htmlPart;
  part->closeURL();
  part->begin(projectBaseURL());
  part->write( "" );
  part->end();

  slotNewStatus();
}

void QuantaApp::slotFileNext()
{
   m_doc->nextDocument();
}

void QuantaApp::slotFilePrev()
{
   m_doc->prevDocument();
}

void QuantaApp::slotFilePrint()
{
 if (m_view->writeExists())
     dynamic_cast<KTextEditor::PrintInterface*>(m_view->write()->doc())->printDialog();
}

void QuantaApp::slotFileQuit()
{
  close();
}


void QuantaApp::slotEditFindInFiles()
{
  if (!grepDialog)
  {
    QString startDir = QDir::homeDirPath();
    KURL pBase = projectBaseURL();
    if (pBase.isLocalFile()) startDir = pBase.path(1);
    grepDialog = new GrepDialog( startDir, this, "grep_dialog" );
    connect( grepDialog, SIGNAL( itemSelected   (const QString& , int)),
             this,       SLOT  ( gotoFileAndLine(const QString& , int)));
  }
  grepDialog->show();
  grepDialog->raise();
}


void QuantaApp::slotViewToolBar()
{
  QToolBar *mbar = toolBar("mainToolBar");
  QToolBar *ebar = toolBar("mainEditToolBar");
  QToolBar *nbar = toolBar("mainNaviToolBar");

  if(mbar->isVisible()) {
    mbar->hide();
    ebar->hide();
    nbar->hide();
  }
  else {
    nbar->show();
    ebar->show();
    mbar->show();
  }
}

void QuantaApp::slotViewStatusBar()
{
  if (showStatusbarAction->isChecked())
    statusBar()->show();
  else
    statusBar()->hide();
}

void QuantaApp::slotHelpTip()
{
   KTipDialog::showTip(this, QString::null, true);
}

void QuantaApp::slotStatusMsg(const QString &msg)
{
  statusbarTimer->stop();
  statusBar()->changeItem(" " + msg, IDS_STATUS);
  statusBar()->repaint();
  kapp->processEvents();
  statusbarTimer->start(10000, true);
}

/** repaint preview */
void QuantaApp::slotRepaintPreview()
{
  WHTMLPart *part = m_htmlPart;
  QWidgetStack *s = widgetStackOfHtmlPart();

  if ( !s ) return;
  if ( !part ) return;
  if ( !s->id( s ->visibleWidget()) ) return;
  if (!m_view->writeExists()) return;

  previewCopyMade = false;

  KHTMLView *html = part->view();
  int xOffset = html->contentsX(), yOffset = html->contentsY();

  part->closeURL();

  KParts::URLArgs  args(true, browserExtension()->xOffset(), browserExtension()->yOffset());
  browserExtension()->setURLArgs( args );

  KURL url;
  Document *w = m_view->write();
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
        part->begin( projectBaseURL(), xOffset, yOffset );
      else
        part->begin( w->url(), xOffset, yOffset );
      part->write(noFramesText);
    }
  }

  if (!m_noFramesPreview)
  {
    if (!w->isUntitled())
    {
  //if it's  not untitled, than it was loaded from somewhere. In this case show it from that place
      url = w->url();

      if ( m_doc->isModified() ) //m_doc->saveDocument( m_doc->url() );
          w->saveIt();

      url = m_project->urlWithPrefix(url);

      part->begin(url, xOffset, yOffset);
      part->openURL(url);
    } else  //the document is Untitled, preview the text from it
    {
      QString text = w->editIf->text();
      if ( text.isEmpty() )
      {
        text = i18n( "<center><h3>The current document is empty...</h3></center>" );
      } else
      if (m_noFramesPreview)
      {
      }
      part->begin( projectBaseURL(), xOffset, yOffset );
      part->write( text );
    }
   }
 part->end();
 part->show();
}

void QuantaApp::slotOpenFileInPreview(const KURL& a_url)
{
  if ( qConfig.previewPosition == "Disabled" )
     return;
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
  if ( qConfig.previewPosition == "Disabled" )
     return;

  slotShowPreviewWidget(true);
  WHTMLPart *part = m_htmlPart;
  QString text = "<html>\n<body>\n<div align=\"center\">\n<img src=\"";
  text += url.url(); //TODO
  text += "\">\n</div>\n</body>\n</html>\n";
  part->closeURL();
  part->begin( projectBaseURL() );
  part->write( text );
  part->end();

  part->show();
}


/** insert <img> tag for images or <a> for other */
void QuantaApp::slotInsertTag(const KURL& url, DirInfo dirInfo)
{
  if (m_view->writeExists())
  {
    KURL baseURL ;
    if  ( m_view->write()->isUntitled() )
    {
      baseURL = projectBaseURL();
    } else
    {
      baseURL = m_view->write()->url();
      baseURL.setFileName("");
    }
    KURL relURL = QExtFileInfo::toRelative( url, baseURL);
    QString urlStr = relURL.url();
    if (relURL.protocol() == baseURL.protocol())
        urlStr = relURL.path();
    Document *w = m_view->write();
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
          width.setNum( img.width () );
          height.setNum( img.height() );
          w->insertTag("<img src=\""+urlStr+"\" width=\""+width+"\" height=\""+height+"\" border=\"0\">");
          isImage = true;
        }
      }
      if (!isImage)
      {
        w->insertTag( "<a href=\""+urlStr+"\">","</a>");
      }
    }
    w->view()->setFocus();
  }
}

////////////////////////
// status slots
///////////////////////
/** slot for new modify flag */
void QuantaApp::slotNewStatus()
{
  if (m_view->writeExists())
  {
    Document *w = m_view->write();
    setTitle( w->url().prettyURL() );

    if (w->doc()->isReadWrite())
        statusBar()->changeItem(w->kate_view->isOverwriteMode() ? i18n(" OVR ") : i18n(" INS "),IDS_INS_OVR);
    else
        statusBar()->changeItem(i18n(" R/O "),IDS_INS_OVR);
    statusBar()->changeItem(w->isModified() ? " * " : "",IDS_MODIFIED);

    saveAction   ->setEnabled(m_doc->isModified());
    saveAllAction->setEnabled(m_doc->isModifiedAll());
    if (m_project->isModified())
        m_project->slotSaveProject();

    bool projectExists = m_project->hasProject();
    closeprjAction     ->setEnabled(projectExists);
    openPrjViewAction  ->setEnabled(projectExists);
    savePrjViewAction  ->setEnabled(projectExists);
    saveAsPrjViewAction->setEnabled(projectExists);
    deletePrjViewAction->setEnabled(projectExists);

    insertFileAction   ->setEnabled(projectExists);
    insertDirAction    ->setEnabled(projectExists);
    rescanPrjDirAction ->setEnabled(projectExists);
    uploadProjectAction->setEnabled(projectExists);
    projectOptionAction->setEnabled(projectExists);
    saveAsProjectTemplateAction->setEnabled(projectExists);
    saveSelectionAsProjectTemplateAction->setEnabled(projectExists);

    actionCollection()->action("toolbars_load_project")->setEnabled(projectExists);
    actionCollection()->action("toolbars_save_project")->setEnabled(projectExists);

    viewBorder->setChecked(w->kate_view->iconBorder());
    viewLineNumbers->setChecked(w->kate_view->lineNumbersOn());

     //viewFoldingMarkers->setChecked(w->kate_view->lineNumbersOn());
    viewDynamicWordWrap->setChecked(dynamic_cast<KTextEditor::DynWordWrapInterface*>(w->view())->dynWordWrap());
    if (setHighlight) setHighlight->updateMenu (w->kate_doc);

    QTabWidget *wTab = m_view->writeTab();
    w = static_cast<Document*>(wTab->currentPage());
    // try to set the icon from mimetype
    QIconSet mimeIcon (KMimeType::pixmapForURL(w->url(), 0, KIcon::Small));
    if (mimeIcon.isNull())
      mimeIcon = QIconSet(SmallIcon("document"));
#if KDE_IS_VERSION(3,1,90)
    if (qConfig.showCloseButtons)
    {
      if (w->isModified())
      {
        wTab->changeTab( w, SmallIcon("fileclose"), QExtFileInfo::shortName(w->url().path()) + " " + i18n("[modified]"));
      } else
      {
        wTab->changeTab( w, SmallIcon("fileclose"), QExtFileInfo::shortName(w->url().path()));
      }
    } else
#endif
    {
      if ( w->isModified() )
        wTab->changeTab( w, UserIcon("save_small"), wTab->tabLabel(w));
      else
        wTab->changeTab( w, mimeIcon,  wTab->tabLabel(w));
    }
//This is a really dirty fix for the QTabWidget problem. After the changeTab call,
//it will reset itself and you will see the first tabs, even if the actual page is on
//a tab eg. at the end, and it won't be visible now. This is really confusing.
//I thought it is fixed in QT 3.x, but it is not. :-(
    int pageId = wTab->currentPageIndex();
    bool block=wTab->signalsBlocked();
    wTab->blockSignals(true);
    wTab->setCurrentPage(pageId-1);
    wTab->setCurrentPage(pageId);
    wTab->blockSignals(block);
 }
}

void QuantaApp::slotClosePage(QWidget *w)
{
  QTabWidget *writeTab = m_view->writeTab();
  QString tabTitle = writeTab->tabLabel(w);
  QuantaPlugin *plugin = m_pluginInterface->plugin(tabTitle);
  if (plugin)
  {
    plugin->unload();
    plugin->m_action->setChecked(false);
  } else
  {
    QWidget *oldPage = writeTab->currentPage();
    if (oldPage != w)
        writeTab->showPage(w);
    m_doc->closeDocument();
    if (oldPage != w)
        writeTab->showPage(oldPage);
    reparse(true);
  }
  if (!writeTab->currentPage())
  {
    m_doc->openDocument(KURL());
  }
}

void QuantaApp::slotUpdateStatus(QWidget* w)
{
//remove the GUI of the plugin, if the last visible tab was a plugin
  QString tabTitle =m_view->writeTab()->tabLabel(m_view->oldTab);
  QuantaPlugin *plugin = m_pluginInterface->plugin(tabTitle);
  if (plugin)
  {
    plugin->showGui(false);
    if (showStatusbarAction->isChecked())
    {
      showStatusbarAction->setChecked(true);
      statusBar()->changeItem("", IDS_STATUS);
      statusBar()->show();
    }
    if (qConfig.enableDTDToolbar)
        m_view->toolbarTab()->show();
  }

  Document *newWrite = dynamic_cast<Document *>(w);
  if (!newWrite)
  {
//add the GUI for the currently visible plugin
    tabTitle = m_view->writeTab()->tabLabel(w);
    plugin = m_pluginInterface->plugin(tabTitle);
    if (plugin)
       plugin->showGui(true);
    m_view->oldTab = w;
    m_view->toolbarTab()->hide();
    return;
  }
  dynamic_cast<KTextEditor::PopupMenuInterface*>(newWrite->view())->installPopup((QPopupMenu *)factory()->container("popup_editor", quantaApp));
  newWrite->checkDirtyStatus();
  if (newWrite != m_view->oldWrite && sTab)
    sTab->useOpenLevelSetting = true;
  reparse(true);
  slotNewStatus();
  slotNewLineColumn();

  loadToolbarForDTD(newWrite->getDTDIdentifier());

  Document *currentWrite = m_view->write();
#ifdef BUILD_KAFKAPART
   m_view->resize(m_view->writeTab()->size().width()-5, m_view->writeTab()->size().height()-35);
#else
   currentWrite->view()->resize(m_view->writeTab()->size().width()-5, m_view->writeTab()->size().height()-35);
#endif

  m_view->oldWrite = currentWrite;
  currentWrite->kate_view->setIconBorder(qConfig.iconBar);
  currentWrite->kate_view->setLineNumbersOn(qConfig.lineNumbers);
  viewBorder->setChecked(qConfig.iconBar);
  viewLineNumbers->setChecked(qConfig.lineNumbers);

  dynamic_cast<KTextEditor::DynWordWrapInterface*>(currentWrite->view())->setDynWordWrap(qConfig.dynamicWordWrap);
  viewDynamicWordWrap->setChecked(dynamic_cast<KTextEditor::DynWordWrapInterface*>(currentWrite->view())->dynWordWrap());

  QWidgetStack *s = widgetStackOfHtmlPart();
  if (s->id(s->visibleWidget()) == 1)
  {
    slotRepaintPreview();
  }
  #ifdef BUILD_KAFKAPART
  m_view->updateViews();
  #endif
  m_view->oldTab = w;
  slotNewLineColumn();

  emit reloadTreeviews();
}

void QuantaApp::slotOptionsConfigureKeys()
{
  KKeyDialog::configure( actionCollection(), this, true);
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

void QuantaApp::slotConfigureToolbars(const QString& defaultToolbar)
{
 currentPageIndex = m_view->toolbarTab()->currentPageIndex();

 QDomNodeList nodeList;
 ToolbarEntry *p_toolbar = 0L;
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
 saveMainWindowSettings(KGlobal::config(), autoSaveGroup());
 KEditToolbar *dlg;
#if KDE_IS_VERSION(3,1,90)
  dlg = new KEditToolbar(defaultToolbar, factory(), this);
#else
  dlg = new KEditToolbar(factory(), this);
#endif

 //remove the manually added menus BEFORE the dlg shows up
 menuBar()->removeItem(menuBar()->idAt(TAGS_MENU_PLACE));
 menuBar()->removeItem(menuBar()->idAt(PLUGINS_MENU_PLACE));

 connect( dlg, SIGNAL( newToolbarConfig() ), SLOT( slotNewToolbarConfig() ) );
 dlg->exec();
 delete dlg;

 QPopupMenu *menu = 0L;
 QString toolbarName;
 ToolbarTabWidget *tb = m_view->toolbarTab();
 for (int i = 0; i < tb->count(); i++)
 {
   toolbarName = tb->label(i);
   p_toolbar = quantaApp->toolbarList[toolbarName.lower()];
   if (p_toolbar)
   {
    delete p_toolbar->menu;
    menu = new QPopupMenu(m_tagsMenu);
    nodeList = p_toolbar->guiClient->domDocument().elementsByTagName("Action");
    for (uint i = 0; i < nodeList.count(); i++)
    {
      KAction *action = actionCollection()->action(nodeList.item(i).toElement().attribute("name"));
      if (action)
          action->plug(menu);
    }

    m_tagsMenu->insertItem(toolbarName,menu);
    p_toolbar->menu = menu;
   }
 }

 //add the menus
 menuBar()->insertItem(i18n("Plu&gins"), m_pluginMenu, -1, PLUGINS_MENU_PLACE);
 menuBar()->insertItem(i18n("&Tags"),m_tagsMenu,-1, TAGS_MENU_PLACE);
 tb->setCurrentPage(currentPageIndex);
}

void QuantaApp::slotOptionsConfigureToolbars()
{
  slotConfigureToolbars();
}

void QuantaApp::slotNewToolbarConfig()
{
 applyMainWindowSettings(KGlobal::config(), autoSaveGroup());
 m_view->toolbarTab()->setCurrentPage(currentPageIndex);
}

void QuantaApp::slotOptionsConfigureActions()
{
  ActionConfigDialog dlg( this, "actions_config_dlg", true);
  dlg.exec();
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

  // Environment options
  //TODO FileMasks name is not good anymore
  page=kd->addVBoxPage(i18n("Environment"), QString::null, BarIcon("files", KIcon::SizeMedium ) );
  FileMasks *fileMasks = new FileMasks((QWidget *)page);

  fileMasks->lineMarkup->setText( qConfig.markupMimeTypes );
  fileMasks->lineScript->setText( qConfig.scriptMimeTypes );
  fileMasks->lineImage->setText( qConfig.imageMimeTypes );
  fileMasks->lineText->setText( qConfig.textMimeTypes );
  fileMasks->showDTDSelectDialog->setChecked(qConfig.showDTDSelectDialog);
  m_config->setGroup("Notification Messages");
  fileMasks->warnBinaryOpening->setChecked(m_config->readEntry("Open Everything") != "Yes");
  m_config->setGroup("General Options");
  fileMasks->showSplash->setChecked(m_config->readBoolEntry("Show Splash", true));
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
#if KDE_IS_VERSION(3,1,90)
  fileMasks->showCloseButton->setChecked(qConfig.showCloseButtons);
#else
  fileMasks->showCloseButton->setShown(false);
#endif


  // Preview options
  page=kd->addVBoxPage(i18n("Layout"), QString::null, BarIcon("kview", KIcon::SizeMedium ) );
  PreviewOptions *previewOptions = new PreviewOptions( (QWidget *)page );

  previewOptions->setPosition(qConfig.previewPosition);
  previewOptions->setWindowLayout(qConfig.windowLayout);

#ifdef BUILD_KAFKAPART
  //kafka options
  page = kd->addVBoxPage(i18n("VPL View"), QString::null, BarIcon("files", KIcon::SizeMedium));
  KafkaSyncOptions *kafkaOptions = new KafkaSyncOptions( m_config, (QWidget *)page );
#endif

  page=kd->addVBoxPage(i18n("Parser"), QString::null, BarIcon("kcmsystem", KIcon::SizeMedium ) );
  ParserOptions *parserOptions = new ParserOptions( m_config, (QWidget *)page );
  QStringList lst;
  QDictIterator<DTDStruct> it(*dtds);
  for( ; it.current(); ++it )
  {
    if (it.current()->toplevel)
    {
      lst << it.current()->nickName;
    }
  }
  lst.sort();
  uint pos = 0;
  for (uint i = 0; i < lst.count(); i++)
  {
    fileMasks->defaultDTDCombo->insertItem(lst[i]);
    if (lst[i] == QuantaCommon::getDTDNickNameFromName(qConfig.defaultDocType.lower()))
       pos = i;
  }
  fileMasks->defaultDTDCombo->setCurrentItem(pos);

  parserOptions->refreshFrequency->setValue(qConfig.refreshFrequency);
  parserOptions->instantUpdate->setChecked(qConfig.instantUpdate);
  parserOptions->showEmptyNodes->setChecked(qConfig.showEmptyNodes);
  parserOptions->showClosingTags->setChecked(qConfig.showClosingTags);
  parserOptions->spinExpand->setValue(qConfig.expandLevel);

  page = kd->addVBoxPage(i18n("Abbreviations"), QString::null, BarIcon("source", KIcon::SizeMedium));
  Abbreviation *abbreviationOptions = new Abbreviation((QWidget*)(page));
  lst.clear();
  it.toFirst();
  for( ; it.current(); ++it )
  {
      lst << it.current()->nickName;
  }
  lst.sort();
  uint abbrevDTDPos = 0;
  QString defaultDTDNickName;
  if (m_view->writeExists())
      defaultDTDNickName = m_view->write()->defaultDTD()->nickName;
  for (uint i = 0; i < lst.count(); i++)
  {
    if (lst[i] == defaultDTDNickName)
       abbrevDTDPos = i;
  }
  abbreviationOptions->dtdCombo->insertStringList(lst);
  abbreviationOptions->dtdCombo->setCurrentItem(abbrevDTDPos);
  if (!defaultDTDNickName.isEmpty())
    abbreviationOptions->slotDTDChanged(defaultDTDNickName);

  page=kd->addVBoxPage(i18n("PHP Debug"), QString::null, BarIcon("gear", KIcon::SizeMedium ) );
  DebuggerOptionsS *debuggerOptions = new DebuggerOptionsS( (QWidget *)(page) );

  if (debuggerStyle=="PHP3") debuggerOptions->radioPhp3->setChecked(true);
  if (debuggerStyle=="None") debuggerOptions->checkDebugger->setChecked(false);

//Spelling options
  page=kd->addVBoxPage(i18n("Spelling"), QString::null, BarIcon("spellcheck", KIcon::SizeMedium ) );
  KSpellConfig *spellOptions = new KSpellConfig( (QWidget *)page, 0L, qConfig.spellConfig, false );

  kd->adjustSize();
  if ( kd->exec() )
  {
    qConfig.tagCase = styleOptionsS->tagCase->currentItem();
    qConfig.attrCase = styleOptionsS->attributeCase->currentItem();
    qConfig.attrValueQuotation = styleOptionsS->attributeQuotation->currentItem() == 0 ? '"': '\'';
    qConfig.closeTags = styleOptionsS->tagAutoClose->isChecked();
    qConfig.closeOptionalTags = styleOptionsS->optionalTagAutoClose->isChecked();
    qConfig.useAutoCompletion = styleOptionsS->useAutoCompletion->isChecked();
    qConfig.updateClosingTags = styleOptionsS->tagUpdateClosing->isChecked();

    qConfig.markupMimeTypes = fileMasks->lineMarkup->text();
    qConfig.scriptMimeTypes = fileMasks->lineScript->text();
    qConfig.imageMimeTypes = fileMasks->lineImage->text();
    qConfig.textMimeTypes = fileMasks->lineText->text();
    qConfig.showDTDSelectDialog = fileMasks->showDTDSelectDialog->isChecked();
    qConfig.autosaveInterval =  QString::number(fileMasks->sbAutoSave->value(), 10);
    autosaveTimer->start(60000 * qConfig.autosaveInterval.toInt(), false);
    m_config->setGroup("Notification Messages");
    m_config->writeEntry("Open Everything", fileMasks->warnBinaryOpening->isChecked() ? "" : "Yes");
    m_config->setGroup("General Options");
    m_config->writeEntry("Show Splash", fileMasks->showSplash->isChecked());

    qConfig.defaultEncoding = fileMasks->encodingCombo->currentText();
#if KDE_IS_VERSION(3,1,90)
    qConfig.showCloseButtons = fileMasks->showCloseButton->isChecked();
    KTabWidget *tab = static_cast<KTabWidget*>(m_view->writeTab());
    if (qConfig.showCloseButtons)
    {
      tab->setHoverCloseButton(true);
      tab->setHoverCloseButtonDelayed(false);
    } else
    {
      tab->setHoverCloseButton(false);
    }
#endif

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
#ifdef BUILD_KAFKAPART
    kafkaOptions->updateConfig();
    quantaApp->view()->readConfig(m_config);
    /**qConfig.quantaRefreshOnFocus = kafkaOptions->sourceFocusRefresh->isChecked();
    qConfig.quantaRefreshDelay = kafkaOptions->sourceDelay->value();
    qConfig.kafkaRefreshOnFocus = kafkaOptions->kafkaFocusRefresh->isChecked();
    qConfig.kafkaRefreshDelay = kafkaOptions->kafkaDelay->value();
    view()->reloadUpdateTimers();*/
    /**(static_cast<HTMLEnhancer *>(quantaApp->view()->getKafkaInterface()->mainEnhancer))->
      showIconsForScripts(kafkaOptions->showScriptsIcon->isChecked());*/

#endif
    qConfig.defaultDocType = QuantaCommon::getDTDNameFromNickName(fileMasks->defaultDTDCombo->currentText());

    if (!debuggerOptions->checkDebugger->isChecked()) {
      if (debuggerStyle=="PHP3") enablePhp3Debug(false);
      if (debuggerStyle=="PHP4") enablePhp4Debug(false);
      debuggerStyle="None";
    } else if (debuggerOptions->radioPhp3->isChecked()) {
      if (   debuggerStyle=="PHP4") enablePhp4Debug(false);
      if (!(debuggerStyle=="PHP3")) enablePhp3Debug(true);
      debuggerStyle="PHP3";
    } else {
      if (   debuggerStyle=="PHP3") enablePhp3Debug(false);
      if (!(debuggerStyle=="PHP4")) enablePhp4Debug(true);
      debuggerStyle="PHP4";
    }

    abbreviationOptions->saveTemplates();

    qConfig.spellConfig->setDictionary(spellOptions->dictionary());
    qConfig.spellConfig->setNoRootAffix(spellOptions->noRootAffix());
    qConfig.spellConfig->setRunTogether(spellOptions->runTogether());
    qConfig.spellConfig->setDictFromList(spellOptions->dictFromList());
    qConfig.spellConfig->setEncoding(spellOptions->encoding());
    qConfig.spellConfig->setIgnoreList(spellOptions->ignoreList());
    qConfig.spellConfig->setReplaceAllList(spellOptions->replaceAllList());
    qConfig.spellConfig->setClient(spellOptions->client());

    QWidgetStack *s;
    if ( m_htmlPart )
    {
      s = widgetStackOfHtmlPart();
      s ->raiseWidget( 0 );
     }
//    checkCommand( ID_VIEW_PREVIEW, false );

    qConfig.previewPosition = previewOptions->position();

    QString layout = previewOptions->layout();
    layoutDockWidgets(layout);
    qConfig.windowLayout = layout;

    m_htmlPart->closeURL();
    m_htmlPart->begin( projectBaseURL());
    m_htmlPart->write( "" );
    m_htmlPart->end();

    slotRepaintPreview();
    reparse(true);

  }

  m_config->sync();

  saveOptions();

  delete kd;
}

void QuantaApp::slotShowPreviewWidget(bool show)
{
  QWidgetStack *s = widgetStackOfHtmlPart();
  if (!s)
      return;
  if (show)
  {
    int id = s->id(s->visibleWidget());
    if (id != 1)
    {
      previousWidgetList.push_back(id);
      s->raiseWidget(1);
    }
    m_previewVisible = true;
  } else
  {
    int id = 0;
    if (!previousWidgetList.empty())
    {
      id = previousWidgetList.last();
      previousWidgetList.pop_back();
    }
    s->raiseWidget(id);
    m_previewVisible = false;
    m_noFramesPreview = false;
    m_view->write()->view()->setFocus();
  }
  KToggleAction *ta = (KToggleAction *) quantaApp->actionCollection()->action( "show_quanta_editor" );
  if (ta)
  {
    ta->setChecked(!show);
  }
}

void QuantaApp::slotShowPreview()
{
  QWidgetStack *s = widgetStackOfHtmlPart();
  if (!s)
  {
    m_previewVisible = false;
    return;
  }
  int id = s->id(s->visibleWidget());
  if (id == 2)
  {
    m_previewVisible = false;
    return;
  }
  if (!m_view->writeExists())
  {
    m_previewVisible = false;
    return;
  }
  Document *w = m_view->write();


  if (!m_previewVisible)
  {
    if ( qConfig.previewPosition == "Bottom" )
    {
      //TODO: ???
      slotShowBottDock(true);
    }
    slotShowPreviewWidget(true);
    slotRepaintPreview();
    m_previewVisible = true;
  } else
  {
    slotShowPreviewWidget(false);
    if (m_doc->isModified())
    {
      KURL origUrl = w->url();
      if (origUrl.isLocalFile())
          fileWatcher->removeFile(origUrl.path());
      KURL tempUrl;
      tempUrl.setPath(w->tempFileName());
//TODO: Replace with KIO::NetAccess::file_copy, when KDE 3.1 support
//is dropped
      QExtFileInfo::copy(tempUrl, origUrl, -1, true, false, this);
      if (origUrl.isLocalFile())
          fileWatcher->addFile(origUrl.path());
    }
    m_previewVisible = false;
  }
  m_noFramesPreview = false;
}

void QuantaApp::slotShowNoFramesPreview()
{
  m_noFramesPreview = true;
  slotShowPreview();
}


void QuantaApp::slotShowProjectTree()
{
  if ( !ptabdock->isVisible() )
      ptabdock->changeHideShowState();
}

void QuantaApp::newCursorPosition(QString file, int lineNumber, int columnNumber)
{
  idleTimer->start(250, true);
  QString linenumber;
  linenumber = i18n("Line: %1 Col: %2").arg(lineNumber).arg(columnNumber);
  statusBar()->changeItem(linenumber, IDS_STATUS_CLM);
  statusBar()->changeItem(i18n(" R/O "),IDS_INS_OVR);
  statusBar()->changeItem("",IDS_MODIFIED);
}

void QuantaApp::newDebuggerPosition(QString file, int lineNumber)
{
    newCursorPosition(file, lineNumber, 0);
}

void  QuantaApp::openFile(QString file, int lineNumber, int columnNumber)
{
  gotoFileAndLine(file, lineNumber);
  setCursorPosition(lineNumber, columnNumber);
  slotNewStatus();
}

void QuantaApp::slotNewLineColumn()
{
  idleTimer->start(250, true);

  QString linenumber;
  oldCursorLine = cursorLine;
  oldCursorCol = cursorCol;
  m_view->write()->viewCursorIf->cursorPositionReal(&cursorLine, &cursorCol);
  linenumber = i18n("Line: %1 Col: %2").arg(cursorLine+1).arg(cursorCol+1);
  statusBar()->changeItem(linenumber, IDS_STATUS_CLM);
}

void QuantaApp::slotIdleTimerExpired()
{
  if (m_view->writeExists())
  {
    m_view->write()->viewCursorIf->cursorPositionReal(&cursorLine, &cursorCol);
    Node *node = parser->nodeAt(cursorLine, cursorCol);
    if (node)
    {
      sTab->showTagAtPos(node);
    }
#ifdef BUILD_KAFKAPART
    if(view()->hadLastFocus() == QuantaView::quantaFocus)
#endif
    aTab->setCurrentNode(node);
  }
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
  //temp
//  if (!parser->activated()) return;
  if (m_view->writeExists())
  {
    Document *w = m_view->write();
    if (force)
    {
      baseNode = parser->parse(w);
    }

    if ( stabdock->isVisible() && (w->hasChanged() || force))
    {
      slotReloadStructTreeView();
    }

    if (force)
    {
      uint line, col;
      w->viewCursorIf->cursorPositionReal(&line, &col);
      Node *node = parser->nodeAt(line, col);
      if (stabdock->isVisible() && node)
         sTab->showTagAtPos(node);
    }
  }

  return;
}

void QuantaApp::setCursorPosition( int row, int col )
{
  if (m_view->writeExists())
  {
    Document *w = m_view->write();

    int numLines = w->editIf->numLines();

    if ( row < numLines )
      w->viewCursorIf->setCursorPositionReal(row, col);
    else
      w->viewCursorIf->setCursorPositionReal(numLines - 1, col);
  }
}

void QuantaApp::gotoFileAndLine(const QString& filename, int line )
{
  if ( !filename.isEmpty() ) m_doc->openDocument( filename );

  if (m_view->writeExists())
  {
    Document *w = m_view->write();
    int numLines = w->editIf->numLines();
    if ( numLines > line && line >= 0 )
    {
      w->viewCursorIf->setCursorPositionReal(line, 0);
    }
  }
}


void QuantaApp::slotDockStatusChanged()
{
  qConfig.windowLayout = "Custom";
  showMessagesAction->setChecked(bottdock->isVisible());
  if (!stabdock->isVisible()) {
    m_problemOutput->clear();
    m_problemOutput->showMessage(i18n("Switch to the Structure Tree in order to turn on the error recognition.\n"));
  }
}

void QuantaApp::slotDockChanged()
{
  static bool docTabOpened = false;
  QWidgetStack *s = widgetStackOfHtmlPart();
  if ( dtabdock->isVisible() )
  {
    static bool first = true;
    previousWidgetList.push_back(s->id(s->visibleWidget()));
    s->raiseWidget(2);
    if ( first )
    {
      openDoc( locate("appdata","doc/intro.html") );
      first = false;
    }
    docTabOpened = true;
  }
  else {
    if ( docTabOpened )
    {
      int id = 0;
      previousWidgetList.remove(2);
      if (s->id(s->visibleWidget()) == 2)
      {
        if (!previousWidgetList.empty())
        {
          id = previousWidgetList.last();
          previousWidgetList.pop_back();
        }
      } else
      {
        id = s->id(s->visibleWidget());
      }
      s->raiseWidget(id);
      docTabOpened = false;
    }
    if ( !exitingFlag )
    {
      if (m_view->writeExists()) m_view->write()->view()->setFocus();
    }
  }
  showMessagesAction->setChecked(bottdock->isVisible());
  if (!stabdock->isVisible()) {
    m_problemOutput->clear();
    m_problemOutput->showMessage(i18n("Switch to the Structure Tree in order to turn on the error recognition.\n"));
  }
}

void QuantaApp::selectArea(int line1, int col1, int line2, int col2)
{
  if (m_view->writeExists())
  {
    Document *w = m_view->write();
    int numLines = w->editIf->numLines();

    if ( line1 > numLines-1 )
      line1 = numLines-1;

    if ( line2 > numLines-1 )
      line2 = numLines-1;

    w->viewCursorIf->setCursorPositionReal(line2, col2);
    w->selectionIf->setSelection(line1, col1, line2, col2);
  }
}

void QuantaApp::openDoc( QString url )
{
   static QString oldUrl("");

   if ( url == oldUrl ) return;

   m_htmlPartDoc->closeURL();
   m_htmlPartDoc->openURL(url);
   m_htmlPartDoc->show();
   m_htmlPartDoc->addToHistory(url);

   oldUrl = url;
}

void QuantaApp::slotContextHelp()
{
  int id_w = rightWidgetStack->id( rightWidgetStack->visibleWidget());

  QWidgetStack *s = widgetStackOfHtmlPart();
  if (  id_w == 1 || id_w == 2 )
  {
    if ( !m_oldTreeViewWidget->isVisible() )
        m_oldTreeViewWidget->changeHideShowState();
    int id = 0;
    if (!previousWidgetList.empty())
    {
      id = previousWidgetList.last();
      previousWidgetList.pop_back();
    }
    s->raiseWidget(id);
  }
  else
  {
    QString currentW =m_view->write()->kate_view->currentWord();
    QString *url = dTab->contextHelp( currentW);

    if ( url )
    {
      previousWidgetList.push_back(s->id(s->visibleWidget()));
      if (ftabdock->isVisible()) m_oldTreeViewWidget = ftabdock;
      if (ptabdock->isVisible()) m_oldTreeViewWidget = ptabdock;
      if (ttabdock->isVisible()) m_oldTreeViewWidget = ttabdock;
      if (scripttabdock->isVisible()) m_oldTreeViewWidget = scripttabdock;
      if (stabdock->isVisible()) m_oldTreeViewWidget = stabdock;
      if (dtabdock->isVisible()) m_oldTreeViewWidget = dtabdock;
      if (!dtabdock->isVisible()) dtabdock->changeHideShowState();

      s->raiseWidget(2);
      m_htmlPartDoc->view()->setFocus();

      openDoc(*url);
    }
  }
}

void QuantaApp::slotShowFTabDock() { ftabdock->changeHideShowState();}
void QuantaApp::slotShowPTabDock() { ptabdock->changeHideShowState();}
void QuantaApp::slotShowTTabDock() { ttabdock->changeHideShowState();}
void QuantaApp::slotShowScriptTabDock() { scripttabdock->changeHideShowState(); }
void QuantaApp::slotShowSTabDock() { stabdock->changeHideShowState();}
void QuantaApp::slotShowATabDock() { atabdock->changeHideShowState();}
void QuantaApp::slotShowDTabDock() { dtabdock->changeHideShowState();}
void QuantaApp::slotShowProblemsDock()
{
  problemsdock->changeHideShowState();
  if (!stabdock->isVisible()) {
    m_problemOutput->clear();
    m_problemOutput->showMessage(i18n("Switch to the Structure Tree in order to turn on the error recognition.\n"));
  }
}
void QuantaApp::slotShowBottDock(bool force)
{
  if ( bottdock->parent() == 0L )
  {
    bottdock->manualDock(maindock, KDockWidget::DockBottom, 80);
    showMessagesAction->setChecked(true);
  }
  else
  {
    if (!force)
        bottdock->changeHideShowState();
    else
    {
      showMessagesAction->setChecked(true);
      if (!bottdock->isVisible())
        bottdock->changeHideShowState();
      bottdock->show();
    }
  }
}

void QuantaApp::settingsMenuAboutToShow()
{
  showMessagesAction->setChecked( bottdock->isVisible() );
//Plug the Highlight menu
  setHighlight->unplug(pm_set);
  setHighlight->plug(pm_set);
//Plug the End of line menu
  setEndOfLine->unplug(pm_set);
  setEndOfLine->plug(pm_set);
}

void QuantaApp::viewMenuAboutToShow()
{
  showFTabAction->setChecked( ftabdock->isVisible() );
  showPTabAction->setChecked( ptabdock->isVisible() );
  showTTabAction->setChecked( ttabdock->isVisible() );
  showScriptTabAction->setChecked( scripttabdock->isVisible() );
  showSTabAction->setChecked( stabdock->isVisible() );
  showATabAction->setChecked( atabdock->isVisible() );
  showDTabAction->setChecked( dtabdock->isVisible() );
  showProblemsAction->setChecked(problemsdock->isVisible());
}


QWidget* QuantaApp::createContainer( QWidget *parent, int index, const QDomElement &element, int &id )
{

  QString tabname = element.attribute( "tabname", "" );

  if ( element.tagName().lower() == "toolbar" && !tabname.isEmpty() )
  {
//avoid QToolBar warning in the log
    QtMsgHandler oldHandler = qInstallMsgHandler( silenceQToolBar );
    ToolbarTabWidget *toolbarTab = m_view->toolbarTab();
    QWidget *w = new QWidget(toolbarTab, "ToolbarHoldingWidget");
    QuantaToolBar *tb = new QuantaToolBar(w, element.attribute("name"), true, true);
    tb->loadState(element);
    tb->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
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
    //kdDebug(24000) << "QuantaToolBar minimumSizeHeight :" << tb->minimumSizeHint().height() << endl;
    if (tb->minimumSizeHint().height() > 30)
    {
      if (toolbarTab->tabHeight() < 30)
        toolbarTab->setFixedHeight(tb->minimumSizeHint().height() + 30);
      else
        toolbarTab->setFixedHeight(tb->minimumSizeHint().height() + toolbarTab->tabHeight());
      tb->adjustSize();
      tb->setGeometry(0,0, toolbarTab->width(), tb->height());
    } else //this happens if the toolbar does not contain any buttons
    {
      tb->setGeometry(0,0, toolbarTab->width(), 30);
      if (toolbarTab->tabHeight() < 30)
        toolbarTab->setFixedHeight(tb->height() + 30);
      else
        toolbarTab->setFixedHeight(tb->height() + toolbarTab->tabHeight());

    }
    toolbarTab->insertTab(tb, i18n(tabname));
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
  if ( container->parent() &&  QString(container->parent()->name()) == "ToolbarHoldingWidget")
  {
    m_view->toolbarTab()->removePage(container);
  }
  else
    KMainWindow::removeContainer( container, parent, element, id );
}

void QuantaApp::slotBack()
{
  QWidgetStack *s = widgetStackOfHtmlPart();
  if ( rightWidgetStack->id( rightWidgetStack->visibleWidget()) == 0 )
  {
     slotFilePrev();
     return; //TODO: switching between files has precedence now!
  }
  if ( s->id( s->visibleWidget()) == 1 ) m_htmlPart->back();
  if ( s->id( s->visibleWidget()) == 2 ) m_htmlPartDoc->back();
}

void QuantaApp::slotForward()
{
  QWidgetStack *s = widgetStackOfHtmlPart();
  if ( rightWidgetStack->id( rightWidgetStack->visibleWidget()) == 0 )
  {
    slotFileNext();
    return; //TODO: switching between files has precedence now!
  }
  if ( s->id( s->visibleWidget()) == 1 ) m_htmlPart->forward();
  if ( s->id( s->visibleWidget()) == 2 ) m_htmlPartDoc->forward();
}

void QuantaApp::slotEnableMessageWidget(bool enable)
{
  if (!bottdock->isVisible() == enable)
    bottdock->changeHideShowState();
}

void QuantaApp::slotShowOpenFileList()
{
  KURL::List fileList;
  QStringList openList;
  KURL url;
  KURL baseURL = projectBaseURL();
  fileList = m_doc->openedFiles(false);

  for (uint i = 0; i < fileList.count(); i++)
  {
    url = fileList[i];
    if (m_project->hasProject())
    {
      url = QExtFileInfo::toRelative(url, baseURL);
      if (url.protocol() == baseURL.protocol())
         url.setProtocol("");
    }
    openList.append(url.prettyURL());
  }

  ListDlg listDlg(openList);
  if (listDlg.exec())
  {
    KURL docURL= fileList[listDlg.getEntryNum()];
    m_view->writeTab()->showPage(m_doc->isOpened(docURL));
  }
}

/** No descriptions */
void QuantaApp::slotNewProjectLoaded()
{
  tTab->slotSetTemplateURL(m_project->templateURL);
}

/** No descriptions */
void QuantaApp::slotInsertFile(const KURL& url)
{
  if (m_view->writeExists())
  {
    m_view->write()->insertFile(url);
  }
}


//Kate related
void QuantaApp::setEOLMenuAboutToShow()
{
  if (m_view->writeExists())
  {
    int eol = m_view->write()->kate_view->getEol();
    eol = eol>=0? eol: 0;
    setEndOfLine->setCurrentItem( eol );
  }
}

void QuantaApp::slotContextMenuAboutToShow()
{
  if (m_view->writeExists())
  {
    QString name;
    uint line, col;
    int bl, bc, el, ec;
    QString tagStr;
    m_view->write()->viewCursorIf->cursorPositionReal(&line, &col);
    Node *node = parser->nodeAt(line, col, false);
    if (node)
    {
      if (node->tag->dtd->family == Script)
      {
        StructTreeGroup group;
        for (uint i = 0; i < node->tag->dtd->structTreeGroups.count(); i++)
        {
          group = node->tag->dtd->structTreeGroups[i];
          if (group.hasFileName)
          {
            if (!group.hasSearchRx )
              continue;
            tagStr =  node->tag->tagStr();
            int pos = 0;
            while (pos != -1)
            {
              pos = group.searchRx.search(node->tag->cleanStr, pos);
              if (pos != -1)
              {
                name = tagStr.mid(pos, group.searchRx.matchedLength());
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
                name.remove(group.clearRx);
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
        QMap<QString, XMLStructGroup>::ConstIterator it = node->tag->dtd->xmlStructTreeGroups.find(node->tag->name.lower());

        if (it != node->tag->dtd->xmlStructTreeGroups.end())
        {
          XMLStructGroup group = it.data();
          for (uint j = 0; j <group.attributes.count(); j++ )
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
        KURL baseUrl = QExtFileInfo::path(m_view->write()->url());
        urlUnderCursor = QExtFileInfo::toAbsolute(urlUnderCursor, baseUrl);
        action->setText(i18n("Open File: %1").arg(urlUnderCursor.prettyURL()));
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
      action->setEnabled(m_project->contains(m_view->write()->url()));
    }
  }
}

void QuantaApp::slotOpenFileUnderCursor()
{
  if ( QuantaCommon::checkMimeGroup(urlUnderCursor,"text" ) )
  {
    slotFileOpen( urlUnderCursor, defaultEncoding() );
  }
  else if ( QuantaCommon::checkMimeGroup(urlUnderCursor,"image" ) )
  {
    slotShowPreviewWidget(true);
    slotImageOpen( urlUnderCursor );
  }

}

void QuantaApp::bookmarkMenuAboutToShow()
{
  if (m_view->writeExists())
  {
    pm_bookmark->clear ();
    bookmarkToggle->plug (pm_bookmark);
    bookmarkClear->plug (pm_bookmark);

    Document *w = m_view->write();
    markList = dynamic_cast<KTextEditor::MarkInterface*>(w->doc())->marks();
  //Based on Kate code
    bool hassep = false;
    for (int i=0; (uint) i < markList.count(); i++)
    {
      if (markList.at(i)->type & Kate::Document::markType01)
      {
        if (!hassep) {
          pm_bookmark->insertSeparator ();
          hassep = true;
        }
        QString bText = w->editIf->textLine(markList.at(i)->line);
        bText = bText.stripWhiteSpace();
        if (bText.length() > 35)
        {
          bText.truncate(32);
          bText.append ("...");
        }
        pm_bookmark->insertItem ( QString("%1 - \"%2\"").arg(markList.at(i)->line+1).arg(bText),
                                  this, SLOT (gotoBookmark(int)), 0, i );
      }
    }
  }
}

void QuantaApp::gotoBookmark (int n)
{
  m_view->gotoMark(markList.at(n));
}

/** No descriptions */
void QuantaApp::slotSyntaxCheckDone()
{
  if (m_view->writeExists())
  {
    Document *w = m_view->write();
    if (m_doc->isModified())
    {
      KURL origUrl = w->url();
      if (origUrl.isLocalFile())
          fileWatcher->removeFile(origUrl.path());
      KURL tempUrl;
      tempUrl.setPath(w->tempFileName());
//TODO: Replace with KIO::NetAccess::file_copy, when KDE 3.1 support
//is dropped
      QExtFileInfo::copy(tempUrl, origUrl, -1, true, false, this);
      if (origUrl.isLocalFile())
          fileWatcher->addFile(origUrl.path());
    }
  }
}

/** Load an user toolbar file from the disk. */
void QuantaApp::slotLoadToolbarFile(const KURL& url)
{
 QDomDocument actionDom;
 QDomDocument *toolbarDom = new QDomDocument();

 QTextStream str;
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
     KMessageBox::error(this, i18n("Cannot load the toolbars from the archive.\nCheck that the filenames inside the archives begin as the archive name!"));
     return;
   }

   QDomNodeList nodeList = toolbarDom->elementsByTagName("ToolBar");
   QString name = nodeList.item(0).cloneNode().toElement().attribute("tabname");

   //search for another toolbar with the same name
   QPtrList<KXMLGUIClient> xml_clients = guiFactory()->clients();
   QString newName = name;
   QString origName = name;
   bool found = false;
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
         if ((nodeList.item(i).cloneNode().toElement().attribute("name") ) == name.lower())
         {
           newName = origName + QString(" (%1)").arg(count);
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

   ToolbarEntry* p_toolbar = new ToolbarEntry;

   QDomDocument *dom = new QDomDocument();
   dom->setContent(toolbarDom->toString());
   p_toolbar->dom = dom;

   userToolbarsCount++;

   //Change the name also in the XML File -> create a temp XML file
   KTempFile* tempFile = new KTempFile(tmpDir);
   tempFile->setAutoDelete(true);

   nodeList = toolbarDom->elementsByTagName("ToolBar");
   nodeList.item(0).toElement().setAttribute("name",name.lower());
   nodeList.item(0).toElement().setAttribute("tabname",name);
   nodeList = toolbarDom->elementsByTagName("text");
   nodeList.item(0).toElement().firstChild().setNodeValue(name);

   * (tempFile->textStream()) << toolbarDom->toString();
   tempFile->close();

   //create the new toolbar GUI from the temp file
   ToolbarXMLGUI * toolbarGUI = new ToolbarXMLGUI(tempFile->name());

   //setup the actions
   nodeList = actionDom.elementsByTagName("action");
   for (uint i = 0; i < nodeList.count(); i++)
   {
    QDomNode node = nodeList.item(i).cloneNode();
    QDomElement el = node.toElement();
    QString actionName = el.attribute("name");
    //if there is no such action yet, add to the available actions
    if (! actionCollection()->action(actionName))
    {
      TagAction *tagAction = new TagAction(&el, actionCollection() );

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
       TagAction *tagAction = dynamic_cast<TagAction*>(actionCollection()->action(actionName));
       if (tagAction)
          tagAction->setModified(true);
    }
   }

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

   m_tagsMenu->insertItem(name,menu);
   p_toolbar->menu = menu;

   tempFileList.append(tempFile);
   p_toolbar->guiClient = toolbarGUI;
   p_toolbar->name = name;
   p_toolbar->url = url;
   p_toolbar->visible = true;
   p_toolbar->user = true; //TODO
   toolbarList.insert(name.lower(), p_toolbar);

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
    nodeList = p_toolbar->dom->elementsByTagName("Action");
    for (uint i = 0; i < nodeList.count(); i++)
    {
      action = actionCollection()->action(nodeList.item(i).cloneNode().toElement().attribute("name") );
      if (action)
      {
        action->plug(menu);
      }
    }
    m_tagsMenu->insertItem(p_toolbar->name,menu);
    p_toolbar->menu = menu;
    factory()->addClient(p_toolbar->guiClient);
    p_toolbar->visible = true;
  }
}

/** Load an user toolbar from the disk. */
void QuantaApp::slotLoadToolbar()
{
 KURL url;

 url = KFileDialog::getOpenURL(locateLocal("data","quanta/toolbars/"), "*"+toolbarExtension, this);
 if (! url.isEmpty())
 {
   slotLoadToolbarFile(url.path());
 }
}

/** Load an user toolbar from the disk. */
void QuantaApp::slotLoadGlobalToolbar()
{
 KURL url;

 url = KFileDialog::getOpenURL(qConfig.globalDataDir +"quanta/toolbars/", "*"+toolbarExtension+"\n*", this);
 if (! url.isEmpty())
 {
   slotLoadToolbarFile(url.path());
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

  QBuffer buffer2;
  buffer2.open(IO_WriteOnly);
  QTextStream actStr(&buffer2);

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
           if (e.tagName()=="Action")
           {
             TagAction *action = dynamic_cast<TagAction*>(actionCollection()->action(e.attribute("name")));
             if (action)
             {
               action->data().save(actStr,1);
               action->setModified(false);
             }
           }
           if (e.tagName()=="_Separator_")
           {
             e.setTagName("Separator");
           }
           n = n.nextSibling();
          }
          nodeList.item(i).save(toolStream,2);
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
      kdDebug(24000) << QString("Error %1 at (%2, %3)").arg(error).arg(el).arg(ec)<<endl;
  p_toolbar->dom = dom;

  QTextStream bufferStr(&buffer);
  bufferStr << toolStr;
  buffer.close();
  buffer2.close();

//TODO: Implement saving in non-local dirs (first in a temp file, than copy it to the dest)
  KTar tar(tarFile.path(), "application/x-gzip");
  if (!tar.open(IO_WriteOnly))
      return KURL();
  if (!tar.writeFile(QFileInfo(tarFile.path()).baseName()+".toolbar", "user", "group", buffer.buffer().size(), buffer.buffer().data()))
      return KURL();
  if (!tar.writeFile(QFileInfo(tarFile.path()).baseName()+".actions", "user", "group", buffer2.buffer().size(), buffer2.buffer().data()))
      return KURL();
  tar.close();

  return tarFile;
}

/** Saves a toolbar as local or project specific. */
bool QuantaApp::saveToolbar(bool localToolbar, const QString& toolbarToSave, const KURL& destURL)
{
  int query;
  KURL url;
  KURL projectToolbarsURL;
  QString toolbarName;
  QString localToolbarsDir = locateLocal("data","quanta/toolbars/");

  if (toolbarToSave.isEmpty())
  {
    ToolbarTabWidget *tb = m_view->toolbarTab();

    QStringList lst;
    int current=0;
    for (int i = 0; i < tb->count(); i++)
    {
      lst << tb->label(i);
      if ( tb->tabLabel(tb->currentPage()) == tb->label(i) ) current=i;
    }

    bool ok = FALSE;
#if KDE_IS_VERSION(3, 1, 90)
    QString res = KInputDialog::getItem(
                    i18n( "Save Toolbar" ),
                    i18n( "Please select a toolbar:" ), lst, current, FALSE, &ok, this );
#else
    QString res = QInputDialog::getItem(
                    i18n( "Save Toolbar" ),
                    i18n( "Please select a toolbar:" ), lst, current, FALSE, &ok, this );
#endif
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
        url = KFileDialog::getSaveURL(m_project->toolbarURL.url(), "*"+toolbarExtension, this);
      }

      if (url.isEmpty())
          return false;

      if (m_project->hasProject())
          projectToolbarsURL = m_project->toolbarURL;
      if ( ((!localToolbar) && (projectToolbarsURL.isParentOf(url)) ) ||
            ((localToolbar) && (KURL(localToolbarsDir).isParentOf(url))) )
      {
        query = m_view->write()->checkOverwrite( url );
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
        m_project->insertFile(tarName, true);
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
 KLineEditDlg dlg(i18n("Enter toolbar name:"), i18n("User_%1").arg(userToolbarsCount), this);
 dlg.setCaption(i18n("New Toolbar"));
 if (dlg.exec())
 {
  userToolbarsCount++;
  QString name = dlg.text();


  KTempFile* tempFile = new KTempFile(tmpDir);
  tempFile->setAutoDelete(true);
  * (tempFile->textStream()) << QString("<!DOCTYPE kpartgui SYSTEM \"kpartgui.dtd\">\n<kpartgui name=\"quanta\" version=\"2\">\n<ToolBar name=\"%1\" tabname=\"%2\">\n<text>%3</text>\n</ToolBar>\n</kpartgui>\n")\
               .arg(name.lower()).arg(name).arg(name);
  tempFile->close();

  ToolbarXMLGUI * toolbarGUI = new ToolbarXMLGUI(tempFile->name());
  guiFactory()->addClient(toolbarGUI);
  m_view->toolbarTab()->setCurrentPage(m_view->toolbarTab()->count()-1);
  tempFileList.append(tempFile);
  ToolbarEntry *p_toolbar = new ToolbarEntry;
  p_toolbar->guiClient = toolbarGUI;

  QDomDocument *dom = new QDomDocument(toolbarGUI->domDocument());

  p_toolbar->dom = dom;
  p_toolbar->name = name;
  p_toolbar->user = true;
  p_toolbar->visible = true;
  p_toolbar->menu = new QPopupMenu;
  m_tagsMenu->insertItem(p_toolbar->name, p_toolbar->menu);
  toolbarList.insert(name.lower(), p_toolbar);

  slotToggleDTDToolbar(!allToolbarsHidden());
 }
}


/** Removes a user toolbar from the toolbars. */
bool QuantaApp::slotRemoveToolbar()
{
 ToolbarTabWidget *tb = m_view->toolbarTab();
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
#if KDE_IS_VERSION(3, 1, 90)
 QString res = KInputDialog::getItem(
                 i18n( "Remove Toolbar" ),
                 i18n( "Please select a toolbar:" ), lst, current, FALSE, &ok, this );
#else
 QString res = QInputDialog::getItem(
                 i18n( "Remove Toolbar" ),
                 i18n( "Please select a toolbar:" ), lst, current, FALSE, &ok, this );
#endif

 if (ok)
 {
   return slotRemoveToolbar(res.lower());
 } else
   return false;

}


/** Sends a toolbar in mail. */
void QuantaApp::slotSendToolbar()
{
  ToolbarTabWidget *tb = m_view->toolbarTab();

  QStringList lst;
  int current = 0;
  for (int i = 0; i < tb->count(); i++)
  {
    lst << tb->label(i);
    if ( tb->tabLabel(tb->currentPage()) == tb->label(i) ) current=i;
  }

  bool ok = FALSE;
#if KDE_IS_VERSION(3, 1, 90)
  QString res = KInputDialog::getItem(
                  i18n( "Send Toolbar" ),
                  i18n( "Please select a toolbar:" ), lst, current, FALSE, &ok, this );
#else
  QString res = QInputDialog::getItem(
                  i18n( "Send Toolbar" ),
                  i18n( "Please select a toolbar:" ), lst, current, FALSE, &ok, this );
#endif

  if (!ok)
    return;

  QString toolbarName = res.lower();

  QStringList toolbarFile;

  QString prefix="quanta";
  KTempFile* tempFile = new KTempFile(tmpDir, toolbarExtension);
  tempFile->setAutoDelete(true);
  KURL tempURL;
  tempURL.setPath(tempFile->name());
  saveToolbarToFile(toolbarName, tempURL);
  tempFile->close();
  tempFileList.append(tempFile);

  toolbarFile += tempFile->name();

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

    QString nullString="";
    kapp->invokeMailer(toStr, nullString, nullString, subjectStr, message, nullString, toolbarFile);

  }
  delete mailDlg;
}

void QuantaApp::slotRenameToolbar()
{
  ToolbarTabWidget *tb = m_view->toolbarTab();

  QStringList lst;
  int current = 0;
  for (int i = 0; i < tb->count(); i++)
  {
    lst << tb->label(i);
    if ( tb->tabLabel(tb->currentPage()) == tb->label(i) ) current=i;
  }

  bool ok = FALSE;
#if KDE_IS_VERSION(3, 1, 90)
  QString res = KInputDialog::getItem(
                  i18n( "Rename Toolbar" ),
                  i18n( "Please select a toolbar:" ), lst, current, FALSE, &ok, this );
#else
  QString res = QInputDialog::getItem(
                  i18n( "Rename Toolbar" ),
                  i18n( "Please select a toolbar:" ), lst, current, FALSE, &ok, this );
#endif
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
    KLineEditDlg dlg(i18n("Enter the new name:"), p_toolbar->name, this);
    dlg.setCaption(i18n("Rename Toolbar"));
    if (dlg.exec() && dlg.text() != p_toolbar->name)
    {
      toolbarList.take(name.lower());
      p_toolbar->name = dlg.text();
      QDomElement el = p_toolbar->guiClient->domDocument().firstChild().firstChild().toElement();
      el.setAttribute("tabname", p_toolbar->name);
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
      ToolbarTabWidget *tb = m_view->toolbarTab();
      for (int i = 0; i < tb->count(); i++)
      {
        if (tb->label(i).lower() == name)
        {
          tb->setTabLabel(tb->page(i)->parentWidget(), i18n(p_toolbar->name));
          m_tagsMenu->changeItem(m_tagsMenu->idAt(i + 2), i18n(p_toolbar->name));
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
  ToolbarEntry *p_toolbar;
  while (it.current())
  {
    p_toolbar = it.current();
    ++it;
    if (!slotRemoveToolbar(p_toolbar->name.lower()))
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

  QFile f(KGlobal::instance()->dirs()->saveLocation("data")+"quanta/actions.rc" );
  if (f.open( IO_ReadWrite | IO_Truncate ))
  {
    if (!m_actions->firstChild().firstChild().isNull())
    {
      QTextStream qts(&f);
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
  for (uint i = 0; i < actionCollection()->count(); i++)
  {
    if (actionCollection()->action(i)->text() == actionName)
    {
      action = actionCollection()->action(i);
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
          action->unplug(m_view->toolbarTab()->page(toolbarName));
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

void QuantaApp::slotAssignActionToScript(const KURL& a_scriptURL, const QString& a_interpreter)
{
  ActionConfigDialog dlg(this, "actions_config_dlg");
  QString name = a_scriptURL.fileName();
  name.truncate(name.length() - QFileInfo(name).extension().length() - 1);
  dlg.createScriptAction(name, a_interpreter + " " + a_scriptURL.path());
  dlg.exec();
}

/** Reads the DTD info from the file, tries to find the correct DTD and builds the tag/attribute list from the DTD file. */
void QuantaApp::processDTD(const QString& documentType)
{
 Document *w = m_view->write();
 QString foundName;
 QString projectDTD = m_project->defaultDTD();
 w->setDTDIdentifier(projectDTD);
 Tag *tag = 0L;
 if (documentType.isEmpty())
 {
   foundName = w->findDTDName(&tag); //look up the whole file for DTD definition
   bool found = false;
   if (!foundName.isEmpty())   //!DOCTYPE found in file
   {
      KDialogBase dlg(this, 0L, true, i18n("DTD Selector"), KDialogBase::Ok | KDialogBase::Cancel);
      DTDSelectDialog *dtdWidget = new DTDSelectDialog(&dlg);
      dlg.setMainWidget(dtdWidget);
      QStringList lst;
      QDictIterator<DTDStruct> it(*dtds);
      for( ; it.current(); ++it )
      {
        if (it.current()->toplevel)
        {
          lst << it.current()->nickName;
        }
      }
      lst.sort();
      QString foundNickName = QuantaCommon::getDTDNickNameFromName(foundName);
      for (uint i = 0; i < lst.count(); i++)
      {
        dtdWidget->dtdCombo->insertItem(lst[i]);
        if (lst[i] == foundNickName)
        {
          w->setDTDIdentifier(foundName);
          found =true;
        }
      }

      if (!dtds->find(foundName.lower()))
      {
        //try to find the closest matching DTD
        QString s = foundName.lower();
        uint spaceNum = s.contains(' ');
        QStringList dtdList;
        QStringList lastDtdList;
        QDictIterator<DTDStruct> it(*dtds);
        for (;it.current(); ++it)
        {
          dtdList += it.currentKey();
        }
        for (uint i = 0; i <= spaceNum && !dtdList.empty(); i++)
        {
          lastDtdList = dtdList;
          QStringList::Iterator strIt = dtdList.begin();
          while (strIt != dtdList.end())
          {
            if (!(*strIt).startsWith(s.section(' ', 0, i)))
            {
              strIt = dtdList.remove(strIt);
            } else
            {
              ++strIt;
            }
          }
        }
        dtdList = lastDtdList;
        for (uint i = 0; i <= spaceNum && !dtdList.empty(); i++)
        {
          lastDtdList = dtdList;
          QStringList::Iterator strIt = dtdList.begin();
          while (strIt != dtdList.end())
          {
            if (!(*strIt).endsWith(s.section(' ', -(i+1), -1)))
            {
              strIt = dtdList.remove(strIt);
            } else
            {
              ++strIt;
            }
          }
        }
        if (lastDtdList.count() == 1 || lastDtdList[0].startsWith(s.section(' ', 0, 0)))
        {
          projectDTD = lastDtdList[0];
        }
      }

//    dlg->dtdCombo->insertItem(i18n("Create New DTD Info"));
      dtdWidget->messageLabel->setText(i18n("This DTD is not known for Quanta. Choose a DTD or create a new one."));
      dtdWidget->currentDTD->setText(QuantaCommon::getDTDNickNameFromName(foundName));
      QString projectDTDNickName = QuantaCommon::getDTDNickNameFromName(projectDTD);
      for (int i = 0; i < dtdWidget->dtdCombo->count(); i++)
      {
        if (dtdWidget->dtdCombo->text(i) == projectDTDNickName)
        {
          dtdWidget->dtdCombo->setCurrentItem(i);
          break;
        }
      }
      if (!found && qConfig.showDTDSelectDialog && dlg.exec())
      {
        qConfig.showDTDSelectDialog = !dtdWidget->useClosestMatching->isChecked();
        w->setDTDIdentifier(QuantaCommon::getDTDNameFromNickName(dtdWidget->dtdCombo->currentText()));
        DTDStruct *dtd = dtds->find(w->getDTDIdentifier());
        if (dtdWidget->convertDTD->isChecked() && dtd->family == Xml)
        {
          int bLine, bCol, eLine, eCol;
          tag->beginPos(bLine,bCol);
          tag->endPos(eLine,eCol);
          w->editIf->removeText(bLine, bCol, eLine, eCol+1);
          w->viewCursorIf->setCursorPositionReal((uint)bLine, (uint)bCol);
          w->insertText("<!DOCTYPE" + dtd->doctypeStr +">");
          delete tag;
        }
      }
   } else //DOCTYPE not found in file
   {
     QString mimetype = KMimeType::findByURL(w->url())->name();
     QDictIterator<DTDStruct> it(*dtds);
     DTDStruct *currdtd = 0L;
     for( ; it.current(); ++it )
     {
       currdtd = it.current();
       if (currdtd->toplevel && currdtd->mimeTypes.contains(mimetype))
       {
         break;
       }
       currdtd = 0L;
     }
     if (currdtd)
        w->setDTDIdentifier(currdtd->name);
     else
        w->setDTDIdentifier(projectDTD);
   }
 } else //dtdName is read from the method's parameter
 {
   w->setDTDIdentifier(documentType);
 }

  if (!w->isUntitled())
  {
    m_messageOutput->showMessage(i18n("\"%1\" is used for \"%2\".\n").arg(QuantaCommon::getDTDNickNameFromName(w->getDTDIdentifier())).arg(w->url().prettyURL(0, KURL::StripFileProtocol)));
  }
  loadToolbarForDTD(w->getDTDIdentifier());
  sTab->useOpenLevelSetting = true;
}

/** No descriptions */
void QuantaApp::slotChangeDTD()
{
  if (m_view->writeExists())
  {
    KDialogBase dlg(this, 0L, true, i18n("DTD Selector"), KDialogBase::Ok | KDialogBase::Cancel);
    DTDSelectDialog *dtdWidget = new DTDSelectDialog(&dlg);
    dtdWidget->setMinimumHeight(130);
    dlg.setMainWidget(dtdWidget);
    Document *w = m_view->write();
    int pos = -1;
    int defaultIndex = 0;

    Tag *tag = 0L;
    w->findDTDName(&tag);
    QString oldDtdName = w->getDTDIdentifier();
    QString defaultDocType = m_project->defaultDTD();
    QDictIterator<DTDStruct> it(*dtds);
    QStringList lst;
    for (; it.current(); ++it)
    {
      if (it.current()->toplevel)
      {
        lst << it.current()->nickName;
      }
    }
    lst.sort();

    QString oldDtdNickName = QuantaCommon::getDTDNickNameFromName(oldDtdName);
    QString defaultDtdNickName = QuantaCommon::getDTDNickNameFromName(defaultDocType);
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
    dtdWidget->currentDTD->setText(QuantaCommon::getDTDNickNameFromName(w->getDTDIdentifier()));
    //dlg->useClosestMatching->setShown(false);
    delete dtdWidget->useClosestMatching;
    dtdWidget->useClosestMatching = 0L;
    dtdWidget->adjustSize();
    if (dlg.exec())
    {
      w->setDTDIdentifier(QuantaCommon::getDTDNameFromNickName(dtdWidget->dtdCombo->currentText()));
      DTDStruct *dtd = dtds->find(w->getDTDIdentifier());
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
    reparse(true);
  }
}

/** No descriptions */
void QuantaApp::focusInEvent(QFocusEvent* e)
{
 KDockMainWindow::focusInEvent(e);
 Document *w = m_view->write();
 w->view()->setFocus();
 w->checkDirtyStatus();
}

/** No descriptions */
void QuantaApp::slotShowCompletion()
{
  m_view->write()->codeCompletionRequested();
}

/** No descriptions */
void QuantaApp::slotShowCompletionHint()
{
  m_view->write()->codeCompletionHintRequested();
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
  DTDStruct *oldDtd = dtds->find(currentToolbarDTD);
  if (!oldDtd && !currentToolbarDTD.isEmpty())
      oldDtd = dtds->find(m_project->defaultDTD());

  QString fileName;
  DTDStruct *newDtd = dtds->find(dtdName);
  if (!newDtd)
  {
      newDtd = dtds->find(m_project->defaultDTD());
      if (!newDtd)
          newDtd = dtds->find(qConfig.defaultDocType); //extreme case
  }

  ToolbarEntry *p_toolbar;
  if (newDtd != oldDtd)
  {
    KURL::List newToolbars;
    for (uint i = 0; i < newDtd->toolbars.count(); i++)
    {
      KURL url;
      //first load the local version if it exists
      fileName = locateLocal("data", "quanta/toolbars/"+newDtd->toolbars[i]);
      QuantaCommon::setUrl(url, fileName);
      if (QExtFileInfo::exists(url))
      {
        //showToolbarFile(url);
        newToolbars += url;
      } else
      {
        fileName = qConfig.globalDataDir + "quanta/toolbars/"+newDtd->toolbars[i];
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
        QString fileName = qConfig.globalDataDir + "quanta/toolbars/"+oldDtd->toolbars[i];
        QuantaCommon::setUrl(url, fileName);
        KURL urlLocal;
        fileName = locateLocal("data", "quanta/toolbars/"+oldDtd->toolbars[i]);
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
    m_view->toolbarTab()->setCurrentPage(0);
 }

 currentToolbarDTD = newDtd->name;
 slotToggleDTDToolbar(!allToolbarsHidden());
}

/** Remove the toolbar named "name". */
bool QuantaApp::slotRemoveToolbar(const QString& name)
{
  ToolbarEntry *p_toolbar = toolbarList[name];
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
     bool useToolbarGUI = true;
     if ( s1 != s2 /*|| actionsModified */)
     {
      int result;
      if (p_toolbar->url.isEmpty())
      {
         result = KMessageBox::questionYesNoCancel(this, i18n("<qt>Toolbar <b>%1/<b> is new and unsaved. Do you want to save before remove?</qt>").arg(p_toolbar->name),
              i18n("Save Toolbar"));
      } else
      {
         FourButtonMessageBox dlg(this, 0, true);
         dlg.textLabel->setText(i18n("<qt>The toolbar <b>%1</b> was modified. Do you want to save before remove?</qt>").arg(p_toolbar->name));
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
                if (m_project->hasProject() && p_toolbar->url.url().startsWith(m_project->baseURL.url())) local = false;
                if (!saveToolbar(local, p_toolbar->name))
                    return false;
                break;
             }
        case KMessageBox::Continue:
             {
                bool local = true;
                if (m_project->hasProject() && p_toolbar->url.url().startsWith(m_project->baseURL.url())) local = false;
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
    m_view->toolbarTab()->show();
  } else
  {
    m_view->toolbarTab()->hide();
  }
  qConfig.enableDTDToolbar = show;
}


/** No descriptions */
void QuantaApp::slotParsingDTDChanged(const QString& newDTDName)
{
  if (m_view->writeExists())
  {
    Document *w =  m_view->write();
    w->setParsingDTD(newDTDName);
    w->setChanged(true);
    reparse(false);
  }
}

/** Returns the project's base URL if it exists, the HOME dir if there is no project and no opened document (or the current opened document was not saved yet), and the base URL of the opened document, if it is saved somewhere. */
KURL QuantaApp::projectBaseURL() const
{
  KURL result;
  if  ( m_project->hasProject())
  {
     result = m_project->baseURL;
  } else
  {
    if  ( !m_view->writeExists() || m_view->write()->isUntitled() )
    {
      result = QExtFileInfo::home();
    } else
    {
       result = QExtFileInfo::path(m_view->write()->url());
    }
  }
  return result;
}

/** No descriptions */
void QuantaApp::slotBuildPrjToolbarsMenu()
{
  KURL::List toolbarList;
  if (m_project && m_project->hasProject())
  {
    toolbarList = QExtFileInfo::allFiles(m_project->toolbarURL, "*"+toolbarExtension);
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
  if (m_project && m_project->hasProject())
  {
    encoding = m_project->defaultEncoding();
  }
  return encoding;
}

QPopupMenu *QuantaApp::toolbarMenu(const QString& name)
{
  QPopupMenu *menu = 0L;
  ToolbarEntry* p_toolbar = toolbarList[name.lower()];
  if (p_toolbar) menu = p_toolbar->menu;
  return menu;
}

KURL::List QuantaApp::userToolbarFiles()
{
  KURL::List list;
  ToolbarEntry *p_toolbar;
  QDictIterator<ToolbarEntry> iter(toolbarList);
  for( ; iter.current(); ++iter )
  {
    p_toolbar = iter.current();
    if (p_toolbar->user && p_toolbar->visible)
    {
      list += p_toolbar->url;
    }
  }

  return list;
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

/** No descriptions */
void QuantaApp::slotEmailDTEP()
{
  if (m_view->writeExists())
  {
    Document *w = m_view->write();
    QStringList lst;
    int current = 0;
    int i = 0;
    QDictIterator<DTDStruct> it(*dtds);
    for( ; it.current(); ++it )
    {
      lst << it.current()->nickName;
      if (it.current()->name == w->getDTDIdentifier()) current = i;
      i++;
    }
    lst.sort();
    bool ok = FALSE;
#if KDE_IS_VERSION(3, 1, 90)
    QString res = KInputDialog::getItem(
                    i18n( "Send DTD" ),
                    i18n( "Please select a DTD:" ), lst, current, FALSE, &ok, this );
#else
    QString res = QInputDialog::getItem(
                    i18n( "Send DTD" ),
                    i18n( "Please select a DTD:" ), lst, current, FALSE, &ok, this );
#endif
    if (!ok)
      return;

    QString dtdName = QuantaCommon::getDTDNameFromNickName(res);

    QStringList dtdFile;

    QString prefix="quanta";
    KTempFile* tempFile = new KTempFile(tmpDir, ".tgz");
    tempFile->setAutoDelete(true);

  //pack the .tag files and the description.rc into a .tgz file
    KTar tar(tempFile->name(), "application/x-gzip");
    tar.open(IO_WriteOnly);

    KURL dirURL;
    dirURL.setPath(dtds->find(dtdName)->fileName);
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

    tempFile->close();
    tempFileList.append(tempFile);

    dtdFile += tempFile->name();


    TagMailDlg *mailDlg = new TagMailDlg( this, i18n("Send DTD in email"));
    QString toStr;
    QString message = i18n("Hi,\n This is a Quanta Plus [http://quanta.sourceforge.net] DTD definition tarball.\n\nHave fun.\n");
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

      QString nullString="";
      kapp->invokeMailer(toStr, nullString, nullString, subjectStr, message, nullString, dtdFile);
    }
    delete mailDlg;
  }
}

void QuantaApp::slotDocumentProperties()
{
#ifdef BUILD_KAFKAPART
  if(view()->writeExists())
  {
    if(view()->write()->defaultDTD()->name.contains("HTML", false) == 0)
    {
      KMessageBox::information(this, i18n("The Document Properties Dialog is only for HTML and XHTML."));
      return;
    }
    htmlDocumentProperties htmlPropsDlg(this);
    htmlPropsDlg.exec();
  }
#endif
}

/** Returns the interface number for the currently active editor. */
int QuantaApp::currentEditorIfNum() const
{
  if (m_view->writeExists())
  {
    return m_view->write()->editIf->editInterfaceNumber();
  } else
  {
    return m_view->oldWrite->editIf->editInterfaceNumber();
  }
}

/** Return the URL of the currently active document */
QString QuantaApp::currentURL() const
{
  if (m_view->writeExists())
  {
    return m_view->write()->url().url();
  } else
  {
    return m_view->oldWrite->url().url();
  }
}

QString QuantaApp::projectURL() const
{
  return projectBaseURL().url();
}

QStringList QuantaApp::openedURLs() const
{
  QTabWidget *docTab =m_view->writeTab();
  Document *w;
  QStringList list;
  for (int i = docTab->count() -1; i >=0; i--)
  {
    w = dynamic_cast<Document*>(docTab->page(i));
    if ( w )
    {
      list.prepend( QString("%1:%2").arg(w->editIf->editInterfaceNumber()).arg(w->url().url()));
    }
  }
  return list;
}

void QuantaApp::slotExpandAbbreviation()
{
  if (m_view->writeExists())
  {
    Document *w = m_view->write();
    DTDStruct *dtd = w->currentDTD();
    uint line, col;
    w->viewCursorIf->cursorPositionReal(&line, &col);
    QString text = w->text(line, 0, line, col - 1);
    text = w->findWordRev(text) + " ";
    QString textToInsert;
    QMap<QString, QString>::Iterator it;
    for (it = dtd->abbreviations.begin(); it != dtd->abbreviations.end(); ++it)
    {
      if (it.key().startsWith(text))
      {
        textToInsert = it.data();
        break;
      }
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

void QuantaApp::slotUploadFile()
{
  m_project->slotUploadURL(m_view->write()->url());
}


void QuantaApp::slotUploadOpenedFiles()
{
}

void QuantaApp::slotDeleteFile()
{
  KURL url = m_view->write()->url();
  if (KMessageBox::questionYesNo(this,
                   i18n("<qt>Do you really want to delete the file <b>%1</b> ?</qt>")
                   .arg(url.prettyURL(0, KURL::StripFileProtocol)),
                   i18n("Delete File")) == KMessageBox::Yes)
  {
    if (KIO::NetAccess::del(url))
    {
      if (m_project->hasProject())
        m_project->slotRemove(url);
    }
    m_view->write()->setModified(false); //don't ask for save
    slotFileClose();
  }
}

void QuantaApp::slotFind ()
{
  int id = 0;
  QWidgetStack *s = widgetStackOfHtmlPart();
  if (s)
    id = s->id(s->visibleWidget());
  if (id == 0 && m_view->writeExists())
  {
    m_view->write()->kate_view->find();
  } else
  if (id == 1)
  {
    m_htmlPart->findTextBegin();
  } else
  if (id == 2)
  {
    m_htmlPartDoc->findTextBegin();
  }
}

void QuantaApp::slotFindAgain ()
{
  int id = 0;
  QWidgetStack *s = widgetStackOfHtmlPart();
  if (s)
    id = s->id(s->visibleWidget());
  if (id == 0 && m_view->writeExists())
  {
    m_view->write()->kate_view->findAgain(false);
  } else
  if (id == 1)
  {
    m_htmlPart->findTextNext("", true, true, false);
  } else
  if (id == 2)
  {
    m_htmlPartDoc->findTextNext("", true, true, false);
  }
}

void QuantaApp::slotFindAgainB ()
{
  int id = 0;
  QWidgetStack *s = widgetStackOfHtmlPart();
  if (s)
    id = s->id(s->visibleWidget());
  if (id == 0 && m_view->writeExists())
  {
    m_view->write()->kate_view->findPrev();
  } else
  if (id == 1)
  {
    m_htmlPart->findTextNext("", false, true, false);
  } else
  if (id == 2)
  {
    m_htmlPartDoc->findTextNext("", false, true, false);
  }
}

void QuantaApp::slotReplace ()
{
  int id = 0;
  QWidgetStack *s = widgetStackOfHtmlPart();
  if (s)
    id = s->id(s->visibleWidget());
  if (id == 0 && m_view->writeExists())
  {
    m_view->write()->kate_view->replace();
  }
}

bool QuantaApp::structTreeVisible() const
{
  return stabdock->isVisible();
}

QStringList QuantaApp::tagAreas(const QString &tag, bool includeCoordinates, bool skipFoundContent) const
{
  if (view()->writeExists())
    return view()->write()->tagAreas(tag, includeCoordinates, skipFoundContent);
  else
    return QStringList();
}

void QuantaApp::slotAutosaveTimer()
{
  m_config->reparseConfiguration();
  QTabWidget *docTab = view()->writeTab();
  Document *w;

  for (int i = docTab->count() -1; i >= 0; i--)
  {
    w = dynamic_cast<Document*>(docTab->page(i));
    if (w)
      w->createBackup(m_config);
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


QString QuantaApp::searchPathListEntry(const QString& backedUpUrl,const QString& autosavedUrls)
{
  KURL k(backedUpUrl);
  QStringList autosavedUrlsList = QStringList::split(",", autosavedUrls);
  QStringList::Iterator autosavedUrlsIt;
  for ( autosavedUrlsIt = autosavedUrlsList.begin();
        autosavedUrlsIt != autosavedUrlsList.end();
	++autosavedUrlsIt )
  {
   QString quPID = retrievePID((*autosavedUrlsIt));

   QStringList PIDlist = QStringList::split("\n", m_scriptOutput);

   QStringList::Iterator PIDIt;
   bool isOrphan = true;
   for ( PIDIt = PIDlist.begin(); PIDIt != PIDlist.end(); ++PIDIt )
   {
    if((*PIDIt) == quPID && qConfig.quantaPID != quPID)
    {
     isOrphan = false;
     break;
    }
   }
   if(isOrphan)
   {
    if(retrieveHashedPath(Document::hashedFilePath(k.path())) == retrieveHashedPath((*autosavedUrlsIt)))
      return (*autosavedUrlsIt);
   }
  }
  return QString::null;
}

/** Retrieves PID from the name of a backup file */
QString QuantaApp::retrievePID(const QString& filename)
{
 QString strPID = QString::null;
 strPID = filename.right(filename.length() - filename.findRev("P") - 1);

 if (strPID.isEmpty())
   strPID = filename.right(filename.length() - filename.findRev("N") - 1);

 return strPID;
}
/** Retrieves hashed path from the name of a backup file */
QString QuantaApp::retrieveHashedPath(const QString& filename)
{
 return filename.mid(filename.findRev(".") + 1,
                     filename.findRev("P") - 1 - filename.findRev("."));
}
/** Retrieves the non hashed part of the name of a backup file */
QString QuantaApp::retrieveBaseFileName(const QString& filename)
{
 return filename.left(filename.findRev("."));
}

void QuantaApp::layoutDockWidgets(const QString& layout)
{
  if (layout == "Default")
  {
    ftabdock ->manualDock(maindock, KDockWidget::DockLeft,   30);
    bottdock ->manualDock(maindock, KDockWidget::DockBottom, 80);
    problemsdock->manualDock(bottdock, KDockWidget::DockCenter);

    ptabdock->manualDock(ftabdock, KDockWidget::DockCenter);
    ttabdock->manualDock(ftabdock, KDockWidget::DockCenter);
    scripttabdock->manualDock(ftabdock, KDockWidget::DockCenter);
    KDockWidget *w = stabdock->manualDock(ftabdock, KDockWidget::DockCenter);
    atabdock->manualDock(w, KDockWidget::DockBottom, 70);
    dtabdock->manualDock(ftabdock, KDockWidget::DockCenter);
  } else
  if (layout == "Tabbed")
  {
    ftabdock ->manualDock(maindock, KDockWidget::DockLeft,   30);
    bottdock ->manualDock(maindock, KDockWidget::DockBottom, 80);
    problemsdock->manualDock(bottdock, KDockWidget::DockCenter);

    ptabdock->manualDock(ftabdock, KDockWidget::DockCenter);
    ttabdock->manualDock(ftabdock, KDockWidget::DockCenter);
    scripttabdock->manualDock(ftabdock, KDockWidget::DockCenter);
    stabdock->manualDock(ftabdock, KDockWidget::DockCenter);
    atabdock->manualDock(ftabdock, KDockWidget::DockCenter);
    dtabdock->manualDock(ftabdock, KDockWidget::DockCenter);
  }
}

void QuantaApp::slotLoadDTD()
{
  KURL url = KFileDialog::getOpenURL("", "*.dtd", this);
  if (!url.isEmpty())
  {
    DTDParser dtdParser(url, KGlobal::dirs()->saveLocation("data") + "quanta/dtep");
    if (dtdParser.parse())
    {
      QString dirName = dtdParser.dirName();
      KConfig dtdcfg(dirName + "description.rc");
      dtdcfg.setGroup("General");
      QString dtdName = dtdcfg.readEntry("Name");
      QString nickName = dtdcfg.readEntry("NickName", dtdName);
      if (dtds->find(dtdName.lower()) &&
          KMessageBox::questionYesNo(this, i18n("<qt>Do you want to replace the existing <b>%1</b> DTD?</qt>").arg(nickName)) == KMessageBox::No)
      {
        return;
      }
      removeDTD(dtds->find(dtdName.lower()));
      if (readTagDir(dirName))
      {
          QString family = dtdcfg.readEntry("Family", "1");
          if (family == "1" && m_view->writeExists() &&
              KMessageBox::questionYesNo(this, i18n("<qt>Use the newly loaded <b>%1</b> DTD for the current document?</qt>").arg(nickName), i18n("Change DTD")) == KMessageBox::Yes)
          {
            Document *w = m_view->write();
            w->setDTDIdentifier(dtdName);
            loadToolbarForDTD(w->getDTDIdentifier());
            reparse(true);
          }
      }
    }
  }
}

void QuantaApp::slotLoadDTEP()
{
  QString dirName = KFileDialog::getExistingDirectory(QString::null, 0, i18n("Select A DTEP Directory"));
  if (!dirName.isEmpty())
  {
    dirName += "/";
    KConfig dtdcfg(dirName+"description.rc");
    dtdcfg.setGroup("General");
    QString dtdName = dtdcfg.readEntry("Name");
    QString nickName = dtdcfg.readEntry("NickName", dtdName);
    if (dtds->find(dtdName.lower()) &&
        KMessageBox::questionYesNo(this, i18n("<qt>Do you want to replace the existing <b>%1</b> DTD?</qt>").arg(nickName)) == KMessageBox::No)
    {
      return;
    }
    removeDTD(dtds->find(dtdName.lower()));
    if (!readTagDir(dirName))
    {
      KMessageBox::error(this, i18n("<qt>Cannot read the DTEP from <b>%1</b>. Check that the directory contains a valid DTEP (<i>description.rc and *.tag files</i>).</qt>").arg(dirName), i18n("Error loading DTEP"));
    } else
    {
      QString family = dtdcfg.readEntry("Family", "1");
      if (KMessageBox::questionYesNo(this, i18n("<qt>Autoload the <b>%1</b> DTD in the feature?</qt>").arg(nickName)) == KMessageBox::Yes)
      {
        KURL src;
        src.setPath(dirName);
        KURL target;
        QString destDir = KGlobal::dirs()->saveLocation("data") + "quanta/dtep/";
        target.setPath(destDir + src.fileName());
        KIO::copy( src, target, false); //don't care about the result
      }
      if (family == "1" && m_view->writeExists() &&
          KMessageBox::questionYesNo(this, i18n("<qt>Use the newly loaded <b>%1</b> DTD for the current document?</qt>").arg(nickName), i18n("Change DTD")) == KMessageBox::Yes)
      {
        Document *w = m_view->write();
        w->setDTDIdentifier(dtdName);
        loadToolbarForDTD(w->getDTDIdentifier());
        reparse(true);
      }
    }
  }
}

void QuantaApp::removeDTD(DTDStruct *dtd)
{
  if (dtd)
  {
    delete dtd->tagsList;
    dtd->tagsList = 0L;
    delete dtd->commonAttrs;
    dtd->commonAttrs = 0L;
    dtds->remove(dtd->name.lower());
  }
}

void QuantaApp::slotConvertCase()
{
  if (m_view->writeExists())
  {
    m_view->write()->convertCase();
  }
}

void QuantaApp::slotReloadStructTreeView()
{
  if (m_view->writeExists())
  {
    Document *w = m_view->write();
    sTab->setParsingDTD(w->parsingDTD());
    int expandLevel = qConfig.expandLevel;
    if (expandLevel == 0)
        expandLevel = 40;
    sTab->slotReparse(w, baseNode , expandLevel );
  }
}

#include "quanta.moc"
