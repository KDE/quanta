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
#include <qlistbox.h>
#include <qpainter.h>
#include <qstyle.h>

//kde includes
#include <kapplication.h>
#include <kcolorcombo.h>
#include <klistview.h>
#include <klocale.h>
#include <kurlrequester.h>
#include <kdebug.h>

//app includes
#include "tagattributeitems.h"
#include "tagattributetree.h"
#include "../parser/node.h"
#include "../parser/tag.h"
#include "../parser/qtag.h"
#include "../document.h"
#include "../quantacommon.h"
#include "../qextfileinfo.h"

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

ParentItem::ParentItem(TagAttributeTree *listView, QListViewItem* parent)
: KListViewItem(parent)
{
  m_listView = listView;
  comboBox = new QComboBox(false, m_listView->viewport() );
  QRect r = m_listView->itemRect( this );
  if ( !r.size().isValid() )
  {
    m_listView->ensureItemVisible( this );
    r = m_listView->itemRect( this );
  }
  r.setX( m_listView->header()->sectionPos( 0 ) + 20);
  r.setWidth( m_listView->header()->sectionSize( 0 ) - 20);
  r = QRect( m_listView->viewportToContents( r.topLeft() ), r.size() );
  comboBox->resize( r.size() );
  m_listView->moveChild( comboBox, r.x(), r.y() );
}

ParentItem::~ParentItem()
{
  showList(false);
  delete comboBox;
}


void ParentItem::paintCell(QPainter *p, const QColorGroup &cg,
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

void ParentItem::addNode(Node *node)
{
  if (node)
  {
    m_nodeList.append(node);
    comboBox->insertItem(node->tag->name);
  }
}

Node* ParentItem::node(int index)
{
  return m_nodeList.at(index);
}

void ParentItem::showList(bool show)
{
  if (show)
  {
      comboBox->show();
      QObject::connect(comboBox, SIGNAL(activated(int)), m_listView, SLOT(slotParentSelected(int)));
  }
  else
  {
      comboBox->hide();
      QObject::disconnect(comboBox, SIGNAL(activated(int)), m_listView, SLOT(slotParentSelected(int)));
  }
}

//Generic attribute item
AttributeItem::AttributeItem(QListViewItem* parent, const QString &title, const QString& title2)
: KListViewItem(parent, title, title2)
{
  lin = 0L;
}

AttributeItem::AttributeItem(TagAttributeTree* listView, QListViewItem* parent, const QString &title, const QString& title2)
: KListViewItem(parent, title, title2)
{
  m_listView = listView;
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
AttributeBoolItem::AttributeBoolItem(TagAttributeTree* listView, QListViewItem* parent, const QString &title, const QString& title2)
: AttributeItem(parent, title, title2)
{
  m_listView = listView;
  combo = new QComboBox( m_listView->viewport() );
  combo->insertItem(m_listView->node()->tag->dtd->booleanTrue);
  combo->insertItem(m_listView->node()->tag->dtd->booleanFalse);
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
  if (text(1) == m_listView->node()->tag->dtd->booleanTrue)
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

//Boolean attribute item
AttributeUrlItem::AttributeUrlItem(TagAttributeTree* listView, QListViewItem* parent, const QString &title, const QString& title2)
: AttributeItem(parent, title, title2)
{
  m_listView = listView;
  urlRequester = new KURLRequester( m_listView->viewport() );
  urlRequester->setMode(KFile::File | KFile::ExistingOnly );
  urlRequester->hide();
  QObject::connect( urlRequester, SIGNAL( returnPressed() ), m_listView, SLOT( editorContentChanged() ) );
 }

AttributeUrlItem::~AttributeUrlItem()
{
  delete urlRequester;
}

QString AttributeUrlItem::editorText()
{
  KURL url;
  QuantaCommon::setUrl(url, urlRequester->url());
  url = QExtFileInfo::toRelative(url, m_listView->node()->tag->write()->url());
  QString s = url.url();
  if (url.protocol() == m_listView->node()->tag->write()->url().protocol())
    s.remove(0, url.protocol().length() + 1);
  return s;
}

void AttributeUrlItem::showEditor()
{
  placeEditor(urlRequester);
  urlRequester->show();
  KURL url;
  QuantaCommon::setUrl(url, text(1));
  url= QExtFileInfo::toAbsolute(url, m_listView->node()->tag->write()->url());
  urlRequester->setURL(url.url());
  urlRequester->setFocus();
}

void AttributeUrlItem::hideEditor()
{
  m_listView->editorContentChanged();
  setText(1, editorText());
  urlRequester->hide();
}


//editable listbox
//Boolean attribute item
AttributeListItem::AttributeListItem(TagAttributeTree* listView, QListViewItem* parent, const QString &title, const QString& title2)
: AttributeItem(parent, title, title2)
{
  m_listView = listView;
  combo = new QComboBox( m_listView->viewport() );
  Node *node = m_listView->node();
  QTag *qTag = QuantaCommon::tagFromDTD(node->tag->dtd, node->tag->name);
  if (qTag)
  {
    Attribute *attr = qTag->attribute(title);
    if (attr)
        combo->insertStringList(attr->values);
    combo->insertItem("", 0);
    combo->setEditable(true);
  }
  combo->hide();
  QObject::connect( combo, SIGNAL( activated(int) ), m_listView, SLOT( editorContentChanged() ) );
 }

AttributeListItem::~AttributeListItem()
{
  delete combo;
}

QString AttributeListItem::editorText()
{
  return combo->currentText();
}

void AttributeListItem::showEditor()
{
  placeEditor(combo);
  combo->show();
  int index = -1;
  Node *node = m_listView->node();
  QTag *qTag = QuantaCommon::tagFromDTD(node->tag->dtd, node->tag->name);
  if (qTag)
  {
    Attribute *attr = qTag->attribute(text(0));
    if (attr)
    {
      index = attr->values.findIndex(text(1));
    }
  }
  if (index != -1)
      combo->setCurrentItem(index + 1);
  else
  {
    combo->changeItem(text(1), 0);
    combo->setCurrentItem(0);
  }
  combo->setFocus();
}

void AttributeListItem::hideEditor()
{
  m_listView->editorContentChanged();
  setText(1, combo->currentText());
  combo->hide();
}

//editable color combobox
AttributeColorItem::AttributeColorItem(TagAttributeTree* listView, QListViewItem* parent, const QString &title, const QString& title2)
: AttributeItem(parent, title, title2)
{
  m_listView = listView;
  combo = new KColorCombo( m_listView->viewport() );
  combo->setEditable(true);
  combo->hide();
  QObject::connect( combo, SIGNAL( activated(int) ), m_listView, SLOT( editorContentChanged() ) );
 }

AttributeColorItem::~AttributeColorItem()
{
  delete combo;
}

QString AttributeColorItem::editorText()
{
  QString name =  combo->color().name();
  if (name == "#000000")
      name = "";
  combo->setCurrentText(name);
  return name;
}

void AttributeColorItem::showEditor()
{
  placeEditor(combo);
  combo->show();
  combo->setColor(text(1));
  combo->setCurrentText(text(1));
  combo->setFocus();
}

void AttributeColorItem::hideEditor()
{
  m_listView->editorContentChanged();
  QString name =  combo->color().name();
  if (name == "#000000")
      name = "";
  setText(1, name);
  combo->hide();
}
