//
// C++ Implementation: tableitem
//
// Description: 
//
//
// Author: Andras Mantia <amantia@kde.org>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <qtextedit.h>
#include <qpainter.h>
#include "tableitem.h"

TableItem::TableItem(QTable* table, EditType et) : QTableItem(table, et)
{
  setReplaceable(false);
  m_halign = Qt::AlignLeft;
  m_valign = Qt::AlignVCenter;
}

TableItem::TableItem(QTable* table, EditType et, const QString& text) : QTableItem(table, et, text)
{
  setReplaceable(false);
  m_halign = Qt::AlignLeft;
  m_valign = Qt::AlignVCenter;
}

TableItem::TableItem (QTable* table, EditType et, const QString& text, const QPixmap& p) :
  QTableItem(table, et, text, p)
{
  setReplaceable(false);
  m_halign = Qt::AlignLeft;
  m_valign = Qt::AlignVCenter;
}

QWidget* TableItem::createEditor() const
{
  QTextEdit* Editor = new QTextEdit(table()->viewport());
  Editor->setTextFormat(QTextEdit::PlainText);
  Editor->setHScrollBarMode(QScrollView::AlwaysOff);
  Editor->setVScrollBarMode(QScrollView::AlwaysOff);
  Editor->setBold(m_header);
  //  QObject::connect( cb, SIGNAL( activated( int ) ), table(), SLOT( doValueChanged() ) );
  Editor->setText(text());
  return Editor;
}

void TableItem::setContentFromEditor(QWidget *w)
{
  if (w->inherits( "QTextEdit" ))
    setText(((QTextEdit*)w)->text());
  else
    QTableItem::setContentFromEditor(w);
}

void TableItem::paint(QPainter* p, const QColorGroup& cg, const QRect& cr, bool selected)
{
   if (m_header) {
     QFont F = p->font();
     F.setBold(true);
     p->setFont(F);
   }
   QRect cr0 = cr;
   cr0.setTop(0);
   cr0.setLeft(0);
   if (selected) {
     p->fillRect(cr0, cg.brush(QColorGroup::Highlight));  
     p->setPen(cg.highlightedText());
   }
   else {
     p->fillRect(cr0, cg.brush(QColorGroup::Base));
     p->setPen(cg.text());
   }
   p->drawText(4, 4, cr.width()-8, cr.height()-8, m_halign | m_valign | WordBreak, text());
}

QSize TableItem::sizeHint() const
{
   QSize size = QTableItem::sizeHint();
   size.setWidth(size.width()+8);
   size.setHeight(size.height()+8);
   return size;
}

