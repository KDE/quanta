/***************************************************************************
                          tagquicktable.cpp  -  description
                             -------------------
    begin                : Sun Nov 28 1999
    copyright            : (C) 1999 by Yacovlev Alexander & Dmitry Poplavsky
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
//qt includes
#include <qlistview.h>

//kde includes
#include <klocale.h>

//app includes
#include "tagquicktable.h"
#include "tagquicktableitem.h"
#include "tagdialog.h"
#include "../document.h"


TagQuickTable::TagQuickTable(Document *write, QWidget *parent, const char *name)
    : Quick_Table(parent,name,true)
{
  this->write = write;
	setCaption(name);
  connect( buttonOk, SIGNAL(clicked()), SLOT(accept()) );
	connect( buttonCancel, SIGNAL(clicked()), SLOT(reject()) );
  connect( SpinBoxCol, SIGNAL(valueChanged(int)), SLOT(slotChangeColumnNumber(int)));
  connect( ColumnsListView, SIGNAL(doubleClicked ( QListViewItem*)), SLOT(slotEditColumn(QListViewItem *)));
}



TagQuickTable::~TagQuickTable(){
}

/** No descriptions */
void TagQuickTable::slotChangeColumnNumber(int columnNum)
{
 int rowNum = ColumnsListView->childCount();

 for (int i = rowNum + 1; i <= columnNum; i++)
 {
  TagQuickTableItem* newItem = new TagQuickTableItem(ColumnsListView, QString("Column-%1").arg(i));
  ColumnsListView->insertItem(newItem);
  ColumnsListView->setSelected(newItem, true);
  ColumnsListView->setCurrentItem(newItem);
 }

 for (int i = rowNum; i > columnNum; i--)
 {
  QListViewItem* item = ColumnsListView->lastItem();
  ColumnsListView->takeItem(item);
  item = ColumnsListView->lastItem();
  ColumnsListView->setSelected(item,true);
  ColumnsListView->setCurrentItem(item);
 }

}

/** No descriptions */
void TagQuickTable::slotEditColumn(QListViewItem *item)
{
  TagDialog *dlg = new TagDialog("col");

  if (dlg->exec())
  {
    item->setText(1, dlg->getAttribute("width"));
    item->setText(2, dlg->getAttribute("align"));
    item->setText(3, dlg->getAttribute("valign"));
    item->setText(4, dlg->getAttribute("class"));
  }

  delete dlg;
}

#include "tagquicktable.moc"

