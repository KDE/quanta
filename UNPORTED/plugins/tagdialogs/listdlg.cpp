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
#include <k3listbox.h>
#include <klocale.h>

//qt includes
#include <QPushButton>
#include <QString>
#include <QStringList>

ListDlg::ListDlg(QStringList &entryList,QWidget* parent)
  : KDialog(parent), listbox(new K3ListBox(this))
{
   Q_CHECK_PTR(listbox);
   setMainWidget(listbox);
   showButton(Apply, false);
   showButtonSeparator(true);
   setWindowTitle(i18n("Opened Files"));

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
