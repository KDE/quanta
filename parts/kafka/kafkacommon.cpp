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

#include "kafkacommon.h"

Node *kafkaCommon::getNextNode(Node *_node, bool &goingTowardsRootNode, Node *endNode)
{
	//goto next node, my favorite part :)
	if(!_node) return 0L;
	if(goingTowardsRootNode)
	{
		if(_node->next)
		{
			goingTowardsRootNode = false;
			if(_node->next == endNode)
				return 0L;
			return _node->next;
		}
		else
		{
			if(_node->parent == endNode)
				return 0L;
			return getNextNode(_node->parent, goingTowardsRootNode);
		}
	}
	else
	{
		if(_node->child)
		{
			if(_node->child == endNode)
				return 0L;
			return _node->child;
		}
		else if(_node->next)
		{
			if(_node->next == endNode)
				return 0L;
			return _node->next;
		}
		else
		{
			goingTowardsRootNode = true;
			if(_node->parent == endNode)
				return 0L;
			return getNextNode(_node->parent, goingTowardsRootNode);
		}
	}
}

void kafkaCommon::coutTree(Node *node, int indent)
{
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
}
