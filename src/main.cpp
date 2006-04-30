#include <kaboutdata.h>
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <kmainwindow.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>
#include <kdebug.h>
#include <dcopclient.h>
#include <kparts/componentfactory.h>

#include <qfileinfo.h>

//kdevelop includes
#include <shell/api.h>
#include <shell/splashscreen.h>
#include <shell/toplevel.h>
#include <shell/plugincontroller.h>
#include <shell/documentcontroller.h>
#include <shell/core.h>
#include <shell/projectmanager.h>
#include <shell/simplemainwindow.h>
#include <interfaces/kdevplugin.h>

#include "kdevquantaextension.h"

#include "config.h"

static KCmdLineOptions options[] =
{
  { "profile <profile>", I18N_NOOP("Profile to load"), 0 },
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
  QString splashFile = locate("appdata", "pics/quantalogo.png");
  if (!splashFile.isEmpty())
  {
    QPixmap pm;
    pm.load(splashFile);
    splash = new SplashScreen( pm );
  }

  app.processEvents();

  if (splash)
    splash->showMessage( i18n("Loading Settings") );
  TopLevel::getInstance()->loadSettings();

  QObject::connect(PluginController::getInstance(), SIGNAL(loadingPlugin(const QString &)),
                   splash, SLOT(message(const QString &)));
  if (splash)
    splash->show();

  //Load QuantaCore *before* loading other plugins, otherwise the signal
  //connection between them an QuantaCore will not work.
  KDevPlugin *p = PluginController::getInstance()->loadPlugin("KDevelop/Quanta", "");
  if (!p)
  {
    delete splash;
    splash = 0;
    KMessageBox::error( 0L, i18n("The Quanta Core Plugin could not be loaded.\nYour installation seems to be broken!"));
  }
    
  PluginController::getInstance()->loadInitialPlugins();

  Core::getInstance()->doEmitCoreInitialized();

  if (splash)
    splash->showMessage( i18n( "Starting GUI" ) );
/*  //BEGIN a workaround on kmdi bug - we do not allow mainwindow to be shown until now
  SimpleMainWindow *mw = dynamic_cast<SimpleMainWindow*>(TopLevel::getInstance()->main());
  if (mw)
    mw->enableShow();
  //END workaround*/
  TopLevel::getInstance()->main()->show();

  if (splash)
    delete splash;

  for( int i=0; i<args->count(); ++i )
  {
    kDebug(24000) << "------> arg " << args->arg(i) << endl;
  }

  bool openProject = false;
  if( args->count() == 0 ){
    ProjectManager::getInstance()->loadDefaultProject();
    openProject = true;
  } else
    if( args->count() > 0 )
    {
      KUrl url = args->url( 0 );
      QString ext = QFileInfo( url.fileName() ).suffix();
      if( ext == "kdevelop" || ext == "quanta" )
      {
        ProjectManager::getInstance()->loadProject( url );
        openProject = true;
      }
  }

  if( !openProject ){
    for( int a = 0; a < args->count(); ++a )
    {
      DocumentController::getInstance()->editDocument( KUrl(args->url(a)) );
    }
  }

  kapp->dcopClient()->registerAs("quanta");

  return app.exec();
}
