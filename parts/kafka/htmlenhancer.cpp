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
#include <kconfig.h>

#include "../../parser/tag.h"
#include "../../parser/node.h"
#include "wkafkapart.h"
#include "kafkacommon.h"

#include "htmlenhancer.h"

HTMLEnhancer::HTMLEnhancer(WKafkaPart *_wkafkapart)
	: NodeEnhancer()
{
	m_showIconForScripts = true;
	m_wkafkapart = _wkafkapart;
	m_stddirs = new KStandardDirs();
}

HTMLEnhancer::~HTMLEnhancer()
{
	delete m_stddirs;
}

bool HTMLEnhancer::enhanceNode(Node *node, DOM::Node parentDNode, DOM::Node nextDNode)
{
	DOM::Node domNode, domNode2, attr;
	bool tbody, goUp;
	Node *n;
	QString script, filename, text;

	//FIRST update the src attr with the baseURL
	domNode = node->_rootNode.attributes().getNamedItem("src");
	if(!domNode.isNull())
	{
		domNode.setNodeValue(DOM::DOMString(m_baseURL.url()) + domNode.nodeValue());
	}

	//THEN if it is the style element, add a DOM::Node::TEXT_NODE child gathering all the CSS
	//by default, the parser parse it as a script, which can't be translated in DOM::Nodes.
	if(node->tag->type == Tag::ScriptTag && node->tag->name.lower().contains("style"))
	{
		domNode =  m_wkafkapart->getKafkaPart()->createNode("style");
		try
		{
			parentDNode.appendChild(domNode);
		}
		catch(DOM::DOMException e)
		{
			kdDebug(25001)<< "HTMLEnhancer::enhanceNode() ERROR 1" << endl;
			return false;
		}
		node->_rootNode = domNode;
		node->_leafNode = domNode;
		m_wkafkapart->connectDomNodeToQuantaNode(domNode, node);
		n = node->child;
		text = "";
		goUp = false;
		while(n)
		{
			text += n->tag->tagStr();
			n = kafkaCommon::getNextNode(n, goUp, node);
		}
#ifdef HEAVY_DEBUG
		kdDebug(25001)<< "HTMLTranslator::translateNode() - CSS code : " << text << endl;
#endif
		domNode2 = m_wkafkapart->getKafkaPart()->createNode("TEXT");
		domNode2.setNodeValue(DOM::DOMString(text));
		try
		{
			domNode.appendChild(domNode2);
		}
		catch(DOM::DOMException e)
		{
			kdDebug(25001)<< "HTMLEnhancer::enhanceNode() ERROR 2" << endl;
			return false;
		}
		m_wkafkapart->connectDomNodeToQuantaNode(domNode2, node);
	}

	//THEN replace, if asked, scripts by a little icon.
	if(node->tag->type == Tag::ScriptTag && m_showIconForScripts)
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

	//THEN add a red dotted border to FORM tags.
	if(!node->_rootNode.isNull() && node->_rootNode.nodeName().string().lower() == "form")
	{
		attr = m_wkafkapart->getKafkaPart()->htmlDocument().createAttribute("style");
		attr.setNodeValue("border: 1px dotted red");
		node->_rootNode.attributes().setNamedItem(attr);
	}

	//THEN add a blue dotted border to DL tags
	if(!node->_rootNode.isNull() && node->_rootNode.nodeName().string().lower() == "dl")
	{
		attr = m_wkafkapart->getKafkaPart()->htmlDocument().createAttribute("style");
		attr.setNodeValue("border: 1px dotted blue");
		node->_rootNode.attributes().setNamedItem(attr);
	}

	return true;
}

void HTMLEnhancer::readConfig(KConfig *m_config)
{
	m_config->setGroup("HTML Enhancer");
	m_showIconForScripts = m_config->readBoolEntry("Show Scripts Icons", true);
}
