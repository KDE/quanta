/***************************************************************************
                          scripttreeview.cpp  -  description
                             -------------------
    begin                : Thu Sep 16 2003
    copyright            : (C) 2003 by Andras Mantia
    email                : amantia@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
//kde includes
#include <kiconloader.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kurl.h>

//qt includes
#include <qfileinfo.h>
#include <qpopupmenu.h>

//app includes
#include "filestreefile.h"
#include "filestreefolder.h"
#include "scripttreeview.h"
#include "../resource.h"
#include "../quanta.h"

ScriptTreeView::ScriptTreeView(const KURL& projectBaseURL, QWidget *parent, const char *name )
  : FilesTreeView(parent,name)
{
  m_baseURL = projectBaseURL;

  KURL url;
  url.setPath(qConfig.globalDataDir +"quanta/scripts/");
  m_globalDir = new FilesTreeFolder( this , i18n("Global scripts"), url);
  m_globalDir->setPixmap( 0, SmallIcon("folder"));
  m_globalDir->setOpen( true );

  url.setPath(locateLocal("data","quanta/scripts/"));
  m_localDir = new FilesTreeFolder( this , i18n("Local scripts"), url);
  m_localDir->setPixmap( 0, SmallIcon("folder"));
  m_localDir->setOpen( true );

  setRootIsDecorated( true );
  //header()->hide();
  setSorting( 0 );

  setFrameStyle( Panel | Sunken );
  setLineWidth( 2 );
  addColumn( i18n("Scripts"), 600 );

  setFocusPolicy(QWidget::ClickFocus);

  fileMenu = new QPopupMenu();
  fileMenu->insertItem(i18n("Run Script"), this ,SLOT(slotOpen()));
  fileMenu->insertSeparator();
  fileMenu->insertItem(i18n("Edit Script"), this ,SLOT(slotEditScript()));
  fileMenu->insertItem(i18n("Edit in Quanta"), this ,SLOT(slotEditInQuanta()));
  fileMenu->insertItem(i18n("Edit Description"), this ,SLOT(slotEditDescription()));
  fileMenu->insertSeparator();
  fileMenu->insertItem(i18n("Assign Action"), this ,SLOT(slotOpen()));
  fileMenu->insertItem(i18n("Send in Mail"), this ,SLOT(slotOpen()));

  connect(  this, SIGNAL(executed(QListViewItem *)),
            this, SLOT(slotSelectFile(QListViewItem *)));
  connect( this, SIGNAL(rightButtonPressed(QListViewItem*, const QPoint&, int)),
           this, SLOT(slotMenu(QListViewItem*, const QPoint&, int)));

}

ScriptTreeView::~ScriptTreeView()
{
}

void ScriptTreeView::slotMenu(QListViewItem *item, const QPoint &point, int)
{
  if (!item)
      return;
  setSelected(item, true);

  FilesTreeFile *f = dynamic_cast<FilesTreeFile *>( item);
  if (f)
  {
   fileMenu->popup( point);
  }
}

void ScriptTreeView::slotSelectFile(QListViewItem *item)
{
  FilesTreeFile *f = dynamic_cast<FilesTreeFile*>(currentItem());
  if (f)
  {
    KURL urlToOpen = currentURL();
    QString fileName = urlToOpen.fileName();
    fileName.truncate(fileName.length() - QFileInfo(fileName).extension().length() - 1);
    fileName.append(".info");
    urlToOpen.setFileName(fileName);
    emit activatePreview();
    emit openFile(urlToOpen, quantaApp->defaultEncoding());
  }
}

void ScriptTreeView::slotEditDescription()
{
  FilesTreeFile *f = dynamic_cast<FilesTreeFile*>(currentItem());
  if (f)
  {
    KURL urlToOpen = currentURL();
    QString fileName = urlToOpen.fileName();
    fileName.truncate(fileName.length() - QFileInfo(fileName).extension().length() - 1);
    fileName.append(".info");
    urlToOpen.setFileName(fileName);
    emit openFile(urlToOpen, quantaApp->defaultEncoding());
  }
}

void ScriptTreeView::slotEditScript()
{
  FilesTreeFile *f = dynamic_cast<FilesTreeFile*>(currentItem());
  if (f)
  {
    KURL urlToOpen = currentURL();
    emit openFile(urlToOpen, quantaApp->defaultEncoding());
  }
}

void ScriptTreeView::slotEditInQuanta()
{
  FilesTreeFile *f = dynamic_cast<FilesTreeFile*>(currentItem());
  if (f)
  {
    KURL urlToOpen = currentURL();
    emit openFile(urlToOpen, quantaApp->defaultEncoding());
  }
}


 #include "scripttreeview.moc"