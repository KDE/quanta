/*
    KQApplication.cpp

    KNode, the KDE newsreader
    Copyright (c) 1999-2001 the KNode authors.
    See file AUTHORS for details

    Rewritten for Quanta Plus: (C) 2002, 2003 Andras Mantia <amantia@freemail.hu>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software Foundation,
    Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, US
*/

#include <qtimer.h>

#include <kwin.h>
#include <kcmdlineargs.h>
#include <kurl.h>
#include <kiconloader.h>
#include <kstandarddirs.h>
#include <kglobalsettings.h>

#include "quantacommon.h"
#include "project/project.h"
#include "quanta.h"
#include "kqapp.h"


QuantaApp *quantaApp = 0L; //global pointer to the main application object

KSplash::KSplash()
 : QFrame( 0L, QString("Quanta")+VERSION,
   QWidget::WStyle_NoBorder | QWidget::WStyle_Customize)
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

   if (args->isSet("logo")) splash = new KSplash();

   if (isRestored())
   {
     RESTORE(QuantaApp);
   }
   else
   {
     quantaApp = new QuantaApp();
     setMainWidget(quantaApp);
     QTimer::singleShot(10, this, SLOT( slotInit()));
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
}


KQUniqueApplication::~KQUniqueApplication()
{
}


int KQUniqueApplication::newInstance()
{
  args = KCmdLineArgs::parsedArgs();
  if (mainWidget())
  {
    KWin::setActiveWindow(mainWidget()->winId());
    quantaApp = static_cast<QuantaApp*>(mainWidget());
    for (int i = 0; i < args->count(); i++) 
    {
      quantaApp->slotFileOpen(args->url(i), quantaApp->defaultEncoding());  // load initial files
    }
  }
  else
  {
    splash = 0L;
    if (args->isSet("logo")) splash = new KSplash();

    quantaApp = new QuantaApp();
    setMainWidget(quantaApp);
    QTimer::singleShot(10, this, SLOT( slotInit()));
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
    for(QStringList::Iterator it = initialFiles.begin();it != initialFiles.end();++it)
    {
      KURL url;
      QuantaCommon::setUrl(url, *it);
      quantaApp->slotFileOpen(url, quantaApp->defaultEncoding());  // load initial files
    }    
    quantaApp->openLastFiles();
  }
  args->clear();
  if (splash) delete splash;
}

#include "kqapp.moc"
