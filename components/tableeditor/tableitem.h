//
// C++ Interface: tableitem
//
// Description: 
//
//
// Author: Andras Mantia <amantia@kde.org>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef TABLEITEM_H
#define TABLEITEM_H

#include <qtable.h>

class TableItem : public QTableItem
{
private:
  Qt::AlignmentFlags m_halign;
  Qt::AlignmentFlags m_valign;
  bool m_header;
public:
  TableItem(QTable* table, EditType et);
  TableItem(QTable* table, EditType et, const QString& text);
  TableItem (QTable* table, EditType et, const QString& text, const QPixmap& p);
  virtual QWidget* createEditor() const;
  virtual void setContentFromEditor(QWidget *w);
  virtual void paint(QPainter* p, const QColorGroup& cg, const QRect& cr, bool selected);
  virtual QSize TableItem::sizeHint() const;
  Qt::AlignmentFlags vAlignment()	{return m_valign;}
  void setVAlignment(Qt::AlignmentFlags flags)	{m_valign = flags;}
  Qt::AlignmentFlags alignment()	{return m_halign;}
  void setAlignment(Qt::AlignmentFlags flags)	{m_halign = flags;}
  bool header()	{return m_header;}
  void setHeader(bool h)  {m_header = h;}
};



#endif

