/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Wed Apr 4 14:29:38 EET DST 2003
    copyright            : (C) 2003 by Mantia Andras
    email                : amantia@freemail.hu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <kglobal.h>
#include <klocale.h>

#include "framewizard.h"

static const char *description =
  I18N_NOOP("Frame Wizard");
// INSERT A DESCRIPTION FOR YOUR APPLICATION HERE
  
  
static KCmdLineOptions options[] =
{
  {"verbose",I18N_NOOP("Print the results to the stdout."),0},
  { 0, 0, 0 }
  // INSERT YOUR COMMANDLINE OPTIONS HERE
};

int main(int argc, char *argv[])
{

  KAboutData aboutData( "framewizard", I18N_NOOP("Frame Wizard"),
    "0.1", description, KAboutData::License_GPL_V2,
    "(c) 2003 Gulmini Luciano", 0, 0, "gulmini.luciano@student.unife.it");
  aboutData.addAuthor("Gulmini Luciano", 0, "gulmini.luciano@student.unife.it");
  aboutData.addAuthor("Mantia Andras", "bugfixes, patches, Quanta integration", "amantia@freemail.hu");
  KCmdLineArgs::init( argc, argv, &aboutData );
  KCmdLineArgs::addCmdLineOptions( options ); // Add our own options.

  KApplication a;
  FrameWizard* fw=new FrameWizard;
  a.setMainWidget(fw);
  int result = fw->exec();
  if(result){
    fw->generate();
  }
//  int result = a.exec();

  delete fw;

  return result;
}
