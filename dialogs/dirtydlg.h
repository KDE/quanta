/***************************************************************************
                          dirtydlg.h  -  description
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

#ifndef DIRTYDLG_H
#define DIRTYDLG_H

#include <qwidget.h>
// kde includes
#include <kio/jobclasses.h>

#include "dirtydialog.h"

/**
  *@author Andras Mantia
  */

class KProcess;
class QCloseEvent;

class DirtyDlg : public DirtyDialog  {
   Q_OBJECT
public: 
	DirtyDlg(const QString& srcName, const QString& destName, QWidget *parent=0, const char *name=0);
	~DirtyDlg();
protected: // Protected attributes
  KURL m_src;
  KURL m_dest;
  bool m_busy;
  int  m_permissions;  
protected slots: // Protected slots
  /** No descriptions */
   void slotOKPressed();
  /** No descriptions */
  void slotCompareDone(KProcess*);
  /** No descriptions */
  void slotResult(KIO::Job *);
  /** No descriptions */
  virtual void closeEvent(QCloseEvent* ev);

};

#endif
