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

#include <kwin.h>
#include <kcmdlineargs.h>
#include <kurl.h>
#include <kiconloader.h>

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

     quanta ->openLastFiles();

     for (int i = 0; i < args->count(); i++ )
     {
       quanta->slotFileOpen( args->url(i) );
     }
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
}


KQUniqueApplication::~KQUniqueApplication()
{
}


int KQUniqueApplication::newInstance()
{
  if (mainWidget()) {
    KWin::setActiveWindow(mainWidget()->winId());
  } else {
	KSplash *splash = new KSplash();

	quanta = new QuantaApp;
	setMainWidget(quanta);
	quanta->show();
	quanta->openLastFiles();
    delete splash;

  }

  // process URLs...
  QuantaApp *mainWin = static_cast<QuantaApp*>(mainWidget());
  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
  for (int i = 0; i < args->count(); i++) {
    mainWin->slotFileOpen(args->url(i));
  }

  return 0;
}

//--------------------------------

#include "kqapp.moc"
