/***************************************************************************
                          quanta.cpp  -  description
                             -------------------
    begin                : Втр Май  9 13:29:57 EEST 2000
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

// include files for KDE
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kconfig.h>
#include <khtmlview.h>
#include <kdialogbase.h>

// application specific includes
#include "quanta.h"
#include "quantaview.h"
#include "quantadoc.h"
#include "document.h"
#include "resource.h"

#include "project/project.h"

#include "widgets/whtmlpart.h"
#include "widgets/wsplitter.h"

#include "dialogs/filesmask.h"
#include "dialogs/styleoptions.h"
#include "dialogs/previewoptions.h"

#include "treeviews/filestreeview.h"
#include "treeviews/fileslistview.h"
#include "treeviews/filestreefolder.h"
#include "treeviews/structtreeview.h"
#include "treeviews/structtreetag.h"

#include "treeviews/doctreeview.h"

#include "parser/parser.h"



/////////////////////////////////////////////////////////////////////
// SLOT CALLBACK IMPLEMENTATION
/////////////////////////////////////////////////////////////////////

void QuantaApp::commandCallback(int id_)
{
  switch (id_)
  {
    case ID_FILE_NEW_WINDOW:
         slotFileNewWindow();
    	 break;

    case ID_FILE_NEW:
    	 slotFileNew();
         break;

    case ID_FILE_OPEN:
         slotFileOpen();
         break;

    case ID_FILE_SAVE:
         slotFileSave();
         break;

    case ID_FILE_SAVE_AS:
         slotFileSaveAs();
         break;

    case ID_FILE_SAVE_ALL:
         slotFileSaveAll();
         break;

    case ID_FILE_CLOSE:
         slotFileClose();
         break;

    case ID_FILE_CLOSE_ALL:
         slotFileCloseAll();
         break;

    case ID_FILE_PRINT:
         slotFilePrint();
         break;

    case ID_FILE_QUIT:
         slotFileQuit();
         break;

    case ID_EDIT_CUT:
         slotEditCut();
         break;

    case ID_EDIT_COPY:
         slotEditCopy();
         break;

    case ID_EDIT_PASTE:
         slotEditPaste();
         break;

    case ID_EDIT_UNDO:
         slotEditUndo();
         break;

    case ID_EDIT_REDO:
         slotEditRedo();
         break;

    case ID_EDIT_UREDO_HISTORY:
         slotURedoHistory();
         break;

    case ID_EDIT_SELECT_ALL:
         slotEditSelectAll();
         break;

    case ID_EDIT_DESELECT_ALL:
         slotEditDeselectAll();
         break;

    case ID_EDIT_INVERT_SELECT:
         slotEditInvertSelect();
         break;

    case ID_EDIT_SEARCH:
         slotEditSearch();
         break;

    case ID_EDIT_SEARCH_AGAIN:
         slotEditSearchAgain();
         break;

    case ID_EDIT_REPLACE:
         slotEditReplace();
         break;

    case ID_EDIT_INDENT:
         slotEditIndent();
         break;

    case ID_EDIT_UNINDENT:
         slotEditUnindent();
         break;

    case ID_EDIT_GOTO_LINE:
         slotEditGotoLine();
         break;

    case ID_EDIT_CLEAN_INDENT:
         slotEditCleanIndent();
         break;

    case ID_EDIT_CURRENT_TAG:
         view->slotEditCurrentTag();
         break;
  
    case ID_VIEW_TOOLBAR:
         slotViewToolBar();
         break;

    case ID_VIEW_STATUSBAR:
         slotViewStatusBar();
         break;

    case ID_VIEW_REPAINT:
         slotViewRepaint();
         break;

    case ID_VIEW_BACK:
         htmlPartDoc->back();
         break;

    case ID_VIEW_FORWARD:
         htmlPartDoc->forward();
         break;

    case ID_VIEW_PREVIEW:
         slotShowPreview();
         break;

    case ID_PROJECT_NEW:
    		 project	-> newProject();
    		 leftPanel-> showPage( (QWidget *)pTab );
    		 break;

    case ID_PROJECT_OPEN:
    		 project	-> openProject();
    		 leftPanel-> showPage( (QWidget *)pTab );
    		 break;
    		
    case ID_PROJECT_CLOSE:
    		 project	-> closeProject();
    		 leftPanel-> showPage( (QWidget *)fTab );
    		 break;
    		
    case ID_PROJECT_ADD_FILE:
    		 project  -> addFiles();
    		 leftPanel-> showPage( (QWidget *)pTab );
    		 break;
    		
    case ID_PROJECT_ADD_DIRECTORY:
    		 project  -> addDirectory();
    		 leftPanel-> showPage( (QWidget *)pTab );
    		 break;

    case ID_OPTIONS:
    		 slotOptions();
    		 break;

    case ID_OPTIONS_EDITOR:
         doc->write()->editorOptions();
         break;

    case ID_OPTIONS_COLORS:
         // doc->write()->colDlg();
         break;

    case ID_OPTIONS_HIGHLIGHT:
         doc->write()->hlDlg();
         break;

    case ID_CONTEXT_HELP:
         contextHelp();
         break;

    default:
         break;
  }
}

void QuantaApp::statusCallback(int id_)
{
  switch (id_)
  {
    case ID_FILE_NEW_WINDOW:
         slotStatusHelpMsg(i18n("Opens a new application window"));
         break;

    case ID_FILE_NEW:
         slotStatusHelpMsg(i18n("Creates a new document"));
         break;

    case ID_FILE_OPEN:
         slotStatusHelpMsg(i18n("Opens an existing document"));
         break;

    case ID_FILE_OPEN_RECENT:
         slotStatusHelpMsg(i18n("Opens a recently used file"));
         break;

    case ID_FILE_SAVE:
         slotStatusHelpMsg(i18n("Saves the actual document"));
         break;

    case ID_FILE_SAVE_AS:
         slotStatusHelpMsg(i18n("Saves the actual document as..."));
         break;

    case ID_FILE_CLOSE:
         slotStatusHelpMsg(i18n("Closes the actual document"));
         break;

    case ID_FILE_PRINT:
         slotStatusHelpMsg(i18n("Prints out the actual document"));
         break;

    case ID_FILE_QUIT:
         slotStatusHelpMsg(i18n("Quits the application"));
         break;

    case ID_EDIT_CUT:
         slotStatusHelpMsg(i18n("Cuts the selected section and puts it to the clipboard"));
         break;

    case ID_EDIT_COPY:
         slotStatusHelpMsg(i18n("Copies the selected section to the clipboard"));
         break;

    case ID_EDIT_PASTE:
         slotStatusHelpMsg(i18n("Pastes the clipboard contents to actual position"));
         break;

    case ID_VIEW_TOOLBAR:
         slotStatusHelpMsg(i18n("Enables/disables the toolbar"));
         break;

    case ID_VIEW_STATUSBAR:
         slotStatusHelpMsg(i18n("Enables/disables the statusbar"));
         break;

    default:
         break;
  }
}

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

  #warning add options when need to repaint preview
  //repaintPreview();

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
	
  doc->openDocument(recentFilesMenu->text(id_));
  setCaption(doc->getTitle());

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotProjectOpenRecent(int id_)
{
  slotStatusMsg(i18n("Opening project..."));
	
  project->closeProject();
  project->loadProject (recentProjectsMenu->text(id_));
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
    QFileInfo saveAsInfo(newName);
    doc->setTitle(saveAsInfo.fileName());

    doc->saveDocument(newName);
    addRecentFile(newName);

    setCaption(doc->getTitle());
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

  htmlPart()->begin( KURL( doc->basePath() ));
 	htmlPart()->write( "" );
 	htmlPart()->end();

  setCaption(doc->getTitle());

  slotNewStatus();

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}


void QuantaApp::slotFilePrint()
{
  slotStatusMsg(i18n("Printing..."));

  QPrinter printer;
  if (printer.setup(this))
  {
   // view->print(&printer);
   #warning add print from source or html
  }

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

void QuantaApp::slotEditInvertSelect()
{
  slotStatusMsg(i18n("Invert Selection..."));

  doc->write()->invertSelection();

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
  ///////////////////////////////////////////////////////////////////
  // turn Toolbar on or off
  if( viewMenu->isItemChecked(ID_VIEW_TOOLBAR))
  {
    viewMenu->setItemChecked(ID_VIEW_TOOLBAR, false);
    enableToolBar(KToolBar::Hide);
  }
  else
  {
    viewMenu->setItemChecked(ID_VIEW_TOOLBAR, true);
    enableToolBar(KToolBar::Show);
  }		

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void QuantaApp::slotViewStatusBar()
{
  slotStatusMsg(i18n("Toggle the statusbar..."));
  ///////////////////////////////////////////////////////////////////
  //turn Statusbar on or off
  if( viewMenu->isItemChecked(ID_VIEW_STATUSBAR))
  {
    viewMenu->setItemChecked(ID_VIEW_STATUSBAR, false);
    enableStatusBar(KStatusBar::Hide);
  }
  else
  {
    viewMenu->setItemChecked(ID_VIEW_STATUSBAR, true);
    enableStatusBar(KStatusBar::Show);
  }

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
void QuantaApp::slotViewRepaint(){
	slotStatusMsg(i18n("Repainting..."));
	
	repaintPreview( true );
	
	slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

/** repaint preview */
void QuantaApp::repaintPreview( bool clear )
{
  WHTMLPart *part = htmlPart();
  if ( !part ) return;

  static QString oldtext = "";
  if ( clear )   oldtext = "";

  QWidgetStack *s = (QWidgetStack *)part-> parent();

  if ( !s->id( s ->visibleWidget()) ) return;

  QString text = doc->write()->text();

  if ( !qstrcmp( text, oldtext ) ) return;

	KHTMLView *html = part->view();

  int xOffset = html->contentsX(), yOffset = html->contentsY();

	part->begin( KURL( doc->basePath() ), xOffset, yOffset );
  part->write(text);
	part->end();

  part->show();

  oldtext = text;
}

/** view image in preview */
void QuantaApp::slotImageOpen( QString fileToOpen )
{

  if ( previewPosition == "Disabled" )
     return;

  WHTMLPart *part = htmlPart();
  if ( !part ) return;

  QWidgetStack *s = (QWidgetStack *)part-> parent();

  if ( !s->id( s ->visibleWidget()) ) return;

	QString text = "<html>\n<body>\n<div align=\"center\">\n<img src=\"";
	text += fileToOpen;
	text += "\">\n</div>\n</body>\n</html>\n";
	
	part->begin( KURL( doc->basePath() ) );
  part->write(text);
  part->end();

  part->show();
}

////////////////////////
// status slots
///////////////////////
/** slot for new modify flag */
void QuantaApp::slotNewStatus()
{
	
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
	
  QDictIterator<Document> it( doc->docList );

  QIconSet floppyIcon( UserIcon("save_small.xpm"));
  QIconSet emptyIcon ( UserIcon("empty1x16.xpm"));

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
  if(state & 1)
 		enableCommand( ID_EDIT_UNDO);
  else
		disableCommand(ID_EDIT_UNDO);
  if(state & 2)
    enableCommand( ID_EDIT_REDO);
  else
    disableCommand(ID_EDIT_REDO);

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

/** slot for new mark flag */
void QuantaApp::slotNewMarkStatus()
{
	if ( doc->write()->hasMarkedText() ) {
		enableCommand(ID_EDIT_CUT);
    enableCommand(ID_EDIT_COPY);
	}
	else {
		disableCommand(ID_EDIT_CUT);
    disableCommand(ID_EDIT_COPY);
	}
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

void QuantaApp::slotOptions()
{
	KDialogBase *kd = new KDialogBase(KDialogBase::IconList,
				    i18n("Configure Quanta"),
				    KDialogBase::Ok | KDialogBase::Cancel,
				    KDialogBase::Ok, this, "tabdialog");
				
	// Tag Style options
  QVBox *page=kd->addVBoxPage(i18n("Tag Style"), QString::null, UserIcon( "tagstyle" ) );
  StyleOptions *styleOptions = new StyleOptions( (QWidget *)page );

  styleOptions->checkTagsCapital->setChecked( tagsCapital);
  styleOptions->checkAttrCapital->setChecked( attrCapital);
  styleOptions->checkEndTag->setChecked( useCloseTag );


  // Files Masks options
  page=kd->addVBoxPage(i18n("Files Masks"), QString::null, UserIcon( "filesmask" ) );
  FilesMask *filesMask = new FilesMask( (QWidget *)page );

  filesMask->lineHTML->setText( fileMaskHtml );
  filesMask->linePHP->setText( fileMaskPhp );
  filesMask->lineImages->setText( fileMaskImage );
  filesMask->lineText->setText( fileMaskText );

  // Preview options
  page=kd->addVBoxPage(i18n("Preview"), QString::null, UserIcon( "previewoptions" ) );
  PreviewOptions *previewOptions = new PreviewOptions( (QWidget *)page );

  previewOptions->setPosition( previewPosition );

  // Keys options
  page=kd->addVBoxPage(i18n("Keys"), QString::null, UserIcon( "keysconfig" ) );
//  KGuiCmdConfigTab *keys = new KGuiCmdConfigTab((QWidget *)page, KGuiCmdManager::self());

  //IndentConfigTab *indentConfig = new IndentConfigTab(page, kWrite);

  if ( kd->exec() )
  {
    tagsCapital = styleOptions->checkTagsCapital->isChecked();
    attrCapital = styleOptions->checkAttrCapital->isChecked();
    useCloseTag = styleOptions->checkEndTag->isChecked();

    QWidgetStack *s;
    if ( htmlPart() ) {
    	s = (QWidgetStack *)htmlPart()->parent();
	    s -> raiseWidget( 0 );
	 	}
    checkCommand( ID_VIEW_PREVIEW, false );

  	previewPosition = previewOptions->position();
  	
  	htmlPartTop->begin( KURL( doc->basePath() ));
  	htmlPartTop->write( "" );
  	htmlPartTop->end();
  	
  	htmlPartRight->begin( KURL( doc->basePath() ));
  	htmlPartRight->write( "" );
  	htmlPartRight->end();
  	
  	htmlPartBottom->begin( KURL( doc->basePath() ));
  	htmlPartBottom->write( "" );
  	htmlPartBottom->end();
  	
  	repaintPreview(true);
  }

  config->sync();

  saveOptions();

  delete kd;
}

////////////////////////
// editor commands
///////////////////////
void QuantaApp::doCursorCommand(int cmdNum) {
  //view->write()->doCursorCommand(cmdNum);
}

void QuantaApp::doEditCommand(int cmdNum) {
  //view->write()->doEditCommand(cmdNum);
}

void QuantaApp::doStateCommand(int cmdNum) {
  //view->write()->doStateCommand(cmdNum);
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
	QWidgetStack *s = (QWidgetStack *)part->parent();
	
	enableCommand(ID_VIEW_BACK);
  enableCommand(ID_VIEW_FORWARD);
	enableCommand(ID_VIEW_REPAINT);
	s->raiseWidget( 1 );
	
	checkCommand( ID_VIEW_PREVIEW, true );
}

void QuantaApp::slotShowPreview()
{
	WHTMLPart *part = htmlPart();
	if ( !part ) return;
	QWidgetStack *s = (QWidgetStack *)part->parent();
	bool stat = viewMenu -> isItemChecked( ID_VIEW_PREVIEW );
	if ( stat ) {
		disableCommand(ID_VIEW_BACK);
		disableCommand(ID_VIEW_FORWARD);
		disableCommand(ID_VIEW_REPAINT);
		s->raiseWidget( 0 );
	}
	else {
		enableCommand(ID_VIEW_BACK);
		enableCommand(ID_VIEW_FORWARD);
		enableCommand(ID_VIEW_REPAINT);
		s->raiseWidget( 1 );
		repaintPreview(false);
	}
	checkCommand( ID_VIEW_PREVIEW, !stat );
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
		if ( parser->textChanged )
			sTab->slotReparse( node , 3 );
		// delete node;
	}
}

void QuantaApp::setCursorPosition( int row, int col )
{
  view->write()->setCursorPosition( row, col );
	view->write()->view()->setFocus();
}

/*
void QuantaApp::slotStructTreeOpen()
{
  Node *node = parser->parse( view->write()->text() );
	sTab->s = parser->s;
	sTab->slotReparse( node,true );
}

void QuantaApp::slotStructTreeClose()
{
  Node *node = parser->parse( view->write()->text() );
	sTab->s = parser->s;
	sTab->slotReparse( node,false );
}
*/

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
  	rightWidgetStack -> raiseWidget(2);
  	docTabOpened = true;
  }
  else {
    if ( docTabOpened )
    	rightWidgetStack -> raiseWidget(0);
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
   htmlPartDoc->openURL(url);
	 htmlPartDoc->show();
	 htmlPartDoc->addToHistory(url);
}

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
   		
}

void QuantaApp::contextHelp()
{
   QString curWord = view->write()->currentWord();
   QString * url = dTab->contextHelp( curWord );

   if ( url ) {
      leftPanel->showPage(dTab);
   		openDoc(*url);
   }
}
