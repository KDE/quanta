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
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

/* Copyright: (C) 2003 Andras Mantia <amantia@kde.org> */

//kde includes
#include <kapplication.h>
#include <kdebug.h>
#include <kmessagebox.h>
#include <kpopupmenu.h>
#include <kiconloader.h>
#include <kparts/componentfactory.h>
#include <ktexteditor/editinterface.h>

//qt includes
#include <qpoint.h>
#include <qrect.h>
#include <qstring.h>
#include <qiconset.h>

//own includes
#include "tagdialog.h"
#include "../parser/parser.h"
#include "../parser/node.h"
#include "../parser/tag.h"

#include "../quantacommon.h"
#include "../quanta.h"
#include "../resource.h"

int newNum;

void TableEditor::init()
{
  m_popup = new KPopupMenu();
  m_cellEditId = m_popup->insertItem(i18n("&Edit Cell Properties"), this ,SLOT(slotEditCell()));
  m_rowEditId = m_popup->insertItem(i18n("Edit &Row Properties"), this ,SLOT(slotEditRow()));//  m_colEditId = m_popup->insertItem(i18n("Edit &Column Properties"), this ,SLOT(slotEditCol()));
  m_mergeSeparatorId = m_popup->insertSeparator();
  m_mergeCellsId = m_popup->insertItem(i18n("Merge Cells"), this, SLOT(slotMergeCells()));
  m_unmergeCellsId = m_popup->insertItem(i18n("Break Merging"), this, SLOT(slotUnmergeCells()));

  m_popup->insertSeparator();
  m_popup->insertItem(i18n("&Insert Row"), this, SLOT(slotInsertRow()));
  m_popup->insertItem(i18n("Insert Co&lumn"), this, SLOT(slotInsertCol()));
  m_popup->insertItem(i18n("Remove Row"), this, SLOT(slotRemoveRow()));
  m_popup->insertItem(i18n("Remove Column"), this, SLOT(slotRemoveCol()));
  m_popup->insertSeparator();
  m_popup->insertItem(i18n("Edit &Table Properties"), this, SLOT(slotEditTable()));
  m_editChildId = m_popup->insertItem(i18n("Edit Child Table"), this, SLOT(slotEditChildTable()));

  buttonOk->setIconSet(SmallIconSet("button_ok"));
  buttonCancel->setIconSet(SmallIconSet("button_cancel"));
  buttonHelp->setIconSet(SmallIconSet("help"));

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
  m_createNodes = true;
  newNum += 7;
}

void TableEditor::destroy()
{
  delete m_popup;
  delete m_tbody;
  delete m_thead;
  delete m_tfoot;
  delete m_table;
  newNum -=5;
  deleteMatrix(m_tableDataTags);
  deleteMatrix(m_tableHeaderTags);
  deleteMatrix(m_tableFooterTags);
  deleteList(m_tableDataRows);
  deleteList(m_tableHeaderRows);
  deleteList(m_tableFooterRows);
  kdDebug(24000) << "Undeleted new: " << newNum << endl;
}

void TableEditor::slotContextMenuRequested( int row, int col, const QPoint & pos )
{
  m_row = row;
  m_col = col;
  m_popup->setItemEnabled(m_cellEditId, (row >=0 && col >=0));
  m_popup->setItemEnabled(m_rowEditId, (row >=0));
  m_popup->setItemEnabled(m_colEditId, (col >=0));
  m_popup->setItemVisible(m_mergeSeparatorId, false);
  m_popup->setItemVisible(m_mergeCellsId, false);
  m_popup->setItemVisible(m_unmergeCellsId, false);
  if  (row >=0 && col >=0) {
    TableNode tableNode = (*m_tableTags)[m_row][m_col];
    m_popup->setItemVisible(m_mergeSeparatorId, false);
    m_popup->setItemVisible(m_mergeCellsId, false);
     m_popup->setItemVisible(m_editChildId, false);
    if (tableNode.merged) {
      m_popup->setItemVisible(m_unmergeCellsId, true);
      m_popup->setItemVisible(m_mergeSeparatorId, true);
    }
    QTableSelection selection = m_dataTable->selection(m_dataTable->currentSelection());
    QRect rect(QPoint(selection.topRow(), selection.leftCol()) ,
               QPoint(selection.bottomRow(), selection.rightCol()));
    if (rect.isValid() && (rect.width() > 1 || rect.height() > 1) && rect.contains(m_row, m_col)) {
       m_popup->setItemVisible(m_mergeCellsId, true);
       m_popup->setItemVisible(m_mergeSeparatorId, true);
    }
    if (m_dataTable->item(m_row, m_col) && !m_dataTable->item(m_row, m_col)->pixmap().isNull()) {
       m_popup->setItemVisible(m_editChildId, true);
    }
  }
  m_popup->popup(pos);
}


void TableEditor::slotEditCell()
{
  Tag *tag = (*m_tableTags)[m_row][m_col].node->tag;
  TagDialog dlg(QuantaCommon::tagFromDTD(m_dtd, "td"), tag, m_baseURL);
  if (dlg.exec()) {
    tag->modifyAttributes(dlg.getAttributes());
    //TODO: add/remove columns/rows if the colspan/rowspan attribute is changed
  }
}


void TableEditor::slotEditRow()
{
  Tag *tag = (*m_tableRows)[m_row].node->tag;
  TagDialog dlg(QuantaCommon::tagFromDTD(m_dtd,"tr"), tag, m_baseURL);
  if (dlg.exec()) {
    tag->modifyAttributes(dlg.getAttributes());
  }
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
  if (dlg.exec()) {
    m_table->modifyAttributes(dlg.getAttributes());
  }
}


void TableEditor::slotEditTableBody()
{
  TagDialog dlg(QuantaCommon::tagFromDTD(m_dtd,"tbody"), m_tbody, m_baseURL);
  if (dlg.exec()) {
    m_tbody->modifyAttributes(dlg.getAttributes());
  }
}


bool TableEditor::setTableArea( int bLine, int bCol, int eLine, int eCol, Parser *docParser )
{
  m_bLine = bLine;
  m_bCol = bCol;
  m_eLine = eLine;
  m_eCol = eCol;
  m_createNodes = false; //don't create the cell and row content when adding a new cell/row
  Node *node = docParser->nodeAt(bLine, bCol + 1);
  Node *lastNode = docParser->nodeAt(eLine, eCol);
  if (node)
    kdDebug(24000) << "node = " << node->tag->name << endl;
  if (lastNode)
    kdDebug(24000) << "lastnode = " << lastNode->tag->name << endl;
  if (!node || !lastNode)
    return false;
  m_write = node->tag->write();
  m_dtd = node->tag->dtd;
  if ( !QuantaCommon::closesTag(node->tag, lastNode->tag) ) {
    return false;
  }
  int nCol, nRow, maxCol;
  nCol = nRow = maxCol = 0;
  bool countRows = false;
  bool missingBody = false;
  m_rowSpin = 0L;
  m_colSpin = 0L;
  m_dataTable = 0L;
  QValueList<TableNode> tableRowTags;
  TableNode mergeMatrix[100][100];
  for (uint i = 0; i < 100; i++)
    for (uint j = 0; j < 100; j++)
      mergeMatrix[i][j].node = 0L;
  TableNode tableNode;
  Node *n = node;
  while (n != lastNode->nextSibling())
  {
    QString tagName = n->tag->name.lower();
    if (tagName == "table")
    {
      if (m_table && m_dataTable && nRow > 0 && nCol > 0) //nested table!
      {
        int line, col;
        n->tag->beginPos(line, col);
        NestedTable table;
        table.row = nRow -1;
        table.col = nCol - 1;
        table.bLine = line;
        table.bCol = col;
        if (n->next && QuantaCommon::closesTag(n->tag, n->next->tag)) {
          n->next->tag->endPos(table.eLine, table.eCol);
          table.node = n;
          table.nestedData = m_write->text(table.bLine, table.bCol, table.eLine, table.eCol);
          m_nestedTables.append(table);
          m_dataTable->item(nRow -1, nCol -1)->setPixmap(QIconSet(UserIcon("quick_table")).pixmap());
          m_dataTable->updateCell(nRow - 1, nCol - 1);
        }
        n = n->next;
      } else
      {
        m_table = new Tag(*(n->tag));
        newNum++;
      }
    }
    else if (tagName == "thead")
    {
      headerCheckBox->setChecked(true);
      countRows = true;
      m_rowSpin = headerRowSpinBox;
      m_colSpin = headerColSpinBox;
      m_dataTable= headerTableData;
      m_tableTags = m_tableHeaderTags;
      m_tableRows = m_tableHeaderRows;
      if (m_thead) { //there was already a <thead> tag in the area
        nRow = m_dataTable->numRows();
      } else {
        m_thead = new Tag(*(n->tag));
        newNum++;
      }
    }
    else if (tagName == "/thead")
    {
      headerRowSpinBox->setValue(nRow);
      headerColSpinBox->setValue(maxCol);
      countRows = false;
      nCol = nRow = maxCol = 0;
      m_rowSpin = 0L;
      m_colSpin = 0L;
      m_dataTable = 0L;
    }
    else if (tagName == "tfoot")
    {
      footerCheckBox->setChecked(true);
      m_rowSpin = footerRowSpinBox;
      m_colSpin = footerColSpinBox;
      m_tableTags = m_tableFooterTags;
      m_tableRows = m_tableFooterRows;
      m_dataTable = footerTableData;
      countRows = true;
      if (m_tfoot) { //there was already a <tfoot> tag in the area
        nRow = m_dataTable->numRows();
      } else {
        m_tfoot = new Tag(*(n->tag));
        newNum++;
      }
    }
    else if (tagName == "/tfoot")
    {
      footerRowSpinBox->setValue(nRow);
      footerColSpinBox->setValue(maxCol);
      countRows = false;
      nCol = nRow = maxCol = 0;
      m_rowSpin = 0L;
      m_colSpin = 0L;
      m_dataTable = 0L;
    }
    else if (tagName == "tbody")
    {
      m_rowSpin = rowSpinBox;
      m_colSpin = colSpinBox;
      m_tableTags = m_tableDataTags;
      m_tableRows = m_tableDataRows;
      m_dataTable = tableData;
      countRows = true;
      m_tbody = new Tag(*(n->tag));
      newNum++;
    }
    else if (tagName == "/tbody")
    {
      rowSpinBox->setValue(nRow);
      colSpinBox->setValue(maxCol);
      countRows = false;
      nCol = nRow = maxCol = 0;
      m_tableTags = 0L;
      m_tableRows = 0L;
      m_rowSpin = 0L;
      m_colSpin = 0L;
      m_dataTable = 0L;
    }
    else if (tagName == "tr")
    {
      if (!countRows)
      {
        missingBody = true;
        m_rowSpin = rowSpinBox;
        m_colSpin = colSpinBox;
        m_tableTags = m_tableDataTags;
        m_tableRows = m_tableDataRows;
        m_dataTable = tableData;
        countRows = true;
        m_tbody = new Tag();
        newNum++;
        m_tbody->parse("<tbody>", m_write);
      }
      tableRowTags.clear();
      nRow++;
      m_rowSpin->setValue(nRow);
      nCol = 0;
      tableNode.node = new Node(0L);
      tableNode.node->tag = new Tag(*(n->tag));
      newNum++;
      tableNode.merged = false;
      m_tableRows->append(tableNode);
    }
    else if (tagName == "/tr")
    {
      if (countRows)
      {
        maxCol = (nCol > maxCol) ? nCol : maxCol;
        for (int i = 0; i < maxCol - nCol; i++)
        {
          tableNode.node = new Node(0L);
          newNum++;
          tableNode.node->tag = new Tag();
          tableNode.node->tag->dtd = m_dtd;
          tableNode.node->tag->parse("<td>", m_write);
          tableNode.merged = false;
          tableRowTags.append(tableNode);
        }
        m_tableTags->append(tableRowTags);
      }
    }
    else if (tagName == "th" || tagName == "td")
    {
      if (countRows)
      {
        int col = nCol;
        while (mergeMatrix[nRow - 1][col].node != 0L) {
          if (m_colSpin->value() < col)
              m_colSpin->setValue(col);
          TableNode tableN = mergeMatrix[nRow - 1][col];
          Node *n = tableN.node;
          m_dataTable->setText(nRow - 1, col, i18n("Merged with (%1, %2).").arg(tableN.mergedRow).arg(tableN.mergedCol));
          m_dataTable->item(nRow-1, col)->setEnabled(false);
          tableNode.node = new Node(0L);
          tableNode.node->tag = new Tag(*(n->tag));
          newNum++;
          tableNode.merged = true;
          tableNode.mergedRow = tableN.mergedRow;
          tableNode.mergedCol = tableN.mergedCol;
          tableRowTags.append(tableNode);
          col++;
          nCol++;
        }
        nCol++;
        if (m_rowSpin && m_colSpin && m_dataTable)
        {
          m_rowSpin->setValue(nRow);
          if (m_colSpin->value() < nCol)
            m_colSpin->setValue(nCol);
          m_dataTable->setText(nRow - 1, nCol - 1, tagContent(n));
          tableNode.node = new Node(0L);
          tableNode.node->tag = new Tag(*(n->tag));
          newNum++;
          tableNode.merged = false;
          tableRowTags.append(tableNode);
        }
        QString colspanValue = n->tag->attributeValue("colspan");
        int colValue = 1;
        int lastCol = nCol;
        if (!colspanValue.isEmpty())
        {
          bool ok;
          colValue = colspanValue.toInt(&ok, 10);
          if (ok & colValue > 1)
          {
            nCol += (colValue - 1);
            if (m_colSpin->value() < nCol)
              m_colSpin->setValue(nCol);
            for (int i = 0; i < colValue - 1; i++)
            {
              m_dataTable->setText(nRow - 1, lastCol + i, i18n("Merged with (%1, %2).").arg(nRow).arg(lastCol));
              m_dataTable->item(nRow-1, lastCol + i)->setEnabled(false);
              tableNode.node = new Node(0L);
              tableNode.node->tag = new Tag(*(n->tag));
              newNum++;
              tableNode.merged = true;
              tableNode.mergedRow = nRow - 1;
              tableNode.mergedCol = lastCol - 1;
              tableRowTags.append(tableNode);
            }
          } else
            colValue = 1;
        }
        QString rowspanValue = n->tag->attributeValue("rowspan");
        if (!rowspanValue.isEmpty())
        {
          bool ok;
          int rowValue = rowspanValue.toInt(&ok, 10);
          if (ok & rowValue > 1)
          {
            lastCol--;
            for (int i = 0; i < rowValue - 1; i++)
              for (int j = 0; j < colValue; j++) {
                mergeMatrix[nRow + i][lastCol + j].mergedRow = nRow - 1;
                mergeMatrix[nRow + i][lastCol + j].mergedCol = lastCol;
                mergeMatrix[nRow + i][lastCol + j].node = n;
              }
          }
        }
      }
    }
    else if (tagName == "caption")
    {
      captionText->setText(tagContent(n));
    } else if (tagName == "col" || tagName == "colgroup") {
      m_colTags.append(n->tag);
    }
    n = n->nextSibling();
  }
  if (missingBody) {
      rowSpinBox->setValue(nRow);
      colSpinBox->setValue(maxCol);
  }
  //by default the current page is the data handling page
  m_tableTags = m_tableDataTags;
  m_tableRows = m_tableDataRows;
  m_dataTable = tableData;
  m_rowSpin = rowSpinBox;
  m_colSpin = colSpinBox;

  //create the thead, tbody, tfoot tags if they were not present in the parsed area
  if (!m_thead) {
    m_thead = new Tag();
    newNum++;
    m_thead->parse("<thead>", m_write);
  }
  if (!m_tfoot) {
    m_tfoot = new Tag();
    newNum++;
    m_tfoot->parse("<tfoot>", m_write);
  }
  m_createNodes = true; //enable cell/row creation
  return true;
}


void TableEditor::setBaseURL( const KURL & url )
{
  m_baseURL = url;
}


void TableEditor::slotEditTableHeader()
{
  TagDialog dlg(QuantaCommon::tagFromDTD(m_dtd, "thead"), m_thead, m_baseURL);
  if (dlg.exec()) {
    m_thead->modifyAttributes(dlg.getAttributes());
  }
}


void TableEditor::slotEditTableFooter()
{
  TagDialog dlg(QuantaCommon::tagFromDTD(m_dtd,"tfoot"), m_tfoot, m_baseURL);
  if (dlg.exec()) {
    m_tfoot->modifyAttributes(dlg.getAttributes());
  }
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
      m_colSpin = colSpinBox;
      m_rowSpin = rowSpinBox;
      break;
    }
   case 1: {
      m_tableTags = m_tableHeaderTags;
      m_tableRows = m_tableHeaderRows;
      m_dataTable = headerTableData;
      m_colSpin = headerColSpinBox;
      m_rowSpin = headerRowSpinBox;
      break;
    }
     case 2: {
      m_tableTags = m_tableFooterTags;
      m_tableRows = m_tableFooterRows;
      m_dataTable = footerTableData;
      m_colSpin = footerColSpinBox;
      m_rowSpin = footerRowSpinBox;
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
  for (QValueList<Tag*>::Iterator it = m_colTags.begin(); it != m_colTags.end(); ++it) {
    tableString += indent(2);
    tableString += (*it)->toString();
  }
  if (headerCheckBox->isChecked() && headerTableData->numRows() > 0) {
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
  if (footerCheckBox->isChecked() && footerTableData->numRows() > 0) {
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
 if (!m_dataTable)
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
        if ((*m_tableTags)[i][j].node && !(*m_tableTags)[i][j].merged)
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
      n->tag->beginPos(el, ec);
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
  m_dataTable->setRowHeight(num, 50);
  if (m_createNodes) {
    TableNode tableNode;
    tableNode.merged = false;
    tableNode.node = new Node(0L);
    newNum++;
    tableNode.node->tag = new Tag();
    tableNode.node->tag->dtd = m_dtd;
    tableNode.node->tag->parse("<tr>", m_write);
    QValueList<TableNode>::Iterator rowIt = m_tableRows->at(num);
    if (rowIt != m_tableRows->end())
      m_tableRows->insert(rowIt, tableNode);
    else
      m_tableRows->append(tableNode);
    QValueList<TableNode> tableRowTags;
    for (int i = 0; i < m_dataTable->numCols(); i++) {
      tableNode.merged = false;
      tableNode.node = new Node(0L);
      newNum++;
      tableNode.node->tag = new Tag();
      tableNode.node->tag->dtd = m_dtd;
      if (m_tableTags == m_tableHeaderTags) {
        tableNode.node->tag->parse("<th>", m_write);
      } else {
        tableNode.node->tag->parse("<td>", m_write);
      }
      tableRowTags.append(tableNode);
    }
    QValueList<QValueList<TableNode> >::Iterator it = m_tableTags->at(num);
    if (it != m_tableTags->end())
      m_tableTags->insert(it, tableRowTags);
    else
      m_tableTags->append(tableRowTags);
  }
  m_rowSpin->setValue(num + 1);
}


void TableEditor::slotInsertCol()
{
  int num = m_dataTable->numCols();
  if (m_col >= 0)
      num = m_col;
  m_dataTable->insertColumns(num);
  m_dataTable->setColumnWidth(num, 150);
  if (m_createNodes) {
    TableNode tableNode;
    for (QValueList<QValueList<TableNode> >::Iterator it = m_tableTags->begin(); it != m_tableTags->end(); ++it) {
      tableNode.merged = false;
      tableNode.node = new Node(0L);
      newNum++;
      tableNode.node->tag = new Tag();
      tableNode.node->tag->dtd = m_dtd;
      if (m_tableTags == m_tableHeaderTags) {
        tableNode.node->tag->parse("<th>", m_write);
      } else {
        tableNode.node->tag->parse("<td>", m_write);
      }
      (*it).append(tableNode);
    }
  }
  m_colSpin->setValue(num + 1);
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
  //TODO: change the main tag's rowspan if necessary
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
  //TODO: change the main tag's colspan if necessary}
}

void TableEditor::slotRemoveRow()
{
  if (m_row == -1)
    m_row = m_dataTable->numRows() - 1;
  QValueList<TableNode>::Iterator it = m_tableRows->at(m_row);
  delete (*it).node;
  newNum--;
  m_tableRows->erase(it);
  QValueList<QValueList<TableNode> >::Iterator it2 = m_tableTags->at(m_row);
  for (QValueList<TableNode>::Iterator it3 = (*it2).begin(); it3 != (*it2).end(); ++it3) {
    delete (*it3).node;
    newNum--;
  }
  m_tableTags->erase(it2);
  m_dataTable->removeRow(m_row);
}


void TableEditor::slotRemoveCol()
{
  int i = 0;
  int j = 0;
  for (QValueList<QValueList<TableNode> >::Iterator it = m_tableTags->begin(); it != m_tableTags->end(); ++it) {
      for (QValueList<TableNode>::Iterator it2 = (*it).begin(); it2 != (*it).end(); ++it2) {
        if ((*it2).merged && (*it2).mergedCol == m_col) {
          (*it2).merged = false;
          m_dataTable->setText(i, j, tagContent((*it2).node));
          m_dataTable->item(i, i)->setEnabled(true);
        }
        j++;
      }
      i++;
  }
  if (m_col == -1)
    m_col = m_dataTable->numCols() - 1;
  for (QValueList<QValueList<TableNode> >::Iterator it = m_tableTags->begin(); it != m_tableTags->end(); ++it) {
    QValueList<TableNode>::Iterator it2 = (*it).at(m_col);
    delete (*it2).node;
    newNum--;
    (*it).erase(it2);
  }
 m_dataTable->removeColumn(m_col);
}


void TableEditor::createNewTable(Document *write, DTDStruct *dtd)
{
  m_write = write;
  m_dtd = dtd;
  m_table = new Tag();
  newNum++;
  m_table->parse("<table>", m_write);
  m_thead = new Tag();
  newNum++;
  m_thead->parse("<thead>", m_write);
  m_tfoot = new Tag();
  newNum++;
  m_tfoot->parse("<tfoot>", m_write);
  m_tbody = new Tag();
  newNum++;
  m_tbody->parse("<tbody>", m_write);
  //by default the current page is the data handling page
  m_tableTags = m_tableDataTags;
  m_tableRows = m_tableDataRows;
  m_dataTable = tableData;
  m_bLine = m_bCol = m_eLine = m_eCol = 0;
}


void TableEditor::deleteList( QValueList<TableNode> *table )
{
  for (QValueList<TableNode>::Iterator it = table->begin(); it != table->end(); ++it) {
    delete (*it).node;
    newNum--;
  }
  delete table;
  newNum--;
}


void TableEditor::deleteMatrix( QValueList<QValueList<TableNode> > *matrix )
{
    for (QValueList<QValueList<TableNode> >::Iterator it = matrix->begin(); it != matrix->end(); ++it) {
      for (QValueList<TableNode>::Iterator it2 = (*it).begin(); it2 != (*it).end(); ++it2) {
        delete (*it2).node;
        newNum--;
      }
  }
  delete matrix;
  newNum--;
}


void TableEditor::slotMergeCells()
{
  slotUnmergeCells(); //first unmerge all cells from the selection

  QTableSelection selection = m_dataTable->selection(m_dataTable->currentSelection());
  int tRow, bRow, lCol, rCol;
  tRow = selection.topRow();
  bRow = selection.bottomRow();
  lCol = selection.leftCol();
  rCol = selection.rightCol();
  TableNode *mainTableNode = &((*m_tableTags)[tRow][lCol]);
  mainTableNode->node->tag->editAttribute("colspan", QString("%1").arg(rCol - lCol + 1));
  mainTableNode->node->tag->editAttribute("rowSpan", QString("%1").arg(bRow - tRow + 1));
  for (int i = 0; i < bRow - tRow + 1; i++)
    for (int j = 0; j < rCol - lCol + 1; j++) {
      if (i != 0 || j != 0) {
        m_dataTable->setText(tRow + i, lCol + j, i18n("Merged with (%1, %2).").arg(tRow).arg(lCol));
        m_dataTable->item(tRow + i, lCol + j)->setEnabled(false);
        TableNode *tableNode = &((*m_tableTags)[tRow + i][lCol + j]);
        tableNode->node = new Node(0L);
        newNum++;
        tableNode->node->tag = new Tag(*(mainTableNode->node->tag));
        tableNode->merged = true;
        tableNode->mergedRow = tRow;
        tableNode->mergedCol = lCol;
      }
    }
}


void TableEditor::slotUnmergeCells()
{
  int tRow, bRow, lCol, rCol;
  int selectionNum = m_dataTable->currentSelection();
  if (selectionNum != -1) {
    QTableSelection selection = m_dataTable->selection(selectionNum);
    tRow = selection.topRow();
    bRow = selection.bottomRow();
    lCol = selection.leftCol();
    rCol = selection.rightCol();
  } else {
    tRow = m_row;
    bRow = m_row;
    lCol = m_col;
    rCol = m_col;
  }
  for (int row = tRow; row <= bRow; ++row)
    for (int col = lCol; col <= rCol; ++col) {
      TableNode tableNode = (*m_tableTags)[row][col];
      if (!tableNode.merged)
        continue;
      TableNode newTableNode;
      int i = 0;
      int j = 0;
      for (QValueList<QValueList<TableNode> >::Iterator it = m_tableTags->begin(); it != m_tableTags->end(); ++it) {
        j = 0;
        QValueList<TableNode>::Iterator it2 = (*it).begin();
        while (it2 != (*it).end()) {
          if ((*it2).merged &&
              tableNode.mergedRow == (*it2).mergedRow &&
              tableNode.mergedCol == (*it2).mergedCol) {

              delete (*it2).node;
              newNum--;
              it2 = (*it).erase(it2);
              newTableNode.merged = false;
              newTableNode.node = new Node(0L);
              newNum++;
              newTableNode.node->tag = new Tag();
              newTableNode.node->tag->dtd = m_dtd;
              if (m_tableTags == m_tableHeaderTags) {
                newTableNode.node->tag->parse("<th>", m_write);
              } else {
                newTableNode.node->tag->parse("<td>", m_write);
              }
              (*it).insert(it2, newTableNode);
              m_dataTable->setText(i, j, tagContent(newTableNode.node));
              m_dataTable->item(i, j)->setEnabled(true);
          } else {
            ++it2;
          }
          j++;
        }
        i++;
      }
      newTableNode = (*m_tableTags)[tableNode.mergedRow][tableNode.mergedCol];
      newTableNode.node->tag->deleteAttribute("colspan");
      newTableNode.node->tag->deleteAttribute("rowspan");
      //change the main node
      TableNode tmpNode = newTableNode;
      newTableNode.node = new Node(0L);
      newNum++;
      newTableNode.node->tag = new Tag(*(tmpNode.node->tag));
      QValueList<QValueList<TableNode> >::Iterator iter1 = m_tableTags->at(tableNode.mergedRow);
      QValueList<TableNode>::Iterator iter2 = (*iter1).at(tableNode.mergedCol);
      iter2 = (*iter1).erase(iter2);
      (*iter1).insert(iter2, newTableNode);
      delete tmpNode.node;
      newNum--;
    }
}



void TableEditor::slotEditChildTable()
{
  bool tempDocCreated = false;
  bool error = false;
  QValueList<NestedTable>::Iterator errorIt;
  Parser *localParser = 0L;
  Document *w = 0L;
  Node *savedBaseNode = 0L;
  NestedTable table;

  for (QValueList<NestedTable>::Iterator it = m_nestedTables.begin(); it != m_nestedTables.end(); ++it) {
    table = *it;
    if (table.row == m_row  && table.col == m_col) {
      QString cellData = m_dataTable->text(table.row, table.col);
      int pos = cellData.find(table.nestedData);
      if (pos == -1) {
        KMessageBox::error(this, i18n("Cannot edit the child table. Most probably you modified the cell containing the table manually."), i18n("Cannot Read Table"));
        error = true;
        errorIt = it;
        break;
      }
      //create a new editor object and save the current state of the table there
      KTextEditor::Document *doc =
  KTextEditor::createDocument ("libkatepart", 0L, "KTextEditor::Document");
      w = new Document(quantaApp->projectBaseURL(), doc, quantaApp->project(),
                             quantaApp->m_pluginInterface, 0L);
      QString tableData = readModifiedTable();
      w->editIf->insertText(0, 0, tableData);
      localParser = new Parser();
      savedBaseNode = baseNode; //we must save it as it's deleted in the localParser->parse();
      baseNode = 0L;
      baseNode = localParser->parse(w);
      tempDocCreated = true;
      //try to find the child table position
      int pos2 = tableData.find(cellData);
      if (pos2 != -1)
        pos2 = tableData.find(table.nestedData, pos2);
      if (pos2 == -1) {
        KMessageBox::error(this, i18n("Cannot edit the child table. Most probably you modified the cell containing the table manually."), i18n("Cannot Read Table"));
        error = true;
        errorIt = it;
        break;
      }
      tableData = tableData.left(pos2);
      table.bLine = tableData.contains('\n');
      pos2 = tableData.findRev('\n');
      if (pos2 != -1) {
        table.bCol = tableData.length() - pos2;
      } else {
        table.bCol = tableData.length();
      }
      Node *childTableNode = localParser->nodeAt(table.bLine, table.bCol);
      if (!childTableNode->next || !QuantaCommon::closesTag(childTableNode->tag, childTableNode->next->tag)) {
        KMessageBox::error(this, i18n("Cannot find the closing tag of the child table. Most probably you've introduced unclosed tags in the table and have broken its consistency."), i18n("Cannot Read Table"));
        error = true;
        errorIt = it;
        break;
      }
      childTableNode->next->tag->endPos(table.eLine, table.eCol);
      TableEditor editor;
      editor.setBaseURL(m_baseURL);
      editor.setTableArea(table.bLine, table.bCol, table.eLine, table.eCol, localParser);
      if (editor.exec()) {
       int length = table.nestedData.length();
       (*it).nestedData =  editor.readModifiedTable();
       cellData.replace(pos, length, (*it).nestedData);
       m_dataTable->setText(table.row, table.col, cellData);
      }
      //cleanup on success
      delete baseNode;
      baseNode = savedBaseNode;
      delete localParser;
      delete w;
      return;
    }
  }
  //cleanup on error
  if (error) {
    m_nestedTables.erase(errorIt);
    m_dataTable->item(table.row, table.col)->setPixmap(QPixmap());
    m_dataTable->updateCell(table.row, table.col);
    if (tempDocCreated) {
      delete baseNode;
      baseNode = savedBaseNode;
      delete localParser;
      delete w;
    }
  }
}


void TableEditor::slotHelpInvoked()
{
//FIXME: "tag-mail" should be replaced with the real help section tag
   kapp->invokeHelp("table-editor","quanta");
}
