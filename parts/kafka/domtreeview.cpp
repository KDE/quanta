/***************************************************************************
                               domtreeview.cpp
                             -------------------

    copyright            : (C) 2001 - The Kafka Team
    email                : kde-kafka@master.kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "kafkacommon.h"
#ifdef HEAVY_DEBUG

#include <kdebug.h>
#include <khtml_part.h>
#include <klocale.h>
#include <qstring.h>
#include <qlayout.h>
#include <dom/dom_text.h>

#include "domtreeview.moc"

DOMTreeView::DOMTreeView(QWidget *parent, KHTMLPart *currentpart, const char * name) : KListView(parent, name)
{
    setCaption(name);
    setRootIsDecorated(true);
    addColumn(i18n( "Name" ));
    addColumn(i18n( "Value (limited to 20 char)" ));
    addColumn(i18n( "Length" ));
    addColumn(i18n( "ID" ));
    addColumn("");
    setSorting(-1);
    part = currentpart;
    connect(part, SIGNAL(nodeActivated(const DOM::Node &)), this, SLOT(showTree(const DOM::Node &)));
    connect(this, SIGNAL(clicked(QListViewItem *)), this, SLOT(slotItemClicked(QListViewItem *)));
    m_nodedict.setAutoDelete(true);
    title = "";
    titleItem = new QListViewItem(static_cast<QListView *>(this), title, "");
}

DOMTreeView::~DOMTreeView()
{
    disconnect(part);
}

void DOMTreeView::setTitle(const QString &str)
{
  title = str;
  titleItem->setText(1, title);
}

void DOMTreeView::showTree(const DOM::Node &pNode)
{

//    if(pNode.isNull() || document != pNode.ownerDocument())
//    {
	clear();
	m_itemdict.clear();
	m_nodedict.clear();
	if(pNode.isNull())
	    return;
	if(pNode.firstChild() == 0)
		return;
	else if(pNode.ownerDocument().isNull())
	{
	    document = pNode.ownerDocument();
	    recursive(0, pNode);
	}
	else
	{
	    document = pNode.ownerDocument();
	    recursive(0, pNode.ownerDocument());
	}
	titleItem = new QListViewItem(static_cast<QListView *>(this), title, "");
//    }
    setCurrentItem(m_itemdict[pNode.handle()]);
    ensureItemVisible(m_itemdict[pNode.handle()]);
}

void DOMTreeView::recursive(const DOM::Node &pNode, const DOM::Node &node)
{
    QListViewItem *cur_item;
    int len;
    if(pNode.ownerDocument() != document)
    {
	QString val = node.nodeValue().string();
	if ( val.length() > 20 )
	    val.truncate( 20 );
	cur_item = new QListViewItem(static_cast<QListView *>(this), node.nodeName().string(), val);
	document = pNode.ownerDocument();
    }
    else {
	QString val = node.nodeValue().string();
	if ( val.length() > 20 )
	    val.truncate( 20 );
	if(node.nodeType() == DOM::Node::TEXT_NODE)
	    len = (static_cast<DOM::CharacterData>(node)).length();
	else
	    len = 0;
	cur_item = new QListViewItem(m_itemdict[pNode.handle()], node.nodeName().string(), val, QString::number(len), QString::number(node.elementId()) );
	unsigned long i;
	QListViewItem *tmp = new QListViewItem(cur_item, "properties");
	for(i = 0; i < node.attributes().length(); i++)
	{
		new QListViewItem(tmp, node.attributes().item(i).nodeName().string(),
			node.attributes().item(i).nodeValue().string());
	}
    }

    if(node.handle())
    {
	m_itemdict.insert(node.handle(), cur_item);
	m_nodedict.insert(cur_item, new DOM::Node(node));
    }

    DOM::Node cur_child = node.lastChild();
    while(!cur_child.isNull())
    {
	recursive(node, cur_child);
	cur_child = cur_child.previousSibling();
    }
}

void DOMTreeView::slotItemClicked(QListViewItem *cur_item)
{
    DOM::Node *handle = m_nodedict[cur_item];
    if(handle) {
	emit part->setActiveNode(*handle);
        //kdDebug() << handle->toHTML() << endl;
    }
}

KafkaDOMTreeDialog::KafkaDOMTreeDialog(QWidget *parent, KHTMLPart *part, const char* name, bool modal, WFlags fl )
	: QDialog(parent, name, modal, fl)
{
	setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)1, 0, 0, sizePolicy().hasHeightForWidth() ) );
	DialogLayout = new QGridLayout( this, 1, 1, 11, 6, "DialogLayout");
	domview = new DOMTreeView(this, part, name);
	domview->setTitle(i18n( "Debugging KafkaWidget DOM Tree " ));
        DialogLayout->addWidget(domview, 1,1);

}

KafkaDOMTreeDialog::~KafkaDOMTreeDialog()
{

}

#endif

