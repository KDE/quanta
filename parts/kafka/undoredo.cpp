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
#include <dom/dom_node.h>
#include <dom/dom_exception.h>

#include "../../document.h"
#include "../../quanta.h"
#include "../../quantaview.h"
#include "../../quantacommon.h"
#include "../../parser/node.h"
#include "../../parser/tag.h"
#include "../../resource.h"
#include "wkafkapart.h"
#include "kafkacommon.h"
#include "kafkaresource.h"

#include "undoredo.h"

NodeModif::NodeModif()
{
	m_type = -1;
	m_node = 0L;
	m_tag = 0L;
	m_childrenMovedUp = 0;
	m_neighboursMovedDown = 0;
}

NodeModif::~NodeModif()
{
	if(m_node)
	{
		m_node->parent = 0L;
		m_node->next = 0L;
		m_node->prev = 0L;
		if(m_type == NodeRemoved)
			m_node->child = 0L;
		delete m_node;
	}
	if(m_tag)
		delete m_tag;
}

void NodeModif::setNode(Node *node)
{
	//TEMPORARY to avoid the Node::~Node bug which seems to come from the fact
	//the node is deleted a while after it is removed and thus some pointer are invalid
	//cf Node::~Node
	m_node = node;
	if(m_node)
	{
		m_node->parent = 0L;
		m_node->next = 0L;
		m_node->prev = 0L;
		if(m_type == NodeRemoved)
			m_node->child = 0L;
		delete m_node;
		m_node = 0L;
	}
}

void NodeModif::setTag(Tag *tag)
{
	//TEMPORARY cf setNode
	m_tag = tag;
	if(m_tag)
	{
		delete m_tag;
		m_tag = 0L;
	}
}

NodeModifsSet::NodeModifsSet()
{
}

NodeModifsSet::~NodeModifsSet()
{
	m_nodeModifList.setAutoDelete(true);
	m_nodeModifList.clear();
}

undoRedo::undoRedo(Document *doc)
	:documentIterator(m_undoList),
	sourceIterator(m_undoList),
	kafkaIterator(m_undoList), m_doc(doc)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "undoRedo::undoRedo() - *doc" << endl;
#endif
	//TODO:add it to the config
	//temp: set to 3 to avoid too much memory consumption until the new undo/redo system works.
	m_listLimit = 3;
	m_merging = false;
	m_mergeNext = false;

	//add a first empty NodeModifs struct
	NodeModifsSet *modifs = new NodeModifsSet();
	//don't call addNewModifsSet(modifs); otherwise it will delete it as it is empty
	m_undoList.append(modifs);

	documentIterator.toFirst();
	sourceIterator.toFirst();
	kafkaIterator.toFirst();
}

undoRedo::~undoRedo()
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "undoRedo::~undoRedo()" << endl;
#endif
}

void undoRedo::addNewModifsSet(NodeModifsSet *modifs, int modifLocation)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "undoRedo::addNewModifsSet() - NodeModifsSet type: " << modifLocation << endl;
#endif
	QValueList<NodeModif>::iterator it2;
	NodeModifsSet *NMSet;
	QValueList<int> loc;
	int curFocus;

	// For the 3.2.x releases
	/**if(m_mergeNext)
	{
		//we merge modifs with the previous NodeModifsSet
		for(it2 = modifs.NodeModifList.begin(); it2 != modifs.NodeModifList.end(); it2++)
		{
			(*fromLast()).NodeModifList.append(*it2);
		}
		modifs.NodeModifList.clear();
		m_mergeNext = false;
#ifdef HEAVY_DEBUG
		debugOutput();
#endif
		return;
	}

	if(m_dontAddModifSet >= 0)
		m_dontAddModifSet--;
	if(m_dontAddModifSet == 0)
	{
		//we don't add the modifsset
		m_dontAddModifSet = -1;
#ifdef HEAVY_DEBUG
		debugOutput();\
#endif
		return;
	}*/


	// For the 3.2.x releases
	/**if((!m_merging || beginToMerge) && !noMerge)
	{*/
		//inserting the NodeModifsSet
		/**if(!empty())
		{
			// if we are in the middle of the stack, we delete the end of the stack
			if(fromLast() != end() && sourceIterator != fromLast())
			{
				it = sourceIterator;
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
		}*/
		m_undoList.append(modifs);
#ifdef HEAVY_DEBUG
		debugOutput();
#endif
		while(m_undoList.count() > (unsigned)m_listLimit)
		{
			// FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME
			//FIXME: This is to prevent the list to be infinite, change when undoRedo is finished!! FIXME
			if(kafkaIterator.atFirst())
			{
				kafkaIterator = sourceIterator;
				--kafkaIterator;
			}
			else if(sourceIterator.atFirst())
			{
				sourceIterator = kafkaIterator;
				--sourceIterator;
			}
			//END FIXME
			NMSet = m_undoList.getFirst();
			m_undoList.remove(NMSet);
			delete NMSet;
		}
		if(modifLocation == undoRedo::SourceModif)
		{
			sourceIterator.toLast();
			//The node Tree is ALWAYS in sync
			documentIterator.toLast();
		}
		else if(modifLocation == undoRedo::KafkaModif)
		{
			kafkaIterator.toLast();
			//The node Tree is ALWAYS in sync
			documentIterator.toLast();
		}
		else if(modifLocation == undoRedo::NodeTreeModif)
		{
			documentIterator.toLast();
			curFocus = quantaApp->view()->hadLastFocus();
			if(curFocus == QuantaView::quantaFocus)
				reloadQuantaEditor();
			else
				reloadKafkaEditor();
		}
#ifdef HEAVY_DEBUG
		kdDebug(25001)<<"-------------------------------------------------------------------------------"<< endl;
		debugOutput();
#endif
	/**}*/

	/** A lot more to do:
	 * -NodeModifs fusionning in case of typing text multiple times, and also for some similar
	 * actions like NodeCreated and then just after NodeModified.
	 * -Flags to prevent fusionning in case of copy/paste, and to provoke NodeModifs separation
	 * in case of too heavy NodeModified (e.g. typing 100 lines of text shouldn't be undo'ed in one time)
	 */
#ifdef HEAVY_DEBUG
	 //debugOutput();
#endif
}

bool undoRedo::undo(bool /**kafkaUndo*/)
{
//TODO:BIG CHANGES here to do for 3.2.x, x != 0
/**	QValueList<NodeModif>::iterator it;
	bool success = true;
	bool updateClosing = qConfig.updateClosingTags;

#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "undoRedo::undo() - bool: " << kafkaUndo << endl;
	QTime t;
	t.start();
#endif
	if(sourceIterator.atBegin())//the first one is empty
		return false;
	it = (*sourceIterator).NodeModifList.fromLast();
	m_doc->activateParser(false);
	m_doc->activateRepaintView(false);
	qConfig.updateClosingTags = false;
	while(1)
	{
		if(!UndoNodeModif(*it))
		{
			//one undo has failed, trying to recover (hope that the undo has done nothing).
			kdDebug(25001)<< "Undo failed, trying to recover." << endl;
			it++;
			while(it != (*sourceIterator).NodeModifList.end() && success)
			{
				success = RedoNodeModif(*it);
#ifdef LIGHT_DEBUG
				kdDebug(25001) << "NodeModif type :" << (*it).type <<" redoed!" << endl;
#endif
				it++;
			}
			return false;
		}
#ifdef LIGHT_DEBUG
		kdDebug(25001) << "NodeModif type :" << (*it).type <<" undoed!" << endl;
#endif
#ifdef HEAVY_DEBUG
		//kafkaCommon::coutTree(baseNode, 2);
#endif
		if(it == (*sourceIterator).NodeModifList.begin())
			break;
		it--;
	}
	m_doc->viewCursorIf->setCursorPositionReal((*sourceIterator).cursorY, (*sourceIterator).cursorX);
	sourceIterator--;
	if(quantaApp->view()->writeExists())
	{
		Document* w;
		w = quantaApp->view()->write();
		w->setModified((*sourceIterator).isModified);
		if(!(*sourceIterator).isModified)
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
	parser->setRootNode(baseNode);
	qConfig.updateClosingTags = updateClosing;
	m_doc->activateRepaintView(true);
	m_doc->activateParser(true);
#ifdef LIGHT_DEBUG
	kdDebug(25001) << "undoRedo::undo() : " << t.elapsed() << " ms" << endl;
#endif
#ifdef HEAVY_DEBUG
	//debugOutput();
#endif
	return !(sourceIterator == begin());*/
	return true;
}

bool undoRedo::redo(bool /**kafkaUndo*/)
{
	/**QValueList<NodeModif>::iterator it;
	bool success;
	bool updateClosing = qConfig.updateClosingTags;

#ifdef LIGHT_DEBUG
	QTime t;
	t.start();
	kdDebug(25001)<< "undoRedo::redo() - bool: " << kafkaUndo << endl;
#endif
	if(sourceIterator == fromLast())
		return false;
	sourceIterator++;
	m_doc->activateParser(false);
	m_doc->activateRepaintView(false);
	qConfig.updateClosingTags = false;
	for(it = (*sourceIterator).NodeModifList.begin(); it != (*sourceIterator).NodeModifList.end(); it++)
	{
		success = RedoNodeModif(*it);
		if(!success)
		{
			kdDebug(25001)<< "Redo failed!" << endl;
			m_doc->activateRepaintView(true);
			m_doc->activateParser(true);
			return false;
		}
#ifdef LIGHT_DEBUG
		kdDebug(25001) << "NodeModif type :" << (*it).type <<" redoed!" << endl;
#endif
	}
	m_doc->viewCursorIf->setCursorPositionReal((*sourceIterator).cursorY2, (*sourceIterator).cursorX2);
	if(quantaApp->view()->writeExists())
	{
		Document* w;
		w = quantaApp->view()->write();
		w->setModified((*sourceIterator).isModified);
		if(!(*sourceIterator).isModified)
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
	parser->setRootNode(baseNode);
	qConfig.updateClosingTags = updateClosing;
	m_doc->activateRepaintView(true);
	m_doc->activateParser(true);
#ifdef LIGHT_DEBUG
	kdDebug(25001) << "undoRedo::redo() : " << t.elapsed() << " ms" << endl;
#endif
#ifdef HEAVY_DEBUG
	//debugOutput();
#endif
	return !(sourceIterator == fromLast());*/
	return true;
}

bool undoRedo::UndoNodeModif(NodeModif &/**_nodeModif*/, bool /**undoTextModifs*/,
	bool /**generateText*/)
{
	/*Node *_node = 0L, *_nodeNext = 0L, *n = 0L, *m = 0L, *newNode = 0L;
	int bLine, bCol, eLine, eCol, bLine2, bCol2, eLine2, eCol2, bLine3, bCol3, eLine3, eCol3, i, j;
	bool b;
	QValueList<int> loc;
	Tag *_tag;
	QString text, totalText;
	KafkaDocument *kafkaInterface = quantaApp->view()->getKafkaInterface();

	if(_nodeModif.type == undoRedo::NodeTreeAdded)
	{
		// Remove all the text and set baseNode to 0L
		if(undoTextModifs)
			m_doc->editIf->removeText(0, 0, m_doc->editIf->numLines() - 1,
				m_doc->editIf->lineLength(m_doc->editIf->numLines() - 1));
		_nodeModif.node = baseNode;
		baseNode = 0L;
	}
	else if(_nodeModif.type == undoRedo::NodeAndChildsAdded ||
		_nodeModif.type == undoRedo::NodeAdded)
	{
		// Removing the node
		_node = kafkaCommon::getNodeFromLocation(_nodeModif.location);
		if(!_node)
		{
			kdDebug(25001)<< "undoRedo::UndoNodeModif() - ERROR1" << endl;
			m_doc->activateParser(true);
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
			_nodeNext = kafkaCommon::getNextNode(_node, b);
			if(_node->prev)
				_node->prev->next = _node->next;
			if(_node->next)
				_node->next->prev = _node->prev;
			if(_node->parent && _node->parent->child == _node)
				_node->parent->child = _node->next;
			if(_node->closesPrevious && _node->next)
			{
				// If _node close the previous Node, it means that all the node next to _node
				// will be moved next to the last child of _node->prev TODO:(need to be updated)
				n = _node->prev;
				if(!n)
				{
					kdDebug(25001)<< "undoRedo::UndoNodeModif() - ERROR2" << endl;
					m_doc->activateParser(true);
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
			_nodeNext = kafkaCommon::getNextNode(_node, b);
			_node->tag->endPos(eLine, eCol);
			if(_node->parent && _node->parent->child == _node)
				_node->parent->child = _node->child;
			n = _node->child;//not null
			i = 0;
			while(n)
			{
				// _node is deleted, all its child go up
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
			kafkaCommon::fitsNodesPosition(_nodeNext, bCol - eCol - 1, bLine - eLine);
			m_doc->editIf->removeText(bLine, bCol, eLine, eCol + 1);
		}
	}
	else if(_nodeModif.type == undoRedo::NodeModified)
	{
		// Simply replacing the tag(node->tag) of the node by the old tag.
		_node = kafkaCommon::getNodeFromLocation(_nodeModif.location);
		if(!_node)
		{
			kdDebug(25001)<< "undoRedo::UndoNodeModif() - ERROR3" << endl;
			m_doc->activateParser(true);
			return false;
		}
		_tag = _nodeModif.tag;
		_nodeModif.tag = _node->tag;
		b = false;
		_nodeNext = kafkaCommon::getNextNode(_node, b);
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
				if(_node->tag->type == Tag::Text)
					text = kafkaInterface->getEncodedText(text, bLine2, bCol2, eLine2, eCol2);
				else if(_node->tag->type == Tag::XmlTag || _node->tag->type == Tag::XmlTagEnd)
					text = kafkaInterface->generateCodeFromNode(_node, bLine2, bCol2, eLine2, eCol2);
				else
					kdDebug(25001)<< "undoRedo::UndoNodeModif - ERROR can't generate text for type "
						<< _node->tag->type << endl;
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
			m_doc->editIf->removeText(bLine, bCol, eLine, eCol + 1);
			m_doc->editIf->insertText(bLine, bCol, _node->tag->tagStr());
			_node->tag->beginPos(bLine2, bCol2);
			_node->tag->endPos(eLine2, eCol2);
			kafkaCommon::fitsNodesPosition(_nodeNext, (eCol2 - bCol2) - (eCol - bCol),
				(eLine2 - bLine2) - (eLine - bLine));
		}
	}
	else if(_nodeModif.type == undoRedo::NodeRemoved ||
		_nodeModif.type == undoRedo::NodeAndChildsRemoved)
	{
		// Adding the node
		newNode = _nodeModif.node;
		_nodeModif.node = 0L;
		_node = kafkaCommon::getNodeFromLocation(_nodeModif.location);
		if(!_node)
		{
			// No node at this location, getting the parent Node and appending newNode after
			// the last child of the parent.
			loc = _nodeModif.location;
			loc.remove(loc.fromLast());
			if(loc.empty())
			{
				// No parent, adding it on top of the tree.
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
				_node = kafkaCommon::getNodeFromLocation(loc);
				if(!_node)
				{
					kdDebug(25001)<< "undoRedo::UndoNodeModif() - ERROR4" << endl;
					m_doc->activateParser(true);
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
				// Moving nodes from the last child of newNode->prev next to the right of
				// newNode.
				n = newNode->prev;
				if(!n)
				{
					kdDebug(25001)<< "undoRedo::UndoNodeModif() - ERROR5" << endl;
					m_doc->activateParser(true);
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
			_nodeNext = kafkaCommon::getNextNode(newNode, b);
		}
		else
		{
			// A node is already here. Moving it to the right and adding newNode here.
			m = _node;
			if(_node->parent && _node->parent->child == _node)
				_node->parent->child = newNode;
			newNode->parent = _node->parent;
			if(_node->prev)
				_node->prev->next = newNode;
			newNode->prev = _node->prev;
			if(_nodeModif.type == undoRedo::NodeRemoved && _nodeModif.childsNumber != 0)
			{
				// Some Nodes at the right of newNode will be childs of newNode.
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
						kdDebug(25001)<< "undoRedo::UndoNodeModif() - ERROR6" << endl;
						m_doc->activateParser(true);
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
		if(newNode->next && QuantaCommon::closesTag(newNode->tag,  newNode->next->tag))
			newNode->next->closesPrevious = true;
		n = newNode;
		while(n)
		{
			//remove any reference to a now deleted QListViewItem
			n->listItem = 0L;
			n = kafkaCommon::getNextNode(n, b, newNode);
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
						text += kafkaInterface->getEncodedText(text, bLine3, bCol3 + 1, eLine, eCol);
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
						text = kafkaInterface->generateCodeFromNode(n, bLine2, bCol2, eLine, eCol);
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
				n = kafkaCommon::getNextNode(n, b, newNode);
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
			m_doc->editIf->insertText(bLine, bCol, totalText);
			kafkaCommon::fitsNodesPosition(_nodeNext, eCol - bCol + 1, eLine - bLine);
		}
	}
	else if(_nodeModif.type == undoRedo::NodeTreeRemoved)
	{
		// Appending the tree in the editor and putting its adress in baseNode.
		baseNode = _nodeModif.node;
		_nodeModif.node = 0L;
		n = baseNode;
		b = false;
		while(n)
		{
			//remove any reference to a now deleted QListViewItem
			n->listItem = 0L;
			totalText += n->tag->tagStr();
			n = kafkaCommon::getNextNode(n, b);
		}
		if(undoTextModifs)
			m_doc->editIf->insertText(0, 0, totalText);
	}
	else if(_nodeModif.type == undoRedo::NodeMoved ||
		_nodeModif.type == undoRedo::NodeAndChildsMoved)
	{
		// NOT IMPLEMENTED and i don't think i will need to.
	}
*/
	return true;
}

bool undoRedo::RedoNodeModif(NodeModif &/**_nodeModif*/, bool /**undoTextModifs*/,
	bool /**generateText*/)
{
/**	bool success;
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
	return success;*/
	return true;
}

bool undoRedo::UndoNodeModifInKafka(NodeModif &/**_nodeModif*/)
{
	/**Node *_node, *n;
	Tag *_tag;
	DOM::Node domNode, domNode2, dn, dm;
	bool goUp;
	KafkaDocument *kafkaInterface = quantaApp->view()->getKafkaInterface();
	KafkaWidget *kafkaPart = quantaApp->view()->getKafkaInterface()->getKafkaWidget();

	if(_nodeModif.type == undoRedo::NodeTreeAdded)
	{
		//clear the kafkaPart
		kafkaInterface->disconnectAllDomNodes();
		while(kafkaPart->document().hasChildNodes())
		{
			//try{
				kafkaPart->document().removeChild(kafkaPart->document().firstChild());
			//} catch(DOM::DOMException e) {kafkaSyncError();}
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
		_node = kafkaCommon::getNodeFromLocation(_nodeModif.location);
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
						//try
						//{
							dn = n->_rootNode.parentNode().removeChild(n->_rootNode);
						//} catch(DOM::DOMException e) {kafkaSyncError();}
						//try{
							domNode.parentNode().insertBefore(dn, domNode);
						//} catch(DOM::DOMException e) {}
					}
					else if(n->tag->type == Tag::XmlTag || n->tag->type == Tag::Text)
						kafkaInterface->buildKafkaNodeFromNode(n, true);
					n = n->next;
				}
				if(domNode.hasChildNodes() && domNode != domNode2)
				{
					//HTML Specific to handle one specific case!!
					kafkaInterface->disconnectDomNodeFromQuantaNode(domNode.firstChild());
					//try{
						domNode.removeChild(domNode.firstChild());
					//} catch(DOM::DOMException e) {kafkaSyncError();}
				}
			}
			else if(domNode.hasChildNodes())
			{
				dm = domNode.firstChild();
				goUp = false;
				while(!dm.isNull())
				{
					kafkaInterface->disconnectDomNodeFromQuantaNode(dm);
					dm = kafkaCommon::getNextDomNode(dm, goUp, false, domNode);
				}
			}
			//try{
				domNode.parentNode().removeChild(domNode);
			//} catch(DOM::DOMException e) {kafkaSyncError();}
		}
		else if(_node->tag->type == Tag::XmlTagEnd && _node->closesPrevious &&
			!domNode.nextSibling().isNull())
		{
			n = _node->prev;
			if(!n)
			{
				kdDebug(25001)<< "undoRedo::UndoNodeModifInKafka() - ERROR2" << endl;
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
						//try{
							dm.parentNode().removeChild(dm);
						//} catch(DOM::DOMException e) {kafkaSyncError();}
						dm = kafkaCommon::getNextDomNode(dm, goUp, false, domNode.parentNode());
					}
				}
				else
				{
					domNode2 = n->parent->_leafNode;
					while(!domNode.nextSibling().isNull())
					{
						//try{
							dn = domNode.parentNode().removeChild(domNode.nextSibling());
						//} catch(DOM::DOMException e) {kafkaSyncError();}
						//try{
							domNode2.appendChild(dn);
						//} catch(DOM::DOMException e) {}
					}
				}
			}
			else
			{
				while(!domNode.nextSibling().isNull())
				{
					//try{
						dn = domNode.parentNode().removeChild(domNode.nextSibling());
					//} catch(DOM::DOMException e) {kafkaSyncError();}
					//try{
						domNode2.appendChild(dn);
					//} catch(DOM::DOMException e) {}
				}
			}
		}
	}
	else if(_nodeModif.type == undoRedo::NodeModified)
	{
		//reload the kafka Node
		_node = kafkaCommon::getNodeFromLocation(_nodeModif.location);
		if(!_node)
		{
			kdDebug(25001)<< "undoRedo::UndoNodeModifInKafka() - ERROR4" << endl;
			return false;
		}
		if(_node->_rootNode.isNull())
			return true;//no kafka node here, due to an invalid pos.
		domNode = _node->_rootNode;
		//try{
			domNode.parentNode().removeChild(domNode);
		//} catch(DOM::DOMException e) {kafkaSyncError();}
		kafkaInterface->disconnectDomNodeFromQuantaNode(domNode);
		kafkaInterface->buildKafkaNodeFromNode(_node);
	}
	else if(_nodeModif.type == undoRedo::NodeRemoved ||
		_nodeModif.type == undoRedo::NodeAndChildsRemoved)
	{
		//adding a kafka Node and moving the others.
		_node = kafkaCommon::getNodeFromLocation(_nodeModif.location);
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
						//try{
							dn = n->_rootNode.parentNode().removeChild(n->_rootNode);
						//} catch(DOM::DOMException e) {kafkaSyncError();}
						//try{
							domNode.appendChild(dn);
						//} catch(DOM::DOMException e) {}
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
					//try{
						dn = n->_rootNode.parentNode().removeChild(n->_rootNode);
					//} catch(DOM::DOMException e) {kafkaSyncError();}
					//try{
						domNode.parentNode().appendChild(dn);
					//} catch(DOM::DOMException e) {}
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
			_node = kafkaCommon::getNextNode(_node, goUp);
		}
	}
*/
	return true;
}

void undoRedo::reloadKafkaEditor(bool force, bool syncKafkaCursor)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "undoRedo::reloadKafkaEditor()" << endl;
#endif

	if(kafkaIterator == documentIterator && !force)
	{
		if(syncKafkaCursor)
			syncKafkaCursorAndSelection();
		return;
	}

	kafkaIterator = documentIterator;
	KafkaDocument *kafkaInterface = quantaApp->view()->getKafkaInterface();

	Document *m_doc = kafkaInterface->getCurrentDoc();
	kafkaInterface->unloadDocument();
	kafkaInterface->loadDocument(m_doc);

	if(syncKafkaCursor)
		syncKafkaCursorAndSelection();
}

void undoRedo::reloadQuantaEditor(bool force, bool syncQuantaCursor)
{
	QString text, allText;
	Node *node = baseNode;
	int bCol, bLine, eCol, eLine, bCol2, bLine2;
	KafkaDocument *kafkaInterface = quantaApp->view()->getKafkaInterface();
	bool updateClosing, goUp;

#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "undoRedo::reloadQuantaEditor()" << endl;
#endif

	if(documentIterator == sourceIterator && !force)
	{
		if(syncQuantaCursor)
			syncQuantaCursorAndSelection();
		return;
	}

	if(m_doc->editIfExt)
		m_doc->editIfExt->editBegin();
	sourceIterator = documentIterator;

	updateClosing = qConfig.updateClosingTags;
        m_doc->activateParser(false);
	m_doc->activateRepaintView(false);
	qConfig.updateClosingTags = false;

	bLine = 0;
	bCol = 0;
	while(node)
	{
		if(!node->tag->cleanStrBuilt)
		{
			//FIXME FIXME FIXME KafkaDocument::translateKafkaIntoNodeCursorPosition() set the
			//clean string but does not touch the position!!! FIXME FIXME FIXME
			node->tag->setStr(kafkaInterface->generateCodeFromNode(node, bLine, bCol, eLine, eCol));
			node->tag->setTagPosition(bLine, bCol, eLine, eCol);
			//kdDebug(25001)<< "POS1 " << bLine <<  " " <<  bCol << " " << eLine << " " << eCol << endl;
			goUp = false;
			kafkaCommon::fitIndentationNodes(node, kafkaCommon::getNextNodeNE(node, goUp));
			kafkaCommon::applyIndentation(node, 2, 0);
			//int a, b, c, d;
			//_node->tag->beginPos(a,b);
			//_node->tag->endPos(c,d);
			//kdDebug(25001)<< "POS2 " << a <<  " " <<  b << " " << c << " " << d << endl;
			node->tag->cleanStrBuilt = true;
		}
		//_node->tag->beginPos(bLine, bCol);
		//i can't stop redraw events of Kate!
		//m_doc->editIf->insertText(bLine, bCol, _node->tag->tagStr());
		//allText += _node->tag->tagStr();
		node->tag->endPos(bLine, bCol);
		bCol++;
		node = node->nextSibling();
	}

	node = baseNode;
	goUp = false;
	while(node)
	{
		//kdDebug(25001)<< "CurNode : " << _node->tag->name << " - " << _node->tag->tagStr() << endl;
		if(node->parent)
		{
			node->parent->tag->beginPos(bLine, bCol);
			node->parent->tag->endPos(eLine, eCol);
		}
		node->tag->beginPos(bLine2, bCol2);

		//if we are in a Script inside a tag e.g. <a href="<? PHP stuff here ?>">, skip it
		if(node->tag->type == Tag::ScriptTag && node->parent &&
			QuantaCommon::isBetween(bLine2, bCol2, bLine, bCol, eLine,eCol) == 0)
		{
			goUp = true;

			//if we found the closing script tag, skip it too
			if(node->next && node->next->tag->type == Tag::XmlTagEnd)
				node = node->next;
		}
		else
			allText += node->tag->tagStr();
		node = kafkaCommon::getNextNode(node, goUp);
	}

	//temp
	m_doc->editIf->removeText(0, 0, m_doc->editIf->numLines() - 1,
		m_doc->editIf->lineLength(m_doc->editIf->numLines() - 1));
	m_doc->editIf->insertText(0, 0, allText);
	//m_doc->editIf->setText(allText);
	if(m_doc->editIfExt)
		m_doc->editIfExt->editEnd();
	if(syncQuantaCursor)
		syncQuantaCursorAndSelection();

	qConfig.updateClosingTags = updateClosing;
	m_doc->activateRepaintView(true);
	m_doc->activateParser(true);
}

bool undoRedo::RedoNodeModifInKafka(NodeModif &/**_nodeModif*/)
{
	/**bool success;
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
	return success;*/
	return true;
}

bool undoRedo::syncKafkaView()
 {
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "undoRedo::syncKafkaView()" << endl;
#endif
/**	QValueList<NodeModifsSet>::iterator it;
	QValueList<NodeModif>::iterator it2;
	bool undoKafkaView = true;

	if(kafkaIterator == sourceIterator)
		return true;
	it = kafkaIterator;
	while(it != end())
	{
		if(it == sourceIterator)
		{
			undoKafkaView = false;
			break;
		}
		it++;
	}

	it = sourceIterator;
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
#ifdef LIGHT_DEBUG
				kdDebug(25001)<< "undoRedo::syncKafkaView() - Nodes without text undoed!" << endl;
#endif
				if(it2 == (*it).NodeModifList.begin())
					break;
				it2--;
			}
			it--;
		}
		//then for each NodeModif, it is redoed, and the kafka Nodes are build/deleted/modified
		while(kafkaIterator != sourceIterator)
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
#ifdef LIGHT_DEBUG
				kdDebug(25001)<< "undoRedo::syncKafkaView() - Nodes without text, and kafka Nodes redoed!" << endl;
#endif
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
#ifdef LIGHT_DEBUG
				kdDebug(25001)<< "undoRedo::syncKafkaView() - Nodes without text redoed!" << endl;
#endif
			}
		}
		//then for each NodeModif, Nodes are undoed, and the kafka Nodes are build/deleted/modified
		while(kafkaIterator != sourceIterator)
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
#ifdef LIGHT_DEBUG
				kdDebug(25001)<< "undoRedo::syncKafkaView() - Nodes without text, and kafka Nodes undoed!" << endl;
#endif
				if(it2 == (*kafkaIterator).NodeModifList.begin())
					break;
				it2--;
			}
			kafkaIterator--;
		}
	}
	kafkaIterator = sourceIterator;*/
	return true;
 }

bool undoRedo::syncQuantaView()
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "undoRedo::syncQuantaView()" << endl;
#endif
	/**QValueList<NodeModifsSet>::iterator it;
	QValueList<NodeModif>::iterator it2;
	bool undoQuantaView = true;

	if(kafkaIterator == sourceIterator)
		return true;
	it = sourceIterator;
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
		while(it != sourceIterator)
		{
			it2 = (*it).NodeModifList.fromLast();
			while(it2 != (*it).NodeModifList.end())
			{
				if(!UndoNodeModif((*it2), false))
				{
					kdDebug(25001)<< "undoRedo::syncQuantaView() - ERROR 1" << endl;
					return false;
				}
#ifdef LIGHT_DEBUG
				kdDebug(25001)<< "undoRedo::syncQuantaView() - Nodes without text undoed!" << endl;
#endif
				if(it2 == (*it).NodeModifList.begin())
					break;
				it2--;
			}
			it--;
		}
		//then for each NodeModif, Nodes are redoed, and the tags text is generated and inserted.
		while(sourceIterator != kafkaIterator)
		{
			sourceIterator++;
			for (it2 = (*sourceIterator).NodeModifList.begin(); it2 != (*sourceIterator).NodeModifList.end(); it2++)
			{
				if(!RedoNodeModif((*it2), true, true))
				{
					kdDebug(25001)<< "undoRedo::syncQuantaView() - ERROR 2" << endl;
					return false;
				}
#ifdef LIGHT_DEBUG
				kdDebug(25001)<< "undoRedo::syncQuantaView() - Nodes and text redoed!" << endl;
#endif
			}
		}
	}
	else
	{
		//undo operations have been done in the kafka view
		//First redo all the Node modifs made after the last update.
		//This might be called when an user action occurs after undoing : we must sync before the
		//deletion of part of the undo stack.
		while(it != sourceIterator)
		{
			it++;
			for(it2 = (*it).NodeModifList.begin(); it2 != (*it).NodeModifList.end(); it2++)
			{
				if(!RedoNodeModif((*it2), false))
				{
					kdDebug(25001)<< "undoRedo::syncQuantaView() - ERROR 3" << endl;
					return false;
				}
#ifdef LIGHT_DEBUG
				kdDebug(25001)<< "undoRedo::syncQuantaView() - Nodes without text redoed!" << endl;
#endif
			}
		}
		//then for each NodeModif, Nodes are undoed, and the tags text is generated and inserted.
		while(sourceIterator != kafkaIterator)
		{
			it2 = (*sourceIterator).NodeModifList.fromLast();
			while(it2 != (*sourceIterator).NodeModifList.end())
			{
				if(!UndoNodeModif((*it2), true, true))
				{
					kdDebug(25001)<< "undoRedo::syncQuantaView() - ERROR 4" << endl;
					return false;
				}
#ifdef LIGHT_DEBUG
				kdDebug(25001)<< "undoRedo::syncQuantaView() - Nodes and text undoed!" << endl;
#endif
				if(it2 == (*sourceIterator).NodeModifList.begin())
					break;
				it2--;
			}
			sourceIterator--;
		}
	}
	sourceIterator = kafkaIterator;*/
	return true;
 }

void undoRedo::syncKafkaCursorAndSelection()
{
	KafkaWidget *kafkaPart = quantaApp->view()->getKafkaInterface()->getKafkaWidget();
	DOM::Node node;
	int offset;
	uint curLine, curCol/**, curLine2, curCol2*/;
	/**DOM::Range range(kafkaPart) = kafkaPart->selection();*/

	/**DOM::Range tempRange(document());
	tempRange.setStart(document(), 0);
	tempRange.setEnd(m_currentNode, 2);
	setSelection(tempRange);*/

	//Translate and set the cursor.
	quantaApp->view()->write()->viewCursorIf->cursorPositionReal(&curLine, &curCol);
	quantaApp->view()->getKafkaInterface()->translateQuantaIntoKafkaCursorPosition(curLine,
		curCol, node, offset);
	kafkaPart->setCurrentNode(node, offset);

	//Translate and set the selection.
	//quantaApp->view()->write()->selectionIf()
}

void undoRedo::syncQuantaCursorAndSelection()
{
	KafkaWidget *kafkaPart = quantaApp->view()->getKafkaInterface()->getKafkaWidget();
	int curCol, curLine, curCol2, curLine2;
	uint oldCurCol, oldCurLine;
	DOM::Node domNode, domNodeEnd;
	int offset;
	long offsetBegin, offsetEnd;
	DOM::Range range(kafkaPart);

	//Translate and set the cursor.
	quantaApp->view()->getKafkaInterface()->getKafkaWidget()->getCurrentNode(domNode, offset);
 	quantaApp->view()->getKafkaInterface()->translateKafkaIntoQuantaCursorPosition(domNode,
		offset, curLine, curCol);
	quantaApp->view()->write()->viewCursorIf->cursorPositionReal(&oldCurLine, &oldCurCol);
	if(oldCurCol != (uint)curCol || oldCurLine != (uint)curLine)
 		quantaApp->view()->write()->viewCursorIf->setCursorPositionReal((uint)curLine, (uint)curCol);

	//Translate and set the selection
	kafkaPart->selection(domNode, offsetBegin, domNodeEnd, offsetEnd);
	quantaApp->view()->getKafkaInterface()->translateKafkaIntoQuantaCursorPosition(
		domNode, (int)offsetBegin, curLine, curCol);
	quantaApp->view()->getKafkaInterface()->translateKafkaIntoQuantaCursorPosition(
		domNodeEnd, (int)offsetEnd, curLine2, curCol2);
	quantaApp->view()->write()->selectionIf->setSelection(curLine, curCol, curLine2, curCol2);
}

void undoRedo::debugOutput()
{
#ifdef HEAVY_DEBUG
	int i = 0;
	bool afterEditorIt = false;

	kdDebug(24000)<< "Undo/redo stack contents:" << endl;
	if(m_undoList.isEmpty())
	{
		kdDebug(24000)<< "Empty!" << endl;
		return;
	}
	QPtrListIterator<NodeModifsSet> it(m_undoList);
	for(it.toFirst(); it ; ++it )
	{
		kdDebug(24000)<< "== Node Modifications set #" << i << endl;
		if((*it)->nodeModifList().isEmpty())
		{
			kdDebug(24000)<< "== Empty!" << endl;
			kdDebug(24000)<< "== End Node Modifications set #" << i << endl;
			i++;
			continue;
		}
		//kdDebug(24000)<< "== Cursor Pos: " << (*it).cursorY << ":" << (*it).cursorX << endl;
		//kdDebug(24000)<< "== Cursor Pos2:" << (*it).cursorY2 << ":" << (*it).cursorX2 << endl;
		QPtrListIterator<NodeModif> it2((*it)->nodeModifList());
		for(it2.toFirst(); it2; ++it2)
		{
			kdDebug(24000)<< "==== NodeModif type:" << (*it2)->type() << endl;
			kdDebug(24000)<< "==== Location1: " << endl;
			QValueList<int>::iterator it3;
			if((*it2)->location().empty())
			{
				kdDebug(24000)<< "==== Empty location!!" << endl;
			}
			else if((*it2)->type() != NodeModif::NodeTreeAdded &&
				(*it2)->type() != NodeModif::NodeTreeRemoved)
			{
				for(it3 = (*it2)->location().begin(); it3 != (*it2)->location().end(); ++it3)
					kdDebug(24000)<< (*it3) << endl;
			}
			if((((*it2)->type() == NodeModif::NodeRemoved && !afterEditorIt) ||
				((*it2)->type() == NodeModif::NodeAdded && afterEditorIt)) && (*it2)->node())
				kdDebug(24000)<< "==== Node: " << (*it2)->node()->tag->name <<
					" - contents: " << (*it2)->node()->tag->tagStr() << endl;
			if((*it2)->type() == NodeModif::NodeModified)
				kdDebug(24000)<< "==== Tag: " << (*it2)->tag()->name <<
					" - contents: " << (*it2)->tag()->tagStr() << endl;
			if(((*it2)->type() == NodeModif::NodeRemoved && !afterEditorIt) ||
				((*it2)->type() == NodeModif::NodeAdded && afterEditorIt))
				kdDebug(24000)<< "==== ChildsNumber1 : " << (*it2)->childrenMovedUp() <<
					" - ChildsNumber2 : " << (*it2)->neighboursMovedDown() << endl;
		}
		kdDebug(24000)<< "== End Node Modifications set #" << i << endl;
		i++;
		if(it == sourceIterator)
			afterEditorIt = true;
	}
	kdDebug(24000)<< "End Undo/redo stack contents" << endl;
	kafkaCommon::coutTree(baseNode, 2);
#endif
}

void undoRedo::fileSaved()
{/**
	QValueList<NodeModifsSet>::iterator it = sourceIterator;
	(*sourceIterator).isModified = false;
	//seting isModified = true to all others
	while(it != begin())
	{
		it--;
		(*it).isModified = true;
	}
	it = sourceIterator;
	it++;
	while(it != end())
	{
		(*it).isModified = true;
		it++;
	}*/
}

void undoRedo::kafkaLoaded()
{
	kafkaIterator = documentIterator;
}

