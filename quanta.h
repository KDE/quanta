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
 

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// include files for Qt
#include <qstrlist.h>

// include files for KDE 
#include <kapp.h>
#include <ktmainwindow.h>
#include <kaccel.h>
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
class QListViewItem;

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
    
    /** opens a file specified by commandline option
     */
    void openDocumentFile(const char *_cmdl=0);
    /** returns a pointer to the current document connected to the KTMainWindow instance and is used by
     * the View class to access the document object's methods
     */	
    QuantaDoc *getDocument() const;
    QuantaDoc *getDoc()   { return doc; };
    QuantaView *getView() { return view;};

    MessageOutput *getMessages() { return messageOutput; }

    void openLastFiles();
    
    /** from @see KXMLGUIBuilder, for tollbars in tabbar ( tags,... )
     *
     */
    QWidget* createContainer( QWidget *parent, int index, const QDomElement &element, int &id );
 	

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
    
    
    virtual bool queryClose();
    virtual bool queryExit();
    virtual void saveProperties(KConfig *_cfg);
    virtual void readProperties(KConfig *_cfg);
    

  public slots:
    
    void slotFileNewWindow();
    void addRecentFile   (const QString &file);
    void addRecentProject(const QString &file);
    
    void slotFileNew ();
    void slotFileOpen();
    void slotFileOpen ( QString fileToOpen );
    void slotFileSave    ();
    void slotFileSaveAs  ();
    void slotFileSaveAll ();
    void slotFileClose   ();
    void slotFileCloseAll();
    void slotFilePrint   ();
    void slotFileNext    ();
		void slotFilePrev    ();
    void slotFileQuit    ();
    
    void slotImageOpen( QString fileToOpen );
    void slotFileOpenRecent   (int id_);
    void slotProjectOpenRecent(int id_);


    void slotEditCut           ();
    void slotEditCopy          ();
    void slotEditPaste         ();
    void slotEditUndo          ();
    void slotEditRedo          ();
    void slotURedoHistory      ();
    void slotEditSelectAll     ();
    void slotEditDeselectAll   ();
    void slotEditInvertSelect  ();
    void slotEditVerticalSelect();
    void slotEditSearch        ();
    void slotEditSearchAgain   ();
    void slotEditReplace       ();
    void slotEditFindInFiles   ();
    void slotEditIndent        ();
    void slotEditUnindent      ();
    void slotEditCleanIndent   ();
    void slotEditGotoLine      ();
    
    void slotToolSyntaxCheck();

    /** open url in documentation window
     */
    void openDoc( QString url );

    void updateNavButtons( bool back, bool forward );

    void contextHelp();

    /** toggles the toolbar
     */
    void slotViewToolBar();
    /** toggles the statusbar
     */
    void slotViewStatusBar();
    /** changes the statusbar contents for the standard label permanently, used to indicate current actions.
     * @param text the text that is displayed in the statusbar
     */
    void slotStatusMsg(const QString &text);
    /** changes the status message of the whole statusbar for two seconds, then restores the last status. This is used to display
     * statusbar messages that give information about actions for toolbar icons and menuentries.
     * @param text the text that is displayed in the statusbar
     */
    void slotStatusHelpMsg(const QString &text);
    
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
    
    void slotInsertTag(QString);

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
	  
	protected:
	  KParts::BrowserExtension *browserExtension() {
      return static_cast<KParts::BrowserExtension *>(((KParts::ReadOnlyPart *)htmlPart())->child( 0L, "KParts::BrowserExtension" ));
    }
	
  private:
    /** project class */
    Project *project;

    /** return htmlPart or 0L if use previewPosition=="Disabled" */
    WHTMLPart *htmlPart();
    QWidgetStack *widgetStackOfHtmlPart();

    /** contains the recently used filenames */
    QStrList recentFiles;
    QStrList recentProjects;

    // CONFIG
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
    QuantaDoc *doc;
    QuantaView *view;

    /** parsered tree of document  */
    Node   *node;
    Parser *parser;

    ToolBars *toolbars;

    int phpDebugPort;
};
 
#endif // QUANTA_H
