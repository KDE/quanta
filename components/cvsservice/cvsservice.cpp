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
#include <qtextedit.h>

//kde includes
#include <dcopref.h>
#include <kaction.h>
#include <kcombobox.h>
#include <klistbox.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kdeversion.h>

// CvsService
#include <repository_stub.h>
#include <cvsservice_stub.h>
#include <cvsjob_stub.h>

#include "cvsservice.h"
#include "cvscommitdlgs.h"

CVSService::CVSService(KActionCollection *ac)
{
  m_menu = new QPopupMenu;
#if KDE_VERSION < KDE_MAKE_VERSION(3,2,90)
  KAction *action = new KAction(i18n("&Commit"), 0, this, SLOT(slotCommit()), ac);
#else  
  KAction *action = new KAction(i18n("&Commit"), "vcs_commit", 0, this, SLOT(slotCommit()), ac);
#endif  
  action->plug(m_menu);
#if KDE_VERSION < KDE_MAKE_VERSION(3,2,90)
  action = new KAction(i18n("&Update"), 0, this, SLOT(slotUpdate()), ac);
#else  
  action = new KAction(i18n("&Update"), "vcs_update", 0, this, SLOT(slotUpdate()), ac);
#endif  
  action->plug(m_menu);
  m_cvsJob = 0L;
  m_repository = 0L;
  m_cvsService =0L;
  m_commitDlg = new CVSCommitDlgS();
}

CVSService::~CVSService()
{
  if (m_cvsService)
    m_cvsService->quit();
  delete m_cvsService;
  delete m_menu;
  delete m_repository;
  m_repository = 0L;
  m_cvsService = 0L;
  delete m_commitDlg;
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
         notInRepository();
     }
  }
}

void CVSService::slotUpdate(const QStringList &files)
{
   if (m_repository && !m_appId.isEmpty())
   {
      emit clearMessages();
      m_files = files;
      DCOPRef job = m_cvsService->update(files, true, true, true, "");
      m_cvsCommand = "update";
      m_cvsJob = new CvsJob_stub( job.app(), job.obj() );

      connectDCOPSignal(job.app(), job.obj(), "jobExited(bool, int)", "slotJobExited(bool, int)", true);
      connectDCOPSignal(job.app(), job.obj(), "receivedStdout(QString)", "slotReceivedStdout(QString)", true);
      connectDCOPSignal(job.app(), job.obj(), "receivedStderr(QString)", "slotReceivedStderr(QString)", true);
      m_cvsJob->execute();
   }
}

void CVSService::slotCommit()
{
  QStringList files;
  if (!m_defaultFile.isEmpty())
  {
     if (m_defaultFile.startsWith(m_repositoryPath))
     {
         files += m_defaultFile.remove(m_repositoryPath);
         slotCommit(files);
     } else
     {
         notInRepository();
     }
  }
}

void CVSService::slotCommit(const QStringList &files)
{
   m_commitDlg->fileList->clear();
   m_commitDlg->fileList->insertStringList(files);
   m_commitDlg->logEdit->clear();

   if (m_repository && !m_appId.isEmpty() && m_commitDlg->exec())
   {
      QString message = m_commitDlg->logEdit->text();
      if (message != m_commitDlg->messageCombo->currentText())
          m_commitDlg->messageCombo->insertItem(message, 0);
      emit clearMessages();
      m_files = files;
      DCOPRef job = m_cvsService->commit(files, message, true);
      m_cvsCommand = "commit";
      m_cvsJob = new CvsJob_stub( job.app(), job.obj() );

      connectDCOPSignal(job.app(), job.obj(), "jobExited(bool, int)", "slotJobExited(bool, int)", true);
      connectDCOPSignal(job.app(), job.obj(), "receivedStdout(QString)", "slotReceivedStdout(QString)", true);
      connectDCOPSignal(job.app(), job.obj(), "receivedStderr(QString)", "slotReceivedStderr(QString)", true);
      m_cvsJob->execute();
   }
}

void CVSService::slotJobExited(bool normalExit, int exitStatus)
{
    if (!normalExit)
    {
        KMessageBox::sorry(0, i18n("<qt>The CVS command <b>%1</b>has failed. The error code was <i>%2</i>.</qt>").arg(m_cvsCommand).arg(exitStatus), i18n("Command Failed"));
    }
    if (exitStatus == 0)
    {
        emit commandExecuted(m_cvsCommand, m_files);
    }
    delete m_cvsJob;
    m_cvsJob = 0L;
}

void CVSService::slotReceivedStdout(QString output)
{
   emit showMessage(output, false);
}

void CVSService::slotReceivedStderr(QString output)
{
   emit showMessage(output, false);
}

void CVSService::notInRepository()
{
    emit clearMessages();
    emit showMessage(i18n("Error: \"%1\" is not part of the\n\"%2\" repository.").arg(m_defaultFile).arg(m_repositoryPath), false);
}

 #include "cvsservice.moc"
