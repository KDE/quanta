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
#include "toolbar/toolbars.h"

//
// Enable this define to test the KAction/XMLGUI code (Rich).
//
//#define NEW_STUFF
//

#ifdef NEW_STUFF

#include <kaction.h>
#include "quanta_actions.h"

#endif

QuantaView::QuantaView( QuantaApp *app, QWidget *parent, const char *name )
	: QWidget( parent, name)
{
	this->app = app;
	this->doc = app->doc;
	
	QGridLayout *layout = new QGridLayout( this );
	
  tabBar = new QTabBar( this);
  toolbarStack = new QWidgetStack(this);

	tabBar 			-> setFocusPolicy(QWidget::NoFocus);
	toolbarStack-> setFocusPolicy(QWidget::NoFocus);
	
	writeTab = new QTabWidget(this);
  writeTab ->setTabPosition( QTabWidget::Bottom );
  writeTab ->setFocusPolicy( QWidget::NoFocus );

  layout->addWidget( tabBar       ,0,0);
  layout->addWidget( toolbarStack ,1,0);
  layout->addWidget( writeTab     ,2,0);

  connect( writeTab,	SIGNAL(selected(const QString &)), app, SLOT(slotUpdateStatus(const QString &)));
  connect( tabBar,		SIGNAL(selected(int)), toolbarStack, SLOT(raiseWidget(int)));

  initMenu();

#ifdef NEW_STUFF
  initActions();
#endif

  writeTab->show();
}

QuantaView::~QuantaView()
{
}

void QuantaView::updateToolBars( ToolBars *t)
{
  // remove all items from toolbar
  for ( WToolBar *tb = wtoolbars.first(); tb!=0; tb = wtoolbars.next() ) {
    int id = toolbarStack->id(tb);
    toolbarStack->removeWidget(tb);
    tabBar->removeTab( tabBar->tab(id) );
  }
  wtoolbars.clear();

  // insert toolbars
  for ( int i=0; i < t->count(); i++ ) 
  {
    WToolBar *tb = new WToolBar(app);
    tb -> setFocusPolicy(QWidget::NoFocus);
    wtoolbars.append(tb);

    QDomNodeList list = t->actions(i);
    for ( unsigned int ai=0; ai<list.count(); ai++ ) 
    {

      QDomElement el = list.item(ai).toElement();
      int id = el.attribute("id","-1").toInt();

      if ( el.attribute("type") != "separator") 
      {
        QString tooltip = el.attribute("name","");
        
        QDomNodeList nl = el.childNodes();
        for ( unsigned int i=0; i<nl.count(); i++ )
        {
       		QDomElement el = nl.item(i).toElement();
       		if ( el.nodeName() == "tooltip" )	tooltip = el.text();
        }
        tooltip.replace( QRegExp("&lt;"), "<" );
        tooltip.replace( QRegExp("&gt;"), ">" );
        
        tb->insertButton( t->actionIcon(id),  el.attribute("id","-1").toInt(), true, i18n( tooltip ));
      }
      else
        tb->insertSeparator();
    }

    toolbarStack->addWidget( tb, tabBar->insertTab( new QTab( i18n( t->toolbarName(i) ))));

    connect( tb, SIGNAL(clicked(int)), this, SLOT(userToolbarCallback(int)));
  }

  tabBar->repaint();
  toolbarStack->raiseWidget( wtoolbars.first() );

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

#ifndef NEW_STUFF
  connect(app->editMenu, SIGNAL(activated(int)), SLOT(slotToolbarCallback(int)));
  connect(app->viewMenu, SIGNAL(activated(int)), SLOT(slotToolbarCallback(int)));
#endif
}


#ifdef NEW_STUFF

void QuantaView::initActions()
{
    KActionCollection *actionCollection = app->actionCollection();

    (void) new KAction( i18n( "Quick Start..." ), "quick_start", 0,
                        this, SLOT( slotTagQuickStart() ),
                        actionCollection, "tag_quick_start" );
    (void) new KAction( i18n( "Quick Table..." ), "quick_table", 0,
                        this, SLOT( slotTagQuickTable() ),
                        actionCollection, "tag_quick_table" );
    (void) new KAction( i18n( "Quick List..." ), "quick_list", 0,
                        this, SLOT( slotTagQuickList() ),
                        actionCollection, "tag_quick_list" );

    (void) new KAction( i18n( "Insert Image..." ), "tag_image", 0,
                        this, SLOT( slotTagImg() ),
                        actionCollection, "tag_img" );
    (void) new KAction( i18n( "Insert Anchor..." ), "tag_a", 0,
                        this, SLOT( slotTagA() ),
                        actionCollection, "tag_a" );
    (void) new KAction( i18n( "Font..." ), "tag_font", 0,
                        this, SLOT( slotTagFont() ),
                        actionCollection, "tag_font" );

    (void) new KAction( i18n( "Color..." ), "color", CTRL+Key_NumberSign,
                        this, SLOT( slotTagColor() ),
                        actionCollection, "tag_color" );
    (void) new KAction( i18n( "Date" ), "date", 0,
                        this, SLOT( slotTagDate() ),
                        actionCollection, "tag_date" );
    (void) new KAction( i18n( "EMail..." ), "tag_mail", 0,
                        this, SLOT( slotTagMail() ),
                        actionCollection, "tag_mail" );

    (void) new KAction( i18n( "Paste &HTML Quoted" ), "editpaste", 0,
                        this, SLOT( slotPasteHTMLQuoted() ),
                        actionCollection, "edit_paste_html_quoted" );
    (void) new KAction( i18n( "Paste &URL Encoded" ), "editpaste", 0,
                        this, SLOT( slotPasteURLEncoded() ),
                        actionCollection, "edit_paste_url_encoded" );

    //
    // Standard style tags
    //
    QuantaTagAction *tagB = new QuantaTagAction( i18n( "Bold" ), "tag_bold", CTRL+Key_B,
                                                 actionCollection, "tag_b", this );
    tagB->setTag( I18N_NOOP( "b" ) );

    QuantaTagAction *tagI = new QuantaTagAction( i18n( "Italic" ), "tag_i", CTRL+Key_I,
                                                 actionCollection, "tag_i", this );
    tagI->setTag( I18N_NOOP( "i" ) );

    QuantaTagAction *tagU = new QuantaTagAction( i18n( "Underline" ), "tag_u", CTRL+Key_U,
                                                 actionCollection, "tag_u", this );
    tagU->setTag( I18N_NOOP( "u" ) );

    QuantaTagAction *tagBr = new QuantaTagAction( i18n( "New Line" ), "tag_br", CTRL+Key_Enter,
                                                 actionCollection, "tag_br", this );
    tagBr->setTag( I18N_NOOP( "br" ) );

    QuantaTagAction *tagP = new QuantaTagAction( i18n( "Paragraph" ), "tag_p", CTRL+Key_0,
                                                 actionCollection, "tag_p", this );
    tagP->setTag( I18N_NOOP( "p" ) );

    (void) new KAction( i18n( "Non-Breaking Space" ), "tag_nbsp", CTRL+Key_Space,
                        this, SLOT( slotTagNbsp() ),
                        actionCollection, "tag_nbsp" );

    QuantaTagAction *tagHr = new QuantaTagAction( i18n( "Horizontal Rule" ), "tag_hr", CTRL+Key_Equal,
                                                  actionCollection, "tag_hr", this );
    tagHr->setTag( I18N_NOOP( "hr" ) );

    QuantaTagAction *tagComment = new QuantaTagAction( i18n( "Comment" ), "tag_comm", 0,
                                                 actionCollection, "tag_comment", this );
    tagComment->setTag( I18N_NOOP( "<!-- " ), I18N_NOOP( " -->" ) );

    QuantaTagAction *tagDivLeft = new QuantaTagAction( i18n( "Align Left" ), "div_left", 0,
                                                       actionCollection, "tag_div_left", this );
    QString start = tagCase( I18N_NOOP("<div %1>") );
    QString end = tagCase( I18N_NOOP("</div>") );
    start = start.arg( attrCase( I18N_NOOP("align%2") ) );
    start = start.arg( I18N_NOOP("=\"left\"") );
    tagDivLeft->setTag( start, end );

    QuantaTagAction *tagDivCenter = new QuantaTagAction( i18n( "Align Center" ), "div_center", 0,
                                                       actionCollection, "tag_div_center", this );
    start = tagCase( I18N_NOOP("<div %1>") );
    end = tagCase( I18N_NOOP("</div>") );
    start = start.arg( attrCase( I18N_NOOP("align%2") ) );
    start = start.arg( I18N_NOOP("=\"center\"") );
    tagDivCenter->setTag( start, end );

    QuantaTagAction *tagDivRight = new QuantaTagAction( i18n( "Align Right" ), "div_right", 0,
                                                       actionCollection, "tag_div_right", this );
    start = tagCase( I18N_NOOP("<div %1>") );
    end = tagCase( I18N_NOOP("</div>") );
    start = start.arg( attrCase( I18N_NOOP("align%2") ) );
    start = start.arg( I18N_NOOP("=\"right\"") );
    tagDivRight->setTag( start, end );

    QuantaTagAction *tagDivJustify = new QuantaTagAction( i18n( "Align Justify" ), "div_justify", 0,
                                                       actionCollection, "tag_div_justify", this );
    start = tagCase( I18N_NOOP("<div %1>") );
    end = tagCase( I18N_NOOP("</div>") );
    start = start.arg( attrCase( I18N_NOOP("align%2") ) );
    start = start.arg( I18N_NOOP("=\"justify\"") );
    tagDivJustify->setTag( start, end );
    // TODO: CSS

    //
    // Fonts
    //
    (void) new KAction( i18n( "Base Font" ), "tag_font_base", 0,
                        this, SLOT( slotTagBaseFont() ),
                        actionCollection, "tag_font_base" );

    QuantaTagAction *tagFontInc = new QuantaTagAction( i18n( "Font Size +1" ), "font_inc", 0,
                                                       actionCollection, "tag_font_inc", this );
    start = tagCase( I18N_NOOP("<font %1>") );
    end = tagCase( I18N_NOOP("</font>") );
    start = start.arg( attrCase( I18N_NOOP("size%2") ) );
    start = start.arg( "=\"+1\"" );
    tagFontInc->setTag( start, end );

    QuantaTagAction *tagFontDec = new QuantaTagAction( i18n( "Font Size -1" ), "font_dec", 0,
                                                       actionCollection, "tag_font_dec", this );
    start = tagCase( I18N_NOOP("<font %1>") );
    end = tagCase( I18N_NOOP("</font>") );
    start = start.arg( attrCase( I18N_NOOP("size%2") ) );
    start = start.arg( "=\"-1\"" );
    tagFontDec->setTag( start, end );

    QuantaTagAction *tagPre = new QuantaTagAction( i18n( "Pre-Formatted" ), "tag_pre", 0,
                                                   actionCollection, "tag_pre", this );
    tagPre->setTag( I18N_NOOP( "pre" ) );

    QuantaTagAction *tagSub = new QuantaTagAction( i18n( "Subscript" ), "tag_sub", 0,
                                                   actionCollection, "tag_sub", this );
    tagSub->setTag( I18N_NOOP( "sub" ) );

    QuantaTagAction *tagSup = new QuantaTagAction( i18n( "Superscript" ), "tag_sup", 0,
                                                   actionCollection, "tag_sup", this );
    tagSup->setTag( I18N_NOOP( "sup" ) );

    //
    // Headings
    //
    QuantaTagAction *tagH1 = new QuantaTagAction( i18n( "Heading 1" ), "tag_h1", CTRL+Key_1,
                                                   actionCollection, "tag_h1", this );
    tagH1->setTag( I18N_NOOP( "h1" ) );
    QuantaTagAction *tagH2 = new QuantaTagAction( i18n( "Heading 2" ), "tag_h2", CTRL+Key_2,
                                                   actionCollection, "tag_h2", this );
    tagH2->setTag( I18N_NOOP( "h2" ) );
    QuantaTagAction *tagH3 = new QuantaTagAction( i18n( "Heading 3" ), "tag_h3", CTRL+Key_3,
                                                   actionCollection, "tag_h3", this );
    tagH3->setTag( I18N_NOOP( "h3" ) );
    QuantaTagAction *tagH4 = new QuantaTagAction( i18n( "Heading 4" ), "tag_h4", CTRL+Key_4,
                                                   actionCollection, "tag_h4", this );
    tagH4->setTag( I18N_NOOP( "h4" ) );
    QuantaTagAction *tagH5 = new QuantaTagAction( i18n( "Heading 5" ), "tag_h5", CTRL+Key_5,
                                                   actionCollection, "tag_h5", this );
    tagH5->setTag( I18N_NOOP( "h5" ) );

    //
    // Tables
    //
    (void) new KAction( i18n( "Table..." ), "tag_table", 0,
                        this, SLOT( slotTagTable() ),
                        actionCollection, "tag_table" );
    (void) new KAction( i18n( "Table Row" ), "tag_table_row", 0,
                        this, SLOT( slotTagTableRow() ),
                        actionCollection, "tag_table_row" );
    (void) new KAction( i18n( "Table Head" ), "table_head", 0,
                        this, SLOT( slotTagTableHead() ),
                        actionCollection, "tag_table_head" );
    (void) new KAction( i18n( "Table Data" ), "table_data", 0,
                        this, SLOT( slotTagTableData() ),
                        actionCollection, "tag_table_data" );

    QuantaTagAction *tagTh = new QuantaTagAction( i18n( "Table Head" ), "tag_th", 0,
                                                   actionCollection, "tag_th", this );
    tagTh->setTag( I18N_NOOP( "th" ) );

    QuantaTagAction *tagTr = new QuantaTagAction( i18n( "Table Row" ), "tag_tr", 0,
                                                   actionCollection, "tag_tr", this );
    tagTr->setTag( I18N_NOOP( "tr" ) );

    QuantaTagAction *tagTd = new QuantaTagAction( i18n( "Table Data" ), "tag_td", 0,
                                                   actionCollection, "tag_td", this );
    tagTd->setTag( I18N_NOOP( "td" ) );

    QuantaTagAction *tagCaption = new QuantaTagAction( i18n( "Caption" ), "tag_caption", 0,
                                                       actionCollection, "tag_caption", this );
    tagCaption->setTag( I18N_NOOP( "caption" ) );

    //
    // Forms
    //
    (void) new KAction( i18n( "Form..." ), "form", 0,
                        this, SLOT( slotTagForm() ),
                        actionCollection, "tag_form" );

    (void) new KAction( i18n( "Radio Button" ), "radio", 0,
                        this, SLOT( slotTagFormRadio() ),
                        actionCollection, "tag_form_radio" );
    (void) new KAction( i18n( "Check Box" ), "check", 0,
                        this, SLOT( slotTagFormCheck() ),
                        actionCollection, "tag_form_check" );

    (void) new KAction( i18n( "Selection List" ), "select", 0,
                        this, SLOT( slotTagSelect() ),
                        actionCollection, "tag_form_select" );

    (void) new KAction( i18n( "Line Edit" ), "lineedit", 0,
                        this, SLOT( slotTagFormLineEdit() ),
                        actionCollection, "tag_form_lineedit" );
    (void) new KAction( i18n( "Password Edit" ), "linepas", 0,
                        this, SLOT( slotTagFormPas() ),
                        actionCollection, "tag_form_passedit" );

    (void) new KAction( i18n( "Text Area" ), "textarea", 0,
                        this, SLOT( slotTagFormTextArea() ),
                        actionCollection, "tag_form_textarea" );

    (void) new KAction( i18n( "Submit Button" ), "submit", 0,
                        this, SLOT( slotTagFormSubmit() ),
                        actionCollection, "tag_form_submit" );
    (void) new KAction( i18n( "Reset Button" ), "reset", 0,
                        this, SLOT( slotTagFormReset() ),
                        actionCollection, "tag_form_reset" );

    //
    // Lists
    //
    QuantaTagAction *tagUl = new QuantaTagAction( i18n( "Unordered List" ), "tag_ul", CTRL+Key_U,
                        actionCollection, "tag_ul", this );
    tagUl->setTag( I18N_NOOP( "ul" ) );

    QuantaTagAction *tagOl = new QuantaTagAction( i18n( "Ordered List" ), "tag_ol", 0,
                        actionCollection, "tag_ol", this );
    tagOl->setTag( I18N_NOOP( "ol" ) );

    QuantaTagAction *tagLi = new QuantaTagAction( i18n( "List Item" ), "tag_li", CTRL+Key_L,
                        actionCollection, "tag_li", this );
    tagLi->setTag( I18N_NOOP( "li" ) );

    QuantaTagAction *tagDl = new QuantaTagAction( i18n( "Definition List" ), "tag_dl", 0,
                        actionCollection, "tag_dl", this );
    tagDl->setTag( I18N_NOOP( "dl" ) );

    QuantaTagAction *tagDt = new QuantaTagAction( i18n( "Definition Term" ), "tag_dt", 0,
                        actionCollection, "tag_dt", this );
    tagDt->setTag( I18N_NOOP( "dt" ) );

    QuantaTagAction *tagDd = new QuantaTagAction( i18n( "Definition" ), "tag_dd", 0,
                        actionCollection, "tag_dd", this );
    tagDd->setTag( I18N_NOOP( "dd" ) );

    QuantaTagAction *tagMenu = new QuantaTagAction( i18n( "Menu" ), "tag_menu", 0,
                        actionCollection, "tag_menu", this );
    tagMenu->setTag( I18N_NOOP( "menu" ) );
}

#else

void QuantaView::initActions()
{
}

#endif // NEW_STUFF
