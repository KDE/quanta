/***************************************************************************
                          quantaview.h  -  description
                             -------------------
    begin                : ���� 9 13:29:57 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com>
                           (C) 2001-2002 Andras Mantia <amantia@kde.org>
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
#include <qptrlist.h>
#include <qvaluelist.h>

class QuantaDoc;
class Document;
class QTabWidget;
class KProcess;
class KDockTabGroup;
class KConfig;
class QTabBar;
class QWidgetStack;
class QDropEvent;
class QPopupMenu;
class ToolbarTabWidget;
class KafkaDocument;
class KURL;
namespace DOM
{
  class Node;
}
namespace Kate
{
  class View;
}
namespace KTextEditor
{
  class Mark;
}
class QSplitter;
class Node;

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

public:

  QuantaView(QWidget *parent = 0, const char *name=0);
  ~QuantaView();

  //for dual-views mode sync and VPL config
  void readConfig(KConfig *m_config);

  ToolbarTabWidget *toolbarTab() const {return m_toolbarTab;}
  QTabWidget *writeTab() const {return m_writeTab;}

  /** return current KWrite class */
  Document* write();
  /** Add new kwrite class to writeStack and return id in stack */
  void addWrite( QWidget* w , QString label );
  /** remove KWrite class from stack, return id of new KWrite */
  QWidget* removeWrite();
#ifdef BUILD_KAFKAPART

  /** Return the KafkaDocument */
  KafkaDocument *getKafkaInterface() {return kafkaInterface;}

  /** Tells which widget had the focus the more recently */
  int hadLastFocus() {return currentFocus;}

  enum focuses {
    quantaFocus = 0,
    kafkaFocus
  };

  /** Return the curren views layout*/
  int getViewsLayout() {return currentViewsLayout;}
#endif

    enum viewsLayout {
    QuantaViewOnly = 0,
    QuantaAndKafkaViews,
    KafkaViewOnly
  };

#ifdef BUILD_KAFKAPART
  /** Reloads both views ONLY when changes have been made to the Node tree ONLY.
    * Set force to true if you want to reload even if not necessary.*/
  void reloadBothViews(bool force = false);
  /** reload the Kafka view from the Node Tree. Set force to true if you want to reload even if not necessary. */
  void reloadKafkaView(bool force = false);
  /** reload the Quanta view from the Node Tree. Set force to true if you want to reload even if not necessary. */
  void reloadQuantaView(bool force = false);
#endif

  /** Resize the current view */
  void resize(int width, int height);

  /** Update the views when the current page changed. Called by quantaApp::slotUpdateStatus. */
  void updateViews();

  /** initialise tags menu */
  void initMenu();
  void initActions();
  void insertTag( const char *tag);

  /** No descriptions */
 // void resizeEvent (QResizeEvent *);
  /** Insert a new tag by bringing up the TagDialog. */
  void insertNewTag(QString tag, QString attr = QString::null,bool insertInLine = true);

  /**
   * Show a TagDialog of Node tag, with attrs attr.
   * @param tag The name of the new Node to create.
   * @param attr The string containing the attrs of the new Node to create.
   * @return Returns a new Node created according to the contents of the TagDialog.
   */
  Node *showTagDialogAndReturnNode(QString tag, QString attr = QString::null);
  /** Returns the baseURL of the document. */
  KURL baseURL();
  /** True if a Document object exists, false otherwise. */
  bool writeExists();

public slots:
  void slotTagMail();
  void slotTagQuickList();
  void slotTagEditTable();
  void slotTagColor();
  void slotTagDate();
  void slotTagSelect();
  /** Add the starting and closing text for a
  user specified tag. */
  void slotTagMisc();
  void slotEditCurrentTag();

  void slotInsertCSS();
  void slotFrameWizard();
  void slotNewCurPos();

  void slotViewInKFM();
  void slotViewInNetscape();
  void slotViewInMozilla();
  void slotViewInOpera();
  void slotViewInNetscapeOrMozilla();
  void slotNetscapeStatus(KProcess *proc);
  void slotViewInLynx();

  void slotGetScriptOutput(KProcess *proc, char *buffer, int buflen);
  void slotGetScriptError (KProcess *proc, char *buffer, int buflen);

  void slotPasteHTMLQuoted();
  void slotPasteURLEncoded();
  void slotInsertChar(const QString &selected);

/** Kate related slots */
//Edit
  void slotUndo ();
  void slotRedo ();

  void slotCut ();
  void slotCopy ();
  void slotPaste ();

  void slotSelectAll ();
  void slotDeselectAll ();
  void toggleVertical ();
  void toggleInsert();

  void slotIndent();
  void slotUnIndent();
  void slotCleanIndent();
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
  void toggleDynamicWordWrap();
  void toggleIconBorder();
  void toggleLineNumbers();
  void slotEditorOptions();
  void setEol(int);

  //views
  void slotShowQuantaEditor();
  void slotShowKafkaPart();
  void slotShowKafkaAndQuanta();

  /**
   * Called whenever the KafkaWidget widget get/lost the focus.
   */
  void slotKafkaGetFocus(bool focus);

  /**
   * Called whenever the KTextEditor::View widget get the focus.
   */
  void slotQuantaGetFocus(Kate::View *view);

  /**
   * Called when we want to set the Quanta cursor.
   * Record the position until Quanta get the focus again.
   * Useful when we want to set it when quanta doesn't have the focus.
   */
  void slotSetQuantaCursorPosition(int col, int line);

  /**
   * Called when we want to set the kafka cursor.
   * Record the position until kafka get the focus.
   * Useful when we want to set it when kafka doesn't have the fovus.
   */
  void slotSetKafkaCursorPosition(DOM::Node node, int offset);

  /** closes all the other but active tabs */
  void slotCloseOtherTabs();

signals:
  void newCurPos();
  /** emit when select document from tabbar */
  void writeSelected(int);
  /** emitted when a file from the template view is dropped on the view */
  void dragInsert(QDropEvent *);

public:
  Document *oldWrite;
  QWidget *oldTab;

private:
  QuantaDoc *doc;

  /** collum of cursor position */
  int column;
  QString space;

#ifdef BUILD_KAFKAPART
/** Kafka stuff */
  KafkaDocument *kafkaInterface;
  QSplitter *splitter;
  QValueList<int> _splittSizes;
  int currentFocus;
  int quantaUpdateTimer;
  int kafkaUpdateTimer;
  int curCol, curLine, curOffset;
  DOM::Node curNode;
  bool m_needKafkaReload;
#endif
  int currentViewsLayout;

  bool beginOfScriptOutput;
  bool beginOfScriptError;
  QString scriptOutputDest;
  QString scriptErrorDest;
  ToolbarTabWidget *m_toolbarTab;
  QTabWidget *m_writeTab;
  QString m_netscape;

protected:
  virtual void dropEvent(QDropEvent *e);
  virtual void dragEnterEvent(QDragEnterEvent *e);

#ifdef BUILD_KAFKAPART
  /**
   * The timer event, called by the kafkaUpdateTimer and quantaTimerUpdate which
   * triggers the update of the other view.
   */
  virtual void timerEvent(QTimerEvent *e );

#endif
};

#endif // QUANTAVIEW_H
