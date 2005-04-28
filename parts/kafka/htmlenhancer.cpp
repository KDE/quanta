/***************************************************************************
                              htmltranslator.cpp
                             -------------------

    copyright            : (C) 2003, 2004 - Nicolas Deschildre
    email                : ndeschildre@kdewebdev.org
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

#include "quantacommon.h"
#include "document.h"
#include "tag.h"
#include "node.h"
#include "wkafkapart.h"
#include "nodeproperties.h"
#include "kafkacommon.h"
#include "qextfileinfo.h"
#include "viewmanager.h"

#include "htmlenhancer.h"

HTMLEnhancer::HTMLEnhancer(KafkaDocument *_wkafkapart)
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
	DOM::Node domNode, domNode2, attr, *ptDomNode;
	bool tbody, goUp;
	Node *n;
	QString script, filename, text, oldName;
	KURL url, baseURL;
	int oldType;

	//FIRST update the src attr with the baseURL
	if(node->rootNode())
	{
		domNode = node->rootNode()->attributes().getNamedItem("src");
		if(!domNode.isNull())
		{
			baseURL.setPath(ViewManager::ref()->activeDocument()->url().directory());
			QuantaCommon::setUrl(url, domNode.nodeValue().string());
			url = QExtFileInfo::toAbsolute(url, baseURL);
			domNode.setNodeValue(url.url());
#ifdef HEAVY_DEBUG
			kdDebug(25001)<< "HTMLTranslator::translateNode() - new src : " << url.url() << endl;
#endif
		}
	}
        
        //THEN update the href attr of the LINK node with the baseURL
        if(node->tag->name.lower() == "link" && node->rootNode())
        {
          domNode = node->rootNode()->attributes().getNamedItem("href");
          if(!domNode.isNull())
          {
            baseURL.setPath(ViewManager::ref()->activeDocument()->url().directory());
            QuantaCommon::setUrl(url, domNode.nodeValue().string());
            url = QExtFileInfo::toAbsolute(url, baseURL);
            domNode.setNodeValue(url.url());
#ifdef HEAVY_DEBUG
            kdDebug(25001)<< "HTMLTranslator::translateNode() - new href : " << url.url() << endl;
#endif        
          }
        }

	//THEN if it is the style element, add a DOM::Node::TEXT_NODE child gathering all the CSS
	//by default, the parser parse it as a script, which can't be translated in DOM::Nodes.
	if((node->tag->type == Tag::XmlTag && node->tag->name.lower() == "style") ||
		(node->tag->type == Tag::ScriptTag && node->tag->name.lower().contains("style") != 0))
	{
		//If the style Node doesn't exists, create it
		if(!node->rootNode())
		{
			oldType = node->tag->type;
			node->tag->type = Tag::XmlTag;
			oldName = node->tag->name;
			node->tag->name = "style";
			m_wkafkapart->buildKafkaNodeFromNode(node);
			node->tag->type = oldType;
			node->tag->name = oldName;
		}

		if(node->rootNode())
		{
			domNode = *node->rootNode();
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
			domNode2 = kafkaCommon::createTextDomNode(text, m_wkafkapart->getKafkaWidget()->document());
			if(!kafkaCommon::insertDomNode(domNode2, domNode))
				return false;
			m_wkafkapart->connectDomNodeToQuantaNode(domNode2, node);
		}
	}
    
    QTag* qTag = QuantaCommon::tagFromDTD(m_wkafkapart->getCurrentDoc()->defaultDTD(),
                                          parentDNode.nodeName().string());

	//THEN replace, if asked, scripts by a little icon.
    if(node->tag->type == Tag::ScriptTag && m_showIconForScripts && qTag->isChild("IMG", false))
	{
		script = node->tag->name.left(node->tag->name.find("block", 0, false) - 1).lower();
#ifdef LIGHT_DEBUG
		kdDebug(25001)<< "HTMLTranslator::translateNode() - BLOCK:" << script << ":" << endl;
#endif
		filename = m_stddirs->findResource("data", "kafkapart/pics/" + script + ".png" );
		if(!filename.isEmpty())
		{
			//FIXME DTD!
			domNode = kafkaCommon::createDomNode("IMG", m_wkafkapart->defaultDTD(),
				m_wkafkapart->getKafkaWidget()->document());
				
			kafkaCommon::editDomNodeAttribute(domNode, "IMG", m_wkafkapart->defaultDTD(), "src",
				filename, m_wkafkapart->getKafkaWidget()->document());

			//Add a tooltip indicating the content of the script
			n = node->child;
			text = "";
			goUp = false;
			while(n && n != node)
			{
				text += n->tag->tagStr();
 				n = kafkaCommon::getNextNode(n, goUp, node);
			}
			//if(text == "")
			//	text = i18n("Empty")
			kafkaCommon::editDomNodeAttribute(domNode, "img", m_wkafkapart->defaultDTD(),
				"title", text, m_wkafkapart->getKafkaWidget()->document());

			if(!kafkaCommon::insertDomNode(domNode, parentDNode, nextDNode))
				return false;
			m_wkafkapart->connectDomNodeToQuantaNode(domNode, node);
		}
	}
	
	//THEN if it is a comment, add a little icon ;o)
    if(node->tag->type == Tag::Comment && m_showIconForScripts && qTag->isChild("IMG", false))
	{
#ifdef LIGHT_DEBUG
		kdDebug(25001)<< "HTMLTranslator::translateNode() - Comment" << endl;
#endif
            
        filename = m_stddirs->findResource("data", "kafkapart/pics/comment.png" );
        if(!filename.isEmpty())
        {
            //FIXME DTD!
            domNode = kafkaCommon::createDomNode("IMG", m_wkafkapart->defaultDTD(),
                m_wkafkapart->getKafkaWidget()->document());
            kafkaCommon::editDomNodeAttribute(domNode, "IMG", m_wkafkapart->defaultDTD(), "src",
                filename, m_wkafkapart->getKafkaWidget()->document());

            //Add a tooltip indicating the content of the script
            n = node->child;
            text = "";
            goUp = false;
            while(n && n != node)
            {
                text += n->tag->tagStr();
                n = kafkaCommon::getNextNode(n, goUp, node);
            }
            //if(text == "")
            //	text = i18n("Empty")
            kafkaCommon::editDomNodeAttribute(domNode, "img", m_wkafkapart->defaultDTD(),
                "title", text, m_wkafkapart->getKafkaWidget()->document());

            if(!kafkaCommon::insertDomNode(domNode, parentDNode, nextDNode))
                return false;
            m_wkafkapart->connectDomNodeToQuantaNode(domNode, node);
        }
    }

	//THEN add a TBODY tag if necessary
	if(node->rootNode() && node->rootNode()->nodeName().string().lower() == "table")
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
			domNode = kafkaCommon::createDomNode("TBODY", m_wkafkapart->defaultDTD(),
				m_wkafkapart->getKafkaWidget()->htmlDocument());
			if(!kafkaCommon::insertDomNode(domNode, *node->rootNode()))
				return false;
			m_wkafkapart->connectDomNodeToQuantaNode(domNode, node);
			ptDomNode = new DOM::Node(domNode);
			node->setLeafNode(ptDomNode);
		}
	}

	//THEN add a red dotted border to FORM tags.
	if(node->rootNode() && node->rootNode()->nodeName().string().lower() == "form")
	{
		kafkaCommon::editDomNodeAttribute(*node->rootNode(), node, "style", "border: 1px dotted red",
			m_wkafkapart->getKafkaWidget()->document());
	}

    // THEN add a tooltip indicating the content of the name attribute
    if(node->rootNode() && node->rootNode()->nodeName().string().lower() == "input")
    {
        domNode = *(node->rootNode());
        QString text = node->tag->attributeValue("name");
        kafkaCommon::editDomNodeAttribute(domNode, "input", m_wkafkapart->defaultDTD(),
                                          "title", text, m_wkafkapart->getKafkaWidget()->document());
    }

	//THEN add a blue dotted border to DL, OL, UL tags
	if(node->rootNode())
	{
		text = node->rootNode()->nodeName().string().lower();
		if(text == "dl" || text == "ol" || text == "ul")
		{
			kafkaCommon::editDomNodeAttribute(*node->rootNode(), node, "style", "border: 1px dotted blue",
				m_wkafkapart->getKafkaWidget()->document());
		}
	}
    
	//THEN add a minimal border for borderless tables
	//TODO: make it configurable, and look if CSS hasn't defined a border first
	if(node->rootNode() && node->rootNode()->nodeName().string().lower() == "table")
	{
		attr = node->rootNode()->attributes().getNamedItem("border");
		if(attr.isNull() || (!attr.isNull() && attr.nodeValue().string() == "0"))
		{
			kafkaCommon::editDomNodeAttribute(*node->rootNode(), node, "border", "1",
				m_wkafkapart->getKafkaWidget()->document());
		}
	}

    //THEN add a blue dotted border to DIV tags
    if(node->rootNode())
    {
        text = node->rootNode()->nodeName().string().lower();
        if(text == "div")
        {
            kafkaCommon::editDomNodeAttribute(*node->rootNode(), node, "style", "border: 1px dotted green",
                                               m_wkafkapart->getKafkaWidget()->document());
        }
    }
    
    return true;
}

void HTMLEnhancer::postEnhanceNode(DOM::Node domNode)
{
	DOM::Node textNode;
	kNodeAttrs *props;
	QTag *qTag;
	bool isInline;

	if(domNode.isNull())
		return;

	//If domNode is a Block and there is no text around, and if domNode's parent can handle
	//text or a P tag, add an empty text DOM::Node
	// so that the user can access this area.
	qTag = QuantaCommon::tagFromDTD(m_wkafkapart->getCurrentDoc()->defaultDTD(),
		domNode.nodeName().string());
	isInline = kafkaCommon::isInline(domNode.nodeName().string());
	if(domNode.nodeType() == DOM::Node::ELEMENT_NODE &&
		(!isInline || (isInline && qTag && qTag->isSingle())))
	{
		qTag = QuantaCommon::tagFromDTD(m_wkafkapart->getNode(domNode.parentNode()));

		if((domNode.nextSibling().isNull() ||
			(!domNode.nextSibling().isNull() &&
			domNode.nextSibling().nodeType() == DOM::Node::ELEMENT_NODE &&
			!kafkaCommon::isInline(domNode.nextSibling().nodeName().string())))
			&& qTag && (qTag->isChild("#text", false) || qTag->isChild("p", false)) &&
			domNode.nodeName().string().lower() != "p")
		{
			textNode = kafkaCommon::createTextDomNode("",
				m_wkafkapart->getKafkaWidget()->document());
			props = m_wkafkapart->connectDomNodeToQuantaNode(textNode, 0L);
			props->setIsLinkedToNode(false);
			props->setSpecialBehavior(kNodeAttrs::emptyTextSurroundingBlockElementAtTheRight);
			kafkaCommon::insertDomNode(textNode, domNode.parentNode(),
				domNode.nextSibling());
		}

		if((domNode.previousSibling().isNull() || (!domNode.previousSibling().isNull() &&
			domNode.previousSibling().nodeType() == DOM::Node::ELEMENT_NODE &&
			!kafkaCommon::isInline(domNode.previousSibling().nodeName().string())))
			&& qTag && (qTag->isChild("#text", false) || qTag->isChild("p", false)) &&
			domNode.nodeName().string().lower() != "p")
		{
			textNode = kafkaCommon::createTextDomNode("",
				m_wkafkapart->getKafkaWidget()->document());
			props = m_wkafkapart->connectDomNodeToQuantaNode(textNode, 0L);
			props->setIsLinkedToNode(false);
			props->setSpecialBehavior(kNodeAttrs::emptyTextSurroundingBlockElementAtTheLeft);
			kafkaCommon::insertDomNode(textNode, domNode.parentNode(),
				domNode);
		}
	}

	//If domNode is an childless element, and if it can handle Text or a P tag,
	//add an empty text DOM::Node so that the
	//user can access this area.
	qTag = QuantaCommon::tagFromDTD(m_wkafkapart->getNode(domNode));
	if(domNode.nodeType() == DOM::Node::ELEMENT_NODE &&
		!domNode.hasChildNodes() && qTag && (qTag->isChild("#text", false) ||
		qTag->isChild("p", false)))
	{
		textNode = kafkaCommon::createTextDomNode("",
			m_wkafkapart->getKafkaWidget()->document());
		props = m_wkafkapart->connectDomNodeToQuantaNode(textNode, 0L);
		props->setIsLinkedToNode(false);
		props->setSpecialBehavior(kNodeAttrs::emptyTextAsChildOfAChildlessElement);
		kafkaCommon::insertDomNode(textNode, domNode);
	}
}

void HTMLEnhancer::postUnenhanceNode(DOM::Node domNode)
{
	DOM::Node child, next;
	kNodeAttrs *attrs;

	if(domNode.isNull())
		return;

	//Try to remove the EmptyTextAsChildOfAChildlessElement Node first if present
	if(domNode.hasChildNodes())
	{
		child = domNode.firstChild();
		while(!child.isNull())
		{
			attrs = m_wkafkapart->getAttrs(child);
			next = child.nextSibling();
			if(attrs && attrs->specialBehavior() == kNodeAttrs::emptyTextAsChildOfAChildlessElement)
				kafkaCommon::removeDomNode(child);
			child = next;
		}
	}

	//Then try to remove the emptyTextSurroundingBlockElement* Nodes if present.
	if(!domNode.previousSibling().isNull())
	{
		attrs = m_wkafkapart->getAttrs(domNode.previousSibling());
		if(attrs && attrs->specialBehavior() == kNodeAttrs::emptyTextSurroundingBlockElementAtTheLeft)
			kafkaCommon::removeDomNode(domNode.previousSibling());
	}
	if(!domNode.nextSibling().isNull())
	{
		attrs = m_wkafkapart->getAttrs(domNode.nextSibling());
		if(attrs && attrs->specialBehavior() == kNodeAttrs::emptyTextSurroundingBlockElementAtTheRight)
			kafkaCommon::removeDomNode(domNode.nextSibling());
	}
}

void HTMLEnhancer::readConfig(KConfig *m_config)
{
	m_config->setGroup("HTML Enhancer");
	m_showIconForScripts = m_config->readBoolEntry("Show Scripts Icons", true);
}
