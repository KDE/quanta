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

#include <kconfig.h>
#include <kdebug.h>
#include <kwin.h>
#include <kcmdlineargs.h>
#include <kurl.h>
#include <kiconloader.h>
#include <kstandarddirs.h>
#include <kglobalsettings.h>
#include <ksplashscreen.h>
#include <dcopclient.h>

#include "config.h"
#include "quantacommon.h"
#include "project.h"
#include "quanta.h"
#include "quanta_init.h"
#include "kqapp.h"

QuantaApp *quantaApp = 0L; //global pointer to the main application object

KSplash::KSplash()
 : QFrame( 0L, QString("Quanta")+QUANTA_VERSION,
   QWidget::WStyle_NoBorder | QWidget::WStyle_Customize | WX11BypassWM)
{
   QPixmap pm( UserIcon("quantalogo_be") );

   setBackgroundPixmap(pm);
  QRect desk = KGlobalSettings::splashScreenDesktopGeometry();
  setGeometry( desk.center().x()-225, desk.center().y()-150, 450, 300 );
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
     KConfig *config = kapp->config();
     config->setGroup("General Options");
     int mdiMode = config->readNumEntry("MDI mode", KMdi::IDEAlMode);
     QString layout = config->readEntry("Window layout", "Default");
     if (layout == "Default")
         mdiMode = KMdi::IDEAlMode;
     quantaApp = new QuantaApp(mdiMode);
     config->setGroup("General Options");
    if (config->readBoolEntry("Show Splash", true) && args->isSet("logo"))
     {
       splash = new KSplash();
//      sp = new KSplashScreen(UserIcon("quantalogo_be"));
//      sp->show();
       connect(quantaApp, SIGNAL(showSplash(bool)), splash, SLOT(setShown(bool)));
       QTimer::singleShot(10*1000, this, SLOT(slotSplashTimeout()));
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

void KQApplication::slotSplashTimeout()
{
  delete splash;
  delete sp;
  splash = 0L;
  sp = 0L;
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
    KWin::activateWindow( mainWidget()->winId() );
    quantaApp = static_cast<QuantaApp*>(mainWidget());
    for (int i = 0; i < args->count(); i++)
    {
      quantaApp->slotFileOpen(args->url(i), quantaApp->defaultEncoding());  // load initial files
    }
  }
  else
  {
    splash = 0L;
    KConfig *config = kapp->config();
    config->setGroup("General Options");
    int mdiMode = config->readNumEntry("MDI mode", KMdi::IDEAlMode);
    QString layout = config->readEntry("Window layout", "Default");
    if (layout == "Default")
        mdiMode = KMdi::IDEAlMode;
    quantaApp = new QuantaApp(mdiMode);
    config->setGroup("General Options");
    if (config->readBoolEntry("Show Splash", true) && args->isSet("logo"))
    {
      splash = new KSplash();
      connect(quantaApp, SIGNAL(showSplash(bool)), splash, SLOT(setShown(bool)));
      QTimer::singleShot(10*1000, this, SLOT(slotSplashTimeout()));
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

void KQUniqueApplication::slotSplashTimeout()
{
  delete splash;
  delete sp;
  splash = 0L;
  sp = 0L;
}


void KQApplicationPrivate::init()
{
  if (quantaApp->quantaStarted)
  {
    quantaApp->enableIdleTimer(false);
    quantaApp->m_quantaInit->initQuanta();
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
    quantaApp->m_quantaInit->loadInitialProject(initialProject);
    //recoverCrashed manages the autosaved copies
    quantaApp->m_quantaInit->recoverCrashed(initialFiles);

    for(QStringList::Iterator it = initialFiles.begin();it != initialFiles.end();++it)
    {
      KURL url;
      QuantaCommon::setUrl(url, (*it));
      quantaApp->slotFileOpen(url, quantaApp->defaultEncoding());  // load initial files
    }
    quantaApp->m_quantaInit->openLastFiles();
  }
  args->clear();
  delete splash;
  splash = 0L;
  delete sp;
  sp = 0L;
  kdDebug(24000) << "Delete splash" << endl;
  delete quantaApp->m_quantaInit;
  quantaApp->m_quantaInit = 0L;
  quantaApp->enableIdleTimer(true);
}

#include "kqapp.moc"
