/***************************************************************************
                          projectnewweb.cpp  -  description
                             -------------------
    begin                : Fri Oct 27 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com>
                           (C) 2002, 2003 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// qt includes
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qstringlist.h>
#include <qlistview.h>
#include <qfileinfo.h>
#include <qcombobox.h>

// kde includes
#include <klocale.h>
#include <kprocess.h>
#include <kiconloader.h>
#include <kio/job.h>
#include <kmessagebox.h>
#include <kled.h>

// app includes
#include "projectnewweb.h"
#include "qextfileinfo.h"
#include "quantacommon.h"
#include "uploadtreeview.h"

ProjectNewWeb::ProjectNewWeb(QWidget *parent, const char *name )
  : ProjectNewWebS(parent,name)
{
  button->setEnabled(false);
  siteUrl->setFocus();

  imagelabel->setPixmap( UserIcon("wiznewprjweb") );
  listView->removeColumn(1);
  listView->removeColumn(1);
  listView->removeColumn(1);

  connect( commandLine, SIGNAL(textChanged(const QString&)),
           this,        SLOT  (enableStart(const QString&)));
  connect( siteUrl,     SIGNAL(textChanged(const QString&)),
           this,        SLOT  (setCommandL(const QString&)));
  connect( button,      SIGNAL(clicked()),
           this,        SLOT  (slotStart()));
  connect( protocolCombo,SIGNAL(highlighted(const QString&)),
           this,         SLOT  (setProtocol(const QString&)));

  start = false;
  KLed1->setState(KLed::Off);
  KLed2->setState(KLed::Off);
}

ProjectNewWeb::~ProjectNewWeb(){
}

void ProjectNewWeb::enableStart(const QString&)
{
  if ( !commandLine->text().isEmpty() ) button->setEnabled(true);
  else                                  button->setEnabled(false);
}

void ProjectNewWeb::setCommandL(const QString& url)
{
  QString siteurl = url;

  commandLine->setText("wget -c -np -r --level=5 -nH "+ protocolCombo->currentText().lower()+ "://"+siteurl);
}

/** No descriptions */
void ProjectNewWeb::setProtocol(const QString& protocol)
{
  commandLine->setText("wget -c -np -r --level=5 -nH "+ protocol.lower()+ "://"+siteUrl->text());
}



void ProjectNewWeb::setBaseURL(const KURL& a_baseURL)
{
  baseURL = a_baseURL;
  baseURL.adjustPath(1);
}

void ProjectNewWeb::slotStart()
{
  if ( !start )
  {
    emit enableMessagesWidget();

    if (QExtFileInfo::createDir(baseURL, this))
    {
      if (baseURL.protocol() == "file")
      {
        chdir( baseURL.path(1));

        proc = new KProcess();
        proc ->clearArguments();

        QStringList list = QStringList::split (" ", commandLine->text());

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
          KMessageBox::error(this, i18n("There was an error while trying to run the \"wget\" application.\
           Check first that it is present on your system and that it is in your PATH."));
          delete proc;
        }
      } else
      {
        KMessageBox::sorry(this,i18n("This feature is available only if the project lies on a local disk."));
        start = false;
        button->setText( i18n("Start") );
        emit enableNextButton((QWidget *)this->parent(),true);
      }
   } else
   {
     QuantaCommon::dirCreationError(this, baseURL);
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

void ProjectNewWeb::slotGetWgetExited(KProcess*)
{
  KLed2->setState(KLed::Off);
  KLed1->setState(KLed::Off);
  start = false;
    button->setText( i18n("Start") );
    emit messages(i18n("wget finished...\n"));
  emit enableNextButton((QWidget *)this->parent(),true);
}

void ProjectNewWeb::slotGetWgetOutput(KProcess *, char *buffer, int buflen)
{
  QString output = buffer;
  output = output.left( buflen );

  emit messages(output);

  int pos;
  QString basePath = baseURL.path(1);
  while ( (pos = output.find("saved")) != -1 )
  {
    if (KLed1->state() == KLed::Off)
    {
      KLed1->setState(KLed::On);
      KLed2->setState(KLed::Off);
    } else
    {
      KLed2->setState(KLed::On);
      KLed1->setState(KLed::Off);
    }
    int begName = output.findRev('`',pos);
    if ( begName == -1 ) {
      output = output.remove(0,pos+1);
      continue;
    }

    int endName = output.find('\'',begName);
    if ( endName == -1 || endName > pos ) {
      output = output.remove(0,pos+1);
      continue;
    }

    QString fileName = output.left(endName);
    fileName = fileName.right( endName - begName-1);

    output = output.remove(0,pos+1);

    KURL u = baseURL;
    QuantaCommon::setUrl(u, fileName);
    listView->addItem(u, KFileItem(KFileItem::Unknown, KFileItem::Unknown, KURL()));
  }
}

KURL::List ProjectNewWeb::files()
{
   //return fileList;
  KURL::List r;

  KURL u;
  QListViewItem *item;
  QListViewItemIterator it(listView);
  for ( ; it.current(); ++it )
  {
    item = it.current();
    if (dynamic_cast<UploadTreeFolder*>(item))
    {
    u = dynamic_cast<UploadTreeFolder*>(item)->url();
    } else
    {
    u = dynamic_cast<UploadTreeFile*>(item)->url();
    }

    if (!u.isEmpty()) r.append(u);
  }
  return r;
}

void ProjectNewWeb::resizeEvent ( QResizeEvent *t )
{
  ProjectNewWebS::resizeEvent(t);
 // listView->setColumnWidth(0,listView->width()-listView->columnWidth(1)-20);
}

#include "projectnewweb.moc"
