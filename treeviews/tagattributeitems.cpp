//
// C++ Implementation: cpp
//
// Description:
//
//
// Author: Andras Mantia <amantia@freemail.hu>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//


//qt includes
#include <qcombobox.h>
#include <qheader.h>
#include <qlineedit.h>
#include <qpainter.h>
#include <qstyle.h>

//kde includes
#include <kapplication.h>
#include <klistview.h>
#include <klocale.h>

//app includes
#include "tagattributeitems.h"
#include "tagattributetree.h"
#include "../parser/qtag.h"

TopLevelItem::TopLevelItem(KListView* parent, const QString &title)
: KListViewItem(parent, title)
{
}

TopLevelItem::TopLevelItem(KListView* parent, QListViewItem* after, const QString &title)
: KListViewItem(parent, after, title)
{
}

TopLevelItem::~TopLevelItem()
{
}


void TopLevelItem::paintCell(QPainter *p, const QColorGroup &cg,
                             int column, int width, int align)
{
    if ( !p )
        return;

    // make toplevel item names bold
    if (column == 0 && !parent())
    {
        QFont f = p->font();
        f.setBold(true);
        p->setFont(f);
    }
    KListViewItem::paintCell( p, cg, column, width, align );
    if (column == 0)
    {
      p->setPen(QPen(QColor(0,0,0)));
      p->drawLine( width - 1, 0, width - 1, height());
    }
}


//Generic attribute item
//Generic attribute item
AttributeItem::AttributeItem(QListViewItem* parent, const QString &title, const QString& title2)
: KListViewItem(parent, title, title2)
{
  lin = 0L;
}

AttributeItem::AttributeItem(TagAttributeTree* listView, DTDStruct *dtd, QListViewItem* parent, const QString &title, const QString& title2)
: KListViewItem(parent, title, title2)
{
  m_listView = listView;
  m_dtd = dtd;
  lin = new QLineEdit( m_listView->viewport() );
  QObject::connect( lin, SIGNAL( returnPressed() ), m_listView, SLOT( editorContentChanged() ) );
  lin->hide();
}

AttributeItem::~AttributeItem()
{
  delete lin;
}

void AttributeItem::paintCell(QPainter *p, const QColorGroup &cg,
                             int column, int width, int align)
{
    if ( !p )
        return;
    KListViewItem::paintCell( p, cg, column, width, align );
    if (column == 0)
    {
      p->setPen(QPen(QColor(0,0,0)));
      p->drawLine( width - 1, 0, width - 1, height());
    }
}

QString AttributeItem::editorText()
{
  return lin->text();
}

void AttributeItem::showEditor()
{
  placeEditor(lin);
  lin->show();
  lin->setText(text(1));
  lin->setFocus();
}

void AttributeItem::hideEditor()
{
  m_listView->editorContentChanged();
  setText(1, lin->text());
  lin->hide();
}

void AttributeItem::placeEditor( QWidget *w )
{
  QRect r = m_listView->itemRect( this );
  if ( !r.size().isValid() )
  {
    m_listView->ensureItemVisible( this );
    r = m_listView->itemRect( this );
  }
  r.setX( m_listView->header()->sectionPos( 1 ) );
  r.setWidth( m_listView->header()->sectionSize( 1 ) - 1 );
  r = QRect( m_listView->viewportToContents( r.topLeft() ), r.size() );
  w->resize( r.size() );
  m_listView->moveChild( w, r.x(), r.y() );
}

//Boolean attribute item
AttributeBoolItem::AttributeBoolItem(TagAttributeTree* listView, DTDStruct *dtd, QListViewItem* parent, const QString &title, const QString& title2)
: AttributeItem(parent, title, title2)
{
  m_listView = listView;
  m_dtd = dtd;
  combo = new QComboBox( m_listView->viewport() );
  combo->insertItem(m_dtd->booleanTrue);
  combo->insertItem(m_dtd->booleanFalse);
  combo->hide();
  QObject::connect( combo, SIGNAL( activated(int) ), m_listView, SLOT( editorContentChanged() ) );
 }

AttributeBoolItem::~AttributeBoolItem()
{
  delete combo;
}

QString AttributeBoolItem::editorText()
{
  return combo->currentText();
}

void AttributeBoolItem::showEditor()
{
  placeEditor(combo);
  combo->show();
  if (text(1) == m_dtd->booleanTrue)
      combo->setCurrentItem(0);
  else
      combo->setCurrentItem(1);
  combo->setFocus();
}

void AttributeBoolItem::hideEditor()
{
  m_listView->editorContentChanged();
  setText(1, combo->currentText());
  combo->hide();
}
