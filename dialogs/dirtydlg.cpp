/***************************************************************************
                          dirtydlg.cpp  -  description
                             -------------------
    begin                : Fri Sep 13 2002
    copyright            : (C) 2002 by Andras Mantia
    email                : amantia@freemail.hu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//qt includes
#include <qevent.h>
#include <qradiobutton.h>
#include <qpushbutton.h>

//kde includes
#include <kprocess.h>
#include <kio/job.h>
#include <kio/jobclasses.h>
#include <kio/netaccess.h>
#include <kfileitem.h>

//app includes
#include "dirtydlg.h"
#include "../resource.h"

DirtyDlg::DirtyDlg(const QString& srcName, const QString& destName, QWidget *parent, const char *name ) : DirtyDialog(parent,name)
{
 m_src.setPath(srcName);
 m_dest.setPath(destName);
 connect(okButton,SIGNAL(clicked()),SLOT(slotOKPressed()));
 connect(cancelButton,SIGNAL(clicked()),SLOT(reject()));
 m_busy = false;
}

DirtyDlg::~DirtyDlg(){
}
/** No descriptions */
void DirtyDlg::slotOKPressed()
{
 if (buttonCompare->isChecked())
 {
   m_busy = true;
   KProcess *proc = new KProcess();
   *proc << "kompare" << m_src.path() << m_dest.path();
   proc->start();
   connect(proc, SIGNAL(processExited(KProcess*)),SLOT(slotCompareDone(KProcess*)));
   okButton->setEnabled(false);
   cancelButton->setEnabled(false);
 } else
 if (buttonLoad->isChecked())
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

 KIO::UDSEntry entry;
 KIO::NetAccess::stat(m_src, entry);
 KFileItem item(entry, m_src, false, true);
 m_permissions = item.permissions();
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
  if(m_busy) ev->ignore();
  else ev->accept();
}

#include "dirtydlg.moc"
