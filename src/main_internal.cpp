/***************************************************************************
 *   Copyright (C) 2005 by Andras Mantia <amantia@kde.org>                 *
 *   Copyright (C) 2010 Milian Wolff <mail@milianw.de>                     *
 *                                                                         *
 *   Copied parts from KDevelop:                                           *
 *   Copyright 2003-2009 Alexander Dymo <adymo@kdevelop.org>               *
 *   Copyright 2007 Ralf Habacker  <Ralf.Habacker@freenet.de>              *
 *   Copyright 2006-2007 Matt Rogers  <mattr@kde.org>                      *
 *   Copyright 2006-2007 Hamish Rodda <rodda@kde.org>                      *
 *   Copyright 2005-2007 Adam Treat <treat@kde.org>                        *
 *   Copyright 2003-2007 Jens Dagerbo <jens.dagerbo@swipnet.se>            *
 *   Copyright 2001-2002 Bernd Gehrmann <bernd@mail.berlios.de>            *
 *   Copyright 2001-2002 Matthias Hoelzer-Kluepfel <hoelzer@kde.org>       *
 *   Copyright 2003 Roberto Raggi <roberto@kdevelop.org>                   *
 *   Copyright 2010 Niko Sams <niko.sams@gmail.com>                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include <kaboutdata.h>
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <kxmlguiwindow.h>
#include <kstandarddirs.h>
#include <kdebug.h>
#include <ksplashscreen.h>
#include <ktexteditor/cursor.h>

#include <QFileInfo>
#include <QPixmap>
#include <QTimer>
#include <QDir>
#include <QSessionManager>

#include <shell/core.h>
#include <shell/mainwindow.h>
#include <shell/projectcontroller.h>
#include <shell/documentcontroller.h>
#include <shell/plugincontroller.h>
#include <shell/sessioncontroller.h>
#include <shell/runcontroller.h>
#include <shell/launchconfiguration.h>
#include <interfaces/ilauncher.h>
#include <interfaces/iproject.h>

#include "kdevquantaextension.h"
#include <KMessageBox>
#include <KProcess>

#include <iostream>
#include <QtCore/QTextStream>
#include <shell/session.h>

#include "version.h"

using KDevelop::Core;

class QuantaApplication: public KApplication
{

public:
  explicit QuantaApplication( bool GUIenabled = true )
      : KApplication( GUIenabled ) {
  }

  void saveState( QSessionManager& sm ) {
    if ( KDevelop::Core::self() && KDevelop::Core::self()->sessionController() ) {
      sm.setRestartCommand(
        QStringList() << "quanta" << "-s" << KDevelop::Core::self()->sessionController()->activeSession()->id().toString()
      );
    }

    KApplication::saveState( sm );
  }

};

int main( int argc, char *argv[] )
{
#include "shared_app_init.cpp"

  KCmdLineArgs::addCmdLineOptions( options );
  KCmdLineArgs* args = KCmdLineArgs::parsedArgs();

  QuantaApplication app;
  KDevQuantaExtension::init();

  if ( !getenv( "KDEV_SESSION" ) ) {
    QTextStream qerr( stderr );
    qerr << "Cannot run this internal app without KDEV_SESSION environment variable set!" << endl;
    exit( -127 );
  }

  KSplashScreen* splash = 0;

  QString splashFile = KStandardDirs::locate( "appdata", "pics/quanta-splash.png" );

  if ( !splashFile.isEmpty() ) {
    QPixmap pm;
    pm.load( splashFile );
    splash = new KSplashScreen( pm );
    splash->show();
  }

  if ( !Core::initialize( splash ) ) {
    return 5;
  }

  KGlobal::locale()->insertCatalog( Core::self()->componentData().catalogName() );

  Core* core = Core::self();

  QStringList projectNames = args->getOptionList( "project" );

  if ( !projectNames.isEmpty() ) {
    foreach( const QString& p, projectNames ) {
      QFileInfo info( p );

      if ( info.suffix() == "kdev4" ) {
        // make sure the project is not already opened by the session controller
        bool shouldOpen = true;
        KUrl url( info.absoluteFilePath() );
        foreach( KDevelop::IProject* p, core->projectController()->projects() ) {
          if ( p->projectFileUrl() == url ) {
            shouldOpen = false;
            break;
          }
        }

        if ( shouldOpen ) {
          core->projectController()->openProject( url );
        }
      }
    }
  }

  if ( args->isSet( "debug" ) ) {
    if ( debugArgs.isEmpty() ) {
      QTextStream qerr( stderr );
      qerr << endl << i18n( "Specify the binary you want to debug." ) << endl;
      return 1;
    }

    QString binary = debugArgs.first();

    if ( binary.contains( '/' ) ) {
      binary = binary.right( binary.lastIndexOf( '/' ) );
    }

    QString launchName = i18n( "Debug" ) + ' ' + binary;

    KDevelop::LaunchConfiguration* launch = 0;
    kDebug() << launchName;
    foreach( KDevelop::LaunchConfiguration *l, core->runControllerInternal()->launchConfigurationsInternal() ) {
      kDebug() << l->name();

      if ( l->name() == launchName ) {
        launch = l;
      }
    }

    KDevelop::LaunchConfigurationType *type = 0;
    foreach( KDevelop::LaunchConfigurationType *t, core->runController()->launchConfigurationTypes() ) {
      kDebug() << t->id();

      if ( t->id() == "Native Application" ) {
        type = t;
        break;
      }
    }

    if ( !type ) {
      QTextStream qerr( stderr );
      qerr << endl << i18n( "Cannot find native launch configuration type" ) << endl;
      return 1;
    }

    if ( launch && launch->type()->id() != "Native Application" ) {
      launch = 0;
    }

    if ( launch && launch->launcherForMode( "debug" ) != args->getOption( "debug" ) ) {
      launch = 0;
    }

    if ( !launch ) {
      kDebug() << launchName << "not found, creating a new one";
      QPair<QString, QString> launcher;
      launcher.first = "debug";
      foreach( KDevelop::ILauncher *l, type->launchers() ) {
        if ( l->id() == args->getOption( "debug" ) ) {
          if ( l->supportedModes().contains( "debug" ) ) {
            launcher.second = l->id();
          }
        }
      }

      if ( launcher.second.isEmpty() ) {
        QTextStream qerr( stderr );
        qerr << endl << i18n( "Cannot find launcher %1" ).arg( args->getOption( "debug" ) ) << endl;
        return 1;
      }

      KDevelop::ILaunchConfiguration* ilaunch = core->runController()->createLaunchConfiguration( type, launcher, 0, launchName );

      launch = dynamic_cast<KDevelop::LaunchConfiguration*>( ilaunch );
    }

    type->configureLaunchFromCmdLineArguments( launch->config(), debugArgs );

    launch->config().writeEntry( "Break on Start", true );
    core->runControllerInternal()->setDefaultLaunch( launch );

    core->runControllerInternal()->execute( "debug", launch );

  } else {
    int count = args->count();

    for ( int i = 0; i < count; ++i ) {
      QString file = args->arg( i );
      //Allow opening specific lines in documents, like mydoc.cpp:10
      int lineNumberOffset = file.lastIndexOf( ':' );
      KTextEditor::Cursor line;

      if ( lineNumberOffset != -1 ) {
        bool isInt;
        int lineNr = file.mid( lineNumberOffset + 1 ).toInt( &isInt );

        if ( isInt ) {
          file = file.left( lineNumberOffset );
          line = KTextEditor::Cursor( lineNr, 0 );
        }
      }

      KUrl url( file );

      if ( url.isRelative() ) {
        url = KUrl( QDir::currentPath(), file );
      }

      if ( !core->documentController()->openDocument( url, line ) ) {
        kWarning() << i18n( "Could not open %1" ) << args->arg( i );
      }
    }

    args->clear();
  }

  return app.exec();
}

// kate: indent-mode cstyle; space-indent on; indent-width 2; replace-tabs on;  replace-tabs on;  replace-tabs on;
