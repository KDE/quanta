/***************************************************************************
                          quanta.h  -  description
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

#ifndef QUANTA_H
#define QUANTA_H
 
#ifndef VERSION
  #define VERSION "2"
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// include files for Qt
#include <qstrlist.h>

// include files for KDE 
#include <kapp.h>
#include <kmainwindow.h>
#include "widgets/whtmlpart.h"
#include <kparts/browserextension.h>

// forward declaration
class QuantaDoc;
class QuantaView;

class WSplitter;
class QTabWidget;
class QWidgetStack;

class WHTMLPart;
class KHTMLView;

class KAction;
class KToggleAction;
class QListViewItem;
class KRecentFilesAction;

class Node;
class Parser;
class DocTreeView;
class FilesListView;
class FilesTreeView;
class StructTreeView;
class ProjectTreeView;
class Project;
class ToolBars;
class GrepDialog;
class MessageOutput;
class QDomDocument;

/**
  * The base class for Quanta application windows. 
  */
class QuantaApp : public KMainWindow
{
  Q_OBJECT

  friend class QuantaDoc;
  friend class QuantaView;

  public:
    QuantaApp();
    ~QuantaApp();
    
    QuantaDoc  *getDoc()   { return doc; };
    QuantaView *getView()  { return view;};

    MessageOutput *getMessages() { return messageOutput; }

    void openLastFiles();
    
    /** from @see KXMLGUIBuilder, for tollbars in tabbar ( tags,... )
     *
     */
    QWidget* createContainer( QWidget *parent, int index, const QDomElement &element, int &id );  
    void removeContainer( QWidget *container, QWidget *parent, QDomElement &element, int id );

  protected:
  
    void saveOptions();
    void readOptions();
    
    void initContextMenu();
    void initActions();
    void initStatusBar();
    void initDocument();
    void initView();
    void initProject();
    void initTagDict();
    
    virtual bool queryExit();
    
    void setHSplit(int);
    void setVSplit(int);

  public slots:
    
    void slotFileNew ();
    void slotFileOpen();
    void slotFileOpen( KURL & );
    void slotFileSave    ();
    void slotFileSaveAs  ();
    void slotFileSaveAll ();
    void slotFileClose   ();
    void slotFileCloseAll();
    void slotFilePrint   ();
    void slotFileNext    ();
		void slotFilePrev    ();
    void slotFileQuit    ();
    
    void slotImageOpen        (QString);
    void slotFileOpenRecent   (const KURL&);
    void slotInsertTag        (QString);

    void slotEditFindInFiles   ();
    void slotEditIndent        ();
    void slotEditUnindent      ();
    void slotEditCleanIndent   ();
    void slotEditGotoLine      ();
    
    void slotToolSyntaxCheck();

    /// open url in documentation window
    void openDoc( QString url );

    void updateNavButtons( bool back, bool forward );

    void contextHelp();

    void slotViewToolBar  ();
    void slotViewStatusBar();
    
    void slotStatusMsg    (const QString &text);
    
    void slotNewUndo      ();
    void slotNewStatus    ();
    void slotNewMarkStatus();
    void slotNewLineColumn();
    void slotUpdateStatus (const QString &);

    void slotSwapLeftPanelMode();

    void slotLeftTabChanged(QWidget *);

    void slotActivatePreview();
    
    /** show preview ( F6 )*/
    void slotShowPreview();
    
    void slotShowLeftPanel();

    void slotFtpClient();

    /** Repaint preview ( slot ) */
    void slotViewRepaint();

    void slotViewMessages();
    
    /** repaint preview */
    void repaintPreview( bool clear = false);

    /** options slots */
    void slotOptions();

    /** configure keybindings */
    void slotOptionsConfigureKeys();

    void slotOptionsConfigureToolbars();
    
    void slotOptionsConfigureActions();

    /** set highlight and eol */
    void slotSetHl(int);
    void slotSetEol(int);

    /** reparse current document and initialize node. */
	  void reparse();
	
	  void setCursorPosition( int row, int col );
    void gotoFileAndLine  ( const QString &filename, int line );

	  void selectArea(int col1, int row1, int col2, int row2);
	
	  void slotSelectMessageWidget ();
	  void slotDisableMessageWidget();
	  
	  void setTitle(QString);
	  
	protected:
	  KParts::BrowserExtension *browserExtension() {
      return static_cast<KParts::BrowserExtension *>(((KParts::ReadOnlyPart *)htmlPart())->child( 0L, "KParts::BrowserExtension" ));
    }
	
  private:
    /** project class */
    Project *project;

    /** return htmlPart or 0L if use previewPosition=="Disabled" */
    WHTMLPart    *htmlPart();
    QWidgetStack *widgetStackOfHtmlPart();
    
    // config
    KConfig *config;

    /** widget stack for left panel */
    QWidgetStack *fTab;
    
    /** tabs for left panel */
    ProjectTreeView  *pTab;
    DocTreeView 		 *dTab;
    FilesTreeView		*fTTab;
    FilesListView		*fLTab;
    StructTreeView   *sTab;

    QString previewPosition;

    QWidgetStack *topWidgetStack;
    QWidgetStack *rightWidgetStack;
    QWidgetStack *bottomWidgetStack;

    WSplitter *hSplit;
    WSplitter *vSplit;

   	/** HTML class for preview */
    WHTMLPart *htmlPartTop;
    WHTMLPart *htmlPartBottom;
    WHTMLPart *htmlPartRight;
    WHTMLPart *htmlPartDoc;

    /** Messaage output window */
    MessageOutput *messageOutput;

    /** Grep window */
    GrepDialog *grepDialog;

    /** qtabwidget for left panel */
    QTabWidget *leftPanel;
    
    // DOC & VIEW
    QuantaDoc  *doc;
    QuantaView *view;

    /** parsered tree of document  */
    Node   *node;
    Parser *parser;

    ToolBars *toolbars;

    int phpDebugPort;
    
    // ACTIONS
    KRecentFilesAction *fileRecent;
    
    KToggleAction *verticalSelectAction;
    
    KAction *saveAction, *saveAllAction, *undoAction, *redoAction,
      *cutAction, *copyAction, *findNextAction, *backAction, *forwardAction,
      *closeprjAction, *insertFileAction, *insertDirAction, 
      *rescanPrjDirAction, *uploadProjectAction, *projectOptionAction;
      
    QDomDocument *actions;
};
 
#endif // QUANTA_H
