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

#include "newstuff.moc"
