/***************************************************************************
                          projectnewweb.cpp  -  description
                             -------------------
    begin                : Fri Oct 27 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com>
                           (C) 2002, 2003, 2005 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
// app includes
#include "webimportwizardpage.h"
#include "quantanetaccess.h"
#include "simplefiletree.h"
#include "simpletreeviewitem.h"

// qt includes
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStringList>
#include <QListView>
#include <QFileInfo>
#include <QComboBox>

// kde includes
#include <klocale.h>
#include <kprocess.h>
#include <kprogressbar.h>
#include <kiconloader.h>
#include <kinstance.h>
#include <kio/job.h>
#include <kmessagebox.h>


WebImportWizardPage::WebImportWizardPage(KInstance *instance, QWidget *parent)
  : QWidget(parent)
{
  setupUi(this);
  button->setEnabled(false);
  siteUrl->setFocus();

  imagelabel->setPixmap(UserIcon("thirdwizardpage", instance));

  connect( commandLine, SIGNAL(textChanged(const QString&)),
           this,        SLOT  (enableStart(const QString&)));
  connect( siteUrl,     SIGNAL(textChanged(const QString&)),
           this,        SLOT  (setCommandL(const QString&)));
  connect( button,      SIGNAL(clicked()),
           this,        SLOT  (slotStart()));
  connect( protocolCombo,SIGNAL(highlighted(const QString&)),
           this,         SLOT  (setProtocol(const QString&)));

  start = false;
  progress->setRange(0, 0);
  progress->setTextVisible(false);
}

WebImportWizardPage::~WebImportWizardPage(){
}

void WebImportWizardPage::enableStart(const QString&)
{
  if (!commandLine->text().isEmpty())
    button->setEnabled(true);
  else
    button->setEnabled(false);
}

void WebImportWizardPage::setCommandL(const QString& url)
{
  QString siteurl = url;

  commandLine->setText("wget -c -np -r --level=5 -nH " + protocolCombo->currentText().toLower() + "://"+siteurl);
}

void WebImportWizardPage::setProtocol(const QString& protocol)
{
  commandLine->setText("wget -c -np -r --level=5 -nH " + protocol.toLower() + "://" + siteUrl->text());
}

void WebImportWizardPage::setBaseURL(const KUrl& a_baseURL)
{
  baseURL = a_baseURL;
  baseURL.adjustPath(KUrl::AddTrailingSlash);
}

void WebImportWizardPage::slotStart()
{
  if ( !start )
  {
    emit enableMessagesWidget();

    if (QuantaNetAccess::mkdir(baseURL, 0L, -1, false))
    {
      if (baseURL.protocol() == "file")
      {
          ::chdir(baseURL.path(KUrl::AddTrailingSlash).toLocal8Bit());

        proc = new KProcess();
        proc ->clearArguments();

        QStringList list = commandLine->text().split(" ");

        for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it )
        {
          *proc << *it;
        }

        connect( proc, SIGNAL(receivedStdout(   KProcess*,char*,int)), this,
                       SLOT(  slotGetWgetOutput(KProcess*,char*,int)));
        connect( proc, SIGNAL(receivedStderr(   KProcess*,char*,int)), this,
                       SLOT(  slotGetWgetOutput(KProcess*,char*,int)));
        connect( proc, SIGNAL(processExited(    KProcess *)), this,
                       SLOT(  slotGetWgetExited(KProcess *)));

        if (proc->start(KProcess::NotifyOnExit, KProcess::AllOutput))
        {
          start = true;
          button->setText( i18n("Stop") );
          emit enableNextButton((QWidget *)this->parent(),false);
        } else
        {
          KMessageBox::error(this, i18n("<qt>There was an error while trying to run the <b>wget</b> application.<br>Check first that it is present on your system and that it is in your PATH.</qt>"));
          delete proc;
        }
      } else
      {
        KMessageBox::sorry(this, i18n("This feature is available only if the project lies on a local disk."));
        start = false;
        button->setText(i18n("Start"));
        emit enableNextButton((QWidget *)this->parent(),true);
      }
   } else
   {
    //FIXME QuantaCommon::dirCreationError(this, baseURL);
     start = false;
     button->setText( i18n("Start") );
     emit enableNextButton((QWidget *)this->parent(),true);
   }
  } else {

    // STOPPING !!!

    delete proc;

    start = false;
    button->setText( i18n("Start") );
    emit enableNextButton((QWidget *)this->parent(),true);
  }
}

void WebImportWizardPage::slotGetWgetExited(KProcess*)
{
  progress->setValue(0);
  start = false;
  button->setText( i18n("Start") );
  emit messages(i18n("wget finished...\n"));
  emit enableNextButton((QWidget *)this->parent(),true);
}

void WebImportWizardPage::slotGetWgetOutput(KProcess *, char *buffer, int buflen)
{
  QString output = buffer;
  output = output.left( buflen );

  emit messages(output);

  int pos;
  QString basePath = baseURL.path(KUrl::AddTrailingSlash);
  while ( (pos = output.indexOf("saved")) != -1 )
  {
    progress->advance(1);
    int begName = output.lastIndexOf('`', pos);
    if ( begName == -1 ) {
      output = output.remove(0, pos+1);
      continue;
    }

    int endName = output.indexOf('\'', begName);
    if ( endName == -1 || endName > pos ) {
      output = output.remove(0,pos+1);
      continue;
    }

    QString fileName = output.left(endName);
    fileName = fileName.right(endName - begName-1);

    output = output.remove(0, pos+1);

    KUrl u = baseURL;
    u.addPath(fileName);
    listView->createItem(fileName, u);
  }
}

KUrl::List WebImportWizardPage::files()
{
  KUrl::List list;
  Q3ListViewItemIterator it(listView);
  while (it.current())
  {
    SimpleTreeViewItem *item = static_cast<SimpleTreeViewItem*>(it.current());
    list.append(item->url());
    ++it;
  }
  return list;
}



#include "webimportwizardpage.moc"
