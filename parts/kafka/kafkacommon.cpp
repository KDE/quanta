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
#include <dom/dom_exception.h>
#include <dom/dom_doc.h>
#include <dom/dom_element.h>
#include <dom/dom_text.h>

#include "../../parser/node.h"
#include "../../parser/tag.h"
#include "../../document.h"
#include "../../resource.h"
#include "../../quantacommon.h"

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

Node* kafkaCommon::getNextNodeNE(Node *node, bool &goUp, Node *endNode)
{
	Node *n = node;
	n = getNextNode(n, goUp, endNode);
	while(n && n->tag->type == Tag::Empty)
		n = getNextNode(n, goUp, endNode);
	return n;
}

Node* kafkaCommon::getPrevNode(Node *node, Node *endNode)
{
	Node *n = node;

	if(!node)
		return 0L;

	if(n->prev && n->prev->child)
	{
		n = n->prev;
		if(n == endNode)
			return 0L;
		while(n->child)
		{
			n = n->child;
			while(n && n->next)
				n = n->next;
			if(n == endNode)
				return 0L;
		}
	}
	else if(n->prev)
	{
		n = n->prev;
		if(n == endNode)
			return 0L;
	}
	else
	{
		n = n->parent;
		if(n == endNode)
			return 0L;
	}
	return n;
}

Node* kafkaCommon::getPrevNodeNE(Node *node, Node *endNode)
{
	Node *n = node;
	n = getPrevNode(node, endNode);
	while(n && n->tag->type == Tag::Empty)
		n = getPrevNode(n, endNode);
	return n;
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

void kafkaCommon::applyIndentation(Node *node, int nbOfSpaces, int nbOfTabs)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "kafkaCommon::applyIndentation()" << endl;
#endif
	Node *parent, *nextNE, *realPrevNE, *realNextNE, *realPrev, *realNext;
	int nonInlineDepth = 0, nonInlineDepth2 = 0, i;
	bool b = false;
	QString indentation1, indentation2, smallIndentation2;

	if(!node)
		return;

	nextNE = getNextNodeNE(node, b);
	realPrevNE = node->prevNE();
	realNextNE = node->nextNE();
	realPrev = node->prev;
	realNext = node->next;

	//compute the "non-inline depth" of the Node and of the next NE (not Empty) Node
	// i.e. we count how many non-inline parent they have.
	parent = node->parent;
	while(parent)
	{
		if(getNodeDisplay(parent->tag->name) != kafkaCommon::inlineDisplay)
			nonInlineDepth++;
		parent = parent->parent;
	}

	if (nextNE)
		parent = nextNE->parent;
	else
		parent = 0L;
	while(parent)
	{
		parent = nextNE->parent;
		while(parent)
		{
			if(getNodeDisplay(parent->tag->name) != kafkaCommon::inlineDisplay)
				nonInlineDepth2++;
			parent = parent->parent;
		}
	}

	parent = node->parent;

	if(!parent || getNodeDisplay(parent->tag->name, true) != kafkaCommon::inlineDisplay)
	{
		//prepare the indentation
		indentation1 = "\n";
		for(i = 1; i <= nbOfSpaces * nonInlineDepth; i++)
			indentation1 += " ";
		for(i = 1; i <= nbOfTabs * nonInlineDepth; i++)
			indentation1 += "\t";

		indentation2 = "\n";
		for(i = 1; i <= nbOfSpaces * nonInlineDepth2; i++)
			indentation2 += " ";
		for(i = 1; i <= nbOfTabs * nonInlineDepth2; i++)
			indentation2 += "\t";

		smallIndentation2 = "\n";
		for(i = 1; i <= nbOfSpaces * (nonInlineDepth2 - 1); i++)
			smallIndentation2 += " ";
		for(i = 1; i <= nbOfTabs * (nonInlineDepth2 - 1); i++)
			smallIndentation2 += "\t";
		//kdDebug(25001)<< "CURNODE:" << node->tag->name << endl;

		//test and add indentations if necessary
		if(!realPrevNE || (realPrevNE && getNodeDisplay(node->tag->name, true) != kafkaCommon::inlineDisplay) ||
			(realPrevNE && getNodeDisplay(node->tag->name, true) == kafkaCommon::inlineDisplay &&
			getNodeDisplay(realPrevNE->tag->name, true) != kafkaCommon::inlineDisplay))
		{
			//kdDebug(25001)<< "BOO1" << endl;
			if(node->tag->type == Tag::Text)
			{
				//kdDebug(25001)<< "BOO2" << endl;
				//node->tag->setStr(indentation1 + node->tag->tagStr());
				setTagStringAndFitsNodes(node, indentation1 + node->tag->tagStr());
			}
			else if(realPrev && realPrev->tag->type == Tag::Empty)
			{
				//kdDebug(25001)<< "BOO3" << endl;
				//realPrev->tag->setStr(indentation1);
				setTagStringAndFitsNodes(realPrev, indentation1);
			}
		}

		if(!realNextNE || (realNextNE && getNodeDisplay(node->tag->name, true) != kafkaCommon::inlineDisplay) ||
			(realNextNE && getNodeDisplay(node->tag->name, true) == kafkaCommon::inlineDisplay &&
			getNodeDisplay(realNextNE->tag->name, true) != kafkaCommon::inlineDisplay))
		{
			//kdDebug(25001)<< "BOO4" << endl;
			if(node->tag->type == Tag::Text)
			{
				//kdDebug(25001)<< "BOO5" << endl;
				//node->tag->setStr(node->tag->tagStr() + indentation2);
				setTagStringAndFitsNodes(node, node->tag->tagStr() + indentation2);
			}
			else if(realNext && realNext->tag->type == Tag::Empty)
			{
				//kdDebug(25001)<< "BOO6" << endl;
				if(realNext->nextNE())
					//realNext->tag->setStr(indentation2);
					setTagStringAndFitsNodes(realNext, indentation2);
				else
					//realNext->tag->setStr(smallIndentation2);
					setTagStringAndFitsNodes(realNext, smallIndentation2);
			}
		}
	}
	else
	{
		//The parent is inline, so no indentation.
		//Nothing to do.
	}
}

void kafkaCommon::fitIndentationNodes(Node *n1, Node *n2)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "kafkaCommon::fitIndentationNodes()" << endl;
#endif
	Node *parent, *child, *node, *emptyNode = 0L, *emptyNode2 = 0L;
	int nbEmptyNodes = 0;
	bool lastChild = false, firstChild = false;
	//We don't want to log the modifications made to Empty Nodes /
	//addition of whitespaces in Text Nodes, so let's create a dummy one.
	NodeModifsSet *modifs = new NodeModifsSet();

	if(!n1 || !n2 || n1 == n2 || n1->tag->type == Tag::Empty || n2->tag->type == Tag::Empty)
		return;

	if(n1->parent == n2 || n2->parent == n1)
	{
		//kdDebug(25001)<< "BOO1" << endl;
		if(n1->parent == n2)
		{
			//kdDebug(25001)<< "BOO2" << endl;
			child = n1;
			parent = n2;
			if(n2->firstChild() == n1)
				firstChild = true;
			if(n2->lastChild() == n1)
				lastChild = true;
		}
		else
		{
			//kdDebug(25001)<< "BOO2-1" << endl;
			child = n2;
			parent = n1;
			if(n1->firstChild() == n2)
				firstChild = true;
			if(n1->lastChild() == n2)
				lastChild = true;
		}

		//counting the Empty Nodes and deleting them to have only one empty node.
		if(firstChild)
		{
			//kdDebug(25001)<< "BOO3" << endl;
			node = child->prev;
			while(node)
			{
				if(node->tag->type == Tag::Empty)
					nbEmptyNodes++;
				node = node->prev;
			}
			node = child->prev;
			while(nbEmptyNodes > 1)
			{
				extractAndDeleteNode(node, modifs, false, false);
				nbEmptyNodes--;
				node = child->prev;
			}
			if(nbEmptyNodes == 1)
				emptyNode = child->prev;
		}

		nbEmptyNodes = 0;
		if(lastChild)
		{
			//kdDebug(25001)<< "BOO4" << endl;
			node = child->next;
			while(node)
			{
				if(node->tag->type == Tag::Empty)
					nbEmptyNodes++;
				node = node->next;
			}
			node = child->next;
			while(nbEmptyNodes > 1)
			{
				extractAndDeleteNode(node, modifs, false, false);
				nbEmptyNodes--;
				node = child->next;
			}
			if(nbEmptyNodes == 1)
				emptyNode2 = child->next;
		}

		//adding/deleting a empty node if necessary
		if(firstChild)
		{
			if(getNodeDisplay(parent->tag->name) != kafkaCommon::inlineDisplay)
			{
				if(child->tag->type != Tag::Text && !emptyNode)
				{
				//kdDebug(25001)<< "BOO5" << endl;
					createAndInsertNode("", "", Tag::Empty, n2->tag->write(), parent, child, child, modifs);
				}
			}
			else
			{
				if(child->tag->type == Tag::Text && emptyNode)
				{
					//kdDebug(25001)<< "BOO6" << endl;
					extractAndDeleteNode(emptyNode, modifs, false, false);
				}
			}
		}

		if(lastChild)
		{
			if(getNodeDisplay(parent->tag->name) != kafkaCommon::inlineDisplay)
			{
				if(child->tag->type != Tag::Text && !emptyNode2)
				{
					//kdDebug(25001)<< "BOO7" << endl;
					createAndInsertNode("", "", Tag::Empty, n2->tag->write(), parent, 0L, 0L, modifs);
				}
			}
			else
			{
				if(child->tag->type == Tag::Text && emptyNode2)
				{
					//kdDebug(25001)<< "BOO8" << endl;
					extractAndDeleteNode(emptyNode2, modifs, false, false);
				}
			}
		}
	}
	else
	{
		if(n1->next != n2)
		{
			//counting the Empty Nodes and deleting them to have only one empty node.
			node = n1->next;
			while(node && node != n2)
			{
				if(node->tag->type == Tag::Empty)
					nbEmptyNodes++;
				node = node->next;
			}
			node = n1->next;
			while(nbEmptyNodes > 1 || (nbEmptyNodes > 0 && n1->getClosingNode() == n2))
			{
				extractAndDeleteNode(node, modifs, false, false);
				nbEmptyNodes--;
				node = n1->next;
			}
			if(nbEmptyNodes == 1)
				emptyNode = n1->next;

			if(n1->getClosingNode() == n2 && n1->child && n1->child->tag->type == Tag::Empty)
				emptyNode = n1->child;
		}

		//adding/deleting a empty node if necessary
		parent = n1->parent;
		if(!parent || getNodeDisplay(parent->tag->name) != kafkaCommon::inlineDisplay)
		{
			if(getNodeDisplay(n1->tag->name) != kafkaCommon::inlineDisplay)
			{
				if(n2->tag->type == Tag::Text && emptyNode)
				{
				//kdDebug(25001)<< "BOO9" << endl;
					extractAndDeleteNode(emptyNode, modifs, false, false);
				}
				else if(n2->tag->type != Tag::Text && !emptyNode)
				{
					if(n1->getClosingNode() == n2)
					{
					//kdDebug(25001)<< "BOO10" << endl;
						createAndInsertNode("", "", Tag::Empty, n2->tag->write(), n1, 0L, 0L, modifs);
					}
					else
					{
					//kdDebug(25001)<< "BOO11" << endl;
						createAndInsertNode("", "", Tag::Empty, n2->tag->write(), parent, n2, n2, modifs);
					}
				}
			}
			else
			{
				if((n2->tag->type == Tag::Text ||
					getNodeDisplay(n2->tag->name) == kafkaCommon::inlineDisplay) && emptyNode)
				{
				//kdDebug(25001)<< "BOO12" << endl;
					extractAndDeleteNode(emptyNode, modifs, false, false);
				}
				else if(n2->tag->type != Tag::Text &&
					getNodeDisplay(n2->tag->name) != kafkaCommon::inlineDisplay && !emptyNode)
				{
					if(n1->getClosingNode() == n2)
					{
					//kdDebug(25001)<< "BOO13" << endl;
						createAndInsertNode("", "", Tag::Empty, n2->tag->write(), n1, 0L, 0L, modifs);
					}
					else
					{
					//kdDebug(25001)<< "BOO14" << endl;
						createAndInsertNode("", "", Tag::Empty, n2->tag->write(), parent, n2, n2, modifs);
					}
				}
			}
		}
		else
		{
			if(emptyNode)
				extractAndDeleteNode(emptyNode, modifs, false, false);
		}
	}

	delete modifs;
}

void kafkaCommon::fitsNodesPosition(Node* startNode, int colMovement, int lineMovement, int colEnd, int lineEnd)
{
	bool b = false;
	int j, SNbeginLine, SNbeginCol/**, SNlastLine, SNlastCol*/;
	int beginLine, beginCol, lastLine, lastCol;
	Node *node = startNode;

	if(!node)
		return;

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

int kafkaCommon::getNodeDisplay(const QString &nodeNam, bool closingNodeToo)
{
	QString nodeName = nodeNam.lower();
	if(closingNodeToo && nodeName.startsWith("/"))
		nodeName = nodeName.mid(1);
	if(nodeName == "html" || nodeName == "head" || nodeName == "meta" ||
		nodeName == "link" || nodeName == "style" || nodeName == "option" ||
		nodeName == "optgroup" || nodeName == "area" || nodeName == "param" ||
		nodeName == "thead" || nodeName == "tbody" || nodeName == "dt" ||
		nodeName == "tfoot" || nodeName == "col" || nodeName == "colgroup" ||
		nodeName == "tr" || nodeName == "td" || nodeName == "th" || nodeName == "caption" ||
		nodeName == "ins" || nodeName == "legend")
		return kafkaCommon::noneDisplay;
	else if(nodeName == "body" || nodeName == "p" || nodeName == "div" ||
		nodeName == "blockquote" || nodeName == "isindex" ||
		nodeName == "center" || nodeName == "hr" || nodeName == "h1" ||
		nodeName == "h2" || nodeName == "h3" || nodeName == "h4" || nodeName == "h5" ||
		nodeName == "h6" || nodeName == "table" ||
		nodeName == "ul" || nodeName == "menu" || nodeName == "dir" || nodeName == "ol" ||
		nodeName == "li" || nodeName == "ul" || nodeName == "dd" || nodeName == "dl" ||
		 nodeName == "form" || nodeName == "fieldset" ||
		nodeName == "pre" || nodeName == "noscript" || nodeName == "noframes" ||
		nodeName == "frameset" || nodeName == "frame" ||
		nodeName == "address" || nodeName == "del")
		return kafkaCommon::blockDisplay;
	else if(nodeName == "q" || nodeName == "u" || nodeName == "i" || nodeName == "b" ||
		nodeName == "cite" || nodeName == "em" || nodeName == "var" || nodeName == "em" ||
		nodeName == "tt" || nodeName == "code" || nodeName == "kbd" || nodeName == "samp" ||
		 nodeName == "big" || nodeName == "small" || nodeName == "s" || nodeName == "strike" ||
		  nodeName == "sub" || nodeName == "sup" || nodeName == "abbr" || nodeName == "title" ||
		 nodeName == "acronym" || nodeName == "a" || nodeName == "bdo" ||
		 nodeName == "font" || nodeName == "#text" || nodeName == "strong" || nodeName == "dfn" ||
		 nodeName == "img" ||  nodeName == "applet" ||  nodeName == "object" ||  nodeName == "basefont" ||
		 nodeName == "br" ||  nodeName == "script" ||  nodeName == "map" || nodeName == "span" ||
		 nodeName == "iframe" || nodeName == "input" || nodeName == "select" || nodeName == "textarea" ||
		 nodeName == "label" || nodeName == "button" )
		 return kafkaCommon::inlineDisplay;
	else
	{
		kdDebug(25001)<< "kafkaCommon::getNodeType() - ERROR " << nodeName <<" not found" << endl;
		return kafkaCommon::noneDisplay;
	}
}

Node* kafkaCommon::createNode(const QString &nodeName, const QString &tagString, int nodeType, Document *doc)
{
	Node *node;

	//create the Node.
	node = new Node(0L);

	//Create the corresponding Tag.
	node->tag = new Tag();
	if(doc)
		node->tag->dtd = doc->defaultDTD();
	else
		node->tag->dtd = 0L;
	node->tag->setWrite(doc);
	node->tag->type = nodeType;
	node->tag->name = QuantaCommon::tagCase(nodeName);
	if(doc)
		node->tag->single = QuantaCommon::isSingleTag(doc->defaultDTD()->name, nodeName);
	else
		node->tag->single = false;
	node->tag->setStr(tagString);
	node->tag->cleanStrBuilt = false;
	return node;
}

Node *kafkaCommon::createDoctypeNode(Document *doc)
{
	Node *node;
	QString tagStr;

	if(!doc)
		return 0L;

	//create the Node
	tagStr = "<!DOCTYPE" + doc->defaultDTD()->doctypeStr + ">";

	//As the Tag class was not made to handle value-less attrs, we won't load them.
	//We'll simply set the Tag string.
	node = createNode("!DOCTYPE", tagStr, Tag::XmlTag, doc);
	node->tag->cleanStrBuilt = true;

	return node;
}

Node* kafkaCommon::insertNode(Node *node, Node* parentNode, Node* nextSibling,
	NodeModifsSet *modifs, bool merge)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "kafkaCommon::insertNode()" << endl;
#endif
	NodeModif* modif;
	Node *n;
	bool nodeIsFirstChild = false;

	if(!node)
		return 0L;

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
	if(modifs)
	{
		modif = new NodeModif();
		modif->setType(NodeModif::NodeAdded);
		modif->setLocation(getLocation(node));
		modifs->addNodeModif(modif);
	}

	//Then try to merge with the siblings
	if(merge)
	{
		if(node->prev)
		{
			n = node->prev;
			if(mergeNodes(node->prev, node, modifs))
				node = n;
		}
		if(node->next)
		{
			mergeNodes(node, node->next, modifs);
		}
	}

#ifdef HEAVY_DEBUG
	coutTree(baseNode, 2);
#endif
	return node;
}

Node *kafkaCommon::insertNode(Node *newNode, Node *parent, Node *nextSibling, Node *nextEndSibling,
	NodeModifsSet *modifs)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "kafkaCommon::insertNode()1" << endl;
#endif
	Node *n, *nodeEnd;

	if(!newNode)
		return 0L;

	//place the new Node.
	newNode = insertNode(newNode, parent, nextSibling, modifs);

	if(!newNode->tag->single && newNode->tag->type == Tag::XmlTag)
	{
		//create the new closing Node.
		nodeEnd = createNode("/" + newNode->tag->name, "", Tag::XmlTagEnd, newNode->tag->write());
		nodeEnd->closesPrevious = true;

		//place the new closing Node.
		nodeEnd = insertNode(nodeEnd, parent, nextEndSibling, modifs);
	}

	//If nextSibling != nextEndSibling, move all Nodes between node and nodeEnd as child of node
	if(nextSibling != nextEndSibling)
	{
		n = newNode->next;
		while(newNode->next && newNode->next != nodeEnd)
			moveNode(newNode->next, newNode, 0L, modifs);
	}

	return newNode;
}

Node* kafkaCommon::insertNode(Node *newNode, Node *parent, Node *startNodeToSurround,
	Node *endNodeToSurround, int startOffset, int endOffset, NodeModifsSet *modifs)
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
	Node *endNodeToSurround, int startOffset, int endOffset, NodeModifsSet *modifs,
	bool generateElements)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "kafkaCommon::DTDinsertNode()1" << endl;
#endif
	Node *curNode, *beginSelection = 0L, *endSelection = 0L, *node, *nodeParent;
	QTag *parentQTag, *newNodeQTag, *qTag, *lastListQTag;
	bool parentRelationOK = false, selectionStarted = false, insertNodeAroundSelection = false;
	QPtrList<QTag> qTagList;
	QMap<QString, bool>::iterator it;

	if(!newNode || (!startNodeToSurround && endNodeToSurround) ||
		(startNodeToSurround && !endNodeToSurround))
		return false;

	//check if the relationship parent->nodeName is DTD valid.
	if(parent)
	{
		parentQTag = QuantaCommon::tagFromDTD(parent->tag->dtd, parent->tag->name);
		if(parentQTag)
			parentRelationOK = parentQTag->isChild(newNode);
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

	//Build the list of Mandatory childs e.g. TABLE - TD - TR
	qTagList.append(newNodeQTag);
	qTag = newNodeQTag;
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "current QNode: " << qTag->name()<< endl;
#endif
	while(generateElements && qTag)
	{
		lastListQTag = qTag;
		for(it = qTag->childTags.begin(); it != qTag->childTags.end(); it++)
		{
			if(it.data())
			{
				qTag = QuantaCommon::tagFromDTD(qTag->parentDTD, it.key());
				qTagList.append(qTag);
#ifdef HEAVY_DEBUG
				kdDebug(25001)<< "Mandatory node: " << qTag->name()<< endl;
#endif
				break;
			}
		}
		if(qTag == lastListQTag)
			qTag = 0L;
	}

	//if there is no node to surround, we simply directly add the node to insert.
	if(!startNodeToSurround && !endNodeToSurround)
		insertNodeAroundSelection = true;

	//Add the new Node only when the child relationship is DTD valid.
	curNode = startNodeToSurround;
	while(curNode || (startNodeToSurround == endNodeToSurround))
	{
		if(newNodeQTag->isChild(curNode) || (lastListQTag->isChild(curNode) && generateElements))
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
				endSelection = curNode->next;
				insertNodeAroundSelection = true;
			}
		}
		else
		{
			if(selectionStarted)
			{
				selectionStarted = false;
				endSelection = curNode;
				insertNodeAroundSelection = true;
			}
		}
		if(insertNodeAroundSelection)
		{
			/**if(newNodeQTag->isChild(curNode))
			{
				//insert newNode only
				if(startNodeToSurround == endNodeToSurround && endOffset == 0)
					insertNode(newNode, parent, beginSelection, beginSelection, modifs);
				else
					insertNode(newNode, parent, beginSelection, endSelection->next, modifs);
			}
			else if(generateElements)// && lastListQTag->isChild(curNode)
			{*/
				//insert newNode and some mandatory Nodes
				for(qTag = qTagList.first(); qTag; qTag = qTagList.next())
				{
					if((!startNodeToSurround && !endNodeToSurround))
					{
						beginSelection = 0L;
						endSelection = 0L;
					}
					if(qTag == qTagList.getFirst())
					{
						if(startNodeToSurround == endNodeToSurround && endOffset == 0)
							insertNode(newNode, parent, beginSelection, beginSelection, modifs);
						else
							insertNode(newNode, parent, beginSelection, endSelection, modifs);
						nodeParent = newNode;
					}
					else if(generateElements)
					{
						node = createNode(qTag->name(), "", Tag::XmlTag, newNode->tag->write());
						insertNode(node, nodeParent, nodeParent->firstChild(), 0L, modifs);
						nodeParent = node;
					}
					if(qTag == qTagList.getLast())
					{
						if(generateElements && !nodeParent->hasChildNodes())
						{
							node = createNode("#text", "&nbsp;", Tag::Text, newNode->tag->write());
							node->tag->cleanStrBuilt = true;
							insertNode(node, nodeParent, nodeParent->firstChild(), 0L, modifs);
						}
						//dirty thing to exit the while loop  when (startNodeToSurd == endNodeToSurd) ;-)
						startNodeToSurround = newNode;
					}
				}
			/**}*/
			insertNodeAroundSelection = false;
		}
		if(curNode)
			curNode = curNode->next;
	}
	return true;
}

bool kafkaCommon::DTDinsertNode(Node *newNode, Node *startNode, int startOffset, Node *endNode,
	int endOffset, NodeModifsSet *modifs)
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
		if(DTDinsertNode(newNode, startNode->parent, startNode, startNode, startOffset, endOffset, modifs))
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
		if(parentNodeQTag && parentNodeQTag->isChild(newNode))
			lastValidStartParent = parentNode;
		parentNode = parentNode->parent;
	}
	parentNode = endNode->parent;
	while(parentNode && parentNode != commonParent)
	{
		parentNodeQTag = QuantaCommon::tagFromDTD(parentNode->tag->dtd, parentNode->tag->name);
		if(parentNodeQTag && parentNodeQTag->isChild(newNode))
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

Node *kafkaCommon::createAndInsertNode(const QString &nodeName, const QString &tagString,
	int nodeType, Document *doc, Node* parent, Node* nextSibling, NodeModifsSet *modifs,
	bool merge)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "kafkaCommon::createAndInsertNode() - nodeName :" << nodeName <<
		" - tagStr :" << tagString << " - nodeType :" << nodeType << endl;
#endif
	Node *node;

	//create the new Node.
	node = createNode(nodeName, tagString, nodeType, doc);

	//insert the new Node.
	insertNode(node, parent, nextSibling, modifs, merge);

	return node;
}

Node *kafkaCommon::createAndInsertNode(const QString &nodeName, const QString &tagString,
	int nodeType, Document *doc, Node *parent, Node *nextSibling, Node *nextEndSibling,
	NodeModifsSet *modifs)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "kafkaCommon::createAndInsertNode()2- nodeName :" << nodeName <<
		" - tagStr :" << tagString << " - nodeType :" << nodeType << endl;
#endif
	Node *node;

	//create the new Node.
	node = createNode(nodeName, tagString, nodeType, doc);

	//insert the new Node.
	insertNode(node, parent, nextSibling, nextEndSibling, modifs);

	return node;
}

Node *kafkaCommon::createAndInsertNode(const QString &nodeName, const QString &tagString,
	int nodeType, Document *doc, Node *parent, Node *startNodeToSurround,
	Node *endNodeToSurround, int startOffset, int endOffset, NodeModifsSet *modifs)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "kafkaCommon::createAndInsertNode()3 - nodeName :" << nodeName <<
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

bool kafkaCommon::DTDcreateAndInsertNode(const QString &nodeName, const QString &tagString,
	int nodeType, Document *doc, Node *parent, Node *startNodeToSurround, Node *endNodeToSurround,
	int startOffset, int endOffset, NodeModifsSet *modifs, bool generateElements)
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
		modifs, generateElements);
}


bool kafkaCommon::DTDcreateAndInsertNode(const QString &nodeName, const QString &tagString,
	int nodeType, Document *doc, Node *startNode, int startOffset, Node *endNode, int endOffset,
	NodeModifsSet *modifs)
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

}

bool kafkaCommon::addNodeRecursively(Node *newNode, Node* startNode, Node *endNode,
	Node *currentNode, bool &addingStarted, int level, NodeModifsSet *modifs)
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
		if(currentNodeParentQTag->isChild(newNode))
			validCurNodeParent = true;
	}

	while(currentNode)
	{
		if(currentNode->tag->type == Tag::XmlTagEnd || currentNode->tag->type == Tag::Empty)
		{
			//do nothing
		}
		else if((newNodeQTag->isChild(currentNode)) && validCurNodeParent)
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
		else//(!newNodeQTag->isChild(currentNode))
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

	newNode = new Node(0L);
	(*newNode) = node;
	newNode->tag->cleanStrBuilt = false;

	return newNode;
}

Node* kafkaCommon::extractNode(Node *node, NodeModifsSet *modifs, bool deleteChildren)
{
	NodeModif *modif;
	Node *lastChild, *curNode;
	Node *parent, *prevNE, *nextNE, *child;

	if(!node)
		return 0L;

	if(!node->child)
		deleteChildren = true;

	parent = node->parent;
	nextNE = node->nextNE();
	prevNE = node->prevNE();
	child = node->child;

	//logging
	if(modifs)
	{
		modif = new NodeModif();
		if(deleteChildren)
			modif->setType(NodeModif::NodeAndChildsRemoved);
		else
			modif->setType(NodeModif::NodeRemoved);
		modif->setLocation(getLocation(node));
	}

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

	if(modifs)
	{
		modif->setNode(node);
		modifs->addNodeModif(modif);
	}

	return node;
}

void kafkaCommon::extractAndDeleteNode(Node *node, NodeModifsSet *modifs, bool deleteChildren,
	bool deleteClosingTag, bool mergeAndFormat)
{
	NodeModif modif;
	Node  *curNode, *nodePrev, *nodeNext, *nodeNext2, *n, *n2;
	QString nodeName, closingNodeName, namespaceName, namespaceName2;
	bool isSingle, caseSensitive, caseSensitive2;

	if(!node)
		return;

	isSingle = node->tag->single;
	nodeName = node->tag->name;
	namespaceName = node->tag->nameSpace;
	caseSensitive = node->tag->dtd->caseSensitive;
	nodePrev = node->prev;
	nodeNext = node->next;
	if(!node->child)
		deleteChildren = true;
	node = extractNode(node, modifs, deleteChildren);

	//delete the closing Tag
	if(!isSingle && deleteClosingTag && nodeNext)
	{
		curNode = nodeNext;
		while(curNode && curNode->tag->type == Tag::Empty)
			curNode = curNode->next;
		if(curNode)
		{
			closingNodeName = curNode->tag->name;
			namespaceName2 = curNode->tag->nameSpace;
			caseSensitive2 = curNode->tag->dtd->caseSensitive;
			if(QuantaCommon::closesTag(namespaceName, nodeName, caseSensitive,
				namespaceName2, closingNodeName, caseSensitive2))
			{
				curNode = nodeNext;
				while(curNode)
				{
					nodeNext2 = curNode->next;
					closingNodeName = curNode->tag->name;
					namespaceName2 = curNode->tag->nameSpace;
					caseSensitive2 = curNode->tag->dtd->caseSensitive;
					curNode = extractNode(curNode, modifs, deleteChildren);
					curNode = nodeNext2;
					if(QuantaCommon::closesTag(namespaceName, nodeName, caseSensitive,
						namespaceName2, closingNodeName, caseSensitive2))
						break;
				}
				nodeNext = curNode;
			}
		}
	}

	//merge the next and prev Nodes if they are both of type Text or Empty
	if(mergeAndFormat && nodePrev)
	{
		n = nodePrev;
		n2 = nodePrev->next;
		while(n && n2 && n2->prev != nodeNext)
		{
			if(!mergeNodes(n, n2, modifs))
				break;
			n2 = n->next;
		}
	}
}

void kafkaCommon::moveNode(Node *nodeToMove, Node *newParent, Node *newNextSibling,
	NodeModifsSet *modifs)
{
	NodeModif *modif;
	Node *newNode;

	//DON'T log the removal and addition of the same Node!! When spliting the undoRedo stack
	//it will delete the remove NodeModif and thus the Node inside which is the Node inserted.
	if(modifs)
	{
		modif = new NodeModif();
		modif->setType(NodeModif::NodeAndChildsMoved);
		modif->setLocation(getLocation(nodeToMove));
	}

	//extract the old Node.
	newNode = extractNode(nodeToMove, 0L, true);

	//insert the new Node.
	insertNode(newNode, newParent, newNextSibling, 0L);
	modif->setFinalLocation(getLocation(newNode));

	if(modifs)
		modifs->addNodeModif(modif);
}

bool kafkaCommon::splitNode(Node *n, int offset, NodeModifsSet *modifs)
{
	NodeModif *modif;
	Tag *tag;
	QString tagStr;
	Node *node;

	if(!n || n->tag->type != Tag::Text || offset <= 0 || offset >= (signed)n->tag->tagStr().length())
		return false;

	tag = new Tag(*(n->tag));
	tagStr = n->tag->tagStr();
	n->tag->setStr(tagStr.left(offset));

	//logging
	if(modifs)
	{
		modif = new NodeModif();
		modif->setType(NodeModif::NodeModified);
		modif->setTag(tag);
		modif->setLocation(getLocation(n));
		modifs->addNodeModif(modif);
	}

	node = createAndInsertNode("#text", tagStr.right(tagStr.length() - offset), Tag::Text, n->tag->write(),
		n->parent, n->next, modifs, false);

	//Node's string is a part of n's clean string
	node->tag->cleanStrBuilt = true;
	return true;
}

 bool kafkaCommon::mergeNodes(Node *n, Node *n2, NodeModifsSet *modifs)
 {
 	NodeModif *modif;
	Tag *tag;
	if(!n || !n2)
		return false;

	if((n->tag->type == Tag::Empty || n->tag->type == Tag::Text) && (n2->tag->type == Tag::Empty ||
		n2->tag->type == Tag::Text))
	{
		tag = new Tag(*(n->tag));

		//logging
		if(modifs)
		{
			modif = new NodeModif();
			modif->setType(NodeModif::NodeModified);
			modif->setTag(tag);
			modif->setLocation(getLocation(n));
			modifs->addNodeModif(modif);
		}
		n->tag->setStr(n->tag->tagStr() + n2->tag->tagStr());
		if(n->tag->type == Tag::Text || n2->tag->type == Tag::Text)
			n->tag->type = Tag::Text;
		kafkaCommon::extractAndDeleteNode(n2, modifs, false, false, false);

		return true;
	}
	return false;
 }

bool kafkaCommon::isInline(const QString &nodeNam)
{
	QString nodeName = nodeNam.lower();
	if(nodeName == "q" || nodeName == "u" || nodeName == "i" || nodeName == "b" ||
		nodeName == "cite" || nodeName == "em" || nodeName == "var" || nodeName == "em" ||
		nodeName == "tt" || nodeName == "code" || nodeName == "kbd" || nodeName == "samp" ||
		nodeName == "big" || nodeName == "small" || nodeName == "s" || nodeName == "strike" ||
		nodeName == "sub" || nodeName == "sup" || nodeName == "abbr" ||
		nodeName == "acronym" || nodeName == "a" || nodeName == "bdo" ||
		nodeName == "font" || nodeName == "#text" || nodeName == "strong" || nodeName == "dfn" ||
		nodeName == "img" ||  nodeName == "applet" ||  nodeName == "object" ||  nodeName == "basefont" ||
		nodeName == "br" ||  nodeName == "script" ||  nodeName == "map" || nodeName == "span" ||
		nodeName == "iframe" || nodeName == "input" || nodeName == "select" || nodeName == "textarea" ||
		nodeName == "label" || nodeName == "button" )
		return true;
	else
		return false;
}

void kafkaCommon::getEndPosition(const QString &tagString, int bLine, int bCol, int &eLine, int &eCol)
{
	int result, oldResult;

	result = tagString.find("\n", 0);
	if(result == -1)
	{
		eLine = bLine;
		eCol = bCol + tagString.length() - 1;
	}
	else
	{
		eLine = bLine;
		while(result != -1)
		{
			eLine++;
			oldResult = result;
			result = tagString.find("\n", result + 1);
		}
		eCol = tagString.length() - oldResult - 2;
	}
}

void kafkaCommon::getEndPosition(Node *node, int bLine, int bCol, int &eLine, int &eCol)
{
	if(!node)
	{
		eLine = 0;
		eCol = 0;
		return;
	}

	getEndPosition(node->tag->tagStr(), bLine, bCol, eLine, eCol);
}

void kafkaCommon::setTagString(Node *node, const QString &newTagString)
{
	int eLine, eCol, bLine, bCol;

	if(!node)
		return;

	node->tag->beginPos(bLine, bCol);
	node->tag->setStr(newTagString);
	getEndPosition(node, bLine, bCol, eLine, eCol);
	node->tag->setTagPosition(bLine, bCol, eLine, eCol);
}

void kafkaCommon::setTagStringAndFitsNodes(Node *node, const QString &newTagString)
{
	int eLine, eCol, oldELine, oldECol;
	bool b = false;

	if(!node)
		return;

	node->tag->endPos(oldELine, oldECol);
	setTagString(node, newTagString);
	node->tag->endPos(eLine, eCol);

	fitsNodesPosition(getNextNode(node, b), eCol - oldECol, eLine - oldELine);
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

QValueList<int> kafkaCommon::getLocation(DOM::Node domNode)
{
	QValueList<int> loc;
	int i = 0;

	while(!domNode.isNull())
	{
		i = 1;
		while(!domNode.previousSibling().isNull())
		{
			i++;
			domNode = domNode.previousSibling();
		}
		loc.prepend(i);
		domNode = domNode.parentNode();
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

DOM::Node kafkaCommon::getNodeFromLocation(QValueList<int> loc, DOM::Node rootNode)
{
	QValueList<int>::iterator it;
	DOM::Node node = rootNode;
	DOM::Node m = rootNode;
	int i;

	if(rootNode.isNull())
		return DOM::Node();

	for(it = loc.begin(); it != loc.end(); it++)
	{
		if(node.isNull())
			return DOM::Node();
		for(i = 1; i < (*it); i++)
		{
			if(node.nextSibling().isNull())
				return DOM::Node();
			node = node.nextSibling();
		}
		m = node;
		node = node.firstChild();
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

bool kafkaCommon::insertDomNode(DOM::Node node, DOM::Node parent, DOM::Node nextSibling,
	DOM::Node rootNode)
{
	if(node.isNull())
		return false;

	if(parent.isNull())
	{
		if(rootNode.isNull())
			return false;
		parent = rootNode;
	}

	try{
		parent.insertBefore(node, nextSibling);
#ifdef HEAVY_DEBUG
	}catch(DOM::DOMException e)
	{
		kdDebug(25001)<< "kafkaCommon::insertDomNode() - ERROR code :" << e.code << endl;
#else
	}catch(DOM::DOMException)
	{
#endif
		return false;
	}
	return true;
}

bool kafkaCommon::removeDomNode(DOM::Node node)
{
	DOM::Node parent = node.parentNode();

	if(parent.isNull())
		return false;

	parent.removeChild(node);

	return true;
}

DOM::Node kafkaCommon::createDomNode(const QString &nodeName, DTDStruct* dtd,
	DOM::Document rootNode)
{
	//this will change with the futur multi-DTDs support
	//It does not use exceptions handling, so everything is checked via the DTEP definitions.
	DOM::Node dn;
	QTag *qTag = 0L;

	qTag = QuantaCommon::tagFromDTD(dtd, nodeName);

	if(qTag)
		dn =  rootNode.createElement(nodeName);
#ifdef HEAVY_DEBUG
	else
		kdDebug(25001)<< "kafkaCommon::createDomNode() - ERROR bad nodeName :" <<
			nodeName << endl;
#endif

	return dn;
}

DOM::Node kafkaCommon::createDomNode(Node *node, DOM::Document rootNode)
{
	if(!node)
		return DOM::Node();

	return createDomNode(node->tag->name, node->tag->dtd, rootNode);
}

DOM::Node kafkaCommon::createTextDomNode(const QString &textString, DOM::Document rootNode)
{
	return rootNode.createTextNode(textString);
}

DOM::Node kafkaCommon::createDomNodeAttribute(const QString &nodeName, DTDStruct* dtd,
	const QString &attrName, const QString &attrValue, DOM::Document rootNode)
{
	DOM::Node attr;
	QTag *qTag = 0L;

	qTag = QuantaCommon::tagFromDTD(dtd, nodeName);
	if(!qTag)
		return DOM::Node();

	if(qTag->isAttribute(attrName))
	{
		attr = rootNode.createAttribute(attrName);
		attr.setNodeValue(attrValue);
	}
#ifdef HEAVY_DEBUG
	else
		kdDebug(25001)<< "kafkaCommon::createDomNodeAttribute() - ERROR bad attrName " <<
			attrName << endl;
#endif

	return attr;
}

DOM::Node kafkaCommon::createDomNodeAttribute(Node* node, const QString &attrName,
	DOM::Document rootNode)
{
	if(!node)
		return DOM::Node();

	return createDomNodeAttribute(node->tag->name, node->tag->dtd, attrName, "", rootNode);
}

//DOM::node kafkaCommon::createDomNodeAttribute(DOM::Node node, const QString &attrName,
//	DOM::Document rootNode)
//{
	/**if(node.isNull())
		return DOM::node();

	return createDomNodeAttribute()*/
//}

bool kafkaCommon::insertDomNodeAttribute(DOM::Node node, DOM::Node attr)
{
	if(node.isNull())
		return false;

	//should we check if the attr is valid???
	node.attributes().setNamedItem(attr);

	return true;
}

bool kafkaCommon::editDomNodeAttribute(DOM::Node node, const QString &nodeName, DTDStruct* dtd,
	const QString &attrName, const QString &attrValue, DOM::Document rootNode)
{
	DOM::Node attr;

	if(node.isNull())
		return false;

	attr = node.attributes().getNamedItem(attrName);
	if(attr.isNull())
	{
		//let's create it
		attr = createDomNodeAttribute(nodeName, dtd, attrName, attrValue, rootNode);
		if(attr.isNull())
			return false;
		insertDomNodeAttribute(node, attr);
	}

	return true;
}

bool kafkaCommon::editDomNodeAttribute(DOM::Node domNode, Node* node,
	const QString &attrName, const QString &attrValue, DOM::Document rootNode)
{
	if(!node)
		return false;

	return editDomNodeAttribute(domNode, node->tag->name, node->tag->dtd, attrName, attrValue, rootNode);
}

#ifdef HEAVY_DEBUG
void kafkaCommon::coutTree(Node *node, int indent)
#else
void kafkaCommon::coutTree(Node *, int)
#endif
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
		kdDebug(25001)<< dots << "  +++ prev " << node->prev << " next " << node->next << " parent " <<
			node->parent << " child " << node->child << endl;
		for(j = 0; j < node->tag->attrCount(); j++)
		{
			kdDebug(25001)<< dots << " *** attr" << j << " " <<
				node->tag->getAttribute(j).nameLine << ":" <<
				node->tag->getAttribute(j).nameCol << ":" <<
				node->tag->getAttribute(j).name <<  " - " <<
				node->tag->getAttribute(j).valueLine << ":" <<
				node->tag->getAttribute(j).valueCol << ":" <<
				node->tag->getAttribute(j).value << endl;
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
