/***************************************************************************
                          projectnewlocal.cpp  -  description
                             -------------------
    begin                : Fri Oct 27 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com>
                           (C) 2001, 2002 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//app includes
#include "localimportwizardpage.h"
#include "extfileinfo.h"
#include "simplefiletree.h"
#include "simpletreeviewitem.h"



// qt includes
#include <qlabel.h>
#include <qfileinfo.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qradiobutton.h>
#include <qpushbutton.h>
#include <qregexp.h>

// kde includes
#include <kdebug.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kinstance.h>
#include <kfiledialog.h>
#include <k3listview.h>
#include <kprogressbar.h>
#include <kmimetype.h>
#include <kdeversion.h>
#include <kurlrequesterdlg.h>
#include <kurlrequester.h>


LocalImportWizardPage::LocalImportWizardPage(KInstance *instance, QWidget *parent)
  : QWidget(parent)
{
  setupUi(this);
  connect(insertExisting, SIGNAL(clicked()), SLOT(slotInsertFiles()));
  connect(addFiles, SIGNAL(clicked()), SLOT(slotAddFiles()));
  connect(addFolder, SIGNAL(clicked()), SLOT(slotAddFolder()));
  connect(clearList, SIGNAL(clicked()), SLOT(slotClearList()));
  imagelabel->setPixmap(UserIcon("thirdwizardpage", instance));

  filterMask->setText("*");
  checkInsertWeb->setChecked( true );
}

LocalImportWizardPage::~LocalImportWizardPage(){
}

void LocalImportWizardPage::setBaseURL(const KUrl& a_baseURL)
{
  baseURL = a_baseURL;
  baseURL.adjustPath(1);
  slotClearList();
}

QList<QPair<KUrl, KUrl> > LocalImportWizardPage::files()
{
  QList<QPair<KUrl, KUrl> > list;
  Q3ListViewItemIterator it(listView);
  KUrl destURL;
  QString path;
  while (it.current()) 
  {
    SimpleTreeViewItem *item = static_cast<SimpleTreeViewItem*>(it.current());
    destURL = baseURL;
    destURL.setPath(destURL.directory(false, true) + item->treePath());
    destURL.adjustPath(-1);
    list.append(qMakePair(item->url(), destURL));
    ++it;
  }
  return list;
}

KUrl::List LocalImportWizardPage::projectFiles()
{
  KUrl::List list;

  if (!ExtFileInfo::exists(baseURL)) 
    return list;

  QString ffilterMask = "*";
  if (checkInsertWeb->isChecked())
  {
    KMimeType::List list = KMimeType::allMimeTypes();
    KMimeType::List::ConstIterator it;
    KMimeType::List::ConstIterator end = list.constEnd();
    QString name;
    ffilterMask = "";
    for ( it = list.constBegin(); it != end; ++it )
    {
      name = (*it)->name();
      if (name.startsWith("image") || name.startsWith("text") || 
          (*it)->is("text/plain"))
      {
        QStringList patterns = (*it)->patterns();
        for (int i = 0 ; i < patterns.count(); i++)
        {
          ffilterMask = ffilterMask + patterns[i] + ";";
        }
      }
    }
  } else
  {
    ffilterMask = filterMask->text();
  }
  ffilterMask.replace(';', ' '); //allFiles expects a space separated filterMask
  list = ExtFileInfo::allFiles(baseURL, ffilterMask);

  return list;
}

void LocalImportWizardPage::slotInsertFiles()
{
  KUrl::List list = projectFiles();
  insertURLs(list.first().directory(false, true), list);
}

void LocalImportWizardPage::resizeEvent ( QResizeEvent *t )
{
  QWidget::resizeEvent(t);
  listView->setColumnWidth(0,listView->width()-listView->columnWidth(1)-20);
}

void LocalImportWizardPage::slotAddFiles()
{
  KUrl::List list = KFileDialog::getOpenURLs(
    baseURL.url(),  i18n("*"), this, i18n("Insert Files in Project"));

  if (!list.isEmpty())
  {
    insertURLs(list.first().directory(false, true), list);
  }
}

void LocalImportWizardPage::slotAddFolder()
{
  KUrl dirURL ;
  dirURL = KFileDialog::getExistingURL(
           baseURL.url(),  this, i18n("Insert Folder in Project"));
  if ( !dirURL.isEmpty() )
  {
  //  listView->setBase(dirURL.directory(false, true));
    dirURL.adjustPath(1);
    KUrl::List list = ExtFileInfo::allFiles(dirURL, "*");
    insertURLs(dirURL.directory(false, true), list);
  }
}

void LocalImportWizardPage::insertURLs(const QString &sourceBase, const KUrl::List &urls)
{
  SimpleTreeViewItem *item = static_cast<SimpleTreeViewItem*>(listView->currentItem());
  QString prefix = baseURL.fileName();
  if (item)
  {
    prefix = item->treePath();
  }
  KUrl::List::ConstIterator end = urls.constEnd();  
  for (KUrl::List::ConstIterator it = urls.constBegin(); it != end; ++it)
  {
    KUrl u = *it;
    listView->createItem(prefix + u.path().remove(sourceBase), u); 
  }
}

void LocalImportWizardPage::slotClearList()
{
  listView->clear();
  SimpleTreeViewItem *item = listView->createItem(baseURL.fileName(), baseURL);
  if (item)
    item->setURL(baseURL);
  listView->setCurrentItem(item); //creates the base item in the treeview
}

#include "localimportwizardpage.moc"
