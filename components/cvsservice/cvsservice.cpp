/***************************************************************************
                                              cvsservice.cpp  -  description
                                             ------------------------------
    begin                : Sun May 16 17:50:25 2004
    copyright          : (C) 2004 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2
 *
 ***************************************************************************/

//qt include
#include <qpopupmenu.h>

//kde includes
#include <dcopref.h>
#include <kaction.h>
#include <klocale.h>
#include <kmessagebox.h>

// CvsService
#include <repository_stub.h>
#include <cvsservice_stub.h>
#include <cvsjob_stub.h>

#include "cvsservice.h"
#include "viewmanager.h"
#include "document.h"

CVSService::CVSService(KActionCollection *ac)
{
  m_menu = new QPopupMenu;
  KAction *action = new KAction(i18n("&Update"), 0, this, SLOT(slotUpdate()), ac);
  action->plug(m_menu);
  m_cvsJob = 0L;
  m_repository = 0L;
  m_cvsService =0L;
}

CVSService::~CVSService()
{
  if (m_cvsService)
    m_cvsService->quit();
  delete m_menu;
  delete m_repository;
  m_repository = 0L;
  m_cvsService = 0L;
}

void CVSService::setAppId(const QCString &id)
{
  m_appId = id;
  if (m_cvsService)
    m_cvsService->quit();
  delete m_cvsService;
  m_cvsService = new CvsService_stub(m_appId, "CvsService");
}

void CVSService::setRepository(const QString &repository)
{
   delete m_repository;
   m_repository = new Repository_stub(m_appId, "CvsRepository");
   if (m_repository->setWorkingCopy(repository))
   {
      m_repositoryPath = repository;
      if (!m_repositoryPath.endsWith("/"))
        m_repositoryPath += "/";
   }
}

void CVSService::slotUpdate()
{
  QStringList files;
  if (!m_defaultFile.isEmpty())
  {
     if (m_defaultFile.startsWith(m_repositoryPath))
     {
       files += m_defaultFile.remove(m_repositoryPath);
       slotUpdate(files);
     } else
     {
       emit clearMessages();
       emit showMessage(i18n("Error: \"%1\" is not part of the\n\"%2\" repository.").arg(m_defaultFile).arg(m_repositoryPath), false);
     }
  }
}

void CVSService::slotUpdate(const QStringList &files)
{
   if (m_repository && !m_appId.isEmpty())
   {
      emit clearMessages();
      DCOPRef job = m_cvsService->update(files, true, true, true, "");
      m_cvsJob = new CvsJob_stub( job.app(), job.obj() );

      connectDCOPSignal(job.app(), job.obj(), "jobExited(bool, int)", "slotJobExited(bool, int)", true);
      connectDCOPSignal(job.app(), job.obj(), "receivedStdout(QString)", "slotReceivedStdout(QString)", true);
      connectDCOPSignal(job.app(), job.obj(), "receivedStderr(QString)", "slotReceivedStdout(QString)", true);
      m_cvsJob->execute();
   }
}

void CVSService::slotJobExited(bool normalExit, int exitStatus)
{
    delete m_cvsJob;
    m_cvsJob = 0L;
    if (!normalExit)
    {
        KMessageBox::sorry(0, i18n("<qt>The CVS command has failed. The error was <i>%1</i>.</qt>").arg(exitStatus), i18n("Command Failed"));
        return;
    }
}

void CVSService::slotReceivedStdout(QString output)
{
   emit showMessage(output, false);
}

 #include "cvsservice.moc"
