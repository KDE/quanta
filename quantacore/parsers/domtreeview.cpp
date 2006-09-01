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

// #include "khtml_part.h"
#include "domtreeview.moc"
// #include "xml/dom_nodeimpl.h"

DOMTreeView::DOMTreeView(QWidget *parent, /*KHTMLPart *currentpart,*/ const char * name) : K3ListView(parent)
{
    setWindowTitle(name);
    setRootIsDecorated(true);
    addColumn("Name");
    addColumn("Value");
    addColumn("Renderer");
    setSorting(-1);
//     part = currentpart;
//     connect(part, SIGNAL(nodeActivated(const DOM::Node &)), this, SLOT(showTree(const DOM::Node &)));
    connect(this, SIGNAL(clicked(Q3ListViewItem *)), this, SLOT(slotItemClicked(Q3ListViewItem *)));
    m_nodedict.setAutoDelete(true);
}

DOMTreeView::~DOMTreeView()
{
//     disconnect(part);
}

void DOMTreeView::showTree(const DOM::Node &pNode)
{
   if(pNode.isNull() || document != pNode.ownerDocument())
   {
	clear();
	m_itemdict.clear();
	m_nodedict.clear();
	if(pNode.isNull())
	    return;
	else if(pNode.ownerDocument().isNull())
	    recursive(0, pNode);
	else
	    recursive(0, pNode.ownerDocument());
    }
    setCurrentItem(m_itemdict[pNode.handle()]);
    ensureItemVisible(m_itemdict[pNode.handle()]);
}

void DOMTreeView::recursive(const DOM::Node &pNode, const DOM::Node &node)
{
    Q3ListViewItem *cur_item;
    if(pNode.isNull() || pNode.ownerDocument() != document)
    {
	QString val = node.nodeValue().string();
	if ( val.length() > 40 )
	    val.truncate( 40 );
	cur_item = new Q3ListViewItem(static_cast<Q3ListView *>(this), node.nodeName().string(), val );
	document = node.ownerDocument();
    }
    else {
	QString val = node.nodeValue().string();
	if ( val.length() > 40 )
	    val.truncate( 40 );
	cur_item = new Q3ListViewItem(m_itemdict[pNode.handle()], node.nodeName().string(), val);
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

void DOMTreeView::slotItemClicked(Q3ListViewItem *cur_item)
{
// // /*    DOM::Node *handle = m_nodedict[cur_item];
// //     if(handle) {
// // 	emit part->setActiveNode(*handle);
// //     }*/
}
