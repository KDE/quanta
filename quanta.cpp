/***************************************************************************
                          quanta.cpp  -  description
                             -------------------
    begin                : ?? ???  9 13:29:57 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
    								  (C) 2001 by Andras Mantia
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
#include <kstddirs.h>
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

#include <kparts/componentfactory.h>

#include <ktexteditor/editinterface.h>
#include <ktexteditor/selectioninterface.h>
#include <ktexteditor/viewcursorinterface.h>

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
#include "dialogs/grepdialog.h"
#include "dialogs/katefiledialog.h"
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

//  slotUpdateStatus(view->write());
}

void QuantaApp::slotFileOpen()
{
//  KURL url = KFileDialog::getOpenURL( QString::null, QString::null, this);
 QString myEncoding = QString::fromLatin1(QTextCodec::codecForName(qConfig.defaultEncoding.latin1())->name());

 KateFileDialog *dialog = new KateFileDialog (QString::null,myEncoding, this, i18n ("Open File"));
 KateFileDialogData data = dialog->exec();
 delete dialog;

//  if ( !url.url().isEmpty() ) slotFileOpen( url );
 for (KURL::List::Iterator i=data.urls.begin(); i != data.urls.end(); ++i)
 {
    slotFileOpen( *i , data.encoding);
//    view->write()->doc()->setEncoding(data.encoding);
 }

  slotUpdateStatus(view->write());
}

void QuantaApp::slotFileOpen( const KURL &url, const QString& encoding )
{
  doc->openDocument( url, encoding );
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
}

void QuantaApp::slotFileSave()
{
  Document *w = view->write();
  w->checkDirtyStatus();
  if ( w->isUntitled() )
  	slotFileSaveAs();
  else
  	doc->saveDocument( doc->url() );

  slotUpdateStatus(w);
}

void QuantaApp::slotFileSaveAs()
{
  QString oldUrl = doc->url().url();
  Document *w = view->write();
  w->checkDirtyStatus();
  fileWatcher->stopScan();
  if (w->kate_view->saveAs() == Kate::View::SAVE_OK)
  {
    w->createTempFile();

    KURL url = w->doc()->url();

    if ( ( project->hasProject() ) &&
         ( KMessageBox::Yes == KMessageBox::questionYesNo(0,"Add file\n " +url.url()+"\n to project ? ")) )
    {
      project->insertFile(url.url(),true);
    }

    if ( oldUrl != url.url() ) doc->changeFileTabName( oldUrl );

    slotUpdateStatus(w);
  }
  fileWatcher->startScan();
}

void QuantaApp::saveAsTemplate(bool projectTemplate,bool selectionOnly)
{
  KURL url;
  int query;
  QString projectTemplateDir;
  Document *w = view->write();
  w->checkDirtyStatus();

  do {
    query = KMessageBox::Yes;

    if (projectTemplate)
    {
  	  url = KFileDialog::getSaveURL(project->templateDir, QString::null, this);
  	} else
  	{
  	  url = KFileDialog::getSaveURL(locateLocal("data","quanta/templates/"), QString::null, this);
  	}

    if (url.isEmpty()) return;

    if ( project->hasProject() )  projectTemplateDir = project->templateDir;
    if ( ((projectTemplate) && (KURL(projectTemplateDir).isParentOf(url)) ) ||
          ((! projectTemplate) && (KURL(locateLocal("data","quanta/templates/")).isParentOf(url))) )
    {
    	query = w->checkOverwrite( url );
    } else
    {
      KMessageBox::sorry(0,i18n("You must save the templates in one of the following directories: \n\n%1\n%2")
      											 .arg(locateLocal("data","quanta/templates/")).arg(projectTemplateDir));
      query = KMessageBox::No;
    }
  } while (query != KMessageBox::Yes);

  if( query == KMessageBox::Cancel ) return;

  QString fileName;
  QString content;
  if (selectionOnly)
  {
    content = w->selectionIf->selection();

  } else
  {
    content = w->editIf->text();
  }
  
  fileName = url.directory(false)+url.fileName();
  QFile templateFile(fileName);
  templateFile.open(IO_WriteOnly);
  QTextStream stream(&templateFile);
  stream << content;
  templateFile.flush();
  templateFile.close();

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
  doc->closeDocument();

  htmlPart()->closeURL();
  htmlPart()->begin( KURL( doc->basePath() ));
  htmlPart()->write( "" );
 	htmlPart()->end();

  slotUpdateStatus(view->write());
}

void QuantaApp::slotFileCloseAll()
{
  doc->closeAll();

  htmlPart()->closeURL();
  htmlPart()->begin( KURL( doc->basePath() ));
 	htmlPart()->write( "" );
 	htmlPart()->end();

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
 view->write()->kate_doc->printDialog();
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
  if (!grepDialog) {
      grepDialog = new GrepDialog( QDir::homeDirPath(), this, "grep_dialog" );
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


void QuantaApp::slotStatusMsg(const QString &msg)
{
  statusbarTimer->stop();
  statusBar()   ->changeItem(" " + msg, IDS_STATUS);
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

  QString url;
  Document *w = view->write();
  if (!w->isUntitled())
  {
//if it's	not untitled, than it was loaded from somewhere. In this case show it from that place
	  url = w->url().url();

	  if ( doc->isModified() ) //doc->saveDocument( doc->url() );
         w->saveIt();

	  url = project->urlWithPrefix(w->url());

	  part->begin(url, xOffset, yOffset );
	  part->openURL( KURL(url) );
  } else  //the document is Untitled, preview the text from it
 {
  	QString text = w->editIf->text();
    if ( text == oldtext ) return;
  	if ( text.isEmpty() )
  	{
    	text = i18n( "<center><h3>The current document is empty...</h3></center>" );
  	}
    oldtext = text;
	  part->begin( KURL( doc->basePath() ), xOffset, yOffset );
    part->write( text );
 }
 part->end();
}

/** view image in preview */
void QuantaApp::slotImageOpen(QString url)
{
  if ( previewPosition == "Disabled" )
     return;

  WHTMLPart *part = htmlPart();
  QWidgetStack *s = widgetStackOfHtmlPart();

  if ( !s ) return;
  if ( !part ) return;

  if ( !s->id( s ->visibleWidget()) ) return;

    KToggleAction *ta = (KToggleAction *) actionCollection()->action( "show_preview" );
	ta->setChecked(true);

	QString text = "<html>\n<body>\n<div align=\"center\">\n<img src=\"";
	text += url;
	text += "\">\n</div>\n</body>\n</html>\n";

	part->closeURL();
	part->begin( KURL( doc->basePath() ) );
  part->write( text.data() );
  part->end();

  part->show();
}


/** insert <img> tag for images or <a> for other */
void QuantaApp::slotInsertTag(QString url, DirInfo dirInfo)
{
 	QImage img(url);

  QString furl = QExtFileInfo::toRelative( url, doc->basePath() );
  Document *w = view->write();

  if (!(dirInfo.preText.isEmpty()) || !(dirInfo.postText.isEmpty()))
  {
	   w->insertTag(dirInfo.preText+furl+dirInfo.postText);
  } else
  {

   if ( !img.isNull() )
   {
     QString width,height;
     width.setNum( img.width () );
	   height.setNum( img.height() );

 	   w->insertTag("<img src=\""+furl+"\" width=\""+width+"\" height=\""+height+"\" border=\"0\">");
   }
   else
     w->insertTag( "<a href=\""+furl+"\">","</a>");
  }

  w->view()->setFocus();
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
    setTitle( doc->url().prettyURL() );

  /*  int  config   = w->config();
    bool readOnly = w->isReadOnly();

    if (readOnly) statusBar()->changeItem(i18n(" R/O "),IDS_INS_OVR);
    else          statusBar()->changeItem(config & KWriteView::cfOvr ? i18n(" OVR ") : i18n(" INS "),IDS_INS_OVR);
                 */
    statusBar()->changeItem(w->isModified() ? " * " : "",IDS_MODIFIED);

    saveAction   ->setEnabled(doc->isModified());
    saveAllAction->setEnabled(doc->isModifiedAll());
    saveprjAction     ->setEnabled(project->isModified());

    bool projectExists = project->hasProject();
    closeprjAction     ->setEnabled(projectExists);
    insertFileAction   ->setEnabled(projectExists);
    insertDirAction    ->setEnabled(projectExists);
    rescanPrjDirAction ->setEnabled(projectExists);
    uploadProjectAction->setEnabled(projectExists);
    projectOptionAction->setEnabled(projectExists);
    saveAsProjectTemplateAction->setEnabled(projectExists);
    saveSelectionAsProjectTemplateAction->setEnabled(projectExists);

    if (projectExists)
    {
      QStringList toolbarList = QExtFileInfo::allFiles(project->toolbarDir+"/", "*.toolbar.tgz");
      projectToolbarFiles->setMaxItems(toolbarList.count());
      for (uint i = 0; i < toolbarList.count(); i++)
      {
        if (QFileInfo(toolbarList[i]).isFile())
          projectToolbarFiles->addURL(KURL(toolbarList[i]));
      }
    } else
    {
      projectToolbarFiles->clearURLList();
    }
    actionCollection()->action("toolbars_load_project")->setEnabled(projectExists);
    actionCollection()->action("toolbars_save_project")->setEnabled(projectExists);

    viewBorder->setChecked(w->kate_view->iconBorder());
    viewLineNumbers->setChecked(w->kate_view->lineNumbersOn());

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
/*!!!!	int state = view->write()->undoState();

  undoAction->setEnabled(state & 1);
  redoAction->setEnabled(state & 2);*/
}

void QuantaApp::slotUpdateStatus(QWidget* w)
{
  Document *newWrite = dynamic_cast<Document *>(w);
  newWrite->checkDirtyStatus();
  if (newWrite != view->oldWrite)
    sTab->useOpenLevelSetting = true;
  reparse();
	slotNewUndo();
	slotNewStatus();
	slotNewLineColumn();

  loadToolbarForDTD(newWrite->getDTDIdentifier());

//Add the Kate menus
/*
  if (view->oldWrite != 0L)
  {
    view->oldWrite->writeConfig(config);
    guiFactory()->removeClient(view->oldWrite->view());
  }
  guiFactory()->addClient(dynamic_cast<Document *>(w)->view());
*/

  newWrite->readConfig(config);

  Document *currentWrite = view->write();
  currentWrite->view()->resize(view->writeTab->size().width()-5, view->writeTab->size().height()-35);
  view->oldWrite = currentWrite;
  currentWrite->view()->setFocus();


  emit reloadTreeviews();
}

void QuantaApp::slotOptionsConfigureKeys()
{
	KKeyDialog::configureKeys( actionCollection(), xmlFile(), true, this );
}

void QuantaApp::slotOptionsConfigureToolbars()
{
 int currentPageIndex = view->toolbarTab->currentPageIndex();

 //clear all the actions - this is also to avoid duplicate actions in the list

 QDictIterator<KXMLGUIClient> iter(toolbarGUIClientList);
 QDomNodeList nodeList;
 KAction *action;
 QPopupMenu *menu;
 for( ; iter.current(); ++iter )
 {
   int actionCount = iter.current()->actionCollection()->count();
   for (int i = 0; i < actionCount; i++)
   {
    action = iter.current()->actionCollection()->action(0);
    iter.current()->actionCollection()->take(action);
   }
 }

 KEditToolbar dlg(factory(), this);

 int result = dlg.exec();

 menuBar()->removeItem(m_tagsMenuId);
 QString actionName;
 QString name;
// QDictIterator<KXMLGUIClient> it(toolbarGUIClientList);
 //for( ; it.current(); ++it )
 KXMLGUIClient *guiClient = 0;
 QPtrList<KXMLGUIClient> guiClients = factory()->clients();
 for (uint i = 0; i < guiClients.count(); i++)
 {
    guiClient = guiClients.at(i);
    if (result == QDialog::Accepted)
    {
      nodeList = guiClient->domDocument().elementsByTagName("ToolBar");
      name = nodeList.item(0).toElement().attribute("tabname");
      toolbarMenuList.remove(name.lower());
      menu = new QPopupMenu;
      //remove all inserted toolbars
      factory()->removeClient(guiClient);
    }
    //plug the actions in again
    nodeList = guiClient->domDocument().elementsByTagName("Action");
    for (uint j = 0; j < nodeList.count(); j++)
    {
      actionName = nodeList.item(j).toElement().attribute("name");
      action = actionCollection()->action(actionName);
      if (action)
      {
        guiClient->actionCollection()->insert(action);
        action->plug(menu);
      }
    }
    //and add them again. Is there a better way to do this?
    if (result == QDialog::Accepted)
    {
      guiFactory()->addClient(guiClient);
      if (!name.isEmpty())
      {
        m_tagsMenu->insertItem(name,menu);
        toolbarMenuList.insert(name.lower(),menu);
      } else
      {
        delete menu;
      }
    }
 }

 m_tagsMenuId = menuBar()->insertItem(i18n("&Tags"),m_tagsMenu,-1,5);
 view->toolbarTab->setCurrentPage(currentPageIndex);
}

void QuantaApp::slotOptionsConfigureActions()
{
 int currentPageIndex = view->toolbarTab->currentPageIndex();

 ActionEditDlg dlg( this, this, "actions_edit_dlg", true); //actionCollection(), QString::null, true, this );

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

  // Environment options
  //TODO FileMasks name is not good anymore
  page=kd->addVBoxPage(i18n("Environment"), QString::null, BarIcon("files", KIcon::SizeMedium ) );
  FileMasks *fileMasks = new FileMasks( (QWidget *)page );

  fileMasks->lineHTML->setText( fileMaskHtml );
  fileMasks->linePHP->setText( fileMaskPhp );
  fileMasks->lineImages->setText( fileMaskImage );
  fileMasks->lineText->setText( fileMaskText );

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
  fileMasks->hideDTDToolbar->setChecked(!qConfig.enableDTDToolbar);

  // Preview options
  page=kd->addVBoxPage(i18n("Preview"), QString::null, BarIcon("kview", KIcon::SizeMedium ) );
  PreviewOptions *previewOptions = new PreviewOptions( (QWidget *)page );

  previewOptions->setPosition( previewPosition );

  page=kd->addVBoxPage(i18n("Parser"), QString::null, BarIcon("kcmsystem", KIcon::SizeMedium ) );
  ParserOptions *parserOptions = new ParserOptions( config, (QWidget *)page );
  QDictIterator<DTDStruct> it(*dtds);
  for( ; it.current(); ++it )
  {
    if (it.current()->family == Xml)
    {
      int index = -1;
      if (it.current()->name == qConfig.defaultDocType) index = 0;
      parserOptions->dtdName->insertItem(QuantaCommon::getDTDNickNameFromName(it.current()->name), index);
    }
  }

  parserOptions->refreshFrequency->setValue(qConfig.refreshFrequency);
  parserOptions->useMimeTypes->setChecked(qConfig.useMimeTypes);
  page=kd->addVBoxPage(i18n("PHP Debug"), QString::null, BarIcon("gear", KIcon::SizeMedium ) );
  DebuggerOptionsS *debuggerOptions = new DebuggerOptionsS( (QWidget *)page );

  if (debuggerStyle=="PHP3") debuggerOptions->radioPhp3->setChecked(true);
  if (debuggerStyle=="None") debuggerOptions->checkDebugger->setChecked(false);

  if ( kd->exec() )
  {
    qConfig.tagCase = styleOptionsS->tagCase->currentItem();
    qConfig.attrCase = styleOptionsS->attributeCase->currentItem();
    qConfig.closeTags = styleOptionsS->tagAutoClose->isChecked();
    qConfig.closeOptionalTags = styleOptionsS->optionalTagAutoClose->isChecked();
    qConfig.useAutoCompletion = styleOptionsS->useAutoCompletion->isChecked();

    fileMaskHtml = fileMasks->lineHTML->text()+" ";
  	fileMaskPhp  = fileMasks->linePHP->text()+" ";
	  fileMaskImage= fileMasks->lineImages->text()+" ";
  	fileMaskText = fileMasks->lineText->text()+" ";
    qConfig.defaultEncoding = fileMasks->encodingCombo->currentText();
    qConfig.enableDTDToolbar = !fileMasks->hideDTDToolbar->isChecked();
    showDTDToolbar->setEnabled(qConfig.enableDTDToolbar);
    slotToggleDTDToolbar(showDTDToolbar->isChecked());

    qConfig.refreshFrequency = parserOptions->refreshFrequency->value();
    refreshTimer->changeInterval(qConfig.refreshFrequency*1000);
    qConfig.useMimeTypes = parserOptions->useMimeTypes->isChecked();

    parserOptions->updateConfig();
    qConfig.defaultDocType = QuantaCommon::getDTDNameFromNickName(parserOptions->dtdName->currentText());

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

  	previewPosition = previewOptions->position();

  	htmlpart->closeURL();
  	htmlpart->begin( KURL( doc->basePath() ));
  	htmlpart->write( "" );
  	htmlpart->end();

  	repaintPreview(true);
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
/*
	enableCommand(ID_VIEW_BACK);
  enableCommand(ID_VIEW_FORWARD);
	enableCommand(ID_VIEW_REPAINT);
*/
	s->raiseWidget( 1 );

//	checkCommand( ID_VIEW_PREVIEW, true );
}

void QuantaApp::slotShowPreview()
{
	WHTMLPart *part = htmlPart();
 	QWidgetStack *s = widgetStackOfHtmlPart();
  Document *w = view->write();

	if ( !s ) return;
	if ( !part ) return;


  KToggleAction *ta = (KToggleAction *) actionCollection()->action( "show_preview" );
	bool stat = !ta->isChecked();

	if ( stat ) {
//		disableCommand(ID_VIEW_BACK);
//		disableCommand(ID_VIEW_FORWARD);
//		disableCommand(ID_VIEW_REPAINT);

		s   ->raiseWidget( 0 );

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
	  w->view()->setFocus();
	}
	else {
//		enableCommand(ID_VIEW_BACK);
//		enableCommand(ID_VIEW_FORWARD);
//		enableCommand(ID_VIEW_REPAINT);

	  if ( previewPosition == "Bottom" )
	  {
	  }

		s->raiseWidget( 1 );
		repaintPreview(false);
	}
//	checkCommand( ID_VIEW_PREVIEW, !stat );

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
  unsigned int line;
  unsigned int col;

  view->write()->viewCursorIf->cursorPositionReal(&line, &col);
  sTab->showTagAtPos(line,col);
  linenumber.sprintf(i18n("Line: %d Col: %d"),line+1,col+1);

  statusBar()->changeItem(linenumber.data(), IDS_STATUS_CLM);
}

/** reparse current document and initialize node. */
void QuantaApp::reparse()
{
//TODO: This function is very ugly...
  if (view->writeExists())view->write()->parseVariables();
  if (view->writeExists() && stabdock->isVisible())
  {
    Document *w = view->write();
    QString s = w->editIf->text();
    if (s != parser->m_text)  //rebuild the node tree only if the text has changed
    {
      if (baseNode)
      {
        delete baseNode;
        baseNode = 0L;
      }
      if (dtds->find(w->getDTDIdentifier()))
      {
        baseNode = parser->parse(w);
        sTab->setParsingDTD(w->parsingDTD());
      }
    }
    sTab->deleteList();
	  if (baseNode)
	  {
      config->setGroup("Parser options");
      int expandLevel = config->readNumEntry("Expand level",8);
      if ( expandLevel == 0 ) expandLevel = 40;
      sTab->slotReparse(baseNode , expandLevel );

      uint line;
      uint col;
      w->viewCursorIf->cursorPositionReal(&line, &col);
      sTab->showTagAtPos(line,col);
	  } // if (stabdock->isVisible())
  } // if (view->writeExists())
}

void QuantaApp::setCursorPosition( int row, int col )
{
  Document *w = view->write();

  int numLines = w->editIf->numLines();

  if ( row < numLines )
    w->viewCursorIf->setCursorPositionReal(row, col);
  else
    w->viewCursorIf->setCursorPositionReal(numLines - 1, col);

  w->view()->setFocus();
}

void QuantaApp::gotoFileAndLine(QString filename, int line )
{
  if ( !filename.isEmpty() ) doc->openDocument( filename );

  Document *w = view->write();
  int numLines = w->editIf->numLines();
  if ( numLines > line && line >= 0 )
  {
    w->viewCursorIf->setCursorPositionReal(line, 0);
  }

  w->view()->setFocus();
}


//void QuantaApp::slotLeftTabChanged(QWidget *)
void QuantaApp::slotDockChanged()
{
  static bool docTabOpened = false;

//  if ( stabdock->isVisible() && baseNode) reparse();

  if ( dtabdock->isVisible() )
  {
    static bool first = true;
  	rightWidgetStack -> raiseWidget(2);
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
    	rightWidgetStack -> raiseWidget(0);
 	    docTabOpened = false;
    }
    if ( !exitingFlag )
    {
      if (view->writeExists()) doc ->write()->setFocus();
    }
  }
}

void QuantaApp::selectArea(int line1, int col1, int line2, int col2)
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

void QuantaApp::contextHelp()
{
  int id_w = rightWidgetStack->id( rightWidgetStack->visibleWidget());

  if (  id_w == 1 || id_w == 2 )
  {
    rightWidgetStack->raiseWidget(0);
    view->write()->view()->setFocus();
  }
  else
  {
    QString currentW =view->write()->kate_view->currentWord();
    QString *url = dTab->contextHelp( currentW);

    if ( url )
    {
      if ( !dtabdock->isVisible() ) dtabdock->changeHideShowState();

      rightWidgetStack->raiseWidget(2);
      htmlPartDoc->view()->setFocus();

  		openDoc(*url);
    }
  }
}

void QuantaApp::slotShowLeftDock() {  }
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
  showFTabAction    ->setChecked( ftabdock->isVisible() );
  showPTabAction    ->setChecked( ptabdock->isVisible() );
  showTTabAction    ->setChecked( ttabdock->isVisible() );
  showSTabAction    ->setChecked( stabdock->isVisible() );
  showDTabAction    ->setChecked( dtabdock->isVisible() );
}

void QuantaApp::slotToolSyntaxCheck()
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

QWidget* QuantaApp::createContainer( QWidget *parent, int index, const QDomElement &element, int &id )
{

  QString tabname = element.attribute( "tabname", "" );

  if ( element.tagName().lower() == "toolbar" && !tabname.isEmpty() ) {
//avoid QToolBar warning in the log
    QtMsgHandler oldHandler = qInstallMsgHandler( silenceQToolBar );
/*
    QDockArea *dockArea = new QDockArea(Qt::Horizontal, QDockArea::Normal, view->toolbarTab);
    KToolBar *tb = new KToolBar( dockArea,0,true,true);
*/
    KToolBar *tb = new KToolBar(view->toolbarTab, 0, true, true);
    tb->loadState(element);
    tb->enableMoving(false);
    tb->setEnableContextMenu(true);
/*
    tb->undock();
    dockArea->moveDockWindow(tb);
    view->toolbarTab->addTab(dockArea, i18n(tabname));
*/
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
  if ( rightWidgetStack->id( rightWidgetStack->visibleWidget()) == 0 ) slotFilePrev();
  if ( rightWidgetStack->id( rightWidgetStack->visibleWidget()) == 1 ) htmlpart->back();
  if ( rightWidgetStack->id( rightWidgetStack->visibleWidget()) == 2 ) htmlPartDoc->back();
}

void QuantaApp::slotForward()
{
  if ( rightWidgetStack->id( rightWidgetStack->visibleWidget()) == 0 ) slotFileNext();
  if ( rightWidgetStack->id( rightWidgetStack->visibleWidget()) == 1 ) htmlpart->forward();
  if ( rightWidgetStack->id( rightWidgetStack->visibleWidget()) == 2 ) htmlPartDoc->forward();
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
  QStringList fileList;
  QStringList openList;
  QString tmpString;
  fileList = view->doc->openedFiles(false);

//A little trick. I fill in the list in reversed order.
  for (int i = fileList.count()-1;  i >=0 ; i--)
  {
     tmpString = KURL(fileList[i]).prettyURL();
     if (tmpString.left(5) == "file:") tmpString.remove(0,5);
     if (tmpString.left(1) == "/")
    {
       	tmpString = QExtFileInfo::toRelative(tmpString,doc->basePath());
       	if (tmpString.contains("../"))
       	{
        	tmpString.replace(QRegExp("\\.\\./"),"");
        	tmpString = ".../"+tmpString;
       	}
     }
  	openList.append(tmpString);
  }

  ListDlg listDlg(openList);
  if (!listDlg.exec()) return;

//This "complex" read-out is due to the reversed list.
  QString docName= fileList[openList.count() - listDlg.getEntryNum() - 1];

  view->writeTab->showPage(doc->docList()->find(docName));
}

/** No descriptions */
void QuantaApp::slotNewProjectLoaded()
{
  delete tTab;
  tTab = new TemplatesTreeView( project->basePath, ttabdock );
  ttabdock ->setWidget( tTab );
  tTab  ->setFocusPolicy(QWidget::NoFocus);
  connect(   tTab, SIGNAL(openFile  (const KURL &, const QString&)),
            this, SLOT(slotFileOpen(const KURL &, const QString&)));
  connect(   tTab, SIGNAL(insertFile  (QString)),
            this, SLOT(slotInsertFile(QString)));
  connect(   tTab,SIGNAL(insertTag(QString, DirInfo)),
            this, SLOT(slotInsertTag(QString, DirInfo)));
  connect(this, SIGNAL(reloadTreeviews()), tTab, SLOT (slotReload()));
  connect(fTTab, SIGNAL(reloadTreeviews()), tTab, SLOT (slotReload()));
  connect(tTab, SIGNAL(reloadTreeviews()), fTTab, SLOT (slotReload()));
  connect(tTab, SIGNAL(reloadTreeviews()), tTab, SLOT (slotReload()));

  connect(getView(), SIGNAL(dragInsert(QDropEvent *)), tTab, SLOT(slotDragInsert(QDropEvent *))); // [MB02]
  connect(project, SIGNAL(templateDirChanged(const QString &)), tTab, SLOT(slotSetTemplateDir(const QString &)));
  tTab->slotSetTemplateDir(project->templateDir);
}

/** No descriptions */
void QuantaApp::slotInsertFile(QString fileName)
{
  view->write()->insertFile(fileName);
}


//Kate releated
void QuantaApp::setEOLMenuAboutToShow()
{
  int eol = view->write()->kate_view->getEol();
  eol = eol>=0? eol: 0;
  setEndOfLine->setCurrentItem( eol );
}

void QuantaApp::bookmarkMenuAboutToShow()
{
  pm_bookmark->clear ();
  bookmarkToggle->plug (pm_bookmark);
  bookmarkClear->plug (pm_bookmark);

  Document *w = view->write();
  markList = w->kate_doc->marks();
//Based on Kate code
  bool hassep = false;
  for (int i=0; (uint) i < markList.count(); i++)
  {
    if (markList.at(i)->type&Kate::Document::markType01)
    {
      if (!hassep) {
        pm_bookmark->insertSeparator ();
        hassep = true;
      }
      QString bText = w->kate_doc->textLine(markList.at(i)->line);
      bText.truncate(32);
      bText.append ("...");
      pm_bookmark->insertItem ( QString("%1 - \"%2\"").arg(markList.at(i)->line).arg(bText),
                                 this, SLOT (gotoBookmark(int)), 0, i );
    }
  }
}

void QuantaApp::gotoBookmark (int n)
{
  view->gotoMark (markList.at(n));
}

/** No descriptions */
void QuantaApp::slotSyntaxCheckDone()
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

/** Load an user toolbar file from the disk. */
void QuantaApp::slotLoadToolbarFile(const KURL& url)
{
 QDomDocument actionDom;
 QDomDocument *toolbarDom = new QDomDocument();

 QTextStream str;
 QString fileName = url.path();

 if ( (fileName.find(".toolbar.tgz") != -1) )
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

 } else //to support loading of separate .toolbar, .action files
 {
   //Load the actions from the associated .action file
   QString actionFile;
   if (QFileInfo(fileName).extension(false) != "toolbar")
   {
     actionFile = fileName + ".actions";
   }else
   {
     actionFile = QFileInfo(fileName).dirPath() + "/"+QFileInfo(fileName).baseName()+".actions";
   }
   QFile f(fileName);
   if ( f.open( IO_ReadOnly ) )
   {
     toolbarDom->setContent(&f);
   }
   f.close();

   f.setName(actionFile);
   if ( f.open( IO_ReadOnly ) )
   {
      actionDom.setContent(&f);
   }
   f.close();
 }


 QDomNodeList nodeList = toolbarDom->elementsByTagName("ToolBar");
 QString name = nodeList.item(0).toElement().attribute("tabname");

//search for another toolbar with the same name
 QPtrList<KXMLGUIClient> xml_clients = factory()->clients();
 bool found = false;
 for (uint index = 0; index < xml_clients.count(); index++)
 {
   nodeList = xml_clients.at(index)->domDocument().elementsByTagName("ToolBar");
   for (uint i = 0; i < nodeList.count(); i++)
   {
    if ((nodeList.item(i).toElement().attribute("name") ) == name.lower())
    {
      QString newName;
      do
      {
        KMessageBox::information(this,i18n("A toolbar with the same name already exists.\n Please rename the loaded toolbar."),i18n("Name conflict"));

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

 QDomDocument *dom = new QDomDocument();
 dom->setContent(toolbarDom->toString());
 toolbarDomList.insert(name.lower(), dom);

//setup the actions
 nodeList = actionDom.elementsByTagName("action");
 for (uint i = 0; i < nodeList.count(); i++)
 {
   QDomElement el = nodeList.item(i).toElement();
   QString actionName = el.attribute("name");
   //if there is no such action yet, add to the available actions
   if (! actionCollection()->action(actionName))
   {
     m_actions->firstChild().appendChild(el);
     TagAction *a = new TagAction(&el, view, actionCollection() );
     actionCollection()->insert(a);
   }
 }

 userToolbarsCount++;

//Change the name also in the XML File -> create a temp XML file
 KTempFile* tempFile = new KTempFile();
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

//Plug in the actions & build the menu
 QPopupMenu *menu = new QPopupMenu;
 KAction *action;
 nodeList = toolbarGUI->domDocument().elementsByTagName("Action");
 for (uint i = 0; i < nodeList.count(); i++)
 {
    action = actionCollection()->action(nodeList.item(i).toElement().attribute("name") );
    if (action)
    {
      toolbarGUI->actionCollection()->insert(action);
      action->plug(menu);
    }
 }
 m_tagsMenu->insertItem(name,menu);
 toolbarMenuList.insert(name.lower(),menu);

 factory()->addClient(toolbarGUI);

 view->toolbarTab->setCurrentPage(view->toolbarTab->count()-1);

 tempFileList.append(tempFile);
 toolbarGUIClientList.insert(name.lower(),toolbarGUI);
 QString *pstr = new QString();
 pstr->append(name.lower());
 toolbarNames.insert(url.prettyURL(),pstr);
 toolbarURLs.insert(name.lower(), new KURL(url));

 slotToggleDTDToolbar(toolbarNames.count() != 0);
}

/** Load an user toolbar from the disk. */
void QuantaApp::slotLoadToolbar()
{
 KURL url;

 url = KFileDialog::getOpenURL(locateLocal("data","quanta/toolbars/"), "*.toolbar.tgz\n*", this);
 if (! url.isEmpty())
 {
   slotLoadToolbarFile(url.path());
 }

}

/** Load an user toolbar from the disk. */
void QuantaApp::slotLoadGlobalToolbar()
{
 KURL url;

 url = KFileDialog::getOpenURL(qConfig.globalDataDir +"quanta/toolbars/", "*.toolbar.tgz\n*", this);
 if (! url.isEmpty())
 {
   slotLoadToolbarFile(url.path());
 }

}

QString QuantaApp::saveToolBar(QString& toolbarName, QString destFile)
{

  QString tarFile = destFile;

  if (QFileInfo(destFile).extension() != "toolbar.tgz")
  {
   tarFile = destFile + ".toolbar.tgz";
  }

  QBuffer buffer;
  buffer.open(IO_WriteOnly);
  QTextStream toolStr(&buffer);

  QBuffer buffer2;
  buffer2.open(IO_WriteOnly);
  QTextStream actStr(&buffer2);

  QDomNodeList nodeList, nodeList2;
  QStringList actionNameList;

/*
  QFile f( KGlobal::instance()->dirs()->saveLocation("data")+"quanta/actions.rc" );
  f.open( IO_ReadWrite | IO_Truncate );
  QTextStream qts(&f);
  m_actions->save(qts,0);
  f.close();
  QString s = actions()->toString();
*/

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
       if ((nodeList.item(i).toElement().attribute("name") ) == toolbarName)
       {
          nodeList.item(i).save(toolStr,2);
          //find the actions registered to the toolbar
          QDomNode n = nodeList.item(i).firstChild();
          while (! n.isNull())
          {
           QDomElement e = n.toElement();
           if (e.tagName()=="Action")
           {
            actionNameList += e.attribute("name");
           }
           n = n.nextSibling();
          }

          nodeList2 = actions()->elementsByTagName("action");
          for (uint k =0; k < nodeList2.count(); k++)
          {
            if (actionNameList.contains(nodeList2.item(k).toElement().attribute("name")) > 0)
            {
                nodeList2.item(k).save(actStr,1);
            }
          }

       }
      }
  }
  toolStr << QString("\n</kpartgui>");
  actStr << QString("\n</actions>");

  toolbarDomList.remove(toolbarName.lower());
  QDomDocument *dom = new QDomDocument();
  dom->setContent(toolStr.read());
  toolbarDomList.insert(toolbarName.lower(), dom);

  buffer.close();
  buffer2.close();

  KTar tar(tarFile, "application/x-gzip");
  tar.open(IO_WriteOnly);
  tar.writeFile(QFileInfo(tarFile).baseName()+".toolbar", "user", "group", buffer.buffer().size(), buffer.buffer().data());
  tar.writeFile(QFileInfo(tarFile).baseName()+".actions", "user", "group", buffer2.buffer().size(), buffer2.buffer().data());
  tar.close();

  return tarFile;
}

/** Saves a toolbar as local or project specific. */
void QuantaApp::slotSaveToolbar(bool localToolbar, QString toolbarToSave)
{
  KURL url;
  int query;
  QString projectToolbarsDir;
  QString toolbarName;

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
  	  url = KFileDialog::getSaveURL(locateLocal("data","quanta/toolbars/"), "*.toolbar.tgz\n*", this);
  	} else
  	{
  	  url = KFileDialog::getSaveURL(project->toolbarDir, "*.toolbar.tgz\n*", this);
  	}

    if (url.isEmpty()) return;

    if ( project->hasProject() )  projectToolbarsDir = project->toolbarDir;
    if ( ((!localToolbar) && (KURL(projectToolbarsDir).isParentOf(url)) ) ||
          ((localToolbar) && (KURL(locateLocal("data","quanta/toolbars/")).isParentOf(url))) )
    {
    	query = doc->write()->checkOverwrite( url );
    } else
    {
      KMessageBox::sorry(0,i18n("You must save the toolbars in one of the following directories: \n\n%1\n%2")
      											 .arg(locateLocal("data","quanta/toolbars/")).arg(projectToolbarsDir));
      query = KMessageBox::No;
    }
  } while (query != KMessageBox::Yes);

  if( query == KMessageBox::Cancel ) return;

  QString tarName = saveToolBar(toolbarName, url.path());
  if (!localToolbar) project->insertFile(tarName, true);
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


  KTempFile* tempFile = new KTempFile();
  tempFile->setAutoDelete(true);
  * (tempFile->textStream()) << QString("<!DOCTYPE kpartgui SYSTEM \"kpartgui.dtd\">\n<kpartgui name=\"quanta\" version=\"2\">\n<ToolBar name=\"%1\" tabname=\"%2\">\n<text>%3</text>\n</ToolBar>\n</kpartgui>\n")\
               .arg(name.lower()).arg(name).arg(name);
  tempFile->close();

  ToolbarXMLGUI * toolbarGUI = new ToolbarXMLGUI(tempFile->name());
  factory()->addClient(toolbarGUI);
  view->toolbarTab->setCurrentPage(view->toolbarTab->count()-1);
  tempFileList.append(tempFile);
  toolbarGUIClientList.insert(name.lower(),toolbarGUI);

  QDomDocument *dom = new QDomDocument(toolbarGUI->domDocument());
  toolbarDomList.insert(name.lower(), dom);
  toolbarNames.insert(tempFile->name(),new QString(name.lower()));
  KURL *url = new KURL();
  QuantaCommon::setUrl(*url,tempFile->name());
  toolbarURLs.insert(name.lower(), url);

  slotToggleDTDToolbar(toolbarNames.count() != 0);
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
  if (toolbarGUIClientList[tb->label(i).lower()])
  {
    lst << tb->label(i);
    if ( tb->tabLabel(tb->currentPage()) == tb->label(i) ) current=j;
    j++;
  }
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
  int current=0;
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
  QString extension=".toolbar.tgz";
  KTempFile* tempFile = new KTempFile(locateLocal("tmp", prefix), extension);;
  tempFile->setAutoDelete(true);
  saveToolBar(toolbarName, tempFile->name());
  tempFile->close();
  tempFileList.append(tempFile);

  toolbarFile += tempFile->name();

  TagMailDlg *mailDlg = new TagMailDlg( this, i18n("Send toolbar by email"));
  QString toStr;
  QString message = i18n("Hi,\n This is a Quanta Plus [http://quanta.sourceforge.net] toolbar.\n\nHave fun.\n");
  QString titleStr;
  QString subjectStr;

  mailDlg->TitleLabel->setText(i18n("Content"));
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
 QDictIterator<QDomDocument> it(toolbarDomList);
 QString s1, s2;
 for( ; it.current(); ++it )
 {
   s1 = it.current()->toString();
   KXMLGUIClient* client = toolbarGUIClientList[it.currentKey()];

   if (client)
   {
     s2 = client->domDocument().toString();

     if ( (s1 != s2) && (!s1.isEmpty()) )
     {
       if (KMessageBox::questionYesNo(this, i18n("The toolbar \"%1\" was modified. Do you want to save before remove?").arg(it.currentKey()),
             i18n("Save Toolbar")) == KMessageBox::Yes)
       {
         slotSaveToolbar(true, it.currentKey() );
       }
     }
   }
 //  toolbarDomList.remove(it.currentKey());
 }
}


/** Reads the DTD info from the file, tries to find the correct DTD and builds the tag/attribute list from the DTD file. */
void QuantaApp::processDTD(QString documentType)
{
 Document *w = view->write();
 QString foundName;
 QString projectDTD = project->defaultDTD();
 w->setDTDIdentifier(projectDTD);

 if (documentType.isEmpty())
 {
   foundName = w->findDTDName(-1,-1, false); //look up the whole file for DTD definition
   bool found = false;
   if (!foundName.isEmpty())   //!DOCTYPE found in file
   {
    DTDSelectDialog *dlg = new DTDSelectDialog(this);
    QDictIterator<DTDStruct> it(*dtds);
    for( ; it.current(); ++it )
    {
      if (it.current()->family == Xml)
      {
        dlg->dtdCombo->insertItem(it.current()->nickName);
        if (it.current()->name == foundName)
        {
          w->setDTDIdentifier(foundName);
          found =true;
        }
      }
    }

//    dlg->dtdCombo->insertItem(i18n("Create new DTD info."));
    if (foundName.isEmpty())
    {
      dlg->messageLabel->setText(i18n("No DTD info was found. Choose a DTD or create a new one."));
      dlg->currentDTD->setText(i18n("Not found"));
    } else
    {
      dlg->messageLabel->setText(i18n("This DTD is not known for Quanta. Choose a DTD or create a new one."));
      dlg->currentDTD->setText(QuantaCommon::getDTDNickNameFromName(foundName));
    }

    for (int i = 0; i < dlg->dtdCombo->count(); i++)
    {
      if (dlg->dtdCombo->text(i) == QuantaCommon::getDTDNickNameFromName(projectDTD))
      {
        dlg->dtdCombo->setCurrentItem(i);
        break;
      }
    }
    if (!found && dlg->exec())
    {
      w->setDTDIdentifier(QuantaCommon::getDTDNameFromNickName(dlg->dtdCombo->currentText()));
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
  DTDSelectDialog *dlg = new DTDSelectDialog(this);
  Document *w = view->write();
  int i=0;
  int pos = -1;
  QDictIterator<DTDStruct> it(*dtds);
  int defaultIndex = 0;

  QString oldDtdName = w->getDTDIdentifier();
  QString defaultDocType = project->defaultDTD();
  for( ; it.current(); ++it )
  {
    if (it.current()->family == Xml)
    {
      dlg->dtdCombo->insertItem(it.current()->nickName);
      if (it.current()->name == oldDtdName) pos = i;
      if (it.current()->name == defaultDocType) defaultIndex = i;
      i++;
    }
  }
  if (pos == -1) pos = defaultIndex;
  dlg->dtdCombo->setCurrentItem(pos);
  dlg->messageLabel->setText(i18n("Change the current DTD."));
  dlg->currentDTD->setText(QuantaCommon::getDTDNickNameFromName(w->getDTDIdentifier()));
  if (dlg->exec())
  {
    w->setDTDIdentifier(QuantaCommon::getDTDNameFromNickName(dlg->dtdCombo->currentText()));
  }

  loadToolbarForDTD(w->getDTDIdentifier());
  reparse();

  delete dlg;
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

 DTDStruct *newDtd = dtds->find(dtdName);
 if (!newDtd) newDtd = dtds->find(project->defaultDTD());
 if (!newDtd) newDtd = dtds->find(qConfig.defaultDocType); //extreme case

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
       QString *toolbarName = toolbarNames[url.prettyURL()];
       if (toolbarName) removeToolbar(*toolbarName);
       fileName = locateLocal("data", "quanta/toolbars/"+oldDtd->toolbars[i]);
       QuantaCommon::setUrl(url, fileName);
       toolbarName = toolbarNames[url.prettyURL()];
       if (toolbarName) removeToolbar(*toolbarName);
     }
   }

   //Load the toolbars for dtdName
   for (uint i = 0; i < newDtd->toolbars.count(); i++)
   {
      QString fileName = qConfig.globalDataDir + "quanta/toolbars/"+newDtd->toolbars[i];
      if (QFileInfo(fileName).exists())
      {
        KURL url;
        QuantaCommon::setUrl(url, fileName);
        if (!toolbarNames[url.prettyURL()]) slotLoadToolbarFile(url);
      } else
      {
        fileName = locateLocal("data", "quanta/toolbars/"+newDtd->toolbars[i]);
        if (QFileInfo(fileName).exists())
        {
          KURL url;
          QuantaCommon::setUrl(url, fileName);
          if (!toolbarNames[url.prettyURL()]) slotLoadToolbarFile(url);
        }
      }
   }
  
   view->toolbarTab->setCurrentPage(0);
 }

 currentToolbarDTD = newDtd->name;
}

/** Remove the toolbar named "name". */
void QuantaApp::removeToolbar(const QString& name)
{
  if (!name.isEmpty())
  {
    KXMLGUIClient* toolbarGUI = toolbarGUIClientList[name];

    if (toolbarGUI)
    {
     //check if the toolbar's XML GUI was modified or not
     QString s1 = toolbarDomList[name]->toString();
     QString s2 = toolbarGUI->domDocument().toString();
     if ( s1 != s2 )
     {
      if (KMessageBox::questionYesNo(this, i18n("The toolbar \"%1\" was modified. Do you want to save before remove?").arg(name),
              i18n("Save Toolbar")) == KMessageBox::Yes)
      {
        slotSaveToolbar(true, name);
      }
     }

     factory()->removeClient(toolbarGUI);
     toolbarGUIClientList.remove(name);
     toolbarDomList.remove(name);
     toolbarMenuList.remove(name);
     KURL *url = toolbarURLs[name];
     QString s = name;
     toolbarNames.remove(url->prettyURL());
     toolbarURLs.remove(s);
    }
  }

  slotToggleDTDToolbar(toolbarNames.count() != 0);
}

/** Show or hide the DTD toolbar */
void QuantaApp::slotToggleDTDToolbar(bool show)
{
  if (show && qConfig.enableDTDToolbar)
  {
    view->toolbarTab->show();
  } else
  {
    view->toolbarTab->hide();
  }
  showDTDToolbar->setChecked(show);
}


/** No descriptions */
void QuantaApp::slotParsingDTDChanged(QString newDTDName)
{
  view->write()->setParsingDTD(newDTDName);
  parser->clear();
  reparse();
}

#include "quanta.moc"
