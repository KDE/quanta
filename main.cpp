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

#include <stdlib.h>

// kde includes
#include <klocale.h>
#include <kaboutdata.h>
#include <kiconloader.h>
#include <kcmdlineargs.h>
#include <ksimpleconfig.h>

// qt includes
#include <qpixmap.h>
#include <qnetwork.h>

// app includes
#include "quanta.h"
#include "kwrite/kwview.h"


static const char *description =
	I18N_NOOP("Quanta Plus Web Development Environment");
// INSERT A DESCRIPTION FOR YOUR APPLICATION HERE
	

static KCmdLineOptions options[] =
{
  { "+[File]", I18N_NOOP("file to open"), 0 },
  { "unique", I18N_NOOP("Whether we start as a one-instance application."), 0 },
  { 0, 0, 0 }
  // INSERT YOUR COMMANDLINE OPTIONS HERE
};

int main(int argc, char *argv[])
{
	KAboutData
		aboutData( "quanta", I18N_NOOP("Quanta"),
		VERSION, description, KAboutData::License_GPL_V2,
		"(c) 2000, 2001",
		"We hope you enjoy our program.",
		"http://quanta.sourceforge.net"
		);

 	aboutData.addAuthor("Dmitry Poplavsky",0, "dima@kde.org");
 	aboutData.addAuthor("Alexander Yakovlev",0, "yshurik@kde.org");
 	aboutData.addAuthor("Eric Laffoon",0, "sequitur@kde.org");

 	aboutData.addCredit("Richard Moore",
    "Coding and tag dialog definition documentation",
    "rich@kde.org");

  aboutData.addCredit("Matthew Colton",
    "Cool splash screen for quanta",
    "mat.colton@web-xs.de");

  aboutData.addCredit("Claus Hindsgaul",
    "Danish translation",
    "claus_h@image.dk");

  aboutData.addCredit("Dmitri Dmitrienko",
    "Part of a code for PHP4 Debugger",
    "dd@cron.ru");


  KCmdLineArgs::init( argc, argv, &aboutData );
  KCmdLineArgs::addCmdLineOptions( options ); // Add our own options.

  // this defeats the purpose of KCmdLineArgs, but there is no other
  // way around, I'm afraid.
  const char *uniq = "--unique";
  bool isUnique = false;
  for ( int i = 1; i < argc; i++ ) {
    if ( strcmp(argv[i], uniq) == 0 ) {
      isUnique = true;
      KUniqueApplication::addCmdLineOptions(); // before calling parsedArgs!
      break;
    }
  }


  KApplication *app;
  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

  if (isUnique) {
	if (!KQUniqueApplication::start())
		exit(0);
	app = new KQUniqueApplication;
  } else {
	app = new KQApplication;
  }
  qInitNetworkProtocols();

  return app->exec();
}
