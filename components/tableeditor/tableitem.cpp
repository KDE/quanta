/***************************************************************************
                          tableitem.cpp -  description
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
  Editor->setText(text());
  QObject::connect(Editor, SIGNAL(textChanged()), table(), SLOT(doValueChanged()));
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
     QFont editFont = p->font();
     editFont.setBold(true);
     p->setFont(editFont);
   }
   QRect cr0(0, 0, cr.width(), cr.height());
   if (selected) {
     p->fillRect(cr0, cg.brush(QColorGroup::Highlight));
     p->setPen(cg.highlightedText());
   }
   else {
     p->fillRect(cr0, cg.brush(QColorGroup::Base));
     p->setPen(cg.text());
   }
   if (!pixmap().isNull()) {
     p->drawPixmap(4, 4, pixmap());
     p->drawText(6 + pixmap().width(), 4, cr0.width()-8, cr0.height()-8, m_halign | m_valign | WordBreak, text());
   }
   else
     p->drawText(4, 4, cr0.width()-8, cr0.height()-8, m_halign | m_valign | WordBreak, text());
}

QSize TableItem::sizeHint() const
{
   QSize size = QTableItem::sizeHint();
   size.setWidth(size.width()+8);
   size.setHeight(size.height()+8);
   return size;
}

