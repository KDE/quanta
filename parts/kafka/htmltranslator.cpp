/***************************************************************************
                              htmltranslator.cpp
                             -------------------

    copyright            : (C) 2003 - Nicolas Deschildre
    email                : nicolasdchd@ifrance.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <dom/dom_node.h>
#include <dom/dom_string.h>
#include <dom/dom_exception.h>
#include <kdebug.h>
#include <kstandarddirs.h>

#include "../../parser/tag.h"
#include "../../parser/node.h"
#include "wkafkapart.h"
#include "kafkacommon.h"

#include "htmltranslator.h"

HTMLTranslator::HTMLTranslator(WKafkaPart *_wkafkapart)
	: NodeTranslator()
{
	m_showIconForScripts = true;
	m_wkafkapart = _wkafkapart;
	m_stddirs = new KStandardDirs();
}

HTMLTranslator::~HTMLTranslator()
{

}

bool HTMLTranslator::translateNode(Node *node, DOM::Node parentDNode, DOM::Node nextDNode)
{
	DOM::Node domNode, attr;
	bool tbody;
	Node *n;
	QString script, filename;

	//FIRST update the src attr with the baseURL
	domNode = node->_rootNode.attributes().getNamedItem("src");
	if(!domNode.isNull())
	{
		domNode.setNodeValue(DOM::DOMString(m_baseURL.url()) + domNode.nodeValue());
	}

	//THEN replace, if asked, scripts by a little icon.
	if(node->tag->type == Tag::ScriptTag)
	{
		script = node->tag->name.left(node->tag->name.find("block", 0, false) - 1).lower();
#ifdef LIGHT_DEBUG
		kdDebug(25001)<< "HTMLTranslator::translateNode() - BLOCK:" << script << ":" << endl;
#endif
		filename = m_stddirs->findResource("data", "kafkapart/pics/" + script + ".png" );
		if(!filename.isEmpty())
		{
			domNode = m_wkafkapart->getKafkaPart()->createNode("IMG");
			attr = m_wkafkapart->getKafkaPart()->htmlDocument().createAttribute("src");
			attr.setNodeValue(filename);
			domNode.attributes().setNamedItem(attr);
			if(!nextDNode.isNull())
			{
				try
				{
					parentDNode.insertBefore(domNode, nextDNode);
				} catch(DOM::DOMException e)
				{
					kdDebug(25001)<< "HTMLTranslator::translateNode - ERROR2 code : " <<
						e.code << endl;
					return false;
				}
			}
			else
			{
				try
				{
					parentDNode.appendChild(domNode);
				} catch(DOM::DOMException e)
				{
					kdDebug(25001)<< "HTMLTranslator::translateNode - ERROR3 code : " <<
						e.code << endl;
					return false;
				}
			}
			m_wkafkapart->connectDomNodeToQuantaNode(domNode, node);
		}
	}

	//THEN add a TBODY tag if necessary
	if(!node->_rootNode.isNull() && node->_rootNode.nodeName().string().lower() == "table")
	{
		tbody = false;
		n = node->child;
		while(n)
		{
			if(n->tag->name.lower() == "tbody")
				tbody = true;
			n = n->next;
		}
		if(!tbody)
		{
			domNode = m_wkafkapart->getKafkaPart()->createNode("TBODY");
			m_wkafkapart->connectDomNodeToQuantaNode(domNode, node);
			try
			{
				domNode = node->_rootNode.appendChild(domNode);
			} catch(DOM::DOMException e)
			{
				kdDebug(25001)<< "HTMLTranslator::translateNode - ERROR code : " <<
					e.code << endl;
				m_wkafkapart->disconnectDomNodeFromQuantaNode(domNode);
				return false;
			}
			node->_leafNode = domNode;
		}
	}
	return true;
}
