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
#include <qtabwidget.h>
#include <qwidgetstack.h>
#include <qlayout.h>
#include <qtoolbutton.h>

// include files for KDE
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kconfig.h>
#include <khtmlview.h>
#include <kstddirs.h>

// application specific includes
#include "quanta.h"
#include "quantaview.h"
#include "quantadoc.h"
#include "resource.h"
#include "document.h"

#include "kwrite/kwdoc.h"
#include "kwrite/kwview.h"

#include "project/project.h"

#include "widgets/wtoolbar.h"
#include "widgets/whtmlpart.h"
#include "widgets/wsplitter.h"
#include "messages/messageoutput.h"

#include "treeviews/fileslistview.h"
#include "treeviews/filestreeview.h"
#include "treeviews/projecttreeview.h"
#include "treeviews/doctreeview.h"
#include "treeviews/structtreeview.h"

#include "phpdebug/phpdebugserversocket.h"

#include "parser/parser.h"

#include "toolbar/toolbars.h"

QString fileMaskHtml 	= "*.*html *.*htm *.php* *.asp *.cfm *.css *.inc* *.*HTML *.*HTM *.PHP* *.ASP *.CFM *.CSS *.INC* *.xml *.XML";
QString fileMaskPhp 	= "*.*PHP* *.*php* ";
QString fileMaskJava  = "*.jss *.js *.JSS *.JS ";
QString fileMaskText  = "*.txt *.TXT ";
QString fileMaskImage = "*.gif *.jpg *.png *.jpeg *.bmp *.xpm *.GIF *.JPG *.PNG *.JPEG *.BMP ";

QStrList *tagsList; // list of known tags
QStrList *tagsCore; // list of tags, allowed core attributes ( id, class, id, title )
QStrList *tagsI18n; // list of tags, allowed i18 attribs.
QStrList *tagsScript; // list of tags, allowed script attribs ( onClicl, omMouseOver... )
QStrList *quotedAttribs; // list of attribs, that have quoted values ( alt, src ... )
QStrList *lCore;          // list of core attributes ( id, class, style ... )
QStrList *lI18n;
QStrList *lScript;
QStrList *singleTags; // tags without end  part </ >
QStrList *optionalTags; // tags with optional end part

bool tagsCapital;
bool attrCapital;
bool useCloseTag;

QDict <QStrList> *tagsDict;

// extern KGuiCmdManager cmdMngr;

//
// Enable this define to test the KAction/XMLGUI code (Rich).
//
// #define NEW_STUFF
//

#ifdef NEW_STUFF

#include <kaction.h>
#include <kstdaction.h>

#endif // NEW_STUFF

QuantaApp::QuantaApp()
{
  grepDialog	= 0;

  config=kapp->config();

  parser = new Parser();

  ///////////////////////////////////////////////////////////////////
  // call inits to invoke all other construction parts
#ifndef NEW_STUFF
  initMenuBar();
  initToolBar();
  initStatusBar();
  initTagDict();
  initDocument();
  initView();
  initKeyAccel();
  initProject();
#else
  initMenuBar();
  initStatusBar();
  initTagDict();
  initDocument();
  initView();
  initProject();
  doc->newDocument();

  initActions();
  createGUI( QString::null, false );

  initContextMenu();
#endif
	
  readOptions();
  
  PhpDebugServerSocket *debugger = 
    new PhpDebugServerSocket( phpDebugPort,0,0);
    
  connect( debugger,      SIGNAL(newConnect()),
           messageOutput, SLOT(newPhpConnect()) );
  connect( debugger,      SIGNAL(endConnect()),
           messageOutput, SLOT(endPhpConnect()) );
  connect( debugger,      SIGNAL(data(QString)),
           messageOutput, SLOT(phpDebug(QString)) );
  connect( messageOutput, SIGNAL(clicked(QString,int)),
           this,          SLOT(slotGoToError(QString,int)));
           
  if ( !debugger->ok() ) {
    QString s;
	  messageOutput->insertItem("\nPhp Debugger:\t\tSorry, but I can't listen port N "+
	    s.sprintf("%i",phpDebugPort)+"\n" );
	}

  ///////////////////////////////////////////////////////////////////
  // disable menu and toolbar items at startup

  disableCommand(ID_FILE_SAVE);
  disableCommand(ID_FILE_SAVE_ALL);
  disableCommand(ID_FILE_PRINT);
 	
  disableCommand(ID_EDIT_CUT);
  disableCommand(ID_EDIT_COPY);
	disableCommand(ID_EDIT_UNDO);
  disableCommand(ID_EDIT_REDO);

  disableCommand(ID_VIEW_BACK);
  disableCommand(ID_VIEW_FORWARD);
  disableCommand(ID_VIEW_REPAINT);

  disableCommand(ID_OPTIONS_COLORS); // don't working kwrite bug

  disableCommand(ID_BOOKMARKS_SET);
  disableCommand(ID_BOOKMARKS_ADD);
  disableCommand(ID_BOOKMARKS_CLEAR);

#ifndef NEW_STUFF
  doc->newDocument();
#endif
}

QuantaApp::~QuantaApp()
{
}

void QuantaApp::initKeyAccel()
{
  keyAccel = new KAccel(this);

  keyAccel->insertItem( "Undo", Key_Z+CTRL);
  keyAccel->connectItem("Undo",   this, SLOT(slotEditUndo()));

  keyAccel->insertItem( "Find", Key_F+CTRL);
  keyAccel->connectItem("Find",   this, SLOT(slotEditSearch()));

  keyAccel->insertItem( "Replace", Key_R+CTRL);
  keyAccel->connectItem("Replace",this, SLOT(slotEditReplace()));

  keyAccel->insertItem( "Redo", Key_Y+CTRL);
  keyAccel->connectItem("Redo", this, SLOT(slotEditRedo()));

  keyAccel->insertItem( "Find next", Key_F3);
  keyAccel->connectItem("Find next", this, SLOT(slotEditSearchAgain()));

  keyAccel->insertItem( "Tag attributes", Key_Down+ALT);
  keyAccel->connectItem("Tag attributes", doc, SLOT(slotAttribPopup()));

  keyAccel->connectItem(KStdAccel::Help, this, SLOT(appHelpActivated()));

  keyAccel->insertItem("Edit current tag",Key_F4);
  keyAccel->connectItem("Edit current tag",view,SLOT(slotEditCurrentTag()));

  keyAccel->insertItem("Context help",Key_H+CTRL);
  keyAccel->connectItem("Context help",this,SLOT( contextHelp() ) );

  keyAccel->insertItem("Indent",ALT+SHIFT+Key_Right);
  keyAccel->connectItem("Indent",this,SLOT( slotEditIndent() ) );

  keyAccel->insertItem("Unindent",ALT+SHIFT+Key_Left);
  keyAccel->connectItem("Unindent",this,SLOT( slotEditUnindent() ) );

  keyAccel->insertItem("Refresh Preview",Key_F5);
  keyAccel->connectItem("Refresh Preview",this,SLOT(slotViewRepaint()));

  keyAccel->insertItem("Preview",Key_F6);
  keyAccel->connectItem("Preview",this,SLOT(slotShowPreview()));

  keyAccel->insertItem("Reparse",Key_X+CTRL);
  keyAccel->connectItem("Reparse",this,SLOT(reparse()));

  keyAccel->insertItem("<br>",Key_Return+CTRL);
  keyAccel->connectItem("<br>",view,SLOT(slotTagBr()));

  keyAccel->insertItem("<img>",Key_I+ALT);
  keyAccel->connectItem("<img>",view,SLOT(slotTagImg()));

  keyAccel->insertItem("<a>",Key_A+ALT);
  keyAccel->connectItem("<a>",view,SLOT(slotTagA()));

  keyAccel->insertItem("#color",Key_C+ALT);
  keyAccel->connectItem("#color",view,SLOT(slotTagColor()));

  keyAccel->insertItem("<td>",0);
  keyAccel->connectItem("<td>",view,SLOT(slotTagTableData()));

  keyAccel->insertItem("<tr>",0);
  keyAccel->connectItem("<tr>",view,SLOT(slotTagTableRow()));

  keyAccel->insertItem("<th>",0);
  keyAccel->connectItem("<th>",view,SLOT(slotTagTableHead()));

  keyAccel->insertItem("<table>",Key_T+ALT);
  keyAccel->connectItem("<table>",view,SLOT(slotTagQuickTable()));

  keyAccel->insertItem("<font>",Key_F+ALT);
  keyAccel->connectItem("<font>",view,SLOT(slotTagFont()));

  keyAccel->insertItem("<ul> / <ol>",Key_L+ALT);
  keyAccel->connectItem("<ul> / <ol>",view,SLOT(slotTagQuickList()));

  keyAccel->insertItem("Preview in Konqueror",0);
  keyAccel->connectItem("Preview in Konqueror",view,SLOT(slotViewInKFM()));

  keyAccel->insertItem("Preview in Netscape",Key_F5+SHIFT);
  keyAccel->connectItem("Preview in Netscape",view,SLOT(slotViewInNetscape()));

  keyAccel->insertItem("&nbsp;",Key_Space+CTRL);
  keyAccel->connectItem("&nbsp;",view,SLOT(slotTagNbsp()));

  keyAccel->insertItem("&lt;",Key_Less+CTRL);
  keyAccel->connectItem("&lt;",view,SLOT(slotTagLt()));

  keyAccel->insertItem("&gt;",Key_Greater+CTRL);
  keyAccel->connectItem("&gt;",view,SLOT(slotTagGt()));

  keyAccel->insertItem("goto line", ALT+Key_N);
  keyAccel->connectItem("goto line",this,SLOT(slotEditGotoLine()));

  keyAccel->insertItem("Next file",CTRL+ALT+Key_Right);
	keyAccel->connectItem("Next file", this, SLOT(slotFileNext()));

	keyAccel->insertItem("Previous file",CTRL+ALT+Key_Left);
	keyAccel->connectItem("Previous file", this, SLOT(slotFilePrev()));

  keyAccel->readSettings();

  keyAccel->connectItem(KStdAccel::New,   this, SLOT(slotFileNew()));
  keyAccel->connectItem(KStdAccel::Open,  this, SLOT(slotFileOpen()));
  keyAccel->connectItem(KStdAccel::Save,  this, SLOT(slotFileSave()));
  keyAccel->connectItem(KStdAccel::Close, this, SLOT(slotFileClose()));
  keyAccel->connectItem(KStdAccel::Print, this, SLOT(slotFilePrint()));
  keyAccel->connectItem(KStdAccel::Quit,  this, SLOT(slotFileQuit()));
  keyAccel->connectItem(KStdAccel::Cut,   this, SLOT(slotEditCut()));
  keyAccel->connectItem(KStdAccel::Copy,  this, SLOT(slotEditCopy()));
  keyAccel->connectItem(KStdAccel::Paste, this, SLOT(slotEditPaste()));

  keyAccel->changeMenuAccel(fileMenu, ID_FILE_NEW,   KStdAccel::New);
  keyAccel->changeMenuAccel(fileMenu, ID_FILE_OPEN,  KStdAccel::Open);
  keyAccel->changeMenuAccel(fileMenu, ID_FILE_SAVE,  KStdAccel::Save);
  keyAccel->changeMenuAccel(fileMenu, ID_FILE_CLOSE, KStdAccel::Close);
  keyAccel->changeMenuAccel(fileMenu, ID_FILE_PRINT, KStdAccel::Print);
  keyAccel->changeMenuAccel(fileMenu, ID_FILE_QUIT,  KStdAccel::Quit);

  keyAccel->changeMenuAccel(editMenu, ID_EDIT_CUT,   KStdAccel::Cut);
  keyAccel->changeMenuAccel(editMenu, ID_EDIT_COPY,  KStdAccel::Copy);
  keyAccel->changeMenuAccel(editMenu, ID_EDIT_PASTE, KStdAccel::Paste);
  keyAccel->changeMenuAccel(editMenu, ID_EDIT_UNDO,         "Undo");
  keyAccel->changeMenuAccel(editMenu, ID_EDIT_REDO,         "Redo");
  keyAccel->changeMenuAccel(editMenu, ID_EDIT_SEARCH,       "Find");
  keyAccel->changeMenuAccel(editMenu, ID_EDIT_REPLACE,      "Replace");
  keyAccel->changeMenuAccel(editMenu, ID_EDIT_SEARCH_AGAIN, "Find next");

  keyAccel->changeMenuAccel(insertStandardMenu, ID_TAG_BR,          "<br>");
  keyAccel->changeMenuAccel(insertMenu,         ID_TAG_IMAGE,       "<img>");
  keyAccel->changeMenuAccel(insertMenu,         ID_TAG_A,           "<a>");
  keyAccel->changeMenuAccel(insertMenu,         ID_TAG_COLOR,       "#color");
  keyAccel->changeMenuAccel(insertTablesMenu,   ID_TAG_TABLE_DATA,  "<td>");
  keyAccel->changeMenuAccel(insertTablesMenu,   ID_TAG_TABLE_ROW,   "<tr>");
  keyAccel->changeMenuAccel(insertTablesMenu,   ID_TAG_TABLE_HEAD,  "<th>");
  keyAccel->changeMenuAccel(insertMenu,         ID_TAG_QUICK_TABLE, "<table>");
  keyAccel->changeMenuAccel(insertMenu,         ID_TAG_FONT,        "<font>");
  keyAccel->changeMenuAccel(insertMenu,         ID_TAG_QUICK_LIST,  "<ul> / <ol>");
  keyAccel->changeMenuAccel(insertStandardMenu, ID_TAG_NBSP,        "&nbsp;");

  keyAccel->changeMenuAccel(viewMenu, ID_VIEW_IN_KFM2,     "Preview in Konqueror");
  keyAccel->changeMenuAccel(viewMenu, ID_VIEW_IN_NETSCAPE, "Preview in Netscape");
  keyAccel->changeMenuAccel(viewMenu, ID_VIEW_REPAINT,     "Refresh Preview");
  keyAccel->changeMenuAccel(viewMenu, ID_VIEW_PREVIEW,     "Preview");
  keyAccel->changeMenuAccel(viewMenu, ID_VIEW_PREV_FILE,     "Previous file");
  keyAccel->changeMenuAccel(viewMenu, ID_VIEW_NEXT_FILE,     "Next file");

  keyAccel->changeMenuAccel(toolMenu, ID_EDIT_CURRENT_TAG, "Edit current tag");
  keyAccel->changeMenuAccel(toolMenu, ID_ATTRIB_POPUP,     "Tag attributes");
  keyAccel->changeMenuAccel(toolMenu, ID_CONTEXT_HELP,     "Context help");
  keyAccel->changeMenuAccel(toolMenu, ID_EDIT_INDENT,      "Indent");
  keyAccel->changeMenuAccel(toolMenu, ID_EDIT_UNINDENT,    "Unindent");
  keyAccel->changeMenuAccel(toolMenu, ID_EDIT_GOTO_LINE,   "goto line");

  // keyAccel->changeMenuAccel(sTab->popupMenu, ID_REPARSE,   "Reparse");
}

void QuantaApp::initMenuBar()
{
  ///////////////////////////////////////////////////////////////////
  // MENUBAR

  recentFilesMenu = new QPopupMenu();
  connect(recentFilesMenu, SIGNAL(activated(int)), SLOT(slotFileOpenRecent(int)));

  ///////////////////////////////////////////////////////////////////
  // menuBar entry fileMenu
  fileMenu = new QPopupMenu();
  fileMenu->insertItem(UserIcon("new"), i18n("&New"), ID_FILE_NEW);
  fileMenu->insertItem(UserIcon("open"), i18n("&Open..."), ID_FILE_OPEN);
  fileMenu->insertItem(i18n("Open &recent"), recentFilesMenu, ID_FILE_OPEN_RECENT);

  fileMenu->insertItem(i18n("&Close"), 		ID_FILE_CLOSE);
  fileMenu->insertItem(i18n("Close All"), ID_FILE_CLOSE_ALL);
  fileMenu->insertSeparator();

//  fileMenu->insertItem(UserIcon("upload_file"),      i18n("&Upload"),        ID_PROJECT_UPLOAD_FILE);
//  fileMenu->insertSeparator();

  fileMenu->insertItem(UserIcon("save") ,i18n("&Save"), ID_FILE_SAVE);
  fileMenu->insertItem(i18n("Save &As..."), ID_FILE_SAVE_AS);
  fileMenu->insertItem(UserIcon("save_all"),  i18n("Save All..."),  ID_FILE_SAVE_ALL);
  fileMenu->insertSeparator();
//  fileMenu->insertItem(BarIcon("fileprint"), i18n("&Print..."), ID_FILE_PRINT);
//  fileMenu->insertSeparator();
  fileMenu->insertItem(UserIcon("exit"), i18n("E&xit"), ID_FILE_QUIT);
	

  ///////////////////////////////////////////////////////////////////
  // menuBar entry editMenu
  editMenu = new QPopupMenu();
  editMenu ->setCheckable(true);

  editMenu->insertItem(UserIcon("undo"),i18n("&Undo"), ID_EDIT_UNDO);
  editMenu->insertItem(UserIcon("redo"),i18n("&Redo"), ID_EDIT_REDO);
  editMenu->insertItem(i18n("Undo/Redo &History..."),  ID_EDIT_UREDO_HISTORY);

  editMenu->insertSeparator();

  editMenu->insertItem(UserIcon("cut"), 	i18n("Cu&t"), 	ID_EDIT_CUT);
  editMenu->insertItem(UserIcon("copy"), 	i18n("&Copy"), 	ID_EDIT_COPY);
  editMenu->insertItem(UserIcon("paste"), i18n("&Paste"), ID_EDIT_PASTE);
  editMenu->insertItem(i18n("Select A&ll"),       ID_EDIT_SELECT_ALL);
  editMenu->insertItem(i18n("&Deselect All"),     ID_EDIT_DESELECT_ALL);
  editMenu->insertItem(i18n("&Invert Selection"), ID_EDIT_INVERT_SELECT);
  editMenu->insertItem(i18n("&Vertical Selection"), ID_EDIT_VERTICAL_SELECT);

  editMenu->insertSeparator();

  editMenu->insertItem(UserIcon("find"),      i18n("&Search..."),      ID_EDIT_SEARCH);
  editMenu->insertItem(UserIcon("findnext"),  i18n("Search &Again"),ID_EDIT_SEARCH_AGAIN);
  editMenu->insertItem(UserIcon("replace"),   i18n("R&eplace..."),     ID_EDIT_REPLACE);

  editMenu->insertSeparator();

  editMenu->insertItem(UserIcon("find"), i18n("Search In &Files"), ID_EDIT_FIND_IN_FILES );

  ///////////////////////////////////////////////////////////////////
  // menuBar entry toolMenu
  toolMenu = new QPopupMenu();
  toolMenu->setCheckable(true);
  toolMenu->insertItem( i18n("&Goto Line..."),         ID_EDIT_GOTO_LINE);

  toolMenu->insertSeparator();

  toolMenu->insertItem( i18n("&Indent"),            ID_EDIT_INDENT);
  toolMenu->insertItem( i18n("&Unindent"),          ID_EDIT_UNINDENT);
  toolMenu->insertItem( i18n("&Clean Indentation"), ID_EDIT_CLEAN_INDENT);

  toolMenu->insertSeparator();

  toolMenu->insertItem( i18n("Context Help..."),  		  ID_CONTEXT_HELP);
  toolMenu->insertItem( i18n("Tag Attributes"),  		ID_ATTRIB_POPUP);
  toolMenu->insertItem( i18n("&Edit Current Tag..."),  ID_EDIT_CURRENT_TAG);

  toolMenu->insertSeparator();

  toolMenu->insertItem(UserIcon("spellcheck"),i18n("Spe&lling..."),		ID_EDIT_SPELL);
  toolMenu->insertItem(UserIcon("ftpclient"),	i18n("&Ftp client..."),	ID_VIEW_FTP);
  toolMenu->insertItem( 	i18n("&Syntax check..."),	ID_TOOL_SYNTAX_CHECK);
 

  ///////////////////////////////////////////////////////////////////
  // menuBar entry viewMenu
  viewMenu = new QPopupMenu();
  viewMenu->setCheckable(true);
  viewMenu->insertItem(i18n("Show &Toolbar"), 	ID_VIEW_TOOLBAR);
  viewMenu->insertItem(i18n("Show &Statusbar"),ID_VIEW_STATUSBAR);

  viewMenu->insertItem(UserIcon("tree_win"),	i18n("Show Tr&ee"),    ID_VIEW_TREE);
  viewMenu->insertItem(i18n("Show &Messages"), ID_VIEW_MES);
  viewMenu->insertItem(UserIcon("preview"), 	i18n("&Preview"), 		ID_VIEW_PREVIEW);
  viewMenu->insertSeparator();
  viewMenu->insertItem(UserIcon("back"),   		i18n("&Back"), 		ID_VIEW_BACK);
  viewMenu->insertItem(UserIcon("forward"),   i18n("&Forward"), ID_VIEW_FORWARD);
  viewMenu->insertItem(UserIcon("repaint"),   i18n("&Refresh Preview"), ID_VIEW_REPAINT);
  viewMenu->insertSeparator();
  viewMenu->insertItem(i18n("Open Document In Netscape"), ID_VIEW_IN_NETSCAPE);
  viewMenu->insertItem(i18n("Open Document In Konqueror"),      ID_VIEW_IN_KFM2);
  viewMenu->insertSeparator();
  viewMenu->insertItem(i18n("Previous file"), ID_VIEW_PREV_FILE);
  viewMenu->insertItem(i18n("Next file"),      ID_VIEW_NEXT_FILE);

  ///////////////////////////////////////////////////////////////////
  // menuBar entry insertMenu
  insertMenu = new QPopupMenu();

  recentProjectsMenu = new QPopupMenu();
  connect(recentProjectsMenu, SIGNAL(activated(int)), SLOT(slotProjectOpenRecent(int)));
  ///////////////////////////////////////////////////////////////////
  // menuBar entry projectMenu
  projectMenu = new QPopupMenu();
  projectMenu->insertItem(i18n("&New Project..."),    ID_PROJECT_NEW);
  projectMenu->insertItem(UserIcon("openprj"),    i18n("&Open Project..."),  ID_PROJECT_OPEN);
  projectMenu->insertItem(i18n("Open &Recent Project"),recentProjectsMenu, ID_PROJECT_OPEN_RECENT);
  projectMenu->insertItem(i18n("&Close Project"),         ID_PROJECT_CLOSE);
  projectMenu->insertSeparator();
  projectMenu->insertItem(i18n("&Insert File(s)..."),   ID_PROJECT_ADD_FILE);
  projectMenu->insertItem(i18n("Insert &Directory..."), ID_PROJECT_ADD_DIRECTORY);
  projectMenu->insertSeparator();
  projectMenu->insertItem(i18n("&Upload project..."),   ID_PROJECT_UPLOAD_PROJECT);
  projectMenu->insertItem(i18n("&Project options..."), ID_PROJECT_PROPERTIES);
//  projectMenu->insertItem(i18n("Rescan project's directory"), ID_PROJECT_RESCAN_FILES);

  ///////////////////////////////////////////////////////////////////
  // menuBar entry bookmarksmenu
  bookmarksMenu = new QPopupMenu();
  bookmarksMenu->insertItem(i18n("&Set Bookmark"),   ID_BOOKMARKS_SET);
  bookmarksMenu->insertItem(UserIcon("bookmark"),    i18n("&Add Bookmark"),   ID_BOOKMARKS_ADD);
  bookmarksMenu->insertItem(i18n("&Clear Bookmarks"),ID_BOOKMARKS_CLEAR);

  ///////////////////////////////////////////////////////////////////
  // menuBar entry optionsmenu
  optionsMenu = new QPopupMenu();
  optionsMenu ->insertItem(i18n("&Highlighting..."),       ID_OPTIONS_HIGHLIGHT );

  highlightMenu = new QPopupMenu();

  for (int z = 0; z < HlManager::self()->highlights(); z++)
    highlightMenu -> insertItem( i18n(HlManager::self()->hlName(z)) );

  optionsMenu->insertItem(i18n("Highlighting Mode"), highlightMenu );
  connect( highlightMenu, SIGNAL(activated(int)), this, SLOT(slotSetHl(int)));

  optionsMenu->insertItem(i18n("&Editor options..."),   ID_OPTIONS_EDITOR);
  optionsMenu->insertItem(i18n("Configure &key bindings..."),   ID_OPTIONS_KEYS);
  optionsMenu->insertItem(i18n("Configure &toolbars..."),   ID_OPTIONS_TOOLBARS);
  optionsMenu->insertSeparator();
  optionsMenu->insertItem(UserIcon("options"), i18n("&General Options..."),  ID_OPTIONS );

  ///////////////////////////////////////////////////////////////////

  menuBar()->insertItem(i18n(" &File "), fileMenu);
  menuBar()->insertItem(i18n(" &Edit "), editMenu);
  menuBar()->insertItem(i18n(" &Tool "), toolMenu);
  menuBar()->insertItem(i18n(" &View "), viewMenu);
  menuBar()->insertItem(i18n(" &Project "), projectMenu);

  //menuBar()->insertItem(i18n(" &Bookmarks "), bookmarksMenu);
  menuBar()->insertItem(i18n(" &Options "), optionsMenu);

  menuBar()->insertSeparator();
  menuBar()->insertItem(i18n("&Help"), helpMenu(QString::null,false));


  ///////////////////////////////////////////////////////////////////
  // CONNECT THE MENU SLOTS WITH SIGNALS
  // for execution slots and statusbar messages

  connect(fileMenu, SIGNAL(activated(int)), SLOT(commandCallback(int)));
  connect(fileMenu, SIGNAL(highlighted(int)), SLOT(statusCallback(int)));

  connect(editMenu, SIGNAL(activated(int)), SLOT(commandCallback(int)));
  connect(editMenu, SIGNAL(highlighted(int)), SLOT(statusCallback(int)));

  connect(toolMenu, SIGNAL(activated(int)), SLOT(commandCallback(int)));
  connect(toolMenu, SIGNAL(highlighted(int)), SLOT(statusCallback(int)));

  connect(viewMenu, SIGNAL(activated(int)), SLOT(commandCallback(int)));
  connect(viewMenu, SIGNAL(highlighted(int)), SLOT(statusCallback(int)));

  connect(projectMenu, SIGNAL(activated(int)), SLOT(commandCallback(int)));
  connect(projectMenu, SIGNAL(highlighted(int)), SLOT(statusCallback(int)));

  connect(optionsMenu, SIGNAL(activated(int)), SLOT(commandCallback(int)));
  connect(optionsMenu, SIGNAL(highlighted(int)), SLOT(statusCallback(int)));
}

void QuantaApp::initToolBar()
{

  QFile f( locate("appdata","toolbars.rc") );

	f.open( IO_ReadOnly );

	QDomDocument doc;
	doc.setContent( &f );
  toolbars = new ToolBars(doc);
  
  ///////////////////////////////////////////////////////////////////
  // TOOLBAR
#ifndef NEW_STUFF
  toolBar()->insertButton(UserIcon("tree_win"),   ID_VIEW_TREE,     true, i18n("View tree"));
  toolBar()->insertButton(UserIcon("preview"),    ID_VIEW_PREVIEW,  true, i18n("Preview"));

  toolBar()->setToggle(ID_VIEW_PREVIEW);
  toolBar()->setToggle(ID_VIEW_TREE);
  toolBar()->setToggle(ID_VIEW_MES);

  toolBar()->insertButton(BarIcon("back"),      ID_VIEW_BACK,    true, i18n("Back"));
  toolBar()->insertButton(BarIcon("forward"),   ID_VIEW_FORWARD, true, i18n("Forward"));
  toolBar()->insertButton(BarIcon("reload"),    ID_VIEW_REPAINT, true, i18n("Reload Preview"));
  
  toolBar()->setFullSize(false);

  toolbar1 = new KToolBar(this);
  toolbar1->setFullSize(false);
  
  toolbar1->insertButton(UserIcon("new"),     ID_FILE_NEW,      true, i18n("<b>New</b> File<br>Create new <i>empty</i> document"));
  toolbar1->insertButton(UserIcon("open"),    ID_FILE_OPEN,     true, i18n("<b>Open</b> existing <i>File</i> from disk"));
  toolbar1->insertButton(UserIcon("openprj"), ID_PROJECT_OPEN,  true, i18n("<b>Open</b> existing <i>Web-Workspace</i> from disk"));
  
  toolbar1->insertSeparator();

  toolbar1->insertButton(UserIcon("save"),    ID_FILE_SAVE,     true, i18n("Save File"));
  toolbar1->insertButton(UserIcon("save_all"),ID_FILE_SAVE_ALL, true, i18n("Save All Files"));

  addToolBar(toolbar1);

  toolbar2 = new KToolBar(this);
  toolbar2->setFullSize(false);
  
  toolbar2->insertButton(UserIcon("cut"),   ID_EDIT_CUT, true, i18n("Cut"));
  toolbar2->insertButton(UserIcon("copy"),  ID_EDIT_COPY, true, i18n("Copy"));
  toolbar2->insertButton(UserIcon("paste"), ID_EDIT_PASTE, true, i18n("Paste"));

  toolbar2->insertButton(UserIcon("undo"),   ID_EDIT_UNDO,   true, i18n("Undo"));
  toolbar2->insertButton(UserIcon("redo"),   ID_EDIT_REDO,   true, i18n("Redo"));

  toolbar2->insertButton(UserIcon("spellcheck"),   ID_EDIT_SPELL,   true, i18n("Spell Checker"));

  toolbar2->insertSeparator();
  toolbar2->insertSeparator();

  toolbar2->insertButton(UserIcon("find"),  		ID_EDIT_SEARCH,  			true, i18n("Search"));
  toolbar2->insertButton(UserIcon("findnext"),	ID_EDIT_SEARCH_AGAIN, true, i18n("Search Again"));
  
  addToolBar(toolbar2);
  
  connect(toolbar1, SIGNAL(clicked(int)), SLOT(commandCallback(int)));
  connect(toolbar1, SIGNAL(pressed(int)), SLOT(statusCallback(int)));
  
  connect(toolbar2, SIGNAL(clicked(int)), SLOT(commandCallback(int)));
  connect(toolbar2, SIGNAL(pressed(int)), SLOT(statusCallback(int)));
  
  connect(toolBar(), SIGNAL(clicked(int)), SLOT(commandCallback(int)));
  connect(toolBar(), SIGNAL(pressed(int)), SLOT(statusCallback(int)));
#endif
}

void QuantaApp::initStatusBar()
{
  ///////////////////////////////////////////////////////////////////
  // STATUSBAR
  // TODO: add your own items you need for displaying current application status.

  statusBar()->insertItem(i18n(IDS_STATUS_DEFAULT), ID_STATUS_MSG);

  statusBar()->insertFixedItem(i18n("Line: 00000 Col: 000"), ID_STATUS_CLM, true);
  statusBar()->changeItem("", ID_STATUS_CLM);

  // statusBar()->setInsertOrder(KStatusBar::RightToLeft);
}

void QuantaApp::initContextMenu()
{
#ifdef NEW_STUFF
  kwritePopupMenu = (QPopupMenu *)factory()->container("documentContextPopup", this);
  doc->setRBMenu(kwritePopupMenu);
#endif
}

void QuantaApp::initDocument()
{
  doc = new QuantaDoc(this,this);

#ifndef NEW_STUFF
  kwritePopupMenu = new QPopupMenu();

  kwritePopupMenu->insertItem(UserIcon("undo"),i18n("&Undo"), ID_EDIT_UNDO );
  kwritePopupMenu->insertItem(UserIcon("redo"),i18n("&Redo"), ID_EDIT_REDO );
  kwritePopupMenu->insertSeparator();
  kwritePopupMenu->insertItem(UserIcon("cut"), 	i18n("Cu&t"), 	 ID_EDIT_CUT );
  kwritePopupMenu->insertItem(UserIcon("copy"), 	i18n("&Copy"),  ID_EDIT_COPY );
  kwritePopupMenu->insertItem(UserIcon("paste"), i18n("&Paste"), ID_EDIT_PASTE );
  kwritePopupMenu->insertSeparator();
  kwritePopupMenu->insertItem(UserIcon("open"),       i18n("Open File..."),ID_FILE_OPEN);
  kwritePopupMenu->insertItem(UserIcon("save"),       i18n("Save"),        ID_FILE_SAVE);
  kwritePopupMenu->insertItem(SmallIcon("fileclose"), i18n("Close"),       ID_FILE_CLOSE );
  kwritePopupMenu->insertSeparator();
  kwritePopupMenu->insertItem( i18n("Context Help"),  		ID_CONTEXT_HELP );
  kwritePopupMenu->insertItem( i18n("Tag Attributes"),  	ID_ATTRIB_POPUP);
  kwritePopupMenu->insertItem( i18n("&Edit Current Tag"), ID_EDIT_CURRENT_TAG);

  connect(kwritePopupMenu, SIGNAL(activated(int)), SLOT(commandCallback(int)));
  connect(kwritePopupMenu, SIGNAL(highlighted(int)), SLOT(statusCallback(int)));

  doc->setRBMenu(kwritePopupMenu);
#endif

}

void QuantaApp::initProject()
{
	project = new Project(this);
	
	connect(project, 	SIGNAL(openFile(QString)),
					this, 		SLOT  (slotFileOpen(QString)));
	connect(project, 	SIGNAL(reloadTree(QStringList,bool,bool)),
					pTab, 		SLOT  (slotReloadTree(QStringList,bool,bool)));
	connect(project, 	SIGNAL(setBasePath(QString)),
					pTab, 		SLOT  (slotSetBasePath(QString)));
	connect(project, 	SIGNAL(setProjectName(QString)),
					pTab, 		SLOT  (slotSetProjectName(QString)));
	connect(project,	SIGNAL(closeFiles()),
					doc,			SLOT	(closeAll()));
	connect(project,	SIGNAL(requestOpenedFiles()),
					doc,			SLOT	(slotRequestOpenedFiles()));
	connect(doc,			SIGNAL(openedFiles(QStringList)),
					project,	SLOT  (slotOpenedFiles(QStringList)));
	connect(project,  SIGNAL(addRecentProject(const QString &)),
					this,			SLOT	(addRecentProject(const QString &)));
					
	connect(fLTab,		SIGNAL(insertDirInProject(QString)),
					project,	SLOT	(addDirectory(QString)));
	connect(fTTab,		SIGNAL(insertDirInProject(QString)),
					project,	SLOT	(addDirectory(QString)));
					
	connect(fLTab,		SIGNAL(insertFileInProject(QString)),
					project,	SLOT	(insertFile(QString)));
	connect(fTTab,		SIGNAL(insertFileInProject(QString)),
					project,	SLOT	(insertFile(QString)));
					
	connect(pTab,			SIGNAL(removeFileFromProject(QString)),
					project,	SLOT	(slotRemoveFile(QString)));
	connect(pTab,			SIGNAL(removeFolderFromProject(QString)),
					project,	SLOT	(slotRemoveFolder(QString)));
					
	connect(project,	SIGNAL(selectMessageWidget()),
					this,			SLOT  (slotSelectMessageWidget()));
	connect(project,	SIGNAL(disableMessageWidget()),
					this,			SLOT  (slotDisableMessageWidget()));
	connect(project,	SIGNAL(messages(QString)),
					messageOutput, SLOT(insertItem(QString)));

	connect(project,	SIGNAL(saveAllFiles()),
					this, SLOT(slotFileSaveAll()));					
}

void QuantaApp::initView()
{ 
  ////////////////////////////////////////////////////////////////////
  // create the main widget here that is managed by KTMainWindow's view-region and
  // connect the widget to your document to display document contents.

	
  topWidgetStack = new QWidgetStack( this );

  hSplit = new WSplitter( topWidgetStack );
  hSplit->setOrientation( Vertical);
  vSplit = new WSplitter( hSplit);
  vSplit->setOrientation( Horizontal);

  leftPanel = new QTabWidget( vSplit);
  leftPanel ->setFocusPolicy(QWidget::NoFocus);

  fTab = new QWidgetStack( leftPanel );

  QStrList topList;
  config->setGroup("General Options");
  config->readListEntry("Top folders", topList);

  fTTab = new FilesTreeView( QDir::homeDirPath()+"/" , topList, fTab );
  fLTab = new FilesListView( QDir::homeDirPath()+"/" , 0L, fTab );

  fTab  ->setFocusPolicy(QWidget::NoFocus);
  fTTab ->setFocusPolicy(QWidget::ClickFocus);
  fLTab ->setFocusPolicy(QWidget::ClickFocus);

  fTab -> addWidget( fTTab, 0 );
  fTab -> addWidget( fLTab, 1 );
  fTab -> raiseWidget( 0 );

  pTab	= new ProjectTreeView( leftPanel);
  dTab	= new DocTreeView( leftPanel);

  pTab -> setFocusPolicy(QWidget::NoFocus);
  dTab -> setFocusPolicy(QWidget::NoFocus);


  sTab = new StructTreeView( parser, config, leftPanel ,"struct");
	sTab ->setFocusPolicy(QWidget::NoFocus);

  leftPanel->addTab( fTab,	"Files"  );
  leftPanel->addTab( pTab,	"Project");
  leftPanel->addTab( sTab,	"Struct" );
  leftPanel->addTab( dTab,	"Docs"   );

  rightWidgetStack = new QWidgetStack( vSplit );

  view = new QuantaView( this, rightWidgetStack );
  view->app = this;
  view->doc = doc;

  bottomWidgetStack = new QWidgetStack( hSplit );

  messageOutput = new MessageOutput( bottomWidgetStack );
  messageOutput ->setFocusPolicy(QWidget::NoFocus);


  vSplit->activate( leftPanel, rightWidgetStack );
  hSplit->activate( vSplit, bottomWidgetStack );

  leftPanel->setMinimumWidth( 1 );

  bottomWidgetStack ->setMinimumHeight( 1 );
	leftPanel					->setMinimumHeight( 1 );
  rightWidgetStack  ->setMinimumHeight( 1 );
	hSplit	 					->setMinimumHeight( 1 );

  vSplit->setPos( 25);
  hSplit->setPos( 100);

  htmlPartRight	 = new WHTMLPart( rightWidgetStack,  "rightHTML");
  htmlPartBottom = new WHTMLPart( bottomWidgetStack, "bottomHTML");
  htmlPartTop    = new WHTMLPart( topWidgetStack, 	 "topHTML");
  htmlPartDoc 	 = new WHTMLPart( rightWidgetStack,  "docHTML");

  bottomWidgetStack->addWidget( messageOutput, 0 );
  bottomWidgetStack->addWidget( htmlPartBottom->view(), 1 );
  bottomWidgetStack->raiseWidget(0);

  rightWidgetStack->addWidget( view, 0 );
  rightWidgetStack->addWidget( htmlPartRight->view(), 1 );
  rightWidgetStack->addWidget( htmlPartDoc  ->view(), 2 );
  rightWidgetStack->raiseWidget(0);

  topWidgetStack->addWidget( hSplit, 0);
  topWidgetStack->addWidget( htmlPartTop->view(), 1 );
  topWidgetStack->raiseWidget(0);

  doc->addView( view );
  setView( topWidgetStack );
  setCaption( doc->getTitle());

#ifndef NEW_STUFF
  view->updateToolBars(toolbars);
#endif

  connect( 	fTTab,SIGNAL(openFile(QString)),
  					this, SLOT(slotFileOpen(QString)));
  connect( 	fLTab,SIGNAL(openFile(QString)),
  					this, SLOT(slotFileOpen(QString)));
  					
  connect( 	fTTab,SIGNAL(openImage(QString)),
  					this, SLOT(slotImageOpen(QString)));
  connect( 	fLTab,SIGNAL(openImage(QString)),
  					this, SLOT(slotImageOpen(QString)));
  					
  connect(	fLTab,SIGNAL(changeMode()),
  					this, SLOT(slotSwapLeftPanelMode()));
  connect(	fTTab,SIGNAL(changeMode()),
  					this, SLOT(slotSwapLeftPanelMode()));
  					
  connect( 	pTab, SIGNAL(openFile(QString)),
  					this, SLOT(slotFileOpen(QString)));
  connect( 	pTab, SIGNAL(openImage(QString)),
  					this, SLOT(slotImageOpen(QString)));
  					
  connect( 	fTTab,SIGNAL(insertTag(QString)),
  					this, SLOT(slotInsertTag(QString)));
  connect( 	fLTab,SIGNAL(insertTag(QString)),
  					this, SLOT(slotInsertTag(QString)));
  connect( 	pTab,SIGNAL(insertTag(QString)),
  					this, SLOT(slotInsertTag(QString)));
  					
  					
  connect( 	fLTab,SIGNAL(activatePreview()),
  					this, SLOT(slotActivatePreview()));
  connect( 	fTTab,SIGNAL(activatePreview()),
  					this, SLOT(slotActivatePreview()));
  connect( 	pTab, SIGNAL(activatePreview()),
  					this, SLOT(slotActivatePreview()));
  					
  connect(  htmlPartRight,  SIGNAL(onURL(const QString&)), this, SLOT(slotStatusMsg(const QString&)));
  connect(  htmlPartBottom, SIGNAL(onURL(const QString&)), this, SLOT(slotStatusMsg(const QString&)));
  connect(  htmlPartTop,    SIGNAL(onURL(const QString&)), this, SLOT(slotStatusMsg(const QString&)));
  connect(  htmlPartDoc,    SIGNAL(onURL(const QString&)), this, SLOT(slotStatusMsg(const QString&)));

  connect(  htmlPartDoc,    SIGNAL(updateStatus(bool, bool)), SLOT(updateNavButtons( bool, bool)));

  connect( view, SIGNAL(newCurPos()), this, SLOT(slotNewLineColumn()));

  connect( sTab, SIGNAL(newCursorPosition(int,int)), SLOT(setCursorPosition(int,int)));
  connect( sTab, SIGNAL(onTag(const QString &)), SLOT( slotStatusHelpMsg(const QString &)));
  connect( sTab, SIGNAL(selectArea(int,int,int,int)), SLOT( selectArea(int,int,int,int)));
  connect( sTab, SIGNAL(needReparse()), SLOT(reparse()));

  connect( dTab, SIGNAL(openURL(QString)), SLOT(openDoc(QString)));


  connect( leftPanel, SIGNAL(currentChanged(QWidget*)), this, SLOT( slotLeftTabChanged(QWidget*)));
}


WHTMLPart * QuantaApp::htmlPart()
{
  WHTMLPart *res = 0L;

  if ( previewPosition == "Bottom" )
     res = htmlPartBottom;

  if ( previewPosition == "Right" )
     res = htmlPartRight;

  if ( previewPosition == "FWSpace" )
     res = htmlPartTop;

  return res;

}


QWidgetStack *QuantaApp::widgetStackOfHtmlPart()
{
  QWidgetStack *res = 0L;

  if ( previewPosition == "Bottom" )
     res = bottomWidgetStack;

  if ( previewPosition == "Right" )
     res = rightWidgetStack;

  if ( previewPosition == "FWSpace" )
     res = topWidgetStack;

  return res;

}


void QuantaApp::enableCommand(int id_)
{
  ///////////////////////////////////////////////////////////////////
  // enable menu and toolbar functions by their ID's
  menuBar()->setItemEnabled(id_, true);

#ifndef NEW_STUFF
  toolBar()->setItemEnabled(id_, true);
  toolbar1->setItemEnabled(id_, true);
  toolbar2->setItemEnabled(id_, true);
#endif
}

void QuantaApp::disableCommand(int id_)
{
  ///////////////////////////////////////////////////////////////////
  // disable menu and toolbar functions by their ID's
  menuBar()->setItemEnabled(id_, false);

#ifndef NEW_STUFF
  toolBar()->setItemEnabled(id_, false);
  toolbar1->setItemEnabled(id_, false);
  toolbar2->setItemEnabled(id_, false);
#endif
}

/** check button/item in stat */
void QuantaApp::checkCommand(int id_, bool stat)
{
  ///////////////////////////////////////////////////////////////////
#ifndef NEW_STUFF

  viewMenu ->setItemChecked( id_, stat);
  editMenu ->setItemChecked( id_, stat);

	toolBar()->setButton(      id_, stat);
	toolbar1->setButton(      id_, stat);
	toolbar2->setButton(      id_, stat);
#endif
}

void QuantaApp::addRecentFile(const QString &file)
{
  if(recentFiles.find(file) == -1)
  {
    if( recentFiles.count() < 5)
    {
      recentFiles.insert(0, file);
    }
    else
    {
      recentFiles.remove(4);
      recentFiles.insert(0, file);
    }
    recentFilesMenu->clear();
    for ( int i=0 ; i < (int) recentFiles.count(); i++)
    {
      recentFilesMenu->insertItem(recentFiles.at(i));
    }
  }
}

void QuantaApp::addRecentProject(const QString &file)
{
  if(recentProjects.find(file) == -1)
  {
    if( recentProjects.count() < 5)
    {
      recentProjects.insert(0, file);
    }
    else
    {
      recentProjects.remove(4);
      recentProjects.insert(0, file);
    }
    recentProjectsMenu->clear();
    for ( int i=0 ; i < (int) recentProjects.count(); i++)
    {
      recentProjectsMenu->insertItem(recentProjects.at(i));
    }
  }
}

void QuantaApp::openDocumentFile(const char* _cmdl)
{
  slotStatusMsg(i18n("Opening file..."));

  doc->openDocument(_cmdl);

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}


QuantaDoc *QuantaApp::getDocument() const
{
  return doc;
}

void QuantaApp::saveOptions()
{	
  config->setGroup("General Options");
  doc->write()->writeConfig(config);
  config->writeEntry("Geometry", size());

#ifndef NEW_STUFF
  config->writeEntry("Show Toolbar", toolBar()->isVisible());
  config->writeEntry("Show Toolbar File", toolbar1->isVisible());
  config->writeEntry("Show Toolbar Edit", toolbar2->isVisible());
#endif // NEW_STUFF
  config->writeEntry("Show Statusbar",statusBar()->isVisible());
  config->writeEntry("ToolBarPos", (int) toolBar()->barPos());
  config->writeEntry("Recent Files", recentFiles);
  config->writeEntry("Recent Projects", recentProjects);

  config->writeEntry("Html mask", fileMaskHtml );
  config->writeEntry("Images mask", fileMaskImage );
  config->writeEntry("Php mask", fileMaskPhp );
  config->writeEntry("Java mask", fileMaskJava );
  config->writeEntry("Text mask", fileMaskText );

  config->writeEntry("Capitals for tags", tagsCapital);
  config->writeEntry("Capitals for attr", attrCapital);
  config->writeEntry("Use close tag if optional", useCloseTag);

  config->writeEntry("Preview position", previewPosition);

  config->writeEntry("Left panel mode", fTab->id( fTab->visibleWidget()));

  config->writeEntry("HSplit position", hSplit->getPos() );
  config->writeEntry("VSplit position", vSplit->getPos() );

  config->writeEntry("Follow Cursor", sTab->followCursor() );

  config->writeEntry("PHP Debugger Port", phpDebugPort );
  
  // save list of open files

  QStrList fileList;
  QDictIterator<Document> it( *(doc->docList) ); // iterator for dict

  while ( Document *twrite = it.current() ) {
    if ( twrite->hasFileName() )
    	fileList.append( twrite->fileName() );
    ++it;
  }

  config->writeEntry("List of opened files", fileList);
  config->writeEntry("Top folders", fTTab->dirList);

  if ( project->hasProject() )
  {
    project->saveProject();
    config->writeEntry("Last Project", project->projectFileName);
  }
  else config->writeEntry("Last Project", "");
}


void QuantaApp::readOptions()
{
	
  config->setGroup("General Options");

  fileMaskHtml  = config->readEntry("Html mask", 	fileMaskHtml)	+" ";
  fileMaskImage = config->readEntry("Images mask",fileMaskImage)+" ";
  fileMaskPhp   = config->readEntry("Php mask", 	fileMaskPhp)	+" ";
  fileMaskJava  = config->readEntry("Java mask", 	fileMaskJava)	+" ";
  fileMaskText  = config->readEntry("Text mask", 	fileMaskText)	+" ";

  tagsCapital = config->readBoolEntry("Capitals for tags", false);
  attrCapital = config->readBoolEntry("Capitals for attr", false);
  useCloseTag = config->readBoolEntry("Use close tag if optional", true);

  previewPosition   = config->readEntry("Preview position","Right");

  int mode = config->readNumEntry("Left panel mode", 1);
  if ( mode == 0 || mode == 1 ) fTab->raiseWidget(mode);

  hSplit->setPos( config->readNumEntry("HSplit position", 1000) );
  vSplit->setPos( config->readNumEntry("VSplit position", 250 ) );

  phpDebugPort = config->readNumEntry("PHP Debugger Port", 7869 );
  
  if ( vSplit->getPos() == 0 )
        checkCommand( ID_VIEW_TREE, false );
  else  checkCommand( ID_VIEW_TREE, true  );

  sTab->setFollowCursor( config->readBoolEntry("Follow Cursor", true ) );

  bool bViewStatusbar = config->readBoolEntry("Show Statusbar", true);

#ifndef NEW_STUFF
  viewMenu->setItemChecked(ID_VIEW_STATUSBAR, bViewStatusbar);
#endif

  if(!bViewStatusbar)
  {
    enableStatusBar(KStatusBar::Hide);
  }

  // bar position settings
  KToolBar::BarPosition toolBarPos;
  toolBarPos=(KToolBar::BarPosition) config->readNumEntry("ToolBarPos", KToolBar::Top);
  toolBar()->setBarPos(toolBarPos);
	
  // initialize the recent file list
  recentFiles.setAutoDelete(true);
  config->readListEntry("Recent Files", recentFiles);
	
  recentProjects.setAutoDelete(true);
  config->readListEntry("Recent Projects", recentProjects);

#ifndef NEW_STUFF
  for (int i=0; i < (int) recentFiles.count(); i++)
    recentFilesMenu->insertItem(recentFiles.at(i));

  for (int i=0; i < (int) recentProjects.count(); i++)
    recentProjectsMenu->insertItem(recentProjects.at(i));
#endif // NEW_STUFF

  QSize size=config->readSizeEntry("Geometry");
  if(!size.isEmpty())
  {
    resize(size);
  } else
  	resize( QSize(800,580) );
}

void QuantaApp::openLastFiles()
{
  config->setGroup("General Options");

  QString projectFileName = config->readEntry("Last Project");
  QFileInfo fi( projectFileName );

  if ( projectFileName.isEmpty() || !fi.exists() )
  {	  	
	  QStrList fileList;
  	config->readListEntry("List of opened files",fileList);

	  for ( fileList.last();fileList.current();fileList.prev() )
      doc->openDocument( fileList.current() );
  }
  else {
  	project->loadProject( projectFileName );
  	leftPanel-> showPage( (QWidget *)pTab );
  }

  if ( doc->docList->count() == 0 ) // no documents opened
  {
  	
  }
}

void QuantaApp::saveProperties(KConfig *)
{
    doc->saveAll();
}


void QuantaApp::readProperties(KConfig *)
{
}		

bool QuantaApp::queryClose()
{
  return true;
}

bool QuantaApp::queryExit()
{
  saveOptions();
  return doc->saveAll(false);
}

/**
  read dictionary of known tags and attributes from tags.rc file.
*/

void QuantaApp::initTagDict()
{
  tagsList = new QStrList();
  tagsCore = new QStrList();
  tagsI18n = new QStrList();
  tagsScript = new QStrList();
  quotedAttribs = new QStrList();
  lCore = new QStrList();
  lI18n = new QStrList();
  lScript = new QStrList();
  singleTags = new QStrList();
  optionalTags = new QStrList();

  tagsDict = new QDict <QStrList>(233,false);

  KConfig *config = new KConfig( locate("appdata","tagdata.rc") );

  config->setGroup("Tags");

  config->readListEntry("TagsCore", *tagsCore);
  config->readListEntry("TagsI18n", *tagsI18n);
  config->readListEntry("TagsScript", *tagsScript);
  config->readListEntry("Quoted Attribs", *quotedAttribs);

  config->readListEntry("Core", *lCore);
  config->readListEntry("I18n", *lI18n);
  config->readListEntry("Scripts", *lScript);

  config->readListEntry("TagsList", *tagsList);

  config->readListEntry("Single tags", *singleTags);
  config->readListEntry("Optional tags", *optionalTags);

  char *tag, *t;
  for ( tag = tagsList->first(); tag; tag = tagsList->next() )
	{
    QStrList *attrList = new QStrList();
    config->readListEntry(tag, *attrList);

    if ( tagsCore->find(tag) != -1 )
      for ( t = lCore->first(); t; t = lCore->next() )
        attrList->append(t);

    if ( tagsI18n->find(tag) != -1 )
      for ( t = lI18n->first(); t; t = lI18n->next() )
        attrList->append(t);

    if ( tagsScript->find(tag) != -1 )
      for ( t = lScript->first(); t; t = lScript->next() )
        attrList->append(t);

    tagsDict->insert(tag, attrList);
  }
}

#ifdef NEW_STUFF

void QuantaApp::initActions()
{
    //
    // File actions
    //

    KStdAction::openNew( this, SLOT( slotFileNew() ), actionCollection() );
    KStdAction::open( this, SLOT( slotFileOpen() ), actionCollection() );
    // TODO recent files
    KStdAction::close( this, SLOT( slotFileClose() ), actionCollection() );

    (void) new KAction( i18n( "Close All" ), 0, this, SLOT( slotFileCloseAll() ),
                        actionCollection(), "close_all" );

    KStdAction::save( this, SLOT( slotFileSave() ), actionCollection() );
    KStdAction::saveAs( this, SLOT( slotFileSaveAs() ), actionCollection() );

    (void) new KAction( i18n( "Save All..." ), UserIcon("save_all"), 0,
                        this, SLOT( slotFileSaveAll() ),
                        actionCollection(), "save_all" );

    KStdAction::quit( this, SLOT( slotFileQuit() ), actionCollection() );

    //
    // Edit actions
    //

    KStdAction::undo( this, SLOT( slotEditUndo()), actionCollection());
    KStdAction::redo( this, SLOT( slotEditRedo()), actionCollection());

    KAction *undoRedo = new KAction( i18n( "Undo/Redo &History..."), 0, this, SLOT( slotURedoHistory()),
                                     actionCollection(), "undo_history" );
    undoRedo->setGroup( "edit_undo_merge" );
    // TODO: Manually plug this to the right place (or figure out how to do it in XMLGUI)

    KStdAction::cut( this, SLOT( slotEditCut()), actionCollection());
    KStdAction::copy( this, SLOT( slotEditCopy()), actionCollection());
    KStdAction::paste( this, SLOT( slotEditPaste()), actionCollection());
    KStdAction::selectAll( this, SLOT( slotEditSelectAll()), actionCollection());

    (void) new KAction(  i18n( "&Unselect All"), 0, this, SLOT( slotEditDeselectAll()),
                         actionCollection(), "unselect_all" );
    (void) new KAction(  i18n( "&Invert Selection"), 0, this, SLOT( slotEditInvertSelect()),
                         actionCollection(), "invert_selection" );
    (void) new KAction(  i18n( "&Vertical Selection"), 0, this, SLOT( slotEditVerticalSelect()),
                         actionCollection(), "vertical_selection" );

    KStdAction::find( this, SLOT( slotEditSearch()), actionCollection());
    KStdAction::findNext( this, SLOT( slotEditSearchAgain()), actionCollection());
    KStdAction::replace( this, SLOT( slotEditReplace()), actionCollection());

    (void) new KAction( i18n( "Find In Files" ), UserIcon("find"), CTRL+ALT+Key_F,
                        this, SLOT( slotEditFindInFiles() ),
                        actionCollection(), "find_in_files" );

    //
    // Tool actions
    //

    KStdAction::gotoLine( this, SLOT( slotEditGotoLine() ), actionCollection() )
        ->setAccel( CTRL+Key_G );

    (void) new KAction( i18n( "&Indent" ), ALT+SHIFT+Key_Right, this, SLOT( slotEditIndent() ),
                        actionCollection(), "indent" );
    (void) new KAction( i18n( "&Unindent" ), ALT+SHIFT+Key_Left, this, SLOT( slotEditUnindent() ),
                        actionCollection(), "unindent" );
    (void) new KAction( i18n( "&Clean Indentation" ), 0, this, SLOT( slotEditCleanIndent() ),
                        actionCollection(), "clean_indentation" );

    (void) new KAction( i18n( "Context &Help..." ), CTRL+Key_H, this, SLOT( contextHelp() ),
                        actionCollection(), "context_help" );
    KAction *tagAttr = new KAction( i18n( "Tag &Attributes" ), ALT+Key_Down,
                                    doc, SLOT( slotAttribPopup() ),
                                    actionCollection(), "tag_attributes" );
    QAccel *menuKey = new QAccel( this );
    menuKey->connectItem( menuKey->insertItem( Key_Menu ),
                          tagAttr, SLOT( activate() ) );

    (void) new KAction( i18n( "&Edit Current Tag..." ), Key_F4, view, SLOT( slotEditCurrentTag() ),
                        actionCollection(), "edit_current_tag" );

    KStdAction::spelling( view->write(), SLOT( slotSpellCheck() ), actionCollection() )
        ->setAccel( Key_F9 );
    (void) new KAction( i18n( "&FTP Client..." ), UserIcon("ftpclient"), 0, 
                        this, SLOT( slotFtpClient() ),
                        actionCollection(), "ftp_client" );

    //
    // View actions
    //

    KStdAction::showToolbar( this, SLOT( slotViewToolBar() ), actionCollection() );
    KStdAction::showStatusbar( this, SLOT( slotViewStatusBar() ), actionCollection() );

    (void) new KAction( i18n( "Show Tr&ee" ), "tree_win", CTRL+Key_T,
                        this, SLOT( slotShowLeftPanel() ),
                        actionCollection(), "show_tree" );
    (void) new KAction( i18n( "Show &Messages" ), CTRL+Key_B, this, SLOT( slotViewMessages() ),
                        actionCollection(), "show_messages" );
    (void) new KAction( i18n( "&Preview" ), "preview", Key_F6, this, SLOT( slotShowPreview() ),
                        actionCollection(), "show_preview" );

    KStdAction::back( htmlPartDoc, SLOT( back() ), actionCollection() );
    KStdAction::forward( htmlPartDoc, SLOT( forward() ), actionCollection() );

    (void) new KAction( i18n( "&Reload" ), "reload", KStdAccel::key(KStdAccel::Reload)
                        , this, SLOT( slotViewRepaint() ),
                        actionCollection(), "reload" );

    //    (void) new KAction( i18n( "Open Document In Netscape" ), 0, this, SLOT( slotViewInNetscape() ),
    //                        actionCollection(), "open_with_netscape" );
    //    (void) new KAction( i18n( "Open Document In Konqueror" ), 0, this, SLOT( slotViewInKFM() ),
    //                        actionCollection(), "open_with_konqueror" );

    (void) new KAction( i18n( "Previous File" ), CTRL+ALT+Key_Right, this, SLOT( slotFilePrev() ),
                        actionCollection(), "previous_file" );
    (void) new KAction( i18n( "Next File" ), CTRL+ALT+Key_Left, this, SLOT( slotFileNext() ),
                        actionCollection(), "next_file" );

    //
    // Project actions
    //

    (void) new KAction( i18n( "&New Project..." ), 0, project, SLOT( newProject() ),
                        actionCollection(), "new_project" );
    (void) new KAction( i18n( "&Open Project..." ), UserIcon("openprj"), 0,
                        project, SLOT( openProject() ),
                        actionCollection(), "open_project" );
    // TODO: Recent projects
    (void) new KAction( i18n( "&Close Project" ), 0, project, SLOT( closeProject() ),
                        actionCollection(), "close_project" );

    (void) new KAction( i18n( "&Insert File(s)..." ), 0, project, SLOT( addFiles() ),
                        actionCollection(), "insert_file" );
    (void) new KAction( i18n( "&Insert Directory..." ), 0, project, SLOT( addDirectory() ),
                        actionCollection(), "insert_directory" );

    (void) new KAction( i18n( "&Upload Project..." ), Key_F8, project, SLOT( upload() ),
                        actionCollection(), "upload_project" );
    (void) new KAction( i18n( "&Project Options..." ), Key_F7, project, SLOT( options() ),
                        actionCollection(), "project_options" );

    //
    // Options actions
    //

    (void) new KAction( i18n( "&Hightlighting..." ), 0, doc->write(), SLOT( hlDlg() ),
                        actionCollection(), "highlight_options" );
    // TODO: Highlighting mode
    //    (void) new KAction( i18n( "&Editor Options..." ), 0, doc->write(), SLOT( editorOptions() ),
    //                        actionCollection(), "editor_options" );

    KStdAction::keyBindings( this, SLOT( slotOptionsConfigureKeys() ), actionCollection() );
    KStdAction::configureToolbars( this, SLOT( slotOptionsConfigureToolbars() ), actionCollection() );
    KStdAction::preferences( this, SLOT( slotOptions() ), actionCollection() );
}

#else
void QuantaApp::initActions()
{
    // Prevent a linker error!
}

#endif // NEW_STUFF
