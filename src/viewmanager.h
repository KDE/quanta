/***************************************************************************
                           viewmanager  -  description
    begin              : Fri Mar 26 2004
    copyright          : (C) 2004 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; version 2 of the License.
 *
 ***************************************************************************/

#ifndef VIEWMANAGER_H
#define VIEWMANAGER_H

#include <kurl.h>

//forward declarations
class Document;
class QuantaView;
class QuantaBookmarks;
class KafkaDocument;
class KPopupMenu;

class KMdiChildView;

/** This singleton class takes care of creating new views for documents, help, plugins, preview and so.
* As the name says, it also manages the views. */

class ViewManager : public QObject
{
   Q_OBJECT
public:
  /** Returns a reference to the viewmanager object */
  static ViewManager* const ref(QObject *parent = 0L, const char *name = 0L)
  {
    static ViewManager *m_ref;
    if (!m_ref) m_ref = new ViewManager(parent, name);
    return m_ref;
  }
  /** The destructor. */
  virtual ~ViewManager(){};

  /** Creates a QuantaView object */
  QuantaView *createView(const QString &caption = QString::null);
  /** Removes a QuantaView object. Returns false on failure (eg. the view was not saved and it refused
  the delete itself.) If force is true, the view is removed without asking for save.
  */
  bool removeView(QuantaView *view, bool force = false, bool createNew = true);
  /** Returns the active view */
  QuantaView *activeView();
  /** Returns the active document or 0L */
  Document *activeDocument();
  /** Returns the view holding the document loaded from url. */
  QuantaView *isOpened(const KURL &url);

  bool saveAll();

  /** Returns true if at least one view has the modified flag set. */
  bool isOneModified();

  void createNewDocument();
/** Returns a list with the URLs of the opened documents */
  KURL::List openedFiles(bool noUntitled=true);
  /** Returns a list with the Document* object of the opened documents */
  QValueList<Document*> openedDocuments();

  /** Returns the view holding the documentation widget. If create is true and there is no such view yet,
  it creates one. */
  QuantaView *documentationView(bool create = true);

  /** Returns the last active view which had an editor inside or 0 if there was no such view */
  QuantaView *lastActiveEditorView() {return m_lastActiveEditorView;}
  
  /** Return the URL of the currently active document */
  QString currentURL();

public slots:
  /**called when a new view was activated */
  void slotViewActivated(KMdiChildView *view);

  /** Removes the active view Returns false on failure (eg. the view was not saved and it refused the delete itself.) */
  bool removeActiveView(bool createNew = true)  { return removeView(activeView(), false, createNew); }
  /** closes all the other but active tabs */
  void slotCloseOtherTabs();
  /** closes all views. If createNew is true, it creates a new view after closing the others. Return true if all documents were closed.*/
  bool closeAll(bool createNew = true);
  /** called when the last view is closed */
  void slotLastViewClosed();

  /** called when the context menu was invoked on a tab */
  void slotTabContextMenu(QWidget *widget, const QPoint & point);
  /** called when the user requests to close a tab with the close button */
  void slotCloseRequest(KMdiChildView *widget);
  /** called from the views and just emits the signal @ref documentClosed */
  void slotDocumentClosed(const KURL&);

signals:
  /** emitted when a file from the template view is dropped on a view */
  void dragInsert(QDropEvent *);
  /** emitted when a view was activated */
  void viewActivated(const KURL &);
  /** emitted when a view was closed */
  void documentClosed(const KURL&);
  void eventHappened(const QString&, const QString&, const QString& );
  /** emitted when all files were closed. The argument is true if the closes
  was successful, false if the unser canceled the closing */
  void filesClosed(bool);

private slots:
 /** called before the file list menu shows up, so it can be updated */
  void slotFileListPopupAboutToShow();
  /** called when an item is selected in the file list menu */
  void slotFileListPopupItemActivated(int id);

  /** Handle tab context menus for editor views */
  void slotReloadFile();
  void slotUploadFile();
  void slotDeleteFile();
  void slotCloseView();

private:
  /** Private constructor for the singleton object. */
  ViewManager(QObject * parent = 0, const char * name = 0);
  /** Returns true if there isn't any opened view holding an editor */
  bool allEditorsClosed();

  QuantaView *m_lastActiveView; ///< Holds the last active view. Used to deactivate it when a new view is selected
  QuantaView *m_lastActiveEditorView; ///< Contains the last active view which has an editor inside
  QuantaView *m_documentationView; ///< Contains the view which holds the documentation browser
  KPopupMenu *m_tabPopup; ///< the menu which pops up when the user clicks on a view tab
  KPopupMenu *m_fileListPopup; ///< a menu containing the opened views as menu items
  KPopupMenu *m_bookmarksMenu;
  QuantaBookmarks *m_bookmarks;
    
  QuantaView *m_contextView; ///<the tab where the context menu was requested
  bool m_separatorVisible;
  int m_cvsMenuId;
  int m_bookmarksMenuId;
};

#endif
