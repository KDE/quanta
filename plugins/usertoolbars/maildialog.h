/***************************************************************************
    begin                : Sat Nov 24 2001
    copyright            : (C) 2001, 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef MAILDIALOG_H
#define MAILDIALOG_H

#include "maildialogbase.h"

/**
  *@author Andras Mantia
  */

class MailDialog : public MailDialogBase  {
Q_OBJECT
public:
  MailDialog(QWidget *parent=0, const QString &caption = QString::null);
  ~MailDialog();
private slots: // Private slots
  /** Select a mail address, using the KABC */
  void slotSelectAddress();
  /** No descriptions */
  void slotShowHelp();
};

#endif
