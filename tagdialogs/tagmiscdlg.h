/***************************************************************************
                          tagmiscdlg.h  -  description
                             -------------------
    begin                : Thu Dec 13 2001
    copyright            : (C) 2001 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef TAGMISCDLG_H
#define TAGMISCDLG_H

#include "tagmisc.h"

/**
  *@author Andras Mantia
  */

class TagMiscDlg : public TagMisc  {
Q_OBJECT
public:
  TagMiscDlg(QString element, bool closeTag, QWidget *parent=0, const char *name=0);
  ~TagMiscDlg();
private slots: // Private slots
   /** No descriptions */
  void slotShowHelp();
};

#endif
