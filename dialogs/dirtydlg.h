/***************************************************************************
                          dirtydlg.h  -  description
                             -------------------
    begin                : Fri Sep 13 2002
    copyright            : (C) 2002, 2003 by Andras Mantia
    email                : amantia@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#ifndef DIRTYDLG_H
#define DIRTYDLG_H

// kde includes
#include <kio/jobclasses.h>

class DirtyDialog;
class KDialogBase;

class KProcess;
class QCloseEvent;

class DirtyDlg : public KDialogBase  {
   Q_OBJECT

public:
  DirtyDlg(const QString& srcName, const QString& destName, bool createBackup = false, QWidget *parent=0, const char *name=0);
  ~DirtyDlg();

protected slots: // Protected slots
  /** No descriptions */
   void slotOk();
  /** No descriptions */
  void slotCompareDone(KProcess*);
  /** No descriptions */
  void slotResult(KIO::Job *);
  /** No descriptions */
  virtual void closeEvent(QCloseEvent* ev);

protected: // Protected attributes
  KURL m_src;
  KURL m_dest;
  bool m_busy;
  bool m_createBackup;
  int  m_permissions;
  DirtyDialog* m_mainWidget;
};

#endif
