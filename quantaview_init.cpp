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

#include <kaction.h>
#include "quanta_actions.h"

QuantaView::QuantaView( QuantaApp *app, QWidget *parent, const char *name )
	: QWidget( parent, name)
{
	this->app = app;
	this->doc = app->doc;
	
	QGridLayout *layout = new QGridLayout( this );
	
  tabBar = new QTabBar( this);
  toolbarStack = new QWidgetStack(this,"ToolBar stack");

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

  initActions();

  writeTab->show();
}

QuantaView::~QuantaView()
{
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
	
	writeTab->addTab  ( w,  emptyIcon,  label );
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
                        
    (void) new KAction( i18n( "Insert Font..." ), "tag_font", 0,
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

    // Standard style tags
    //
    QuantaTagAction *tagB 
      = new QuantaTagAction( i18n( "Bold" ), "tag_bold", CTRL+Key_B,
                             actionCollection, "tag_b", this );
                             
    tagB->setTag( I18N_NOOP( "b" ) );

    QuantaTagAction *tagI 
      = new QuantaTagAction( i18n( "Italic" ), "tag_i", CTRL+Key_I,
                             actionCollection, "tag_i", this );
                             
    tagI->setTag( I18N_NOOP( "i" ) );

    QuantaTagAction *tagU 
      = new QuantaTagAction( i18n( "Underline" ), "tag_u", CTRL+Key_U,
                             actionCollection, "tag_u", this );
                             
    tagU->setTag( I18N_NOOP( "u" ) );

    QuantaTagAction *tagBr 
      = new QuantaTagAction( i18n( "New Line" ), "tag_br", CTRL+Key_Enter,
                             actionCollection, "tag_br", this );
                             
    tagBr->setTag( I18N_NOOP( "br" ) );

    QuantaTagAction *tagP 
      = new QuantaTagAction( i18n( "Paragraph" ), "tag_p", CTRL+Key_0,
                             actionCollection, "tag_p", this );
                             
    tagP->setTag( I18N_NOOP( "p" ) );

    (void) new KAction( i18n( "Non-Breaking Space" ), "tag_nbsp", CTRL+Key_Space,
                        this, SLOT( slotTagNbsp() ),
                        actionCollection, "tag_nbsp" );

    QuantaTagAction *tagHr 
      = new QuantaTagAction( i18n( "Horizontal Rule" ), "tag_hr", CTRL+Key_Equal,
                             actionCollection, "tag_hr", this );
                             
    tagHr->setTag( I18N_NOOP( "hr" ) );

    QuantaTagAction *tagComment 
      = new QuantaTagAction( i18n( "Comment" ), "tag_comm", 0,
                             actionCollection, "tag_comment", this );
                             
    tagComment->setTag( I18N_NOOP( "<!-- " ), I18N_NOOP( " -->" ) );

    QuantaTagAction *tagDivLeft 
      = new QuantaTagAction( i18n( "Align Left" ), "div_left", 0,
                             actionCollection, "tag_div_left", this );
                             
    QString start = tagCase( I18N_NOOP("<div %1>") );
    QString end   = tagCase( I18N_NOOP("</div>"  ) );
    
    start = start.arg( attrCase( I18N_NOOP("align%2") ) );
    start = start.arg(           I18N_NOOP("=\"left\"") );
    
    tagDivLeft->setTag( start, end );

    QuantaTagAction *tagDivCenter 
      = new QuantaTagAction( i18n( "Align Center" ), "div_center", 0,
                             actionCollection, "tag_div_center", this );
                             
    start = tagCase( I18N_NOOP("<div %1>") );
    end   = tagCase( I18N_NOOP("</div>"  ) );
    
    start = start.arg( attrCase( I18N_NOOP("align%2"  ) ) );
    start = start.arg(           I18N_NOOP("=\"center\"") );
    
    tagDivCenter->setTag( start, end );

    QuantaTagAction *tagDivRight 
      = new QuantaTagAction( i18n( "Align Right" ), "div_right", 0,
                             actionCollection, "tag_div_right", this );
                             
    start = tagCase( I18N_NOOP("<div %1>") );
    end   = tagCase( I18N_NOOP("</div>"  ) );
    
    start = start.arg( attrCase( I18N_NOOP("align%2" ) ) );
    start = start.arg(           I18N_NOOP("=\"right\"") );
    
    tagDivRight->setTag( start, end );

    QuantaTagAction *tagDivJustify 
      = new QuantaTagAction( i18n( "Align Justify" ), "div_justify", 0,
                             actionCollection, "tag_div_justify", this );
                             
    start = tagCase( I18N_NOOP("<div %1>") );
    end   = tagCase( I18N_NOOP("</div>"  ) );
    
    start = start.arg( attrCase( I18N_NOOP("align%2") ) );
    start = start.arg(           I18N_NOOP("=\"justify\"") );
    
    tagDivJustify->setTag( start, end );

    (void) new KAction( i18n( "Insert CSS..." ), "mini-modules", 0,
                        this, SLOT( slotInsertCSS() ),
                        actionCollection, "insert_css" );

    // Fonts
    //
    (void) new KAction( i18n( "Base Font" ), "tag_font_base", 0,
                        this, SLOT( slotTagBaseFont() ),
                        actionCollection, "tag_font_base" );

    QuantaTagAction *tagFontInc 
      = new QuantaTagAction( i18n( "Font Size +1" ), "font_inc", 0,
                             actionCollection, "tag_font_inc", this );
                             
    start = tagCase( I18N_NOOP("<font %1>") );
    end   = tagCase( I18N_NOOP("</font>"  ) );
    
    start = start.arg( attrCase( I18N_NOOP("size%2") ) );
    start = start.arg( "=\"+1\"" );
    
    tagFontInc->setTag( start, end );

    QuantaTagAction *tagFontDec 
      = new QuantaTagAction( i18n( "Font Size -1" ), "font_dec", 0,
                             actionCollection, "tag_font_dec", this );
                             
    start = tagCase( I18N_NOOP("<font %1>") );
    end   = tagCase( I18N_NOOP("</font>"  ) );
    
    start = start.arg( attrCase( I18N_NOOP("size%2") ) );
    start = start.arg( "=\"-1\"" );
    
    tagFontDec->setTag( start, end );

    QuantaTagAction *tagPre 
      = new QuantaTagAction( i18n( "Pre-Formatted" ), "tag_pre", 0,
                             actionCollection, "tag_pre", this );
                             
    tagPre->setTag( I18N_NOOP( "pre" ) );

    QuantaTagAction *tagSub 
      = new QuantaTagAction( i18n( "Subscript" ), "tag_sub", 0,
                             actionCollection, "tag_sub", this );
                             
    tagSub->setTag( I18N_NOOP( "sub" ) );

    QuantaTagAction *tagSup 
      = new QuantaTagAction( i18n( "Superscript" ), "tag_sup", 0,
                             actionCollection, "tag_sup", this );
                             
    tagSup->setTag( I18N_NOOP( "sup" ) );

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
                        
    (void) new KAction( i18n( "Table Body" ), "tag_table_body", 0,
                        this, SLOT( slotTagTableBody() ),
                        actionCollection, "tag_table_body" );                    
                        
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
