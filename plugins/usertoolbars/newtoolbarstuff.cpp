/***************************************************************************
    begin                : Thu May 19 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

//own includes
#include "newtoolbarstuff.h"
#include "extfileinfo.h"

//qt includes
#include <QFileInfo>

//kde includes
#include <kio/netaccess.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>


NewToolbarStuff::NewToolbarStuff(const QString &type,  QWidget *parentWidget)
  :KNewStuffSecure(type, parentWidget)
{
  connect(this, SIGNAL(loadToolbarFile(const KUrl&)), parentWidget, SLOT(slotLoadToolbarFile(const KUrl&)));
}


void NewToolbarStuff::installResource()
{
    KUrl destURL = KUrl::fromPathOrUrl(KGlobal::dirs()->saveLocation("data") + "kdevusertoolbars/toolbars/" + QFileInfo(m_tarName).fileName());
    bool ok = true;
    if (ExtFileInfo::checkOverwrite(destURL))
    {
        if (!ExtFileInfo::copy(KUrl::fromPathOrUrl(m_tarName), destURL, -1, true, false, parentWidget()))
          ok = false;
        else
        {
           if (KMessageBox::questionYesNo(parentWidget(), i18n("Do you want to load the newly downloaded toolbar?"), i18n("Load Toolbar")) == KMessageBox::Yes)
           {
              emit loadToolbarFile(destURL);
           }
        }
        if (!ok)
            KMessageBox::error(parentWidget(), i18n("There was an error with the downloaded toolbar tarball file. Possible causes are damaged archive or invalid directory structure in the archive."), i18n("Toolbar Installation Error"));
     }
}
#include "newtoolbarstuff.moc"
