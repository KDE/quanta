/***************************************************************************
                          quantaview.cpp  -  description
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

// include files for Qt
#include <qprinter.h>
#include <qpainter.h>
#include <qtabbar.h>
#include <qtabwidget.h>
#include <qlayout.h>
#include <qwidgetstack.h>

// include files for KDE
#include <klocale.h>
#include <kmenubar.h>
#include <kiconloader.h>

// application specific includes
#include "resource.h"
#include "quantaview.h"
#include "quantadoc.h"
#include "quanta.h"
#include "document.h"

#include "widgets/wtoolbar.h"
#include "kwrite/kwview.h"
#include "kwrite/highlight/highlight.h"

QuantaView::QuantaView( QuantaApp *app, QWidget *parent, const char *name )
	: QWidget( parent, name)
{
	this->app = app;
	this->doc = app->doc;
	
	QGridLayout *layout = new QGridLayout( this );
	
  QTabBar *tabBar = new QTabBar( this);

  QWidgetStack *toolbarStack = new QWidgetStack(this);

  bStandard = new WToolBar( app );
	bFonts    = new WToolBar( app );
	bTables   = new WToolBar( app );
	bForms    = new WToolBar( app );
	bLists    = new WToolBar( app );
	bUser     = new WToolBar( app );
	
  // Insert button in tool bars Standard
	bStandard->insertButton( UserIcon("quick_start"), ID_TAG_QUICKSTART, true, i18n("Quick Start"));
  bStandard->insertSeparator();
  bStandard->insertButton( UserIcon("tag_bold"),ID_TAG_BOLD, true, i18n("Bold"));
	bStandard->insertButton( UserIcon("tag_i"),		ID_TAG_I,    true, i18n("Italic"));
	bStandard->insertButton( UserIcon("tag_u"),		ID_TAG_U,    true, i18n("Underline"));
  bStandard->insertSeparator();
	bStandard->insertButton( UserIcon("tag_br"),  ID_TAG_BR,   true, i18n("New Line"));
  bStandard->insertButton( UserIcon("tag_p"),   ID_TAG_P,    true, i18n("Paragraph"));
  bStandard->insertButton( UserIcon("tag_nbsp"),ID_TAG_NBSP, true, i18n("Non-Breaking Space"));
  bStandard->insertSeparator();
  bStandard->insertButton( UserIcon("tag_a"),    ID_TAG_A,     true, i18n("Insert Anchor"));
  bStandard->insertButton( UserIcon("tag_image"),ID_TAG_IMAGE, true, i18n("Insert Image"));
  bStandard->insertButton( UserIcon("tag_hr"),   ID_TAG_HR,    true, i18n("Horizontal Rule"));
  bStandard->insertButton( UserIcon("tag_comm"), ID_TAG_COMM,  true, i18n("Comment"));
  bStandard->insertSeparator();
  bStandard->insertButton( UserIcon("div_left"),   ID_TAG_DIV_LEFT,   true, i18n("Align Left"));
  bStandard->insertButton( UserIcon("div_center"), ID_TAG_DIV_CENTER, true, i18n("Align Center"));
  bStandard->insertButton( UserIcon("div_right"),  ID_TAG_DIV_RIGHT,  true, i18n("Align Right"));
  bStandard->insertButton( UserIcon("div_justify"),ID_TAG_DIV_JUSTIFY,true, i18n("Align Justify"));
  bStandard->insertSeparator();
  bStandard->insertButton( UserIcon("css"),ID_TAG_CSS,true, i18n("CSS"));


  // Insert button in tool bars Fonts
	bFonts->insertButton( UserIcon("tag_font"), ID_TAG_FONT, true, i18n("Font"));
  bFonts->insertSeparator();
	bFonts->insertButton( UserIcon("font_inc"), ID_TAG_FONT_INC, true, i18n("Font Size +1"));
	bFonts->insertButton( UserIcon("font_dec"), ID_TAG_FONT_DEC, true, i18n("Font Size -1"));
  bFonts->insertSeparator();
  bFonts->insertButton( UserIcon("tag_pre"), ID_TAG_PRE, true, i18n("Pre-Formated"));
  bFonts->insertButton( UserIcon("tag_sub"), ID_TAG_SUB, true, i18n("Subscript"));
  bFonts->insertButton( UserIcon("tag_sup"), ID_TAG_SUP, true, i18n("Superscript"));
  bFonts->insertSeparator();
  bFonts->insertButton( UserIcon("tag_h1"), ID_TAG_H1, true, i18n("Head 1"));
	bFonts->insertButton( UserIcon("tag_h2"), ID_TAG_H2, true, i18n("Head 2"));
	bFonts->insertButton( UserIcon("tag_h3"), ID_TAG_H3, true, i18n("Head 3"));
	bFonts->insertButton( UserIcon("tag_h4"), ID_TAG_H4, true, i18n("Head 4"));
	bFonts->insertButton( UserIcon("tag_h5"), ID_TAG_H5, true, i18n("Head 5"));
	
	// Insert button in tool bars Tables
	bTables->insertButton( UserIcon("quick_table"), ID_TAG_QUICK_TABLE,  true, i18n("Quick Table"));
  bTables->insertSeparator();
	bTables->insertButton( UserIcon("tag_table"),    	ID_TAG_TABLE,        true, i18n("Table"));
	bTables->insertButton( UserIcon("tag_table_row"),	ID_TAG_TABLE_ROW,    true, i18n("Table Row"));
	bTables->insertButton( UserIcon("table_head"),		ID_TAG_TABLE_HEAD,   true, i18n("Table Head"));
	bTables->insertButton( UserIcon("table_data"),		ID_TAG_TABLE_DATA,   true, i18n("Table Data"));
  bTables->insertSeparator();
	bTables->insertButton( UserIcon("tag_th"),     ID_TAG_TH,     true, i18n("Table Head"));
	bTables->insertButton( UserIcon("tag_tr"),     ID_TAG_TR,     true, i18n("Table Row"));
	bTables->insertButton( UserIcon("tag_td"),     ID_TAG_TD,     true, i18n("Table Data"));
	bTables->insertButton( UserIcon("tag_caption"),ID_TAG_CAPTION,true, i18n("Caption"));
	
	// Insert button in tool bars Lists
	bLists->insertButton( UserIcon("quick_list"), ID_TAG_QUICK_LIST, true, i18n("Quick List"));
  bLists->insertSeparator();
	bLists->insertButton( UserIcon("tag_ul"), ID_TAG_UL, true, i18n("Unordered list"));
	bLists->insertButton( UserIcon("tag_ol"), ID_TAG_OL, true, i18n("Ordered List"));
	bLists->insertButton( UserIcon("tag_li"), ID_TAG_LI, true, i18n("List Item"));
  bLists->insertSeparator();
  bLists->insertButton( UserIcon("tag_dl"), ID_TAG_DL, true, i18n("Definition List"));
  bLists->insertButton( UserIcon("tag_dt"), ID_TAG_DT, true, i18n("Definition Term"));
  bLists->insertButton( UserIcon("tag_dd"), ID_TAG_DD, true, i18n("Definition"));
	
	// Insert button in tool bars Forms
	bForms->insertButton( UserIcon("form"),  ID_TAG_FORM, true, i18n("Form"));
	bForms->insertSeparator();
	bForms->insertButton( UserIcon("radio"),     ID_TAG_RADIO,     true, i18n("Radio Button"));
	bForms->insertButton( UserIcon("check"),     ID_TAG_CHECK,     true, i18n("Check Box"));
	bForms->insertButton( UserIcon("select"),    ID_TAG_SELECT,    true, i18n("Select"));
	bForms->insertButton( UserIcon("lineedit"),  ID_TAG_LINEEDIT, true, i18n("Text"));
	bForms->insertButton( UserIcon("linepas"),   ID_TAG_LINEPAS, true, i18n("Password"));
	bForms->insertButton( UserIcon("textarea"),  ID_TAG_TEXTAREA, true, i18n("Text Area"));
	bForms->insertButton( UserIcon("submit"),    ID_TAG_SUBMIT,    true, i18n("Submit"));
	bForms->insertButton( UserIcon("reset"),     ID_TAG_RESET,     true, i18n("Reset"));

	
	toolbarStack->addWidget( bStandard, tabBar->insertTab( new QTab( i18n( "Standard"))));
	toolbarStack->addWidget( bFonts,		tabBar->insertTab( new QTab( i18n( "Fonts"))));
	toolbarStack->addWidget( bTables,		tabBar->insertTab( new QTab( i18n( "Tables"))));
	toolbarStack->addWidget( bLists,		tabBar->insertTab( new QTab( i18n( "Lists"))));
	toolbarStack->addWidget( bForms,		tabBar->insertTab( new QTab( i18n( "Forms"))));

	bStandard -> setFocusPolicy(QWidget::NoFocus);
	bFonts		-> setFocusPolicy(QWidget::NoFocus);
	bTables		-> setFocusPolicy(QWidget::NoFocus);
	bLists		-> setFocusPolicy(QWidget::NoFocus);
	bForms		-> setFocusPolicy(QWidget::NoFocus);
	
	tabBar 			-> setFocusPolicy(QWidget::NoFocus);
	toolbarStack-> setFocusPolicy(QWidget::NoFocus);
	
	writeTab = new QTabWidget(this);
  writeTab ->setTabPosition( QTabWidget::Bottom );
  writeTab ->setFocusPolicy( QWidget::NoFocus );

  layout->addWidget( tabBar       ,0,0);
  layout->addWidget( toolbarStack ,1,0);
  layout->addWidget( writeTab     ,2,0);

  connect( bStandard, SIGNAL(clicked(int)), this, SLOT(slotToolbarCallback(int)) );
  connect( bFonts,    SIGNAL(clicked(int)), this, SLOT(slotToolbarCallback(int)) );
  connect( bTables,   SIGNAL(clicked(int)), this, SLOT(slotToolbarCallback(int)) );
  connect( bForms,    SIGNAL(clicked(int)), this, SLOT(slotToolbarCallback(int)) );
  connect( bLists,    SIGNAL(clicked(int)), this, SLOT(slotToolbarCallback(int)) );
  connect( bUser,     SIGNAL(clicked(int)), this, SLOT(userToolbarCallback(int)) );

  connect( writeTab,	SIGNAL(selected(const QString &)), app, SLOT(slotUpdateStatus(const QString &)));
  connect( tabBar,		SIGNAL(selected(int)), toolbarStack, SLOT(raiseWidget(int)));

  initMenu();
  writeTab->show();
}

QuantaView::~QuantaView()
{
}

QuantaDoc *QuantaView::getDocument() const
{
  QuantaApp *theApp=(QuantaApp *) parentWidget();

  return theApp->getDocument();
}

void QuantaView::print(QPrinter *pPrinter)
{
  QPainter printpainter;
  printpainter.begin(pPrinter);
	
  // TODO: add your printing code here

  printpainter.end();
}


/** return current KWrite class */
Document* QuantaView::write()
{
  return ( Document* )writeTab->currentPage();
}

/** Add new kwrite class to writeStack and return id in stack */
void QuantaView::addWrite( Document* w , QString label )
{
	QIconSet emptyIcon ( UserIcon("empty1x16"));
	
	writeTab->addTab( w,  emptyIcon,  label );
	writeTab->showPage( w );
	
	connect( w, SIGNAL(newCurPos()), this, SLOT(slotNewCurPos()));
}

/** remove KWrite class from stack, return id of new KWrite */
Document* QuantaView::removeWrite()
{
  Document *w = write();
  writeTab->removePage( w );
  return write();
}

/** Callback for toolbar with tags. */
void QuantaView::slotToolbarCallback( int id ){
	
//	column = write()->currentColumn();
//	space.fill(' ',column);
	
	switch (id)
  {
    case ID_VIEW_IN_NETSCAPE:
         slotViewInNetscape();
         break;
    case ID_VIEW_IN_KFM2:
         slotViewInKFM();
         break;
    case ID_TAG_CSS:
         slotInsertCSS();
         break;
    case ID_TAG_BOLD:
    		 insertTag("b");
    	 	 break;
    case ID_TAG_H1:
    		 insertTag("h1");
    	 	 break;
    case ID_TAG_H2:
    		 insertTag("h2");
    	 	 break;
    case ID_TAG_H3:
    		 insertTag("h3");
    	 	 break;
    case ID_TAG_H4:
    		 insertTag("h4");
    	 	 break;
    case ID_TAG_H5:
    		 insertTag("h5");
    	 	 break;
    case ID_TAG_I:
    		 insertTag("i");
    	 	 break;
    case ID_TAG_U:
    		 insertTag("u");
    	 	 break;
    case ID_TAG_BR:
    		 insertTag("br");
    	 	 break;
    case ID_TAG_P:
    		 insertTag("p");
    	 	 break;
    case ID_TAG_NBSP:
         slotTagNbsp();
    	 	 break;
    case ID_TAG_DIV_LEFT:
    		 write()->insertTag( tagCase("<div ")+attrCase("align")+"=\"left\">", tagCase("</div>") );
    	 	 break;
    case ID_TAG_DIV_RIGHT:
    		 write()->insertTag( tagCase("<div ")+attrCase("align")+"=\"right\">", tagCase("</div>") );
    	 	 break;
    case ID_TAG_DIV_CENTER:
    		 write()->insertTag( tagCase("<div ")+attrCase("align")+"=\"center\">", tagCase("</div>") );
    	 	 break;
    case ID_TAG_DIV_JUSTIFY:
    		 write()->insertTag( tagCase("<div ")+attrCase("align")+"=\"justify\">", tagCase("</div>") );
    	 	 break;
    case ID_TAG_FONT_INC:
    		 write()->insertTag( tagCase("<font ")+attrCase("size")+"=\"+1\">",tagCase("</font>"));
    	 	 break;
    case ID_TAG_FONT_DEC:
    		 write()->insertTag( tagCase("<font ")+attrCase("size")+"=\"-1\">",tagCase("</font>"));
    	 	 break;
    case ID_TAG_TH:
    		 insertTag("th");
    	 	 break;
    case ID_TAG_TR:
    		 insertTag("tr");
    	 	 break;
    case ID_TAG_TD:
    		 insertTag("td");
    	 	 break;
    case ID_TAG_CAPTION:
    		 insertTag("caption");
    	 	 break;
    case ID_TAG_COMM:
    		 write()->insertTag("<!-- "," -->");
    	 	 break;
    case ID_TAG_PRE:
    		 insertTag("pre");
    	 	 break;
    case ID_TAG_SUB:
    		 insertTag("sub");
    	 	 break;
    case ID_TAG_SUP:
    		 insertTag("sup");
    	 	 break;
    case ID_TAG_UL:
    		 insertTag("ul");
    	 	 break;
    case ID_TAG_OL:
    		 insertTag("ol");
    	 	 break;
    case ID_TAG_LI:
    		 insertTag("li");
    	 	 break;
    case ID_TAG_DL:
    		 insertTag("dl");
    	 	 break;
    case ID_TAG_DT:
    		 insertTag("dt");
    	 	 break;
    case ID_TAG_DD:
    		 insertTag("dd");
    	 	 break;
    case ID_TAG_MENU:
    		 insertTag("menu");
    	 	 break;
    case ID_TAG_HR:
    		 insertTag("hr");
    	 	 break;
    case ID_TAG_SUBMIT:
         slotTagFormSubmit();
    	 	 break;
    case ID_TAG_RESET:
         slotTagFormReset();
    	 	 break;
    case ID_TAG_LINEEDIT:
    		 slotTagFormLineEdit();
    		 break;
    case ID_TAG_LINEPAS:
    		 slotTagFormPas();
    		 break;
    case ID_TAG_RADIO:
    		 slotTagFormRadio();
    		 break;
    case ID_TAG_CHECK:
    		 slotTagFormCheck();
    		 break;
    case ID_TAG_FORM:
    		 slotTagForm();
    		 break;
    case ID_TAG_MAIL:
    		 slotTagMail();
    		 break;
    case ID_TAG_IMAGE:
    		 slotTagImg();
    		 break;
    case ID_TAG_A:
    		 slotTagA();
    		 break;
    case ID_TAG_FONT:
    		 slotTagFont();
    		 break;
    case ID_TAG_FONT_BASE:
    		 slotTagBaseFont();
    		 break;
    case ID_TAG_QUICK_LIST:
    		 slotTagQuickList();
    		 break;
    case ID_TAG_TABLE:
    		 slotTagTable();
    		 break;
    case ID_TAG_QUICK_TABLE:
    		 slotTagQuickTable();
    		 break;
    case ID_TAG_TABLE_ROW:
    		 slotTagTableRow();
    		 break;
    case ID_TAG_TABLE_HEAD:
    		 slotTagTableHead();
    		 break;
    case ID_TAG_TABLE_DATA:
    		 slotTagTableData();
    		 break;
    case ID_TAG_QUICKSTART:
     		 slotTagQuickStart();
    	 	 break;
    case ID_TAG_COLOR:
     		 slotTagColor();
     	 	 break;
    case ID_TAG_DATE:
     		 slotTagDate();
    	 	 break;
    case ID_TAG_SELECT:
     		 slotTagSelect();
    	 	 break;
    case ID_TAG_TEXTAREA:
     		 slotTagFormTextArea();
    	 	 break;
    case ID_EDIT_CURRENT_TAG:
     		 slotEditCurrentTag();
    	 	 break;
/*    case ID_TOOLBAR_OTHER_CONFIGURE:
         slotToolBarConfig();
         break;*/
    	 	
    default:
         break;
  }
  app->repaintPreview();
}


/** initialise tags menu */
void QuantaView::initMenu()
{
  QPopupMenu *insertMenu  = new QPopupMenu();
  app->insertMenu = insertMenu;

  QPopupMenu *mStandard = new QPopupMenu();
  QPopupMenu *mFonts    = new QPopupMenu();
  QPopupMenu *mTables   = new QPopupMenu();
  QPopupMenu *mForms    = new QPopupMenu();
  QPopupMenu *mLists    = new QPopupMenu();

  app->insertStandardMenu = mStandard;
  app->insertFontsMenu    = mFonts;
  app->insertTablesMenu   = mTables;
  app->insertFormsMenu    = mForms;
  app->insertListsMenu    = mLists;

  insertMenu->insertItem( UserIcon("quick_start"), i18n("Quick Start..."), ID_TAG_QUICKSTART);
  insertMenu->insertItem( UserIcon("quick_table"),   i18n("Quick Table..."),  ID_TAG_QUICK_TABLE);
  insertMenu->insertItem( UserIcon("quick_list"), i18n("Quick List..."), ID_TAG_QUICK_LIST);
  insertMenu->insertSeparator();
  insertMenu->insertItem( UserIcon("tag_image"),i18n("Insert Image..."),    ID_TAG_IMAGE);
  insertMenu->insertItem( UserIcon("tag_a"),    i18n("Insert Anchor..."),   ID_TAG_A);
  insertMenu->insertItem( UserIcon("tag_font"),      i18n("Font..."),      ID_TAG_FONT);
  insertMenu->insertSeparator();
	
  insertMenu->insertItem( i18n("Standard"), mStandard, ID_MENU_STANDARD);
  insertMenu->insertItem( i18n("Fonts"),    mFonts,    ID_MENU_FONTS);
  insertMenu->insertItem( i18n("Tables"),   mTables,   ID_MENU_TABLES);
  insertMenu->insertItem( i18n("Forms"),    mForms,    ID_MENU_FORMS);
  insertMenu->insertItem( i18n("Lists"),    mLists,    ID_MENU_LISTS);

  insertMenu->insertSeparator();
  insertMenu->insertItem( UserIcon("color"),   i18n("Color..."),   ID_TAG_COLOR);
	insertMenu->insertItem( UserIcon("date"),    i18n("Date"),    ID_TAG_DATE);
	insertMenu->insertItem( UserIcon("tag_mail"),i18n("E-Mail..."),  ID_TAG_MAIL);

  // Insert button in menu Standard
  mStandard->insertItem( UserIcon("tag_bold"),  i18n("Bold"),       ID_TAG_BOLD);
	mStandard->insertItem( UserIcon("tag_i"),     i18n("Italic"),     ID_TAG_I);
	mStandard->insertItem( UserIcon("tag_u"),     i18n("Underline"),  ID_TAG_U);
	
  mStandard->insertSeparator();

  mStandard->insertItem( UserIcon("tag_br"),  i18n("New Line"),           ID_TAG_BR);
  mStandard->insertItem( UserIcon("tag_p"),   i18n("Paragraph"),          ID_TAG_P);
  mStandard->insertItem( UserIcon("tag_nbsp"),i18n("Non-Breaking Space"), ID_TAG_NBSP);
	
  mStandard->insertSeparator();

  mStandard->insertItem( UserIcon("tag_hr"),   i18n("Horizontal Rule"), ID_TAG_HR);
  mStandard->insertItem( UserIcon("tag_comm"), i18n("Comment"),         ID_TAG_COMM);

  mStandard->insertSeparator();

  mStandard->insertItem( UserIcon("div_left"),   i18n("Align Left"),    ID_TAG_DIV_LEFT);
  mStandard->insertItem( UserIcon("div_center"), i18n("Align Center"),  ID_TAG_DIV_CENTER);
  mStandard->insertItem( UserIcon("div_right"),  i18n("Align Right"),   ID_TAG_DIV_RIGHT);
  mStandard->insertItem( UserIcon("div_justify"),i18n("Align Justify"), ID_TAG_DIV_JUSTIFY);

  mStandard->insertSeparator();

  mStandard->insertItem( i18n("CSS"), ID_TAG_CSS );


  	
	// Insert button in menu Fonts
	mFonts->insertItem( UserIcon("tag_font"),      i18n("Font"),      ID_TAG_FONT);
	mFonts->insertItem( UserIcon("tag_font_base"), i18n("Base Font"), ID_TAG_FONT_BASE);
	
  mFonts->insertSeparator();

	mFonts->insertItem( UserIcon("font_inc"), i18n("Font Size +1"), ID_TAG_FONT_INC);
	mFonts->insertItem( UserIcon("font_dec"), i18n("Font Size -1"), ID_TAG_FONT_DEC);
	
  mFonts->insertSeparator();

  mFonts->insertItem( UserIcon("tag_pre"), i18n("Pre Formated"),  ID_TAG_PRE);
  mFonts->insertItem( UserIcon("tag_sub"), i18n("Subscript"),     ID_TAG_SUB);
  mFonts->insertItem( UserIcon("tag_sup"), i18n("Superscript"),   ID_TAG_SUP);

  mFonts->insertSeparator();

  mFonts->insertItem( UserIcon("tag_h1"), i18n("Head 1"), ID_TAG_H1);
	mFonts->insertItem( UserIcon("tag_h2"), i18n("Head 2"), ID_TAG_H2);
	mFonts->insertItem( UserIcon("tag_h3"), i18n("Head 3"), ID_TAG_H3);
	mFonts->insertItem( UserIcon("tag_h4"), i18n("Head 4"), ID_TAG_H4);
	mFonts->insertItem( UserIcon("tag_h5"), i18n("Head 5"), ID_TAG_H5);
	
	
	// Insert button in menu Tables
	mTables->insertItem( UserIcon("tag_table"),     i18n("Table"),        ID_TAG_TABLE);
	mTables->insertItem( UserIcon("tag_table_row"), i18n("Table Row"),    ID_TAG_TABLE_ROW);
	mTables->insertItem( UserIcon("table_head"),    i18n("Table Head"),   ID_TAG_TABLE_HEAD);
	mTables->insertItem( UserIcon("table_data"),    i18n("Table Data"),   ID_TAG_TABLE_DATA);
	
  mTables->insertSeparator();
	
	mTables->insertItem( UserIcon("tag_th"),     i18n("Table Head"),  ID_TAG_TH);
	mTables->insertItem( UserIcon("tag_tr"),     i18n("Table Row"),   ID_TAG_TR);
	mTables->insertItem( UserIcon("tag_td"),     i18n("Table Data"),  ID_TAG_TD);
	mTables->insertItem( UserIcon("tag_caption"),i18n("Caption"),     ID_TAG_CAPTION);
	
	
	// Insert button in menu Forms
	mForms->insertItem( UserIcon("form"),      i18n("Form"),          ID_TAG_FORM);
	mForms->insertItem( UserIcon("radio"),     i18n("Radio Button"),  ID_TAG_RADIO);
	mForms->insertItem( UserIcon("check"),     i18n("Check Box"),     ID_TAG_CHECK);
	mForms->insertItem( UserIcon("select"),    i18n("Select"),        ID_TAG_SELECT);
	mForms->insertItem( UserIcon("lineedit"),  i18n("Text"),          ID_TAG_LINEEDIT);
	mForms->insertItem( UserIcon("linepas"),   i18n("Password"),      ID_TAG_LINEPAS);
	mForms->insertItem( UserIcon("textarea"),  i18n("Text Area"),     ID_TAG_TEXTAREA);
	mForms->insertItem( UserIcon("submit"),    i18n("Submit"),        ID_TAG_SUBMIT);
	mForms->insertItem( UserIcon("reset"),     i18n("Reset"),         ID_TAG_RESET);
	
	
	// Insert button in menu Lists
	mLists->insertItem( UserIcon("tag_ul"), i18n("Unordered List"), ID_TAG_UL);
	mLists->insertItem( UserIcon("tag_ol"), i18n("Ordered List"),   ID_TAG_OL)	;
	mLists->insertItem( UserIcon("tag_li"), i18n("List Item"),      ID_TAG_LI);
	
  mLists->insertSeparator();

  mLists->insertItem( UserIcon("tag_dl"), i18n("Definition List"),  ID_TAG_DL);
  mLists->insertItem( UserIcon("tag_dt"), i18n("Definition Term"),  ID_TAG_DT);
  mLists->insertItem( UserIcon("tag_dd"), i18n("Definition"),       ID_TAG_DD);

  mLists->insertSeparator();

  mLists->insertItem( UserIcon("tag_menu"), i18n("Menu"), ID_TAG_MENU);

	app->menuBar()->insertItem(i18n("&Insert"), insertMenu, -1, 3);

  connect(mStandard, SIGNAL(activated(int)), SLOT(slotToolbarCallback(int)) );
	connect(mFonts,    SIGNAL(activated(int)), SLOT(slotToolbarCallback(int)) );
	connect(mTables,   SIGNAL(activated(int)), SLOT(slotToolbarCallback(int)) );
	connect(mForms,    SIGNAL(activated(int)), SLOT(slotToolbarCallback(int)) );
	connect(mLists,    SIGNAL(activated(int)), SLOT(slotToolbarCallback(int)) );
  connect(insertMenu,SIGNAL(activated(int)), SLOT(slotToolbarCallback(int)) );

  connect(app->editMenu, SIGNAL(activated(int)), SLOT(slotToolbarCallback(int)));
  connect(app->viewMenu, SIGNAL(activated(int)), SLOT(slotToolbarCallback(int)));
}
