/***************************************************************************
                           tableeditor.h  -  table editor dialog
    begin              : Thu 15 Apr 2004
    copyright          : (C) 2004 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; version 2 of the License.
 *
 ***************************************************************************/

#ifndef TABLEEDITOR_H
#define TABLEEDITOR_H

#include <kurl.h>
#include <qvaluelist.h>

#include "tableeditors.h"
#include "document.h"

class QSpinBox;
class QTable;
class KPopupMenu;
class Tag;
struct DTDStruct;
class Node;

class TableNode 
{   
public:
  Node *node; 
  bool merged; 
  int mergedRow; 
  int mergedCol; 
  TableNode()    {node = 0;}
};



typedef struct NestedTable{ Node *node; int row; int col; int bLine; int bCol; int eLine; int eCol; QString nestedData;};
class Parser;


class TableEditor : public TableEditorS
{
  Q_OBJECT

public:
  TableEditor( QWidget* parent = 0, const char* name = 0 );
  ~TableEditor();

  virtual bool setTableArea( int bLine, int bCol, int eLine, int eCol, Parser * docParser );
  virtual void setBaseURL( const KURL & url );
  virtual QString readModifiedTable();
  virtual void createNewTable( Document * write, const DTDStruct * dtd );
  // Set defaults for table: enable word wrap, fit content, allow swapping col/rows with D&D
  virtual void configureTable( QTable * table );
  virtual void setCellText( QTable * table, int row, int col, const QString & text );
  // Configure TableItem from tag attributes
  virtual void configureCell( int row, int col, Node * node );

public slots:
  virtual void slotContextMenuRequested( int row, int col, const QPoint & pos );
  virtual void slotEditCell();
  // Wrapper for setText to use TableItem instead of QTableItem
  virtual void slotEditCellText( int r, int );
  virtual void slotEditRow();
  virtual void slotEditCol();
  virtual void slotEditTable();
  virtual void slotEditTableBody();
  virtual void slotEditTableHeader();
  virtual void slotEditTableFooter();
  virtual void slotTabChanged( QWidget * w );
  virtual void slotInsertRow();
  virtual void slotInsertCol();
  virtual void slotAddRemoveRow( int num );
  virtual void slotAddRemoveCol( int num );
  virtual void slotRemoveRow();
  virtual void slotRemoveCol();
  virtual void slotMergeCells();
  virtual void slotUnmergeCells();
  virtual void slotEditChildTable();
  virtual void slotHelpInvoked();

protected:
  QValueList<Tag*> m_colTags;
  int m_unmergeCellsId;
  int m_mergeSeparatorId;
  int m_mergeCellsId;
  bool m_createNodes;
  QValueList<QValueList<TableNode> > *m_tableTags;
  QValueList<QValueList<TableNode> > *m_tableFooterTags;
  QValueList<QValueList<TableNode> > *m_tableHeaderTags;
  QValueList<TableNode> *m_tableFooterRows;
  QValueList<TableNode> *m_tableHeaderRows;
  QValueList<TableNode> *m_tableRows;
  Document* m_write;
  Tag *m_table;
  Tag *m_tfoot;
  Tag *m_thead;
  int newVariable;
  Tag *m_tbody;
  int m_colEditId;
  int m_rowEditId;
  int m_cellEditId;
  int m_col;
  int m_row;
  KPopupMenu *m_popup;
  KURL m_baseURL;
  const DTDStruct *m_dtd;
  QValueList<QValueList<TableNode> > *m_tableDataTags;
  QValueList<TableNode> *m_tableDataRows;
  QTable *m_dataTable;
  QSpinBox *m_rowSpin;
  QSpinBox *m_colSpin;
  int m_bLine, m_bCol, m_eLine, m_eCol;
  QValueList<NestedTable> m_nestedTables;
  int m_editChildId;

  virtual QString indent( int n );
  virtual QString cellValue( int row, int col );
  virtual QString tableToString();
  virtual QString tagContent( Node * node );
  virtual void deleteList( QValueList<TableNode> * table );
  virtual void deleteMatrix( QValueList<QValueList<TableNode> > * matrix );

};

#endif 
