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


int main( int argc, char *argv[] )
{
  #include "shared_app_init.cpp"

  //We copy the original argv here, as it seems that KCmdLineArgs changes the arguments ("--style" becomes "-style")
  QList<QByteArray> argvOrig;

  for ( int a = 0; a < argc; ++a )
    argvOrig << argv[a];

  KCmdLineArgs::init( argc, argv, &aboutData );

  options.add( "cs <name>", ki18n( "Create new session with given name." ) );

  options.add( "s <session>", ki18n( "Session to load. You can pass either hash or the name of the session." ) );

  options.add( "sessions", ki18n( "List available sessions and quit" ) );

  KCmdLineArgs::addCmdLineOptions( options );

  KCmdLineArgs* args = KCmdLineArgs::parsedArgs();

  KApplication app;

  if ( args->isSet( "sessions" ) ) {
    QTextStream qout( stdout );
    qout << endl << ki18n( "Available sessions (use '-s HASH' or '-s NAME' to open a specific one):" ).toString() << endl << endl;
    qout << QString( "%1" ).arg( ki18n( "Hash" ).toString(), -38 ) << '\t' << ki18n( "Name: Opened Projects" ).toString() << endl;
    foreach( const KDevelop::SessionInfo& si, KDevelop::SessionController::availableSessionInfo() ) {
      if ( si.name.isEmpty() && si.projects.isEmpty() ) {
        continue;
      }

      qout << si.uuid.toString() << '\t' << si.description;

      if ( !KDevelop::SessionController::tryLockSession( si.uuid.toString() ) )
        qout << "     " << i18n( "[running]" );

      qout << endl;
    }

    return 0;
  }

  // if empty, restart kdevelop with last active session, see SessionController::defaultSessionId
  QString session;

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

    session = i18n( "Debug" ) + " " + binary;

  } else if ( args->isSet( "cs" ) ) {
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

    if ( !KDevelop::SessionController::tryLockSession( KDevelop::SessionController::defaultSessionId( session ) ) ) {
      QTextStream qerr( stderr );
      qerr << endl << i18n( "Session %1 is already running.", session ) << endl;
      return 1;
    }
  }

  ///Manage sessions: There always needs a KDEV_SESSION to be set, so the duchain can be stored in the session-specific directory
  session = KDevelop::SessionController::defaultSessionId( session );

  //if session is already running create a new one
  //if the user explicitely picked a already running session using the -s switch he already
  //gets got an error
  if ( !KDevelop::SessionController::tryLockSession( session ) ) {
    session = QUuid::createUuid();
  }

#ifdef Q_WS_WIN
  QFileInfo fi( QFileInfo( QApplication::applicationFilePath() ).path() + "/quanta.bin.exe" );

#else
  QFileInfo fi( QFileInfo( QApplication::applicationFilePath() ).path() + "/quanta.bin" );

#endif
  if ( !fi.exists() ) {
    QTextStream qerr( stderr );
    qerr << endl << i18n( "Cannot start Quanta, the Quanta.bin executable is missing in %1. Please fix your Quanta installation.", fi.absolutePath() ) << endl;
    return -1;
  }

  //Forward all arguments, except -s as the internal app doesn't setup -s or --sessions arguments
  QList<QByteArray> binArgs;

  for ( int a = 1; a < argvOrig.count(); ++a ) {
    // note: KCmdLineArgs does not care whether an option gets passed as with one or two dashes
    if ( argvOrig[a] == "-s" || argvOrig[a] == "--s" || argvOrig[a] == "--cs" || argvOrig[a] == "-cs" ) {
      ++a;

    } else if ( argvOrig[a] != "--sessions" && argvOrig[a] != "-sessions" ) {
      binArgs << argvOrig[a];
    }
  }

#ifdef Q_WS_WIN
  KProcess proc;

  proc << fi.absoluteFilePath();

  foreach( const QString &arg, binArgs ) {
    proc << arg;
  }

  proc.setEnv( "KDEV_SESSION", session );

  return proc.execute();
#else
  QByteArray pathData = fi.absoluteFilePath().toLocal8Bit();

  char **cmd = 0;
  cmd = ( char** ) realloc( cmd, ( binArgs.count() + 2 ) * sizeof( char* ) );
  cmd[0] = pathData.data();

  for ( int i = 0; i < binArgs.length(); ++i ) {
    cmd[i+1] = binArgs[i].data();
  }

  cmd[binArgs.count() + 1] = 0;

  QByteArray sessionData = session.toLocal8Bit();

  setenv( "KDEV_SESSION", sessionData.data(), true );
  return execv( qPrintable( fi.absoluteFilePath() ), cmd );
#endif
}
// kate: indent-mode cstyle; space-indent on; indent-width 2; replace-tabs on;  replace-tabs on;
