/***************************************************************************
                               kafkacommon.cpp
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

#include <kdebug.h>

#include "../../parser/node.h"
#include "../../parser/tag.h"
#include "../../document.h"
#include "../../resource.h"
#include "../../quantacommon.h"
#include "undoredo.h"

#include "kafkacommon.h"

Node *kafkaCommon::getNextNode(Node *node, bool &goUp, Node *endNode)
{
	//goto next node, my favorite part :)
	if(!node) return 0L;
	if(goUp)
	{
		if(node->next)
		{
			goUp = false;
			if(node->next == endNode)
				return 0L;
			return node->next;
		}
		else
		{
			if(node->parent == endNode)
				return 0L;
			return getNextNode(node->parent, goUp);
		}
	}
	else
	{
		if(node->child)
		{
			if(node->child == endNode)
				return 0L;
			return node->child;
		}
		else if(node->next)
		{
			if(node->next == endNode)
				return 0L;
			return node->next;
		}
		else
		{
			goUp = true;
			if(node->parent == endNode)
				return 0L;
			return getNextNode(node->parent, goUp);
		}
	}
}

DOM::Node kafkaCommon::getNextDomNode(DOM::Node node, bool &goUp, bool returnParentNode, DOM::Node endNode)
{
	if(node.isNull())
		return 0L;
	if(node.hasChildNodes() && !goUp)
	{
		if(endNode == node.firstChild())
			return 0L;
		else
			return node.firstChild();
	}
	else if(!node.nextSibling().isNull())
	{
		goUp = false;
		if(endNode == node.nextSibling())
			return 0L;
		else
			return node.nextSibling();
	}
	else
	{
		goUp = true;
		if(node.parentNode().isNull() || endNode == node.parentNode())
			return 0L;
		if(returnParentNode)
			return node.parentNode();
		else
			return getNextDomNode(node.parentNode(), goUp, returnParentNode, endNode);
	}
}

void kafkaCommon::fitsNodesPosition(Node* startNode, int colMovement, int lineMovement, int colEnd, int lineEnd)
{
	bool b = false;
	int j, SNbeginLine, SNbeginCol/**, SNlastLine, SNlastCol*/;
	int beginLine, beginCol, lastLine, lastCol;
	Node *node = startNode;

	if(!node) return;

	node->tag->beginPos(SNbeginLine, SNbeginCol);
	//node->tag->endPos(SNlastLine, SNlastCol);

	while(node)
	{
		node->tag->beginPos(beginLine, beginCol);
		node->tag->endPos(lastLine, lastCol);
		if(beginLine >= lineEnd && beginCol >= colEnd &&
			colEnd != -2 && lineEnd != -2)
			return;
		if(beginLine == SNbeginLine && lastLine == SNbeginLine)
			node->tag->setTagPosition(beginLine + lineMovement,
				beginCol + colMovement, lastLine + lineMovement,
				lastCol + colMovement);
		else if(beginLine == SNbeginLine)//&&lastLine != SNbeginLine
			node->tag->setTagPosition(beginLine + lineMovement,
				beginCol + colMovement, lastLine + lineMovement,
				lastCol);
		else
			node->tag->setTagPosition(beginLine + lineMovement,
				beginCol, lastLine + lineMovement, lastCol);
		for(j = 0; j < node->tag->attrCount(); j++)
		{
			if(node->tag->getAttribute(j).nameLine == SNbeginLine)
			{
				node->tag->getAttribute(j).nameLine += lineMovement;
				node->tag->getAttribute(j).nameCol += colMovement;
				node->tag->getAttribute(j).valueLine += lineMovement;
				node->tag->getAttribute(j).valueCol += colMovement;
			}
			else
			{
				node->tag->getAttribute(j).nameLine += lineMovement;
				node->tag->getAttribute(j).valueLine += lineMovement;
			}
		}
		node = getNextNode(node, b);
	}
}

void kafkaCommon::coutTree(Node *node, int indent)
{
#ifdef HEAVY_DEBUG
	QString output;
	int bLine, bCol, eLine, eCol, j;
	if(!node)
		kdDebug(24000)<< "kafkaCommon::coutTree() - bad node!" << endl;
	while (node)
	{
		output = "";
		output.fill('.', indent);
		node->tag->beginPos(bLine, bCol);
		node->tag->endPos(eLine, eCol);
		if (node->tag->type != Tag::Text)
			output += node->tag->name.replace('\n'," ");
		else
			output+= node->tag->tagStr().replace('\n'," ");
		kdDebug(24000) << output <<" (" << node->tag->type << ") at pos " <<
			bLine << ":" << bCol << " - " << eLine << ":" << eCol << endl;
		for(j = 0; j < node->tag->attrCount(); j++)
		{
			kdDebug(24000)<< " attr" << j << " " <<
				node->tag->getAttribute(j).nameLine << ":" <<
				node->tag->getAttribute(j).nameCol << " - " <<
				node->tag->getAttribute(j).valueLine << ":" <<
				node->tag->getAttribute(j).valueCol << endl;
		}

		if (node->child)
			coutTree(node->child, indent + 4);
		node = node->next;
	}
#endif
}

QValueList<int> kafkaCommon::getLocation(Node * node)
{
	QValueList<int> loc;
	int i = 0;

	while(node)
	{
		i = 1;
		while(node->prev)
		{
			i++;
			node = node->prev;
		}
		loc.prepend(i);
		node = node->parent;
	}
	return loc;
}
	
Node* kafkaCommon::getNodeFromLocation(QValueList<int> loc)
{
	QValueList<int>::iterator it;
	Node *node = baseNode;
	Node *m = 0L;
	int i;

	if(!node) return 0L;
	for(it = loc.begin(); it != loc.end(); it++)
	{
		if(!node)
			return 0L;
		for(i = 1; i < (*it); i++)
		{
			if(!node->next)
				return 0L;
			node = node->next;
		}
		m = node;
		node = node->child;
	}
	return m;
}

Node *kafkaCommon::createAndInsertNode(QString nodeName, int nodeType, Document *doc,
	Node *parent, Node *nextSibling, NodeModifsSet &modifs)
{
	Node *leftNode, *rightNode, *node, *nodeEnd;
	Node *n;
	NodeModif modif, modif2;
	bool endTagBuild = false;

	node = new Node(parent);
	node->tag = new Tag();
	node->tag->dtd = doc->defaultDTD();
	node->tag->setWrite(doc);
	node->tag->type = nodeType;
	node->tag->name = QuantaCommon::tagCase(nodeName);
	node->tag->single = QuantaCommon::isSingleTag(doc->defaultDTD()->name, nodeName);
	node->tag->cleanStrBuilt = false;
	modif.type = undoRedo::NodeAdded;
	modif.node = 0L;
	modif.childsNumber = 0;
	modif.childsNumber2 = 0;
	leftNode = node;

	if(!node->tag->single && nodeType == Tag::XmlTag)
	{
		nodeEnd = new Node(parent);
		nodeEnd->prev = node;
		node->next = nodeEnd;
		nodeEnd->tag = new Tag();
		nodeEnd->tag->setWrite(doc);
		nodeEnd->tag->type = Tag::XmlTagEnd;
		nodeEnd->tag->name = QuantaCommon::tagCase("/" + nodeName);
		nodeEnd->closesPrevious = true;
		nodeEnd->tag->single = false;
		nodeEnd->tag->dtd = doc->defaultDTD();
		nodeEnd->tag->cleanStrBuilt = false;
		modif2.type = undoRedo::NodeAdded;
		modif2.node = 0L;
		modif2.childsNumber = 0;
		modif2.childsNumber2 = 0;
		rightNode = nodeEnd;
		endTagBuild = true;
	}
	else
		rightNode = node;

	//place the Nodes
	if(parent)
		n = parent->child;
	else
		n = baseNode;
	while(n && n->next)
		n = n->next;

	if(!parent && (!nextSibling || (nextSibling && !nextSibling->prev)))
		baseNode = node;
	if(parent && !parent->child)
		parent->child = leftNode;
	if(nextSibling && nextSibling->prev)
		nextSibling->prev->next = leftNode;
	else if(n)
		n->next = leftNode;
	leftNode->prev = nextSibling?nextSibling->prev:n;
	if(nextSibling)
		nextSibling->prev = rightNode;
	rightNode->next = nextSibling;

	modif.location = kafkaCommon::getLocation(node);
	modifs.NodeModifList.append(modif);
	if(endTagBuild)
	{
		modif2.location = kafkaCommon::getLocation(nodeEnd);
		modifs.NodeModifList.append(modif2);
	}

	return node;
}

void kafkaCommon::extractAndDeleteNode(Node *node, NodeModifsSet &modifs)
{
	NodeModif modif;

	modif.type = undoRedo::NodeAndChildsRemoved;
	modif.location = kafkaCommon::getLocation(node);
	modif.node = node;
	modif.childsNumber = 0;
	modif.childsNumber2 = 0;

	if(node == baseNode)
		baseNode = 0L;
	if(node->parent && node->parent->child == node)
		node->parent->child = node->next;
	node->parent = 0L;
	if(node->prev)
		node->prev->next = node->next;
	if(node->next)
		node->next->prev = node->prev;
	node->prev = 0L;
	node->next = 0L;

	modifs.NodeModifList.append(modif);
}

int kafkaCommon::getNodeType(QString nodeName)
{
	nodeName = nodeName.lower();
	if(nodeName == "html" || nodeName == "head" || nodeName == "meta" || nodeName == "title" ||
		nodeName == "link" || nodeName == "style" || nodeName == "script" || nodeName == "option" ||
		nodeName == "optgroup" || nodeName == "area" || nodeName == "param" || nodeName == "noframes")
		return kafkaCommon::noneDisplay;
	else if(nodeName == "body" || nodeName == "p" || nodeName == "div" || nodeName == "address" ||
		nodeName == "blockquote" || nodeName == "iframe" || nodeName == "object" || nodeName == "applet" ||
		nodeName == "center" || nodeName == "hr" || nodeName == "map" || nodeName == "h1" ||
		nodeName == "h2" || nodeName == "h3" || nodeName == "h4" || nodeName == "h5" ||
		nodeName == "h6" || nodeName == "table" || nodeName == "thead" || nodeName == "tbody" ||
		nodeName == "tfoot" || nodeName == "" || nodeName == "col" || nodeName == "colgroup" ||
		nodeName == "tr" || nodeName == "td" || nodeName == "th" || nodeName == "caption" ||
		nodeName == "ul" || nodeName == "menu" || nodeName == "dir" || nodeName == "ol" ||
		nodeName == "li" || nodeName == "ul" || nodeName == "dd" || nodeName == "dl" ||
		nodeName == "dt" || nodeName == "form" || nodeName == "legend" || nodeName == "fieldset" ||
		nodeName == "button" || nodeName == "pre" || nodeName == "input" || nodeName == "select" ||
		nodeName == "frameset" || nodeName == "frame")
		return kafkaCommon::blockDisplay;
	else if(nodeName == "q" || nodeName == "u" || nodeName == "ins" || nodeName == "i" ||
		nodeName == "cite" || nodeName == "em" || nodeName == "var" || nodeName == "address" ||
		nodeName == "tt" || nodeName == "code" || nodeName == "kbd" || nodeName == "samp" ||
		 nodeName == "big" || nodeName == "small" || nodeName == "s" || nodeName == "strike" ||
		 nodeName == "del" || nodeName == "sub" || nodeName == "sup" || nodeName == "abbr" ||
		 nodeName == "acronym" || nodeName == "a" || nodeName == "bdo")
		 return kafkaCommon::inlineDisplay;
	else
	{
		kdDebug(25001)<< "kafkaCommon::getNodeType() - ERROR node Name not found" << endl;
		return kafkaCommon::noneDisplay;
	}
}
