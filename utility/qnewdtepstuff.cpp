/***************************************************************************
                          qnewdtepstuff.cpp  -  description
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
#include <kio/netaccess.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>
#include <ktar.h>
#include <ktempdir.h>

//app includes
#include "qnewdtepstuff.h"
#include "dtds.h"
#include "resource.h"
#include "security.h"

QNewDTEPStuff::QNewDTEPStuff(const QString &type,  QWidget *parentWidget)
 : KNewStuff(type, parentWidget)
{
  connect(Security::ref(), SIGNAL(validityResult(int)), this, SLOT(slotValidated(int)));
  m_tempDir = 0L;
}


QNewDTEPStuff::~QNewDTEPStuff()
{
}

bool QNewDTEPStuff::install(const QString &fileName)
{
  bool ok = true;

  m_tempDir = new KTempDir(tmpDir);
  m_tempDir->setAutoDelete(true);
  KTar tar(fileName, "application/x-gzip");
  if (tar.open(IO_ReadOnly))
  {
      const KArchiveDirectory *directory = tar.directory();
      directory->copyTo(m_tempDir->name(), true);
      m_tarName = "";
      QStringList entries = directory->entries();
      for (QStringList::Iterator it = entries.begin(); it != entries.end(); ++it)
      {
        if (*it != "signature" && *it != "md5sum")
        {
          m_tarName = *it;
          break;
        }
      }
      tar.close();
      if (m_tarName.isEmpty())
        ok = false;
      else
      {
         m_tarName.prepend(m_tempDir->name());
         Security::ref()->checkValidity(m_tarName);
      }
  } else
      ok = false;
  if (!ok)
    KMessageBox::error(parentWidget(), i18n("There was an error with the downloaded DTEP tarball file. Possible causes are damaged archive or invalid directory structure in the archive."), i18n("DTEP Installation Error"));
  return ok;
}

void QNewDTEPStuff::slotValidated(int result)
{
   QString errorString;
   QString signatureStr;
   bool valid = true;
   if (result == 0)
   {
       errorString ="<br>-    " +  i18n("The validation failed for unknown reason.");
       valid = false;
   } else
   {
      KeyStruct key = Security::ref()->signatureKey();
      if (!(result & Security::MD5_OK ))
      {
          errorString = "<br>-    " + i18n("The MD5SUM check failed, the archive might be broken.");
          valid = false;
      }
      if (result & Security::SIGNED_BAD)
      {
          errorString += "<br>-    " + i18n("The signature is bad, the archive might be broken or altered.");
          valid = false;
      }
      if (result & Security::SIGNED_OK)
      {
         if (result & Security::TRUSTED)
         {
            kdDebug() << "Signed and trusted " << endl;
         } else
         {
            errorString += "<br>-    " + i18n("The signature is valid, but untrusted.");
            valid = false;
         }
      }
      if (result & Security::UNKNOWN)
      {
          errorString += "<br>-    " + i18n("The signature is unknown.");
          valid = false;
      } else
      {
          signatureStr = i18n("The resource was signed with key <i>0x%1</i>, belonging to <i>%2 &lt;%3&gt;</i>.").arg(key.id.right(8)).arg(key.name).arg(key.mail);
      }
   }
  if (!valid)
  {
      signatureStr.prepend( "<br>");
      if (KMessageBox::warningYesNo(parentWidget(), i18n("<qt>There is a problem with the DTEP resource file you have downloaded. The errors are :<b>%1</b><br>%2<br><br>Installation of the resource is <b>not recommended</b>.<br><br>Do you want to proceed with the installation?</qt>").arg(errorString).arg(signatureStr), i18n("Problematic Resource File")) == KMessageBox::Yes)
          valid = true;
  } else
    KMessageBox::information(parentWidget(), i18n("<qt>%1<br><br>Press OK to install it.</qt>").arg(signatureStr), i18n("Valid Resource"), "Show Valid Signature Information");
  if (valid)
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
    KIO::NetAccess::del(KURL().fromPathOrURL(m_tempDir->name()), parentWidget());
    delete m_tempDir;
    m_tempDir = 0L;
}

bool QNewDTEPStuff::createUploadFile(const QString &fileName)
{
  Q_UNUSED(fileName);
  return true; //upload is not supported yet
}

#include "qnewdtepstuff.moc"
