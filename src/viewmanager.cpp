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

//kde includes
#include <kdirwatch.h>
#include <klocale.h>
#include <kmdimainfrm.h>
#include <kurl.h>
#include <ktexteditor/encodinginterface.h>

//app includes
#ifdef BUILD_KAFKAPART
#include "undoredo.h"
#include "wkafkapart.h"
#endif

#include "document.h"
#include "quantaview.h"
#include "viewmanager.h"
#include "resource.h"
#include "quanta.h"
#include "templatestreeview.h"
#include "parser.h"

ViewManager::ViewManager(QObject *parent, const char *name) : QObject(parent, name)
{
   KafkaDocument *m_kafkaDocument = KafkaDocument::ref(0, 0, "KafkaPart");
   m_kafkaDocument->getKafkaWidget()->view()->setMinimumHeight(50);

    m_lastActiveView = 0L;
}

QuantaView* ViewManager::createView()
{
    QuantaView *view = new QuantaView(quantaApp);
    quantaApp->addWindow(view);
    connect(view, SIGNAL(cursorPositionChanged()), quantaApp, SLOT(slotNewLineColumn()));
    connect(view, SIGNAL(title(const QString &)), quantaApp, SLOT(slotNewLineColumn()));
    connect(view, SIGNAL(dragInsert(QDropEvent*)), this, SIGNAL(dragInsert(QDropEvent *)));
    return view;
}

void ViewManager::createNewDocument()
{
   QuantaView *view = createView();
   int i = 1;
   while ( isOpened(KURL("file:/"+i18n("Untitled%1").arg(i)))) i++;
  QString fname = i18n("Untitled%1").arg(i);

  KTextEditor::Document *doc =
  KTextEditor::createDocument ("libkatepart", quantaApp, "KTextEditor::Document");
/*                               KTextEditor::EditorChooser::createDocument(
                                quantaApp->view->writeTab(),
                                "KTextEditor::Document"
                                );*/

  Document *w = new Document(doc, quantaApp);
  w->readConfig(quantaApp->config());
  QString encoding = quantaApp->defaultEncoding();
  dynamic_cast<KTextEditor::EncodingInterface*>(doc)->setEncoding(encoding);

  KTextEditor::View * v = w->view();

  //[MB02] connect all kate views for drag and drop
  connect((QObject *)w->view(), SIGNAL(dropEventPass(QDropEvent *)), (QObject *) TemplatesTreeView::ref(), SLOT(slotDragInsert(QDropEvent *)));

  w->setUntitledUrl( fname );
  dynamic_cast<KTextEditor::PopupMenuInterface*>(w->view())->installPopup((QPopupMenu *)quantaApp->factory()->container("popup_editor", quantaApp));

  quantaApp->setFocusProxy(w->view());
  w->view()->setFocusPolicy(QWidget::WheelFocus);
  connect( v, SIGNAL(newStatus()), quantaApp, SLOT(slotNewStatus()));

  quantaApp->slotNewPart(doc, true);  // register new part in partmanager and make active
  view->addDocument(w);
}

bool ViewManager::removeView(QuantaView *view, bool force)
{
    if (view)
    {
       if (force || view->mayRemove())
      {
        quantaApp->closeWindow(view);
        if (!quantaApp->activeWindow())
        {
            createNewDocument();
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

void ViewManager::slotViewDeactivated(KMdiChildView *view)
{
/*
   QuantaView *qView = dynamic_cast<QuantaView*>(view);
   if (qView)
     qView->deactivated(); */
}

void ViewManager::slotViewActivated(KMdiChildView *view)
{
   if (m_lastActiveView)
     m_lastActiveView->deactivated();
   QuantaView *qView = dynamic_cast<QuantaView*>(view);
   if (qView)
     qView->activated();

  parser->clearGroups();
  kdDebug(24000) << "Calling reparse from update " << endl;
  parser->setSAParserEnabled(true);
  quantaApp->reparse(true); //FIXME
  quantaApp->slotNewStatus();
  quantaApp->slotNewLineColumn();
  typingInProgress = false; //need to reset, as it's set to true in the above slots
  m_lastActiveView = dynamic_cast<QuantaView *>(view);
}


#ifdef BUILD_KAFKAPART
#endif

void ViewManager::slotCloseOtherTabs()
{
  KMdiChildView *currentView = quantaApp->activeWindow();
  KMdiIterator<KMdiChildView*> *it = quantaApp->createIterator();
  KMdiChildView *view;
  for (it->first(); !it->isDone(); it->next())
  {
      view = it->currentItem();
      if (view != currentView)
      {
          if (dynamic_cast<QuantaView*>(view) && !static_cast<QuantaView*>(view)->mayRemove() )
              continue;
          quantaApp->closeWindow(view);
      }
  }
  delete it;
}

QuantaView* ViewManager::isOpened(const KURL& url)
{
  KMdiIterator<KMdiChildView*> *it = quantaApp->createIterator();
  QuantaView *view;
  for (it->first(); !it->isDone(); it->next())
  {
      view = dynamic_cast<QuantaView*>(it->currentItem());
      if (view && view->document() && view->document()->url() == url)
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
              if (!w->isUntitled() && w->url().isLocalFile())
                fileWatcher->removeFile(w->url().path());
              if (dont_ask && !w->isUntitled())
              {
#ifdef BUILD_KAFKAPART
                  w->docUndoRedo->fileSaved();
#endif
                  w->save();
                  w->closeTempFile();
                  w->createTempFile();
                  if (w->isModified())
                      flagsave = false;
              }
          } else
          {
              if (!view->saveModified())
                  flagsave = false;
          }
          if (w && w->url().isLocalFile())
              fileWatcher->addFile(w->url().path());
      }
  }
  delete it;

  return flagsave;
}

void ViewManager::closeAll()
{
  KMdiIterator<KMdiChildView*> *it = quantaApp->createIterator();
  QuantaView *view;
  for (it->first(); !it->isDone(); it->next())
  {
      view = dynamic_cast<QuantaView*>(it->currentItem());
      if (view)
      {
          Document *w = view->document();
          if ( w)
          {
              if (view->saveModified())
              {
                 w->closeTempFile();
                 if (!w->isUntitled() && w->url().isLocalFile())
                   fileWatcher->removeFile(w->url().path());
                 quantaApp->guiFactory()->removeClient(w->view());
                 quantaApp->closeWindow(view);
              } else
              {
                delete it;
                return;
               }
          }
      }
  }
  delete it;

  createNewDocument();
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

#include "viewmanager.moc"
