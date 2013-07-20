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
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "version.h"

#include "kdevquantaextension.h"

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

#include <shell/sessioncontroller.h>

#include <KMessageBox>
#include <KProcess>

#include <iostream>
#include <QtCore/QTextStream>

#include <QSessionManager>
#include <shell/core.h>
#include <shell/sessioncontroller.h>
#include <interfaces/isession.h>
#include <interfaces/iprojectcontroller.h>
#include <interfaces/idocumentcontroller.h>

#include <shell/runcontroller.h>
#include <shell/launchconfiguration.h>
#include <interfaces/ilauncher.h>
#include <interfaces/iproject.h>

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
  static const char description[] =
    I18N_NOOP("Quanta Plus Web Development Environment");

  static const char othertext[] =
    I18N_NOOP("Our goal is to be nothing less than the best possible tool for\n \
    working with tagging and scripting languages.\
    \n\nQuanta Plus is not in any way affiliated with any commercial\n \
    versions of Quanta. \
    \n\nWe hope you enjoy Quanta Plus.\n\n");

  KAboutData aboutData("quanta", 0, ki18n("Quanta"),
                        QUANTA_VERSION, ki18n(description), KAboutData::License_GPL,
                        ki18n("(c) 1999-2010, The Quanta+ developers"), ki18n(othertext), "http://www.kdewebdev.org");

  aboutData.addAuthor(ki18n("Eric Laffoon"),ki18n("Project Lead - public liaison"), "sequitur@kde.org");
  aboutData.addAuthor(ki18n("Andras Mantia"),ki18n("Program Lead - bug squisher"), "amantia@kde.org");
  aboutData.addCredit(ki18n("Jens Herden"),
                      ki18n("Invaluable member - Help with code cleanup, porting to KDevelop and KDE4, etc."),
                      "jens@kdewebdev.org");

  KCmdLineArgs::init( argc, argv, &aboutData );
  KCmdLineOptions options;
  options.add("project <project>", ki18n( "Url to project to load" ));
  options.add("+files", ki18n( "Files to load" ));

  KCmdLineArgs::init( argc, argv, &aboutData );

  options.add( "cs <name>", ki18n( "Create new session with given name." ) );

  options.add( "s <session>", ki18n( "Session to load. You can pass either hash or the name of the session." ) );

  options.add( "sessions", ki18n( "List available sessions and quit" ) );

  KCmdLineArgs::addCmdLineOptions( options );

  KCmdLineArgs* args = KCmdLineArgs::parsedArgs();

  QuantaApplication app;

  if ( args->isSet( "sessions" ) ) {
    QTextStream qout( stdout );
    qout << endl << ki18n( "Available sessions (use '-s HASH' or '-s NAME' to open a specific one):" ).toString() << endl << endl;
    qout << QString( "%1" ).arg( ki18n( "Hash" ).toString(), -38 ) << '\t' << ki18n( "Name: Opened Projects" ).toString() << endl;
    foreach( const KDevelop::SessionInfo& si, KDevelop::SessionController::availableSessionInfo() ) {
      if ( si.name.isEmpty() && si.projects.isEmpty() ) {
        continue;
      }

      qout << si.uuid.toString() << '\t' << si.description;

      if ( !KDevelop::SessionController::tryLockSession( si.uuid.toString() ).lock )
        qout << "     " << i18n( "[running]" );

      qout << endl;
    }

    return 0;
  }

  // if empty, restart kdevelop with last active session, see SessionController::defaultSessionId
  QString session;

  if ( args->isSet( "cs" ) ) {
    session = args->getOption( "cs" );
    foreach( const KDevelop::SessionInfo& si, KDevelop::SessionController::availableSessionInfo() ) {
      if ( session == si.name ) {
        QTextStream qerr( stderr );
        qerr << endl << i18n( "A session with the name %1 exists already. Use the -s switch to open it.", session ) << endl;
        return 1;
      }
    }

    // session doesn't exist, we can create it

  } else if ( args->isSet( "s" ) ) {
    session = args->getOption( "s" );
    bool found = false;
    foreach( const KDevelop::SessionInfo& si, KDevelop::SessionController::availableSessionInfo() ) {
      if ( session == si.name || session == si.uuid.toString() ) {
        found = true;
        break;
      }
    }

    if ( !found ) {
      QTextStream qerr( stderr );
      qerr << endl << i18n( "Cannot open unknown session %1. See --sessions switch for available sessions or use -cs to create a new one.", session ) << endl;
      return 1;
    }
  }

  KDevQuantaExtension::init();

  KSplashScreen* splash = 0;

  QString splashFile = KStandardDirs::locate( "appdata", "pics/quanta-splash.png" );

  if ( !splashFile.isEmpty() ) {
    QPixmap pm;
    pm.load( splashFile );
    splash = new KSplashScreen( pm );
    splash->show();
  }

  if ( !KDevelop::Core::initialize( splash, KDevelop::Core::Default, session ) ) {
    return 5;
  }

  KGlobal::locale()->insertCatalog( KDevelop::Core::self()->componentData().catalogName() );

  KDevelop::Core* core = KDevelop::Core::self();

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

  return app.exec();
}
// kate: indent-mode cstyle; space-indent on; indent-width 2; replace-tabs on;  replace-tabs on;
