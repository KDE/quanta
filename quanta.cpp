/***************************************************************************
                          quanta.cpp  -  description
                             -------------------
    begin                : ?? ???  9 13:29:57 EEST 2000
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

// include files for QT
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

// include files for KDE
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
#include <kparts/componentfactory.h>

#include <ktexteditor/editinterface.h>
#include <ktexteditor/selectioninterface.h>
#include <ktexteditor/viewcursorinterface.h>
#include <ktexteditor/printinterface.h>
#include <ktexteditor/popupmenuinterface.h>

#if (KDE_VERSION > 308)
#include <ktexteditor/dynwordwrapinterface.h>
#endif


#include <kate/view.h>

// application specific includes
#include "document.h"
#include "quanta.h"
#include "quantaview.h"
#include "quantadoc.h"
#include "qextfileinfo.h"
#include "resource.h"

#include "project/project.h"

#include "widgets/whtmlpart.h"

#include "dialogs/filemasks.h"
#include "dialogs/styleoptionss.h"
#include "dialogs/previewoptions.h"
#include "dialogs/previewfontoptions.h"
#include "dialogs/parseroptions.h"
#include "dialogs/debuggeroptionss.h"
#include "dialogs/dtdselectdialog.h"
#include "dialogs/donationdialog.h"

#include "treeviews/filestreeview.h"
#include "treeviews/fileslistview.h"
#include "treeviews/filestreefolder.h"
#include "treeviews/structtreeview.h"
#include "treeviews/structtreetag.h"
#include "treeviews/doctreeview.h"
#include "treeviews/templatestreeview.h"

#include "tagdialogs/listdlg.h"
#include "tagdialogs/tagmaildlg.h"

#include "parser/parser.h"
#include "parser/dtd.h"

#include "messages/messageoutput.h"

#include "toolbar/actioneditdlg.h"
#include "toolbar/toolbarxmlgui.h"
#include "toolbar/tagaction.h"

#include "dialogs/kategrepdialog.h"
#include "dialogs/katefiledialog.h"

#include "plugins/quantakpartplugin.h"
#include "plugins/quantaplugininterface.h"

// from kfiledialog.cpp - avoid qt warning in STDERR (~/.xsessionerrors)
static void silenceQToolBar(QtMsgType, const char *){}


/////////////////////////////////////////////////////////////////////
// SLOT IMPLEMENTATION
/////////////////////////////////////////////////////////////////////

void QuantaApp::setTitle(QString title)
{
  setCaption( "  [  "+title+"  ]  " );
}

void QuantaApp::slotFileNew()
{
  doc->openDocument( KURL() );
  if (!setHighlight)
     setHighlight = view->write()->kate_doc->hlActionMenu (i18n("&Highlight Mode"), actionCollection(), "set_highlight");
}

void QuantaApp::slotFileOpen()
{
 //QTextCodec *codec = QTextCodec::codecForName(defaultEncoding().upper().latin1());
 //QString myEncoding = QString::fromLatin1(codec->name());
 QString myEncoding = defaultEncoding();

 KateFileDialog *dialog = new KateFileDialog (QString::null,myEncoding, this, i18n ("Open File"));
 KateFileDialogData data = dialog->exec();
 delete dialog;

 for (KURL::List::Iterator i=data.urls.begin(); i != data.urls.end(); ++i)
 {
    slotFileOpen( *i , data.encoding);
 }

  slotUpdateStatus(view->write());
}

void QuantaApp::slotFileOpen( const KURL &url, const QString& encoding )
{
  doc->openDocument( url, encoding );
  view->write()->view()->setFocus();
}

void QuantaApp::slotFileOpenRecent(const KURL &url )
{
  QString fn = url.prettyURL();
  if ( fn.left(5) == "file:" ) fn.remove(0,5);

  if ( (url.isLocalFile()) && (! QFileInfo(fn).exists()) )
 {
    if (KMessageBox::questionYesNo( this,
         i18n("The file %1 does not exist.\n Do you want to remove from the list?").arg(fn) )
         == KMessageBox::Yes)
    {
      fileRecent->removeURL(url);
    }
  } else
  {
    doc->openDocument( url );
  }
  view->write()->view()->setFocus();
}

void QuantaApp::slotFileSave()
{
  Document *w = view->write();
  w->checkDirtyStatus();
  if ( w->isUntitled() )
    slotFileSaveAs();
  else
    doc->saveDocument( w->url() );

  slotUpdateStatus(w);
}

void QuantaApp::slotFileSaveAs()
{
  if (view->writeExists())
  {
    Document *w = view->write();
    KURL oldURL = w->url();
    w->checkDirtyStatus();
    fileWatcher->stopScan();
    if (w->kate_view->saveAs() == Kate::View::SAVE_OK)
    {
      w->createTempFile();

      KURL url = w->doc()->url();

      if ( ( project->hasProject() ) &&
          ( KMessageBox::Yes == KMessageBox::questionYesNo(0,i18n("Add file\n %1 \n to project ?").arg(url.url())) )
        )
      {
        project->insertFile(url,true);
      }

      if ( oldURL != url )
      {
        doc->changeFileTabName();
      }
      slotUpdateStatus(w);
    }
    fileWatcher->startScan();
  }
}

void QuantaApp::saveAsTemplate(bool projectTemplate,bool selectionOnly)
{
//TODO: Saving is limited to local files...

  if (!view->writeExists()) return;

  KURL url;
  int query;
  KURL projectTemplateURL;
  Document *w = view->write();
  w->checkDirtyStatus();
  QString localTemplateDir = locateLocal("data","quanta/templates/");

  do {
    query = KMessageBox::Yes;

    if (projectTemplate)
    {
      url = KFileDialog::getSaveURL(project->templateURL.url(), QString::null, this);
    } else
    {
      url = KFileDialog::getSaveURL(locateLocal("data","quanta/templates/"), QString::null, this);
    }

    if (url.isEmpty()) return;

    if ( project->hasProject() )  projectTemplateURL = project->templateURL;
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
    doc->saveDocument( url );
    fileName = url.path();
  }

  if (projectTemplate) project->insertFile(fileName, true);

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
  doc->saveAll();
  slotUpdateStatus(view->write());
}

void QuantaApp::slotFileClose()
{
//[MB]  QWidget *activeWidget = rightWidgetStack->visibleWidget();
  doc->closeDocument();
  WHTMLPart *part = htmlPart();
  part->closeURL();
  part->begin(projectBaseURL());
  part->write( "" );
   part->end();

  slotUpdateStatus(view->write());
}

void QuantaApp::slotFileCloseAll()
{
  doc->closeAll();

  WHTMLPart *part = htmlPart();
  part->closeURL();
  part->begin(projectBaseURL());
  part->write( "" );
   part->end();

  slotNewStatus();
}

void QuantaApp::slotFileNext()
{
   doc->nextDocument();
}

void QuantaApp::slotFilePrev()
{
   doc->prevDocument();
}

void QuantaApp::slotFilePrint()
{
 if (view->writeExists())
     dynamic_cast<KTextEditor::PrintInterface*>(view->write()->doc())->printDialog();
}

void QuantaApp::slotFileQuit()
{
  saveOptions();
  saveModifiedToolbars();

  if(memberList)
  {
    for(unsigned int i= 0; i< memberList->count(); i++)
    {
      memberList->at(i)->close();
    }
  }

}


void QuantaApp::slotEditFindInFiles()
{
  if (!grepDialog) 
  {
    QString startDir = QDir::homeDirPath();
    KURL pBase = projectBaseURL();
    if (pBase.isLocalFile()) startDir = pBase.path(1);
    grepDialog = new GrepDialog( startDir, this, "grep_dialog" );
    connect( grepDialog, SIGNAL( itemSelected   (QString , int)),
             this,       SLOT  ( gotoFileAndLine(QString , int)));
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
  if (statusBar()->isVisible())
    statusBar()->hide();
  else
    statusBar()->show();
}

void QuantaApp::slotHelpTip()
{
   KTipDialog::showTip(this, QString::null, true);
}

void QuantaApp::slotStatusMsg(const QString &msg)
{
  statusbarTimer->stop();
  statusBar()   ->changeItem(" " + msg, IDS_STATUS);
  statusBar()->repaint();
  statusbarTimer->start(10000, true);
}

/** Repaint preview */
void QuantaApp::slotViewRepaint()
{
  repaintPreview( true );
}

/** repaint preview */
void QuantaApp::repaintPreview( bool clear )
{
  if (!view->writeExists()) return;
  static QString oldtext = "";

  previewCopyMade = false;

  WHTMLPart *part = htmlPart();
  QWidgetStack *s = widgetStackOfHtmlPart();

  if ( !s ) return;
  if ( !part ) return;
  if ( !s->id( s ->visibleWidget()) ) return;

  if ( clear )
 {
    oldtext = "";
  }

  KHTMLView *html = part->view();
  int xOffset = html->contentsX(), yOffset = html->contentsY();

  part->closeURL();

  KParts::URLArgs  args(true, browserExtension()->xOffset(), browserExtension()->yOffset());
  browserExtension()->setURLArgs( args );

  KURL url;
  Document *w = view->write();
  if (!w->isUntitled())
  {
//if it's  not untitled, than it was loaded from somewhere. In this case show it from that place
    url = w->url();

    if ( doc->isModified() ) //doc->saveDocument( doc->url() );
         w->saveIt();

    url = project->urlWithPrefix(url);

    part->begin(url, xOffset, yOffset );
    part->openURL( url );
  } else  //the document is Untitled, preview the text from it
 {
    QString text = w->editIf->text();
    if ( text == oldtext ) return;
    if ( text.isEmpty() )
    {
      text = i18n( "<center><h3>The current document is empty...</h3></center>" );
    }
    oldtext = text;
    part->begin( projectBaseURL(), xOffset, yOffset );
    part->write( text );
 }
 part->end();
 part->show();
}

/** view image in preview */
void QuantaApp::slotImageOpen(const KURL& url)
{
  if ( qConfig.previewPosition == "Disabled" )
     return;

  WHTMLPart *part = htmlPart();
  QWidgetStack *s = widgetStackOfHtmlPart();

  if ( !s ) return;
  if ( !part ) return;

  if ( !s->id( s ->visibleWidget()) ) return;

    KToggleAction *ta = (KToggleAction *) actionCollection()->action( "show_preview" );
  ta->setChecked(true);

  QString text = "<html>\n<body>\n<div align=\"center\">\n<img src=\"";
  text += url.path(); //TODO
  text += "\">\n</div>\n</body>\n</html>\n";

  part->closeURL();
  part->begin( projectBaseURL() );
  part->write( text.data() );
  part->end();

  part->show();
}


/** insert <img> tag for images or <a> for other */
void QuantaApp::slotInsertTag(const KURL& url, DirInfo dirInfo)
{
  if (view->writeExists())
  {
    KURL baseURL = projectBaseURL();
    KURL relURL = QExtFileInfo::toRelative( url, baseURL);
    QString urlStr = relURL.url();
    if (relURL.protocol() == baseURL.protocol())
        urlStr = relURL.path();
    Document *w = view->write();
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
  //  w->view()->setFocus();
  }
}

////////////////////////
// status slots
///////////////////////
/** slot for new modify flag */
void QuantaApp::slotNewStatus()
{
  if (view->writeExists())
  {
    Document *w = view->write();
    setTitle( w->url().prettyURL() );

  /*  int  config   = w->config();
    bool readOnly = w->isReadOnly();

    if (readOnly) statusBar()->changeItem(i18n(" R/O "),IDS_INS_OVR);
    else          statusBar()->changeItem(config & KWriteView::cfOvr ? i18n(" OVR ") : i18n(" INS "),IDS_INS_OVR);
                 */
    statusBar()->changeItem(w->isModified() ? " * " : "",IDS_MODIFIED);

    saveAction   ->setEnabled(doc->isModified());
    saveAllAction->setEnabled(doc->isModifiedAll());
    saveprjAction->setEnabled(project->isModified());
   
    bool projectExists = project->hasProject();
    closeprjAction     ->setEnabled(projectExists);
    openPrjViewAction  ->setEnabled(projectExists);
    savePrjViewAction  ->setEnabled(projectExists);
    saveAsPrjViewAction->setEnabled(projectExists);

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

#if (KDE_VERSION > 308)
     //viewFoldingMarkers->setChecked(w->kate_view->lineNumbersOn());
     viewDynamicWordWrap->setChecked(dynamic_cast<KTextEditor::DynWordWrapInterface*>(w->view())->dynWordWrap());
#endif
    if (setHighlight) setHighlight->updateMenu (w->kate_doc);

    QIconSet floppyIcon( UserIcon("save_small"));
    QIconSet  emptyIcon( UserIcon("empty1x16" ));

    QTabWidget *wTab = view->writeTab;
    w = static_cast<Document*>(wTab->currentPage());

    if ( w->isModified() )
       wTab->changeTab( w,  floppyIcon, wTab->tabLabel(w));
    else
     wTab->changeTab( w,  emptyIcon,  wTab->tabLabel(w));

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

    w->oldstat = w->isModified();
 }
}

/** slot for new undo flag */
void QuantaApp::slotNewUndo()
{
/*!!!!  int state = view->write()->undoState();

  undoAction->setEnabled(state & 1);
  redoAction->setEnabled(state & 2);*/
}

void QuantaApp::slotUpdateStatus(QWidget* w)
{
//remove the GUI of the plugin, if the last visible tab was a plugin
  QString tabTitle =view->writeTab->tabLabel(view->oldTab);
  QuantaKPartPlugin *plugin = dynamic_cast<QuantaKPartPlugin *>(m_pluginInterface->plugin(tabTitle));
  if (plugin)
    plugin->showGui(false);
  view->oldTab = w;

  Document *newWrite = dynamic_cast<Document *>(w);
  if (!newWrite)
  {
//add the GUI for the currently visible plugin
    tabTitle = view->writeTab->tabLabel(w);
    plugin = dynamic_cast<QuantaKPartPlugin *>(m_pluginInterface->plugin(tabTitle));
    if (plugin)
       plugin->showGui(true);
    return;
  }
  dynamic_cast<KTextEditor::PopupMenuInterface*>(newWrite->view())->installPopup((QPopupMenu *)factory()->container("popup_editor", quantaApp));
  newWrite->checkDirtyStatus();
  if (newWrite != view->oldWrite)
    sTab->useOpenLevelSetting = true;
  reparse(true);
  slotNewUndo();
  slotNewStatus();
  slotNewLineColumn();

  loadToolbarForDTD(newWrite->getDTDIdentifier());

  newWrite->readConfig(config);
  Document *currentWrite = view->write();
  currentWrite->view()->resize(view->writeTab->size().width()-5, view->writeTab->size().height()-35);
  view->oldWrite = currentWrite;
  currentWrite->kate_view->setIconBorder(qConfig.iconBar);
  currentWrite->kate_view->setLineNumbersOn(qConfig.lineNumbers);
  viewBorder->setChecked(qConfig.iconBar);
  viewLineNumbers->setChecked(qConfig.lineNumbers);
  
#if (KDE_VERSION > 308)
  dynamic_cast<KTextEditor::DynWordWrapInterface*>(currentWrite->view())->setDynWordWrap(qConfig.dynamicWordWrap);
  viewDynamicWordWrap->setChecked(dynamic_cast<KTextEditor::DynWordWrapInterface*>(currentWrite->view())->dynWordWrap());
#endif

  QWidgetStack *s = widgetStackOfHtmlPart();
  if (s->id(s->visibleWidget()) == 1)
  {
   repaintPreview(true);
  }
  
  emit reloadTreeviews();
}

void QuantaApp::slotOptionsConfigureKeys()
{
  KKeyDialog::configureKeys( actionCollection(), xmlFile(), true, this );
}

void QuantaApp::slotOptionsConfigureToolbars()
{
 int currentPageIndex = view->toolbarTab->currentPageIndex();

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
 }
 saveMainWindowSettings(KGlobal::config(), autoSaveGroup());
 KEditToolbar dlg(factory(), this);

 //remove the manually added menus BEFORE the dlg shows up
 menuBar()->removeItem(menuBar()->idAt(TAGS_MENU_PLACE));
 menuBar()->removeItem(menuBar()->idAt(PLUGINS_MENU_PLACE));

 connect( &dlg, SIGNAL( newToolbarConfig() ), SLOT( slotNewToolbarConfig() ) );
 dlg.exec();

 //add the menus
 menuBar()->insertItem(i18n("Plu&gins"), m_pluginMenu, -1, PLUGINS_MENU_PLACE);
 menuBar()->insertItem(i18n("&Tags"),m_tagsMenu,-1, TAGS_MENU_PLACE);
 view->toolbarTab->setCurrentPage(currentPageIndex);
}

void QuantaApp::slotNewToolbarConfig()
{
 applyMainWindowSettings(KGlobal::config(), autoSaveGroup());
 view->toolbarTab->setCurrentPage(view->toolbarTab->currentPageIndex());
}

void QuantaApp::slotOptionsConfigureActions()
{
 int currentPageIndex = view->toolbarTab->currentPageIndex();

 ActionEditDlg dlg( this, "actions_edit_dlg", true); //actionCollection(), QString::null, true, this );

 if ( dlg.exec() )
 {
    QFile f( KGlobal::instance()->dirs()->saveLocation("data")+"quanta/actions.rc" );
    f.open( IO_ReadWrite | IO_Truncate );
    QTextStream qts(&f);
    m_actions->save(qts,0);
    f.close();
    view->toolbarTab->setCurrentPage(currentPageIndex);
 }
}


void QuantaApp::slotOptions()
{
  KDialogBase *kd = new KDialogBase(KDialogBase::IconList,
              i18n("Configure Quanta"),
              KDialogBase::Ok | KDialogBase::Cancel,
              KDialogBase::Ok, this, "tabdialog");

  // Tag Style options
  QVBox *page=kd->addVBoxPage(i18n("Tag Style"), QString::null, BarIcon("kwrite", KIcon::SizeMedium ) );
  StyleOptionsS *styleOptionsS = new StyleOptionsS( (QWidget *)page );

  styleOptionsS->tagCase->setCurrentItem( qConfig.tagCase);
  styleOptionsS->attributeCase->setCurrentItem( qConfig.attrCase);
  styleOptionsS->tagAutoClose->setChecked( qConfig.closeTags );
  styleOptionsS->optionalTagAutoClose->setChecked( qConfig.closeOptionalTags );
  styleOptionsS->useAutoCompletion->setChecked( qConfig.useAutoCompletion );
  styleOptionsS->tagUpdateClosing->setChecked(qConfig.updateClosingTags);

  // Environment options
  //TODO FileMasks name is not good anymore
  page=kd->addVBoxPage(i18n("Environment"), QString::null, BarIcon("files", KIcon::SizeMedium ) );
  FileMasks *fileMasks = new FileMasks( (QWidget *)page );

  fileMasks->lineMarkup->setText( qConfig.markupMimeTypes );
  fileMasks->lineScript->setText( qConfig.scriptMimeTypes );
  fileMasks->lineImage->setText( qConfig.imageMimeTypes );
  fileMasks->lineText->setText( qConfig.textMimeTypes );

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

  // Preview options
  page=kd->addVBoxPage(i18n("Preview"), QString::null, BarIcon("kview", KIcon::SizeMedium ) );
  PreviewOptions *previewOptions = new PreviewOptions( (QWidget *)page );

  previewOptions->setPosition( qConfig.previewPosition );

  page=kd->addVBoxPage(i18n("Parser"), QString::null, BarIcon("kcmsystem", KIcon::SizeMedium ) );
  ParserOptions *parserOptions = new ParserOptions( config, (QWidget *)page );
  QString name;
  int index;
  QDictIterator<DTDStruct> it(*dtds);
  for( ; it.current(); ++it )
  {
    index = -1;
    name = it.current()->name;
    if (it.current()->family == Xml)
    {
      if (name.lower() == qConfig.defaultDocType.lower()) index = 0;
      fileMasks->defaultDTDCombo->insertItem(QuantaCommon::getDTDNickNameFromName(name), index);
    }
  }

  parserOptions->refreshFrequency->setValue(qConfig.refreshFrequency);
  parserOptions->instantUpdate->setChecked(qConfig.instantUpdate);
  parserOptions->showEmptyNodes->setChecked(qConfig.showEmptyNodes);
  parserOptions->showClosingTags->setChecked(qConfig.showClosingTags);
  parserOptions->spinExpand->setValue(qConfig.expandLevel);
  page=kd->addVBoxPage(i18n("PHP Debug"), QString::null, BarIcon("gear", KIcon::SizeMedium ) );
  DebuggerOptionsS *debuggerOptions = new DebuggerOptionsS( (QWidget *)page );

  if (debuggerStyle=="PHP3") debuggerOptions->radioPhp3->setChecked(true);
  if (debuggerStyle=="None") debuggerOptions->checkDebugger->setChecked(false);

  if (KDE_VERSION >= 308)
  {
//Spelling options
    page=kd->addVBoxPage(i18n("Spelling"), QString::null, BarIcon("spellcheck", KIcon::SizeMedium ) );
    new KSpellConfig( (QWidget *)page, 0L, qConfig.spellConfig, false );
  }
  kd->adjustSize();
  if ( kd->exec() )
  {
    qConfig.tagCase = styleOptionsS->tagCase->currentItem();
    qConfig.attrCase = styleOptionsS->attributeCase->currentItem();
    qConfig.closeTags = styleOptionsS->tagAutoClose->isChecked();
    qConfig.closeOptionalTags = styleOptionsS->optionalTagAutoClose->isChecked();
    qConfig.useAutoCompletion = styleOptionsS->useAutoCompletion->isChecked();
    qConfig.updateClosingTags = styleOptionsS->tagUpdateClosing->isChecked();

    qConfig.markupMimeTypes = fileMasks->lineMarkup->text();
    qConfig.scriptMimeTypes  = fileMasks->lineScript->text();
    qConfig.imageMimeTypes= fileMasks->lineImage->text();
    qConfig.textMimeTypes = fileMasks->lineText->text();

    qConfig.defaultEncoding = fileMasks->encodingCombo->currentText();

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

    QWidgetStack *s;
    if ( htmlPart() )
    {
      s = widgetStackOfHtmlPart();
      s ->raiseWidget( 0 );
     }
//    checkCommand( ID_VIEW_PREVIEW, false );

    qConfig.previewPosition = previewOptions->position();

    htmlpart->closeURL();
    htmlpart->begin( projectBaseURL());
    htmlpart->write( "" );
    htmlpart->end();

    repaintPreview(true);
    reparse(true);
  }

  config->sync();

  saveOptions();

  delete kd;
}


void QuantaApp::slotSwapLeftPanelMode()
{
  if ( fTab->id( fTab->visibleWidget() ) == 0 )
  {
    FilesTreeFolder *p = dynamic_cast<FilesTreeFolder *>(fTTab->currentItem()->parent());
    if (!p) return;

    QString dir = p->fullName();

    fLTab->dir = dir;
    fLTab->slotReload();

    fTab->raiseWidget(1);
  }
  else if ( fTab->id( fTab->visibleWidget() ) == 1 ) fTab->raiseWidget(0);
}


void QuantaApp::slotActivatePreview()
{
  WHTMLPart *part = htmlPart();
  if ( !part ) return;
  QWidgetStack *s = widgetStackOfHtmlPart();
  previousWidgetId = s->id(s->visibleWidget());
  s->raiseWidget(1);
}

void QuantaApp::slotShowPreview()
{
  if (!view->writeExists()) return;
  WHTMLPart *part = htmlPart();
  QWidgetStack *s = widgetStackOfHtmlPart();
  Document *w = view->write();

  if ( !s ) return;
  if ( !part ) return;


  KToggleAction *ta = (KToggleAction *) actionCollection()->action( "show_preview" );
  bool stat = !ta->isChecked();

  if ( stat )
  {
    s->raiseWidget(previousWidgetId);

//FIXME:
//Restore the original doc from the temp file.
//We should find a better synchronous method to copy the temp file to the current one.
//which works also for non local files
    fileWatcher->stopScan();
    if (doc->isModified())
    {
      KURL origUrl = w->url();
      KURL tempUrl;
      tempUrl.setPath(w->tempFileName());

      KTextEditor::Document *doc2 = KParts::ComponentFactory::createPartInstanceFromQuery<KTextEditor::Document>( "KTextEditor/Document",
                                    QString::null,
                                    this, 0,
                                    this, 0 );
      doc2->openURL(tempUrl);
      doc2->saveAs(origUrl);
      delete doc2;
    }
    fileWatcher->startScan();
  }
  else
  {
    if ( qConfig.previewPosition == "Bottom" )
    {
      //TODO: ???
    }

    previousWidgetId = s->id(s->visibleWidget());
    s->raiseWidget(1);
    repaintPreview(false);
  }
}

void QuantaApp::slotShowProjectTree()
{
  if ( !ptabdock->isVisible() ) ptabdock->changeHideShowState();
}

void QuantaApp::slotShowTemplatesTree()
{
  if ( !ttabdock->isVisible() ) ttabdock->changeHideShowState();
}

void QuantaApp::slotNewLineColumn()
{
  QString linenumber;
  uint line, col;

  view->write()->viewCursorIf->cursorPositionReal(&line, &col);
  sTab->showTagAtPos(line,col);
  linenumber.sprintf(i18n("Line: %d Col: %d"),line+1,col+1);

  statusBar()->changeItem(linenumber.data(), IDS_STATUS_CLM);
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
  if (view->writeExists())
  {
    Document *w = view->write();
    w->parseVariables();
    if (force)
    {
      baseNode = parser->parse(w);
    }

    sTab->setParsingDTD(w->parsingDTD());
    int expandLevel = qConfig.expandLevel;
    if (expandLevel == 0)
        expandLevel = 40;
    sTab->slotReparse(w, baseNode , expandLevel );

    if (force)
    {
      uint line, col;
      w->viewCursorIf->cursorPositionReal(&line, &col);
      sTab->showTagAtPos(line,col);
    }
  }

  return;
}

void QuantaApp::setCursorPosition( int row, int col )
{
  if (view->writeExists())
  {
    Document *w = view->write();

    int numLines = w->editIf->numLines();

    if ( row < numLines )
      w->viewCursorIf->setCursorPositionReal(row, col);
    else
      w->viewCursorIf->setCursorPositionReal(numLines - 1, col);
  }
}

void QuantaApp::gotoFileAndLine(QString filename, int line )
{
  if ( !filename.isEmpty() ) doc->openDocument( filename );

  if (view->writeExists())
  {
    Document *w = view->write();
    int numLines = w->editIf->numLines();
    if ( numLines > line && line >= 0 )
    {
      w->viewCursorIf->setCursorPositionReal(line, 0);
    }
  }
}


//void QuantaApp::slotLeftTabChanged(QWidget *)
void QuantaApp::slotDockChanged()
{
  static bool docTabOpened = false;

  QWidgetStack *s = widgetStackOfHtmlPart();
  if ( dtabdock->isVisible() )
  {
    static bool first = true;
    previousWidgetId = s->id(s->visibleWidget());
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
      s->raiseWidget(previousWidgetId);
      docTabOpened = false;
    }
    if ( !exitingFlag )
    {
      if (view->writeExists()) view->write()->view()->setFocus();
    }
  }
}

void QuantaApp::selectArea(int line1, int col1, int line2, int col2)
{
  if (view->writeExists())
  {
    Document *w = view->write();
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

   htmlPartDoc->closeURL();
   htmlPartDoc->openURL(url);
   htmlPartDoc->show();
   htmlPartDoc->addToHistory(url);

   oldUrl = url;
}
/*
void QuantaApp::updateNavButtons( bool back, bool forward )
{

   if ( back )
       enableCommand(ID_VIEW_BACK);
   else
       disableCommand(ID_VIEW_BACK);

   if ( forward )
       enableCommand(ID_VIEW_FORWARD);
   else
       disableCommand(ID_VIEW_FORWARD);

} */

void QuantaApp::slotContextHelp()
{
  int id_w = rightWidgetStack->id( rightWidgetStack->visibleWidget());

  QWidgetStack *s = widgetStackOfHtmlPart();
  if (  id_w == 1 || id_w == 2 )
  {
    if ( !m_oldTreeViewWidget->isVisible() ) m_oldTreeViewWidget->changeHideShowState();
      s->raiseWidget(previousWidgetId);
  }
  else
  {
    QString currentW =view->write()->kate_view->currentWord();
    QString *url = dTab->contextHelp( currentW);

    if ( url )
    {
      if (ftabdock->isVisible()) m_oldTreeViewWidget = ftabdock;
      if (ptabdock->isVisible()) m_oldTreeViewWidget = ptabdock;
      if (ttabdock->isVisible()) m_oldTreeViewWidget = ttabdock;
      if (stabdock->isVisible()) m_oldTreeViewWidget = stabdock;
      if (dtabdock->isVisible()) m_oldTreeViewWidget = dtabdock;
      if (!dtabdock->isVisible()) dtabdock->changeHideShowState();

      previousWidgetId = s->id(s->visibleWidget());
      s->raiseWidget(2);
      htmlPartDoc->view()->setFocus();

      openDoc(*url);
    }
  }
}

void QuantaApp::slotShowFTabDock() { ftabdock->changeHideShowState();}
void QuantaApp::slotShowPTabDock() { ptabdock->changeHideShowState();}
void QuantaApp::slotShowTTabDock() { ttabdock->changeHideShowState();}
void QuantaApp::slotShowSTabDock() { stabdock->changeHideShowState();}
void QuantaApp::slotShowDTabDock() { dtabdock->changeHideShowState();}
void QuantaApp::slotShowBottDock()
{
  if ( bottdock->parent() == 0L )
       bottdock->manualDock(maindock, KDockWidget::DockBottom, 80);
  else bottdock->changeHideShowState();
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
  showSTabAction->setChecked( stabdock->isVisible() );
  showDTabAction->setChecked( dtabdock->isVisible() );
}

void QuantaApp::slotToolSyntaxCheck()
{
  if (view->writeExists())
  {
  //  slotFileSave();
    Document *w = view->write();

    if ( doc->isModified() )
    {
      w->saveIt();
    }

    if ( !w->isUntitled() )
    {
      QString fname = w->url().prettyURL();
      if ( fname.left(5) == "file:" ) fname.remove(0,5);

      KProcess *p = new KProcess();
      *p << "perl";
      *p << locate("lib","quanta/plugins/weblint");
      *p << "-x" << "Netscape";
      *p << fname;

      connect( p, SIGNAL(processExited(KProcess *)),
              messageOutput, SLOT(weblintFinished()) );
      connect( p, SIGNAL(processExited(KProcess *)),
              this, SLOT(slotSyntaxCheckDone()) );
      connect( p, SIGNAL(receivedStdout(KProcess *, char *, int)),
              messageOutput, SLOT( processWebLint(KProcess *, char *, int)) );


      p->start( KProcess::NotifyOnExit, KProcess::Stdout);
    }
  }
}

QWidget* QuantaApp::createContainer( QWidget *parent, int index, const QDomElement &element, int &id )
{

  QString tabname = element.attribute( "tabname", "" );

  if ( element.tagName().lower() == "toolbar" && !tabname.isEmpty() )
  {
//avoid QToolBar warning in the log
    QtMsgHandler oldHandler = qInstallMsgHandler( silenceQToolBar );
    KToolBar *tb = new KToolBar(view->toolbarTab, 0, true, true);
    tb->loadState(element);
    tb->enableMoving(false);
    tb->setEnableContextMenu(true);

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
    view->toolbarTab->addTab(tb, i18n(tabname));
    qInstallMsgHandler( oldHandler );
    return tb;
  }

  return KMainWindow::createContainer( parent, index, element, id );

}

void QuantaApp::removeContainer( QWidget *container, QWidget *parent, QDomElement &element, int id )
{
//  debug( QString("name:") + container->parent()->name() );

/*  if ( container->parent() && QString(container->parent()->name()) == QString("ToolBar stack") ) {
    ((KToolBar*)container)->saveState( element );
    int id = view->toolbarStack->id( container );
    qDebug("id: %d",id);
    view->toolbarStack->removeWidget( container );
    view->tabBar->removeTab( view->tabBar->tab(id) );
  }
  else                                                */
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
  if ( s->id( s->visibleWidget()) == 1 ) htmlpart->back();
  if ( s->id( s->visibleWidget()) == 2 ) htmlPartDoc->back();
}

void QuantaApp::slotForward()
{
  QWidgetStack *s = widgetStackOfHtmlPart();
  if ( rightWidgetStack->id( rightWidgetStack->visibleWidget()) == 0 )
  {
    slotFileNext();
    return; //TODO: switching between files has precedence now!
  }
  if ( s->id( s->visibleWidget()) == 1 ) htmlpart->forward();
  if ( s->id( s->visibleWidget()) == 2 ) htmlPartDoc->forward();
}

void QuantaApp::slotMessageWidgetEnable()
{
  if ( !bottdock->isVisible() )
    bottdock->changeHideShowState();
}

void QuantaApp::slotMessageWidgetDisable()
{
  if ( bottdock->isVisible() )
    bottdock->changeHideShowState();
}

void QuantaApp::autoComplete()
{
}

void QuantaApp::slotShowOpenFileList()
{
  KURL::List fileList;
  QStringList openList;
  KURL url;
  KURL baseURL = projectBaseURL();
  fileList = doc->openedFiles(false);

//A little trick. I fill in the list in reversed order.
  for (int i = fileList.count()-1;  i >=0 ; i--)
  {
    url = QExtFileInfo::toRelative(fileList[i], baseURL);
    openList.append(url.url());
  }

  ListDlg listDlg(openList);
  if (listDlg.exec())
  {
  //This "complex" read-out is due to the reversed list.
    KURL docURL= fileList[openList.count() - listDlg.getEntryNum() - 1];
    view->writeTab->showPage(doc->isOpened(docURL));
  }
}

/** No descriptions */
void QuantaApp::slotNewProjectLoaded()
{
  tTab->slotSetTemplateURL(project->templateURL);
}

/** No descriptions */
void QuantaApp::slotInsertFile(const KURL& url)
{
  if (view->writeExists())
  {
    view->write()->insertFile(url);
  }
}


//Kate releated
void QuantaApp::setEOLMenuAboutToShow()
{
  if (view->writeExists())
  {
    int eol = view->write()->kate_view->getEol();
    eol = eol>=0? eol: 0;
    setEndOfLine->setCurrentItem( eol );
  }
}

void QuantaApp::bookmarkMenuAboutToShow()
{
  if (view->writeExists())
  {
    pm_bookmark->clear ();
    bookmarkToggle->plug (pm_bookmark);
    bookmarkClear->plug (pm_bookmark);

    Document *w = view->write();
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
        bText.truncate(32);
        bText.append ("...");
        pm_bookmark->insertItem ( QString("%1 - \"%2\"").arg(markList.at(i)->line+1).arg(bText),
                                  this, SLOT (gotoBookmark(int)), 0, i );
      }
    }
  }
}

void QuantaApp::gotoBookmark (int n)
{
  view->gotoMark(markList.at(n));
}

/** No descriptions */
void QuantaApp::slotSyntaxCheckDone()
{
  if (view->writeExists())
  {
  //FIXME:
  //Restore the original doc from the temp file.
  //We should find a better synchronous method to copy the temp file to the current one.
  //A method for this is also a good idea.
    Document *w = view->write();

    fileWatcher->stopScan();
    if (doc->isModified())
    {
      KURL origUrl = w->url();
      KURL tempUrl;
      tempUrl.setPath(w->tempFileName());

      KTextEditor::Document *doc2 = KParts::ComponentFactory::createPartInstanceFromQuery<KTextEditor::Document>( "KTextEditor/Document",
                                    QString::null,
                                    this, 0,
                                    this, 0 );
      doc2->openURL(tempUrl);
      doc2->saveAs(origUrl);
      delete doc2;
    }
    fileWatcher->startScan();
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
   QPtrList<KXMLGUIClient> xml_clients = factory()->clients();
   bool found = false;
   for (uint index = 0; index < xml_clients.count(); index++)
   {
    nodeList = xml_clients.at(index)->domDocument().elementsByTagName("ToolBar");
    for (uint i = 0; i < nodeList.count(); i++)
    {
      if ((nodeList.item(i).cloneNode().toElement().attribute("name") ) == name.lower())
      {
        QString newName;
        do
        {
          KMessageBox::information(this,i18n("A toolbar called \"%1\" already exists.\n Please rename the loaded toolbar.").arg(name),i18n("Name conflict"));

          KLineEditDlg dlg(i18n("Enter toolbar name:"), name, this);
          dlg.setCaption(i18n("Rename Toolbar"));
          if (dlg.exec())
          {
            newName =  dlg.text();
          } else
          {
            KMessageBox::information(this,i18n("The rename was canceled.\n The toolbar won't be loaded."),i18n("Name conflict"));
            return;
          }
        } while (name == newName);
        name = newName;
        found = true;
        break;
      }
     }
     if (found) break;
   }

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
      m_actions->firstChild().appendChild(el);
      new TagAction(&el, actionCollection() );
    }
   }

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

   guiFactory()->addClient(toolbarGUI);
   view->toolbarTab->setCurrentPage(view->toolbarTab->count()-1);

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

KURL QuantaApp::saveToolBar(const QString& toolbarName, const KURL& destFile)
{
  KURL tarFile = destFile;

  if (! destFile.fileName().endsWith(toolbarExtension))
  {
    tarFile.setFileName(destFile.fileName() + toolbarExtension);
  }

  QBuffer buffer;
  buffer.open(IO_WriteOnly);
  QTextStream toolStr(&buffer);

  QBuffer buffer2;
  buffer2.open(IO_WriteOnly);
  QTextStream actStr(&buffer2);

  QDomNodeList nodeList, nodeList2;
  QStringList actionNameList;

  toolStr << QString("<!DOCTYPE kpartgui SYSTEM \"kpartgui.dtd\">\n<kpartgui name=\"quanta\" version=\"2\">\n");
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
          nodeList.item(i).save(toolStr,2);
          //find the actions registered to the toolbar
          QDomNode n = nodeList.item(i).firstChild();
          while (! n.isNull())
          {
           QDomElement e = n.cloneNode().toElement();
           if (e.tagName()=="Action")
           {
            actionNameList += e.attribute("name");
           }
           n = n.nextSibling();
          }

          nodeList2 = actions()->elementsByTagName("action");
          for (uint k =0; k < nodeList2.count(); k++)
          {
            if (actionNameList.contains(nodeList2.item(k).cloneNode().toElement().attribute("name")) > 0)
            {
                nodeList2.item(k).save(actStr,1);
            }
          }

       }
      }
  }
  toolStr << QString("\n</kpartgui>");
  actStr << QString("\n</actions>");

  ToolbarEntry *p_toolbar = toolbarList[toolbarName.lower()];
  if (p_toolbar->dom) delete p_toolbar->dom;
  QDomDocument *dom = new QDomDocument();
  dom->setContent(toolStr.read());
  p_toolbar->dom = dom;

  buffer.close();
  buffer2.close();

//TODO: Implement saving in non-local dirs (first in a temp file, than copy it to the dest)
  KTar tar(tarFile.path(), "application/x-gzip");
  tar.open(IO_WriteOnly);
  tar.writeFile(QFileInfo(tarFile.path()).baseName()+".toolbar", "user", "group", buffer.buffer().size(), buffer.buffer().data());
  tar.writeFile(QFileInfo(tarFile.path()).baseName()+".actions", "user", "group", buffer2.buffer().size(), buffer2.buffer().data());
  tar.close();

  return tarFile;
}

/** Saves a toolbar as local or project specific. */
void QuantaApp::slotSaveToolbar(bool localToolbar, QString toolbarToSave)
{
  int query;
  KURL url;
  KURL projectToolbarsURL;
  QString toolbarName;
  QString localToolbarsDir = locateLocal("data","quanta/toolbars/");

  if (toolbarToSave.isEmpty())
  {
    QTabWidget *tb = view->toolbarTab;

    QStringList lst;
    int current=0;
    for (int i = 0; i < tb->count(); i++)
    {
      lst << tb->label(i);
      if ( tb->tabLabel(tb->currentPage()) == tb->label(i) ) current=i;
    }

    bool ok = FALSE;
    QString res = QInputDialog::getItem(
                    i18n( "Save Toolbar" ),
                    i18n( "Please select a toolbar:" ), lst, current, FALSE, &ok, this );
    if ( !ok )
      return;

    toolbarName = res;
  } else
  {
    toolbarName = toolbarToSave;
  }


  do {
    query = KMessageBox::Yes;

    if (localToolbar)
    {
      url = KFileDialog::getSaveURL(localToolbarsDir, "*"+toolbarExtension, this);
    } else
    {
      url = KFileDialog::getSaveURL(project->toolbarURL.url(), "*"+toolbarExtension, this);
    }

    if (url.isEmpty()) return;

    if ( project->hasProject() )  projectToolbarsURL = project->toolbarURL;
    if ( ((!localToolbar) && (projectToolbarsURL.isParentOf(url)) ) ||
          ((localToolbar) && (KURL(localToolbarsDir).isParentOf(url))) )
    {
      query = doc->write()->checkOverwrite( url );
    } else
    {
      if (!localToolbar) localToolbarsDir = projectToolbarsURL.prettyURL();
      KMessageBox::sorry(0,i18n("You must save the toolbars in the following directory: \n\n%1")
                                .arg(localToolbarsDir));
      query = KMessageBox::No;
    }
  } while (query != KMessageBox::Yes);

  if( query != KMessageBox::Cancel )
  {          
    KURL tarName = saveToolBar(toolbarName, url);
    if (!localToolbar) project->insertFile(tarName, true);
  }
}

/** Saves a toolbar as localspecific. */
void QuantaApp::slotSaveLocalToolbar()
{
  slotSaveToolbar(true);
}
/** Saves a toolbar as project specific. */
void QuantaApp::slotSaveProjectToolbar()
{
 slotSaveToolbar(false);
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
  factory()->addClient(toolbarGUI);
  view->toolbarTab->setCurrentPage(view->toolbarTab->count()-1);
  tempFileList.append(tempFile);
  ToolbarEntry *p_toolbar = new ToolbarEntry;
  p_toolbar->guiClient = toolbarGUI;

  QDomDocument *dom = new QDomDocument(toolbarGUI->domDocument());

  p_toolbar->dom = dom;
  p_toolbar->name = name;
  p_toolbar->user = true;
  p_toolbar->visible = true;
  p_toolbar->menu = 0L; //TODO
  toolbarList.insert(name.lower(), p_toolbar);
  
  slotToggleDTDToolbar(!allToolbarsHidden());
 }
}


/** Removes a user toolbar from the toolbars. */
void QuantaApp::slotRemoveToolbar()
{
 QTabWidget *tb = view->toolbarTab;
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
 QString res = QInputDialog::getItem(
                 i18n( "Remove Toolbar" ),
                 i18n( "Please select a toolbar:" ), lst, current, FALSE, &ok, this );

 if (ok)
 {
   removeToolbar(res.lower());
 }

}


/** Sends a toolbar in mail. */
void QuantaApp::slotSendToolbar()
{
  QTabWidget *tb = view->toolbarTab;

  QStringList lst;
  int current = 0;
  for (int i = 0; i < tb->count(); i++)
  {
    lst << tb->label(i);
    if ( tb->tabLabel(tb->currentPage()) == tb->label(i) ) current=i;
  }

  bool ok = FALSE;
  QString res = QInputDialog::getItem(
                  i18n( "Send Toolbar" ),
                  i18n( "Please select a toolbar:" ), lst, current, FALSE, &ok, this );

  if (!ok)
    return;

  QString toolbarName = res.lower();

  QStringList toolbarFile;

  QString prefix="quanta";
  KTempFile* tempFile = new KTempFile(tmpDir, toolbarExtension);;
  tempFile->setAutoDelete(true);
  KURL tempURL;
  tempURL.setPath(tempFile->name());
  saveToolBar(toolbarName, tempURL);
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
      toStr = +mailDlg->lineEmail->text();
      subjectStr = (mailDlg->lineSubject->text().isEmpty())?i18n("Quanta Plus toolbar"):mailDlg->lineSubject->text();
      if ( !mailDlg->titleEdit->text().isEmpty())
        message = mailDlg->titleEdit->text();
    } else
    {
      KMessageBox::error(this,i18n("No destination address was specified./n Sending is aborted."),i18n("Error sending e-mail"));
      delete mailDlg;
      return;
    }

    QString nullString="";
    kapp->invokeMailer(toStr, nullString, nullString, subjectStr, message, nullString, toolbarFile);

  }
  delete mailDlg;
}

/** Ask for save all the modified user toolbars. */
void QuantaApp::saveModifiedToolbars()
{
 QDictIterator<ToolbarEntry> it(toolbarList);
 QString s1, s2;
 ToolbarEntry *p_toolbar;
 for( ; it.current(); ++it )
 {
   p_toolbar = it.current();
   s1 = p_toolbar->dom->toString();
   KXMLGUIClient* client = p_toolbar->guiClient;

   if (client)
   {
    //Rename the _Separator_ tags back to Separator, so they are not treated
    //as changes
     QDomNodeList nodeList = client->domDocument().elementsByTagName("_Separator_");
     for (uint i = 0; i < nodeList.count(); i++)
     {
       nodeList.item(i).toElement().setTagName("Separator");
     }

     s2 = client->domDocument().toString();

     if ( (s1 != s2) && (!s1.isEmpty()) )
     {
       if (KMessageBox::questionYesNo(this, i18n("The toolbar \"%1\" was modified. Do you want to save before it is removed?").arg(it.current()->name),
             i18n("Save Toolbar")) == KMessageBox::Yes)
       {
         bool local = true;
         if (project->hasProject() && p_toolbar->url.url().startsWith(project->baseURL.url())) local = false;
         slotSaveToolbar(local, it.currentKey());
       }
     }
   }
 }
}


/** Reads the DTD info from the file, tries to find the correct DTD and builds the tag/attribute list from the DTD file. */
void QuantaApp::processDTD(QString documentType)
{
 Document *w = view->write();
 QString foundName;
 QString projectDTD = project->defaultDTD();
 w->setDTDIdentifier(projectDTD);
 Tag *tag = 0L;
 if (documentType.isEmpty())
 { 
   foundName = w->findDTDName(&tag); //look up the whole file for DTD definition
   bool found = false;
   if (!foundName.isEmpty())   //!DOCTYPE found in file
   {
    DTDSelectDialog *dlg = new DTDSelectDialog(this);
     QStringList lst;
     QDictIterator<DTDStruct> it(*dtds);
     for( ; it.current(); ++it )
     {
       if (it.current()->family == Xml)
       {
         lst << it.current()->nickName;
       }
     }
     lst.sort();
     QString foundNickName = QuantaCommon::getDTDNickNameFromName(foundName);
     for (uint i = 0; i < lst.count(); i++)
     {
       dlg->dtdCombo->insertItem(lst[i]);
       if (lst[i] == foundNickName)
       {
         w->setDTDIdentifier(foundName);
         found =true;
       }
    }

//    dlg->dtdCombo->insertItem(i18n("Create new DTD info."));
    dlg->messageLabel->setText(i18n("This DTD is not known for Quanta. Choose a DTD or create a new one."));
    dlg->currentDTD->setText(QuantaCommon::getDTDNickNameFromName(foundName));
    QString projectDTDNickName = QuantaCommon::getDTDNickNameFromName(projectDTD);
    for (int i = 0; i < dlg->dtdCombo->count(); i++)
    {
      if (dlg->dtdCombo->text(i) == projectDTDNickName)
      {
        dlg->dtdCombo->setCurrentItem(i);
        break;
      }
    }
    if (!found && dlg->exec())
    {
      w->setDTDIdentifier(QuantaCommon::getDTDNameFromNickName(dlg->dtdCombo->currentText()));
      if (dlg->convertDTD->isChecked())
      {
        QDict<QString> attrDict;
        uint tagCase = qConfig.tagCase;
        qConfig.tagCase = 2; //upper case
        w->changeTag(tag, &attrDict);
        qConfig.tagCase = tagCase;
        uint line, col;
        w->viewCursorIf->cursorPositionReal(&line, &col);
        if (col > 0) w->viewCursorIf->setCursorPositionReal(line, col-1);
        DTDStruct *dtd = dtds->find(w->getDTDIdentifier());
        w->insertText(dtd->doctypeStr);
        delete tag;
      }
    }
    delete dlg;
  } else //DOCTYPE not found in file
  {
    w->setDTDIdentifier(projectDTD);
  }
 } else //dtdName is read from the method's parameter
 {
   w->setDTDIdentifier(documentType);
 }

  loadToolbarForDTD(w->getDTDIdentifier());
  sTab->useOpenLevelSetting = true;
}

/** No descriptions */
void QuantaApp::slotToolsChangeDTD()
{
  if (view->writeExists())
  {
    DTDSelectDialog *dlg = new DTDSelectDialog(this);
    Document *w = view->write();
    int pos = -1;
    int defaultIndex = 0;

    Tag *tag = 0L;
    w->findDTDName(&tag);
    QString oldDtdName = w->getDTDIdentifier();
    QString defaultDocType = project->defaultDTD();
    QDictIterator<DTDStruct> it(*dtds);
    QStringList lst;
    for (; it.current(); ++it)
    {
      if (it.current()->family == Xml)
      {
        lst << it.current()->nickName;
      }
    }
    lst.sort();

    QString oldDtdNickName = QuantaCommon::getDTDNickNameFromName(oldDtdName);
    QString defaultDtdNickName = QuantaCommon::getDTDNickNameFromName(defaultDocType);
    for(uint i = 0; i < lst.count(); i++)
    {
      dlg->dtdCombo->insertItem(lst[i]);
      if (lst[i] == oldDtdNickName) pos = i;
      if (lst[i] == defaultDtdNickName) defaultIndex = i;
    }

    if (pos == -1) pos = defaultIndex;
    dlg->dtdCombo->setCurrentItem(pos);
    dlg->messageLabel->setText(i18n("Change the current DTD."));
    dlg->currentDTD->setText(QuantaCommon::getDTDNickNameFromName(w->getDTDIdentifier()));
    if (dlg->exec())
    {
      w->setDTDIdentifier(QuantaCommon::getDTDNameFromNickName(dlg->dtdCombo->currentText()));
      if (dlg->convertDTD->isChecked())
      {
        if (tag)
        {
          QDict<QString> attrDict;
          uint tagCase = qConfig.tagCase;
          qConfig.tagCase = 2; //upper case
          w->changeTag(tag, &attrDict);
          qConfig.tagCase = tagCase;
          uint line, col;
          w->viewCursorIf->cursorPositionReal(&line, &col);
          if (col > 0) w->viewCursorIf->setCursorPositionReal(line, col-1);
          DTDStruct *dtd = dtds->find(w->getDTDIdentifier());
          w->insertText(dtd->doctypeStr);
          delete tag;
        } else
        {
          w->viewCursorIf->setCursorPositionReal(0,0);
          DTDStruct *dtd = dtds->find(w->getDTDIdentifier());
          w->insertText("<!DOCTYPE" + dtd->doctypeStr + ">\n");
        }
      }
    }

    loadToolbarForDTD(w->getDTDIdentifier());
    reparse(true);

    delete dlg;
  }
}

/** No descriptions */
void QuantaApp::focusInEvent(QFocusEvent* e)
{
 KDockMainWindow::focusInEvent(e);
 Document *w = view->write();
 w->view()->setFocus();
 w->checkDirtyStatus();
}

/** No descriptions */
void QuantaApp::slotShowCompletion()
{
  view->write()->codeCompletionRequested();
}

/** No descriptions */
void QuantaApp::slotShowCompletionHint()
{
  view->write()->codeCompletionHintRequested();
}

/** No descriptions */
void QuantaApp::slotMakeDonation()
{
 DonationDialog *dlg = new DonationDialog(this);
 dlg->exec();
 delete dlg;
}

/** No descriptions */
void QuantaApp::slotHelpHomepage()
{
  kapp->invokeBrowser("http://quanta.sourceforge.net");
}

/** Loads the toolbars for dtd named dtdName and unload the ones belonging to oldDtdName. */
void QuantaApp::loadToolbarForDTD(const QString& dtdName)
{
 DTDStruct *oldDtd = dtds->find(currentToolbarDTD);
 if (!oldDtd && !currentToolbarDTD.isEmpty()) oldDtd = dtds->find(project->defaultDTD());

 QString fileName;
 DTDStruct *newDtd = dtds->find(dtdName);
 if (!newDtd) newDtd = dtds->find(project->defaultDTD());
 if (!newDtd) newDtd = dtds->find(qConfig.defaultDocType); //extreme case

 ToolbarEntry *p_toolbar;
 if (newDtd != oldDtd)
 {
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
       QDictIterator<ToolbarEntry> iter(toolbarList);
       for( ; iter.current(); ++iter )
       {
          p_toolbar = iter.current();
          if (p_toolbar->url == url || p_toolbar->url == urlLocal)
          {
       //     removeToolbar(iter.currentKey());
            factory()->removeClient(p_toolbar->guiClient);
            p_toolbar->visible = false;
            if (p_toolbar->menu) delete p_toolbar->menu;
            p_toolbar->menu = 0L;
            break;          
          }         
       }
     }
   }

   //Load the toolbars for dtdName   
   for (uint i = 0; i < newDtd->toolbars.count(); i++)
   {
      KURL url;
      //first load the local version if it exists
      fileName = locateLocal("data", "quanta/toolbars/"+newDtd->toolbars[i]);
      QuantaCommon::setUrl(url, fileName);
      if (QExtFileInfo::exists(url))
      {
        showToolbarFile(url);
      } else
      {
        fileName = qConfig.globalDataDir + "quanta/toolbars/"+newDtd->toolbars[i];
        QuantaCommon::setUrl(url, fileName);
        if (QExtFileInfo::exists(url))
        {
          showToolbarFile(url);
        }
      }
   }

   view->toolbarTab->setCurrentPage(0);
 }

 currentToolbarDTD = newDtd->name;
 slotToggleDTDToolbar(!allToolbarsHidden());
}

/** Remove the toolbar named "name". */
void QuantaApp::removeToolbar(const QString& name)
{
  ToolbarEntry *p_toolbar = toolbarList[name];
  if (p_toolbar)
  {
    KXMLGUIClient* toolbarGUI = p_toolbar->guiClient;

    if (toolbarGUI)
    {
    //Rename the _Separator_ tags back to Separator, so they are not treated
    //as changes
     QDomNodeList nodeList = toolbarGUI->domDocument().elementsByTagName("_Separator_");
     for (uint i = 0; i < nodeList.count(); i++)
     {
       nodeList.item(i).toElement().setTagName("Separator");
     }

     //check if the toolbar's XML GUI was modified or not
     QString s1 = p_toolbar->dom->toString();
     kdDebug(24000) << s1 << endl;
     QString s2 = toolbarGUI->domDocument().toString();
     kdDebug(24000) << s2 << endl;
     if ( s1 != s2 )
     {
      if (KMessageBox::questionYesNo(this, i18n("The toolbar \"%1\" was modified. Do you want to save before remove?").arg(name),
              i18n("Save Toolbar")) == KMessageBox::Yes)
      {
        bool local = true;
        if (project->hasProject() && p_toolbar->url.url().startsWith(project->baseURL.url())) local = false;
        slotSaveToolbar(local, name);
      }
     }

     guiFactory()->removeClient(toolbarGUI);
     if (p_toolbar->dom) delete p_toolbar->dom;
     if (p_toolbar->guiClient) delete p_toolbar->guiClient;
     if (p_toolbar->menu) delete p_toolbar->menu;
     toolbarList.remove(name);
    }
  }

  slotToggleDTDToolbar(!allToolbarsHidden());
}

/** Show or hide the DTD toolbar */
void QuantaApp::slotToggleDTDToolbar(bool show)
{
  if (show)
  {
    view->toolbarTab->show();
  } else
  {
    view->toolbarTab->hide();
  }
  qConfig.enableDTDToolbar = show;
}


/** No descriptions */
void QuantaApp::slotParsingDTDChanged(QString newDTDName)
{
  if (view->writeExists())
  {
    view->write()->setParsingDTD(newDTDName);
    reparse(false);
  }
}

/** Returns the project's base URL if it exists, the HOME dir if there is no project and no opened document (or the current opened document was not saved yet), and the base URL of the opened document, if it is saved somewhere. */
KURL QuantaApp::projectBaseURL()
{
  KURL result;
  if  ( project->hasProject())
  {
     result = project->baseURL;
  } else
  {
    if  ( !view->writeExists() || view->write()->isUntitled() )
    {
      result = QExtFileInfo::home();
    } else
    {
      result = QExtFileInfo::path(view->write()->url());
    }
  }
  return result;
}

/** No descriptions */
void QuantaApp::slotBuildPrjToolbarsMenu()
{
    if (project && project->hasProject())
    {
      KURL::List toolbarList = QExtFileInfo::allFiles(project->toolbarURL, "*"+toolbarExtension);
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
  if (project && project->hasProject())
  {
    encoding = project->defaultEncoding();
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
bool QuantaApp::allToolbarsHidden()
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
void QuantaApp::slotEmailDTD()
{
  if (view->writeExists())
  {
    Document *w = view->write();
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
    QString res = QInputDialog::getItem(
                    i18n( "Send DTD" ),
                    i18n( "Please select a DTD:" ), lst, current, FALSE, &ok, this );

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
      toStr = +mailDlg->lineEmail->text();
      subjectStr = (mailDlg->lineSubject->text().isEmpty())?i18n("Quanta Plus DTD"):mailDlg->lineSubject->text();
      if ( !mailDlg->titleEdit->text().isEmpty())
          message = mailDlg->titleEdit->text();
    } else
    {
      KMessageBox::error(this,i18n("No destination address was specified./n Sending is aborted."),i18n("Error sending e-mail"));
      delete mailDlg;
      return;
    }

    QString nullString="";
    kapp->invokeMailer(toStr, nullString, nullString, subjectStr, message, nullString, dtdFile);
    }
    delete mailDlg;
  }
}

/** Returns the interface number for the currently active editor. */
int QuantaApp::currentEditorIfNum() const
{
  if (view->writeExists())
  {
    return view->write()->editIf->editInterfaceNumber();
  } else
  {
    return view->oldWrite->editIf->editInterfaceNumber();
  }
}

/** Return the URL of the currently active document */
QString QuantaApp::currentURL() const
{
  if (view->writeExists())
  {
    return view->write()->url().url();
  } else
  {
    return view->oldWrite->url().url();
  }
}


#include "quanta.moc"


