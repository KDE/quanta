/***************************************************************************
                          quanta.cpp  -  description
                             -------------------
    begin                : ?? ???  9 13:29:57 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
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
#include <qtextstream.h>
#include <qtabbar.h>

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

// application specific includes
#include "quanta.h"
#include "quantaview.h"
#include "quantadoc.h"
#include "document.h"
#include "qextfileinfo.h"
#include "resource.h"

#include "project/project.h"

#include "kwrite/highlight/highlight.h"

#include "widgets/whtmlpart.h"
#include "widgets/wsplitter.h"

#include "dialogs/filemasks.h"
#include "dialogs/styleoptionss.h"
#include "dialogs/previewoptions.h"
#include "dialogs/previewfontoptions.h"
#include "dialogs/parseroptions.h"
#include "dialogs/grepdialog.h"

#include "treeviews/filestreeview.h"
#include "treeviews/fileslistview.h"
#include "treeviews/filestreefolder.h"
#include "treeviews/structtreeview.h"
#include "treeviews/structtreetag.h"

#include "treeviews/doctreeview.h"

#include "parser/parser.h"

#include "messages/messageoutput.h"

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
//doc->openDocument( KURL() );
}

void QuantaApp::slotFileOpen()
{
  KURL url = KURL( KFileDialog::getOpenURL( QDir::homeDirPath(), 
                   i18n("*|All files"), this, i18n("Open File...")));
  slotFileOpen( url );
}

void QuantaApp::slotFileOpen( KURL &url )
{
  doc->openDocument( url );
}

void QuantaApp::slotFileOpenRecent(const KURL &url )
{
  doc->openDocument( url );
}

void QuantaApp::slotProjectOpenRecent(const KURL&)
{
  project->closeProject();
#warning open project recent  
//  project->loadProject (recentProjectsMenu->text(id_));
  leftPanel-> showPage( (QWidget *)pTab );
}

void QuantaApp::slotFileSave()
{
  if ( doc->write()->isUntitled() )
  	slotFileSaveAs();
  else
  	doc->saveDocument( doc->url() );
}

void QuantaApp::slotFileSaveAs()
{
  KURL url =
    KFileDialog::getSaveURL(doc->basePath(),
                            i18n("*|All files"), this, i18n("Save as..."));
                            
  if( url.url().isEmpty() ) return;
  
  doc->saveDocument( url );
    
  bool addToProject = false;
  if ( project->hasProject() ) 
    if ( KMessageBox::Yes == KMessageBox::questionYesNo(0,"Add file\n " +url.url()+"\n to project ? "))
      project->insertFile(url.url(),true);
}

void QuantaApp::slotFileSaveAll()
{
	doc->saveAll();
}

void QuantaApp::slotFileClose()
{
  doc->closeDocument();

  htmlPart()->closeURL();
  htmlPart()->begin( KURL( doc->basePath() ));
  htmlPart()->write( "" );
 	htmlPart()->end();
  
  slotNewStatus();
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

  KMainWindow* w;
  if(memberList)
  {
    for(w=memberList->first(); w!=0; w=memberList->first())
    {
      if(!w->close()) break;
    }
  }	
}


void QuantaApp::slotEditFindInFiles()
{
  slotStatusMsg(i18n("Search in Files..."));

  if (!grepDialog) {
      grepDialog = new GrepDialog( ".", this, "grep_dialog" );
      connect( grepDialog, SIGNAL( itemSelected( const QString &, int ) ),
               this, SLOT( gotoFileAndLine( const QString &, int ) ) );
  }
  grepDialog->show();

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotEditIndent()
{
  doc->write()->indent();
}

void QuantaApp::slotEditUnindent()
{
  doc->write()->unIndent();
}

void QuantaApp::slotEditCleanIndent()
{
  doc->write()->cleanIndent();
}

void QuantaApp::slotEditGotoLine()
{
  doc->write()->gotoLine();
}

void QuantaApp::slotViewToolBar()
{
#warning view tool bar  
}

void QuantaApp::slotViewStatusBar()
{
#warning view status bar  
}


void QuantaApp::slotStatusMsg(const QString &text)
{
  statusBar()->changeItem(text, ID_STATUS_MSG);
}

/** Repaint preview */
void QuantaApp::slotViewRepaint()
{
	repaintPreview( true );
}

/** repaint preview */
void QuantaApp::repaintPreview( bool clear )
{
  WHTMLPart *part = htmlPart();
	QWidgetStack *s = widgetStackOfHtmlPart();
	
	if ( !s ) return;
	if ( !part ) return;
  if ( !s->id( s ->visibleWidget()) ) return;
	  
  static QString oldtext = "";
  if ( clear )   oldtext = "";

  // check use prefix for preview or not
  QString url;
	bool    usePrefix = false;
	
	if ( project->hasProject() && !project->previewPrefix.isEmpty() )
	{
		static QString oldUrl = "";
		if ( clear )   oldUrl = "";
		
		url = doc->write()->url().url();
		url = QExtFileInfo::toRelative( url, project->basePath );
		
		if ( url.left(2) != ".." ) usePrefix = true;
		
		if ( url == oldUrl && !doc->isModified() ) return;
		
		else oldUrl = url;
	}

  // check if don't need reload
  QString text = doc->write()->text();
  if ( !qstrcmp( text, oldtext ) && !usePrefix ) return;

  if ( text.isEmpty() ) 
  {
    text = i18n( "<center><h3>The current document is empty...</h3></center>" );
  }

  KHTMLView *html = part->view();

  int xOffset = html->contentsX(), yOffset = html->contentsY();

  part->closeURL();
		
	KParts::URLArgs 
	  args(true, browserExtension()->xOffset(), browserExtension()->yOffset());
 	
 	browserExtension()->setURLArgs( args );

	if ( usePrefix )
	{
   	if ( doc->isModified() ) doc->saveDocument( doc->url() );
		
		part->begin( project->previewPrefix+url, xOffset, yOffset );
		part->openURL( KURL( project->previewPrefix+url ) );
		part->end();
	}
	else {
		part->begin( KURL( doc->basePath() ), xOffset, yOffset );
  	part->write( text );
		part->end();
	}

  oldtext = text;
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
 	QImage img;
  
  QString furl = QExtFileInfo::toRelative( url, doc->basePath() );
  
   if ( img.load(url) )  
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
/*	
  if ( doc->isModified() )
  {
    enableCommand(ID_FILE_SAVE);
    enableCommand(ID_FILE_SAVE_ALL);
  }
  else {
    disableCommand(ID_FILE_SAVE);

    if ( !doc->isModifiedAll() ) {
      disableCommand(ID_FILE_SAVE_ALL);
    }
    else enableCommand(ID_FILE_SAVE_ALL);
  }
*/	
  QDictIterator<Document> it( *(doc->docList) );

  QIconSet floppyIcon( UserIcon("save_small"));
  QIconSet  emptyIcon( UserIcon("empty1x16" ));

  QTabWidget *wTab = view->writeTab;
  
  while ( Document *w = it.current() )
  {
    if ( w->isModified())
    	wTab->changeTab( w,  floppyIcon,  wTab->tabLabel(w) );
    else
    	wTab->changeTab( w,  emptyIcon,   wTab->tabLabel(w) );

    ++it;
  }
}

/** slot for new undo flag */
void QuantaApp::slotNewUndo()
{
//	int state = doc->write()->undoState();
	// undo/redo
#warning need rewrite statuses	
/*  
  if(state & 1)
 		enableCommand( ID_EDIT_UNDO);
  else
		disableCommand(ID_EDIT_UNDO);
  if(state & 2)
    enableCommand( ID_EDIT_REDO);
  else
    disableCommand(ID_EDIT_REDO);
*/
}

/** slot for new mark flag */
void QuantaApp::slotNewMarkStatus()
{
/*  
	if ( doc->write()->hasMarkedText() ) {
		enableCommand(ID_EDIT_CUT);
    enableCommand(ID_EDIT_COPY);
	}
	else {
		disableCommand(ID_EDIT_CUT);
    disableCommand(ID_EDIT_COPY);
	}
*/	
}

void QuantaApp::slotUpdateStatus(const QString &)
{
	slotNewUndo();
	slotNewStatus();
	slotNewMarkStatus();
	slotNewLineColumn();
}

void QuantaApp::slotOptionsConfigureKeys()
{
	KKeyDialog::configureKeys( actionCollection(), QString::null, true, this );
}

void QuantaApp::slotOptionsConfigureToolbars()
{
    KEditToolbar dlg( actionCollection(), QString::null, true, this );
    
    if ( dlg.exec() ) createGUI();
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

    QWidgetStack *s;
    if ( htmlPart() )
    {
      s = widgetStackOfHtmlPart();
      s ->raiseWidget( 0 );
	 	}
//    checkCommand( ID_VIEW_PREVIEW, false );

  	previewPosition = previewOptions->position();
  	
  	htmlPartTop->closeURL();
  	htmlPartTop->begin( KURL( doc->basePath() ));
  	htmlPartTop->write( "" );
  	htmlPartTop->end();
  	
  	htmlPartRight->closeURL();
  	htmlPartRight->begin( KURL( doc->basePath() ));
  	htmlPartRight->write( "" );
  	htmlPartRight->end();
  	
  	htmlPartBottom->closeURL();
  	htmlPartBottom->begin( KURL( doc->basePath() ));
  	htmlPartBottom->write( "" );
  	htmlPartBottom->end();
  	
  	repaintPreview(true);
  }

  config->sync();

  saveOptions();

  delete kd;
}

void QuantaApp::slotSwapLeftPanelMode()
{
	QWidgetStack *s;
	QWidget *w = leftPanel->currentPage();
	if ( !( s = dynamic_cast<QWidgetStack *>(w)) ) return;
	
	if 	( s->id( s->visibleWidget() ) == 0 )
	{
		FilesTreeView *fT = dynamic_cast<FilesTreeView *>( s->visibleWidget() );
		s->raiseWidget(1);
		FilesListView *fL = dynamic_cast<FilesListView *>( s->visibleWidget() );
		if (!fT) return;
		if (!fL) return;
		FilesTreeFolder *p = dynamic_cast<FilesTreeFolder *>(fT->currentItem()->parent());
		if (!p) return;
		
		QString dir = p->fullName();
		fL->dir = dir;
		fL->slotReload();
	}
	else if ( s->id( s->visibleWidget() ) == 1 ) s->raiseWidget(0);
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

	static int hSplitPos = 1000;
//	static int vSplitPos = 250;

  KToggleAction *ta = (KToggleAction *) actionCollection()->action( "show_preview" );
	bool stat = !ta->isChecked();

	if ( stat ) {
//		disableCommand(ID_VIEW_BACK);
//		disableCommand(ID_VIEW_FORWARD);
//		disableCommand(ID_VIEW_REPAINT);
		
		if ( previewPosition == "Bottom" )
		{
	      hSplitPos = hSplit->getPos();
	      hSplit -> setPos( 1000 );
	  }
		
		s   ->raiseWidget( 0 );
		doc ->write()->setFocus();
	}
	else {
//		enableCommand(ID_VIEW_BACK);
//		enableCommand(ID_VIEW_FORWARD);
//		enableCommand(ID_VIEW_REPAINT);
		
	  if ( previewPosition == "Bottom" )
	  {
	    if ( (hSplit -> getPos()) > 800 && hSplitPos > 800 )
	         hSplit -> setPos(800);
	    else hSplit -> setPos( hSplitPos );
	
	    hSplitPos = hSplit->getPos();
	  }
		
		s->raiseWidget( 1 );
		repaintPreview(false);
	}
//	checkCommand( ID_VIEW_PREVIEW, !stat );
}

void QuantaApp::slotShowLeftPanel()
{
	static int vSplitPos = 250;

  KToggleAction *ta = (KToggleAction *) actionCollection()->action( "show_tree" );
	bool stat = !ta->isChecked();

	if ( stat )
	{
    vSplitPos = vSplit->getPos();
    vSplit -> setPos( 0 );
	}
	else {
    if ( vSplitPos == 0 ) vSplitPos = 250;
    vSplit -> setPos( vSplitPos );
	  vSplitPos = vSplit->getPos();
	}
//	checkCommand( ID_VIEW_TREE, !stat );
}

void QuantaApp::slotNewLineColumn()
{
  QString linenumber;
  int y = view->write()->currentLine()+1;
  int x = view->write()->currentColumn() +1;

  sTab->showTagAtPos(x-1,y-1);

  linenumber.sprintf(i18n("Line: %d Col: %d"),y,x);

  statusBar()->changeItem(linenumber.data(), ID_STATUS_CLM);
}

/** reparse current document and initialize node. */
void QuantaApp::reparse()
{
	if ( QString(leftPanel->currentPage()->name()) == "struct" )
	{
		Node *node = parser->parse( view->write()->text() );
		//sTab->s = parser->s;
		if ( parser->textChanged ) {
		  config->setGroup("Parser options");
		  int expandLevel = config->readNumEntry("Expand level",8);
		  if ( expandLevel == 0 )
		  	expandLevel = 100;
		  	
		  sTab->slotReparse( node , expandLevel );
		}
		// delete node;
	}
}

void QuantaApp::setCursorPosition( int row, int col )
{
  view->write()->setCursorPosition( row, col );
  view->write()->view()->setFocus();
}

void QuantaApp::gotoFileAndLine( const QString &filename, int line )
{
  doc->openDocument( filename );
  
  if ( view->write()->numLines() > line && line >= 0 )
  {
    view->write()->setCursorPosition( line, 0 );
  }
  
  view->write()->view()->setFocus();
}


void QuantaApp::slotSelectMessageWidget()
{
	bottomWidgetStack -> raiseWidget(0);
	if ( hSplit->getPos() > 800 ) hSplit->setPos(800);
}

void QuantaApp::slotDisableMessageWidget()
{
	hSplit->setPos(1000);
}


void QuantaApp::slotLeftTabChanged(QWidget *)
{

  static bool docTabOpened = false;

  StructTreeView *sView = dynamic_cast<StructTreeView *>( leftPanel->currentPage());
  if ( sView )
  	reparse();
  DocTreeView *dView = dynamic_cast<DocTreeView *>( leftPanel->currentPage());

  if ( dView ) {
    static bool first = true;
  	rightWidgetStack -> raiseWidget(2);
  	if ( first )
  	{
  		openDoc( locate("appdata","doc/documentation.html") );
  		first = false;
  	}
  	docTabOpened = true;
  }
  else {
    if ( docTabOpened )
    {
    	rightWidgetStack -> raiseWidget(0);
    	doc ->write()->setFocus();
    }
  	docTabOpened = false;
  }
}

void QuantaApp::selectArea(int col1, int row1, int col2, int row2)
{
  setCursorPosition( row2, col2 );
  view->write()->selectText(col1,row1,col2,row2);
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
   if ( leftPanel->currentPage() == dTab ) {
   	 leftPanel->showPage(fTab);
   }
   else {
     QString curWord = view->write()->currentWord();
     QString * url = dTab->contextHelp( curWord );

     if ( url ) {
        leftPanel->showPage(dTab);
   		  openDoc(*url);
     }
   }
}

void QuantaApp::slotSetHl( int _hl )
{
  doc->write()->setHl(_hl);
}

void QuantaApp::slotSetEol( int _eol )
{
  doc->write()->setEol(_eol);
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

void QuantaApp::slotViewMessages()
{
  KToggleAction *ta = (KToggleAction *) actionCollection()->action( "show_messages" );
	bool stat = !ta->isChecked();
  
  static int oldpos = (hSplit->getPos() > 850) ? 850 : hSplit->getPos();
  
  int pos = hSplit->getPos();
  
  if ( stat ) 
  {
    oldpos=( pos > 850) ? 850 : pos;
    hSplit->setPos(1000);
  }
  else hSplit->setPos(oldpos);
}

void QuantaApp::slotToolSyntaxCheck()
{
  slotFileSave();
  if ( !doc->write()->isUntitled() ) 
  {
    KProcess *p = new KProcess();
    *p << "perl";
    *p << locate("lib","quanta/plugins/weblint");
    *p << "-x" << "Netscape";
    *p << doc->write()->url().url();
    
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
  debug( QString("name:") + container->parent()->name() );
  
  if ( container->parent() && QString(container->parent()->name()) == QString("ToolBar stack") ) {
    ((KToolBar*)container)->saveState( element );
    int id = view->toolbarStack->id( container );
    qDebug("id: %d",id);
    view->toolbarStack->removeWidget( container );
    view->tabBar->removeTab( view->tabBar->tab(id) );
  }
  else
    KMainWindow::removeContainer( container, parent, element, id );
}
