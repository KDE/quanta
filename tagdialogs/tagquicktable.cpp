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
#include "quicktablecolumnedit.h"

TagQuickTable::TagQuickTable(QWidget *parent, const char *name)
    : Quick_Table(parent,name,true)
{
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
  QuickTableColumnEdit *columnDlg = new QuickTableColumnEdit(this, i18n("Edit column"));

  columnDlg->columnNum->setText(item->text(0).section("-",-1));
  if (columnDlg->exec())
  {
    item->setText(1, columnDlg->widthEdit->text());
    item->setText(2, columnDlg->alignCombo->currentText());
    item->setText(3, columnDlg->valignCombo->currentText());
    item->setText(4, columnDlg->classEdit->text());
  }
}

#include "tagquicktable.moc"
