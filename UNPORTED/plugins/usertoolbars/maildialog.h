/***************************************************************************
    begin                : Sat Nov 24 2001
    copyright            : (C) 2001, 2005, 2006 by Andras Mantia <amantia@kde.org>
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

#include "ui_maildialogbase.h"

#include <kdialog.h>

/**
  *@author Andras Mantia
  */

class MailDialog : public KDialog, public Ui::MailDialogBase  {
Q_OBJECT
public:
  explicit MailDialog(QWidget *parent=0, const QString &caption = QString());
  ~MailDialog();
private slots: // Private slots
  /** Select a mail address, using the KABC */
  void slotSelectAddress();
};

#endif
