/***************************************************************************
                          quantaview.h  -  description
                             -------------------
    begin                : ��� ���  9 13:29:57 EEST 2000
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

#ifndef QUANTAVIEW_H
#define QUANTAVIEW_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif 

// include files for Qt
#include <qwidget.h>

class QuantaApp;
class QuantaDoc;

class WToolBar;

class Document;
class QTabWidget;
class KProcess;

/** The QuantaView class provides the view widget for the QuantaApp instance.	
 * The View instance inherits QWidget as a base class and represents the view object of a KTMainWindow. As QuantaView is part of the
 * docuement-view model, it needs a reference to the document object connected with it by the QuantaApp class to manipulate and display
 * the document structure provided by the QuantaDoc class.
 * 	
 * @author Source Framework Automatically Generated by KDevelop, (c) The KDevelop Team.
 * @version KDevelop version 0.4 code generation
 */
class QuantaView : public QWidget
{
  Q_OBJECT

  friend class QuantaApp;
  friend class QuantaDoc;

  public:
    /** Constructor for the main view */
    QuantaView( QuantaApp *app, QWidget *parent = 0, const char *name=0);
    //QuantaView( QWidget *parent,  const char* name);
    /** Destructor for the main view */
    ~QuantaView();

    /** returns a pointer to the document connected to the view instance. Mind that this method requires a QuantaApp instance as a parent
     * widget to get to the window document pointer by calling the QuantaApp::getDocument() method.
     *
     * @see QuantaApp#getDocument
     */
    QuantaDoc *getDocument() const;

    /** contains the implementation for printing functionality */
    void print(QPrinter *pPrinter);


    /** repaint preview */
    //void repaintPreview( bool scroll = true);
    /** return current KWrite class */
    Document* write();
    /** Add new kwrite class to writeStack and return id in stack */
		void addWrite( Document* w , QString label );
    /** remove KWrite class from stack, return id of new KWrite */
    Document* removeWrite();
	  /** initialise tags menu */
	  void initMenu();
	  /**  */
	  void insertTag( char *tag);
	
	  /** convert tag to upper or lower case */
	  QString tagCase( const char*  tag);
  	/** convert attribute of tag to upper or lower case */
  	QString attrCase( const char*  attr);

	public slots:
	
	  /** Callback for toolbar with tags. */
	  void slotToolbarCallback( int id );
  /** insert <img > tag */
  void slotTagImg();
  /** for <a href> tag */
  void slotTagA();
  /** fro <a href=mailto> tag  */
  void slotTagMail();
  /** quick html text generate */
  void slotTagQuickStart();
  /** insert  <font> tag in document */
  void slotTagFont();
  /** insert tag <basefont> */
  void slotTagBaseFont();
  /** for tag <table> */
  void slotTagTable();
  /** do quick list */
  void slotTagQuickList();
  /** for quick create table */
  void slotTagQuickTable();
  /** for row properties */
  void slotTagTableRow();
  /** head properties */
  void slotTagTableHead();
  /** table data properties */
  void slotTagTableData();
  /** Open color Dialog and
insert color in the text */
  void slotTagColor();
  /**  */
  void slotTagFormRadio();
  /** insert TextArea tag */
  void slotTagFormTextArea();
  /** insert Form tag */
  void slotTagForm();
  /**  */
  void slotTagFormCheck();
  /**  */
  void slotTagFormPas();
  /**  */
  void slotTagFormLineEdit();
  /** insert  &nbsp; */
  void slotTagNbsp();
  /** insert  &szlig; */
  void slotTagSzlig();
  /** insert  &auml; */
  void slotTagauml();
  /** insert  &uuml; */
  void slotTaguuml();
  /** insert  &ouml; */
  void slotTagouml();
  /** insert  &Auml; */
  void slotTagAuml();
  /** insert  &Uuml; */
  void slotTagUuml();
  /** insert  &Ouml; */
  void slotTagOuml();
  /** insert  &amp; */
  void slotTagAmp();
  /** insert  &lt; */
  void slotTagLt();
  /** insert  &gt; */
  void slotTagGt();
  /** insert <br> tag */
  void slotTagBr();
  /** Add new kwrite class to
      writeStack and return id in stack */
  /** insert date */
  void slotTagDate();
  /** for select form */
  void slotTagSelect();
  /**  */
  void slotViewInKFM();
  /** view in external browser netscape */
  void slotViewInNetscape();
  /** check netscape status */
  void slotNetscapeStatus(KProcess *proc);
  /** insert tags from tree view */
  void slotInsertTagFromTree(QString name);
  /**  */
  void slotNewCurPos();
  /** edit tag */
  void slotEditCurrentTag();
  /**  */
  void slotTagFormReset();
  /**  */
  void slotTagFormSubmit();
  /** slot for user toolbar */
  void userToolbarCallback(int);
  /** configure toolbar "other" */
  void slotToolBarConfig();

  void slotGetScriptOutput(KProcess *proc, char *buffer, int buflen);

  signals: // Signals
  /**  */
  void newCurPos();
  /** emit when select document from tabbar */
  void writeSelected(int);
	
  private:
  	/** Quanta classes */
  	QuantaApp *app;
  	QuantaDoc *doc;
  	
  	/** tool bar Standard */
  	WToolBar *bStandard;
  	/** tool bar fonts */
  	WToolBar *bFonts;
  	/** tool bar fonts */
  	WToolBar *bTables;
  	/** tool bar tables */
  	WToolBar *bForms;
  	/** tool bar forms */
  	WToolBar *bLists;
  	/** tool bar user */
  	WToolBar *bUser;
  	
  	QTabWidget *writeTab;
  	
    /** collum of cursor position */
    int column;
    QString space;

};

#endif // QUANTAVIEW_H
