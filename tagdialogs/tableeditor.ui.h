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
  m_rowEditId = m_popup->insertItem(i18n("Edit &Row Properties"), this ,SLOT(slotEditRow()));
  m_colEditId = m_popup->insertItem(i18n("Edit &Column Properties"), this ,SLOT(slotEditCol()));
  m_popup->insertSeparator();
  m_popup->insertItem(i18n("&Insert Row"), this, SLOT(slotInsertRow()));
  m_popup->insertItem(i18n("Insert Co&lumn"), this, SLOT(slotInsertCol()));
  m_popup->insertSeparator();
  m_popup->insertItem(i18n("Edit &Table Properties"), this, SLOT(slotEditTable()));
  m_row = m_col = -1;
  m_tbody = 0L;
  m_thead = 0L;
  m_tfoot = 0L;
  m_table = 0L;
  m_dtd = 0L;
  m_write = 0L;
  m_tableDataTags = new QValueList<QValueList<TableNode> >;
  m_tableHeaderTags = new QValueList<QValueList<TableNode> >;
  m_tableFooterTags = new QValueList<QValueList<TableNode> >;
  m_tableTags = 0L;
  m_tableDataRows = new QValueList<TableNode>;
  m_tableHeaderRows = new QValueList<TableNode>;
  m_tableFooterRows = new QValueList<TableNode>;
  m_tableRows = 0L;
}

void TableEditor::destroy()
{
  delete m_popup;
  delete m_tbody;
  delete m_thead;
  delete m_tfoot;
  delete m_table;
  deleteMatrix(m_tableDataTags);
  deleteMatrix(m_tableHeaderTags);
  deleteMatrix(m_tableFooterTags);
  deleteList(m_tableDataRows);
  deleteList(m_tableHeaderRows);
  deleteList(m_tableFooterRows);
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
  TagDialog dlg(QuantaCommon::tagFromDTD(m_dtd, "td"), (*m_tableTags)[m_row][m_col].node->tag, m_baseURL);
  dlg.exec();
}


void TableEditor::slotEditRow()
{
  TagDialog dlg(QuantaCommon::tagFromDTD(m_dtd,"tr"), (*m_tableRows)[m_row].node->tag, m_baseURL);
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
  m_dataTable = 0L;
  QValueList<TableNode> tableRowTags;
  TableNode tableNode;
  Node *n = node;
  while (n != lastNode->nextSibling())
  {
    QString tagName = n->tag->name.lower();
    if (tagName == "table")
    {
      if (m_table) //nested table!
      {
        n = n->next;
      } else
      {
        m_table = new Tag(*(n->tag));
      }
    }
    else if (tagName == "thead")
    {
      headerCheckBox->setChecked(true);
      countRows = true;
      rowSpin = headerRowSpinBox;
      colSpin = headerColSpinBox;
      m_dataTable= headerTableData;
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
      m_dataTable = 0L;
      tableArea = -1;
    }
    else if (tagName == "tfoot")
    {
      footerCheckBox->setChecked(true);
      rowSpin = footerRowSpinBox;
      colSpin = footerColSpinBox;
      m_tableTags = m_tableFooterTags;
      m_tableRows = m_tableFooterRows;
      m_dataTable = footerTableData;
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
      m_dataTable = 0L;
      tableArea = -1;
    }
    else if (tagName == "tbody")
    {
      rowSpin = rowSpinBox;
      colSpin = colSpinBox;
      m_tableTags = m_tableDataTags;
      m_tableRows = m_tableDataRows;
      m_dataTable = tableData;
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
      m_dataTable = 0L;
      tableArea = -1;
    }
    else if (tagName == "tr")
    {
      if (countRows)
      {
        tableRowTags.clear();
        nRow++;
        nCol = 0;
        tableNode.node = n;
        tableNode.isFromDocument = true;
        m_tableRows->append(tableNode);
      }
    }
    else if (tagName == "/tr")
    {
      if (countRows)
      {
        maxCol = (nCol > maxCol) ? nCol : maxCol;
        for (int i = 0; i < maxCol - nCol; i++)
        {
          tableNode.node = new Node(0L);
          tableNode.node->tag = new Tag();
          tableNode.node->tag->parse("<td>", m_write);
          tableNode.isFromDocument = false;
          tableRowTags.append(tableNode);
        }
        m_tableTags->append(tableRowTags);
      }
    }
    else if (tagName == "th" || tagName == "td")
    {
      if (countRows)
      {
        nCol++;
        if (rowSpin && colSpin && m_dataTable)
        {
          rowSpin->setValue(nRow);
          if (colSpin->value() < nCol)
            colSpin->setValue(nCol);
          m_dataTable->setText(nRow - 1, nCol - 1, tagContent(n));
          tableNode.node = n;
          tableNode.isFromDocument = true;
          tableRowTags.append(tableNode);
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
              m_dataTable->setText(nRow - 1, lastCol + i, i18n("Merged with column #%1.").arg(lastCol));
              m_dataTable->item(nRow-1, lastCol + i)->setEnabled(false);
              tableNode.node = n;
              tableNode.isFromDocument = true;
              tableRowTags.append(tableNode);
            }
          }
        }
      }
    }
    else if (tagName=="caption")
    {
      captionText->setText(tagContent(n));
    }
    n = n->nextSibling();
  }

  //by default the current page is the data handling page
  m_tableTags = m_tableDataTags;
  m_tableRows = m_tableDataRows;
  m_dataTable = tableData;
  //just for testing
  /*
  for (uint i = 0; i < m_tableDataTags.count(); i++)
  {
    for (uint j = 0; j < m_tableDataTags[i].count(); j++)
    {
         Node *n = m_tableDataTags[i][j];
          content = m_write->text(bl, bc, el, ec);
          kdDebug(24000) << QString("Table[%1, %2] = %3").arg(i).arg(j).arg(tagContent(n)) << endl;
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
      m_dataTable = tableData;
      break;
    }
   case 1: {
      m_tableTags = m_tableHeaderTags;
      m_tableRows = m_tableHeaderRows;
      m_dataTable = headerTableData;
      break;
    }
     case 2: {
      m_tableTags = m_tableFooterTags;
      m_tableRows = m_tableFooterRows;
      m_dataTable = footerTableData;
      break;
    }
  }
}


QString TableEditor::readModifiedTable()
{
  QString tableString;
  tableString = m_table->toString();
  tableString += indent(2);
  tableString += "<" + QuantaCommon::tagCase("caption") + ">";
  tableString += captionText->text();
  tableString += "</" + QuantaCommon::tagCase("caption") + ">";
  if (headerCheckBox->isChecked()) {
    //insert the <thead> tag
    tableString += indent(2);
    tableString += m_thead->toString();

    kdDebug(24000) << "thead" << endl;
    m_tableTags = m_tableHeaderTags;
    m_tableRows = m_tableHeaderRows;
    m_dataTable = headerTableData;
    tableString += tableToString();
    tableString += indent(2);
    tableString += "</" + QuantaCommon::tagCase(m_thead->name) +">";
  }
  if (footerCheckBox->isChecked()) {
    //insert the <tfoot> tag
    tableString += indent(2);
    tableString += m_tfoot->toString();

    kdDebug(24000) << "tfoot" << endl;
    m_tableTags = m_tableFooterTags;
    m_tableRows = m_tableFooterRows;
    m_dataTable = footerTableData;
    tableString += tableToString();
    tableString += indent(2);
    tableString += "</" + QuantaCommon::tagCase(m_tfoot->name) +">";
  }
  //isert the <tbody> tag
  tableString += indent(2);
  tableString += m_tbody->toString();
  kdDebug(24000) << "tbody" << endl;
  m_tableTags = m_tableDataTags;
  m_tableRows = m_tableDataRows;
  m_dataTable = tableData;
  tableString += tableToString();
  //close the <tbody> and <table> tags
  tableString += indent(2);
  tableString += "</" + QuantaCommon::tagCase(m_tbody->name) +">";
  tableString += "\n";
  tableString += "</" + QuantaCommon::tagCase(m_table->name) + ">";
  tableString += "\n";

  //kdDebug(24000) << tableString << endl;
  return tableString;
}


QString TableEditor::indent( int n )
{
  QString str;
  str.fill(' ', n);
  str.prepend('\n');
  return str;
}


QString TableEditor::cellValue( int row, int col )
{
  if (!m_dataTable || !m_tableTags)
    return QString::null;
 QString str;
 Node *node = (*m_tableTags)[row][col].node;
 if (!node)
   return QString::null;

 str = node->tag->toString();
 str += m_dataTable->text(row, col);
 str += "</" + QuantaCommon::tagCase(node->tag->name) + ">";
 return str;
}


QString TableEditor::tableToString()
{
    QString tableStr;
    for (int i = 0; i < m_dataTable->numRows(); i++) {
      tableStr += indent(4);
      Node *node = (*m_tableRows)[i].node;
      Tag *tag = 0L;
      if (node)
          tag = node->tag;
      if (tag)
        tableStr += tag->toString();
      else
        tableStr += QuantaCommon::tagCase("<tr>");
      for (int j = 0; j < m_dataTable->numCols(); j++)  {
        if ((*m_tableTags)[i][j].node)
        {
          tableStr += indent(6);
          tableStr += cellValue(i, j);
        }
      }
      tableStr += indent(4);
      if (tag)
        tableStr += "</" + QuantaCommon::tagCase(tag->name) +">";
      else
        tableStr += QuantaCommon::tagCase("</tr>");
    }
  return tableStr;
}


QString TableEditor::tagContent(Node *node)
{
  if (!node)
    return QString::null;
  QString content;
  int bl, bc, el, ec;
  node->tag->endPos(bl, bc);
  bc++;
  if (node->next)
  {
    node->next->tag->beginPos(el, ec);
    ec--;
  }
  else
  {
    Node *n = node->nextSibling();
    if (n) {
      node->next->tag->beginPos(el, ec);
      ec--;
    } else {
      return QString::null;
    }
  }
 content = m_write->text(bl, bc, el, ec);
 return content;
}


void TableEditor::slotInsertRow()
{
  int num = m_dataTable->numRows();
  if (m_row >= 0)
     num = m_row;
  m_dataTable->insertRows(num);
  TableNode tableNode;
  tableNode.node = new Node(0L);
  tableNode.node->tag = new Tag();
  tableNode.node->tag->parse("<tr>", m_write);
  QValueList<TableNode>::Iterator rowIt = m_tableRows->at(num);
  if (rowIt != m_tableRows->end()) 
    m_tableRows->insert(rowIt, tableNode);
  else
    m_tableRows->append(tableNode);
  QValueList<TableNode> tableRowTags;
  for (int i = 0; i < m_dataTable->numCols(); i++) {    
    tableNode.node = new Node(0L);
    tableNode.node->tag = new Tag();
    tableNode.node->tag->parse("<td>", m_write);
    tableRowTags.append(tableNode);
  }
  QValueList<QValueList<TableNode> >::Iterator it = m_tableTags->at(num);
  if (it != m_tableTags->end())
    m_tableTags->insert(it, tableRowTags);
  else
    m_tableTags->append(tableRowTags);
  rowSpinBox->setValue(num + 1);
}


void TableEditor::slotInsertCol()
{
  int num = m_dataTable->numCols();
  if (m_col >= 0)
      num = m_col;
  m_dataTable->insertColumns(num);
  TableNode tableNode;
  for (QValueList<QValueList<TableNode> >::Iterator it = m_tableTags->begin(); it != m_tableTags->end(); ++it) {
    tableNode.node = new Node(0L);
    tableNode.node->tag = new Tag();
    tableNode.node->tag->parse("<td>", m_write);
    (*it).append(tableNode);
  }

  colSpinBox->setValue(num + 1);
}


void TableEditor::slotAddRemoveRow( int num )
{
  m_row = -1;
  int numRows = m_dataTable->numRows();
  if (num > numRows) {
    for (int i = numRows; i < num; i++) {
      slotInsertRow();
    }
  }
  else {
    for (int i = num; i < numRows; i++) {
      slotRemoveRow();
    }
  }
}


void TableEditor::slotAddRemoveCol( int num )
{
  m_col = -1;
  int numCols = m_dataTable->numCols();
  if (num > numCols) {
    for (int i = numCols; i < num; i++) {
      slotInsertCol();
    }
  }
  else {
    for (int i = num; i < numCols; i++) {
      slotRemoveCol();
    }
  }
}


void TableEditor::slotRemoveRow()
{

}


void TableEditor::slotRemoveCol()
{

}


void TableEditor::createNewTable(Document *write, DTDStruct *dtd)
{
  m_write = write;
  m_dtd = dtd;
  m_table = new Tag();
  m_table->parse("<table>", m_write);
  m_thead = new Tag();
  m_thead->parse("<thead>", m_write);
  m_tfoot = new Tag();
  m_tfoot->parse("<tfoot>", m_write);
  m_tbody = new Tag();
  m_tbody->parse("<tbody>", m_write);
  //by default the current page is the data handling page
  m_tableTags = m_tableDataTags;
  m_tableRows = m_tableDataRows;
  m_dataTable = tableData;
}


void TableEditor::deleteList( QValueList<TableNode> *table )
{
  for (QValueList<TableNode>::Iterator it = table->begin(); it != table->end(); ++it) {
    if (!(*it).isFromDocument)
      delete (*it).node;
  }
  delete table;
}


void TableEditor::deleteMatrix( QValueList<QValueList<TableNode> > *matrix )
{
    for (QValueList<QValueList<TableNode> >::Iterator it = matrix->begin(); it != matrix->end(); ++it) {
      for (QValueList<TableNode>::Iterator it2 = (*it).begin(); it2 != (*it).end(); ++it2) {
        if (!(*it2).isFromDocument)
          delete (*it2).node;
      }      
  }
  delete matrix;

}
