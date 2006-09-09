/***************************************************************************
                          dirtydlg.cpp  -  description
                             -------------------
    begin                : Fri Sep 13 2002
    copyright            : (C) 2002, 2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

//qt includes
#include <qevent.h>
#include <qlabel.h>
#include <qradiobutton.h>
#include <qpushbutton.h>

//kde includes
#include <kdialogbase.h>
#include <klocale.h>
#include <kprocess.h>
#include <kio/job.h>
#include <kio/jobclasses.h>
#include <kio/netaccess.h>
#include <kfileitem.h>

//app includes
#include "dirtydialog.h"
#include "dirtydlg.h"
#include "resource.h"
#include "qextfileinfo.h"

DirtyDlg::DirtyDlg(const QString& srcName, const QString& destName, bool createBackup, QWidget *parent, const char *name ) : KDialogBase(parent, name, true, i18n("File Changed"), KDialogBase::Ok)
{
 m_src.setPath(srcName);
 m_dest.setPath(destName);
 m_busy = false;
 m_createBackup = createBackup;
 m_mainWidget = new DirtyDialog(this);
 m_mainWidget->textLabel->setText(i18n("<qt>The file <b>%1</b> was changed outside of the Quanta editor.</qt>").arg(srcName));
 setMainWidget(m_mainWidget);
}

DirtyDlg::~DirtyDlg(){
}
/** No descriptions */
void DirtyDlg::slotOk()
{
 if (m_mainWidget->buttonCompare->isChecked())
 {
   m_busy = true;
   KProcess *proc = new KProcess();
   *proc << "kompare" << m_src.path() << m_dest.path();
   proc->start();
   connect(proc, SIGNAL(processExited(KProcess*)),SLOT(slotCompareDone(KProcess*)));
   enableButton(KDialogBase::Ok, false);
   enableButton(KDialogBase::Cancel, false);
 } else
 if (m_mainWidget->buttonLoad->isChecked())
 {
    accept();
 } else
 {
    reject();
 }
}

/** No descriptions */
void DirtyDlg::slotCompareDone(KProcess* proc)
{
 delete proc;

 if (m_createBackup)
 {
   KURL backupURL = m_src;
   backupURL.setPath(backupURL.path()+".backup");
   QExtFileInfo::copy(m_src, backupURL, -1, true, false, this);
 }

 KIO::UDSEntry entry;
 KIO::NetAccess::stat(m_src, entry, this);
 KFileItem item(entry, m_src, false, true);
 m_permissions = item.permissions();
 //TODO: Replace with KIO::NetAccess::file_move, when KDE 3.1 support
 //is dropped
 KIO::FileCopyJob *job = KIO::file_move(m_dest, m_src, m_permissions, true, false,false );
 connect( job, SIGNAL(result( KIO::Job *)),
                 SLOT  (slotResult( KIO::Job *)));
}


/** No descriptions */
void DirtyDlg::slotResult(KIO::Job *)
{
 m_busy = false;
 accept();
}

/** No descriptions */
void DirtyDlg::closeEvent(QCloseEvent* ev)
{
  if (m_busy)
    ev->ignore();
  else
    ev->accept();
}

#include "dirtydlg.moc"
