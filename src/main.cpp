/***************************************************************************
 *   Copyright (C) 2005 by Andras Mantia                                   *
 *   amantia@kde.org                                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <kaboutdata.h>
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <kxmlguiwindow.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>
#include <kdebug.h>
#include <kparts/componentfactory.h>

#include <QFileInfo>

//kdevelop includes
#include <shell/core.h>
#include <interfaces/idocumentcontroller.h>
#include <interfaces/iplugin.h>
#include <interfaces/iplugincontroller.h>
#include <interfaces/iprojectcontroller.h>
#include <shell/plugincontroller.h>

#include "kdevquantaextension.h"

#include "version.h"


int main(int argc, char *argv[])
{
  static const char description[] =
      I18N_NOOP("Quanta Plus Web Development Environment");
// INSERT A DESCRIPTION FOR YOUR APPLICATION HERE

  static const char othertext[] =
      I18N_NOOP("Our goal is to be nothing less than the best possible tool for\n \
      working with tagging and scripting languages.\
      \n\nQuanta Plus is not in any way affiliated with any commercial\n \
      versions of Quanta. \
      \n\nWe hope you enjoy Quanta Plus.\n\n");

  KAboutData aboutData("quanta", 0, ki18n("Quanta"),
                       QUANTA_VERSION, ki18n(description), KAboutData::License_GPL,
                       ki18n("(c) 1999-2006, The Quanta+ developers"), ki18n(othertext), "http://www.kdewebdev.org");
                           
  aboutData.addAuthor(ki18n("Eric Laffoon"),ki18n("Project Lead - public liaison"), "sequitur@kde.org");
  aboutData.addAuthor(ki18n("Andras Mantia"),ki18n("Program Lead - bug squisher"), "amantia@kde.org");
  aboutData.addCredit(ki18n("Jens Herden"),
                      ki18n("Invaluable member - Help with code cleanup, porting to KDevelop and KDE4, etc."),
                      "jens@kdewebdev.org");
 

  KCmdLineArgs::init(argc, argv, &aboutData);

  KCmdLineOptions options;
  options.add("profile <profile>", ki18n("Profile to load"));
  options.add("project <project>", ki18n("Project to load"));
  options.add("+file(s)", ki18n("Files to load"));
  KCmdLineArgs::addCmdLineOptions( options );
  KCmdLineArgs* args = KCmdLineArgs::parsedArgs();

  KApplication app;
  KDevQuantaExtension::init();
  KDevelop::Core::initialize();
  
  for( int i=0; i<args->count(); ++i )
  {
    kDebug(24000) << "------> arg " << args->arg(i);
  }

  bool openProject = false;
  if( args->count() == 0 ){
    openProject = true;
  } else
    if( args->count() > 0 )
    {
      KUrl url = args->url( 0 );
      QString ext = QFileInfo( url.fileName() ).suffix();
      if( ext == KDevelop::ShellExtension::getInstance()->projectFileExtension() )
      {
        KDevelop::Core::self()->projectController()->openProject( url );
        openProject = true;
      }
  }

  if( !openProject ){
    for( int a = 0; a < args->count(); ++a )
    {
      KDevelop::Core::self()->documentController()->openDocument( KUrl(args->url(a)) );
    }
  }

  return app.exec();
}
