/***************************************************************************
                          tableitem.h  -  description
                             -------------------
    begin                : Mon 15 Mar 2004
    copyright            : (C) 2004 by Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TABLEITEM_H
#define TABLEITEM_H

#include <qtable.h>
#include <kdeversion.h>

class TableItem : public QTableItem
{
private:
  Qt::AlignmentFlags m_halign;
  Qt::AlignmentFlags m_valign;
  bool m_header;
public:
  // Standard constructors copied from QTableItem
  TableItem(QTable* table, EditType et);
  TableItem(QTable* table, EditType et, const QString& text);
  TableItem (QTable* table, EditType et, const QString& text, const QPixmap& p);
  // Use QTextEdit instead of standard QLineEdit
  virtual QWidget* createEditor() const;
  // Get text from QTextEdit
  virtual void setContentFromEditor(QWidget *w);
  // Paint cell - handle alignment (horizontal and vertical) and bold for header
  virtual void paint(QPainter* p, const QColorGroup& cg, const QRect& cr, bool selected);
  // Return A bit larger sizeHint because QTextEdit has some margin around
  virtual QSize sizeHint() const;
  // Get and set vertical aligment
  Qt::AlignmentFlags vAlignment()	{return m_valign;}
  void setVAlignment(Qt::AlignmentFlags flags)	{m_valign = flags;}
  // Get and set horizontal aligment
  Qt::AlignmentFlags alignment()	{return m_halign;}
  void setAlignment(Qt::AlignmentFlags flags)	{m_halign = flags;}
  // Get and set header status (use true for TH, false for TD)
  bool header()	{return m_header;}
  void setHeader(bool h)  {m_header = h;}
};



#endif

