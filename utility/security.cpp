/***************************************************************************
                          security.cpp  -  description
                             -------------------
    begin                : Thu Jun 24 11:22:12 2004
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
#include <qfile.h>
#include <qfileinfo.h>
#include <qstringlist.h>
#include <qtimer.h>

 //kde includes
#include <kdebug.h>
#include <klocale.h>
#include <kmdcodec.h>
#include <kmessagebox.h>
#include <kprocio.h>

 //app includes
#include "security.h"

Security::Security()
{
   m_keysRead = false;
   m_gpgRunning = false;
   readKeys();
}


Security::~Security()
{
}

void Security::readKeys()
{
    if (m_gpgRunning)
    {
      QTimer::singleShot(5, this, SLOT(readKeys()));
      return;
    }
    m_runMode = List;
    m_keys.clear();
    KProcIO *readProcess=new KProcIO();
    *readProcess << "gpg"<<"--no-secmem-warning"<<"--no-tty"<<"--with-colon"<<"--list-keys";
    connect(readProcess, SIGNAL(processExited(KProcess *)), this, SLOT(slotProcessExited()));
    connect(readProcess, SIGNAL(readReady(KProcIO *)) ,this, SLOT(slotDataArrived(KProcIO *)));
    if (!readProcess->start(KProcess::NotifyOnExit, true))
      KMessageBox::error(0L, i18n("<qt>Cannot start <i>gpg</i> and retrieve the available keys. Make sure that <i>gpg</i> is installed, otherwise verification of downloaded resources will not be possible.</qt>"));
    else
      m_gpgRunning = true;
}

void Security::slotProcessExited()
{
   switch (m_runMode)
   {
     case List: m_keysRead = true;
                      break;
     case Verify:
                      emit validityResult(m_result);
                      break;
   }
   m_gpgRunning = false;
/*
   QMap<QString, KeyStruct>::Iterator it;
   for (it = m_keys.begin(); it != m_keys.end(); ++it)
   {
       KeyStruct key = it.data();
       kdDebug(24000) << "Id: " << it.key() << " Name: " << key.name << " Mail: " << key.mail << " Trusted: " << key.trusted << endl;
   }*/
}

void Security::slotDataArrived(KProcIO *procIO)
{
  QString data;
  while (procIO->readln(data) != -1)
  {
     switch (m_runMode)
     {
        case List:
          if (data.startsWith("pub"))
          {
              KeyStruct key;
              QStringList line = QStringList::split(":", data, true);
              key.id = line[4];
              QString shortId = key.id.right(8);
              QString trustStr = line[1];
              key.trusted = false;
              if (trustStr == "u" || trustStr == "f")
                  key.trusted = true;
              data = line[9];
              key.mail=data.section('<', -1, -1);
              key.mail.truncate(key.mail.length() - 1);
              key.name=data.section('<',0,0);
              if (key.name.find("(")!=-1)
                  key.name=key.name.section('(',0,0);
              m_keys[shortId] = key;
          }
          break;
       case Verify:
          data = data.section("]",1,-1).stripWhiteSpace();
          if (data.startsWith("GOODSIG"))
          {
              m_result |= SIGNED_OK;
              QString id = data.section(" ", 1 , 1).right(8);
              if (!m_keys.contains(id))
              {
                  m_result |= UNKNOWN;
              } else
              {
                 m_signatureKey = m_keys[id];
              }
          } else
          if (data.startsWith("NO_PUBKEY"))
          {
              m_result |= UNKNOWN;
          } else
          if (data.startsWith("BADSIG"))
          {
              m_result |= SIGNED_BAD;
              QString id = data.section(" ", 1 , 1).right(8);
              if (!m_keys.contains(id))
              {
                  m_result |= UNKNOWN;
              } else
              {
                 m_signatureKey = m_keys[id];
              }
          } else
          if (data.startsWith("TRUST_ULTIMATE"))
            m_result |= TRUSTED;
          break;
     }
  }
}

void Security::checkValidity(const QString& filename)
{
  if (!m_keysRead)
  {
    emit validityResult(0); //other error
    return;
  }

  m_result = 0;
  m_runMode = Verify;
  QFileInfo f(filename);
  //check the MD5 sum
  QString md5sum;
  const char* c = "";
  KMD5 context(c);
  QFile file(filename);
  if (file.open(IO_ReadOnly))
  {
     context.reset();
     context.update(file);
     md5sum = context.hexDigest();
     file.close();
  }
  file.setName(f.dirPath() + "/md5sum");
  if (file.open(IO_ReadOnly))
  {
     QString md5sum_file;
     file.readLine(md5sum_file, 50);
     if (!md5sum.isEmpty() && !md5sum_file.isEmpty() && md5sum_file.startsWith(md5sum))
       m_result |= MD5_OK;
     file.close();
  }

  //verify the signature
  KProcIO *verifyProcess=new KProcIO();
  *verifyProcess<<"gpg"<<"--no-secmem-warning"<<"--status-fd=2"<<"--command-fd=0"<<"--verify" << f.dirPath() + "/signature"<< filename;
  connect(verifyProcess, SIGNAL(processExited(KProcess *)),this, SLOT(slotProcessExited()));
  connect(verifyProcess, SIGNAL(readReady(KProcIO *)),this, SLOT(slotDataArrived(KProcIO *)));
  if (verifyProcess->start(KProcess::NotifyOnExit,true))
      m_gpgRunning = true;
  else
  {
      KMessageBox::error(0L, i18n("<qt>Cannot start <i>gpg</i> and check the validity of the file. Make sure that <i>gpg</i> is installed, otherwise verification of downloaded resources will not be possible.</qt>"));
      emit validityResult(0);
  }
}

#include "security.moc"
