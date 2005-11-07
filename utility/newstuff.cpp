/***************************************************************************
                          newstuff.cpp  -  description
                             -------------------
    begin                : Tue Jun 22 12:19:55 2004
    copyright          : (C) 2004 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
 //qt includes
#include <qfileinfo.h>

//kde includes
#include <kglobal.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>
#include <ktar.h>

//app includes
#include "newstuff.h"
#include "dtds.h"
#include "resource.h"
#include "quantacommon.h"
#include "qextfileinfo.h"

void QNewDTEPStuff::installResource()
{
    bool ok = true;
    KTar tar(m_tarName, "application/x-gzip");
    if (tar.open(IO_ReadOnly))
    {
        const KArchiveDirectory *directory = tar.directory();
        QString dtepDir =KGlobal::dirs()->saveLocation("data") + resourceDir + "dtep/";
        QString dtdName = (*directory->entries().at(0));
        if (dtdName.isEmpty())
        {
          ok = false;
        } else
        {
          directory->copyTo(dtepDir, true);
          DTDs::ref()->slotLoadDTEP(dtepDir + dtdName, false);
        }
        tar.close();
    } else
        ok = false;
    if (!ok)
        KMessageBox::error(parentWidget(), i18n("There was an error with the downloaded DTEP tarball file. Possible causes are damaged archive or invalid directory structure in the archive."), i18n("DTEP Installation Error"));
}

QNewToolbarStuff::QNewToolbarStuff(const QString &type,  QWidget *parentWidget)
  :KNewStuffSecure(type, parentWidget)
{
  connect(this, SIGNAL(loadToolbarFile(const KURL&)), parentWidget, SLOT(slotLoadToolbarFile(const KURL&)));
}


void QNewToolbarStuff::installResource()
{
    KURL destURL = KURL::fromPathOrURL(KGlobal::dirs()->saveLocation("data") + resourceDir + "toolbars/" + QFileInfo(m_tarName).fileName());
    bool ok = true;
    if (QuantaCommon::checkOverwrite(destURL, parentWidget()))
    {
        if (!QExtFileInfo::copy(KURL::fromPathOrURL(m_tarName), destURL, -1, true, false, parentWidget()))
          ok = false;
        else
        {
           if (KMessageBox::questionYesNo(parentWidget(), i18n("Do you want to load the newly downloaded toolbar?"), i18n("Load Toolbar"), i18n("Load"), KStdGuiItem::cancel()) == KMessageBox::Yes)
           {
              emit loadToolbarFile(destURL);
           }
        }
        if (!ok)
            KMessageBox::error(parentWidget(), i18n("There was an error with the downloaded toolbar tarball file. Possible causes are damaged archive or invalid directory structure in the archive."), i18n("Toolbar Installation Error"));
     }
}

QNewTemplateStuff::QNewTemplateStuff(const QString &type,  QWidget *parentWidget)
  :KNewStuffSecure(type, parentWidget)
{
  connect(this, SIGNAL(openFile(const KURL&)), parentWidget, SLOT(slotFileOpen(const KURL&)));
}


void QNewTemplateStuff::installResource()
{
    KURL destURL = KURL::fromPathOrURL(KGlobal::dirs()->saveLocation("data") + resourceDir + "templates/" + QFileInfo(m_tarName).fileName());
    bool ok = true;
    if (QuantaCommon::checkOverwrite(destURL, parentWidget()))
    {
        if (!QExtFileInfo::copy(KURL::fromPathOrURL(m_tarName), destURL, -1, true, false, parentWidget()))
          ok = false;
        else
        {
           if (KMessageBox::questionYesNo(parentWidget(), i18n("Do you want to open the newly downloaded template?"), i18n("Open Template"), KStdGuiItem::open(), KStdGuiItem::cancel()) == KMessageBox::Yes)
           {
              emit openFile(destURL);
           }
        }
        if (!ok)
            KMessageBox::error(parentWidget(), i18n("There was an error with the downloaded template file."), i18n("Template Installation Error"));
     }
}

void QNewScriptStuff::installResource()
{
    bool ok = true;
    KTar tar(m_tarName, "application/x-gzip");
    if (tar.open(IO_ReadOnly))
    {
        const KArchiveDirectory *directory = tar.directory();
        QString scriptsDir =KGlobal::dirs()->saveLocation("data") + resourceDir + "scripts/";
        directory->copyTo(scriptsDir, true);
        tar.close();
    } else
        ok = false;

    if (!ok)
        KMessageBox::error(parentWidget(), i18n("There was an error with the downloaded script tarball file. Possible causes are damaged archive or invalid directory structure in the archive."), i18n("Script Installation Error"));
}

void QNewDocStuff::installResource()
{
  bool ok = true;
  KTar tar(m_tarName, "application/x-gzip");
  if (tar.open(IO_ReadOnly))
  {
    const KArchiveDirectory *directory = tar.directory();
    QString docDir =KGlobal::dirs()->saveLocation("data") + resourceDir + "doc/";
    directory->copyTo(docDir, true);
    tar.close();
  } else
    ok = false;

    if (!ok)
      KMessageBox::error(parentWidget(), i18n("There was an error with the downloaded script tarball file. Possible causes are damaged archive or invalid directory structure in the archive."), i18n("Documentation Installation Error"));
}

#include "newstuff.moc"
