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
#include <core.h>
#include <idocumentcontroller.h>
#include <iplugin.h>
#include <iplugincontroller.h>
#include <iprojectcontroller.h>
#include <plugincontroller.h>

#include "kdevquantaextension.h"

#include "version.h"

static KCmdLineOptions options[] =
{
  { "profile <profile>", I18N_NOOP("Profile to load"), 0 },
  { "project <project>", I18N_NOOP("Project to load"), 0 },
  { "+file(s)", I18N_NOOP("Files to load"), 0 },
  { 0,0,0 }
};


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

  KAboutData aboutData("quanta", I18N_NOOP("Quanta"),
                       QUANTA_VERSION, description, KAboutData::License_GPL,
                       I18N_NOOP("(c) 1999-2006, The Quanta+ developers"), othertext, "http://www.kdewebdev.org");
                           
  aboutData.addAuthor("Eric Laffoon",I18N_NOOP("Project Lead - public liaison"), "sequitur@kde.org");
  aboutData.addAuthor("Andras Mantia",I18N_NOOP("Program Lead - bug squisher"), "amantia@kde.org");
  aboutData.addCredit("Jens Herden",
                      I18N_NOOP("Invaluable member - Help with code cleanup, porting to KDevelop and KDE4, etc."),
                      "jens@kdewebdev.org");
 

  KCmdLineArgs::init(argc, argv, &aboutData);
  KCmdLineArgs::addCmdLineOptions( options );
  KCmdLineArgs* args = KCmdLineArgs::parsedArgs();

  KApplication app;

  KDevQuantaExtension::init();

//FIXME: There is no SplashScreen at this moment
  /*
  SplashScreen *splash = 0;

  QString splashFile = KStandardDirs::locate("appdata", "pics/quantalogo.png");
  if (!splashFile.isEmpty())
  {
    QPixmap pm;
    pm.load(splashFile);
    splash = new SplashScreen( pm );
    splash->show();
    splash->repaint();
  }
  */
  
    //initialize the api object
    //WARNING! the order is important
  KDevelop::Core::initialize();
  
  /*
  //initialize the api object
  //WARNING! the order is important
  Koncrete::Core::setMainWindow( new Koncrete::MainWindow );
  Koncrete::Core::setPartController( new Koncrete::PartController );
  Koncrete::Core::setDocumentController( new Koncrete::DocumentController );
  
  Koncrete::Core::setLanguageController( new Koncrete::LanguageController );
  Koncrete::Core::setProjectController( new Koncrete::ProjectController );
  Koncrete::Core::setBackgroundParser( new Koncrete::BackgroundParser );
  Koncrete::Core::setEnvironment( new Koncrete::Environment );
  */
  
//FIXME: There is no SplashScreen at this moment
  /*
  if ( splash )
  {
    QObject::connect(Core::self(), SIGNAL(loadingPlugin(const QString&)),
                     splash, SLOT(showMessage(const QString&)));
   QTimer::singleShot(0, splash, SLOT(deleteLater()));
    QObject::connect( Koncrete::Core::documentController(),
                      SIGNAL( openingDocument( const QString & ) ),
                      splash, SLOT( showMessage( const QString & ) ) );

    splash->showMessage( i18n( "Starting GUI" ) );
  }
*/
  
  //Load QuantaCore *before* loading other plugins, otherwise the signal
  //connection between them an QuantaCore will not work.
  KDevelop::IPlugin *p = KDevelop::Core::self()->pluginController()->loadPlugin("KDevQuantaCore");
  if (!p)
  {
 //   delete splash;
 //   splash = 0;
    KMessageBox::error( 0L, i18n("The Quanta Core Plugin could not be loaded.\nYour installation seems to be broken."));
  }

  KDevelop::Core::self()->pluginController()->loadPlugins( KDevelop::PluginController::Global);


  for( int i=0; i<args->count(); ++i )
  {
    kDebug(24000) << "------> arg " << args->arg(i) << endl;
  }

  bool openProject = false;
  if( args->count() == 0 ){
//    Koncrete::Core::projectController()->init(); do we still need it?
    openProject = true;
  } else
    if( args->count() > 0 )
    {
      KUrl url = args->url( 0 );
      QString ext = QFileInfo( url.fileName() ).suffix();
      if( ext == "kdevelop" || ext == "quanta" )
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
