/***************************************************************************
                                undoredo.cpp
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

//debug only
#include <qdatetime.h>

#include "document.h"
#include "parser/node.h"
#include "parser/tag.h"
//debug only
#include "resource.h"

#include "undoredo.h"

undoRedo::undoRedo(Document *doc)
	:_doc(doc)
{
	//TODO:add it to the config
	_listLimit = 50;
	//add a first empty NodeModifs struct
	NodeModifsSet modifs;
	addNewModifsSet(modifs);
}

undoRedo::~undoRedo()
{

}

void undoRedo::addNewModifsSet(NodeModifsSet modifs)
{
	if(!empty())
	{
		/** A new UndoRedo struct : if we are in the middle of the stack, we delete the end of the stack*/
		if(fromLast() != end() && editorIterator != fromLast())
		{
			QValueList<NodeModifsSet>::iterator it = editorIterator;
			it++;
			while(it != end())
			{
				it = erase(it);
			}
		}
	}
	append(modifs);
	while(count() > (unsigned)_listLimit)
		remove(begin());
	editorIterator = fromLast();
	/** A lot more to do:
	 * -NodeModifs fusionning in case of typing text multiple times, and also for some similar
	 * actions like NodeCreated and then just after NodeModified.
	 * -Flags to prevent fusionning in case of copy/paste, and to provoke NodeModifs separation
	 * in case of too heavy NodeModified (e.g. typing 100 lines of text shouldn't be undo'ed in one time)
	 */
	 //debugOutput();
}

bool undoRedo::undo()
{
	QTime t;
	NodeModifsSet::iterator it;
	Node *_node = 0L, *_nodeNext = 0L, *n = 0L, *m = 0L, *newNode = 0L;
	int bLine, bCol, eLine, eCol, bLine2, bCol2, eLine2, eCol2, bLine3, bCol3, i;
	bool b;
	QValueList<int> loc;
	Tag *_tag;
	QString text, totalText;

	t.start();
	if(editorIterator == begin())//the first one is empty
		return false;
	it = (*editorIterator).fromLast();
	_doc->activateParser(false);
	while(1)
	{
		if((*it).type == undoRedo::NodeTreeAdded)
		{
			_doc->editIf->removeText(0, 0, _doc->editIf->numLines() - 1, _doc->editIf->lineLength(_doc->editIf->numLines() - 1));
			(*it).node = baseNode;
			baseNode = 0L;
		}
		else if((*it).type == undoRedo::NodeAndChildsAdded ||
			(*it).type == undoRedo::NodeAdded)
		{
			_node = getNodeFromLocation((*it).location);
			if(!_node)
			{
				kdDebug(24000)<< "undoRedo::undo() - ERROR1" << endl;
				_doc->activateParser(true);
				return false;
			}
			(*it).node = _node;
			_node->tag-> beginPos(bLine, bCol);
			if((*it).type == undoRedo::NodeAndChildsAdded ||
				((*it).type == undoRedo::NodeAdded && !_node->child))
			{
				b = true;
				_nodeNext = getNextNode(_node, b);
				if(_node->prev)
					_node->prev->next = _node->next;
				if(_node->next)
					_node->next->prev = _node->prev;
				if(_node->parent && _node->parent->child == _node)
					_node->parent->child = _node->next;
				_node->parent = 0L;
				_node->next = 0L;
				_node->prev = 0L;
				n = _node;
				while(n->child)
				{
					n = n->child;
					while(n->next)
						n = n->next;
				}
				n->tag->endPos(eLine, eCol);
			}
			else
			{
				b = false;
				_nodeNext = getNextNode(_node, b);
				_node->tag->endPos(eLine, eCol);
				if(_node->parent && _node->parent->child == _node)
					_node->parent->child = _node->child;
				n = _node->child;//not null
				i = 0;
				while(n)
				{
					if(i == 0)
					{
						n->prev = _node->prev;
						if(_node->prev)
							_node->prev->next = n;
					}
					i++;
					m = n;
					n->parent = _node->parent;
					n = n->next;
					if(!n)
					{
						m->next = _node->next;
						if(_node->next)
							_node->next->prev = m;
					}
				}
				_node->parent = 0L;
				_node->next = 0L;
				_node->prev = 0L;
				(*it).childsNumber = i;
			}
			fitsNodesPosition(_nodeNext, bCol - eCol, bLine - eLine);
			_doc->editIf->removeText(bLine, bCol, eLine, eCol + 1);
		}
		else if((*it).type == undoRedo::NodeModified)
		{
			_node = getNodeFromLocation((*it).location);
			if(!_node)
			{
				kdDebug(24000)<< "undoRedo::undo() - ERROR2" << endl;
				_doc->activateParser(true);
				return false;
			}
			_tag = (*it).tag;
			(*it).tag = _node->tag;
			b = false;
			_nodeNext = getNextNode(_node, b);
			_node->tag->beginPos(bLine, bCol);
			_node->tag->endPos(eLine, eCol);
			_node->tag = _tag;
			bLine2 = bLine;
			bCol2 = bCol;
			eLine2 = bLine2;
			eCol2 = bCol2;
			text = _node->tag->tagStr();
			for(i = 0; (unsigned)i < text.length(); i++)
			{
				if(text[i] != QChar(Qt::Key_Return))
					eCol2++;
				else
				{
					eLine2++;
					eCol2 = 0;
				}
			}
			_node->tag->setTagPosition(bLine2, bCol2, eLine2, eCol2);
			_doc->editIf->removeText(bLine, bCol, eLine, eCol + 1);
			_doc->editIf->insertText(bLine2, bCol2, newNode->tag->tagStr());
			fitsNodesPosition(_nodeNext, (bCol2 - eCol2) - (bCol - eCol),
				(bLine2 - eLine2) - (bLine - eLine));
		}
		else if((*it).type == undoRedo::NodeRemoved ||
			(*it).type == undoRedo::NodeAndChildsRemoved)
		{
			newNode = (*it).node;
			_node = getNodeFromLocation((*it).location);
			if(!_node)
			{//no node at this location, simply appending newNode here
				loc = (*it).location;
				loc.remove(loc.fromLast());
				if(loc.empty())
				{
					newNode->parent = 0L;
					if(baseNode)
					{
						_node = baseNode;
						while(_node->next)
							_node = _node->next;
						_node->tag->endPos(bLine, bCol);
						bCol++;
						b = true;
						_nodeNext = 0L;
						newNode->next = 0L;
						newNode->prev = _node;
						_node->next = newNode;
					}
					else
					{
						baseNode = newNode;
						bCol = 0;
						bLine = 0;
						_nodeNext = 0L;
						newNode->prev = 0L;
						newNode->next = 0L;
					}
				}
				else
				{
					_node = getNodeFromLocation(loc);
					if(!_node)
					{
						kdDebug(24000)<< "undoRedo::undo() - ERROR3" << endl;
						_doc->activateParser(true);
						return false;
					}
					b = true;
					_nodeNext = getNextNode(_node, b);
					newNode->parent = _node;
					if(!_node->child)
						_node->child = newNode;
					_node->tag->endPos(bLine, bCol);
					_node = _node->child;
					if(_node)
					{
						while(_node->next)
							_node = _node->next;
						_node->tag->endPos(bLine, bCol);
						b = true;
						_nodeNext = getNextNode(_node, b);
						_node->next = newNode;
					}
					newNode->prev = _node;
					newNode->next = 0L;
					bCol++;
				}
				eCol = bCol;
				eLine = bLine;
				n = newNode;
				b = false;
				while(n)
				{
					text = n->tag->tagStr();
					bLine3 = eLine;
					bCol3 = eCol;
					for(i = 0; (unsigned)i < text.length(); i++)
					{
						if(text[i] != QChar(Qt::Key_Return))
							eCol++;
						else
						{
							eLine++;
							eCol = 0;
						}
					}
					n->tag->setTagPosition(bLine3, bCol3, eLine, eCol);
					totalText += text;
					if((*it).type == undoRedo::NodeRemoved)
						break;
					n = getNextNode(n, b, newNode);
					if(n)
					{
						n->tag->beginPos(bLine2, bCol2);
						if(bCol2 == 0)
						{
							eLine++;
							eCol = 0;
						}
						else
							eCol++;
					}
				}
			}
			else
			{//ouch, a node here. Moving it.
				m = _node;
				if(_node->parent && _node->parent->child == _node)
					_node->parent = newNode;
				newNode->parent = _node->parent;
				if(_node->prev)
					_node->prev->next = newNode;
				newNode->prev = _node->prev;
				if((*it).type == undoRedo::NodeRemoved)
				{
					for(i = 0; (*it).childsNumber < i; i++)
					{
						if(i == 0)
						{
							newNode->child = m;
							m->prev = 0L;
						}
						m->parent = newNode;
						if(i == ((*it).childsNumber - 1))
						{
							if(m->next)
								m->next->prev = newNode;
							newNode->next = m->next;
							m->next = 0L;
							break;
						}
						if(m->next)
							m = m->next;
						else//ERROR
						{
							kdDebug(24000)<< "undoRedo::undo() - ERROR4" << endl;
							_doc->activateParser(true);
							return false;
						}
					}
				}
				else
				{
					newNode->next = _node;
					_node->prev = newNode;
				}
				(*it).node = 0L;
				_nodeNext = _node;
				if(newNode->prev)
					newNode->prev->tag->endPos(bLine, bCol);
				else if(newNode->parent)
					newNode->parent->tag->endPos(bLine, bCol);
				//else ERROR;
				bCol++;
				eCol = bCol;
				eLine = bLine;
				n = newNode;
				b = false;
				while(n)
				{
					text = n->tag->tagStr();
					bLine3 = eLine;
					bCol3 = eCol;
					for(i = 0; (unsigned)i < text.length(); i++)
					{
						if(text[i] != QChar(Qt::Key_Return))
							eCol++;
						else
						{
							eLine++;
							eCol = 0;
						}
					}
					n->tag->setTagPosition(bLine3, bCol3, eLine, eCol);
					totalText += text;
					if((*it).type == undoRedo::NodeRemoved)
						break;
					n = getNextNode(n, b, newNode);
					if(n)
					{
						n->tag->beginPos(bLine2, bCol2);
						if(bCol2 == 0)
						{
							eLine++;
							eCol = 0;
						}
						else
							eCol++;
					}
				}
			}
			_doc->editIf->insertText(bLine, bCol, totalText);
			fitsNodesPosition(_nodeNext, bCol - eCol, eLine - bLine);
		}
		else if((*it).type == undoRedo::NodeTreeRemoved)
		{
 			baseNode = (*it).node;
 			(*it).node = 0L;
			n = baseNode;
			b = false;
			while(n)
			{
				totalText += n->tag->tagStr();
				n = getNextNode(n, b);
			}
			_doc->editIf->insertText(0, 0, totalText);
		}
		else if((*it).type == undoRedo::NodeMoved ||
			(*it).type == undoRedo::NodeAndChildsMoved)
		{

		}

		if(it == (*editorIterator).begin())
			break;
		it--;
		_nodeNext = 0L;
		_node = 0L;
		totalText = "";
		n = 0L;
		m = 0L;
	}
	editorIterator--;
	kdDebug(24000) << "undoRedo::undo() : " << t.elapsed() << " ms \n";
	debugOutput();
	_doc->activateParser(true);
	return !(editorIterator == begin());
}

bool undoRedo::redo()
{
	NodeModifsSet::iterator it;
	QTime t;

	t.start();
	if(editorIterator == fromLast())
		return false;
	editorIterator++;
	_doc->activateParser(false);
	for(it = (*editorIterator).begin(); it != (*editorIterator).end(); it++)
	{
		if((*it).type == undoRedo::NodeAndChildsAdded ||
			(*it).type == undoRedo::NodeAdded)
		{

		}
		else if((*it).type == undoRedo::NodeModified)
		{

		}
		else if((*it).type == undoRedo::NodeRemoved ||
			(*it).type == undoRedo::NodeAndChildsRemoved)
		{

		}
		else if((*it).type == undoRedo::NodeMoved ||
			(*it).type == undoRedo::NodeAndChildsMoved)
		{

		}
	}
	kdDebug(24000) << "undoRedo::redo() : " << t.elapsed() << " ms \n";
	debugOutput();
	_doc->activateParser(true);
}

void undoRedo::debugOutput()
{
	int i = 0;
	kdDebug(24000)<< "Undo/redo stack contents:" << endl;
	if(empty())
	{
		kdDebug(24000)<< "Empty!" << endl;
		return;
	}
	QValueList<NodeModifsSet>::iterator it;
	for(it = begin(); it != end(); ++it )
	{
		kdDebug(24000)<< "== Node Modifications set #" << i << endl;
		if((*it).empty())
		{
			kdDebug(24000)<< "== Empty!" << endl;
			kdDebug(24000)<< "== End Node Modifications set #" << i << endl;
			i++;
			continue;
		}
		NodeModifsSet::iterator it2;
		for(it2 = (*it).begin(); it2 != (*it).end(); ++it2)
		{
			if((*it2).node)
				kdDebug(24000)<< "==== NodeModif type:" << (*it2).type << " - node:" <<
					(*it2).node->tag->name << endl;
			else
				kdDebug(24000)<< "==== NodeModif type:" << (*it2).type << endl;
			if((*it2).location.empty())
			{
				kdDebug(24000)<< "==== Empty location!!" << endl;
				continue;
			}
			QValueList<int>::iterator it3;
			kdDebug(24000)<< "==== Location: " << endl;
			for(it3 = (*it2).location.begin(); it3 != (*it2).location.end(); ++it3)
			{
				kdDebug(24000)<< (*it3) << endl;
			}
			kdDebug(24000)<< endl;
		}
		kdDebug(24000)<< "== End Node Modifications set #" << i << endl;
		i++;
	}
	kdDebug(24000)<< "End Undo/redo stack contents" << endl;
	coutTree(baseNode, 2);
}

QValueList<int> undoRedo::getLocation(Node * _node)
{
	QValueList<int> loc;
	int i = 0;

	while(_node)
	{
		i = 1;
		while(_node->prev)
		{
			i++;
			_node = _node->prev;
		}
		loc.prepend(i);
		_node = _node->parent;
	}
	return loc;
}

void undoRedo::coutTree(Node *node, int indent)
{
	QString output;
	int bLine, bCol, eLine, eCol, j;
	if(!node)
		kdDebug(24000)<< "undoRedo::coutTree() - bad node!" << endl;
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

Node * undoRedo::getNodeFromLocation(QValueList<int> loc)
{
	QValueList<int>::iterator it;
	Node *_node = baseNode;
	Node *m;
	int i;

	if(!_node) return 0L;
	for(it = loc.begin(); it != loc.end(); it++)
	{
		if(!_node)
			return 0L;
		for(i = 1; i < (*it); i++)
		{
			if(!_node->next)
				return 0L;
			_node = _node->next;
		}
		m = _node;
		_node = _node->child;
	}
	return m;
}

void undoRedo::fitsNodesPosition(Node* _startNode, int colMovement, int lineMovement, int colEnd, int lineEnd)
{
	bool b = false;
	int j, SNbeginLine, SNbeginCol/**, SNlastLine, SNlastCol*/;
	int beginLine, beginCol, lastLine, lastCol;
	Node *_node = _startNode;

	if(!_node) return;

	_startNode->tag->beginPos(SNbeginLine, SNbeginCol);
	//_startNode->tag->endPos(SNlastLine, SNlastCol);

	while(_node)
	{
		_node->tag->beginPos(beginLine, beginCol);
		_node->tag->endPos(lastLine, lastCol);
		if(beginLine >= lineEnd && beginCol >= colEnd &&
			colEnd != -2 && lineEnd != -2)
			return;
		if(beginLine == SNbeginLine && lastLine == SNbeginLine)
			_node->tag->setTagPosition(beginLine + lineMovement,
				beginCol + colMovement, lastLine + lineMovement ,
				lastCol + colMovement);
		else if(beginLine == SNbeginLine)//&&lastLine != SNbeginLine
			_node->tag->setTagPosition(beginLine + lineMovement,
				beginCol + colMovement, lastLine + lineMovement ,
				lastCol);
		else
			_node->tag->setTagPosition(beginLine + lineMovement,
				beginCol, lastLine + lineMovement, lastCol);
		for(j = 0; j < _node->tag->attrCount(); j++)
		{
			if(_node->tag->getAttribute(j).nameLine == SNbeginLine)
			{
				_node->tag->getAttribute(j).nameLine += lineMovement;
				_node->tag->getAttribute(j).nameCol += colMovement;
				_node->tag->getAttribute(j).valueLine += lineMovement;
				_node->tag->getAttribute(j).valueCol += colMovement;
			}
			else
			{
				_node->tag->getAttribute(j).nameLine += lineMovement;
				_node->tag->getAttribute(j).valueLine += lineMovement;
			}
		}
		_node = getNextNode(_node, b);
	}
}

Node *undoRedo::getNextNode(Node *_node, bool &goingTowardsRootNode, Node *endNode)
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
