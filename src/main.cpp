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
#include <kmainwindow.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>
#include <kdebug.h>
#include <kparts/componentfactory.h>

#include <QFileInfo>

//kdevelop includes
#include <kdevcore.h>
#include <kdevbackgroundparser.h>
#include <kdevenvironment.h>
#include <kdevpartcontroller.h>
#include <kdevprojectcontroller.h>
#include <kdevlanguagecontroller.h>
#include <kdevmainwindow.h>
#include <kdevplugincontroller.h>
#include <kdevdocumentcontroller.h>
#include <shell/splashscreen.h>
#include <kdevplugin.h>
#include <kdeveditorintegrator.h>

#include "kdevquantaextension.h"

#include "config.h"

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
                       VERSION, description, KAboutData::License_GPL,
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
  
  // initialize the editor integrator - it needs a qobject on the main thread
  KDevEditorIntegrator::initialise();
  
  //initialize the api object
  //WARNING! the order is important
  KDevCore::setMainWindow( new KDevMainWindow );
  KDevCore::setPartController( new KDevPartController );
  KDevCore::setDocumentController( new KDevDocumentController );
  
  KDevCore::setLanguageController( new KDevLanguageController );
  KDevCore::setProjectController( new KDevProjectController );
  KDevCore::setBackgroundParser( new KDevBackgroundParser );
  KDevCore::setEnvironment( new KDevEnvironment );
  
  if ( splash )
  {
    QObject::connect(KDevPluginController::self(), SIGNAL(loadingPlugin(const QString&)),
                     splash, SLOT(showMessage(const QString&)));
    QObject::connect( KDevCore::documentController(),
                      SIGNAL( openingDocument( const QString & ) ),
                      splash, SLOT( showMessage( const QString & ) ) );

    splash->showMessage( i18n( "Starting GUI" ) );
  }

  QObject::connect( KDevCore::mainWindow(), SIGNAL( finishedLoading() ),
                    splash, SLOT( deleteLater() ) );

  KDevCore::initialize();

  //Load QuantaCore *before* loading other plugins, otherwise the signal
  //connection between them an QuantaCore will not work.
  KDevPlugin *p = KDevPluginController::self()->loadPlugin("KDevQuantaCore");
  if (!p)
  {
    delete splash;
    splash = 0;
    KMessageBox::error( 0L, i18n("The Quanta Core Plugin could not be loaded.\nYour installation seems to be broken."));
  }

  KDevPluginController::self()->loadPlugins( KDevPluginController::Global );


  for( int i=0; i<args->count(); ++i )
  {
    kDebug(24000) << "------> arg " << args->arg(i) << endl;
  }

  bool openProject = false;
  if( args->count() == 0 ){
//    KDevCore::projectController()->init(); do we still need it?
    openProject = true;
  } else
    if( args->count() > 0 )
    {
      KUrl url = args->url( 0 );
      QString ext = QFileInfo( url.fileName() ).suffix();
      if( ext == "kdevelop" || ext == "quanta" )
      {
        KDevCore::projectController()->openProject( url );
        openProject = true;
      }
  }

  if( !openProject ){
    for( int a = 0; a < args->count(); ++a )
    {
      KDevCore::documentController()->editDocument( KUrl(args->url(a)) );
    }
  }
#warning "kde4: port it"
  //kapp->dcopClient()->registerAs("quanta");

  return app.exec();
}
