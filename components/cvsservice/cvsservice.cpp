/***************************************************************************
                                              cvsservice.cpp  -  description
                                             ------------------------------
    begin                : Sun May 16 17:50:25 2004
    copyright          : (C) 2004, 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2
 *
 ***************************************************************************/

//qt include
#include <qfile.h>
#include <qfileinfo.h>
#include <qradiobutton.h>
#include <qtextedit.h>
#include <qtextstream.h>

//kde includes
#include <kaction.h>
#include <kcombobox.h>
#include <kdeversion.h>
#include <kiconloader.h>
#include <klineedit.h>
#include <klistbox.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kpopupmenu.h>

// CvsService
#include <repository_stub.h>
#include <cvsservice_stub.h>
#include <cvsjob_stub.h>

#include "cvsservice.h"
#include "cvscommitdlgs.h"
#include "cvsupdatetodlgs.h"

CVSService::CVSService(KActionCollection *ac)
{
  m_menu = new KPopupMenu();
#if KDE_VERSION < KDE_MAKE_VERSION(3,2,90)
  KAction *action = new KAction(i18n("&Commit..."), 0, this, SLOT(slotCommit()), ac);
#else
  KAction *action = new KAction(i18n("&Commit..."), "vcs_commit", 0, this, SLOT(slotCommit()), ac);
#endif
  action->plug(m_menu);
#if KDE_VERSION < KDE_MAKE_VERSION(3,2,90)
  action = new KAction(i18n("&Update"), 0, this, SLOT(slotUpdate()), ac);
#else
  action = new KAction(i18n("&Update"), "vcs_update", 0, this, SLOT(slotUpdate()), ac);
#endif
  action->plug(m_menu);
  KPopupMenu *updateToMenu = new KPopupMenu(m_menu);
#if KDE_VERSION < KDE_MAKE_VERSION(3,2,90)
  m_menu->insertItem(i18n("Update &To"), updateToMenu);
  action = new KAction(i18n("&Tag/Date..."), 0, this, SLOT(slotUpdateToTag()), ac);
#else
  m_menu->insertItem(SmallIconSet("vcs_update"), i18n("Update &To"), updateToMenu);
  action = new KAction(i18n("&Tag/Date..."), "vcs_update", 0, this, SLOT(slotUpdateToTag()), ac);
#endif
  action->plug(updateToMenu);
#if KDE_VERSION < KDE_MAKE_VERSION(3,2,90)
  action = new KAction(i18n("&HEAD"), 0, this, SLOT(slotUpdateToHead()), ac);
#else
  action = new KAction(i18n("&HEAD"), "vcs_update", 0, this, SLOT(slotUpdateToHead()), ac);
#endif
  action->plug(updateToMenu);
  action = new KAction(i18n("Re&vert"), "reload", 0, this, SLOT(slotRevert()), ac);
  action->plug(m_menu);
  m_menu->insertSeparator();
#if KDE_VERSION < KDE_MAKE_VERSION(3,2,90)
  action = new KAction(i18n("&Add to Repository..."), 0, this, SLOT(slotAdd()), ac);
#else
  action = new KAction(i18n("&Add to Repository..."), "vcs_add", 0, this, SLOT(slotAdd()), ac);
#endif
  action->plug(m_menu);
#if KDE_VERSION < KDE_MAKE_VERSION(3,2,90)
  action = new KAction(i18n("&Remove from Repository..."), 0, this, SLOT(slotRemove()), ac);
#else
  action = new KAction(i18n("&Remove from Repository..."), "vcs_remove", 0, this, SLOT(slotRemove()), ac);
#endif
  action->plug(m_menu);
  action = new KAction(i18n("&Ignore in CVS Operations"), 0, this, SLOT(slotAddToCVSIgnore()), ac);
  action->plug(m_menu);
  action = new KAction(i18n("Do &Not Ignore in CVS Operations"), 0, this, SLOT(slotRemoveFromCVSIgnore()), ac);
  action->plug(m_menu);
  
  m_menu->insertSeparator();
  action = new KAction(i18n("Show &Log Messages"), 0, this, SLOT(slotBrowseLog()), ac);
  action->plug(m_menu);
    
  m_cvsJob = 0L;
  m_repository = 0L;
  m_cvsService =0L;
  m_commitDlg = new CVSCommitDlgS();
  m_updateToDlg = new CVSUpdateToDlgS();
}

CVSService::~CVSService()
{
  if (m_cvsService)
    m_cvsService->quit();
  delete m_menu;
  delete m_repository;
  m_repository = 0L;
  m_cvsService = 0L;
  delete m_commitDlg;
  delete m_updateToDlg;
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
      emit showMessage(i18n("Running CVS update..."), false);
      m_files = files;
      m_job = m_cvsService->update(files, true, true, true, "");
      m_cvsCommand = "update";
      m_cvsJob = new CvsJob_stub(m_job.app(), m_job.obj());

      connectDCOPSignal(m_job.app(), m_job.obj(), "jobExited(bool, int)", "slotJobExited(bool, int)", true);
      connectDCOPSignal(m_job.app(), m_job.obj(), "receivedStdout(QString)", "slotReceivedStdout(QString)", true);
      connectDCOPSignal(m_job.app(), m_job.obj(), "receivedStderr(QString)", "slotReceivedStderr(QString)", true);
      m_cvsJob->execute();
   }
}

void CVSService::slotUpdateToTag()
{
  QStringList files;
  if (!m_defaultFile.isEmpty())
  {
    if (m_defaultFile.startsWith(m_repositoryPath))
    {
      files += m_defaultFile.remove(m_repositoryPath);
      slotUpdateToTag(files);
    } else
    {
      notInRepository();
    }
  }
}

void CVSService::slotUpdateToTag(const QStringList &files)
{
  if (m_repository && !m_appId.isEmpty() && m_updateToDlg->exec())
  {
    QString extraOpts;
    QString commandStr;
    if (m_updateToDlg->tagRadioButton->isChecked())
    {
      extraOpts = "-r " + m_updateToDlg->tagCombo->currentText();
      commandStr = i18n("Updating to revision %1 ...").arg(m_updateToDlg->tagCombo->currentText());
    } else
    {
      extraOpts = "-D " + m_updateToDlg->dateLineEdit->text();
      commandStr = i18n("Updating to the version from %1 ...").arg(+ m_updateToDlg->dateLineEdit->text());
    }
    emit clearMessages();
    emit showMessage(commandStr, false);
    m_files = files;
    m_job = m_cvsService->update(files, true, true, true, extraOpts);
    m_cvsCommand = "update";
    m_cvsJob = new CvsJob_stub(m_job.app(), m_job.obj());

    connectDCOPSignal(m_job.app(), m_job.obj(), "jobExited(bool, int)", "slotJobExited(bool, int)", true);
    connectDCOPSignal(m_job.app(), m_job.obj(), "receivedStdout(QString)", "slotReceivedStdout(QString)", true);
    connectDCOPSignal(m_job.app(), m_job.obj(), "receivedStderr(QString)", "slotReceivedStderr(QString)", true);
    m_cvsJob->execute();
  }
}

void CVSService::slotUpdateToHead()
{
  QStringList files;
  if (!m_defaultFile.isEmpty())
  {
    if (m_defaultFile.startsWith(m_repositoryPath))
    {
      files += m_defaultFile.remove(m_repositoryPath);
      slotUpdateToHead(files);
    } else
    {
      notInRepository();
    }
  }
}

void CVSService::slotUpdateToHead(const QStringList &files)
{
  if (m_repository && !m_appId.isEmpty())
  {
    emit clearMessages();
    emit showMessage(i18n("Updating to HEAD..."), false);
    m_files = files;
    m_job = m_cvsService->update(files, true, true, true, "-A");
    m_cvsCommand = "update";
    m_cvsJob = new CvsJob_stub(m_job.app(), m_job.obj());

    connectDCOPSignal(m_job.app(), m_job.obj(), "jobExited(bool, int)", "slotJobExited(bool, int)", true);
    connectDCOPSignal(m_job.app(), m_job.obj(), "receivedStdout(QString)", "slotReceivedStdout(QString)", true);
    connectDCOPSignal(m_job.app(), m_job.obj(), "receivedStderr(QString)", "slotReceivedStderr(QString)", true);
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
   m_commitDlg->messageCombo->insertItem(i18n("Current"), 0);
   m_commitDlg->messageCombo->setCurrentItem(0);
   
   if (m_repository && !m_appId.isEmpty() && m_commitDlg->exec())
   {
      m_commitDlg->messageCombo->removeItem(0);
      QString message = m_commitDlg->logEdit->text();
      if (message != m_commitDlg->messageCombo->currentText())
          m_commitDlg->messageCombo->insertItem(message, 0);
      emit clearMessages();
      emit showMessage(i18n("Running CVS commit..."), false);
      m_files = files;
      m_job = m_cvsService->commit(files, message, true);
      m_cvsCommand = "commit";
      m_cvsJob = new CvsJob_stub(m_job.app(), m_job.obj());

      connectDCOPSignal(m_job.app(), m_job.obj(), "jobExited(bool, int)", "slotJobExited(bool, int)", true);
      connectDCOPSignal(m_job.app(), m_job.obj(), "receivedStdout(QString)", "slotReceivedStdout(QString)", true);
      connectDCOPSignal(m_job.app(), m_job.obj(), "receivedStderr(QString)", "slotReceivedStderr(QString)", true);
      m_cvsJob->execute();
   }
}

void CVSService::slotRevert()
{
  QStringList files;
  if (!m_defaultFile.isEmpty())
  {
    if (m_defaultFile.startsWith(m_repositoryPath))
    {
      files += m_defaultFile.remove(m_repositoryPath);
      slotRevert(files);
    } else
    {
      notInRepository();
    }
  }
}

void CVSService::slotRevert(const QStringList &files)
{
  if (m_repository && !m_appId.isEmpty())
  {
    emit clearMessages();
    emit showMessage(i18n("Reverting to the version from the repository..."), false);
    m_files = files;
    m_job = m_cvsService->update(files, true, true, true, "-C");
    m_cvsCommand = "update";
    m_cvsJob = new CvsJob_stub(m_job.app(), m_job.obj());

    connectDCOPSignal(m_job.app(), m_job.obj(), "jobExited(bool, int)", "slotJobExited(bool, int)", true);
    connectDCOPSignal(m_job.app(), m_job.obj(), "receivedStdout(QString)", "slotReceivedStdout(QString)", true);
    connectDCOPSignal(m_job.app(), m_job.obj(), "receivedStderr(QString)", "slotReceivedStderr(QString)", true);
    m_cvsJob->execute();
  }
}

void CVSService::slotAdd()
{
  QStringList files;
  if (!m_defaultFile.isEmpty())
  {
    if (m_defaultFile.startsWith(m_repositoryPath))
    {
      files += m_defaultFile.remove(m_repositoryPath);
      slotAdd(files);
    } else
    {
      notInRepository();
    }
  }
}

void CVSService::slotAdd(const QStringList &files)
{
  if (m_repository && !m_appId.isEmpty() && (KMessageBox::questionYesNoList(0, i18n("Add the following files to repository?"), files, i18n("CVS Add")) == KMessageBox::Yes))
  {
    emit clearMessages();
    emit showMessage(i18n("Adding file to the repository..."), false);
    m_files = files;
    m_job = m_cvsService->add(files, false);
    m_cvsCommand = "add";
    m_cvsJob = new CvsJob_stub(m_job.app(), m_job.obj());

    connectDCOPSignal(m_job.app(), m_job.obj(), "jobExited(bool, int)", "slotJobExited(bool, int)", true);
    connectDCOPSignal(m_job.app(), m_job.obj(), "receivedStdout(QString)", "slotReceivedStdout(QString)", true);
    connectDCOPSignal(m_job.app(), m_job.obj(), "receivedStderr(QString)", "slotReceivedStderr(QString)", true);
    m_cvsJob->execute();
  }
}

void CVSService::slotRemove()
{
  QStringList files;
  if (!m_defaultFile.isEmpty())
  {
    if (m_defaultFile.startsWith(m_repositoryPath))
    {
      files += m_defaultFile.remove(m_repositoryPath);
      slotRemove(files);
    } else
    {
      notInRepository();
    }
  }
}

void CVSService::slotRemove(const QStringList &files)
{
  if (m_repository && !m_appId.isEmpty() && (KMessageBox::warningContinueCancelList(0, i18n("<qt>Remove the following files from the repository?<br>This will remove your <b>working copy</b> as well.</qt>"), files, i18n("CVS Remove")) == KMessageBox::Continue))
  {
    emit clearMessages();
    emit showMessage(i18n("Removing files from the repository..."), false);
    m_files = files;
    m_job = m_cvsService->remove(files, true);
    m_cvsCommand = "remove";
    m_cvsJob = new CvsJob_stub(m_job.app(), m_job.obj());

    connectDCOPSignal(m_job.app(), m_job.obj(), "jobExited(bool, int)", "slotJobExited(bool, int)", true);
    connectDCOPSignal(m_job.app(), m_job.obj(), "receivedStdout(QString)", "slotReceivedStdout(QString)", true);
    connectDCOPSignal(m_job.app(), m_job.obj(), "receivedStderr(QString)", "slotReceivedStderr(QString)", true);
    m_cvsJob->execute();
  }
}

void CVSService::slotBrowseLog()
{
  if (!m_defaultFile.isEmpty())
  {
    if (m_defaultFile.startsWith(m_repositoryPath))
    {
      QString file = m_defaultFile.remove(m_repositoryPath);
      if (m_repository && !m_appId.isEmpty() )
      {
        emit clearMessages();
        emit showMessage(i18n("Showing CVS log..."), false);
        m_files += file;
        m_job = m_cvsService->log(file);
        m_cvsCommand = "log";
        m_cvsJob = new CvsJob_stub(m_job.app(), m_job.obj());

        connectDCOPSignal(m_job.app(), m_job.obj(), "jobExited(bool, int)", "slotJobExited(bool, int)", true);
        connectDCOPSignal(m_job.app(), m_job.obj(), "receivedStdout(QString)", "slotReceivedStdout(QString)", true);
        connectDCOPSignal(m_job.app(), m_job.obj(), "receivedStderr(QString)", "slotReceivedStderr(QString)", true);
        m_cvsJob->execute();
      }
    } else
    {
      notInRepository();
    }
  }
}

void CVSService::slotAddToCVSIgnore()
{
  if (!m_defaultFile.isEmpty())
  {
    if (m_defaultFile.startsWith(m_repositoryPath))
    {
      emit clearMessages();
      QFileInfo fInfo(m_defaultFile);
      QFile f(fInfo.dirPath()+ "/.cvsignore");
      if (f.open(IO_ReadWrite))
      {
        bool found = false;
        QTextStream str(&f);
        str.setEncoding(QTextStream::UnicodeUTF8);
        QString line;
        while (!str.atEnd())
        {
          line = str.readLine().stripWhiteSpace();
          if (line == fInfo.fileName())
          {
            emit showMessage(i18n("\"%1\" is already in the CVS ignore list.").arg(fInfo.fileName()), false);
            found = true;
            break;
          }
        }       
        if (!found)
        {
          str << fInfo.fileName() << endl;
          emit showMessage(i18n("\"%1\" added to the CVS ignore list.").arg(fInfo.fileName()), false);
        }
        f.close();
      }
      
    } else
    {
      notInRepository();
    }
  }
}

void CVSService::slotRemoveFromCVSIgnore()
{
  if (!m_defaultFile.isEmpty())
  {
    if (m_defaultFile.startsWith(m_repositoryPath))
    {
      emit clearMessages();
      QString content;
      QFileInfo fInfo(m_defaultFile);
      QFile f(fInfo.dirPath()+ "/.cvsignore");
      bool found = false;
      if (f.open(IO_ReadWrite))
      {
        QTextStream str(&f);
        str.setEncoding(QTextStream::UnicodeUTF8);
        QString line;
        while (!str.atEnd())
        {
          line = str.readLine().stripWhiteSpace();
          if (line != fInfo.fileName())
          {
            content += line + "\n";
          } else
            found = true;
        }       
        if (!found)
        {
          emit showMessage(i18n("\"%1\" is not in the CVS ignore list.").arg(fInfo.fileName()), false);
        }
        f.close();
      }
      if (found && f.open(IO_WriteOnly))
      {
        QTextStream str(&f);
        str.setEncoding(QTextStream::UnicodeUTF8);
        str << content;
        emit showMessage(i18n("\"%1\" removed from the CVS ignore list.").arg(fInfo.fileName()), false);
        f.close();
      }
      
    } else
    {
      notInRepository();
    }
  }
}

void CVSService::slotJobExited(bool normalExit, int exitStatus)
{
    if (!normalExit)
    {
        KMessageBox::sorry(0, i18n("<qt>The CVS command <b>%1</b> has failed. The error code was <i>%2</i>.</qt>").arg(m_cvsCommand).arg(exitStatus), i18n("Command Failed"));
    }
    if (exitStatus == 0)
    {
        emit commandExecuted(m_cvsCommand, m_files);
    }
    disconnectDCOPSignal(m_job.app(), m_job.obj(), "jobExited(bool, int)", "slotJobExited(bool, int)");
    disconnectDCOPSignal(m_job.app(), m_job.obj(), "receivedStdout(QString)", "slotReceivedStdout(QString)");
    disconnectDCOPSignal(m_job.app(), m_job.obj(), "receivedStderr(QString)", "slotReceivedStderr(QString)");
    //delete m_cvsJob;
    //m_cvsJob = 0L;
    emit showMessage(i18n("CVS command finished."), false);
    emit showMessage(" ", false);
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
