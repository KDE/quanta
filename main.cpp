/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Втр Май  9 13:29:57 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
    email                : pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// kde includes
#include <klocale.h>
#include <kaboutdata.h>
#include <kiconloader.h>
#include <kcmdlineargs.h>

// qt includes
#include <qpixmap.h>

// app includes
#include "quanta.h"
#include "kwrite/kwview.h"

static const char *description =
	I18N_NOOP("Quanta Plus Web Development Environment");
// INSERT A DESCRIPTION FOR YOUR APPLICATION HERE
	
	
static KCmdLineOptions options[] =
{
  { "+[File]", I18N_NOOP("file to open"), 0 },
  { 0, 0, 0 }
  // INSERT YOUR COMMANDLINE OPTIONS HERE
};

int main(int argc, char *argv[])
{
	KAboutData
		aboutData( "quanta", I18N_NOOP("Quanta"),
		VERSION, description, KAboutData::License_GPL,
		"(c) 2000, Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon");
		aboutData.addAuthor("Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon",0, "pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com");

	KCmdLineArgs::init( argc, argv, &aboutData );
	KCmdLineArgs::addCmdLineOptions( options ); // Add our own options.

	KApplication app;
	
	QFrame *f = new QFrame( 0L, QString("Quanta")+VERSION,
													QWidget::WStyle_NoBorder | QWidget::WStyle_Customize );

  QPixmap pm( UserIcon("quantalogo") );

  f->setBackgroundPixmap(pm);
  f->setGeometry(	QApplication::desktop()->width ()/2-160,
  						    QApplication::desktop()->height()/2-120,
  						    320, 240 );
  f->setLineWidth(0);
  						
  f->show();
	
  if (app.isRestored())
  {
    RESTORE(QuantaApp);
  }
  else
  {
    QuantaApp *quanta = new QuantaApp();

    delete f;

    quanta->show();
    quanta->openLastFiles();

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
		
		if (args->count())
		{
        quanta->openDocumentFile(args->arg(0));
		}
		else
		{
		  quanta->openDocumentFile();
		}
		args->clear();
  }

  return app.exec();
}
