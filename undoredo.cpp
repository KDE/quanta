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

//debug only
#include <qdatetime.h>
//end debug only
#include <qtabwidget.h>

#include <kdebug.h>
#include <kiconloader.h>
#ifdef BUILD_KAFKAPART
#include <dom/dom_node.h>
#include <dom/dom_exception.h>
#endif

#include "document.h"
#include "quanta.h"
#include "quantaview.h"
#include "parser/node.h"
#include "parser/tag.h"
#include "resource.h"
#ifdef BUILD_KAFKAPART
#include "parts/kafka/wkafkapart.h"
#endif

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
	modifs.isModified = false;
	//don't call addNewModifsSet(modifs); otherwise it will delete it as it is empty
	append(modifs);
	editorIterator = begin();
	kafkaIterator = begin();
}

undoRedo::~undoRedo()
{

}

void undoRedo::addNewModifsSet(NodeModifsSet modifs, bool kafkaModifSet)
{
	kdDebug(24000)<< "undoRedo::addNewModifsSet" << endl;
	QValueList<NodeModifsSet>::iterator it;
	QValueList<NodeModif>::iterator it2, it3, it4;
	NodeModifsSet modifications;
	Node *n;
	QValueList<int> loc;
	bool beginToMerge = false;
	bool noMerge = false;
	bool textTyped;

#ifndef BUILD_KAFKAPART
	//Delete the modifs
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
		debugOutput();
		return;
	}

	if(_dontAddModifSet >= 0)
		_dontAddModifSet--;
	if(_dontAddModifSet == 0)
	{
		//we don't add the modifsset
		_dontAddModifSet = -1;
		debugOutput();
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
	if(modifs.NodeModifList.empty())
		return;

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
		if(kafkaModifSet)
			kafkaIterator = fromLast();
		else
			editorIterator = fromLast();
	}

	/** A lot more to do:
	 * -NodeModifs fusionning in case of typing text multiple times, and also for some similar
	 * actions like NodeCreated and then just after NodeModified.
	 * -Flags to prevent fusionning in case of copy/paste, and to provoke NodeModifs separation
	 * in case of too heavy NodeModified (e.g. typing 100 lines of text shouldn't be undo'ed in one time)
	 */
	 debugOutput();
}

bool undoRedo::undo(bool kafkaUndo)
{
	QTime t;
	QValueList<NodeModif>::iterator it;
	bool success = true;
	bool updateClosing = qConfig.updateClosingTags;

	t.start();
	kdDebug(24000)<< "************* Begin Undo *****************" << endl;
	if(editorIterator == begin())//the first one is empty
		return false;
	it = (*editorIterator).NodeModifList.fromLast();
	_doc->activateParser(false);
	_doc->activateRepaintView(false);
	qConfig.updateClosingTags = false;
	while(1)
	{
		if(!UndoNodeModif(*it))
		{
			//one undo has failed, trying to recover (hope that the undo has done nothing).
			kdDebug(24000)<< "Undo failed, trying to recover." << endl;
			it++;
			while(it != (*editorIterator).NodeModifList.end() && success)
			{
				success = RedoNodeModif(*it);
				kdDebug(24000) << "NodeModif type :" << (*it).type <<" redoed!" << endl;
				it++;
			}
			return false;
		}
		kdDebug(24000) << "NodeModif type :" << (*it).type <<" undoed!" << endl;
		//coutTree(baseNode, 2);
		if(it == (*editorIterator).NodeModifList.begin())
			break;
		it--;
	}
	_doc->viewCursorIf->setCursorPositionReal((*editorIterator).cursorY, (*editorIterator).cursorX);
	editorIterator--;
	if(quantaApp->view()->writeExists())
	{
		Document* w;
		w = quantaApp->view()->write();
		w->setModified((*editorIterator).isModified);
		if(!(*editorIterator).isModified)
		{
			QIconSet  emptyIcon( UserIcon("empty16x16" ));
			QTabWidget *wTab = quantaApp->view()->writeTab();
			wTab->changeTab( w,  emptyIcon,  wTab->tabLabel(w));
		}
		else
		{
			QIconSet floppyIcon( UserIcon("save_small"));
			QTabWidget *wTab = quantaApp->view()->writeTab();
			wTab->changeTab( w,  floppyIcon,  wTab->tabLabel(w));
		}
	}
	//We need to update the internal pointer of baseNode in the parser.
	parser->setM_node(baseNode);
	qConfig.updateClosingTags = updateClosing;
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
	bool updateClosing = qConfig.updateClosingTags;

	t.start();
	kdDebug(24000)<< "************* Begin Redo *****************" << endl;
	if(editorIterator == fromLast())
		return false;
	editorIterator++;
	_doc->activateParser(false);
	_doc->activateRepaintView(false);
	qConfig.updateClosingTags = false;
	for(it = (*editorIterator).NodeModifList.begin(); it != (*editorIterator).NodeModifList.end(); it++)
	{
		success = RedoNodeModif(*it);
		if(!success)
		{
			kdDebug(24000)<< "Redo failed!" << endl;
			_doc->activateRepaintView(true);
			_doc->activateParser(true);
			return false;
		}
		kdDebug(24000) << "NodeModif type :" << (*it).type <<" redoed!" << endl;
	}
	_doc->viewCursorIf->setCursorPositionReal((*editorIterator).cursorY2, (*editorIterator).cursorX2);
	if(quantaApp->view()->writeExists())
	{
		Document* w;
		w = quantaApp->view()->write();
		w->setModified((*editorIterator).isModified);
		if(!(*editorIterator).isModified)
		{
			QIconSet  emptyIcon( UserIcon("empty16x16" ));
			QTabWidget *wTab = quantaApp->view()->writeTab();
			wTab->changeTab( w,  emptyIcon,  wTab->tabLabel(w));
		}
		else
		{
			QIconSet floppyIcon( UserIcon("save_small"));
			QTabWidget *wTab = quantaApp->view()->writeTab();
			wTab->changeTab( w,  floppyIcon,  wTab->tabLabel(w));
		}
	}
	//We need to update the internal pointer of baseNode in the parser.
	parser->setM_node(baseNode);
	qConfig.updateClosingTags = updateClosing;
	_doc->activateRepaintView(true);
	_doc->activateParser(true);
	kdDebug(24000)<< "************* End Redo *****************" << endl;
	kdDebug(24000) << "undoRedo::redo() : " << t.elapsed() << " ms \n";
	//debugOutput();
	return !(editorIterator == fromLast());
}

bool undoRedo::UndoNodeModif(NodeModif &_nodeModif, bool undoTextModifs, bool generateText)
{
	Node *_node = 0L, *_nodeNext = 0L, *n = 0L, *m = 0L, *newNode = 0L;
	int bLine, bCol, eLine, eCol, bLine2, bCol2, eLine2, eCol2, bLine3, bCol3, eLine3, eCol3, i, j;
	bool b;
	QValueList<int> loc;
	Tag *_tag;
	QString text, totalText;
#ifdef BUILD_KAFKAPART
	WKafkaPart *kafkaInterface = quantaApp->view()->getKafkaInterface();
	KafkaHTMLPart *kafkaPart = quantaApp->view()->getKafkaInterface()->getKafkaPart();
#endif

	if(_nodeModif.type == undoRedo::NodeTreeAdded)
	{
		/** Remove all the text and set baseNode to 0L */
		if(undoTextModifs)
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
				  * will be moved next to the last child of _node->prev TODO:(need to be updated) */
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
		if(undoTextModifs)
		{
			fitsNodesPosition(_nodeNext, bCol - eCol - 1, bLine - eLine);
			_doc->editIf->removeText(bLine, bCol, eLine, eCol + 1);
		}
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
		if(undoTextModifs)
		{
			text = _node->tag->tagStr();
			if(generateText)
			{
				bLine2 = bLine;
				bCol2 = bCol;
				eLine2 = bLine2;
				eCol2 = bCol2;
#ifdef BUILD_KAFKAPART
				if(_node->tag->type == Tag::Text)
					text = kafkaInterface->getEncodedText(text, bLine2, bCol2, eLine2, eCol2);
				else if(_node->tag->type == Tag::XmlTag || _node->tag->type == Tag::XmlTagEnd)
					text = kafkaInterface->generateCodeFromNode(_node, bLine2, bCol2, eLine2, eCol2);
				else
					kdDebug(25001)<< "undoRedo::UndoNodeModif - ERROR can't generate text for type "
						<< _node->tag->type << endl;
#endif
				_node->tag->setStr(text);
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
			_doc->editIf->insertText(bLine, bCol, _node->tag->tagStr());
			_node->tag->beginPos(bLine2, bCol2);
			_node->tag->endPos(eLine2, eCol2);
			fitsNodesPosition(_nodeNext, (eCol2 - bCol2) - (eCol - bCol),
				(eLine2 - bLine2) - (eLine - bLine));
		}
	}
	else if(_nodeModif.type == undoRedo::NodeRemoved ||
		_nodeModif.type == undoRedo::NodeAndChildsRemoved)
	{
		/** Adding the node */
		newNode = _nodeModif.node;
		_nodeModif.node = 0L;
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
		if(newNode->next && ("/" + newNode->tag->name.lower() == newNode->next->tag->name.lower()))
			newNode->next->closesPrevious = true;
		n = newNode;
		while(n)
		{
			//remove any reference to a now deleted QListViewItem
			n->listItem = 0L;
			n = getNextNode(n, b, newNode);
		}
		if(undoTextModifs)
		{
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
				if(generateText)
				{
					//determine the depth of the node
					m = n;
					i = 0;
					while(m->parent)
					{
						i++;
						m = m->parent;
					}
					bLine2 = eLine;
					bCol2 = eCol;
					if(n->tag->type == Tag::Text)
					{
						text = "\n";
						bLine3 = bLine2;
						bCol3 = bCol2;
						bLine3++;
						bCol3 = -1;
						j = i;
						while(j != 0)
						{
							text += "  ";
							bCol3++;
							j--;
						}
#ifdef BUILD_KAFKAPART
						text += kafkaInterface->getEncodedText(text, bLine3, bCol3 + 1, eLine, eCol);
#endif
					}
					else if(n->tag->type == Tag::XmlTag || n->tag->type == Tag::XmlTagEnd)
					{
						//set the empty tag if present
						m = n->previousSibling();
						if(m && m->tag->type == Tag::Empty)
						{
							m->tag->setTagPosition(bLine2, bCol2 - 1, bLine2 + 1, 2*i - 1);
							text = "\n";
							j = i;
							while(j != 0)
							{
								text += "  ";
								j--;
							}
							m->tag->setStr(text);
							bLine2++;
							eLine++;
							bCol2 = 2*i;
							eCol = 2*i;
						}
						else if(m && m->tag->type == Tag::Text)
						{
							m->tag->beginPos(bLine3, bCol3);
							m->tag->endPos(eLine3, eCol3);
							text = m->tag->tagStr();
							text += "\n";
							eLine3++;
							eCol3 = -1;
							j = i;
							while(j != 0)
							{
								text += "  ";
								j--;
								eCol3 += 2;
							}
							m->tag->setStr(text);
							m->tag->setTagPosition(bLine3, bCol3, eLine3, eCol3);
							bLine2++;
							eLine++;
							bCol2 = 2*i;
							eCol = 2*i;
						}
#ifdef BUILD_KAFKAPART
						text = kafkaInterface->generateCodeFromNode(n, bLine2, bCol2, eLine, eCol);
#endif
					}
					n->tag->setStr(text);
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
			/**if(generateText) <== Shouldn't be called
			{
				if(n->tag->type == Tag::Text)
				{
					bCol = eCol + 1;
					bLine = eLine;
					text = n->tag->tagStr();
					text = kafkaInterface->getEncodedText(text, bLine, bCol, eLine, eCol);
				}
				else if(n->tag->type == Tag::XmlTag || n->tag->type == Tag::XmlTagEnd)
				{
					bCol = eCol + 1;
					bLine = eLine;
					text = kafkaInterface->generateCodeFromNode(n, bLine, bCol, eLine, eCol);
				}
				else
				kdDebug(25001) <<"undoRedo::UndoNodeModif - ERROR2 can't generate text for type "
					<< n->tag->type << endl;
				n->tag->setStr(text);
				n->tag->setTagPosition(bLine, bCol, eLine, eCol);
			}
			else*/
				totalText += n->tag->tagStr();
			n = getNextNode(n, b);
		}
		if(undoTextModifs)
			_doc->editIf->insertText(0, 0, totalText);
	}
	else if(_nodeModif.type == undoRedo::NodeMoved ||
		_nodeModif.type == undoRedo::NodeAndChildsMoved)
	{
		/** NOT IMPLEMENTED and i don't think i will need to. */
	}

	return true;
}

bool undoRedo::RedoNodeModif(NodeModif &_nodeModif, bool undoTextModifs, bool generateText)
{
	bool success;
	if(_nodeModif.type == undoRedo::NodeTreeAdded)
		_nodeModif.type = undoRedo::NodeTreeRemoved;
	else if(_nodeModif.type == undoRedo::NodeAndChildsAdded)
		_nodeModif.type = undoRedo::NodeAndChildsRemoved;
	else if(_nodeModif.type == undoRedo::NodeAdded)
		_nodeModif.type = undoRedo::NodeRemoved;
	else if(_nodeModif.type == undoRedo::NodeRemoved)
		_nodeModif.type = undoRedo::NodeAdded;
	else if(_nodeModif.type == undoRedo::NodeAndChildsRemoved)
		_nodeModif.type = undoRedo::NodeAndChildsAdded;
	else if(_nodeModif.type == undoRedo::NodeTreeRemoved)
		_nodeModif.type = undoRedo::NodeTreeAdded;

	success = UndoNodeModif(_nodeModif, undoTextModifs, generateText);

	if(_nodeModif.type == undoRedo::NodeTreeRemoved)
		_nodeModif.type = undoRedo::NodeTreeAdded;
	else if(_nodeModif.type == undoRedo::NodeAndChildsRemoved)
		_nodeModif.type = undoRedo::NodeAndChildsAdded;
	else if(_nodeModif.type == undoRedo::NodeRemoved)
		_nodeModif.type = undoRedo::NodeAdded;
	else if(_nodeModif.type == undoRedo::NodeAdded)
		_nodeModif.type = undoRedo::NodeRemoved;
	else if(_nodeModif.type == undoRedo::NodeAndChildsAdded)
		_nodeModif.type = undoRedo::NodeAndChildsRemoved;
	else if(_nodeModif.type == undoRedo::NodeTreeAdded)
		_nodeModif.type = undoRedo::NodeTreeRemoved;
	return success;
}

#ifdef BUILD_KAFKAPART
bool undoRedo::UndoNodeModifInKafka(NodeModif &_nodeModif)
{
	Node *_node, *n;
	Tag *_tag;
	DOM::Node domNode, domNode2, dn, dm;
	bool goUp;
	WKafkaPart *kafkaInterface = quantaApp->view()->getKafkaInterface();
	KafkaHTMLPart *kafkaPart = quantaApp->view()->getKafkaInterface()->getKafkaPart();

	if(_nodeModif.type == undoRedo::NodeTreeAdded)
	{
		//clear the kafkaPart
		kafkaInterface->disconnectAllDomNodes();
		while(kafkaPart->document().hasChildNodes())
		{
			try{
				kafkaPart->document().removeChild(kafkaPart->document().firstChild());
			} catch(DOM::DOMException e) {kafkaSyncError();}
		}
		//reload the minimum tree
		domNode = kafkaPart->document().createElement("HTML");
		kafkaPart->document().appendChild(domNode);
		_node = new Node(0L);
		_tag = new Tag();
		_tag->name = "HTML";
		_node->tag = _tag;
		kafkaInterface->connectDomNodeToQuantaNode(kafkaPart->document().firstChild(), _node);
		kafkaInterface->html = kafkaPart->document().firstChild();
		domNode = kafkaPart->document().createElement("HEAD");
		kafkaPart->document().firstChild().appendChild(domNode);
		_node = new Node(0L);
		_tag = new Tag();
		_tag->name = "HEAD";
		_node->tag = _tag;
		kafkaInterface->connectDomNodeToQuantaNode(kafkaPart->document().firstChild().firstChild(), _node);
		kafkaInterface->head = kafkaPart->document().firstChild().firstChild();
		domNode = kafkaPart->document().createElement("BODY");
		kafkaPart->document().firstChild().appendChild(domNode);
		_node = new Node(0L);
		_tag = new Tag();
		_tag->name = "BODY";
		_node->tag = _tag;
		kafkaInterface->connectDomNodeToQuantaNode(kafkaPart->document().firstChild().lastChild(), _node);
		kafkaInterface->body = kafkaPart->document().firstChild().lastChild();
	}
	else if(_nodeModif.type == undoRedo::NodeAndChildsAdded ||
		_nodeModif.type == undoRedo::NodeAdded)
	{
		//removing the Kakfa node and moving others nodes.
		_node = getNodeFromLocation(_nodeModif.location);
		if(!_node)
		{
			kdDebug(25001)<< "undoRedo::UndoNodeModifInKafka() - ERROR1" << endl;
			return false;
		}
		if(_node->_rootNode.isNull())
			return true;//no kafka node here, due to an invalid pos.
		domNode = _node->_rootNode;
		domNode2 = _node->_leafNode;
		kafkaInterface->disconnectDomNodeFromQuantaNode(domNode);
		if(_node->tag->type == Tag::XmlTag || _node->tag->type == Tag::Text)
		{
			if(_nodeModif.type == undoRedo::NodeAdded && _node->child)
			{
				n = _node->child;
				while(n)
				{
					if(!n->_rootNode.isNull())
					{
						try
						{
							dn = n->_rootNode.parentNode().removeChild(n->_rootNode);
						} catch(DOM::DOMException e) {kafkaSyncError();}
						try{
							domNode.parentNode().insertBefore(dn, domNode);
						} catch(DOM::DOMException e) {}
					}
					else if(n->tag->type == Tag::XmlTag || n->tag->type == Tag::Text)
						kafkaInterface->buildKafkaNodeFromNode(n, true);
					n = n->next;
				}
				if(domNode.hasChildNodes() && domNode != domNode2)
				{
					//HTML Specific to handle one specific case!!
					kafkaInterface->disconnectDomNodeFromQuantaNode(domNode.firstChild());
					try{
						domNode.removeChild(domNode.firstChild());
					} catch(DOM::DOMException e) {kafkaSyncError();}
				}
			}
			else if(domNode.hasChildNodes())
			{
				dm = domNode.firstChild();
				goUp = false;
				while(!dm.isNull())
				{
					kafkaInterface->disconnectDomNodeFromQuantaNode(dm);
					dm = kafkaPart->getNextNode(dm, goUp, true, true, domNode);
				}
			}
			try{
				domNode.parentNode().removeChild(domNode);
			} catch(DOM::DOMException e) {kafkaSyncError();}
		}
		else if(_node->tag->type == Tag::XmlTagEnd && _node->closesPrevious &&
			!domNode.nextSibling().isNull())
		{
			n = _node->prev;
			if(!n)
			{
				kdDebug(24000)<< "undoRedo::UndoNodeModifInKafka() - ERROR2" << endl;
				return false;
			}
			domNode2 = n->_leafNode;
			if(domNode2.isNull())
				return true;
			if(n->child)
			{
				while(n->child)
				{
					n = n->child;
					while(n->next)
						n = n->next;
				}
				if(n->parent->_leafNode.isNull())
				{
					dm = domNode.nextSibling();
					goUp = false;
					while(!dm.isNull())
					{
						kafkaInterface->disconnectDomNodeFromQuantaNode(dm);
						try{
							dm.parentNode().removeChild(dm);
						} catch(DOM::DOMException e) {kafkaSyncError();}
						dm = kafkaPart->getNextNode(dm, goUp, true, true, domNode.parentNode());
					}
				}
				else
				{
					domNode2 = n->parent->_leafNode;
					while(!domNode.nextSibling().isNull())
					{
						try{
							dn = domNode.parentNode().removeChild(domNode.nextSibling());
						} catch(DOM::DOMException e) {kafkaSyncError();}
						try{
							domNode2.appendChild(dn);
						} catch(DOM::DOMException e) {}
					}
				}
			}
			else
			{
				while(!domNode.nextSibling().isNull())
				{
					try{
						dn = domNode.parentNode().removeChild(domNode.nextSibling());
					} catch(DOM::DOMException e) {kafkaSyncError();}
					try{
						domNode2.appendChild(dn);
					} catch(DOM::DOMException e) {}
				}
			}
		}
	}
	else if(_nodeModif.type == undoRedo::NodeModified)
	{
		//reload the kafka Node
		_node = getNodeFromLocation(_nodeModif.location);
		if(!_node)
		{
			kdDebug(25001)<< "undoRedo::UndoNodeModifInKafka() - ERROR4" << endl;
			return false;
		}
		if(_node->_rootNode.isNull())
			return true;//no kafka node here, due to an invalid pos.
		domNode = _node->_rootNode;
		try{
			domNode.parentNode().removeChild(domNode);
		} catch(DOM::DOMException e) {kafkaSyncError();}
		kafkaInterface->disconnectDomNodeFromQuantaNode(domNode);
		kafkaInterface->buildKafkaNodeFromNode(_node);
	}
	else if(_nodeModif.type == undoRedo::NodeRemoved ||
		_nodeModif.type == undoRedo::NodeAndChildsRemoved)
	{
		//adding a kafka Node and moving the others.
		_node = getNodeFromLocation(_nodeModif.location);
		if(!_node)
		{
			kdDebug(25001)<< "undoRedo::UndoNodeModifInKafka() - ERROR1" << endl;
			return false;
		}
		if(_node->tag->type == Tag::XmlTag || _node->tag->type == Tag::Text)
		{
			kafkaInterface->buildKafkaNodeFromNode(_node, true);
			domNode = _node->_leafNode;
			if(!domNode.isNull() && _node->child)
			{
				n = _node->child;
				while(n)
				{
					if(!n->_rootNode.isNull())
					{
						try{
							dn = n->_rootNode.parentNode().removeChild(n->_rootNode);
						} catch(DOM::DOMException e) {kafkaSyncError();}
						try{
							domNode.appendChild(dn);
						} catch(DOM::DOMException e) {}
					}
					else if(n->tag->type == Tag::XmlTag || n->tag->type == Tag::Text)
						kafkaInterface->buildKafkaNodeFromNode(n, true);
					n = n->next;
				}
			}
		}
		else if(_node->tag->type == Tag::XmlTagEnd && _node->closesPrevious && _node->next)
		{
			n = _node->next;
			while(n)
			{
				if(!n->_rootNode.isNull())
				{
					try{
						dn = n->_rootNode.parentNode().removeChild(n->_rootNode);
					} catch(DOM::DOMException e) {kafkaSyncError();}
					try{
						domNode.parentNode().appendChild(dn);
					} catch(DOM::DOMException e) {}
				}
				else if(n->tag->type == Tag::XmlTag || n->tag->type == Tag::Text)
					kafkaInterface->buildKafkaNodeFromNode(n, true);
				n = n->next;
			}
		}
	}
	else if(_nodeModif.type == undoRedo::NodeTreeRemoved)
	{
		//fill the kafka tree.
		goUp = false;
		_node = baseNode;
		while(_node)
		{
			if(!goUp)
				kafkaInterface->buildKafkaNodeFromNode(_node);
			_node = getNextNode(_node, goUp);
		}
	}

	return true;
}
#endif
#ifdef BUILD_KAFKAPART
void kafkaSyncError()
{
	kdDebug(25001)<< "Kafka Sync ERROR. Reloading kafka." << endl;
	WKafkaPart *kafkaInterface = quantaApp->view()->getKafkaInterface();
	Document *_doc = kafkaInterface->getCurrentDoc();
	kafkaInterface->unloadDocument();
	kafkaInterface->loadDocument(_doc);
}
#endif

#ifdef BUILD_KAFKAPART
bool undoRedo::RedoNodeModifInKafka(NodeModif &_nodeModif)
{
	bool success;
	if(_nodeModif.type == undoRedo::NodeTreeAdded)
		_nodeModif.type = undoRedo::NodeTreeRemoved;
	else if(_nodeModif.type == undoRedo::NodeAndChildsAdded)
		_nodeModif.type = undoRedo::NodeAndChildsRemoved;
	else if(_nodeModif.type == undoRedo::NodeAdded)
		_nodeModif.type = undoRedo::NodeRemoved;
	else if(_nodeModif.type == undoRedo::NodeRemoved)
		_nodeModif.type = undoRedo::NodeAdded;
	else if(_nodeModif.type == undoRedo::NodeAndChildsRemoved)
		_nodeModif.type = undoRedo::NodeAndChildsAdded;
	else if(_nodeModif.type == undoRedo::NodeTreeRemoved)
		_nodeModif.type = undoRedo::NodeTreeAdded;

	success = UndoNodeModifInKafka(_nodeModif);

	if(_nodeModif.type == undoRedo::NodeTreeRemoved)
		_nodeModif.type = undoRedo::NodeTreeAdded;
	else if(_nodeModif.type == undoRedo::NodeAndChildsRemoved)
		_nodeModif.type = undoRedo::NodeAndChildsAdded;
	else if(_nodeModif.type == undoRedo::NodeRemoved)
		_nodeModif.type = undoRedo::NodeAdded;
	else if(_nodeModif.type == undoRedo::NodeAdded)
		_nodeModif.type = undoRedo::NodeRemoved;
	else if(_nodeModif.type == undoRedo::NodeAndChildsAdded)
		_nodeModif.type = undoRedo::NodeAndChildsRemoved;
	else if(_nodeModif.type == undoRedo::NodeTreeAdded)
		_nodeModif.type = undoRedo::NodeTreeRemoved;
	return success;
}
#endif

#ifdef BUILD_KAFKAPART
 bool undoRedo::syncKafkaView()
 {
	kdDebug(25001)<< "undoRedo::syncKafkaView()" << endl;
	QValueList<NodeModifsSet>::iterator it;
	QValueList<NodeModif>::iterator it2;
	bool undoKafkaView = true, success;

	if(kafkaIterator == editorIterator)
		return true;
	it = kafkaIterator;
	while(it != end())
	{
		if(it == editorIterator)
		{
			undoKafkaView = false;
			break;
		}
		it++;
	}

	it = editorIterator;
	if(!undoKafkaView)
	{
		//changes have been made to quanta, syncing the kafka view
		//First undo all the node modifs made after the last update
		//needed to have the right context to update the kafka tree.
		while(it != kafkaIterator)
		{
			it2 = (*it).NodeModifList.fromLast();
			while(it2 != (*it).NodeModifList.end())
			{
				if(!UndoNodeModif((*it2), false))
				{
					kdDebug(25001)<< "undoRedo::syncKafkaView() - ERROR 1" << endl;
					return false;
				}
				kdDebug(25001)<< "undoRedo::syncKafkaView() - Nodes without text undoed!" << endl;
				if(it2 == (*it).NodeModifList.begin())
					break;
				it2--;
			}
			it--;
		}
		//then for each NodeModif, it is redoed, and the kafka Nodes are build/deleted/modified
		while(kafkaIterator != editorIterator)
		{
			kafkaIterator++;
			for (it2 = (*kafkaIterator).NodeModifList.begin(); it2 != (*kafkaIterator).NodeModifList.end(); it2++)
			{
				if((*it2).type == undoRedo::NodeTreeAdded || (*it2).type == undoRedo::NodeAndChildsAdded ||
					(*it2).type == undoRedo::NodeAdded || (*it2).type == undoRedo::NodeModified)
				{
					if(!RedoNodeModif((*it2), false))
					{
						kdDebug(25001)<< "undoRedo::syncKafkaView() - ERROR 2" << endl;
						return false;
					}
					if(!RedoNodeModifInKafka(*it2))
					{
						kdDebug(25001)<< "undoRedo::syncKafkaView() - ERROR 3" << endl;
						return false;
					}
				}
				else
				{
					if(!RedoNodeModifInKafka(*it2))
					{
						kdDebug(25001)<< "undoRedo::syncKafkaView() - ERROR 4" << endl;
						return false;
					}
					if(!RedoNodeModif((*it2), false))
					{
						kdDebug(25001)<< "undoRedo::syncKafkaView() - ERROR 5" << endl;
						return false;
					}
				}
				kdDebug(25001)<< "undoRedo::syncKafkaView() - Nodes without text, and kafka Nodes redoed!" << endl;
			}
		}
	}
	else
	{
		//undo operations have been done in the quanta view
		//First redo all the Node modifs made after the last update.
		//This might be called when an user action occurs after undoing : we must sync before the
		//deletion of part of the undo stack.
		while(it != kafkaIterator)
		{
			it++;
			for(it2 = (*it).NodeModifList.begin(); it2 != (*it).NodeModifList.end(); it2++)
			{
				if(!RedoNodeModif((*it2), false))
				{
					kdDebug(25001)<< "undoRedo::syncKafkaView() - ERROR 6" << endl;
					return false;
				}
				kdDebug(25001)<< "undoRedo::syncKafkaView() - Nodes without text redoed!" << endl;
			}
		}
		//then for each NodeModif, Nodes are undoed, and the kafka Nodes are build/deleted/modified
		while(kafkaIterator != editorIterator)
		{
			it2 = (*kafkaIterator).NodeModifList.fromLast();
			while(it2 != (*kafkaIterator).NodeModifList.end())
			{
				if((*it2).type == undoRedo::NodeTreeAdded || (*it2).type == undoRedo::NodeAndChildsAdded ||
					(*it2).type == undoRedo::NodeAdded || (*it2).type == undoRedo::NodeModified)
				{
					if(!UndoNodeModifInKafka(*it2))
					{
						kdDebug(25001)<< "undoRedo::syncKafkaView() - ERROR 8" << endl;
						return false;
					}
					if(!UndoNodeModif((*it2), false))
					{
						kdDebug(25001)<< "undoRedo::syncKafkaView() - ERROR 7" << endl;
						return false;
					}
				}
				else
				{
					if(!UndoNodeModif((*it2), false))
					{
						kdDebug(25001)<< "undoRedo::syncKafkaView() - ERROR 10" << endl;
						return false;
					}
					if(!UndoNodeModifInKafka(*it2))
					{
						kdDebug(25001)<< "undoRedo::syncKafkaView() - ERROR 9" << endl;
						return false;
					}
				}
				kdDebug(25001)<< "undoRedo::syncKafkaView() - Nodes without text, and kafka Nodes undoed!" << endl;
				if(it2 == (*kafkaIterator).NodeModifList.begin())
					break;
				it2--;
			}
			kafkaIterator--;
		}
	}
	kafkaIterator = editorIterator;
	return true;
 }
 #endif

#ifdef BUILD_KAFKAPART
 bool undoRedo::syncQuantaView()
 {
	kdDebug(25001)<< "undoRedo::syncQuantaView()" << endl;
	QValueList<NodeModifsSet>::iterator it;
	QValueList<NodeModif>::iterator it2;
	bool undoQuantaView = true, success;

	if(kafkaIterator == editorIterator)
		return true;
	it = editorIterator;
	while(it != end())
	{
		if(it == kafkaIterator)
		{
			undoQuantaView = false;
			break;
		}
		it++;
	}

	it = kafkaIterator;
	if(!undoQuantaView)
	{
		//changes have been made to kafka, syncing the quanta view
		//First undo all the node modifs made after the last update
		//needed to have the right context to update the quanta tree.
		while(it != editorIterator)
		{
			it2 = (*it).NodeModifList.fromLast();
			while(it2 != (*it).NodeModifList.end())
			{
				if(!UndoNodeModif((*it2), false))
				{
					kdDebug(25001)<< "undoRedo::syncQuantaView() - ERROR 1" << endl;
					return false;
				}
				kdDebug(25001)<< "undoRedo::syncQuantaView() - Nodes without text undoed!" << endl;
				if(it2 == (*it).NodeModifList.begin())
					break;
				it2--;
			}
			it--;
		}
		//then for each NodeModif, Nodes are redoed, and the tags text is generated and inserted.
		while(editorIterator != kafkaIterator)
		{
			editorIterator++;
			for (it2 = (*editorIterator).NodeModifList.begin(); it2 != (*editorIterator).NodeModifList.end(); it2++)
			{
				if(!RedoNodeModif((*it2), true, true))
				{
					kdDebug(25001)<< "undoRedo::syncQuantaView() - ERROR 2" << endl;
					return false;
				}
				kdDebug(25001)<< "undoRedo::syncQuantaView() - Nodes and text redoed!" << endl;
			}
		}
	}
	else
	{
		//undo operations have been done in the kafka view
		//First redo all the Node modifs made after the last update.
		//This might be called when an user action occurs after undoing : we must sync before the
		//deletion of part of the undo stack.
		while(it != editorIterator)
		{
			it++;
			for(it2 = (*it).NodeModifList.begin(); it2 != (*it).NodeModifList.end(); it2++)
			{
				if(!RedoNodeModif((*it2), false))
				{
					kdDebug(25001)<< "undoRedo::syncQuantaView() - ERROR 3" << endl;
					return false;
				}
				kdDebug(25001)<< "undoRedo::syncQuantaView() - Nodes without text redoed!" << endl;
			}
		}
		//then for each NodeModif, Nodes are undoed, and the tags text is generated and inserted.
		while(editorIterator != kafkaIterator)
		{
			it2 = (*editorIterator).NodeModifList.fromLast();
			while(it2 != (*editorIterator).NodeModifList.end())
			{
				if(!UndoNodeModif((*it2), true, true))
				{
					kdDebug(25001)<< "undoRedo::syncQuantaView() - ERROR 4" << endl;
					return false;
				}
				kdDebug(25001)<< "undoRedo::syncQuantaView() - Nodes and text undoed!" << endl;
				if(it2 == (*editorIterator).NodeModifList.begin())
					break;
				it2--;
			}
			editorIterator--;
		}
	}
	editorIterator = kafkaIterator;
	return true;
 }
#endif

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
	Node *m = 0L;
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

void undoRedo::fileSaved()
{
	QValueList<NodeModifsSet>::iterator it = editorIterator;
	(*editorIterator).isModified = false;
	//seting isModified = true to all others
	while(it != begin())
	{
		it--;
		(*it).isModified = true;
	}
	it = editorIterator;
	it++;
	while(it != end())
	{
		(*it).isModified = true;
		it++;
	}
}

#ifdef BUILD_KAFKAPART
void undoRedo::kafkaLoaded()
{
	kafkaIterator = editorIterator;
}
#endif

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
