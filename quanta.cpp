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

#include "toolbar/toolbarconfigi.h"
#include "toolbar/toolbars.h"

#include "messages/messageoutput.h"

#include <kedittoolbar.h>
#include <kaction.h>



/////////////////////////////////////////////////////////////////////
// SLOT IMPLEMENTATION
/////////////////////////////////////////////////////////////////////

void QuantaApp::slotFileNewWindow()
{
  slotStatusMsg(i18n("Opening a new application window..."));
	
  QuantaApp *new_window= new QuantaApp();
  new_window->show();

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotFileNew()
{
  slotStatusMsg(i18n("Creating new document..."));

  doc->newDocument();		

  setCaption(doc->getTitle());

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotFileOpen()
{
  slotStatusMsg(i18n("Opening file..."));

  QString fileToOpen=KFileDialog::getOpenFileName(QDir::homeDirPath(),
                                                    i18n("*|All files"), this, i18n("Open File..."));
  slotFileOpen( fileToOpen );
}

void QuantaApp::slotFileOpen( QString fileToOpen )
{
  slotStatusMsg(i18n("Opening file..."));
	
  if(!fileToOpen.isEmpty())
  {
    doc->openDocument(fileToOpen);
    setCaption(doc->getTitle());
    addRecentFile(fileToOpen);
  }

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotFileOpenRecent(int id_)
{
  slotStatusMsg(i18n("Opening file..."));
#warning open recent file
//  doc->openDocument(recentFilesMenu->text(id_));
  setCaption(doc->getTitle());

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotProjectOpenRecent(int id_)
{
  slotStatusMsg(i18n("Opening project..."));
	
  project->closeProject();
#warning open project recent  
//  project->loadProject (recentProjectsMenu->text(id_));
  leftPanel-> showPage( (QWidget *)pTab );

  setCaption(doc->getTitle());

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotFileSave()
{
  slotStatusMsg(i18n("Saving file..."));
	
  if ( !doc->write()->hasFileName() )
  	slotFileSaveAs();
  else
  	doc->saveDocument( doc->getAbsFilePath() );

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotFileSaveAs()
{
  slotStatusMsg(i18n("Saving file with a new filename..."));

  QString newName=KFileDialog::getSaveFileName(QDir::currentDirPath(),
                                               i18n("*|All files"), this, i18n("Save as..."));
  if(!newName.isEmpty())
  {
    bool addToProject = false;
    if ( project->hasProject() ) 
      addToProject = ( KMessageBox::Yes == KMessageBox::questionYesNo(0,"Add file\n " +newName+"\n to project ? ") );


    QFileInfo saveAsInfo(newName);
    doc->setTitle(saveAsInfo.fileName());

    doc->saveDocument(newName);
    addRecentFile(newName);

    setCaption(doc->getTitle());

    if ( addToProject ) 
      project->insertFile(newName,true);
  }

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotFileSaveAll()
{
  slotStatusMsg(i18n("Saving all files..."));
	
	doc->saveAll();

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}


void QuantaApp::slotFileClose()
{
  slotStatusMsg(i18n("Closing file..."));
	
  doc->closeDocument();

  htmlPart()->closeURL();
  htmlPart()->begin( KURL( doc->basePath() ));
 	htmlPart()->write( "" );
 	htmlPart()->end();

  setCaption(doc->getTitle());

  slotNewStatus();

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotFileCloseAll()
{
  slotStatusMsg(i18n("Closing all files..."));
	
  doc->closeAll();

  htmlPart()->closeURL();
  htmlPart()->begin( KURL( doc->basePath() ));
 	htmlPart()->write( "" );
 	htmlPart()->end();

  setCaption(doc->getTitle());

  slotNewStatus();

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotFileNext()
{
   slotStatusMsg(i18n("Go to next file..."));

   doc->nextDocument();

   slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotFilePrev()
{
   slotStatusMsg(i18n("Go to previous file..."));

   doc->prevDocument();

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}


void QuantaApp::slotFilePrint()
{
  slotStatusMsg(i18n("Printing..."));

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotFileQuit()
{
  slotStatusMsg(i18n("Exiting..."));
  saveOptions();

  //if ( !doc->saveAll(false) )
  //	return;

  // close the first window, the list makes the next one the first again.
  // This ensures that queryClose() is called on each window to ask for closing
  KMainWindow* w;
  if(memberList)
  {
    for(w=memberList->first(); w!=0; w=memberList->first())
    {
      // only close the window if the closeEvent is accepted. If the user presses Cancel on the saveModified() dialog,
      // the window and the application stay open.
      if(!w->close())
				break;
    }
  }	
}

void QuantaApp::slotEditCut()
{
  slotStatusMsg(i18n("Cutting selection..."));

  doc->write()->cut();

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotEditCopy()
{
  slotStatusMsg(i18n("Copying selection to clipboard..."));

  doc->write()->copy();

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotEditPaste()
{
  slotStatusMsg(i18n("Inserting clipboard contents..."));

  doc->write()->paste();

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotEditUndo()
{
  slotStatusMsg(i18n("Undo..."));

  doc->write()->undo();

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotEditRedo()
{
  slotStatusMsg(i18n("Redo..."));

  doc->write()->redo();

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotURedoHistory()
{
  slotStatusMsg(i18n("Undo/Redo history..."));

  doc->write()->undoHistory();

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotEditSelectAll()
{
  slotStatusMsg(i18n("Undo/Redo history..."));

  doc->write()->selectAll();

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotEditDeselectAll()
{
  slotStatusMsg(i18n("Undo/Redo history..."));

  doc->write()->deselectAll();

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotEditSearch()
{
  slotStatusMsg(i18n("Search..."));

  doc->write()->find();

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotEditSearchAgain()
{
  slotStatusMsg(i18n("Search again..."));

  doc->write()->findAgain();

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotEditReplace()
{
  slotStatusMsg(i18n("Replace..."));

  doc->write()->replace();

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
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

void QuantaApp::slotEditInvertSelect()
{
  slotStatusMsg(i18n("Invert Selection..."));

  doc->write()->invertSelection();

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotEditVerticalSelect()
{
  slotStatusMsg(i18n("Vertical Selection..."));

  doc->write()->toggleVertical();

//  bool stat = editMenu -> isItemChecked( ID_EDIT_VERTICAL_SELECT );
//  checkCommand( ID_EDIT_VERTICAL_SELECT, !stat );

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotEditIndent()
{
  slotStatusMsg(i18n("Indent..."));

  doc->write()->indent();

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotEditUnindent()
{
  slotStatusMsg(i18n("UnIndent..."));

  doc->write()->unIndent();

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotEditCleanIndent()
{
  slotStatusMsg(i18n("Clean Indentation..."));

  doc->write()->cleanIndent();

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotEditGotoLine()
{
  slotStatusMsg(i18n("Goto Line..."));

  doc->write()->gotoLine();

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotViewToolBar()
{
  slotStatusMsg(i18n("Toggle the toolbar..."));

#warning view tool bar  
  
  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotViewStatusBar()
{
  slotStatusMsg(i18n("Toggle the statusbar..."));

#warning view status bar  
  
  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}


void QuantaApp::slotStatusMsg(const QString &text)
{
  ///////////////////////////////////////////////////////////////////
  // change status message permanently
  statusBar()->clear();
  statusBar()->changeItem(text, ID_STATUS_MSG);
}


void QuantaApp::slotStatusHelpMsg(const QString &text)
{
  ///////////////////////////////////////////////////////////////////
  // change status message of whole statusbar temporary (text, msec)
  statusBar()->message(text, 2000);
}


/** Repaint preview */
void QuantaApp::slotViewRepaint()
{
	slotStatusMsg(i18n("Repainting..."));
	
	repaintPreview( true );
	
	slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
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
  QString fname;
	bool usePrefix = false;
	
	if ( project->hasProject() && !project->previewPrefix.isEmpty() )
	{
		static QString oldname = "";
		if ( clear )   oldname = "";
		
		fname = doc->write()->fileName();
		fname = QExtFileInfo::toRelative( fname, project->basePath );
		if ( fname.left(2) != ".." ) usePrefix = true;
		
		if ( fname == oldname && !doc->isModified() ) return;
		else oldname = fname;
	}

  QString text = doc->write()->text();

  // check if don't need reload
  if ( !qstrcmp( text, oldtext ) && !usePrefix ) return;

  if ( text.isNull() ) {
      text = i18n( "<i>The current document is empty</i>" );
  }

  KHTMLView *html = part->view();

  int xOffset = html->contentsX(), yOffset = html->contentsY();

  part->closeURL();
		
	KParts::URLArgs args(true, browserExtension()->xOffset(), browserExtension()->yOffset());
 	browserExtension()->setURLArgs( args );

	if ( usePrefix )
	{
   	if ( doc->isModified() ) doc->saveDocument( doc->getAbsFilePath() );
		
		part->begin( project->previewPrefix+fname, xOffset, yOffset );
		part->openURL( KURL( project->previewPrefix+fname ) );
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
void QuantaApp::slotImageOpen( QString fileToOpen )
{

  if ( previewPosition == "Disabled" )
     return;

  WHTMLPart *part = htmlPart();
  QWidgetStack *s = widgetStackOfHtmlPart();
  
  if ( !s ) return;
  if ( !part ) return;

  if ( !s->id( s ->visibleWidget()) ) return;

	QString text = "<html>\n<body>\n<div align=\"center\">\n<img src=\"";
	text += fileToOpen;
	text += "\">\n</div>\n</body>\n</html>\n";
	
	part->closeURL();
	part->begin( KURL( doc->basePath() ) );
  part->write( text.data() );
  part->end();

  part->show();
}


/** insert <img> tag for images or <a> for other */
void QuantaApp::slotInsertTag( QString file )
{
 	QImage img;
  QExtFileInfo ifile( file );
  ifile.convertToRelative( doc->basePath() );
	QString shortName = ifile.filePath();
	
   if ( img.load(file) )  { // image
     QString w,h;
     w.setNum( img.width() );
	   h.setNum( img.height() );
	   doc->write()->insertTag("<img src=\""+shortName+"\" width=\""+w+"\" height=\""+h+"\" border=\"0\">");
   } 
   else {
     doc->write()->insertTag("<a href=\""+shortName+"\">","</a>");
   }
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
  QIconSet emptyIcon ( UserIcon("empty1x16"));

  QTabWidget *wTab = view->writeTab;
  while ( Document *twrite = it.current() )
  {

    if ( twrite->isModified() )
    	wTab->changeTab( twrite,  floppyIcon,  wTab->tabLabel(twrite) );
    else
    	wTab->changeTab( twrite,  emptyIcon,   wTab->tabLabel(twrite) );

    ++it;
  }

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

/** slot for new undo flag */
void QuantaApp::slotNewUndo()
{
	int state = doc->write()->undoState();
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
  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
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
	slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotUpdateStatus(const QString &)
{
	slotNewStatus();
	slotNewUndo();
	slotNewMarkStatus();
	slotNewLineColumn();
	
	setCaption(doc->getTitle());
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
	static int vSplitPos = 250;

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
  setCaption(doc->getTitle());
  if ( view->write()->numLines() > line && line >= 0 )
    view->write()->setCursorPosition( line, 0 );
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
  if ( doc->write()->hasFileName() ) {
    KProcess *p = new KProcess();
    *p << "perl";
    *p << locate("lib","quanta/plugins/weblint");
    *p << "-x" << "Netscape";
    *p << doc->write()->fileName();
    
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

