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

#include <qptrdict.h>

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
	QString output, dots;
	int bLine, bCol, eLine, eCol, j;
	if(!node)
		kdDebug(25001)<< "kafkaCommon::coutTree() - bad node!" << endl;
	while (node)
	{
		dots = "";
		dots.fill('.', indent);
		output = dots;
		node->tag->beginPos(bLine, bCol);
		node->tag->endPos(eLine, eCol);
		if (node->tag->type != Tag::Text)
			output += node->tag->name.replace('\n'," ");
		else
			output+= node->tag->tagStr().replace('\n'," ");
		kdDebug(25001) << output <<" (" << node->tag->type << ") "<<
			node << " at pos " << bLine << ":" << bCol << " - " <<
			eLine << ":" << eCol << endl;
		kdDebug(25001)<< dots << "prev " << node->prev << " next " << node->next << " parent " <<
			node->parent << " child " << node->child << endl;
		for(j = 0; j < node->tag->attrCount(); j++)
		{
			kdDebug(21005)<< " attr" << j << " " <<
				node->tag->getAttribute(j).nameLine << ":" <<
				node->tag->getAttribute(j).nameCol << " - " <<
				node->tag->getAttribute(j).valueLine << ":" <<
				node->tag->getAttribute(j).valueCol << endl;
		}

		if (node->child)
			coutTree(node->child, indent + 4);
		if(node == node->next || (node->next && node == node->next->next) ||
			(node->next && node->next->next && node == node->next->next->next) ||
			(node->next && node->next->next && node->next->next->next &&
			node == node->next->next->next->next) || (node->next && node->next->next &&
			node->next->next->next && node->next->next->next->next && node ==
			node->next->next->next->next->next))
		{
			//try to detect invalid pointers.
			kdDebug(25001)<< "ERROR - node == node->[..]next" << endl;
			return;
		}
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

Node* kafkaCommon::getNodeFromSubLocation(QValueList<int> loc, int locOffset)
{
	QValueList<int>::iterator it = loc.begin();
	QValueList<int> list;
	int i;

	for(i = 0; i < locOffset; i++)
	{
		list.append((*it));
		it++;
	}

	return getNodeFromLocation(list);
}

Node* kafkaCommon::createNode(QString nodeName, QString tagString, int nodeType, Document *doc)
{
	Node *node;

	//create the Node.
	node = new Node(0L);

	//Create the corresponding Tag.
	node->tag = new Tag();
	node->tag->dtd = doc->defaultDTD();
	node->tag->setWrite(doc);
	node->tag->type = nodeType;
	node->tag->name = QuantaCommon::tagCase(nodeName);
	node->tag->single = QuantaCommon::isSingleTag(doc->defaultDTD()->name, nodeName);
	node->tag->setStr(tagString);
	node->tag->cleanStrBuilt = false;
	return node;
}

void kafkaCommon::insertNode(Node *node, Node* parentNode, Node* nextSibling, NodeModifsSet &modifs)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "kafkaCommon::insertNode()" << endl;
#ifdef HEAVY_DEBUG
	coutTree(baseNode, 2);
#endif
#endif
	NodeModif modif;
	Node *n;
	bool nodeIsFirstChild = false;

	if(!node)
		return;

	//place the new Node.
	if(parentNode)
		n = parentNode->child;
	else
		n = baseNode;
	while(n && n->next)
		n = n->next;

	if(!parentNode && (!baseNode || (nextSibling && !nextSibling->prev)))
	{
		nodeIsFirstChild = true;
		baseNode = node;
		parser->setRootNode(baseNode);
	}
	if(parentNode && (!parentNode->child || nextSibling == parentNode->child))
	{
		nodeIsFirstChild = true;
		parentNode->child = node;
	}
	node->parent = parentNode;

	if(nextSibling && nextSibling->prev)
	{
		nextSibling->prev->next = node;
		node->prev = nextSibling->prev;
	}
	else if(n && !nodeIsFirstChild)
	{
		n->next = node;
		node->prev = n;
	}

	if(nextSibling)
		nextSibling->prev = node;
	node->next = nextSibling;

	//log this.
	modif.type = undoRedo::NodeAdded;
	modif.node = 0L;
	modif.childsNumber = 0;
	modif.childsNumber2 = 0;
	modif.location = kafkaCommon::getLocation(node);
	modifs.NodeModifList.append(modif);
}

Node *kafkaCommon::insertNode(Node *newNode, Node *parent, Node *nextSibling, Node *nextEndSibling,
	NodeModifsSet &modifs)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "kafkaCommon::insertNode()1" << endl;
#endif
	Node *nodeEnd, *n, *nNext;
	NodeModif modif;

	if(!newNode)
		return 0L;

	//place the new Node.
	insertNode(newNode, parent, nextSibling, modifs);

	if(!newNode->tag->single && newNode->tag->type == Tag::XmlTag)
	{
		//create the new closing Node.
		nodeEnd = createNode("/" + newNode->tag->name, "", Tag::XmlTagEnd, newNode->tag->write());
		nodeEnd->closesPrevious = true;

		//place the new closing Node.
		insertNode(nodeEnd, parent, nextEndSibling, modifs);
	}

	//If nextSibling != nextEndSibling, move all Nodes between node and nodeEnd as child of node
	if(nextSibling != nextEndSibling)
	{
		n = newNode->next;
		while(n && n != nodeEnd)
		{
			nNext = n->next;
			moveNode(n, newNode, 0L, modifs);
			n = nNext;
		}
	}

	return newNode;
}

bool kafkaCommon::insertNode(Node *newNode, Node *parent, Node *startNodeToSurround,
	Node *endNodeToSurround, int startOffset, int endOffset, NodeModifsSet &modifs)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "kafkaCommon::insertNode()2" << endl;
#endif

	if(!newNode || !startNodeToSurround || !endNodeToSurround)
		return 0L;

	//first split the Nodes.
	if(splitNode(startNodeToSurround, startOffset, modifs))
	{
		if(endNodeToSurround == startNodeToSurround)
		{
			endNodeToSurround = endNodeToSurround->next;
			endOffset -= startOffset;
		}
		startNodeToSurround = startNodeToSurround->next;
	}
	splitNode(endNodeToSurround, endOffset, modifs);

	//Then create and insert the new Node.
	return insertNode(newNode, parent, startNodeToSurround,
		endNodeToSurround->next, modifs);
}

bool kafkaCommon::DTDinsertNode(Node *newNode, Node *parent, Node *startNodeToSurround,
	Node *endNodeToSurround, int startOffset, int endOffset, NodeModifsSet &modifs)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "kafkaCommon::DTDinsertNode()1" << endl;
#endif
	Node *curNode, *beginSelection = 0L, *endSelection = 0L;
	QTag *parentQTag, *newNodeQTag;
	bool parentRelationOK = false, selectionStarted = false;

	if(!newNode || !startNodeToSurround || !endNodeToSurround)
		return false;

	//check if the relationship parent->nodeName is DTD valid.
	if(parent)
	{
		parentQTag = QuantaCommon::tagFromDTD(parent->tag->dtd, parent->tag->name);
		if(parentQTag)
			parentRelationOK = parentQTag->isChild(newNode->tag->name);
		else
			parentRelationOK = false;
	}
	else
		parentRelationOK = true;
	if(!parentRelationOK)
		return false;

	//Get the QTag of the new Node.
	newNodeQTag = QuantaCommon::tagFromDTD(newNode->tag->dtd, newNode->tag->name);
	if(!newNodeQTag)
		return false;

	//Add the new Node only when the child relationship is DTD valid.
	curNode = startNodeToSurround;
	while(curNode)
	{
		if(newNodeQTag->isChild(curNode->tag->name) || curNode->tag->type == Tag::Text)
		{
			if(curNode == startNodeToSurround)
			{
				if(splitNode(startNodeToSurround, startOffset, modifs))
				{
					if(endNodeToSurround == startNodeToSurround)
					{
						endNodeToSurround = endNodeToSurround->next;
						endOffset -= startOffset;
					}
					startNodeToSurround = startNodeToSurround->next;
					curNode = curNode->next;
				}
			}
			if(!selectionStarted)
			{
				selectionStarted = true;
				beginSelection = curNode;
			}
			if(curNode == endNodeToSurround)
			{
				splitNode(endNodeToSurround, endOffset, modifs);
				endSelection = curNode;
				insertNode(newNode, parent, beginSelection, endSelection->next, modifs);
			}
		}
		else
		{
			if(selectionStarted)
			{
				selectionStarted = false;
				endSelection = curNode->prev;
				insertNode(newNode, parent, beginSelection, endSelection->next, modifs);
			}
		}
		curNode = curNode->next;
	}
	return true;
}

bool kafkaCommon::DTDinsertNode(Node *newNode, Node *startNode, int startOffset, Node *endNode,
	int endOffset, NodeModifsSet &modifs)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "kafkaCommon::DTDinsertNode()2" << endl;
#endif
	QValueList<int> startNodeLocation, endNodeLocation;
	QValueList<int>::iterator itStart, itEnd;
	Node *commonParent, *commonParentStartChild, *commonParentEndChild, *parentNode, *node;
	Node *lastValidStartParent = 0L, *lastValidEndParent = 0L, *newParentNode, *child, *next;
	QTag *parentNodeQTag;
	NodeModif modif;
	int locOffset = 1;

	if(!startNode || !endNode || !newNode)
		return false;
	if(startNode == endNode)
	{
		if(insertNode(newNode, startNode->parent, startNode, startNode, startOffset, endOffset, modifs))
			return true;
		else
			return false;
	}

	//First search for the common parent of startNode and endNode.
	startNodeLocation = getLocation(startNode);
	endNodeLocation = getLocation(endNode);
	itStart = startNodeLocation.begin();
	itEnd = endNodeLocation.begin();
	while(itStart != startNodeLocation.end() && itEnd != endNodeLocation.end() &&
		(*itStart) == (*itEnd))
	{
		commonParent = getNodeFromSubLocation(startNodeLocation, locOffset);
		itStart++;
		itEnd++;
		locOffset++;
	}
	if(itStart != startNodeLocation.end())
		commonParentStartChild = getNodeFromSubLocation(startNodeLocation, locOffset);
	else
		commonParentStartChild = commonParent;
	if(itEnd != endNodeLocation.end())
		commonParentEndChild = getNodeFromSubLocation(endNodeLocation, locOffset);
	else
		commonParentEndChild = commonParent;

	//Now look if at least one of the parent Nodes between startNode and commonParent
	//can have nodeName as child. If so for startNode and endNode, let's find the last
	//parent Nodes which can have nodeName as child.
	parentNode = startNode->parent;
	while(parentNode && parentNode != commonParent)
	{
		parentNodeQTag = QuantaCommon::tagFromDTD(parentNode->tag->dtd, parentNode->tag->name);
		if(parentNodeQTag && parentNodeQTag->isChild(newNode->tag->name))
			lastValidStartParent = parentNode;
		parentNode = parentNode->parent;
	}
	parentNode = endNode->parent;
	while(parentNode && parentNode != commonParent)
	{
		parentNodeQTag = QuantaCommon::tagFromDTD(parentNode->tag->dtd, parentNode->tag->name);
		if(parentNodeQTag && parentNodeQTag->isChild(newNode->tag->name))
			lastValidEndParent = parentNode;
		parentNode = parentNode->parent;
	}

	if(!lastValidEndParent || !lastValidStartParent)
		return false;

	//OK now, we are sure the node can be inserted. Start the work by splitting
	//startNode and endNode if necessary
	if(startNode->tag->type == Tag::Text && startOffset != 0)
	{
		if(splitNode(startNode, startOffset, modifs))
			startNode = startNode->next;
	}
	if(endNode->tag->type == Tag::Text && endOffset != 0)
	{
		splitNode(endNode, endOffset, modifs);
	}

	//Then we "split" the lastValidStartParent - startNode subtree into two : the first part is untouched
	// and the second will be surrounded by the new Node. Same thing for endNode.
	node = startNode;
	parentNode = startNode->parent;
	while(parentNode && parentNode != lastValidStartParent)
	{
		if(node != parentNode->firstChild())
		{
			//node is not the first Child of parentNode, we have to duplicate parentNode, and put node and
			//all its next sibling as child of the new parentNode.
			/**newParentNode = insertNode(parentNode->tag->name, parentNode->tag->tagStr(),
				parentNode->tag->type, parentNode->tag->write(), parentNode->parentNode(),
				parentNode, parentNode, modifs);*/
			newParentNode = duplicateNode(parentNode);
			insertNode(newParentNode, parentNode->parentNode(), parentNode, parentNode, modifs);
			child = parentNode->firstChild();
			while(child && child != startNode)
			{
				next = child->next;
				moveNode(child, newParentNode, 0L, modifs);
				child = next;
			}
		}
		node = parentNode;
		parentNode = parentNode->parent;
	}
	node = endNode;
	parentNode = endNode->parent;
	while(parentNode && parentNode != lastValidEndParent)
	{
		if(node != parentNode->lastChild())
		{
			//node is not the last Child of parentNode, we have to duplicate parentNode, and put all
			//the next sibling of node as child of the new parentNode
			/**newParentNode = insertNode(parentNode->tag->name, parentNode->tag->tagStr(),
				parentNode->tag->type, parentNode->tag->write(), parentNode->parentNode(),
				parentNode, parentNode, modifs);*/
			newParentNode = duplicateNode(parentNode);
			insertNode(newParentNode, parentNode->parentNode(), parentNode, parentNode, modifs);
			child = parentNode->firstChild();
			while(child)
			{
				next = child->next;
				moveNode(child, newParentNode, 0L, modifs);
				if(child == endNode)
					break;
				child = next;
			}
		}
		node = parentNode;
		parentNode = parentNode->parent;
	}


	//Then we apply the recursive function to add the new Node when necessary/possible.
	bool addingStarted = false;
	int level = 0;
	addNodeRecursively(newNode, startNode,
		endNode, commonParentStartChild, addingStarted, level, modifs);
	return true;
}

Node *kafkaCommon::createAndInsertNode(QString nodeName, QString tagString, int nodeType, Document *doc,
	Node *parent, Node *nextSibling, Node *nextEndSibling, NodeModifsSet &modifs)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "kafkaCommon::createAndInsertNode() - nodeName :" << nodeName <<
		" - tagStr :" << tagString << " - nodeType :" << nodeType << endl;
#endif
	Node *node;

	//create the new Node.
	node = createNode(nodeName, tagString, nodeType, doc);

	//insert the new Node.
	insertNode(node, parent, nextSibling, nextEndSibling, modifs);

	return node;
}

Node *kafkaCommon::createAndInsertNode(QString nodeName, QString tagString, int nodeType, Document *doc,
	Node *parent, Node *startNodeToSurround, Node *endNodeToSurround, int startOffset, int endOffset,
	NodeModifsSet &modifs)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "kafkaCommon::createAndInsertNode()2 - nodeName :" << nodeName <<
		" - tagStr :" << tagString << " - nodeType :" << nodeType << endl;
#endif
	Node *node;

	if(!startNodeToSurround || !endNodeToSurround)
		return 0L;

	//create the new Node.
	node = createNode(nodeName, tagString, nodeType, doc);

	//insert the new Node.
	insertNode(node, parent, startNodeToSurround, endNodeToSurround, startOffset, endOffset,
		modifs);

	return node;

}

bool kafkaCommon::DTDcreateAndInsertNode(QString nodeName, QString tagString, int nodeType, Document *doc,
	Node *parent, Node *startNodeToSurround, Node *endNodeToSurround, int startOffset, int endOffset,
	NodeModifsSet &modifs)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "kafkaCommon::DTDcreateAndInsertNode() - nodeName :" << nodeName <<
		" - tagStr :" << tagString << " - nodeType :" << nodeType << endl;
#endif
	Node *node;

	if(!startNodeToSurround || !endNodeToSurround)
		return false;

	//create the new Node.
	node = createNode(nodeName, tagString, nodeType, doc);

	//insert the new Node.
	return DTDinsertNode(node, parent, startNodeToSurround, endNodeToSurround, startOffset, endOffset,
		modifs);
}


bool kafkaCommon::DTDcreateAndInsertNode(QString nodeName, QString tagString, int nodeType, Document *doc,
	Node *startNode, int startOffset, Node *endNode, int endOffset, NodeModifsSet &modifs)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "kafkaCommon::DTDcreateAndInsertNode()2 - nodeName : " << nodeName <<
		" - tagStr" <<tagString << endl;
#endif
	Node *node;

	if(!startNode || !endNode)
		return false;

	//create the new Node.
	node = createNode(nodeName, tagString, nodeType, doc);

	//insert the new Node.
	return DTDinsertNode(node, startNode, startOffset, endNode, endOffset, modifs);

/**	QValueList<int> startNodeLocation, endNodeLocation;
	QValueList<int>::iterator itStart, itEnd;
	Node *commonParent, *commonParentStartChild, *commonParentEndChild, *parentNode, *node;
	Node *lastValidStartParent = 0L, *lastValidEndParent = 0L, *newParentNode, *child, *next;
	QTag *parentNodeQTag;
	NodeModif modif;
	int locOffset = 1;

	if(!startNode || !endNode)
		return false;
	if(startNode == endNode)
	{
		if(DTDcreateAndInsertNode(nodeName, tagString, nodeType, doc, startNode->parent, startNode,
			startNode, startOffset, endOffset, modifs))
			return true;
		else
			return false;
	}

	//First search for the common parent of startNode and endNode.
	startNodeLocation = getLocation(startNode);
	endNodeLocation = getLocation(endNode);
	itStart = startNodeLocation.begin();
	itEnd = endNodeLocation.begin();
	while(itStart != startNodeLocation.end() && itEnd != endNodeLocation.end() &&
		(*itStart) == (*itEnd))
	{
		commonParent = getNodeFromSubLocation(startNodeLocation, locOffset);
		itStart++;
		itEnd++;
		locOffset++;
	}
	if(itStart != startNodeLocation.end())
		commonParentStartChild = getNodeFromSubLocation(startNodeLocation, locOffset);
	else
		commonParentStartChild = commonParent;
	if(itEnd != endNodeLocation.end())
		commonParentEndChild = getNodeFromSubLocation(endNodeLocation, locOffset);
	else
		commonParentEndChild = commonParent;

	//Now look if at least one of the parent Nodes between startNode and commonParent
	//can have nodeName as child. If so for startNode and endNode, let's find the last
	//parent Nodes which can have nodeName as child.
	parentNode = startNode->parent;
	while(parentNode && parentNode != commonParent)
	{
		parentNodeQTag = QuantaCommon::tagFromDTD(doc->defaultDTD(), parentNode->tag->name);
		if(parentNodeQTag && parentNodeQTag->isChild(nodeName))
			lastValidStartParent = parentNode;
		parentNode = parentNode->parent;
	}
	parentNode = endNode->parent;
	while(parentNode && parentNode != commonParent)
	{
		parentNodeQTag = QuantaCommon::tagFromDTD(doc->defaultDTD(), parentNode->tag->name);
		if(parentNodeQTag && parentNodeQTag->isChild(nodeName))
			lastValidEndParent = parentNode;
		parentNode = parentNode->parent;
	}

	if(!lastValidEndParent || !lastValidStartParent)
		return false;

	//OK now, we are sure the node can be inserted. Start the work by splitting
	//startNode and endNode if necessary
	if(startNode->tag->type == Tag::Text && startOffset != 0)
	{
		if(splitNode(startNode, startOffset, modifs))
			startNode = startNode->next;
	}
	if(endNode->tag->type == Tag::Text && endOffset != 0)
	{
		splitNode(endNode, endOffset, modifs);
	}

	//Then we "split" the lastValidStartParent - startNode subtree into two : the first part is untouched
	// and the second will be surrounded by the new Node. Same thing for endNode.
	node = startNode;
	parentNode = startNode->parent;
	while(parentNode && parentNode != lastValidStartParent)
	{
		if(node != parentNode->firstChild())
		{
			//node is not the first Child of parentNode, we have to duplicate parentNode, and put node and
			//all its next sibling as child of the new parentNode.
			newParentNode = createAndInsertNode(parentNode->tag->name, parentNode->tag->tagStr(),
				parentNode->tag->type, parentNode->tag->write(), parentNode->parentNode(),
				parentNode, parentNode, modifs);
			child = parentNode->firstChild();
			while(child && child != startNode)
			{
				next = child->next;
				moveNode(child, newParentNode, 0L, modifs);
				child = next;
			}
		}
		node = parentNode;
		parentNode = parentNode->parent;
	}
	node = endNode;
	parentNode = endNode->parent;
	while(parentNode && parentNode != lastValidEndParent)
	{
		if(node != parentNode->lastChild())
		{
			//node is not the last Child of parentNode, we have to duplicate parentNode, and put all
			//the next sibling of node as child of the new parentNode
			newParentNode = createAndInsertNode(parentNode->tag->name, parentNode->tag->tagStr(),
				parentNode->tag->type, parentNode->tag->write(), parentNode->parentNode(),
				parentNode, parentNode, modifs);
			child = parentNode->firstChild();
			while(child)
			{
				next = child->next;
				moveNode(child, newParentNode, 0L, modifs);
				if(child == endNode)
					break;
				child = next;
			}
		}
		node = parentNode;
		parentNode = parentNode->parent;
	}


	//Then we apply the recursive function to add the new Node when necessary/possible.
	bool addingStarted = false;
	int level = 0;
	addNodeRecursively(nodeName, tagString, nodeType, doc, startNode,
		endNode, commonParentStartChild, addingStarted, level, modifs);
	return true;*/
}

bool kafkaCommon::addNodeRecursively(Node *newNode, Node* startNode, Node *endNode,
	Node *currentNode, bool &addingStarted, int level, NodeModifsSet &modifs)
{
#ifdef HEAVY_DEBUG
	kdDebug(25001)<< "kafkaCommon::addNodeRevursively() [" << level << "] - currentNode :" <<
		currentNode->tag->name << endl;
#endif
	QTag *newNodeQTag, *currentNodeParentQTag;
	Node *startSelection = 0L, *endSelection = 0L, *oldCurrentNode, *copyNewNode;
	bool selectionInProgress = false, validCurNodeParent = false;

	newNodeQTag = QuantaCommon::tagFromDTD(newNode->tag->dtd, newNode->tag->name);
	if(!newNodeQTag)
		return false;

	if(currentNode && currentNode->parent)
	{
		currentNodeParentQTag = QuantaCommon::tagFromDTD(currentNode->parent->tag->dtd,
			currentNode->parent->tag->name);
		if(currentNodeParentQTag->isChild(newNode->tag->name))
			validCurNodeParent = true;
	}

	while(currentNode)
	{
		if(currentNode->tag->type == Tag::XmlTagEnd || currentNode->tag->type == Tag::Empty)
		{
			//do nothing
		}
		else if((newNodeQTag->isChild(currentNode->tag->name) || currentNode->tag->type == Tag::Text) &&
			validCurNodeParent)
		{
#ifdef HEAVY_DEBUG
				kdDebug(25001)<< "kafkaCommon::addNodeRevursively() [" << level <<
					"] -  Valid Child : " << currentNode->tag->name << endl;
#endif
			if(currentNode->hasForChild(startNode))
			{
#ifdef HEAVY_DEBUG
				kdDebug(25001)<< "kafkaCommon::addNodeRevursively() [" << level <<
					"] -  This Node has the startNode as Child : " << currentNode->tag->name << endl;
#endif
				addingStarted = true;
			}
			if(!selectionInProgress && addingStarted)
			{
#ifdef HEAVY_DEBUG
				kdDebug(25001)<< "kafkaCommon::addNodeRevursively() [" << level <<
					"] - selection started at Node " << currentNode->tag->name << endl;
#endif
				selectionInProgress = true;
				startSelection = currentNode;
			}
			if(currentNode->hasForChild(endNode))
			{
#ifdef HEAVY_DEBUG
				kdDebug(25001)<< "kafkaCommon::addNodeRevursively() [" << level <<
					"] -  This Node has the endNode as Child : " << currentNode->tag->name << endl;
#endif
				addingStarted = false;
			}
			if(!addingStarted && selectionInProgress)
			{
#ifdef HEAVY_DEBUG
				kdDebug(25001)<< "kafkaCommon::addNodeRevursively() [" << level <<
					"] - selection ended at Node " << currentNode->tag->name << endl;
#endif
				selectionInProgress = false;
				if(currentNode->tag->single)
					endSelection = currentNode;
				else
					endSelection = currentNode->getClosingNode();
				/**createAndInsertNode(nodeName, tagString, nodeType, doc, startSelection->parent,
					startSelection, endSelection->next, modifs);*/
				copyNewNode = duplicateNode(newNode);
				insertNode(copyNewNode, startSelection->parentNode(), startSelection,
					endSelection->next, modifs);
			}
		}
		else//(!newNodeQTag->isChild(currentNode->tag->name))
		{
#ifdef HEAVY_DEBUG
				kdDebug(25001)<< "kafkaCommon::addNodeRevursively() [" << level <<
					"] -  Invalid Child : " << currentNode->tag->name << endl;
#endif
			endSelection = currentNode->prev;
			if(selectionInProgress)
			{
#ifdef HEAVY_DEBUG
				kdDebug(25001)<< "kafkaCommon::addNodeRevursively() [" << level <<
					"] - selection ended(2) at Node " << currentNode->tag->name << endl;
#endif
				selectionInProgress = false;
				if(addingStarted)
				{
					/**createAndInsertNode(nodeName, tagString, nodeType, doc, startSelection->parent,
						startSelection, endSelection->next, modifs);*/
					copyNewNode = duplicateNode(newNode);
					insertNode(copyNewNode, startSelection->parentNode(), startSelection,
						endSelection->next, modifs);
				}
			}
			if(currentNode->child)
			{
				addNodeRecursively(newNode, startNode, endNode, currentNode->child,
					addingStarted, level + 1, modifs);
			}
		}
		oldCurrentNode = currentNode;
		currentNode = currentNode->next;
	}

	if(selectionInProgress)
	{
#ifdef HEAVY_DEBUG
				kdDebug(25001)<< "kafkaCommon::addNodeRevursively() [" << level <<
					"] - selection ended(3) at Node " << oldCurrentNode->tag->name << endl;
#endif
				selectionInProgress = false;
				endSelection = oldCurrentNode;
				if(addingStarted)
				{
					/**createAndInsertNode(nodeName, tagString, nodeType, doc, startSelection->parent,
						startSelection, endSelection->next, modifs);*/
					copyNewNode = duplicateNode(newNode);
					insertNode(copyNewNode, startSelection->parentNode(), startSelection,
						endSelection->next, modifs);
				}
	}

	//The newNode was a template, let's delete it now.
	if(level == 0)
		delete newNode;
	return true;
}

Node *kafkaCommon::duplicateNode(Node *node)
{
	Node *newNode;

	newNode = new Node(*node);
	newNode->prev = 0L;
	newNode->next = 0L;
	newNode->parent = 0L;
	newNode->child = 0L;
	newNode->listItem = 0L;
	//TODO: see for the groups, reloading Nodes with undo should made the group pointer point
	//where it should point.
	newNode->groupElementLists.clear();
	newNode->group = 0L;
	newNode->groupTag = 0L;
	newNode->tag = new Tag(*(node->tag));
	newNode->tag->cleanStrBuilt = false;

	return newNode;
}

Node* kafkaCommon::extractNode(Node *node, NodeModifsSet &modifs, bool deleteChildren,
	bool /**mergeAndFormat*/)
{
	NodeModif modif;
	Node *lastChild, *curNode;

	if(!node)
		return 0L;

	if(!node->child)
		deleteChildren = true;

	//logging
	if(deleteChildren)
		modif.type = undoRedo::NodeAndChildsRemoved;
	else
		modif.type = undoRedo::NodeRemoved;
	modif.location = kafkaCommon::getLocation(node);
	modif.node = node;
	modif.childsNumber = 0;
	modif.childsNumber2 = 0;

	//starting to extract.
	if(node == baseNode)
	{
		if(deleteChildren)
			baseNode = 0L;
		else
			baseNode = node->child;
		parser->setRootNode(baseNode);
	}
	if(!deleteChildren)
	{
		curNode = node->child;
		while(curNode)
		{
			curNode->parent = node->parent;
			curNode = curNode->next;
		}
	}
	if(node->parent && node->parent->child == node)
	{
		if(deleteChildren)
			node->parent->child = node->next;
		else
			node->parent->child = node->child;
	}
	node->parent = 0L;
	if(node->prev)
	{
		if(deleteChildren)
			node->prev->next = node->next;
		else
		{
			node->prev->next = node->child;
			node->child->prev = node->prev;
		}
	}
	if(node->next)
	{
		if(deleteChildren)
			node->next->prev = node->prev;
		else
		{
			lastChild = node->child;
			while(lastChild->next)
				lastChild = lastChild->next;
			node->next->prev = lastChild;
			lastChild->next = node->next;
		}
	}
	node->prev = 0L;
	node->next = 0L;
	if(!deleteChildren)
		node->child = 0L;

	modifs.NodeModifList.append(modif);
	return node;
}

void kafkaCommon::extractAndDeleteNode(Node *node, NodeModifsSet &modifs, bool deleteChildren,
	bool deleteClosingTag, bool mergeAndFormat)
{
	NodeModif modif;
	Node  *curNode, *nodePrev, *nodeNext, *nodeNext2, *n, *n2;
	QString nodeName, closingNodeName;
	bool isSingle;

	if(!node)
		return;

	isSingle = node->tag->single;
	nodeName = node->tag->name;
	nodePrev = node->prev;
	nodeNext = node->next;
	if(!node->child)
		deleteChildren = true;
	extractNode(node, modifs, deleteChildren, mergeAndFormat);

	//delete the closing Tag
	if(!isSingle && deleteClosingTag && nodeNext)
	{
		curNode = nodeNext;
		while(curNode && curNode->tag->type == Tag::Empty)
			curNode = curNode->next;
		if(curNode && QuantaCommon::closesTag(node->tag,curNode->tag))
		{
			curNode = nodeNext;
			while(curNode)
			{
				nodeNext2 = curNode->next;
				closingNodeName = curNode->tag->name;
				extractNode(curNode, modifs, deleteChildren, mergeAndFormat);
				curNode = nodeNext2;
				if(closingNodeName == ("/" + nodeName))
					break;
			}
			nodeNext = curNode;
		}
	}

	//merge the next and prev Nodes if they are both of type Text or Empty
	if(mergeAndFormat && nodePrev)
	{
		n = nodePrev;
		n2 = nodePrev->next;
		while(n && n2 && n != nodeNext)
		{
			if(!n->child)
				kafkaCommon::mergeNodes(n, n2, modifs);
			n = n2;
			n2 = n2->next;
		}
	}

//	modifs.NodeModifList.append(modif);
}

void kafkaCommon::moveNode(Node *nodeToMove, Node *newParent, Node *newNextSibling,
	NodeModifsSet &modifs)
{
	NodeModif modif;
	Node *newNode;

	//extract the old Node.
	newNode = extractNode(nodeToMove, modifs, true, true);

	//insert the new Node.
	insertNode(newNode, newParent, newNextSibling, modifs);

}

bool kafkaCommon::splitNode(Node *n, int offset, NodeModifsSet &modifs)
{
	NodeModif modif;
	Tag *tag;
	QString tagStr;

	if(!n || n->tag->type != Tag::Text || offset <= 0 || offset >= (signed)n->tag->tagStr().length())
		return false;

	tag = new Tag(*(n->tag));
	tagStr = n->tag->tagStr();
	n->tag->setStr(tagStr.left(offset));
	modif.type = undoRedo::NodeModified;
	modif.tag = tag;
	modif.location = getLocation(n);
	modifs.NodeModifList.append(modif);

	createAndInsertNode("#text", tagStr.right(tagStr.length() - offset), Tag::Text, n->tag->write(),
		n->parent, n->next, n->next, modifs);
	return true;
}

 bool kafkaCommon::mergeNodes(Node *n, Node *n2, NodeModifsSet &modifs)
 {
 	NodeModif modif;
	Tag *tag;
	if(!n || !n2)
		return false;

	if((n->tag->type == Tag::Empty || n->tag->type == Tag::Text) && (n2->tag->type == Tag::Empty ||
		n2->tag->type == Tag::Text))
	{
		tag = new Tag(*(n->tag));
		modif.type = undoRedo::NodeModified;
		modif.tag = tag;
		modif.location = getLocation(n);
		modifs.NodeModifList.append(modif);
		n->tag->setStr(n->tag->tagStr() + n2->tag->tagStr());
		kafkaCommon::extractAndDeleteNode(n2, modifs, false, false, false);

		return true;
	}
	return false;
 }

int kafkaCommon::getNodeType(QString nodeName)
{
	nodeName = nodeName.lower();
	if(nodeName == "html" || nodeName == "head" || nodeName == "meta" || nodeName == "title" ||
		nodeName == "link" || nodeName == "style" || nodeName == "script" || nodeName == "option" ||
		nodeName == "optgroup" || nodeName == "area" || nodeName == "param" || nodeName == "noframes" ||
		nodeName == "ins" || nodeName == "img" || nodeName == "applet")
		return kafkaCommon::noneDisplay;
	else if(nodeName == "body" || nodeName == "p" || nodeName == "div" || nodeName == "address" ||
		nodeName == "blockquote" || nodeName == "iframe" || nodeName == "object" ||
		nodeName == "center" || nodeName == "hr" || nodeName == "map" || nodeName == "h1" ||
		nodeName == "h2" || nodeName == "h3" || nodeName == "h4" || nodeName == "h5" ||
		nodeName == "h6" || nodeName == "table" || nodeName == "thead" || nodeName == "tbody" ||
		nodeName == "tfoot" || nodeName == "col" || nodeName == "colgroup" ||
		nodeName == "tr" || nodeName == "td" || nodeName == "th" || nodeName == "caption" ||
		nodeName == "ul" || nodeName == "menu" || nodeName == "dir" || nodeName == "ol" ||
		nodeName == "li" || nodeName == "ul" || nodeName == "dd" || nodeName == "dl" ||
		nodeName == "dt" || nodeName == "form" || nodeName == "legend" || nodeName == "fieldset" ||
		nodeName == "button" || nodeName == "pre" || nodeName == "input" || nodeName == "select" ||
		nodeName == "frameset" || nodeName == "frame" || nodeName == "strong" || nodeName == "em" ||
		nodeName == "address" || nodeName == "del")
		return kafkaCommon::blockDisplay;
	else if(nodeName == "q" || nodeName == "u" || nodeName == "i" || nodeName == "b" ||
		nodeName == "cite" || nodeName == "em" || nodeName == "var" ||
		nodeName == "tt" || nodeName == "code" || nodeName == "kbd" || nodeName == "samp" ||
		 nodeName == "big" || nodeName == "small" || nodeName == "s" || nodeName == "strike" ||
		  nodeName == "sub" || nodeName == "sup" || nodeName == "abbr" ||
		 nodeName == "acronym" || nodeName == "a" || nodeName == "bdo" ||
		 nodeName == "font")
		 return kafkaCommon::inlineDisplay;
	else
	{
		kdDebug(25001)<< "kafkaCommon::getNodeType() - ERROR node Name not found" << endl;
		return kafkaCommon::noneDisplay;
	}
}
