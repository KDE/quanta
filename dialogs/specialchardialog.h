/***************************************************************************
                     specialchardialog.h
                             -------------------

    copyright            : (C) 2004 - Michal Rudolf
    email                : mrudolf@kdewebdev.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SPECIALCHARDIALOG_H
#define SPECIALCHARDIALOG_H

#include <qstringlist.h> 
#include "specialchardialogs.h" 

class SpecialCharDialog : public SpecialCharDialogS
{
  Q_OBJECT
  
public:

  SpecialCharDialog( QWidget* parent = 0, const char* name = 0, bool modal = true, WFlags fl = 0);
  ~SpecialCharDialog();
  QString selection();

public slots:
  void filterChars(const QString&);
  void insertChar();
  void insertCode();
  void cancel();
  
private slots:
  
private:
  QString m_selection;
};

#endif
