/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
/***************************************************************************
 *                                                                         
 *   This program is free software; you can redistribute it and/or modify  
 *   it under the terms of the GNU General Public License as published by  
 *   the Free Software Foundation; version 2 of the License
 *                                                                        
 ***************************************************************************/

/* Copyright: (C) 2003 Andras Mantia <amantia@kde.org> */

//kde includes
#include <kdebug.h>
#include <kmessagebox.h>
#include <kpopupmenu.h>

//qt includes
#include <qstring.h>

//own includes
#include "tagdialog.h"
#include "../parser/parser.h"
#include "../parser/node.h"
#include "../parser/node.h"

#include "../document.h"
#include "../quantacommon.h"
#include "../resource.h"

void TableEditor::init()
{
  m_popup = new KPopupMenu();
  m_cellEditId = m_popup->insertItem(i18n("&Edit Cell Properties"), this ,SLOT(slotEditCell()));
  m_rowEditId = m_popup->insertItem(i18n("&Edit Row Properties"), this ,SLOT(slotEditRow()));
  m_colEditId = m_popup->insertItem(i18n("&Edit Column Properties"), this ,SLOT(slotEditCol()));  
  m_popup->insertItem(i18n("&Edit Table Properties"), this, SLOT(slotEditTable()));
  m_row = m_col = -1;
  m_tbody = 0L;
  m_thead = 0L;
  m_tfoot = 0L;
  m_table = 0L;
  m_dtd = 0L;
  m_write = 0L;
  m_tableDataTags = new QValueList<QValueList<Node*> >;
  m_tableHeaderTags = new QValueList<QValueList<Node*> >;
  m_tableFooterTags = new QValueList<QValueList<Node*> >;
  m_tableTags = 0L;
  m_tableDataRows = new QValueList<Node*>;
  m_tableHeaderRows = new QValueList<Node*>;
  m_tableFooterRows = new QValueList<Node*>;
  m_tableRows = 0L;
}

void TableEditor::destroy()
{
  delete m_popup;
  delete m_tbody;
  delete m_thead;
  delete m_tfoot;
  delete m_table;
  delete m_tableDataTags;
  delete m_tableHeaderTags;
  delete m_tableFooterTags;
  delete m_tableDataRows;
  delete m_tableHeaderRows;
  delete m_tableFooterRows;
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
  TagDialog dlg(QuantaCommon::tagFromDTD(m_dtd, "td"), (*m_tableTags)[m_row][m_col]->tag, m_baseURL);
  dlg.exec();
}


void TableEditor::slotEditRow()
{
  TagDialog dlg(QuantaCommon::tagFromDTD(m_dtd,"tr"), (*m_tableRows)[m_row]->tag, m_baseURL);
  dlg.exec();
}


void TableEditor::slotEditCol()
{
  KMessageBox::information(this, QString("Edit col: %1").arg(m_col + 1));
  TagDialog dlg(QuantaCommon::tagFromDTD(m_dtd,"col"));
  dlg.exec();
}


void TableEditor::slotEditTable()
{
  TagDialog dlg(QuantaCommon::tagFromDTD(m_dtd,"table"), m_table, m_baseURL);
  dlg.exec();
}


void TableEditor::slotEditTableBody()
{
  TagDialog dlg(QuantaCommon::tagFromDTD(m_dtd,"tbody"), m_tbody, m_baseURL);
  dlg.exec();
}


void TableEditor::setTableArea( int bLine, int bCol, int eLine, int eCol )
{
  Node *node = parser->nodeAt(bLine, bCol + 1);
  Node *lastNode = parser->nodeAt(eLine, eCol);
  if (node)
    kdDebug(24000) << "node = " << node->tag->name << endl;  
  if (lastNode)
    kdDebug(24000) << "lastnode = " << lastNode->tag->name << endl; 
  if (!node || !lastNode)
    return;
  m_write = node->tag->write();
  m_dtd = node->tag->dtd;
  int nCol, nRow, maxCol;
  nCol = nRow = maxCol = 0;
  bool countRows = false;
  int tableArea = -1; //0 = thead, 1 = tbody, 2 = tfoot; -1 = invalid
  QSpinBox *rowSpin = 0L;
  QSpinBox *colSpin = 0L;
  QTable *table = 0L;
  QValueList<Node*> tableRowTags;
  Node *n = node;  
  while (n != lastNode->nextSibling())
  {
    QString tagName = n->tag->name.lower();
    if (tagName == "table")
    {
      m_table = new Tag(*(n->tag));
    }
    else if (tagName == "thead")
    {  
      headerCheckBox->setChecked(true);
      countRows = true;
      rowSpin = headerRowSpinBox;
      colSpin = headerColSpinBox;
      table = headerTableData;
      m_tableTags = m_tableHeaderTags;
      m_tableRows = m_tableHeaderRows;
      tableArea = 0;
      m_thead = new Tag(*(n->tag));
    }
    else if (tagName == "/thead")
    {
      headerRowSpinBox->setValue(nRow);
      headerColSpinBox->setValue(maxCol);
      countRows = false;
      nCol = nRow = maxCol = 0;
      rowSpin = 0L;
      colSpin = 0L;
      table = 0L;
      tableArea = -1;
    }
    else if (tagName == "tfoot")
    {
      footerCheckBox->setChecked(true);
      rowSpin = footerRowSpinBox;
      colSpin = footerColSpinBox;
      m_tableTags = m_tableFooterTags;
      m_tableRows = m_tableFooterRows;
      table = footerTableData;
      countRows = true;
      tableArea = 2;
      m_tfoot = new Tag(*(n->tag));  
    } 
    else if (tagName == "/tfoot")
    {
      footerRowSpinBox->setValue(nRow);
      footerColSpinBox->setValue(maxCol);
      countRows = false;
      nCol = nRow = maxCol = 0;
      rowSpin = 0L;
      colSpin = 0L;
      table = 0L;
      tableArea = -1;
    }
    else if (tagName == "tbody")
    {
      rowSpin = rowSpinBox;
      colSpin = colSpinBox;
      m_tableTags = m_tableDataTags;
      m_tableRows = m_tableDataRows;
      table = tableData;
      countRows = true;
      tableArea = 1;
      m_tbody = new Tag(*(n->tag));
    }
    else if (tagName == "/tbody")
    {
      rowSpinBox->setValue(nRow);
      colSpinBox->setValue(maxCol);
      countRows = false;
      nCol = nRow = maxCol = 0;
      m_tableTags = 0L;
      m_tableRows = 0L;
      rowSpin = 0L;
      colSpin = 0L;
      table = 0L;
      tableArea = -1;
    }
    else if (tagName == "tr")
    {
      if (countRows)
      {
        tableRowTags.clear();
        nRow++;        
        nCol = 0;	
        m_tableRows->append(n);
      }
    } 
    else if (tagName == "/tr")  
    {
      if (countRows)
      {
        maxCol = (nCol > maxCol) ? nCol : maxCol;
        m_tableTags->append(tableRowTags);
      }
    }
    else if (tagName == "th" || tagName == "td") 
    {      
      if (countRows)
      {
        nCol++;
        if (rowSpin && colSpin && table)
        {
          QString content;
          int bl, bc, el, ec;
          n->tag->endPos(bl, bc);
          bc++;
          if (n->next)
          {
            n->next->tag->beginPos(el, ec);
            ec--;
          }
          else
          {
            el = eLine;
            ec = eCol;
          }
          content = m_write->text(bl, bc, el, ec);
          rowSpin->setValue(nRow);
          if (colSpin->value() < nCol)
            colSpin->setValue(nCol);
          table->setText(nRow - 1, nCol - 1, content);
          tableRowTags.append(n);       
        }
        QString colspanValue = n->tag->attributeValue("colspan");
        if (!colspanValue.isEmpty())
        {
          bool ok;
          int value = colspanValue.toInt(&ok, 10);
          if (ok & value > 1)
          {
            int lastCol = nCol;
            nCol += (value - 1);
            if (colSpin->value() < nCol)
              colSpin->setValue(nCol);
            for (int i = 0; i < value - 1; i++)
            {
              table->setText(nRow - 1, lastCol + i, i18n("Merged with column #%1.").arg(lastCol));
              table->item(nRow-1, lastCol + i)->setEnabled(false);
              tableRowTags.append(n);                                   
            }
          }
        }
      }
    }  
    else if (tagName=="caption" && n->child)
    {
      captionText->setText(n->child->tag->tagStr());
    }
    n = n->nextSibling();
  }
  
  //by default the current page is the data handling page
  m_tableTags = m_tableDataTags;
  m_tableRows = m_tableDataRows;
  
  //just for testing
  /*
  for (uint i = 0; i < m_tableDataTags.count(); i++)
  {
    for (uint j = 0; j < m_tableDataTags[i].count(); j++)      
    {
         Node *n = m_tableDataTags[i][j];
         QString content;
          int bl, bc, el, ec;
          n->tag->endPos(bl, bc);
          bc++;
          if (n->next)
          {
            n->next->tag->beginPos(el, ec);
            ec--;
          }
          else
          {
            el = eLine;
            ec = eCol;
          }
          content = m_write->text(bl, bc, el, ec);
          kdDebug(24000) << QString("Table[%1, %2] = %3").arg(i).arg(j).arg(content) << endl;
          kdDebug(24000) << QString("TableData[%1, %2] = %3").arg(i).arg(j).arg(tableData->text(i, j)) << endl;         
    }
  }
  */
}


void TableEditor::setBaseURL( const KURL & url )
{
  m_baseURL = url;
}


void TableEditor::slotEditTableHeader()
{
  TagDialog dlg(QuantaCommon::tagFromDTD(m_dtd, "thead"), m_thead, m_baseURL);
  dlg.exec();
  
}


void TableEditor::slotEditTableFooter()
{
  TagDialog dlg(QuantaCommon::tagFromDTD(m_dtd,"tfoot"), m_tfoot, m_baseURL);
  dlg.exec();
}


void TableEditor::slotTabChanged( QWidget *w)
{
  int i = tabWidget->indexOf(w);
  switch (i)
  {
    case 0: { 
      m_tableTags = m_tableDataTags;
      m_tableRows = m_tableDataRows;
      break;
    }
   case 1: {
      m_tableTags = m_tableHeaderTags;
      m_tableRows = m_tableHeaderRows;
      break;
    }
     case 2: {
      m_tableTags = m_tableFooterTags;
      m_tableRows = m_tableFooterRows;
      break;
    }
  }
}
