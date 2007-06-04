/***************************************************************************
                          listdlg.h  -  description
                             -------------------
    begin                : Mon Nov 26 2001
    copyright            : (C) 2001 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef LISTDLG_H
#define LISTDLG_H

#include <kdialog.h>

class QString;
class QStringList;
class K3ListBox;

/**Select an item from a list. The list is given in
the constructor as a parameter. After calling
the exec() method, you can check the
selected item with the getEntry() method.
  *@author Andras Mantia
  */

class ListDlg : public KDialog  {
Q_OBJECT
public:
  explicit ListDlg(QStringList &entryList,QWidget* parent=0);
  ~ListDlg();
  QString getEntry();
  int getEntryNum();
private: // Private attributes
  /**  */
  K3ListBox * listbox;
};

#endif
