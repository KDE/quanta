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

//forward declarations
class QuantaView;
class KafkaDocument;

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
  QuantaView *createView();
  /** Removes a QuantaView object. Returns false on failure (eg. the view was not saved and it refused
  the delete itself.) If force is true, the view is removed without asking for save.*/
  bool removeView(QuantaView *view, bool force = false);
  /** Removes the active view Returns false on failure (eg. the view was not saved and it refused the delete itself.) */
  bool removeActiveView()  {
      return removeView(activeView());
   };
  /** Returns the active view */
  QuantaView *activeView();
  /** Returns the active document or 0L */
  Document *activeDocument();
  /** Returns the view holding the document loaded from url. */
  QuantaView *isOpened(const KURL &url);

  bool saveAll(bool dont_ask=true);

  /** Returns true if at least one view has the modified flag set. */
  bool isOneModified();

  void createNewDocument();
/** Returns a list with the URLs of the opened documents */
  KURL::List openedFiles(bool noUntitled=true);

public slots:
  void slotViewDeactivated(KMdiChildView *view);
  void slotViewActivated(KMdiChildView *view);

  /** closes all the other but active tabs */
  void slotCloseOtherTabs();
  /** closes all views. If createNew is true, it creates a new view after closing the others. */
  void closeAll(bool createNew = true);

signals:
  /** emitted when a file from the template view is dropped on a view */
  void dragInsert(QDropEvent *);


private:
 /** Private constructor for the singleton object. */
  ViewManager(QObject * parent = 0, const char * name = 0);
  QuantaView *m_lastActiveView;

};

#endif
