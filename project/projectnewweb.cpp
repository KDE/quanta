/***************************************************************************
                          projectnewweb.cpp  -  description
                             -------------------
    begin                : Fri Oct 27 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
    email                : pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com
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

// kde includes
#include <klocale.h>
#include <kprocess.h>
#include <kiconloader.h>

// app includes
#include "projectnewweb.h"
#include "../qextfileinfo.h"

ProjectNewWeb::ProjectNewWeb(QWidget *parent, const char *name )
	: ProjectNewWebS(parent,name)
{
  button->setEnabled(false);
  siteUrl->setFocus();
  
  imagelabel->setPixmap( UserIcon("wiznewprjweb") );
  
  listFiles->setColumnAlignment(1,Qt::AlignRight);
  
  connect( commandLine, SIGNAL(textChanged(const QString&)),
           this,        SLOT  (enableStart(const QString&)));
  connect( siteUrl,     SIGNAL(textChanged(const QString&)),
           this,        SLOT  (setCommandL(const QString&)));
  connect( button,      SIGNAL(clicked()),
           this,        SLOT  (slotStart()));
           
  start = false;
  filesList.clear();
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
  
  if ( url.left(6) != "ftp://" && url.left(7) != "http://" ) 
  {
    siteurl = QString("http://")+url;
  }
  commandLine->setText("wget -c -np -r --level=5 -nH "+siteurl);
}

void ProjectNewWeb::setBasePath(QString path)
{
  this->basePath = path;
  if ( basePath.right(1) != "/" )	basePath += "/";
}

void ProjectNewWeb::slotStart()
{
  if ( !start )
  {
    emit enableMessages();
    
    QExtFileInfo::createDir( basePath );
    
    chdir( basePath );
    
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
                   
    proc->start(KProcess::NotifyOnExit, KProcess::AllOutput);
    
    start = true;
    button->setText( i18n("Stop") );
  	emit enableNextButton((QWidget *)this->parent(),false);
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
	start = false;
  button->setText( i18n("Start") );
	emit messages("wget finished...\n");
	emit enableNextButton((QWidget *)this->parent(),true);
}

void ProjectNewWeb::slotGetWgetOutput(KProcess *, char *buffer, int buflen)
{
  QString output = buffer;
  output = output.left( buflen );

  emit messages(output);
  
  int pos;
  while ( (pos = output.find("saved")) != -1 )
  {
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
    
    textFromTo->setText( siteUrl->text()+" -->> "+basePath+fileName );
    
    filesList.append( basePath+fileName );
    
    QFileInfo fi( basePath+fileName );
    QString size;
    size.sprintf( "%i", fi.size() );
    
    new QListViewItem(listFiles, fileName, size );
    listFiles->setColumnWidth(0,listFiles->width()-listFiles->columnWidth(1)-20);
  }
}

QStringList ProjectNewWeb::files()
{
	return filesList;
}

void ProjectNewWeb::resizeEvent ( QResizeEvent *t )
{
  ProjectNewWebS::resizeEvent(t);
  listFiles->setColumnWidth(0,listFiles->width()-listFiles->columnWidth(1)-20);
}
#include "projectnewweb.moc"
