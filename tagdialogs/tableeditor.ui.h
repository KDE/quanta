/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/* Copyright: (C) 2003 Andras Mantia <amantia@kde.org> */

//kde includes
#include <kmessagebox.h>
#include <kpopupmenu.h>
#include <kurl.h>

//qt includes
#include <qstring.h>

//own includes
#include "tagdialog.h"
#include "../quantacommon.h"

void TableEditor::init()
{
  m_popup = new KPopupMenu();
  m_cellEditId = m_popup->insertItem(i18n("&Edit Cell Properties"), this ,SLOT(slotEditCell()));
  m_rowEditId = m_popup->insertItem(i18n("&Edit Row Properties"), this ,SLOT(slotEditRow()));
  m_colEditId = m_popup->insertItem(i18n("&Edit Column Properties"), this ,SLOT(slotEditCol()));  
  m_popup->insertItem(i18n("&Edit Table Properties"), this, SLOT(slotEditTable()));
  m_row = m_col = -1;
}

void TableEditor::destroy()
{
  delete m_popup;
}

void TableEditor::slotContextMenuRequested( int row, int col, const QPoint & pos )
{
  m_row = row;
  m_col = col;
  m_popup->setItemEnabled(m_cellEditId, (row >=0 && col >=0));
  m_popup->setItemEnabled(m_rowEditId, (row >=0));
  m_popup->setItemEnabled(m_colEditId, (col >=0));
  m_popup->popup(pos);
}


void TableEditor::slotEditCell()
{
  KMessageBox::information(this, QString("Edit cell: %1, %2").arg(m_row + 1).arg(m_col + 1));
  TagDialog dlg(QuantaCommon::tagFromDTD("-//W3C//DTD HTML 4.01 Transitional//EN","td"));
  dlg.exec();
}


void TableEditor::slotEditRow()
{
  KMessageBox::information(this, QString("Edit row: %1").arg(m_row + 1));
  TagDialog dlg(QuantaCommon::tagFromDTD("-//W3C//DTD HTML 4.01 Transitional//EN","tr"));
  dlg.exec();
}


void TableEditor::slotEditCol()
{
  KMessageBox::information(this, QString("Edit col: %1").arg(m_col + 1));
  TagDialog dlg(QuantaCommon::tagFromDTD("-//W3C//DTD HTML 4.01 Transitional//EN","col"));
  dlg.exec();
}


void TableEditor::slotEditTable()
{
  TagDialog dlg(QuantaCommon::tagFromDTD("-//W3C//DTD HTML 4.01 Transitional//EN","table"));
  dlg.exec();
}


void TableEditor::slotEditTableBody()
{
  TagDialog dlg(QuantaCommon::tagFromDTD("-//W3C//DTD HTML 4.01 Transitional//EN","tbody"));
  dlg.exec();
}
