/***************************************************************************
                          listdlg.cpp  -  description
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

#include "listdlg.h"

//kde include
#include <klistbox.h>
#include <klocale.h>

//qt includes
#include <qpushbutton.h>
#include <qstring.h>
#include <qstringlist.h>

ListDlg::ListDlg(QStringList &entryList,QWidget* parent, const char *name )
  :KDialogBase(parent,name),    listbox(new KListBox(this))
{
   Q_CHECK_PTR(listbox);
   setMainWidget(listbox);
   showButtonApply(false);
   enableButtonSeparator(true);

   listbox->clear();
   listbox->insertStringList(entryList);
   if(entryList.count()>0)
   {
      listbox->setCurrentItem(0);
    }
   listbox->setMinimumSize(350,150);
   adjustSize();
   resize(sizeHint());

   connect(listbox,SIGNAL(doubleClicked( QListBoxItem * )), SLOT(slotOk()));
}

ListDlg::~ListDlg(){
}

QString ListDlg::getEntry()
{
   return listbox->currentText();
}

int ListDlg::getEntryNum()
{
   return listbox->currentItem();
}
#include "listdlg.moc"
