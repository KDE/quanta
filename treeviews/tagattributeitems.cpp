/***************************************************************************
                             tagattributeitems.cpp
                             ---------------------
    copyright            : (C) 2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

//qt includes
#include <qcombobox.h>
#include <qheader.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qpainter.h>
#include <qstyle.h>

//kde includes
#include <dcopref.h>
#include <kapplication.h>
#include <kcolorcombo.h>
#include <klistview.h>
#include <klocale.h>
#include <kurlrequester.h>
#include <kdebug.h>

//app includes
#include "tagattributeitems.h"
#include "tagattributetree.h"
#include "node.h"
#include "tag.h"
#include "qtag.h"
#include "document.h"
#include "quantacommon.h"
#include "qextfileinfo.h"
#include "styleeditor.h"

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
  lin2 = 0L;
}

AttributeItem::AttributeItem(EditableTree* listView, QListViewItem* parent, const QString &title, const QString& title2)
: KListViewItem(parent, title, title2)
{
  m_listView = listView;
  lin = new QLineEdit( m_listView->viewport() );
  lin2 = new QLineEdit( m_listView->viewport() );
  QObject::connect( lin, SIGNAL( returnPressed() ), m_listView, SLOT( editorContentChanged() ) );
  lin->hide();
  lin2->hide();
}

AttributeItem::AttributeItem(EditableTree *listView, const QString& title, const QString& title2)
: KListViewItem(listView, title, title2)
{
  m_listView = listView;
  lin = new QLineEdit( m_listView->viewport() );
  lin2 = new QLineEdit( m_listView->viewport() );
  lin2->setText(title);
  lin->setText(title2);
  QObject::connect( lin, SIGNAL( returnPressed() ), m_listView, SLOT( editorContentChanged() ) );
  lin->hide();
  lin2->hide();
}

AttributeItem::AttributeItem(EditableTree *listView, const QString& title, const QString& title2,
  QListViewItem* after)
: KListViewItem(listView, after, title, title2)
{
  m_listView = listView;
  lin = new QLineEdit( m_listView->viewport() );
  lin2 = new QLineEdit( m_listView->viewport() );
  lin2->setText(title);
  lin->setText(title2);
  QObject::connect( lin, SIGNAL( returnPressed() ), m_listView, SLOT( editorContentChanged() ) );
  lin->hide();
  lin2->hide();
}

AttributeItem::~AttributeItem()
{
  delete lin;
  delete lin2;
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

QString AttributeItem::editorText(int column)
{
  if(column == 1)
    return lin->text();
  else
    return lin2->text();
}

void AttributeItem::replaceCurrentEditor()
{
  if(lin->isVisible())
  {
    placeEditor(lin, 1);
  }
  else if(lin2->isVisible())
  {
    placeEditor(lin, 0);
  }
}

void AttributeItem::showEditor(int column)
{
  if(column == 1)
  {
    placeEditor(lin, column);
    lin->show();
    lin->setText(text(1));
    lin->setFocus();
  }
  else
  {
    placeEditor(lin2, column);
    lin2->show();
    lin2->setText(text(0));
    lin2->setFocus();
  }
}

void AttributeItem::hideEditor(int column)
{
  m_listView->editorContentChanged();
  if(column == 1 && lin->isVisible())
  {
    setText(1, lin->text());
    lin->hide();
  }
  else if(column == 0 && lin2->isVisible())
  {
    setText(0, lin2->text());
    lin2->hide();
  }
}

void AttributeItem::placeEditor( QWidget *w, int column)
{
  QRect r = m_listView->itemRect( this );
  if ( !r.size().isValid() )
  {
    m_listView->ensureItemVisible( this );
    r = m_listView->itemRect( this );
  }
  if(column == 1)
  {
    r.setX( m_listView->header()->sectionPos( 1 ) );
    r.setWidth( m_listView->header()->sectionSize( 1 ) - 1 );
  }
  else
  {
    r.setX( m_listView->header()->sectionPos( 0 ) );
    r.setWidth( m_listView->header()->sectionSize( 0 ) - 1 );
  }
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
  combo->insertItem((static_cast<TagAttributeTree *>(m_listView))->node()->tag->dtd()->booleanTrue);
  combo->insertItem((static_cast<TagAttributeTree *>(m_listView))->node()->tag->dtd()->booleanFalse);
  combo->hide();
  QObject::connect( combo, SIGNAL( activated(int) ), m_listView, SLOT( editorContentChanged() ) );
 }

AttributeBoolItem::~AttributeBoolItem()
{
  delete combo;
}

QString AttributeBoolItem::editorText(int)
{
  return combo->currentText();
}

void AttributeBoolItem::showEditor(int)
{
  placeEditor(combo);
  combo->show();
  if (text(1) == (static_cast<TagAttributeTree *>(m_listView))->node()->tag->dtd()->booleanTrue)
      combo->setCurrentItem(0);
  else
      combo->setCurrentItem(1);
  combo->setFocus();
}

void AttributeBoolItem::hideEditor(int)
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

QString AttributeUrlItem::editorText(int)
{
  KURL url, baseURL;
  QuantaCommon::setUrl(url, urlRequester->url());
  baseURL = (static_cast<TagAttributeTree *>(m_listView))->node()->tag->write()->url();
  baseURL.setPath(baseURL.directory());
  url = QExtFileInfo::toRelative(url, baseURL);
  QString s = url.url();
  if (url.protocol() == (static_cast<TagAttributeTree *>(m_listView))->node()->tag->write()->url().protocol())
    s.remove(0, url.protocol().length() + 3);
  return s;
}

void AttributeUrlItem::showEditor(int)
{
  placeEditor(urlRequester);
  urlRequester->show();
  KURL url, baseURL;
  baseURL = (static_cast<TagAttributeTree *>(m_listView))->node()->tag->write()->url();
  baseURL.setPath(baseURL.directory());
  QuantaCommon::setUrl(url, text(1));
  url= QExtFileInfo::toAbsolute(url, baseURL);
  urlRequester->setURL(url.url());
  urlRequester->setFocus();
}

void AttributeUrlItem::hideEditor(int)
{
  m_listView->editorContentChanged();
  setText(1, editorText());
  urlRequester->hide();
}


//editable listbox
//Boolean attribute item
AttributeListItem::AttributeListItem(EditableTree* listView, QListViewItem* parent, const QString &title, const QString& title2, Attribute *attr)
: AttributeItem(parent, title, title2)
{
  m_listView = listView;
  combo = new QComboBox( m_listView->viewport() );
  Node *node = (static_cast<TagAttributeTree *>(m_listView))->node();
  QTag *qTag = QuantaCommon::tagFromDTD(node);
  if (qTag)
  {
    if (!attr)
        attr = qTag->attribute(title);
    if (attr)
    {
      combo->insertStringList(attr->values);
      if (attr->source == "dcop") //fill the list with a result of a DCOP call
      {
        QString interface = "QuantaIf";
        if (!attr->interface.isEmpty())
          interface = attr->interface;
        QString arguments = attr->arguments;
        arguments.replace("%tagname%", node->tag->name);
        DCOPReply reply = QuantaCommon::callDCOPMethod(interface, attr->method, arguments);
        if (reply.isValid())
        {
            QStringList list = reply;
            combo->insertStringList(list);
        }
      }

    }
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

QString AttributeListItem::editorText(int)
{
  return combo->currentText();
}

void AttributeListItem::showEditor(int)
{
  placeEditor(combo);
  combo->show();
  int index = -1;
  Node *node = (static_cast<TagAttributeTree *>(m_listView))->node();
  QTag *qTag = QuantaCommon::tagFromDTD(node);
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

void AttributeListItem::hideEditor(int)
{
  m_listView->editorContentChanged();
  setText(1, combo->currentText());
  combo->hide();
}

//editable color combobox
AttributeColorItem::AttributeColorItem(EditableTree* listView, QListViewItem* parent, const QString &title, const QString& title2)
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

QString AttributeColorItem::editorText(int)
{
  QString name =  combo->color().name();
  if (name == "#000000")
      name = "";
  combo->setCurrentText(name);
  return name;
}

void AttributeColorItem::showEditor(int)
{
  placeEditor(combo);
  combo->show();
  combo->setColor(text(1));
  combo->setCurrentText(text(1));
  combo->setFocus();
}

void AttributeColorItem::hideEditor(int)
{
  m_listView->editorContentChanged();
  QString name =  combo->color().name();
  if (name == "#000000")
      name = "";
  setText(1, name);
  combo->hide();
}

//editable style combobox
AttributeStyleItem::AttributeStyleItem(EditableTree* listView, QListViewItem* parent, const QString &title, const QString& title2)
: AttributeItem(parent, title, title2)
{
  m_listView = listView;
  m_se = new StyleEditor( m_listView->viewport() );
  m_se->setButtonIcon(4,0);
  //combo->setEditable(true);
  m_se->hide();

  QObject::connect( m_se->button(), SIGNAL( clicked() ), m_listView, SLOT( editorContentChanged() ) );
 }

AttributeStyleItem::~AttributeStyleItem()
{
  delete m_se;
}

QString AttributeStyleItem::editorText(int)
{
   return m_se->lineEdit()->text();
}

void AttributeStyleItem::showEditor(int)
{
  placeEditor(m_se);
  m_se->show();
  m_se->lineEdit()->setText(text(1));
  m_se->setFocus();
}

void AttributeStyleItem::hideEditor(int)
{
  m_listView->editorContentChanged();
  setText(1, m_se->lineEdit()->text());
  m_se->hide();
}

