/***************************************************************************
                          quantaview.h  -  description
                             -------------------
    begin                : ���� 9 13:29:57 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com>
                           (C) 2001-2004 Andras Mantia <amantia@kde.org>
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

// include files for Qt
#include <qwidget.h>
#include <qptrlist.h>
#include <qvaluelist.h>
#include <qtimer.h>

//kde includes
#include <kmdichildview.h>

#ifdef BUILD_KAFKAPART
#include <dom/dom_node.h>
#endif

class QuantaDoc;
class Document;
class ToolbarTabWidget;
class KafkaDocument;
class QuantaPlugin;

class QSplitter;
class KURL;
class QGridLayout;

namespace DOM
{
  class Node;
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
class QuantaView : public KMdiChildView
{
  Q_OBJECT

public:

  QuantaView(QWidget *parent = 0, const char *name=0);
  ~QuantaView();

  /** returns true if the view can be removed, false otherwise */
  bool mayRemove();

  /**Adds a Document object to the view. Also creates the VPL widget for the document. */
  void addDocument(Document *document);

  /** Adds a custom widget to the view. */
  void addCustomWidget(QWidget *widget, const QString &label);

  /** returns the Document object associated with this view. Returns 0L if the view holds
  a non-Document object */
  Document *document() {return m_document;};

  /**Adds a QuantaPlugin object to the view.*/
  void addPlugin(QuantaPlugin *plugin);

  QWidget* documentArea() {return m_documentArea;}

   bool saveDocument(const KURL&);
   // "save modified" - asks the user
  // for saving if the document is modified
  bool saveModified();
  /** Returns the baseURL of the document. */
  KURL baseURL();

#ifdef BUILD_KAFKAPART
  /** Tells which widget had the focus the more recently */
  int hadLastFocus() {return m_currentFocus;}
  /** Reloads both views ONLY when changes have been made to the Node tree ONLY.
    * Set force to true if you want to reload even if not necessary.*/
  void reloadBothViews(bool force = false);
  /** reload the Kafka view from the Node Tree. Set force to true if you want to reload even if not necessary. */
  void reloadVPLView(bool force = false);
  /** reload the Quanta view from the Node Tree. Set force to true if you want to reload even if not necessary. */
  void reloadSourceView(bool force = false);
  /** Return the curren views layout*/
  int currentViewsLayout() {return m_currentViewsLayout;}
#endif

 /** Called when this view become the active one */
  void activated();
 /** Called when this view lost the active status */
  void deactivated();

  void resizeEvent(QResizeEvent* e);
 /** Resize the current view */
  void resize(int width, int height);

 /** Updates the icon at the name on the view tab */
  void updateTab();

  /** Returns the tab name associated with this view */
  QString tabName();

  void insertTag( const char *tag);

  /** Insert a new tag by bringing up the TagDialog. */
  void insertNewTag(const QString &tag, const QString &attr = QString::null, bool insertInLine = true);

  enum ViewFocus {
    SourceFocus = 0,
    VPLFocus
  };

  enum ViewLayout {
    SourceOnly = 0,
    SourceAndVPL,
    VPLOnly
  };


public slots:
  void slotSetSourceLayout();
  void slotSetSourceAndVPLLayout();
  void slotSetVPLOnlyLayout();

  /**
   * Called whenever the KafkaWidget widget get/lost the focus.
   */
  void slotVPLGetFocus(bool focus);

  /**
   * Called whenever the KTextEditor::View widget get the focus.
   */
  void slotSourceGetFocus();

  /**
   * Called when we want to set the Quanta cursor.
   * Record the position until Quanta get the focus again.
   * Useful when we want to set it when quanta doesn't have the focus.
   */
  void slotSetCursorPositionInSource(int col, int line);
 /**
   * Called when an error occured when loading kafka.
   * Pop up the Error reporter dialog.
   */
  void slotVPLLoadingError(Node *node);

  /** Restarts the update timers according to the current settings */
  void reloadUpdateTimers();

private slots:
  void slotSavingCompleted();
  void slotSavingFailed(const QString& error);

  /**
   * Called to update VPL.
   */
  void VPLUpdateTimerTimeout();

  /**
   * Called to update the source.
   */
  void sourceUpdateTimerTimeout();

signals:
  /** emitted when a file from the template view is dropped on the view */
  void dragInsert(QDropEvent *);
  /** asks for hiding the preview widget */
  void hidePreview();
  void showProblemsView();
  void cursorPositionChanged();
  void title(const QString &);
  /** emitted if this view contained an editor and it is closed */
  void documentClosed();

public:
  Document *oldWrite;
  QWidget *oldTab;

private:
  /** collum of cursor position */
  int column;

#ifdef BUILD_KAFKAPART
/** Kafka stuff */
  QValueList<int> m_splitterSizes;
  int m_curCol, m_curLine, m_curOffset;
  DOM::Node curNode;
  bool m_kafkaReloadingEnabled, m_quantaReloadingEnabled;
  QTimer m_sourceUpdateTimer, m_VPLUpdateTimer;
#endif

  QWidget *m_documentArea;///< the area of the view which can be used to show the source/VPL
  Document *m_document;
  QuantaPlugin *m_plugin;
  QWidget *m_customWidget; ///<view holds a custom widget, eg. a documentation
  KafkaDocument *m_kafkaDocument;
  QSplitter *m_splitter;
  QGridLayout *m_viewLayout;
  int m_currentViewsLayout; ///< holds the current layout, which can be SourceOnly, VPLOnly or SourceAndVPL
  int m_currentFocus;
  bool m_saveResult;
  bool m_eventLoopStarted;

protected:
  virtual void dropEvent(QDropEvent *e);
  virtual void dragEnterEvent(QDragEnterEvent *e);
};

#endif // QUANTAVIEW_H