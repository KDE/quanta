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


//app includes
#include "undoredo.h"
#include "wkafkapart.h"

#include "whtmlpart.h"
#include "document.h"
#include "quantaview.h"
#include "viewmanager.h"
#include "resource.h"
#include "quanta.h"
#include "toolbartabwidget.h"
#include "parser.h"
#include "qextfileinfo.h"
#include "qpevents.h"

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
    m_tabPopup->insertItem(SmallIcon("up"), i18n("&Upload File..."), this, SLOT(slotUploadFile()), 0, UPLOAD_ID);
    m_tabPopup->insertItem(SmallIcon("editdelete"), i18n("&Delete File"), this, SLOT(slotDeleteFile()), 0, DELETE_ID);
    m_tabPopup->insertSeparator();
    m_fileListPopup = new KPopupMenu(quantaApp);
    connect(m_fileListPopup, SIGNAL(aboutToShow()), this, SLOT(slotFileListPopupAboutToShow()));
    connect(m_fileListPopup, SIGNAL(activated(int)), this, SLOT(slotFileListPopupItemActivated(int)));
    m_tabPopup->insertItem(i18n("&Switch To"), m_fileListPopup);
    m_contextView = 0L;
    m_separatorVisible = false;
}

QuantaView* ViewManager::createView()
{
    QuantaView *view = new QuantaView(quantaApp);
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
  QuantaView *view = createView();
  int i = 1;
  while (isOpened(KURL("file:"+i18n("Untitled%1").arg(i)))) i++;
  QString fname = i18n("Untitled%1").arg(i);

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

bool ViewManager::removeView(QuantaView *view, bool force)
{
    if (!view) return false;
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
        quantaApp->closeWindow(view);
        if (allEditorsClosed())
        {
            createNewDocument();
            quantaApp->slotNewStatus();
        }
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
    return dynamic_cast<QuantaView *>(quantaApp->activeWindow());
}

void ViewManager::slotViewActivated(KMdiChildView *view)
{
   if (!quantaApp || m_lastActiveView == view)
     return;
   if (m_lastActiveView)
   {
     m_lastActiveView->deactivated();
     Document *w = static_cast<QuantaView*>(m_lastActiveView)->document();
      quantaApp->restoreFromTempfile(w);
   }
   QuantaView *qView = static_cast<QuantaView*>(view);
   qView->activated();

#ifdef DEBUG_PARSER
  kdDebug(24000) << "Calling clearGroups from ViewManager::slotViewActivated" << endl;
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
  }
}

void ViewManager::slotCloseOtherTabs()
{
  KMdiChildView *currentView;
   if (m_contextView)
     currentView = m_contextView;
   else
     currentView = quantaApp->activeWindow();
  KMdiIterator<KMdiChildView*> *it = quantaApp->createIterator();
  //save the children first to a list, as removing invalidates our iterator
  QValueList<KMdiChildView *> children;
  for (it->first(); !it->isDone(); it->next())
  {
      children.append(it->currentItem());
  }
  delete it;
  KMdiChildView *view;
  QValueListIterator<KMdiChildView *> childIt;
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
  if (url2.isLocalFile())
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

bool ViewManager::saveAll(bool dont_ask)
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
              if (dont_ask && !w->isUntitled())
              {
                  emit eventHappened("before_save", w->url().url(), QString::null);
                  w->docUndoRedo->fileSaved();
                  w->save();
                  w->closeTempFile();
                  w->createTempFile();
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

void ViewManager::closeAll(bool createNew)
{
  quantaApp->slotShowPreviewWidget(false);
  disconnect(quantaApp, SIGNAL(viewActivated (KMdiChildView *)), this, SLOT(slotViewActivated(KMdiChildView*)));
  disconnect(quantaApp, SIGNAL(lastChildViewClosed()), this, SLOT(slotLastViewClosed()));

  KMdiIterator<KMdiChildView*> *it = quantaApp->createIterator();
  QuantaView *view;
  ToolbarTabWidget::ref()->reparent(0L, 0, QPoint(), false);
  //save the children first to a list, as removing invalidates our iterator
  QValueList<KMdiChildView *> children;
  for (it->first(); !it->isDone(); it->next())
  {
      children.append(it->currentItem());
  }
  delete it;
  QValueListIterator<KMdiChildView *> childIt;
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
                 w->closeTempFile();
                 if (!w->isUntitled() && w->url().isLocalFile())
                 {
                   fileWatcher->removeFile(w->url().path());
                   kdDebug(24000) << "removeFile[closeAll]: " << w->url().path() << endl;
                 }
                 quantaApp->guiFactory()->removeClient(w->view());
                 if (view == m_lastActiveEditorView)
                   m_lastActiveEditorView = 0L;
                 if (view == m_lastActiveView)
                   m_lastActiveView = 0L;
                 quantaApp->closeWindow(view);
              } else
              {
               connect(quantaApp, SIGNAL(viewActivated (KMdiChildView *)), this, SLOT(slotViewActivated(KMdiChildView*)));
               connect(quantaApp, SIGNAL(lastChildViewClosed()), this, SLOT(slotLastViewClosed()));
               view->activated();
                return;
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
    createNewDocument();
    quantaApp->slotNewStatus();
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
   m_contextView = dynamic_cast<QuantaView*>(widget);
   if (m_contextView && m_contextView->document())
   {
       m_tabPopup->insertSeparator(SEPARATOR_INDEX);
       m_tabPopup->setItemVisible(RELOAD_ID, true);
       m_tabPopup->setItemVisible(UPLOAD_ID, true);
       m_tabPopup->setItemVisible(DELETE_ID, true);
       m_separatorVisible = true;
   } else
   {
       m_tabPopup->setItemVisible(RELOAD_ID, false);
       m_tabPopup->setItemVisible(UPLOAD_ID, false);
       m_tabPopup->setItemVisible(DELETE_ID, false);
       m_separatorVisible = false;
   }
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

#include "viewmanager.moc"
