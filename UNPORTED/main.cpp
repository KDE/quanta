/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : ��� ���  9 13:29:57 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com>
                           (C) 2001-2002 Andras Mantia <amantia@kde.org>
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
//#include <kdebug.h>

// qt includes
#include <qpixmap.h>
#include <qnetwork.h>
#include <qdom.h>
#include <qfile.h>
#include <qfileinfo.h>

// app includes
#include "quanta.h"


static const char *description =
  I18N_NOOP("Quanta Plus Web Development Environment");
// INSERT A DESCRIPTION FOR YOUR APPLICATION HERE

static const char *othertext =
  I18N_NOOP("Our goal is to be nothing less than the best possible tool for\n \
working with tagging and scripting languages.\
\n\nQuanta Plus is not in any way affiliated with any commercial\n \
versions of Quanta. \
\n\nWe hope you enjoy Quanta Plus.\n\n");


static KCmdLineOptions options[] =
{
  { "+[File]", I18N_NOOP("File to open"), 0 },
  { "unique", I18N_NOOP("Whether we start as a one-instance application."), 0 },
  { "nologo", I18N_NOOP("Do not show the nice logo during startup."), 0 },
  KCmdLineLastOption
  // INSERT YOUR COMMANDLINE OPTIONS HERE
};

int main(int argc, char *argv[])
{
  KAboutData
    aboutData( PACKAGE, I18N_NOOP("Quanta"),
    VERSION, description, KAboutData::License_GPL_V2,
    "(c) 2000, 2001, 2002",
    othertext,
    "http://quanta.sourceforge.net"
    );

  //aboutData.otherText(&othertext);

   aboutData.addAuthor("Eric Laffoon",I18N_NOOP("Project Lead - public liason"), "sequitur@kde.org");
   aboutData.addAuthor("Andras Mantia",I18N_NOOP("Program Lead - bug squisher"), "amantia@kde.org");
   aboutData.addAuthor("Dmitry Poplavsky",I18N_NOOP("Inactive - left for commercial version"), "dima@kde.org");
   aboutData.addAuthor("Alexander Yakovlev",I18N_NOOP("Inactive - left for commercial version"), "yshurik@kde.org");

   aboutData.addCredit("Richard Moore",
    I18N_NOOP("Coding and tag dialog definition documentation and more"),
    "rich@kde.org");

  aboutData.addCredit("Marc Britton",
    I18N_NOOP("Various fixes, scripting dialog system"),
    "consume@optushome.com.au");

  aboutData.addCredit("Robert Nickel",
    I18N_NOOP("Documentation, many cool parsing scripts to automate \ndevelopment"),
    "robert@artnickel.com");

  aboutData.addCredit("Jason P. Hanley",
    I18N_NOOP("Various fixes, foundational code for the old DTD parsing and other \nDTD related work"),
    "jphanley@buffalo.edu");

  aboutData.addCredit("George Vilches",
    I18N_NOOP("Tree based upload dialog"),
    "gav@creolmail.org");

  aboutData.addCredit("Ted Pibil",
    I18N_NOOP("Addition and maintenance of DTDs"),
    "ted@pibil.org");

  aboutData.addCredit("Nicolas Deschildre",
    I18N_NOOP("Visual Page Layout part, new undo/redo system"),
    "nicolasdchd@ifrance.com");

  aboutData.addCredit("Luciano Gulmini",
    I18N_NOOP("Frame wizard, CSS wizard"),
    "e.gulmini@tiscali.it");

  aboutData.addCredit("Emiliano Gulmini",
    I18N_NOOP("Crash recovery"),
    "emi_barbarossa@yahoo.it");

  aboutData.addCredit("Jens Herden",
    I18N_NOOP("Cleanup of the treeview code, code review"),
    "jhe@epost.de");

  aboutData.addCredit("Andrea Bergia",
    I18N_NOOP("Original CSS editor"),
    "andreabergia@yahoo.it");

  aboutData.addCredit("Matthew Colton",
    I18N_NOOP("Cool splash screen for many version releases of Quanta"),
    "mat.colton@web-xs.de");

  aboutData.addCredit("Claus Hindsgaul",
    I18N_NOOP("Danish translation"),
    "claus_h@image.dk");

  aboutData.addCredit("Dmitri Dmitrienko",
    I18N_NOOP("Part of a code for PHP4 Debugger"),
    "dd@cron.ru");

  aboutData.addCredit("Keith Isdale",
    I18N_NOOP("XSLT tags, XSLT debugger"),
    "k_isdale@tpg.com.au");

  aboutData.addCredit("Lukas Masek",
    I18N_NOOP("Splash screen and icon for 3.2"),
    "luci@sh.ground.cz");

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

  if (isUnique) {
//  if (!KQUniqueApplication::start())
//    exit(0);
  app = new KQUniqueApplication;
  } else {
  app = new KQApplication;
  }

  qInitNetworkProtocols();

//  kdDebug(24000)<<"Calling app->exec()"<<endl;
  return app->exec();
}
