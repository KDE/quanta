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
#include "resource.h"

#include "undoredo.h"

undoRedo::undoRedo(Document *doc)
	:_doc(doc)
{
	//TODO:add it to the config
	_listLimit = 50;
	_merging = false;
	_mergeNext = false;
	//add a first empty NodeModifs struct
	NodeModifsSet modifs;
	modifs.cursorX = 0;
	modifs.cursorY = 0;
	addNewModifsSet(modifs);
}

undoRedo::~undoRedo()
{

}

void undoRedo::addNewModifsSet(NodeModifsSet modifs)
{
	QValueList<NodeModifsSet>::iterator it;
	QValueList<NodeModif>::iterator it2, it3, it4;
	NodeModifsSet modifications;
	Node *n;
	QValueList<int> loc;
	bool beginToMerge = false;
	bool noMerge = false;
	bool textTyped;

#ifdef 0
	//for the release
	for(it2 = modifs.NodeModifList.begin(); it2 != modifs.NodeModifList.end(); it2++)
	{
		if((*it2).type == undoRedo::NodeRemoved || (*it2).type ==
			undoRedo::NodeAndChildsRemoved || (*it2).type ==
			undoRedo::NodeTreeRemoved)
			delete (*it2).node;
		else if((*it2).type == undoRedo::NodeModified)
			delete (*it2).tag;
	}
	return;
#endif

	if(_mergeNext)
	{
		//we merge modifs with the previous NodeModifsSet
		for(it2 = modifs.NodeModifList.begin(); it2 != modifs.NodeModifList.end(); it2++)
		{
			(*fromLast()).NodeModifList.append(*it2);
		}
		modifs.NodeModifList.clear();
		_mergeNext = false;
		//debugOutput();
		return;
	}

	if(_dontAddModifSet >= 0)
		_dontAddModifSet--;
	if(_dontAddModifSet == 0)
	{
		//we don't add the modifsset
		_dontAddModifSet = -1;
		//debugOutput();
		return;
	}

	//we remove the unnecessary Node Modifs
	//WARNING : this is based on the parser::rebuild() behavior
	//any changes to it might compromise this!
	//A NodeModifSet is supposed to be : X*NodeRemoved, X*NodeAdded, [NodeRemoved, NodeModified]
	while(!modifs.NodeModifList.empty())
	{
		it3 = modifs.NodeModifList.fromLast();
		if((*it3).type == undoRedo::NodeModified)
		{
			//A text node is merging, we don't wan't to touch this
			it3--;
			if(it3 == modifs.NodeModifList.begin())
				break;
			it3--;
			if(it3 == modifs.NodeModifList.begin() || (*it3).type != undoRedo::NodeAdded)
				break;
		}
		it2 = modifs.NodeModifList.begin();
		if((*it2).type != undoRedo::NodeRemoved)
			break;
		while(it2 != modifs.NodeModifList.end() && (*it2).type == undoRedo::NodeRemoved)
		{
			it4 = it2;
			it2++;
		}
		it2 = it4;
		//comparing it3, last NodeAdded, and it2, last NodeRemoved
		n = getNodeFromLocation((*it3).location);
		if(!n)
		{
			kdDebug(24000)<< "undoRedo::addNewModifsSet - ERROR1 - merging failed" << endl;
			break;
		}
		//kdDebug(24000)<< n->tag->tagStr() << " - " << (*it2).node->tag->tagStr() << endl;
		if(n->tag->tagStr() == (*it2).node->tag->tagStr())
		{
			//One node is removed, and the same one is replaced. Delete this two NodeModifs.
			modifs.NodeModifList.remove(it2);
			modifs.NodeModifList.remove(it3);
		}
		else
			break;
	}

	//Now merging, if possible, this NodeModifsSet. != of the first merging.
	//adding a text => 1*undoRedo::NodeAdded, modifying a text => 1*undoRedo::NodeRemoved + 1*undoRedo::NodeAdded
	//removing a text => 1*undoRedo::NodeRemoved.
	//adding a tag: [1*undoRedo::NodeRemoved + 1*undoRedo::NodeAdded] + 1*undoRedo::NodeAdded
	//modifying a tag : 1*undoRedo::NodeRemoved + 1*undoRedo::NodeAdded
	//removing a tag => 1*undoRedo::NodeRemoved + 2*undoRedo::NodeAdded [+ 1*undoRedo::NodeAdded]
	if(_merging && editorIterator != fromLast())
	{
		//we can't merge if we are in the middle of the undo stack
		_merging = false;
	}
	else if(modifs.NodeModifList.count() == 1 && modifs.NodeModifList.first().type == undoRedo::NodeAdded)
	{
		//the beginning of a text or a tag
		_merging = true;
		beginToMerge = true;
		_currentLoc = modifs.NodeModifList.last().location;
		addingText = true;
	}
	else if(modifs.NodeModifList.count() == 1 && modifs.NodeModifList.first().type == undoRedo::NodeRemoved)
	{
		//removing the last char of a text or tag
		if(_merging == true)
		{
			(*editorIterator).NodeModifList.remove((*editorIterator).NodeModifList.fromLast());
			(*editorIterator).NodeModifList.append(modifs.NodeModifList.last());
			(*editorIterator).cursorX2 = modifs.cursorX2;
			(*editorIterator).cursorY2 = modifs.cursorY2;
			_merging = false;
			noMerge = true;
		}
		//else _merging == false, simply append modifs
	}
	else if(modifs.NodeModifList.count() == 2 && modifs.NodeModifList[0].type == undoRedo::NodeRemoved &&
		modifs.NodeModifList[1].type == undoRedo::NodeAdded && modifs.NodeModifList[0].location ==
		modifs.NodeModifList[1].location)
	{
		n = getNodeFromLocation(modifs.NodeModifList[1].location);
		textTyped = (n->tag->tagStr().length() >= modifs.NodeModifList[0].node->tag->tagStr().length());
		if(_merging == false || (addingText != textTyped && modifs.NodeModifList[0].node->tag->type == Tag::Text))
		{
			//we are editing a [new] text or tag
			beginToMerge = true;
			_merging = true;
			_currentLoc = modifs.NodeModifList.last().location;
		}
		else if(modifs.NodeModifList[0].location == _currentLoc && modifs.NodeModifList[1].location == _currentLoc)
		{
			//we are editing a text or tag, and a merge was already started
			(*editorIterator).NodeModifList.remove((*editorIterator).NodeModifList.fromLast());
			(*editorIterator).NodeModifList.append(modifs.NodeModifList.last());
			(*editorIterator).cursorX2 = modifs.cursorX2;
			(*editorIterator).cursorY2 = modifs.cursorY2;
		}
		else
			_merging = false;
		addingText = textTyped;
	}
	else if((modifs.NodeModifList.count() == 3 || modifs.NodeModifList.count() == 4) &&
		modifs.NodeModifList[0].type == undoRedo::NodeRemoved &&
		modifs.NodeModifList[1].type == undoRedo::NodeAdded && modifs.NodeModifList[0].location ==
		modifs.NodeModifList[1].location && modifs.NodeModifList.last().type == undoRedo::NodeAdded)
	{
		n = getNodeFromLocation(modifs.NodeModifList[2].location);
		loc = _currentLoc;
		loc.last()++;
		if(!n)
		{
			kdDebug(24000)<< "undoRedo::addNewModifsSet - ERROR2 - merging failed" << endl;
			return;
		}
		if(_merging == false || (_merging == true && modifs.NodeModifList.count() == 3 && n->tag->tagStr().left(1) == "<"))
		{
			//Beginning a Tag or (||) starting a tag in a currently edited text.
			_merging = true;
			beginToMerge = true;
			_currentLoc = modifs.NodeModifList.last().location;
		}
		else if(modifs.NodeModifList[0].location == _currentLoc && modifs.NodeModifList[1].location == _currentLoc &&
		modifs.NodeModifList[2].location == loc)
		{
			//Adding the final ">" in the tag.
			(*editorIterator).NodeModifList.remove((*editorIterator).NodeModifList.fromLast());
			(*editorIterator).NodeModifList.append(modifs.NodeModifList[1]);
			(*editorIterator).NodeModifList.append(modifs.NodeModifList[2]);
			if(modifs.NodeModifList.count() == 4)
				(*editorIterator).NodeModifList.append(modifs.NodeModifList[3]);
			(*editorIterator).cursorX2 = modifs.cursorX2;
			(*editorIterator).cursorY2 = modifs.cursorY2;
		}
		else
			_merging = false;
	}
	else
		_merging = false;

	if((!_merging || beginToMerge) && !noMerge)
	{
		//inserting the NodeModifsSet
		if(!empty())
		{
			// if we are in the middle of the stack, we delete the end of the stack
			if(fromLast() != end() && editorIterator != fromLast())
			{
				it = editorIterator;
				it++;
				while(it != end())
				{
					for(it2 = (*it).NodeModifList.begin(); it2 != (*it).NodeModifList.end(); it2++)
					{
						if((*it2).type == undoRedo::NodeAdded || (*it2).type ==
							undoRedo::NodeAndChildsAdded || (*it2).type ==
							undoRedo::NodeTreeAdded)
							delete (*it2).node;
						else if((*it2).type == undoRedo::NodeModified)
							delete (*it2).tag;
					}
					it = erase(it);
				}
			}
		}
		append(modifs);
		while(count() > (unsigned)_listLimit)
		{
			for(it2 = (*begin()).NodeModifList.begin(); it2 != (*begin()).NodeModifList.end(); it2++)
			{
				if((*it2).type == undoRedo::NodeRemoved || (*it2).type ==
					undoRedo::NodeAndChildsRemoved || (*it2).type ==
					undoRedo::NodeTreeRemoved)
					delete (*it2).node;
				else if((*it2).type == undoRedo::NodeModified)
					delete (*it2).tag;
			}
			remove(begin());
		}
		editorIterator = fromLast();
	}

	/** A lot more to do:
	 * -NodeModifs fusionning in case of typing text multiple times, and also for some similar
	 * actions like NodeCreated and then just after NodeModified.
	 * -Flags to prevent fusionning in case of copy/paste, and to provoke NodeModifs separation
	 * in case of too heavy NodeModified (e.g. typing 100 lines of text shouldn't be undo'ed in one time)
	 */
	 //debugOutput();
}

bool undoRedo::undo(bool kafkaUndo)
{
	QTime t;
	QValueList<NodeModif>::iterator it;

	t.start();
	kdDebug(24000)<< "************* Begin Undo *****************" << endl;
	if(editorIterator == begin())//the first one is empty
		return false;
	it = (*editorIterator).NodeModifList.fromLast();
	_doc->activateParser(false);
	_doc->activateRepaintView(false);
	while(1)
	{
		if(!UndoNodeModif(*it, kafkaUndo))
			return false;
		kdDebug(24000) << "NodeModif type :" << (*it).type <<" undoed!" << endl;
		//coutTree(baseNode, 2);
		if(it == (*editorIterator).NodeModifList.begin())
			break;
		it--;
	}
	_doc->viewCursorIf->setCursorPositionReal((*editorIterator).cursorY, (*editorIterator).cursorX);
	editorIterator--;
	//We need to update the internal pointer of baseNode in the parser.
	parser->setM_node(baseNode);
	_doc->activateRepaintView(true);
	_doc->activateParser(true);
	kdDebug(24000)<< "************* End Undo *****************" << endl;
	kdDebug(24000) << "undoRedo::undo() : " << t.elapsed() << " ms \n";
	//debugOutput();
	return !(editorIterator == begin());
}

bool undoRedo::redo(bool kafkaUndo)
{
	QValueList<NodeModif>::iterator it;
	QTime t;
	bool success;

	t.start();
	kdDebug(24000)<< "************* Begin Redo *****************" << endl;
	if(editorIterator == fromLast())
		return false;
	editorIterator++;
	_doc->activateParser(false);
	_doc->activateRepaintView(false);
	for(it = (*editorIterator).NodeModifList.begin(); it != (*editorIterator).NodeModifList.end(); it++)
	{
		/** Redoing is the opposite of Undoing ... */
		if((*it).type == undoRedo::NodeTreeAdded)
			(*it).type = undoRedo::NodeTreeRemoved;
		else if((*it).type == undoRedo::NodeAndChildsAdded)
			(*it).type = undoRedo::NodeAndChildsRemoved;
		else if((*it).type == undoRedo::NodeAdded)
			(*it).type = undoRedo::NodeRemoved;
		else if((*it).type == undoRedo::NodeRemoved)
			(*it).type = undoRedo::NodeAdded;
		else if((*it).type == undoRedo::NodeAndChildsRemoved)
			(*it).type = undoRedo::NodeAndChildsAdded;
		else if((*it).type == undoRedo::NodeTreeRemoved)
			(*it).type = undoRedo::NodeTreeAdded;

		success = UndoNodeModif(*it, kafkaUndo);
		kdDebug(24000) << "NodeModif type :" << (*it).type <<" redoed!" << endl;

		if((*it).type == undoRedo::NodeTreeRemoved)
			(*it).type = undoRedo::NodeTreeAdded;
		else if((*it).type == undoRedo::NodeAndChildsRemoved)
			(*it).type = undoRedo::NodeAndChildsAdded;
		else if((*it).type == undoRedo::NodeRemoved)
			(*it).type = undoRedo::NodeAdded;
		else if((*it).type == undoRedo::NodeAdded)
			(*it).type = undoRedo::NodeRemoved;
		else if((*it).type == undoRedo::NodeAndChildsAdded)
			(*it).type = undoRedo::NodeAndChildsRemoved;
		else if((*it).type == undoRedo::NodeTreeAdded)
			(*it).type = undoRedo::NodeTreeRemoved;
	}
	_doc->viewCursorIf->setCursorPositionReal((*editorIterator).cursorY2, (*editorIterator).cursorX2);
	//We need to update the internal pointer of baseNode in the parser.
	parser->setM_node(baseNode);
	_doc->activateRepaintView(true);
	_doc->activateParser(true);
	kdDebug(24000)<< "************* End Redo *****************" << endl;
	kdDebug(24000) << "undoRedo::redo() : " << t.elapsed() << " ms \n";
	//debugOutput();
	return !(editorIterator == fromLast());
}

bool undoRedo::UndoNodeModif(NodeModif &_nodeModif, bool kafkaUndo)
{
	Node *_node = 0L, *_nodeNext = 0L, *n = 0L, *m = 0L, *newNode = 0L;
	int bLine, bCol, eLine, eCol, bLine2, bCol2, eLine2, eCol2, bLine3, bCol3, i;
	bool b;
	QValueList<int> loc;
	Tag *_tag;
	QString text, totalText;

	if(_nodeModif.type == undoRedo::NodeTreeAdded)
	{
		/** Remove all the text and set baseNode to 0L */
		_doc->editIf->removeText(0, 0, _doc->editIf->numLines() - 1,
			_doc->editIf->lineLength(_doc->editIf->numLines() - 1));
		_nodeModif.node = baseNode;
		baseNode = 0L;
	}
	else if(_nodeModif.type == undoRedo::NodeAndChildsAdded ||
		_nodeModif.type == undoRedo::NodeAdded)
	{
		/** Removing the node */
		_node = getNodeFromLocation(_nodeModif.location);
		if(!_node)
		{
			kdDebug(24000)<< "undoRedo::UndoNodeModif() - ERROR1" << endl;
			_doc->activateParser(true);
			return false;
		}
		_nodeModif.node = _node;
		_node->tag-> beginPos(bLine, bCol);
		if(_node->next && _node->next->closesPrevious)
			_node->next->closesPrevious = false;
		if(_nodeModif.type == undoRedo::NodeAndChildsAdded ||
			(_nodeModif.type == undoRedo::NodeAdded && !_node->child))
		{
			b = true;
			_nodeNext = getNextNode(_node, b);
			if(_node->prev)
				_node->prev->next = _node->next;
			if(_node->next)
				_node->next->prev = _node->prev;
			if(_node->parent && _node->parent->child == _node)
				_node->parent->child = _node->next;
			if(_node->closesPrevious && _node->next)
			{
				/** If _node close the previous Node, it means that all the node next to _node
				  * will be moved next to the last child of _node->prev (need to be updated) */
				n = _node->prev;
				if(!n)
				{
					kdDebug(24000)<< "undoRedo::UndoNodeModif() - ERROR2" << endl;
					_doc->activateParser(true);
					return false;
				}
				while(n->child)
				{
					n = n->child;
					while(n->next)
						n = n->next;
				}
				m = _node->next;
				if(_node->prev->child)
				{
					_node->next->prev = n;
					n->next = _node->next;
					while(m)
					{
						m->parent = n->parent;
						m = m->next;
					}
				}
				else
				{
					_node->prev->child = _node->next;
					_node->next->prev = 0L;
					while(m)
					{
						m->parent = _node->prev;
						m = m->next;
					}
				}
				_node->prev->next = 0L;
				i = 0;
				n = _node->next;
				while(n)
				{
					i++;
					n = n->next;
				}
				_nodeModif.childsNumber2 = i;
			}
			n = _node;
			while(n->child)
			{
				n = n->child;
				while(n->next)
					n = n->next;
			}
			n->tag->endPos(eLine, eCol);
			_node->parent = 0L;
			_node->next = 0L;
			_node->prev = 0L;
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
				/** _node is deleted, all its child go up */
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
			_node->child = 0L;
			_nodeModif.childsNumber = i;
		}
		fitsNodesPosition(_nodeNext, bCol - eCol - 1, bLine - eLine);
		_doc->editIf->removeText(bLine, bCol, eLine, eCol + 1);
	}
	else if(_nodeModif.type == undoRedo::NodeModified)
	{
		/** Simply replacing the tag(node->tag) of the node by the old tag. */
		_node = getNodeFromLocation(_nodeModif.location);
		if(!_node)
		{
			kdDebug(24000)<< "undoRedo::UndoNodeModif() - ERROR3" << endl;
			_doc->activateParser(true);
			return false;
		}
		_tag = _nodeModif.tag;
		_nodeModif.tag = _node->tag;
		b = false;
		_nodeNext = getNextNode(_node, b);
		_node->tag->beginPos(bLine, bCol);
		_node->tag->endPos(eLine, eCol);
		_node->tag = _tag;
		text = _node->tag->tagStr();
		if(kafkaUndo)
		{
			bLine2 = bLine;
			bCol2 = bCol;
			eLine2 = bLine2;
			eCol2 = bCol2;
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
		}
		else
		{
			if(_node->prev)
			{
				m = _node->prev;
				while(m->child)
				{
					m = m->child;
					while(m->next)
						m = m->next;
				}
				m->tag->endPos(bLine3, bCol3);
			}
			else if(_node->parent)
				_node->parent->tag->endPos(bLine3, bCol3);
			else
			{
				bCol3 = -1;
				bLine3 = 0;
			}
			bCol3++;
			_node->tag->beginPos(bLine2, bCol2);
			_node->tag->endPos(eLine2, eCol2);
			if(bCol2 == -1)
			{
				bCol3 = -1;
				bLine3++;
			}
			if((eLine2 - bLine2) == 0)
				_node->tag->setTagPosition(bLine3, bCol3, bLine3, bCol3 + (eCol2 - bCol2));
			else
				_node->tag->setTagPosition(bLine3, bCol3, bLine3 + (eLine2 - bLine2), eCol2);
		}
		_doc->editIf->removeText(bLine, bCol, eLine, eCol + 1);
		_doc->editIf->insertText(bLine3, bCol3, _node->tag->tagStr());
		_node->tag->beginPos(bLine2, bCol2);
		_node->tag->endPos(eLine2, eCol2);
		fitsNodesPosition(_nodeNext, (eCol2 - bCol2) - (eCol - bCol),
			(eLine2 - bLine2) - (eLine - bLine));
	}
	else if(_nodeModif.type == undoRedo::NodeRemoved ||
		_nodeModif.type == undoRedo::NodeAndChildsRemoved)
	{
		/** Adding the node */
		newNode = _nodeModif.node;
		_node = getNodeFromLocation(_nodeModif.location);
		if(!_node)
		{
			/** No node at this location, getting the parent Node and appending newNode after
			* the last child of the parent. */
			loc = _nodeModif.location;
			loc.remove(loc.fromLast());
			if(loc.empty())
			{
				/** No parent, adding it on top of the tree. */
				newNode->parent = 0L;
				if(baseNode)
				{
					_node = baseNode;
					while(_node->next)
						_node = _node->next;
					newNode->next = 0L;
					newNode->prev = _node;
					_node->next = newNode;
				}
				else
				{
					baseNode = newNode;
					newNode->prev = 0L;
					newNode->next = 0L;
				}
			}
			else
			{
				_node = getNodeFromLocation(loc);
				if(!_node)
				{
					kdDebug(24000)<< "undoRedo::UndoNodeModif() - ERROR4" << endl;
					_doc->activateParser(true);
					return false;
				}
				newNode->parent = _node;
				n = _node->child;
				if(n)
				{
					while(n->next)
						n = n->next;
					n->next = newNode;
				}
				else
					_node->child = newNode;
				newNode->prev = n;
				newNode->next = 0L;
				//bCol++;
			}
			if(_nodeModif.childsNumber2 != 0)
			{
				/** Moving nodes from the last child of newNode->prev next to the right of
				  * newNode. */
				n = newNode->prev;
				if(!n)
				{
					kdDebug(24000)<< "undoRedo::UndoNodeModif() - ERROR5" << endl;
					_doc->activateParser(true);
					return false;
				}
				while(n->child)
				{
					n = n->child;
					while(n->next)
						n = n->next;
				}
				for(i = 0; i < _nodeModif.childsNumber2; i++)
				{
					if(i == (_nodeModif.childsNumber2 - 1))
					{
						if(n->prev)
							n->prev->next = 0L;
						n->prev = newNode;
						newNode->next = n;
						if(n->parent && n->parent->child == n)
							n->parent->child = 0L;
					}
					n->parent = newNode->parent;
					n = n->prev;
				}
			}
			b = true;
			_nodeNext = getNextNode(newNode, b);
		}
		else
		{
			/** A node is already here. Moving it to the right and adding newNode here.*/
			m = _node;
			if(_node->parent && _node->parent->child == _node)
				_node->parent->child = newNode;
			newNode->parent = _node->parent;
			if(_node->prev)
				_node->prev->next = newNode;
			newNode->prev = _node->prev;
			if(_nodeModif.type == undoRedo::NodeRemoved && _nodeModif.childsNumber != 0)
			{
				/** Some Nodes at the right of newNode will be childs of newNode. */
				for(i = 0; i < _nodeModif.childsNumber; i++)
				{
					if(i == 0)
					{
						newNode->child = m;
						m->prev = 0L;
					}
					m->parent = newNode;
					if(i == (_nodeModif.childsNumber - 1))
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
						kdDebug(24000)<< "undoRedo::UndoNodeModif() - ERROR6" << endl;
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
			_nodeNext = _node;
		}
		if(newNode->prev)
		{
			m = newNode->prev;
			while(m->child)
			{
				m = m->child;
				while(m->next)
					m = m->next;
			}
			m->tag->endPos(bLine, bCol);
		}
		else if(newNode->parent)
			newNode->parent->tag->endPos(bLine, bCol);
		else
		{
			bCol = -1;
			bLine = 0;
		}
		bCol++;

		_nodeModif.node = 0L;
		if(newNode->next && ("/" + newNode->tag->name.lower() == newNode->next->tag->name.lower()))
			newNode->next->closesPrevious = true;
		eCol = bCol;
		eLine = bLine;
		n = newNode;
		b = false;
		newNode->tag->beginPos(bLine2, bCol2);
		if(bCol2 == -1)
		{
			bCol = -1;
			bLine++;
		}
		while(n)
		{
			text = n->tag->tagStr();
			//remove any reference to a now deleted QListViewItem
			n->listItem = 0L;
			if(kafkaUndo)
			{
				bLine2 = eLine;
				bCol2 = eCol;
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
				n->tag->setTagPosition(bLine2, bCol2, eLine, eCol);
			}
			else
			{
				n->tag->beginPos(bLine2, bCol2);
				n->tag->endPos(eLine2, eCol2);
				if((eLine2 - bLine2) == 0)
				{
					n->tag->setTagPosition(eLine, eCol, eLine, eCol + (eCol2 - bCol2));
					eCol += (eCol2 - bCol2);
				}
				else
				{
					n->tag->setTagPosition(eLine, eCol, eLine + (eLine2 - bLine2), eCol2);
					eCol = eCol2;
				}
				eLine += (eLine2 - bLine2);
			}
			totalText += text;
			if(_nodeModif.type == undoRedo::NodeRemoved)
				break;
			n = getNextNode(n, b, newNode);
			if(n)
			{
				n->tag->beginPos(bLine3, bCol3);
				if(bCol3 == -1)
				{
					eLine++;
					eCol = -1;
				}
				else
					eCol++;
			}
		}
		_doc->editIf->insertText(bLine, bCol, totalText);
		fitsNodesPosition(_nodeNext, eCol - bCol + 1, eLine - bLine);
	}
	else if(_nodeModif.type == undoRedo::NodeTreeRemoved)
	{
		/** Appending the tree in the editor and putting its adress in baseNode. */
		baseNode = _nodeModif.node;
		_nodeModif.node = 0L;
		n = baseNode;
		b = false;
		while(n)
		{
			//remove any reference to a now deleted QListViewItem
			n->listItem = 0L;
			totalText += n->tag->tagStr();
			n = getNextNode(n, b);
		}
		_doc->editIf->insertText(0, 0, totalText);
	}
	else if(_nodeModif.type == undoRedo::NodeMoved ||
		_nodeModif.type == undoRedo::NodeAndChildsMoved)
	{
		/** NOT IMPLEMENTED and i don't think i will need to. */
	}

	return true;
}

void undoRedo::debugOutput()
{
	int i = 0;
	bool afterEditorIt = false;

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
		if((*it).NodeModifList.empty())
		{
			kdDebug(24000)<< "== Empty!" << endl;
			kdDebug(24000)<< "== End Node Modifications set #" << i << endl;
			i++;
			continue;
		}
		kdDebug(24000)<< "== Cursor Pos: " << (*it).cursorY << ":" << (*it).cursorX << endl;
		kdDebug(24000)<< "== Cursor Pos2:" << (*it).cursorY2 << ":" << (*it).cursorX2 << endl;
		QValueList<NodeModif>::iterator it2;
		for(it2 = (*it).NodeModifList.begin(); it2 != (*it).NodeModifList.end(); ++it2)
		{
			kdDebug(24000)<< "==== NodeModif type:" << (*it2).type << endl;
			kdDebug(24000)<< "==== Location1: " << endl;
			QValueList<int>::iterator it3;
			if((*it2).location.empty())
			{
				kdDebug(24000)<< "==== Empty location!!" << endl;
			}
			else if((*it2).type != undoRedo::NodeTreeAdded && (*it2).type != undoRedo::NodeTreeRemoved)
			{
				for(it3 = (*it2).location.begin(); it3 != (*it2).location.end(); ++it3)
					kdDebug(24000)<< (*it3) << endl;
			}
			if((((*it2).type == undoRedo::NodeRemoved && !afterEditorIt) ||
				((*it2).type == undoRedo::NodeAdded && afterEditorIt)) && (*it2).node)
				kdDebug(24000)<< "==== Node: " << (*it2).node->tag->name <<
					" - contents: " << (*it2).node->tag->tagStr() << endl;
			if((*it2).type == undoRedo::NodeModified)
				kdDebug(24000)<< "==== Tag: " << (*it2).tag->name <<
					" - contents: " << (*it2).tag->tagStr() << endl;
			if(((*it2).type == undoRedo::NodeRemoved && !afterEditorIt) ||
				((*it2).type == undoRedo::NodeAdded && afterEditorIt))
				kdDebug(24000)<< "==== ChildsNumber1 : " << (*it2).childsNumber <<
					" - ChildsNumber2 : " << (*it2).childsNumber2 << endl;
		}
		kdDebug(24000)<< "== End Node Modifications set #" << i << endl;
		i++;
		if(it == editorIterator)
			afterEditorIt = true;
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
