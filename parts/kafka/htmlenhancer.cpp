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
		domNode = kafkaCommon::createDomNode("style", m_wkafkapart->getKafkaPart()->document());
		if(!kafkaCommon::insertDomNode(domNode, parentDNode))
			return false;
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
		domNode2 = kafkaCommon::createTextDomNode(text, m_wkafkapart->getKafkaPart()->document());
		if(!kafkaCommon::insertDomNode(domNode2, domNode))
			return false;
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
			domNode = kafkaCommon::createDomNode("IMG", m_wkafkapart->getKafkaPart()->document());
			kafkaCommon::editDomNodeAttribute(domNode, "src", filename,
				m_wkafkapart->getKafkaPart()->document());
			if(!kafkaCommon::insertDomNode(domNode, parentDNode, nextDNode))
				return false;
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
			domNode = kafkaCommon::createDomNode("TBODY", m_wkafkapart->getKafkaPart()->htmlDocument());
			if(!kafkaCommon::insertDomNode(domNode, node->_rootNode))
				return false;
			m_wkafkapart->connectDomNodeToQuantaNode(domNode, node);
			node->_leafNode = domNode;
		}
	}

	//THEN add a red dotted border to FORM tags.
	if(!node->_rootNode.isNull() && node->_rootNode.nodeName().string().lower() == "form")
	{
		kafkaCommon::editDomNodeAttribute(node->_rootNode, "style", "border: 1px dotted red",
			m_wkafkapart->getKafkaPart()->document());
	}

	//THEN add a blue dotted border to DL, OL, UL tags
	if(!node->_rootNode.isNull())
	{
		text = node->_rootNode.nodeName().string().lower();
		if(text == "dl" || text == "ol" || text == "ul")
		{
			kafkaCommon::editDomNodeAttribute(node->_rootNode, "style", "border: 1px dotted blue",
				m_wkafkapart->getKafkaPart()->document());
		}
	}

	//THEN add a minimal border for borderless tables
	//TODO: make it configurable, and look if CSS hasn't defined a border first
	if(!node->_rootNode.isNull() && node->_rootNode.nodeName().string().lower() == "table")
	{
		attr = node->_rootNode.attributes().getNamedItem("border");
		if(attr.isNull() || (!attr.isNull() && attr.nodeValue().string() == "0"))
		{
			kafkaCommon::editDomNodeAttribute(node->_rootNode, "border", "1",
				m_wkafkapart->getKafkaPart()->document());
		}
	}

	return true;
}

void HTMLEnhancer::readConfig(KConfig *m_config)
{
	m_config->setGroup("HTML Enhancer");
	m_showIconForScripts = m_config->readBoolEntry("Show Scripts Icons", true);
}
