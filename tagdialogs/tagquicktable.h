/***************************************************************************
                          tagquicktable.h  -  description
                             -------------------
    begin                : Sun Nov 28 1999
    copyright            : (C) 1999 by Yacovlev Alexander & Dmitry Poplavsky & Andras Mantia
    email                : pdima@mail.univ.kiev.ua
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TAGQUICKTABLE_H
#define TAGQUICKTABLE_H

#include <qwidget.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qdialog.h>

#include "quicktable.h"

/**
  *@author Dmitry Poplavsky & Yacovlev Alexander
  */

class Document;

class TagQuickTable : public Quick_Table{
   Q_OBJECT
public: 
  TagQuickTable(Document *write, QWidget *parent=0, const char *name=0);
  ~TagQuickTable();

public:

private:
  Document *write;

public slots: // Public slots
  /** No descriptions */
  void slotChangeColumnNumber(int columnNum);
  /** No descriptions */
  void slotEditColumn(QListViewItem *item);
  /** No descriptions */

};

#endif
