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
#include <qlist.h>

#include "quanta.h"

class QuantaApp;
class QuantaDoc;

class WToolBar;

class Document;
class QTabWidget;
class KProcess;
class KDockTabGroup;
class ToolBars;
class QTabBar;
class QWidgetStack;

/** The QuantaView class provides the view widget for the QuantaApp
 * instance.  The View instance inherits QWidget as a base class and
 * represents the view object of a KTMainWindow. As QuantaView is part
 * of the docuement-view model, it needs a reference to the document
 * object connected with it by the QuantaApp class to manipulate and
 * display the document structure provided by the QuantaDoc class.
 * 	
 * @author Source Framework Automatically Generated by KDevelop, (c) The KDevelop Team.
 * @version KDevelop version 0.4 code generation */
class QuantaView : public QWidget
{
  Q_OBJECT

  friend class QuantaApp;
  friend class QuantaDoc;

  public:
  
    QuantaView( QuantaApp *app, QWidget *parent = 0, const char *name=0);
    ~QuantaView();

    QuantaApp *getApp() const { return app; }
    QuantaDoc *getDoc() const { return app->getDoc(); }

    /** contains the implementation for printing functionality */
    void print(QPrinter *pPrinter);

    /** return current KWrite class */
    Document* write();
    
    /** Add new kwrite class to writeStack and return id in stack */
		void addWrite( Document* w , QString label );
		
    /** remove KWrite class from stack, return id of new KWrite */
    Document* removeWrite();
    
	  /** initialise tags menu */
	  void initMenu();

      void initActions();

	  void insertTag( const char *tag);
	
	  /** convert tag to upper or lower case */
	  QString tagCase( const char*  tag);
	  
  	/** convert attribute of tag to upper or lower case */
  	QString attrCase( const char*  attr);
  	
  	void updateToolBars( ToolBars *);
  /** No descriptions */
  void resizeEvent (QResizeEvent *);

	public slots:

    void slotTagImg();
    void slotTagA();
    void slotTagMail();
    void slotTagQuickStart();
    void slotTagFont();
    void slotTagBaseFont();
    void slotTagTable();
    void slotTagQuickList();
    void slotTagQuickTable();
    void slotTagTableRow();
    void slotTagTableHead();
    void slotTagTableBody();
    void slotTagTableData();
    void slotTagColor();
    void slotTagFormRadio();
    void slotTagFormTextArea();
    void slotTagForm();
    void slotTagFormCheck();
    void slotTagFormPas();
    void slotTagFormLineEdit();
    void slotTagFormReset();
    void slotTagFormSubmit();
    void slotTagNbsp();
    void slotTagSzlig();
    void slotTagauml();
    void slotTaguuml();
    void slotTagouml();
    void slotTagAuml();
    void slotTagUuml();
    void slotTagOuml();
    void slotTagAmp();
    void slotTagLt();
    void slotTagGt();
    void slotTagBr();
    void slotTagDate();
    void slotTagSelect();
    
    void slotInsertCSS();
    
    void slotNewCurPos();
    
    void slotEditCurrentTag();
    
    void slotInsertTagFromTree(QString name);
     
    void slotViewInKFM();
    void slotViewInNetscape();
    void slotNetscapeStatus(KProcess *proc);
    void slotViewInLynx();
  
    void slotGetScriptOutput(KProcess *proc, char *buffer, int buflen);
    void slotGetScriptError (KProcess *proc, char *buffer, int buflen);
  
    void slotPasteHTMLQuoted();
    void slotPasteURLEncoded();
    /** Add the starting and closing text for a
    user specified tag. */
    void slotTagMisc();
    void slotInsertChar(const QString &selected);

/** Kate releated slots */
//Edit
    void slotUndo ();
    void slotRedo ();

    void slotCut ();
    void slotCopy ();
    void slotPaste ();

    void slotSelectAll ();
    void slotDeselectAll ();
    void toggleVertical ();

    void slotFind ();
    void slotFindAgain ();
    void slotFindAgainB ();
    void slotReplace ();

    void slotIndent();
    void slotUnIndent();
    void slotComment ();
    void slotUnComment ();
    void slotApplyWordWrap ();

//Tools
    void slotGotoLine ();
    void slotSpellcheck ();

//Bookmarks
    void toggleBookmark();
    void clearBookmarks();
    void gotoMark (KTextEditor::Mark *mark);


//Settings
    void toggleIconBorder ();
    void toggleLineNumbers();
    void editorOptions ();
    void setEol(int);


  signals:
    void newCurPos();
    
    /** emit when select document from tabbar */
    void writeSelected(int);
	
  private:
  	/** Quanta classes */
  	QuantaApp *app;
  	QuantaDoc *doc;
  	
  	QTabBar      *tabBar;
  	QWidgetStack *toolbarStack;

#ifdef USE_KDOCKTABGROUP
	KDockTabGroup *writeTab;
#else 	  	
  	QTabWidget   *writeTab;
#endif
    /** collum of cursor position */
    int column;
    QString space;

    bool beginOfScriptOutput;
    bool beginOfScriptError;
    QString scriptOutputDest;
    QString scriptErrorDest;
public: // Public attributes
  /**  */
  Document *oldWrite;
  /**  */
  QString dontShowSavePreview;
};

#endif // QUANTAVIEW_H
