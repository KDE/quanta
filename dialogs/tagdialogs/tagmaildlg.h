/***************************************************************************
                          tagmaildlg.h  -  description
                             -------------------
    begin                : Sat Nov 24 2001
    copyright            : (C) 2001 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef TAGMAILDLG_H
#define TAGMAILDLG_H

#include "tagmail.h"

/**
  *@author Andras Mantia
  */

class TagMailDlg : public TagMail  {
Q_OBJECT
public:
  TagMailDlg(QWidget *parent=0, const char *name=0);
  ~TagMailDlg();
private slots: // Private slots
  /** Select a mail address, using the KABC */
  void slotSelectAddress();
};

#endif
