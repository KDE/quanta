/*
    KQApplication.cpp

    KNode, the KDE newsreader
    Copyright (c) 1999-2001 the KNode authors.
    See file AUTHORS for details

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software Foundation,
    Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, US
*/

#include <qtimer.h>

#include <kwin.h>
#include <kcmdlineargs.h>
#include <kurl.h>
#include <kiconloader.h>
#include <kstandarddirs.h>

#include "quanta.h"
#include "kqapp.h"

QuantaApp *quanta = 0L;

KSplash::KSplash()
 : QFrame( 0L, QString("Quanta")+VERSION,
	QWidget::WStyle_NoBorder | QWidget::WStyle_Customize)
{
	QPixmap pm( UserIcon("quantalogo") );

	setBackgroundPixmap(pm);
	setGeometry( QApplication::desktop()->width ()/2-160,
				QApplication::desktop()->height()/2-120,
							320, 240 );
	setLineWidth(0);
	show();
}


KSplash::~KSplash()
{
}

KQApplication::KQApplication()
 : KApplication()
{
   KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
   KSplash *splash ;

   KGlobal::dirs()->addPrefix(PREFIX);

   bool showSplash = args->isSet("logo");

   if (showSplash) splash = new KSplash();

   if (isRestored())
   {
     RESTORE(QuantaApp);
   }
   else
   {
     QuantaApp *quanta;

     quanta = new QuantaApp();
     quanta ->show();

     QString initialProject;
     QStringList initialFiles;

     for (int i = 0; i < args->count(); i++ )
     {
       QString arg = args->url(i).url();

       if(arg.findRev(QRegExp(QString(".+\\.webprj"))) != -1)
         initialProject = arg;
       else
         initialFiles += arg;
     }
     quanta->loadInitialProject(initialProject); // open initial project

     for(QStringList::Iterator it = initialFiles.begin();it != initialFiles.end();++it)
       quanta->slotFileOpen(KURL(*it), qConfig.defaultEncoding);  // load initial files

     QTimer::singleShot(10,quanta,SLOT(openLastFiles())); // load files from previous session
   }
   args->clear();
   if (showSplash) delete splash;
}


KQApplication::~KQApplication()
{
}

KQUniqueApplication::KQUniqueApplication()
 : KUniqueApplication()
{
  KGlobal::dirs()->addPrefix(PREFIX);
}


KQUniqueApplication::~KQUniqueApplication()
{
}


int KQUniqueApplication::newInstance()
{
  if (mainWidget())
  {
    KWin::setActiveWindow(mainWidget()->winId());
  }
  else
  {
    KSplash *splash = new KSplash();

    quanta = new QuantaApp;
    setMainWidget(quanta);
    quanta->show();

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

    QString initialProject;
    QStringList initialFiles;
    for (int i = 0; i < args->count(); i++ )
    {
      QString arg = args->url(i).url();

      if(arg.findRev(QRegExp(QString(".+\\.webprj"))) != -1)
        initialProject = arg;
      else
        initialFiles += arg;
    }

    quanta->loadInitialProject(initialProject);

    for(QStringList::Iterator it = initialFiles.begin();it != initialFiles.end();++it)
           QTimer::singleShot(10,quanta,SLOT(openLastFiles()));

    quanta ->openLastFiles();
    delete splash;
  }

  return 0;
}

//--------------------------------

#include "kqapp.moc"
