/***************************************************************************
                                undoredo.cpp
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

//debug only
#include <qdatetime.h>
//end debug only
#include <qtabwidget.h>

#include <kapplication.h>
#include <kconfig.h>
#include <kdebug.h>
#include <kiconloader.h>
#include <ktexteditor/editinterface.h>
#include <ktexteditor/editinterfaceext.h>
#include <ktexteditor/selectioninterface.h>
#include <ktexteditor/viewcursorinterface.h>
#include <dom/dom_node.h>
#include <dom/dom_exception.h>

#include "document.h"
#include "node.h"
#include "quantaview.h"
#include "quantacommon.h"
#include "resource.h"
#include "tag.h"
#include "viewmanager.h"
#include "wkafkapart.h"
#include "kafkacommon.h"
#include "kafkaresource.h"
#include "cursors.h"

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
    Node::deleteNode(m_node);
  }
  if(m_tag)
    delete m_tag;
}

void NodeModif::setNode(Node *node)
{
  ViewManager *viewManager = ViewManager::ref();
  if(viewManager && viewManager->activeDocument() && !viewManager->activeDocument()->docUndoRedo->turnedOn())
  {
    //FIXME: Andras: I don't have the slightest idea what this is supposed to do and what the
    //below comment means, but without a real delete we are seriously leaking memory
    Node::deleteNode(m_node);
    m_node = 0L;
    Node::deleteNode(node);
    return;
  }
  else
  {
    m_node = node;
    if(m_node)
    {
      m_node->parent = 0L;
      m_node->next = 0L;
      m_node->prev = 0L;
      if(m_type == NodeRemoved)
        m_node->child = 0L;
    }
  }
}

void NodeModif::setTag(Tag *tag)
{
  ViewManager *viewManager = ViewManager::ref();
  if(viewManager && viewManager->activeDocument() && !viewManager->activeDocument()->docUndoRedo->turnedOn())
  {
    //TEMPORARY cf setNode
    m_tag = tag;
    if(m_tag)
    {
      delete m_tag;
      m_tag = 0L;
    }
  }
  else
  {
    m_tag = tag;
  }
}

NodeModifsSet::NodeModifsSet()
{
  m_selectionBefore = new NodeSelectionInd();
  m_selectionAfter = new NodeSelectionInd();
  m_indentationStartOffset = -1;

  if(ViewManager::ref()->activeDocument())
    m_isModifiedBefore = ViewManager::ref()->activeDocument()->isModified();
  else
    m_isModifiedBefore = true;
  m_isModifiedAfter = true;

  //A NodeModifsSet instance is created before the changes are made =>
  //Recording the cursor position
  if(ViewManager::ref()->activeView()->hadLastFocus() == QuantaView::VPLFocus)
  {
    m_selectionBefore->fillWithVPLCursorSelection();
  }
  else
  {
  }
}

NodeModifsSet::~NodeModifsSet()
{
  m_nodeModifList.setAutoDelete(true);
  m_nodeModifList.clear();
  delete m_selectionBefore;
  delete m_selectionAfter;
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
  m_listLimit = 50;
  m_merging = false;
  m_mergeNext = false;
  m_loggingEnabled = false;
}

undoRedo::~undoRedo()
{
#ifdef LIGHT_DEBUG
  kdDebug(25001)<< "undoRedo::~undoRedo()" << endl;
#endif
}

void undoRedo::addNewModifsSet(NodeModifsSet *modifs, int modifLocation, NodeSelection *selection, bool encodeText)
{
#ifdef LIGHT_DEBUG
  kdDebug(25001)<< "undoRedo::addNewModifsSet() - NodeModifsSet type: " << modifLocation << endl;
#endif

  QValueList<NodeModif>::iterator it2;
  NodeModifsSet *NMSet;
  QValueList<int> loc;
  int curFocus, foo, foo2;
  int diff, diff2;
  NodeSelectionInd *nodeSelection;
  Node *node;
  bool goUp;

  //If modifs is empty, stop here
  if(!modifs || modifs->nodeModifList().isEmpty())
  {
    delete modifs;
    return;
  }

  KConfig* config = kapp->config();
  config->setGroup("Kate Document Defaults");
  int indentationWidth = config->readNumEntry("Indentation Width", 4);
  
  //Once the changes have been made, we will generate the "clean" string for Text Nodes only, and
  //we will add the empty indentation Nodes.
  modifs->startOfIndentation();
  node = baseNode;
  while(node)
  {
      if(!node->tag->cleanStrBuilt() && 
          (node->tag->type == Tag::Text || (node->tag->type == Tag::Empty && !node->tag->tagStr().isEmpty())))
      {
      if(!node->insideSpecial)
      {
        node->tag->setStr(KafkaDocument::ref()->generateCodeFromNode(node, 0, 0, foo, foo2, encodeText));
        node->tag->setCleanStrBuilt(true);
      }
    }
    if(!node->tag->indentationDone() && !node->insideSpecial)
    {
      kafkaCommon::fitIndentationNodes(kafkaCommon::getPrevNodeNE(node), node, modifs);
      goUp = false;
      kafkaCommon::fitIndentationNodes(node, kafkaCommon::getNextNodeNE(node, goUp), modifs);
      kafkaCommon::applyIndentation(node, indentationWidth, 0, modifs, qConfig.inlineNodeIndentation);
    }
    node = node->nextSibling();
  }

  //Set the modification flag
  if(ViewManager::ref()->activeView()->hadLastFocus() == QuantaView::VPLFocus)
    m_doc->setModified(true);

  //Store the cursor position after the changes.
  nodeSelection = modifs->selectionAfter();
  if(ViewManager::ref()->activeView()->hadLastFocus() == QuantaView::VPLFocus)
    nodeSelection->fillWithVPLCursorSelection();

  //If the previous NodeModifsSet contains some text insertion/deletion and if
  //the current one is doing the same thing, compress the two NodeModifsSet : delete modifs
  if(modifs->nodeModifList().count() >= 1 && modifs->indentationStartOffset() == 1  &&
    modifs->nodeModifList().at(0)->type() == NodeModif::NodeModified)
  {
    QPtrListIterator<NodeModifsSet> it(m_undoList);
    it = documentIterator;
    if((*it) && (*it)->nodeModifList().count() >= 1 && (*it)->indentationStartOffset() == 1 &&
      (*it)->nodeModifList().at(0)->type() == NodeModif::NodeModified &&
      (*it)->isModifiedAfter())
    {
      node = kafkaCommon::getNodeFromLocation(modifs->nodeModifList().at(0)->location());
      diff = modifs->nodeModifList().at(0)->tag()->tagStr().length() - (*it)->nodeModifList().at(0)->tag()->tagStr().length();
      diff2 = node->tag->tagStr().length() - modifs->nodeModifList().at(0)->tag()->tagStr().length();

      if(*((*it)->selectionAfter()) == *(modifs->selectionBefore()) &&
        ((diff >= 0 && diff2 >= 0) || (diff <= 0 && diff2 <= 0)))
      {
        //Ok, we are skipping this one. Update the selection coordinates of (*it)
        (*it)->setSelectionAfter(modifs->selectionAfter());
        modifs->setSelectionAfter(0L);
        delete modifs;

        //Move backward the iterator so that it will refresh next time        
        curFocus = ViewManager::ref()->activeView()->hadLastFocus();
        if((modifLocation == undoRedo::SourceModif || 
          (modifLocation == undoRedo::NodeTreeModif && curFocus == QuantaView::SourceFocus)) && 
          kafkaIterator.atLast())
          --kafkaIterator;
        else if((modifLocation == undoRedo::KafkaModif ||
           (modifLocation == undoRedo::NodeTreeModif && curFocus == QuantaView::VPLFocus)) &&
           sourceIterator.atLast())
          --sourceIterator;
        if(modifLocation == undoRedo::NodeTreeModif)
        {
          if(curFocus == QuantaView::SourceFocus)
            reloadQuantaEditor();
          else
            reloadKafkaEditor(false, selection);
        }
        return;
      }
    }
  }

  //Store the NodeModifsSet
  m_undoList.append(modifs);
#ifdef HEAVY_DEBUG
  debugOutput();
#endif
  while(m_undoList.count() > (unsigned)m_listLimit)
  {
    // FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME FIXME
    //FIXME: This is to prevent the list to be infinite, change when undoRedo is finished!! FIXME
    if(!kafkaIterator.current() || kafkaIterator.atFirst())
    {
      kafkaIterator = sourceIterator;
      --kafkaIterator;
    }
    else if(sourceIterator.current() || sourceIterator.atFirst())
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
    curFocus = ViewManager::ref()->activeView()->hadLastFocus();
    if(curFocus == QuantaView::SourceFocus)
      reloadQuantaEditor();
    else
      reloadKafkaEditor(false, selection);
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

void undoRedo::turnOn(bool on)
{
  if(!m_loggingEnabled && on)
  {
    //TEMPORARY : Delete all the undo/Redo stack, we only want to keep VPL NodeModifs
    m_undoList.setAutoDelete(true);
    m_undoList.clear();
    m_undoList.setAutoDelete(false);
  }

  m_loggingEnabled = on;
}

bool undoRedo::undo()
{
  bool success = true;

#ifdef LIGHT_DEBUG
  kdDebug(25001)<< "undoRedo::undo()"<< endl;
  QTime t;
  t.start();
#endif
  if(documentIterator.isEmpty() || !(*documentIterator))
    return false;
  QPtrListIterator<NodeModif> it((*documentIterator)->nodeModifList());
  it.toLast();

  while(*it)
  {
    //Undo the changes
    if(!undoNodeModif(*it))
    {
      //one undo has failed, trying to recover considering that the undo has done nothing.
      kdDebug(25001)<< "Undo failed, trying to recover." << endl;
      ++it;
      while((*it) && success)
      {
        success = redoNodeModif(*it);
#ifdef LIGHT_DEBUG
        kdDebug(25001) << "NodeModif type :" << (*it)->type() <<" redoed!" << endl;
#endif
        ++it;
      }
      return false;
    }

#ifdef LIGHT_DEBUG
    kdDebug(25001) << "NodeModif type :" << (*it)->type() <<" undoed!" << endl;
#endif
    --it;
  }

  //We need to update the internal pointer of baseNode in the parser. FIXME:why?
  parser->setRootNode(baseNode);

  //Update the modified flag
  m_doc->setModified((*documentIterator)->isModifiedBefore());

  QPtrListIterator<NodeModifsSet> currentIt(documentIterator);
  --documentIterator;

  //TEMP: Reload the kafka editor TODO: update only the modified Nodes
  if(ViewManager::ref()->activeView()->hadLastFocus() == QuantaView::VPLFocus)
    reloadKafkaEditor();

  //Restore the cursor at the old coordinates
  if(ViewManager::ref()->activeView()->hadLastFocus() == QuantaView::VPLFocus)
    KafkaDocument::ref()->setCursorAndSelection((*currentIt)->selectionBefore());


#ifdef LIGHT_DEBUG
  kdDebug(25001) << "undoRedo::undo() : " << t.elapsed() << " ms" << endl;
#endif

#ifdef HEAVY_DEBUG
  //debugOutput();
#endif
  return !(*documentIterator);
}

bool undoRedo::redo()
{
  bool success = true;

#ifdef LIGHT_DEBUG
  QTime t;
  t.start();
  kdDebug(25001)<< "undoRedo::redo()" << endl;
#endif
  if(documentIterator.isEmpty() || documentIterator.atLast())
    return false;

  if(!(*documentIterator))
    documentIterator.toFirst();
  else
    ++documentIterator;

  QPtrListIterator<NodeModif> it((*documentIterator)->nodeModifList());
  it.toFirst();

  while(*it)
  {
    //Redo the changes
    if(!redoNodeModif(*it))
    {
      //one redo has failed, trying to recover considering that the redo has done nothing.
      kdDebug(25001)<< "Redo failed, trying to recover." << endl;
      --it;
      while((*it) && success)
      {
        success = undoNodeModif(*it);
#ifdef LIGHT_DEBUG
        kdDebug(25001) << "NodeModif type :" << (*it)->type() <<" undoed!" << endl;
#endif
        --it;
      }
      --documentIterator;
      return false;
    }
#ifdef LIGHT_DEBUG
    kdDebug(25001) << "NodeModif type :" << (*it)->type() <<" redoed!" << endl;
#endif
    ++it;
  }
  //We need to update the internal pointer of baseNode in the parser. FIXME: why?
  parser->setRootNode(baseNode);

  //Update the modified flag
  m_doc->setModified((*documentIterator)->isModifiedAfter());

  //TEMP: Reload the kafka editor TODO: update only the modified Nodes
  if(ViewManager::ref()->activeView()->hadLastFocus() == QuantaView::VPLFocus)
    reloadKafkaEditor();

  //Restore the cursor at the old coordinates
  if(ViewManager::ref()->activeView()->hadLastFocus() == QuantaView::VPLFocus)
    KafkaDocument::ref()->setCursorAndSelection((*documentIterator)->selectionAfter());

#ifdef LIGHT_DEBUG
  kdDebug(25001) << "undoRedo::redo() : " << t.elapsed() << " ms" << endl;
#endif

#ifdef HEAVY_DEBUG
  //debugOutput();
#endif
  return !documentIterator.atLast();
}

bool undoRedo::undoNodeModif(NodeModif *nodeModif)
{
  Node *node, *newNode;
  QValueList<int> ref;
  Tag *tag;
  bool b;

  //Note : for NodeModif::NodeMoved && NodeModif::NodeAndChildsMoved, we go
  //through the processing of NodeModif::NodeAdded && NodeModif::NodeRemoved

  if(nodeModif->type() == NodeModif::NodeTreeAdded)
  {
    //Set baseNode to 0L
    nodeModif->setNode(baseNode);
    baseNode = 0L;
  }
  if(nodeModif->type() == NodeModif::NodeAndChildsAdded ||
      nodeModif->type() == NodeModif::NodeAdded ||
      nodeModif->type() == NodeModif::NodeMoved ||
      nodeModif->type() == NodeModif::NodeAndChildsMoved)
  {
    // Removing the node
    if(nodeModif->type() == NodeModif::NodeAndChildsAdded ||
        nodeModif->type() == NodeModif::NodeAdded)
      node = kafkaCommon::getNodeFromLocation(nodeModif->location());
    else
      node = kafkaCommon::getNodeFromLocation(nodeModif->finalLocation());

    if(!node)
    {
      kdDebug(25001)<< "undoRedo::undoNodeModif() - NodeModif::NodeAdded/Moved - ERROR1" << endl;
      return false;
    }
    kafkaCommon::extractNode(node, 0L, true,false);
    nodeModif->setNode(node);
  }
  if(nodeModif->type() == NodeModif::NodeModified)
  {
    // Simply replacing the tag(node->tag) of the node by the old tag.
    node = kafkaCommon::getNodeFromLocation(nodeModif->location());
    if(!node)
    {
      kdDebug(25001)<< "undoRedo::undoNodeModif() - NodeModif::NodeModified - ERROR3" << endl;
      return false;
    }
    tag = nodeModif->tag();
    nodeModif->setTag(node->tag);
    b = false;
    node->tag = tag;
  }
  if(nodeModif->type() == NodeModif::NodeRemoved ||
      nodeModif->type() == NodeModif::NodeAndChildsRemoved ||
      nodeModif->type() == NodeModif::NodeMoved ||
      nodeModif->type() == NodeModif::NodeAndChildsMoved)
  {
    // Adding the node
    newNode = nodeModif->node();
    nodeModif->setNode(0L);
    node = kafkaCommon::getNodeFromLocation(nodeModif->location());
    if(!node)
    {
      // No node at this location, getting the parent Node and appending newNode after
      // the last child of the parent.
      ref = nodeModif->location();
      QValueList<int> loc(ref);
      loc.remove(loc.fromLast());
      if(loc.empty())
      {
        // No parent, adding it on top of the tree.
        kafkaCommon::insertNode(newNode, 0L, 0L, 0L, false);
      }
      else
      {
        node = kafkaCommon::getNodeFromLocation(loc);
        if(!node)
        {
          kdDebug(25001)<< "undoRedo::undoNodeModif() - NodeModif::NodeRemoved - ERROR4" << endl;
          return false;
        }
        kafkaCommon::insertNode(newNode, node, 0L, 0L, false);
      }
    }
    else
    {
      // A node is already here. Moving it to the right and adding newNode here.
      kafkaCommon::insertNode(newNode, node->parent, node, 0L, false);
    }
  }
  if(nodeModif->type() == NodeModif::NodeTreeRemoved)
  {
    //Adding the tree.
    baseNode = nodeModif->node();
    nodeModif->setNode(0L);
  }

  return true;
}

bool undoRedo::redoNodeModif(NodeModif *nodeModif)
{
  bool success;
  QValueList<int> tmp;

  //To do the opposite action of undoNodeModif(), we simply have
  //to change the type of nodeModif

  if(nodeModif->type() == NodeModif::NodeTreeAdded)
    nodeModif->setType(NodeModif::NodeTreeRemoved);
  else if(nodeModif->type() == NodeModif::NodeAndChildsAdded)
    nodeModif->setType(NodeModif::NodeAndChildsRemoved);
  else if(nodeModif->type() == NodeModif::NodeAdded)
    nodeModif->setType(NodeModif::NodeRemoved);
  else if(nodeModif->type() == NodeModif::NodeRemoved)
    nodeModif->setType(NodeModif::NodeAdded);
  else if(nodeModif->type() == NodeModif::NodeAndChildsRemoved)
    nodeModif->setType(NodeModif::NodeAndChildsAdded);
  else if(nodeModif->type() == NodeModif::NodeTreeRemoved)
    nodeModif->setType(NodeModif::NodeTreeAdded);
  else if(nodeModif->type() == NodeModif::NodeMoved || nodeModif->type() == NodeModif::NodeAndChildsMoved)
  {
    tmp = nodeModif->location();
    nodeModif->setLocation(nodeModif->finalLocation());
    nodeModif->setFinalLocation(tmp);
  }

  success = undoNodeModif(nodeModif);

  if(nodeModif->type() == NodeModif::NodeTreeRemoved)
    nodeModif->setType(NodeModif::NodeTreeAdded);
  else if(nodeModif->type() == NodeModif::NodeAndChildsRemoved)
    nodeModif->setType(NodeModif::NodeAndChildsAdded);
  else if(nodeModif->type() == NodeModif::NodeRemoved)
    nodeModif->setType(NodeModif::NodeAdded);
  else if(nodeModif->type() == NodeModif::NodeAdded)
    nodeModif->setType(NodeModif::NodeRemoved);
  else if(nodeModif->type() == NodeModif::NodeAndChildsAdded)
    nodeModif->setType(NodeModif::NodeAndChildsRemoved);
  else if(nodeModif->type() == NodeModif::NodeTreeAdded)
    nodeModif->setType(NodeModif::NodeTreeRemoved);
  else if(nodeModif->type() == NodeModif::NodeMoved || nodeModif->type() == NodeModif::NodeAndChildsMoved)
  {
    tmp = nodeModif->location();
    nodeModif->setLocation(nodeModif->finalLocation());
    nodeModif->setFinalLocation(tmp);
  }

  return success;
}

bool undoRedo::undoNodeModifInKafka(NodeModif */**_nodeModif*/)
{
  /**Node *_node, *n;
  Tag *_tag;
  DOM::Node domNode, domNode2, dn, dm;
  bool goUp;
  KafkaDocument *kafkaInterface = quantaApp->view()->kafkaInterface();
  KafkaWidget *kafkaPart = quantaApp->view()->kafkaInterface()->getKafkaWidget();

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
                  kdDebug(25001)<< "undoRedo::undoNodeModifInKafka() - ERROR1" << endl;
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
                          kdDebug(25001)<< "undoRedo::undoNodeModifInKafka() - ERROR2" << endl;
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
                  kdDebug(25001)<< "undoRedo::undoNodeModifInKafka() - ERROR4" << endl;
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
                  kdDebug(25001)<< "undoRedo::undoNodeModifInKafka() - ERROR1" << endl;
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

void undoRedo::reloadKafkaEditor(bool force, NodeSelection *selection)
{
#ifdef LIGHT_DEBUG
  kdDebug(25001)<< "undoRedo::reloadKafkaEditor()" << endl;
#endif

  if(kafkaIterator == documentIterator && !force)
  {
    syncKafkaCursorAndSelection(selection);
    return;
  }

  kafkaIterator = documentIterator;
  KafkaDocument *kafkaInterface = KafkaDocument::ref();

  kafkaInterface->reloadDocument();

  syncKafkaCursorAndSelection(selection);
}

void undoRedo::reloadQuantaEditor(bool force, bool syncQuantaCursor, bool encodeText)
{
  QString text, allText;
  Node *node = baseNode, *child;
  int bCol, bLine, eCol, eLine, bCol2, bLine2, bCol3, bLine3, eCol3, eLine3, i;
  KafkaDocument *kafkaInterface = KafkaDocument::ref();
  bool updateClosing, goUp, isModified;

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

  //save some values which must not be affected by / affect the reload
  isModified = m_doc->isModified();
  updateClosing = qConfig.updateClosingTags;
  m_doc->activateParser(false);
  m_doc->activateRepaintView(false);
  qConfig.updateClosingTags = false;

  //First build the tag string which needs to be updated, and add the necessary
  //empty Nodes for the indentation.
  while(node)
  {
    if(!node->tag->cleanStrBuilt())
    {
      if(!node->insideSpecial)
      {
        node->tag->setStr(kafkaInterface->generateCodeFromNode(node, 0, 0, eLine, eCol, encodeText));
      }
      else
      {
        //Script formatting
      }
      node->tag->setCleanStrBuilt(true);
    }
    //_node->tag->beginPos(bLine, bCol);
    //i can't stop redraw events of Kate!
    //m_doc->editIf->insertText(bLine, bCol, _node->tag->tagStr());
    //allText += _node->tag->tagStr();
    node->tag->endPos(bLine, bCol);
    bCol++;
    node = node->nextSibling();
  }

  //Then, we gather all the tag string and put it into kate, and we set the tag positions.
  node = baseNode;
  goUp = false;
  bCol = 0;
  bLine = 0;
  while(node)
  {
    //kdDebug(25001)<< "CurNode : " << _node->tag->name << " - " << _node->tag->tagStr() << endl;
    if(node->parent)
    {
      node->parent->tag->beginPos(bLine3, bCol3);
      node->parent->tag->endPos(eLine3, eCol3);
    }
    node->tag->beginPos(bLine2, bCol2);

    //if we are in a Script inside a tag e.g. <a href="<? PHP stuff here ?>">, skip it
    if(node->tag->type == Tag::ScriptTag && node->parent &&
        QuantaCommon::isBetween(bLine2, bCol2, bLine3, bCol3, eLine3,eCol3) == 0)
    {
      goUp = true;

      //if we found the closing script tag, skip it too
      if(node->next && node->next->tag->type == Tag::XmlTagEnd)
        node = node->next;
    }
    else
    {
      allText += node->tag->tagStr();

      //If a child is a Script inside this Tag e.g. <a href="<? PHP stuff here ?>">, make
      //its position fits inside the parent
      node->tag->beginPos(bLine3, bCol3);
      node->tag->endPos(eLine3, eCol3);
      child = node->firstChild();
      while(child)
      {
        child->tag->beginPos(bLine2, bCol2);
        if(child->tag->type == Tag::ScriptTag &&
            QuantaCommon::isBetween(bLine2, bCol2, bLine3, bCol3, eLine3,eCol3) == 0)
        {
          child->tag->setTagPosition(bLine, bCol + 1, bLine, bCol + 1);
        }
        child = child->next;
      }

      //Update the node's positions
      node->tag->setTagPosition(bLine, bCol, -1, -1);
      for(i = 0; i < node->tag->attrCount(); i++)
      {
        bCol3 = node->tag->getAttribute(i).nameLine;
        bLine3 = node->tag->getAttribute(i).nameCol;
        eCol3 = node->tag->getAttribute(i).valueLine;
        eLine3 = node->tag->getAttribute(i).valueCol;

        //FIXME: This is OK only when it has just been rebuild.
        node->tag->setAttributePosition(i, bLine3 + bLine, bCol3 + bCol, eLine3 + bLine,
                                        eCol3 + bCol);
      }
      kafkaCommon::getEndPosition(node->tag->tagStr(), bLine, bCol, eLine, eCol);
      node->tag->setTagPosition(bLine, bCol, eLine, eCol);

      bCol = eCol + 1;
      bLine = eLine;
    }
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

  m_doc->setModified(isModified);
  qConfig.updateClosingTags = updateClosing;
  m_doc->activateRepaintView(true);
  m_doc->activateParser(true);

}

void undoRedo::codeFormatting()
{
  Node *node = baseNode;

  while (node)
  {
    node->tag->setCleanStrBuilt(false);
    node->tag->setIndentationDone(false);
    node = node->nextSibling();
  }
  reloadQuantaEditor(true, false, false);
}

bool undoRedo::redoNodeModifInKafka(NodeModif */**_nodeModif*/)
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

  success = undoNodeModifInKafka(_nodeModif);

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
  /**     QValueList<NodeModifsSet>::iterator it;
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
                  ++it;
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
                                  if(!undoNodeModif((*it2), false))
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
                          for (it2 = (*kafkaIterator).NodeModifList.begin(); it2 != (*kafkaIterator).NodeModifList.end(); ++it2)
                          {
                                  if((*it2).type == undoRedo::NodeTreeAdded || (*it2).type == undoRedo::NodeAndChildsAdded ||
                                          (*it2).type == undoRedo::NodeAdded || (*it2).type == undoRedo::NodeModified)
                                  {
                                          if(!redoNodeModif((*it2), false))
                                          {
                                                  kdDebug(25001)<< "undoRedo::syncKafkaView() - ERROR 2" << endl;
                                                  return false;
                                          }
                                          if(!redoNodeModifInKafka(*it2))
                                          {
                                                  kdDebug(25001)<< "undoRedo::syncKafkaView() - ERROR 3" << endl;
                                                  return false;
                                          }
                                  }
                                  else
                                  {
                                          if(!redoNodeModifInKafka(*it2))
                                          {
                                                  kdDebug(25001)<< "undoRedo::syncKafkaView() - ERROR 4" << endl;
                                                  return false;
                                          }
                                          if(!redoNodeModif((*it2), false))
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
                          ++it;
                          for(it2 = (*it).NodeModifList.begin(); it2 != (*it).NodeModifList.end(); ++it2)
                          {
                                  if(!redoNodeModif((*it2), false))
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
                                          if(!undoNodeModifInKafka(*it2))
                                          {
                                                  kdDebug(25001)<< "undoRedo::syncKafkaView() - ERROR 8" << endl;
                                                  return false;
                                          }
                                          if(!undoNodeModif((*it2), false))
                                          {
                                                  kdDebug(25001)<< "undoRedo::syncKafkaView() - ERROR 7" << endl;
                                                  return false;
                                          }
                                  }
                                  else
                                  {
                                          if(!undoNodeModif((*it2), false))
                                          {
                                                  kdDebug(25001)<< "undoRedo::syncKafkaView() - ERROR 10" << endl;
                                                  return false;
                                          }
                                          if(!undoNodeModifInKafka(*it2))
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
          ++it;
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
                          if(!undoNodeModif((*it2), false))
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
                  for (it2 = (*sourceIterator).NodeModifList.begin(); it2 != (*sourceIterator).NodeModifList.end(); ++it2)
                  {
                          if(!redoNodeModif((*it2), true, true))
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
                  ++it;
                  for(it2 = (*it).NodeModifList.begin(); it2 != (*it).NodeModifList.end(); ++it2)
                  {
                          if(!redoNodeModif((*it2), false))
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
                          if(!undoNodeModif((*it2), true, true))
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

void undoRedo::syncKafkaCursorAndSelection(NodeSelection *selection)
{
  QuantaView *view = ViewManager::ref()->activeView();
  KafkaWidget *kafkaPart = KafkaDocument::ref()->getKafkaWidget();
  DOM::Node node;
  long offset;
  uint curLine, curCol/**, curLine2, curCol2*/;
  /**DOM::Range range(kafkaPart) = kafkaPart->selection();*/

  if(!KafkaDocument::ref()->isLoaded())
    return;

  /**DOM::Range tempRange(document());
  tempRange.setStart(document(), 0);
  tempRange.setEnd(m_currentNode, 2);
  setSelection(tempRange);*/

  //Translate and set the cursor.
  if(selection)
  {
    KafkaDocument::ref()->translateNodeIntoKafkaCursorPosition(selection->cursorNode(),
      selection->cursorOffset(), node, offset);
    kafkaPart->setCurrentNode(node, offset);
  }
  else
  {
    view->document()->viewCursorIf->cursorPositionReal(&curLine, &curCol);
    KafkaDocument::ref()->translateQuantaIntoKafkaCursorPosition(curLine,
    curCol, node, offset);
    kafkaPart->setCurrentNode(node, offset);
  }
  
  //Translate and set the selection.
  //quantaApp->view()->write()->selectionIf()
}

void undoRedo::syncQuantaCursorAndSelection()
{
  QuantaView *view = ViewManager::ref()->activeView();
  KafkaWidget *kafkaPart = KafkaDocument::ref()->getKafkaWidget();
  int curCol, curLine, curCol2, curLine2;
  uint oldCurCol, oldCurLine;
  DOM::Node domNode, domNodeEnd;
  long offset, offsetBegin, offsetEnd;
  DOM::Range range(kafkaPart);

  //Translate and set the cursor.
  KafkaDocument::ref()->getKafkaWidget()->getCurrentNode(domNode, offset);
  KafkaDocument::ref()->translateKafkaIntoQuantaCursorPosition(domNode,
      offset, curLine, curCol);
  view->document()->viewCursorIf->cursorPositionReal(&oldCurLine, &oldCurCol);
  if(oldCurCol != (uint)curCol || oldCurLine != (uint)curLine)
    view->document()->viewCursorIf->setCursorPositionReal((uint)curLine, (uint)curCol);

  //Translate and set the selection
  kafkaPart->selection(domNode, offsetBegin, domNodeEnd, offsetEnd);
  KafkaDocument::ref()->translateKafkaIntoQuantaCursorPosition(
    domNode, (int)offsetBegin, curLine, curCol);
  KafkaDocument::ref()->translateKafkaIntoQuantaCursorPosition(
    domNodeEnd, (int)offsetEnd, curLine2, curCol2);
  if (view->document()->selectionIf)
    view->document()->selectionIf->setSelection(curLine, curCol, curLine2, curCol2);
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
    kdDebug(24000)<< "== Node Modifications set #" << i << "(" << (*it)->isModifiedBefore() << "," <<
    (*it)->isModifiedAfter() << ")" << endl;
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
        if((*it2)->type() != NodeModif::NodeMoved ||
            (*it2)->type() != NodeModif::NodeAndChildsMoved)
        {
          kdDebug(24000)<< "==== Location2: " << endl;
          for(it3 = (*it2)->finalLocation().begin(); it3 != (*it2)->finalLocation().end(); ++it3)
            kdDebug(24000)<< (*it3) << endl;
        }
      }
      if((((*it2)->type() == NodeModif::NodeRemoved && !afterEditorIt) ||
          ((*it2)->type() == NodeModif::NodeAdded && afterEditorIt)) && (*it2)->node())
        kdDebug(24000)<< "==== Node: " << (*it2)->node()->tag->name <<
        " - contents: " << (*it2)->node()->tag->tagStr() << endl;
      if((*it2)->type() == NodeModif::NodeModified && (*it2)->tag())
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
{
  QPtrListIterator<NodeModifsSet> it(m_undoList);
  bool previousWasDocIt = false;
  for(it.toFirst(); it ; ++it )
  {
    if(previousWasDocIt)
    {
      (*it)->setIsModifiedBefore(false);
      (*it)->setIsModifiedAfter(true);
      previousWasDocIt = false;
    }
    else if(it == documentIterator)
    {
      (*it)->setIsModifiedBefore(true);
      (*it)->setIsModifiedAfter(false);
      previousWasDocIt = true;
    }
    else
    {
      (*it)->setIsModifiedBefore(true);
      (*it)->setIsModifiedAfter(true);
    }
  }
  /**
          QValueList<NodeModifsSet>::iterator it = sourceIterator;
          (*sourceIterator).isModified = false;
          //seting isModified = true to all others
          while(it != begin())
          {
                  it--;
                  (*it).isModified = true;
          }
          it = sourceIterator;
          ++it;
          while(it != end())
          {
                  (*it).isModified = true;
                  ++it;
          }*/
}

void undoRedo::kafkaLoaded()
{
  kafkaIterator = documentIterator;
}

