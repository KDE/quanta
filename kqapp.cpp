/*
    KQApplication.cpp

    KNode, the KDE newsreader
    Copyright (c) 1999-2001 the KNode authors.
    See file AUTHORS for details

    Rewritten for Quanta Plus: (C) 2002, 2003 Andras Mantia <amantia@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software Foundation,
    Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, US
*/

#include <qtimer.h>

#include <kdebug.h>
#include <kwin.h>
#include <kcmdlineargs.h>
#include <kurl.h>
#include <kiconloader.h>
#include <kstandarddirs.h>
#include <kglobalsettings.h>
#include <dcopclient.h>

#include "quantacommon.h"
#include "project/project.h"
#include "quanta.h"
#include "kqapp.h"

 
QuantaApp *quantaApp = 0L; //global pointer to the main application object
const QString resourceDir = QString(PACKAGE) + "/";

KSplash::KSplash()
 : QFrame( 0L, QString("Quanta")+VERSION,
   QWidget::WStyle_NoBorder | QWidget::WStyle_Customize | WX11BypassWM)
{
   QPixmap pm( UserIcon("quantalogo") );

   setBackgroundPixmap(pm);
#if KDE_VERSION >= KDE_MAKE_VERSION(3,1,90)
  QRect desk = KGlobalSettings::splashScreenDesktopGeometry();
  setGeometry( desk.center().x()-225, desk.center().y()-150, 450, 300 );
#else
  setGeometry( QApplication::desktop()->width ()/2-225,
                QApplication::desktop()->height()/2-150,
                450, 300 );
#endif
   setLineWidth(0);
   show();
}


KSplash::~KSplash()
{
}

KQApplication::KQApplication()
 : KApplication()
{
   args = KCmdLineArgs::parsedArgs();
   splash = 0L;

   KGlobal::dirs()->addPrefix(PREFIX);
   dcopClient()->registerAs("quanta", true);

//FIXME: Do not hide the problem, but really fix the session restoration.
//I have no idea how to do it, but it may be broken because of delayed
//initialization in QuantaApp
/*   if (isRestored())
   {
     RESTORE(QuantaApp);
   }
   else */
   {
     quantaApp = new QuantaApp();
     KConfig *config = quantaApp->config();
     config->setGroup("General Options");
     if (config->readBoolEntry("Show Splash", true) && args->isSet("logo"))
     {
       splash = new KSplash();
       connect(quantaApp, SIGNAL(showSplash(bool)), splash, SLOT(setShown(bool)));
     }
     setMainWidget(quantaApp);
     slotInit();
   }
}

void KQApplication::slotInit()
{
  KQApplicationPrivate::init();
}

KQApplication::~KQApplication()
{
}

KQUniqueApplication::KQUniqueApplication()
 : KUniqueApplication()
{
  KGlobal::dirs()->addPrefix(PREFIX);
  dcopClient()->registerAs("quanta", false);
}


KQUniqueApplication::~KQUniqueApplication()
{
}


int KQUniqueApplication::newInstance()
{
  args = KCmdLineArgs::parsedArgs();
  if (mainWidget())
  {
#if KDE_VERSION < KDE_MAKE_VERSION( 3, 1, 90 )
	KWin::setActiveWindow( mainWidget()->winId() );
#else
	KWin::activateWindow( mainWidget()->winId() );
#endif
	  
    quantaApp = static_cast<QuantaApp*>(mainWidget());
    for (int i = 0; i < args->count(); i++)
    {
      quantaApp->slotFileOpen(args->url(i), quantaApp->defaultEncoding());  // load initial files
    }
  }
  else
  {
    splash = 0L;
    quantaApp = new QuantaApp();
    KConfig *config = quantaApp->config();
    config->setGroup("General Options");
    if (config->readBoolEntry("Show Splash", true) && args->isSet("logo"))
    {
      splash = new KSplash();
      connect(quantaApp, SIGNAL(showSplash(bool)), splash, SLOT(setShown(bool)));
    }
    setMainWidget(quantaApp);
    slotInit();
  }

  return 0;
}

void KQUniqueApplication::slotInit()
{
  KQApplicationPrivate::init();
}

void KQApplicationPrivate::init()
{
  if (quantaApp->quantaStarted)
  {
    quantaApp->initQuanta();
    quantaApp->show();

    QString initialProject;
    QStringList initialFiles;
    for (int i = 0; i < args->count(); i++ )
    {
      QString arg = args->url(i).url();

      if(arg.findRev(QRegExp(".+\\.webprj")) != -1)
        initialProject = arg;
      else
        initialFiles += arg;
    }
    quantaApp->loadInitialProject(initialProject);
    //recoverCrashed manages the autosaved copies
    quantaApp->recoverCrashed(initialFiles);

    for(QStringList::Iterator it = initialFiles.begin();it != initialFiles.end();++it)
    {
      KURL url;
      QuantaCommon::setUrl(url, (*it));
      quantaApp->slotFileOpen(url, quantaApp->defaultEncoding());  // load initial files
    }
    quantaApp->openLastFiles();
  }
  args->clear();
  delete splash;
}

#include "kqapp.moc"
