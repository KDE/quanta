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
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qtabbar.h>
#include <qradiobutton.h>
#include <qimage.h>
#include <qtimer.h>
#include <qtextstream.h>
#include <qiodevice.h>

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
#include <kdockwidget.h>
#include <kstatusbar.h>
#include <kprocess.h>
#include <ktempfile.h>

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

#include "treeviews/filestreeview.h"
#include "treeviews/fileslistview.h"
#include "treeviews/filestreefolder.h"
#include "treeviews/structtreeview.h"
#include "treeviews/structtreetag.h"
#include "treeviews/doctreeview.h"
#include "treeviews/templatestreeview.h"

#include "tagdialogs/listdlg.h"

#include "parser/parser.h"

#include "messages/messageoutput.h"

#include "toolbar/actioneditdlg.h"

#include <kedittoolbar.h>
#include <kaction.h>



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
//  slotUpdateStatus(view->write());
}

void QuantaApp::slotFileOpen()
{
  KURL url = KFileDialog::getOpenURL( QString::null, QString::null, this);
    
  if ( !url.url().isEmpty() ) slotFileOpen( url );

  slotUpdateStatus(view->write());
}

void QuantaApp::slotFileOpen( const KURL &url )
{
  doc->openDocument( url );
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
  if ( doc->write()->isUntitled() )
  	slotFileSaveAs();
  else
  	doc->saveDocument( doc->url() );

  slotUpdateStatus(view->write());
}

void QuantaApp::slotFileSaveAs()
{
  KURL url;
  int query;

  do {
    query = KMessageBox::Yes;
    
    url = KFileDialog::getSaveURL(doc->basePath(), QString::null, this);
    
    if (url.isEmpty()) return;

    query = doc->write()->checkOverwrite( url );
  } 
  while (query != KMessageBox::Yes);

  if( query == KMessageBox::Cancel ) return;
                            
  doc->saveDocument( url );
    
  if ( project->hasProject() ) 
    if ( KMessageBox::Yes == KMessageBox::questionYesNo(0,"Add file\n " +url.url()+"\n to project ? "))
      project->insertFile(url.url(),true);

  slotUpdateStatus(view->write());
}

void QuantaApp::saveAsTemplate(bool projectTemplate)
{
  KURL url;
  int query;
  QString projectTemplateDir;

  do {
    query = KMessageBox::Yes;

    if (projectTemplate)
    {
  	  url = KFileDialog::getSaveURL(doc->basePath()+"templates/", QString::null, this);
  	} else
  	{
  	  url = KFileDialog::getSaveURL(locateLocal("data","quanta/templates/"), QString::null, this);
  	}

    if (url.isEmpty()) return;

    if ( project->hasProject() )  projectTemplateDir = doc->basePath()+"templates/";
    if ( ((projectTemplate) && (KURL(projectTemplateDir).isParentOf(url)) ) ||
          ((! projectTemplate) && (KURL(locateLocal("data","quanta/templates/")).isParentOf(url))) )
    {
    	query = doc->write()->checkOverwrite( url );
    } else
    {
      KMessageBox::sorry(0,i18n("You must save the templates in one of the following directories: \n\n%1\n%2")
      											 .arg(locateLocal("data","quanta/templates/")).arg(projectTemplateDir));
      query = KMessageBox::No;
    }
  } while (query != KMessageBox::Yes);

  if( query == KMessageBox::Cancel ) return;

  doc->saveDocument( url );
  slotUpdateStatus(view->write());
}

void QuantaApp::slotFileSaveAsLocalTemplate()
{
	saveAsTemplate(false);
}

void QuantaApp::slotFileSaveAsProjectTemplate()
{
	saveAsTemplate(true);
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
}

void QuantaApp::slotFileQuit()
{
  saveOptions();

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
      grepDialog = new GrepDialog( ".", this, "grep_dialog" );
      connect( grepDialog, SIGNAL( itemSelected   (const QString &, int)),
               this,       SLOT  ( gotoFileAndLine(const QString &, int)));
  }
  grepDialog->show();
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
  if (!doc->write()->isUntitled())	
  {
//if it's	not untitled, than it was loaded from somewhere. In this case show it from that place
	  url = doc->write()->url().url();
/*
    //First make a backup copy
    previewTmpFile = new KTempFile();//(QFileInfo(url).dirPath());
    QTextStream stream(previewTmpFile->fstream(), IO_ReadWrite);
    QString s= doc->write()->editIf->text() ;
    stream << s;
    previewTmpFile->close();

    previewCopyMade = true;
  */
	  if ( doc->isModified() ) doc->saveDocument( doc->url() );
	
	  url = project->urlWithPrefix(doc->write()->url());
	
	  part->begin(url, xOffset, yOffset );
	  part->openURL( KURL(url) );		
//	  part->openURL(previewTmpFile->name());		
  } else  //the document is Untitled, preview the text from it
 {
  	QString text = doc->write()->editIf->text();
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
void QuantaApp::slotInsertTag(QString url)
{
 	QImage img(url);
  
  QString furl = QExtFileInfo::toRelative( url, doc->basePath() );
  
   if ( !img.isNull() )
   { 
     QString w,h;
     w.setNum( img.width () );
	   h.setNum( img.height() );

	   doc->write()->insertTag("<img src=\""+furl+"\" width=\""+w+"\" height=\""+h+"\" border=\"0\">");
   }
   else
     doc->write()->insertTag( "<a href=\""+furl+"\">","</a>");
}

////////////////////////
// status slots
///////////////////////
/** slot for new modify flag */
void QuantaApp::slotNewStatus()
{
  setTitle( doc->url().prettyURL() );

/*  int  config   = doc->write()->config();
  bool readOnly = doc->write()->isReadOnly();

  if (readOnly) statusBar()->changeItem(i18n(" R/O "),IDS_INS_OVR);
  else          statusBar()->changeItem(config & KWriteView::cfOvr ? i18n(" OVR ") : i18n(" INS "),IDS_INS_OVR);
               */
  statusBar()->changeItem(doc->write()->isModified() ? " * " : "",IDS_MODIFIED);

  saveAction   ->setEnabled(doc->isModified());
  saveAllAction->setEnabled(doc->isModifiedAll());

  saveprjAction     ->setEnabled(project->isModified());
  closeprjAction     ->setEnabled(project->hasProject());
  insertFileAction   ->setEnabled(project->hasProject());
  insertDirAction    ->setEnabled(project->hasProject());
  rescanPrjDirAction ->setEnabled(project->hasProject());
  uploadProjectAction->setEnabled(project->hasProject());
  projectOptionAction->setEnabled(project->hasProject());

  viewBorder->setChecked(view->write()->kate_view->iconBorder());
  viewLineNumbers->setChecked(view->write()->kate_view->lineNumbersOn());

  if (setHighlight) setHighlight->updateMenu (view->write()->kate_doc);

  QIconSet floppyIcon( UserIcon("save_small"));
  QIconSet  emptyIcon( UserIcon("empty1x16" ));

#ifdef USE_KDOCKTABGROUP
  KDockTabGroup *wTab = view->writeTab;
  int pageId = wTab->visiblePageId();
  Document *w = static_cast<Document*>(wTab->visiblePage());
  if (w->isModified())
  {
   	 wTab->setPixmap(pageId, floppyIcon.pixmap());
  }
  else
  {
     wTab->setPixmap(pageId, QPixmap(0,0));
  }
#else
  QTabWidget *wTab = view->writeTab;
  Document *w = static_cast<Document*>(wTab->currentPage());

 if ( w->isModified() )
	  wTab->changeTab( w,  floppyIcon, wTab->tabLabel(w));
 else 	
	wTab->changeTab( w,  emptyIcon,  wTab->tabLabel(w));

//This is a really dirty fix for the QTabWidget problem. After the changeTab call,
//it will reset itself and you will see the first tabs, even if the actual page is on
//a tab eg. at the end, and it won't be visible now. This is really confusing.
/*
	int pageId = wTab->currentPageIndex();	
    bool block=wTab->signalsBlocked();
    wTab->blockSignals(true);
    wTab->setCurrentPage(pageId-1);
    wTab->setCurrentPage(pageId);
    wTab->blockSignals(block);*/
#endif

  w->oldstat = w->isModified();
}

/** slot for new undo flag */
void QuantaApp::slotNewUndo()
{
/*!!!!	int state = doc->write()->undoState();

  undoAction->setEnabled(state & 1);
  redoAction->setEnabled(state & 2);*/
}

void QuantaApp::slotUpdateStatus(QWidget* w)
{
	slotNewUndo();
	slotNewStatus();
	slotNewLineColumn();

//Add the Kate menus
/*
  if (view->oldWrite != 0L)
  {
    view->oldWrite->writeConfig(config);
    guiFactory()->removeClient(view->oldWrite->view());
  }
  guiFactory()->addClient(dynamic_cast<Document *>(w)->view());
*/

  dynamic_cast<Document *>(w)->readConfig(config);

  view->write()->view()->resize(view->writeTab->size().width()-5, view->writeTab->size().height()-35);
  view->oldWrite = view->write();

  emit reloadTreeviews();
}

void QuantaApp::slotOptionsConfigureKeys()
{
	KKeyDialog::configureKeys( actionCollection(), xmlFile(), true, this );
}

void QuantaApp::slotOptionsConfigureToolbars()
{
    KEditToolbar dlg( actionCollection(), QString::null, true, this );
    
    if ( dlg.exec() ) {
    
      // clear tags toolbars
      while ( view->tabBar->count() ) {
      
          int id = view->tabBar->currentTab();
          
          view->toolbarStack->removeWidget( view->toolbarStack->widget(id) );
          view->tabBar->removeTab( view->tabBar->tab(id) );
      }

      createGUI();
    }
}

void QuantaApp::slotOptionsConfigureActions()
{
    ActionEditDlg dlg( this, this, "actions_edit_dlg", true); //actionCollection(), QString::null, true, this );
    
    if ( dlg.exec() ) {
    QFile f( KGlobal::instance()->dirs()->saveLocation("data")+"quanta/actions.rc" );
    f.open( IO_ReadWrite | IO_Truncate );
    QTextStream qts(&f);
    m_actions->save(qts,0);
    f.close();

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

  styleOptionsS->checkTagsCapital->setChecked( tagsCapital);
  styleOptionsS->checkAttrCapital->setChecked( attrCapital);
  styleOptionsS->checkEndTag->setChecked( useCloseTag );


  // Files Masks options
  page=kd->addVBoxPage(i18n("Files Masks"), QString::null, BarIcon("files", KIcon::SizeMedium ) );
  FileMasks *fileMasks = new FileMasks( (QWidget *)page );

  fileMasks->lineHTML->setText( fileMaskHtml );
  fileMasks->linePHP->setText( fileMaskPhp );
  fileMasks->lineImages->setText( fileMaskImage );
  fileMasks->lineText->setText( fileMaskText );

  // Preview options
  page=kd->addVBoxPage(i18n("Preview"), QString::null, BarIcon("kview", KIcon::SizeMedium ) );
  PreviewOptions *previewOptions = new PreviewOptions( (QWidget *)page );

  previewOptions->setPosition( previewPosition );

  page=kd->addVBoxPage(i18n("Parser"), QString::null, BarIcon("kcmsystem", KIcon::SizeMedium ) );
  ParserOptions *parserOptions = new ParserOptions( config, (QWidget *)page );
  
  page=kd->addVBoxPage(i18n("PHP Debug"), QString::null, BarIcon("gear", KIcon::SizeMedium ) );
  DebuggerOptionsS *debuggerOptions = new DebuggerOptionsS( (QWidget *)page );

  if (debuggerStyle=="PHP3") debuggerOptions->radioPhp3->setChecked(true);
  if (debuggerStyle=="None") debuggerOptions->checkDebugger->setChecked(false);
  
  if ( kd->exec() )
  {
    tagsCapital = styleOptionsS->checkTagsCapital->isChecked();
    attrCapital = styleOptionsS->checkAttrCapital->isChecked();
    useCloseTag = styleOptionsS->checkEndTag->isChecked();

    fileMaskHtml = fileMasks->lineHTML->text();
  	fileMaskPhp  = fileMasks->linePHP->text();
	  fileMaskImage= fileMasks->lineImages->text();
  	fileMaskText = fileMasks->lineText->text();

    parserOptions->updateConfig();

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
	
	if ( !s ) return;
	if ( !part ) return;

  KToggleAction *ta = (KToggleAction *) actionCollection()->action( "show_preview" );
	bool stat = !ta->isChecked();

	if ( stat ) {
//		disableCommand(ID_VIEW_BACK);
//		disableCommand(ID_VIEW_FORWARD);
//		disableCommand(ID_VIEW_REPAINT);
		
		s   ->raiseWidget( 0 );
/*
    if (previewCopyMade)
    {
      previewTmpFile->unlink();
      previewCopyMade = false;
    }
*/
		doc ->write()->setFocus();
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
  unsigned int x;
  unsigned int y;

  view->write()->viewCursorIf->cursorPosition(&y, &x);
  x++; y++ ;

  sTab->showTagAtPos(x-1,y-1);

  linenumber.sprintf(i18n("Line: %d Col: %d"),y,x);

  statusBar()->changeItem(linenumber.data(), IDS_STATUS_CLM);
}

/** reparse current document and initialize node. */
void QuantaApp::reparse()
{
	if ( stabdock->isVisible() )
	{
		Node *node = parser->parse( view->write()->editIf->text() );
		//sTab->s = parser->s;
		if ( parser->textChanged ) {
		  config->setGroup("Parser options");
		  int expandLevel = config->readNumEntry("Expand level",8);
		  if ( expandLevel == 0 )
		  	expandLevel = 40;
		  	
		  sTab->slotReparse( node , expandLevel );
		  
      unsigned int x;
      unsigned int y;

      view->write()->viewCursorIf->cursorPosition(&y, &x);

      sTab->showTagAtPos(x,y);

		}
		// delete node;
	}
}

void QuantaApp::setCursorPosition( int row, int col )
{
  
  int numLines = view->write()->editIf->numLines();
  
  if ( row < numLines )
    view->write()->viewCursorIf->setCursorPosition(row, col);
  else
    view->write()->viewCursorIf->setCursorPosition(numLines - 1, col);

  view->write()->view()->setFocus();
}

void QuantaApp::gotoFileAndLine( const QString &filename, int line )
{
  if ( !filename.isEmpty() ) doc->openDocument( filename );
  
  int numLines = view->write()->editIf->numLines();
  if ( numLines > line && line >= 0 )
  {
    view->write()->viewCursorIf->setCursorPosition(line, 0);
  }
  
  view->write()->view()->setFocus();
}


//void QuantaApp::slotLeftTabChanged(QWidget *)
void QuantaApp::slotDockChanged()
{
  static bool docTabOpened = false;
  
  if ( stabdock->isVisible() ) reparse();
  
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
      if (doc->write() != 0) {doc ->write()->setFocus();}
    }
  }
}

void QuantaApp::selectArea(int col1, int row1, int col2, int row2)
{
  int numLines = view->write()->editIf->numLines();

  if ( row1 > numLines-1 )
    row1 = numLines-1;
    
  if ( row2 > numLines-1 )
    row2 = numLines-1;
    
  view->write()->viewCursorIf->setCursorPosition(row2, col2);
  view->write()->selectionIf->setSelection(row1, col1, row2, col2);
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

void QuantaApp::updateNavButtons( bool back, bool forward )
{
/*   
   if ( back )
   	  enableCommand(ID_VIEW_BACK);
   else
   		disableCommand(ID_VIEW_BACK);
   		
   if ( forward )
   	  enableCommand(ID_VIEW_FORWARD);
   else
   		disableCommand(ID_VIEW_FORWARD);
*/   		
}

void QuantaApp::contextHelp()
{
  int id_w = rightWidgetStack->id( rightWidgetStack->visibleWidget());
  
  if (  id_w == 1 || id_w == 2 )
  {
    rightWidgetStack->raiseWidget(0);
    doc ->write()->setFocus();
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

void QuantaApp::slotFtpClient()
{
/*	
	bool stat = toolMenu -> isItemChecked( ID_VIEW_FTP );
	toolMenu->setItemChecked(ID_VIEW_FTP, !stat);
	
	// get the library loader instance
  KLibLoader *loader = KLibLoader::self();
  
  QString lib_name = "quanta/plugins/libftpclient.so";
  
  KLibrary *lib = 
    loader->library(QFile::encodeName( lib_name ));
  
  if (lib)
	{
	  // get the init_ function
	  QString funcname = QString("create_ftpclient");
	  void *init = lib->symbol(funcname.utf8());
	  if (init)
	  {
	    QDialog* (*func)(QWidget *, const char *); 
	    func = (QDialog* (*)(QWidget *, const char *))init;
      QDialog *dlg = func(0,"Small Ftp Client");
      
      dlg->exec();
      
      delete dlg;
      
      loader->unloadLibrary(QFile::encodeName( lib_name ));
	  }
	}
	else debug("Library not found");
*/	
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
  slotFileSave();
  if ( !doc->write()->isUntitled() ) 
  {
    QString fname = doc->write()->url().prettyURL();
    if ( fname.left(5) == "file:" ) fname.remove(0,5);
    
    KProcess *p = new KProcess();
    *p << "perl";
    *p << locate("lib","quanta/plugins/weblint");
    *p << "-x" << "Netscape";
    *p << fname;
    
    connect( p, SIGNAL(processExited(KProcess *)),
             messageOutput, SLOT(weblintFinished()) );
    connect( p, SIGNAL(receivedStdout(KProcess *, char *, int)),
             messageOutput, SLOT( processWebLint(KProcess *, char *, int)) );

    
    p->start( KProcess::NotifyOnExit, KProcess::Stdout);
  }
}
    
QWidget* QuantaApp::createContainer( QWidget *parent, int index, const QDomElement &element, int &id )
{
  QString tabname = element.attribute( "tabname", "" );
  
  if ( element.tagName().lower() == "toolbar" && !tabname.isEmpty() ) {
    KToolBar *tb = new KToolBar( view->toolbarStack );
    tb->loadState( element );
    view->toolbarStack->addWidget( tb, view->tabBar->addTab( new QTab( i18n( tabname ))));
    
    return tb;
  }
  
  return KMainWindow::createContainer( parent, index, element, id );
  
}

void QuantaApp::removeContainer( QWidget *container, QWidget *parent, QDomElement &element, int id )
{
//  debug( QString("name:") + container->parent()->name() );
  
  if ( container->parent() && QString(container->parent()->name()) == QString("ToolBar stack") ) {
/*    ((KToolBar*)container)->saveState( element );
    int id = view->toolbarStack->id( container );
    qDebug("id: %d",id);
    view->toolbarStack->removeWidget( container );
    view->tabBar->removeTab( view->tabBar->tab(id) ); */
  }
  else
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
       	tmpString = QExtFileInfo::toRelative(tmpString,project->basePath);
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

#ifdef USE_KDOCKTABGROUP
  view->writeTab->setVisiblePage(view->doc->docList->find(docName));
#else
  view->writeTab->showPage(view->doc->docList->find(docName));
#endif
}
/** No descriptions */
void QuantaApp::slotNewProjectLoaded()
{
  delete tTab;
  tTab = new TemplatesTreeView( project->basePath, ttabdock );
  ttabdock ->setWidget( tTab );
  tTab  ->setFocusPolicy(QWidget::NoFocus);
  connect(   tTab, SIGNAL(openFile  (const KURL &)),
            this, SLOT(slotFileOpen(const KURL &)));
  connect(   tTab, SIGNAL(insertFile  (QString)),
            this, SLOT(slotInsertFile(QString)));
  connect(   tTab,SIGNAL(insertTag(QString)),
            this, SLOT(slotInsertTag(QString)));
  connect(this, SIGNAL(reloadTreeviews()), tTab, SLOT (slotReload()));
  connect(fTTab, SIGNAL(reloadTreeviews()), tTab, SLOT (slotReload()));
  connect(tTab, SIGNAL(reloadTreeviews()), fTTab, SLOT (slotReload()));
  connect(tTab, SIGNAL(reloadTreeviews()), tTab, SLOT (slotReload()));
}

/** No descriptions */
void QuantaApp::slotInsertFile(QString fileName)
{
  doc->write()->insertFile(fileName);
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

  markList = view->write()->kate_doc->marks();
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
      QString bText = view->write()->kate_doc->textLine(markList.at(i)->line);
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



#include "quanta.moc"
