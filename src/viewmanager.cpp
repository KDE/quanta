/***************************************************************************
                           viewmanager  -  implementation
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

//qt includes
#include <qdir.h>

//kde includes
#include <kdirwatch.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kmdimainfrm.h>
#include <kurl.h>
#include <kpopupmenu.h>
#include <ktexteditor/view.h>
#include <ktexteditor/encodinginterface.h>
#include <ktexteditor/highlightinginterface.h>
#include <ktexteditor/popupmenuinterface.h>
#include <ktexteditor/editorchooser.h>
#include <ktexteditor/markinterface.h>
#include <kio/netaccess.h>


//app includes
#include "undoredo.h"
#include "wkafkapart.h"
#include "tagactionmanager.h"
#include "tagactionset.h"

#include "whtmlpart.h"
#include "document.h"
#include "quantaview.h"
#include "viewmanager.h"
#include "project.h"
#include "resource.h"
#include "quanta.h"
#include "quantabookmarks.h"
#include "toolbartabwidget.h"
#include "parser.h"
#include "qextfileinfo.h"
#include "qpevents.h"
#include "ksavealldialog.h"
#include "annotationoutput.h"

#ifdef ENABLE_CVSSERVICE
#include "cvsservice.h"
#endif

#define SEPARATOR_INDEX 3
#define RELOAD_ID 11
#define UPLOAD_ID 12
#define DELETE_ID 13

ViewManager::ViewManager(QObject *parent, const char *name) : QObject(parent, name)
{
    m_lastActiveView = 0L;
    m_lastActiveEditorView = 0L;
    m_documentationView = 0L;
    m_tabPopup = new KPopupMenu(quantaApp);
    m_tabPopup->insertItem(SmallIcon("fileclose"), i18n("&Close"), this, SLOT(slotCloseView()));
    m_tabPopup->insertItem(i18n("Close &Other Tabs"), this, SLOT(slotCloseOtherTabs()));
    m_tabPopup->insertItem(i18n("Close &All"), this, SLOT(closeAll()));
    m_tabPopup->insertItem(SmallIcon("revert"), i18n("&Reload"), this, SLOT(slotReloadFile()), 0, RELOAD_ID);
    m_tabPopup->insertItem(SmallIcon("up"), i18n("&Upload File"), this, SLOT(slotUploadFile()), 0, UPLOAD_ID);
    m_tabPopup->insertItem(SmallIcon("editdelete"), i18n("&Delete File"), this, SLOT(slotDeleteFile()), 0, DELETE_ID);
    m_tabPopup->insertSeparator();
    m_fileListPopup = new KPopupMenu(quantaApp);
    connect(m_fileListPopup, SIGNAL(aboutToShow()), this, SLOT(slotFileListPopupAboutToShow()));
    connect(m_fileListPopup, SIGNAL(activated(int)), this, SLOT(slotFileListPopupItemActivated(int)));
    m_bookmarks = new QuantaBookmarks(this, QuantaBookmarks::Position, true);
    m_bookmarksMenu = new KPopupMenu(quantaApp);
    m_bookmarks->setBookmarksMenu(m_bookmarksMenu);
    connect(m_bookmarks, SIGNAL(gotoFileAndLine(const QString&, int, int)), quantaApp, SLOT(gotoFileAndLine(const QString&, int, int)));
    m_bookmarksMenuId = m_tabPopup->insertItem(SmallIconSet("bookmark"), i18n("&Bookmarks"), m_bookmarksMenu);
    m_tabPopup->insertItem(i18n("&Switch To"), m_fileListPopup);
    m_contextView = 0L;
    m_cvsMenuId = -1;
    m_separatorVisible = false;
}

QuantaView* ViewManager::createView(const QString &caption)
{
    QuantaView *view = new QuantaView(quantaApp, "", caption);
    quantaApp->addWindow(view);
    connect(view, SIGNAL(cursorPositionChanged()), quantaApp, SLOT(slotNewLineColumn()));
    connect(view, SIGNAL(title(const QString &)), quantaApp, SLOT(slotNewLineColumn()));
    connect(view, SIGNAL(dragInsert(QDropEvent*)), this, SIGNAL(dragInsert(QDropEvent *)));
    connect(view, SIGNAL(hidePreview()), quantaApp, SLOT(slotChangePreviewStatus()));
    disconnect(view, SIGNAL(childWindowCloseRequest( KMdiChildView *)), 0, 0 );
    connect(view, SIGNAL(childWindowCloseRequest( KMdiChildView*)), this, SLOT(slotCloseRequest(KMdiChildView*)));
    connect(view, SIGNAL(documentClosed(const KURL&)), this, SLOT(slotDocumentClosed(const KURL&)));
    connect(view, SIGNAL(eventHappened(const QString&, const QString&, const QString& )), QPEvents::ref(), SLOT(slotEventHappened(const QString&, const QString&, const QString& )));

    return view;
}

void ViewManager::createNewDocument()
{
  int i = 1;
  while (isOpened(KURL("file:"+i18n("Untitled%1").arg(i)))) i++;
  QString fname = i18n("Untitled%1").arg(i);
  QuantaView *view = createView(fname);

#ifdef ENABLE_EDITORS
  KTextEditor::Document *doc =
                              KTextEditor::EditorChooser::createDocument(
                                view,
                                "KTextEditor::Document"
                                );
#else
  KTextEditor::Document *doc = KTextEditor::createDocument ("libkatepart", view, "KTextEditor::Document");
#endif
  Document *w = new Document(doc, 0L);
  connect(w, SIGNAL(showAnnotation(uint, const QString&, const QPair<QString, QString>&)), quantaApp->annotationOutput(), SLOT(insertAnnotation(uint, const QString&, const QPair<QString, QString>&)));
  QString encoding = quantaApp->defaultEncoding();
  KTextEditor::EncodingInterface* encodingIf = dynamic_cast<KTextEditor::EncodingInterface*>(doc);
  if (encodingIf)
      encodingIf->setEncoding(encoding);

  KTextEditor::View * v = w->view();

  //[MB02] connect all kate views for drag and drop
  connect(w->view(), SIGNAL(dropEventPass(QDropEvent *)), this, SIGNAL(dragInsert(QDropEvent *)));

  w->setUntitledUrl( fname );
  KTextEditor::PopupMenuInterface* popupIf = dynamic_cast<KTextEditor::PopupMenuInterface*>(w->view());
  if (popupIf)
     popupIf->installPopup((QPopupMenu *)quantaApp->factory()->container("popup_editor", quantaApp));

  quantaApp->setFocusProxy(w->view());
  w->view()->setFocusPolicy(QWidget::WheelFocus);
  connect( v, SIGNAL(newStatus()), quantaApp, SLOT(slotNewStatus()));

  quantaApp->slotNewPart(doc, true);  // register new part in partmanager and make active
  view->addDocument(w);
  view->activate(); //if we don't call this manually, the activeView() won't return the newly created view
  view->activated(); //the previous activate does not call this, because it detects that the view was not changed (createView() also calls activate())
  m_lastActiveView = view;
  m_lastActiveEditorView = view;
  quantaApp->newCursorPosition("", 1 , 1);
}

bool ViewManager::removeView(QuantaView *view, bool force, bool createNew)
{
    if (!view) return false;
    int noOfViews = 0;
    QValueList<Document*> list;
    KMdiIterator<KMdiChildView*> *it = quantaApp->createIterator();
    for (it->first(); !it->isDone(); it->next())
    {
      noOfViews++;
      if (noOfViews > 1)
        break;
    }
    delete it;
    if (noOfViews == 1 && view->document() && view->document()->isUntitled() && !view->document()->isModified() && createNew)
    {
      quantaApp->slotShowPreviewWidget(false);
      return true;
    }
    bool mayRemove = view->mayRemove();
    if (mayRemove)
    {
      if (force || mayRemove)
      {
        if (view == m_documentationView)
            m_documentationView = 0L;
        if (view == m_lastActiveView)
            m_lastActiveView = 0L;
        if (view == m_lastActiveEditorView)
            m_lastActiveEditorView = 0L;
        if (view == activeView())
            ToolbarTabWidget::ref()->reparent(0L, 0, QPoint(), false);
        if (!createNew)
          disconnect(quantaApp, SIGNAL(lastChildViewClosed()), this, SLOT(slotLastViewClosed()));
        quantaApp->closeWindow(view);
        if (createNew)
        {
          if (allEditorsClosed())
          {
            quantaApp->slotFileNew();
          }
        } else
          connect(quantaApp, SIGNAL(lastChildViewClosed()), this, SLOT(slotLastViewClosed()));
        return true;
       }
    }
    return false;
}

Document *ViewManager::activeDocument()
{
  QuantaView *view = activeView();
  return view ? view->document() : 0L;
}

QuantaView* ViewManager::activeView()
{
    return static_cast<QuantaView *>(quantaApp->activeWindow());
}

void ViewManager::slotViewActivated(KMdiChildView *view)
{
   if (!quantaApp || m_lastActiveView == view)
     return;
   if (m_lastActiveView)
   {
     m_lastActiveView->deactivated();
   }
   QuantaView *qView = static_cast<QuantaView*>(view);
   qView->activated();

#ifdef DEBUG_PARSER
//   kdDebug(24000) << "Calling clearGroups from ViewManager::slotViewActivated" << endl;
#endif
  parser->clearGroups();
  parser->setSAParserEnabled(true);
  quantaApp->reparse(true); //FIXME
  quantaApp->slotNewStatus();
  quantaApp->slotNewLineColumn();
  typingInProgress = false; //need to reset, as it's set to true in the above slots
  m_lastActiveView = qView;
  if (m_lastActiveView->document())
  {
    m_lastActiveEditorView = m_lastActiveView;
  }

  Document *w = qView->document();
  if (w && !w->isUntitled())
  {
    emit viewActivated(w->url());

    bool flag = TagActionManager::canIndentDTD(w->defaultDTD()->name);
    quantaApp->actionCollection()->action("apply_source_indentation")->setEnabled(flag);
  }
}

void ViewManager::slotCloseOtherTabs()
{
  KMdiChildView *currentView;
  KMdiChildView *view;
  QuantaView *qView;
  if (m_contextView)
     currentView = m_contextView;
   else
     currentView = quantaApp->activeWindow();
   if (dynamic_cast<QuantaView*>(currentView) && !static_cast<QuantaView*>(currentView)->document())
     ToolbarTabWidget::ref()->reparent(0, 0, QPoint(), false);
  KMdiIterator<KMdiChildView*> *it = quantaApp->createIterator();
  //save the children first to a list, as removing invalidates our iterator
  QValueList<KMdiChildView *> children;
  for (it->first(); !it->isDone(); it->next())
  {
      children.append(it->currentItem());
  }
  delete it;
  KURL::List modifiedList;
  QValueListIterator<KMdiChildView *> childIt;
  for (childIt = children.begin(); childIt != children.end(); ++childIt)
  {
    view = *childIt;
    qView = dynamic_cast<QuantaView*>(view);
    if (qView && view != currentView)
    {
      Document *w = qView->document();
      if (w && w->isModified())
      {
        modifiedList += w->url();
      }
    }
  }
  if (!modifiedList.isEmpty())
  {
    KURL::List filesToSave;
    KSaveSelectDialog dlg(modifiedList, filesToSave /*empty ignore list */, quantaApp);
    if (dlg.exec() == QDialog::Accepted)
    {
      filesToSave = dlg.filesToSave();
      for (childIt = children.begin(); childIt != children.end(); ++childIt)
      {
        view = *childIt;
        qView = dynamic_cast<QuantaView*>(view);
        if (view && view != currentView)
        {
          Document *w = qView->document();
          if (w)
          {
            if (filesToSave.contains(w->url()))
              if (!qView->saveModified(false))
            {
              return;  //save aborted
            }
            w->setModified(false);
            qView->updateTab();
          }
        }
      }
    } else
    {
      return;  //save aborted
    }
  }

  for (childIt = children.begin(); childIt != children.end(); ++childIt)
  {
      view = *childIt;
      if (view != currentView)
      {
          if (dynamic_cast<QuantaView*>(view) && !static_cast<QuantaView*>(view)->mayRemove() )
              continue;
          quantaApp->closeWindow(view);
      }
  }
}

QuantaView* ViewManager::isOpened(const KURL& url)
{
  KURL url2 = url;
  if (url2.isLocalFile() && !url.path().startsWith(i18n("Untitled")))
  {
      QDir dir(url2.path());
      url2.setPath(dir.canonicalPath());
  }
  KMdiIterator<KMdiChildView*> *it = quantaApp->createIterator();
  QuantaView *view;
  for (it->first(); !it->isDone(); it->next())
  {
      view = dynamic_cast<QuantaView*>(it->currentItem());
      if (view && view->document() && view->document()->url() == url2)
      {
          delete it;
          return view;
      }
  }
  delete it;
  return 0L;
}

KURL::List ViewManager::openedFiles(bool noUntitled)
{
  KURL::List list;

  KMdiIterator<KMdiChildView*> *it = quantaApp->createIterator();
  QuantaView *view;
  for (it->first(); !it->isDone(); it->next())
  {
      view = dynamic_cast<QuantaView*>(it->currentItem());
      if (view)
      {
          Document *w = view->document();
          if ( w && (!w->isUntitled() || !noUntitled) )
            list.append( w->url() );
      }
  }
  delete it;
  return list;
}

QValueList<Document*> ViewManager::openedDocuments()
{
  QValueList<Document*> list;
  KMdiIterator<KMdiChildView*> *it = quantaApp->createIterator();
  QuantaView *view;
  for (it->first(); !it->isDone(); it->next())
  {
    view = dynamic_cast<QuantaView*>(it->currentItem());
    if (view)
    {
      Document *w = view->document();
      if (w)
        list.append(w);
    }
  }
  delete it;
  return list;
}

bool ViewManager::saveAll()
{
  bool flagsave = true;
  KMdiIterator<KMdiChildView*> *it = quantaApp->createIterator();
  QuantaView *view;
  for (it->first(); !it->isDone(); it->next())
  {
      view = dynamic_cast<QuantaView*>(it->currentItem());
      if (view)
      {
          Document *w = view->document();
          if ( w && w->isModified())
          {
              if (!w->isUntitled())
              {
                  emit eventHappened("before_save", w->url().url(), QString::null);
                  w->docUndoRedo->fileSaved();
                  w->save();
                  w->removeBackup(quantaApp->config());
                  if (w->isModified())
                      flagsave = false;
                  emit eventHappened("after_save", w->url().url(), QString::null);
              } else
              {
                  if (!view->saveModified())
                    flagsave = false;
              }
          }
          view->updateTab();
      }
  }
  delete it;

  return flagsave;
}

bool ViewManager::closeAll(bool createNew)
{
  quantaApp->slotShowPreviewWidget(false);
  parser->setSAParserEnabled(false);
  KMdiIterator<KMdiChildView*> *it = quantaApp->createIterator();
  QuantaView *view;
  //save the children first to a list, as removing invalidates our iterator
  QValueList<KMdiChildView *> children;
  for (it->first(); !it->isDone(); it->next())
  {
      children.append(it->currentItem());
  }
  delete it;
  KURL::List modifiedList;
  QValueListIterator<KMdiChildView *> childIt;
  for (childIt = children.begin(); childIt != children.end(); ++childIt)
  {
    view = dynamic_cast<QuantaView*>(*childIt);
    if (view)
    {
      Document *w = view->document();
      if (w && w->isModified())
      {
        modifiedList += w->url();
      }
    }
  }
  if (!modifiedList.isEmpty())
  {
    KURL::List filesToSave;
    KSaveSelectDialog dlg(modifiedList, filesToSave /*empty ignore list */, quantaApp);
    if (dlg.exec() == QDialog::Accepted)
    {
      filesToSave = dlg.filesToSave();
      for (childIt = children.begin(); childIt != children.end(); ++childIt)
      {
        view = dynamic_cast<QuantaView*>(*childIt);
        if (view)
        {
          Document *w = view->document();
          if (w)
          {
            if (filesToSave.contains(w->url()))
              if (!view->saveModified(false))
              {
                emit filesClosed(false);
                return false;  //save aborted
              }
            w->setModified(false);
            view->updateTab();
          }
        }
      }
    } else
    {
      emit filesClosed(false);
      return false;  //save aborted
    }
  }
  disconnect(quantaApp, SIGNAL(viewActivated (KMdiChildView *)), this, SLOT(slotViewActivated(KMdiChildView*)));
  disconnect(quantaApp, SIGNAL(lastChildViewClosed()), this, SLOT(slotLastViewClosed()));
  ToolbarTabWidget::ref()->reparent(0L, 0, QPoint(), false);

  for (childIt = children.begin(); childIt != children.end(); ++childIt)
  {
      view = dynamic_cast<QuantaView*>(*childIt);
      if (view)
      {
          Document *w = view->document();
          if (w)
          {
              if (view->mayRemove())
              {
                 if (!w->isUntitled() && w->url().isLocalFile())
                 {
                   fileWatcher->removeFile(w->url().path());
//                   kdDebug(24000) << "removeFile[closeAll]: " << w->url().path() << endl;
                 }
                 quantaApp->guiFactory()->removeClient(w->view());
                 if (view == m_lastActiveEditorView)
                   m_lastActiveEditorView = 0L;
                 if (view == m_lastActiveView)
                   m_lastActiveView = 0L;
                 quantaApp->closeWindow(view);
              } else
              {
                //actually this code should be never executed
               connect(quantaApp, SIGNAL(viewActivated (KMdiChildView *)), this, SLOT(slotViewActivated(KMdiChildView*)));
               connect(quantaApp, SIGNAL(lastChildViewClosed()), this, SLOT(slotLastViewClosed()));
               view->activated();
               emit filesClosed(false);
                return false;
              }
          } else
          {
              if (view == m_documentationView)
                  m_documentationView = 0L;
              view->mayRemove(); //unloads the plugin and reparents the custom widget
              quantaApp->closeWindow(view);
          }
      }
  }
  connect(quantaApp, SIGNAL(viewActivated (KMdiChildView *)), this, SLOT(slotViewActivated(KMdiChildView*)));
  connect(quantaApp, SIGNAL(lastChildViewClosed()), this, SLOT(slotLastViewClosed()));
  if (createNew)
  {
      createNewDocument();
      quantaApp->slotNewStatus();
  }
  emit filesClosed(true);
  return true;
}

bool ViewManager::isOneModified()
{
 KMdiIterator<KMdiChildView*> *it = quantaApp->createIterator();
  QuantaView *view;
  for (it->first(); !it->isDone(); it->next())
  {
      view = dynamic_cast<QuantaView*>(it->currentItem());
      if (view)
      {
          Document *w = view->document();
           if (w && w->isModified())
           {
              delete it;
              return true;
           }
      }
  }
  delete it;

  return false;
}

QuantaView* ViewManager::documentationView(bool create)
{
    if (!m_documentationView && create)
    {
        m_documentationView = createView();
        m_documentationView->addCustomWidget((QWidget*)quantaApp->documentationPart()->view(), i18n("Documentation"));
    }
    return m_documentationView;
}

void ViewManager::slotLastViewClosed()
{
  quantaApp->slotFileNew();
}

bool ViewManager::allEditorsClosed()
{
  KMdiIterator<KMdiChildView*> *it = quantaApp->createIterator();
  QuantaView *view;
  for (it->first(); !it->isDone(); it->next())
  {
      view = dynamic_cast<QuantaView*>(it->currentItem());
      if (view && view->document())
      {
         delete it;
         return false;
      }
  }
  delete it;
  return true;
}

void ViewManager::slotTabContextMenu(QWidget *widget, const QPoint& point)
{
   if (m_separatorVisible)
       m_tabPopup->removeItemAt(SEPARATOR_INDEX);
   m_separatorVisible = false;
   m_contextView = dynamic_cast<QuantaView*>(widget);
   Document *w = 0L;
   if (m_contextView)
    w = m_contextView->document();
   if (w)
   {
       if (!w->isUntitled())
       {
         m_separatorVisible = true;
         m_tabPopup->insertSeparator(SEPARATOR_INDEX);
       }
       if (Project::ref()->hasProject() && Project::ref()->contains(w->url()))
         m_tabPopup->setItemVisible(UPLOAD_ID, true);
       else
         m_tabPopup->setItemVisible(UPLOAD_ID, false);
       if (w->isUntitled())
       {
         m_tabPopup->setItemVisible(RELOAD_ID, false);
         m_tabPopup->setItemVisible(DELETE_ID, false);
       } else
       {
         m_tabPopup->setItemVisible(RELOAD_ID, true);
         m_tabPopup->setItemVisible(DELETE_ID, true);
       }
   } else
   {
       m_tabPopup->setItemVisible(RELOAD_ID, false);
       m_tabPopup->setItemVisible(UPLOAD_ID, false);
       m_tabPopup->setItemVisible(DELETE_ID, false);
   }
   bool bookmarksFound = false;
   if (w && w->markIf)
   {
     m_bookmarks->setDocument(w);
     QPtrList<KTextEditor::Mark> m = w->markIf->marks();
     QPtrListIterator<KTextEditor::Mark> it(m);
     for(; *it; ++it)
     {
       if ((*it)->type & KTextEditor::MarkInterface::markType01)
       {
         bookmarksFound = true;
         break;
       }
     }
   }
   m_tabPopup->setItemVisible(m_bookmarksMenuId, bookmarksFound);
#ifdef ENABLE_CVSSERVICE
   if (w && w->url().isLocalFile() && !w->isUntitled() && CVSService::ref()->exists())
   {
     if (m_cvsMenuId == -1)
     {
       m_tabPopup->insertSeparator();
       m_cvsMenuId = m_tabPopup->insertItem(SmallIcon("cervisia"), i18n("CVS"), CVSService::ref()->menu());
     }
     if (Project::ref()->contains(w->url()))
       CVSService::ref()->setRepository(Project::ref()->projectBaseURL().path());
     else
         CVSService::ref()->setRepository(w->url().directory());
     CVSService::ref()->setCurrentFile(w->url().path());
   } else
   if (m_cvsMenuId != -1)
   {
       int idx = m_tabPopup->indexOf(m_cvsMenuId);
       m_tabPopup->removeItemAt(idx-1);
       m_tabPopup->removeItem(m_cvsMenuId);
       m_cvsMenuId = -1;
   }
#endif
   m_tabPopup->exec(point);
}

void ViewManager::slotFileListPopupAboutToShow()
{
   m_fileListPopup->clear();
   QStringList viewList;
   KMdiIterator<KMdiChildView*> *it = quantaApp->createIterator();
   QuantaView *view;
   int id = 0;
   for (it->first(); !it->isDone(); it->next())
   {
      view = dynamic_cast<QuantaView*>(it->currentItem());
      if (view)
      {
          m_fileListPopup->insertItem(view->tabName(), id);
          id++;
      }
  }
  delete it;
}

void ViewManager::slotFileListPopupItemActivated(int id)
{
   KMdiIterator<KMdiChildView*> *it = quantaApp->createIterator();
   QuantaView *view;
   int id2 = 0;
   for (it->first(); !it->isDone(); it->next())
   {
      view = dynamic_cast<QuantaView*>(it->currentItem());
      if (view && id == id2)
      {
          view->activate();
          view->activated();
          break;
      }
      id2++;
  }
  delete it;
}

void ViewManager::slotReloadFile()
{
    quantaApp->slotFileReload(m_contextView);
}

void ViewManager::slotUploadFile()
{
    quantaApp->slotUploadFile(m_contextView);
}

void ViewManager::slotDeleteFile()
{
    quantaApp->slotDeleteFile(m_contextView);
}

void ViewManager::slotCloseView()
{
   removeView(m_contextView);
}

void ViewManager::slotCloseRequest(KMdiChildView *widget)
{
   QuantaView *view = dynamic_cast<QuantaView *>(widget);
   if (view)
      removeView(view);
}


void ViewManager::slotDocumentClosed(const KURL& url)
{
  emit documentClosed(url);
}

/** Return the URL of the currently active document */
QString ViewManager::currentURL()
{
  Document *w = activeDocument();
  if (w)
  {
    return w->url().url();
  } else
  {
    QuantaView * view = lastActiveEditorView();
    if (view)
      return view->document()->url().url();
    else
      return "";
  }
}


#include "viewmanager.moc"
