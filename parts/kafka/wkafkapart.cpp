/***************************************************************************
                               wkafkapart.cpp
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

#include <dom/dom_node.h>
#include <dom/dom_exception.h>
#include <dom/dom_string.h>
#include <dom/css_stylesheet.h>
#include <kdebug.h>
#include <kstandarddirs.h>
#include <ktexteditor/editinterface.h>
#include <kstandarddirs.h>
#include <klocale.h>
//#include <kglobal.h>
//#include <kcharsets.h>

#include <qregexp.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qdatetime.h>

#include "../../document.h"
#include "../../quanta.h"
#include "../../quantaview.h"
#include "../../resource.h"
#include "undoredo.h"
#include "../../parser/node.h"
#include "../../parser/parser.h"
#include "../../parser/tag.h"
#include "nodeproperties.h"
#include "htmlenhancer.h"
#include "kafkacommon.h"

#include "wkafkapart.moc"

#define EDITOR_MAX_COL 50

WKafkaPart::WKafkaPart(QWidget *parent, QWidget *widgetParent, const char *name)
:domNodeProps(1021)
{
// i18n reserve
QString a = i18n("Selector");QString b = i18n("Attribute");QString c = i18n("Class");
QString d = i18n("Pseudo-class");QString e = i18n("CSS rules");QString f = i18n("Universal selector");
QString g = i18n("Linked stylesheets");QString h = i18n("Embedded stylesheets");QString i = i18n("Inline style attribute");
QString j = i18n("Link");QString k = i18n("Priority");QString l = i18n("ID");
QString m = i18n("Browser support");QString n = i18n("Pseudo-element");QString o = i18n("Imported");
QString p = i18n("Inheritance");QString q = i18n("Inherited");QString r = "";

QString s = i18n("Name");QString tt = i18n("Undo");QString u = i18n("Redo");
QString v = i18n("Undo/Redo history");QString w = i18n("CSS styles");

QString x = i18n("Sorry, VPL doesn't support this functionality yet!");
QString y = i18n("Merge cells");
QString z = i18n("Split cells");
QString aa = i18n("Edit CSS style of this Tag");
QString ab = i18n("Ident all");
//end

#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "WKafkaPart::WKafkaPart()" << endl;
#endif
	_kafkaPart = new KafkaHTMLPart(parent, widgetParent,this, name);
	_kafkaPart->showDomTree();
	_docLoaded = false;
	_currentDoc = 0L;
	mainEnhancer =  new HTMLEnhancer(this);
	domNodeProps.setAutoDelete(false);

	KStandardDirs *m_stddirs = new KStandardDirs();
	QFile file( m_stddirs->findResource("data", "kafkapart/entities" )/**locate("appdata","chars") */);
	delete m_stddirs;
	QString tmp;
	if ( file.open(IO_ReadOnly) )
	{
		QTextStream t( &file );        // use a text stream
		while ( !t.eof() )
		{
			tmp = t.readLine();
			if(tmp.left(2) == "//") continue;//comments
			int begin = tmp.find("(") + 1;
			if(begin == (-1 + 1)) continue;//"(" not found : invalid line
			int length = tmp.find(")") - begin;
			decodedChars.insert(tmp.left(1), tmp.mid(begin, length));
			encodedChars.insert(tmp.mid(begin, length), tmp.left(1));
		}
		file.close();
	}

	connect(_kafkaPart, SIGNAL(domNodeInserted(DOM::Node, bool)),
		this, SLOT(slotDomNodeInserted(DOM::Node, bool)));
	connect(_kafkaPart, SIGNAL(domNodeModified(DOM::Node)),
		this, SLOT(slotDomNodeModified(DOM::Node)));
	connect(_kafkaPart, SIGNAL(domNodeIsAboutToBeRemoved(DOM::Node, bool)),
		this, SLOT(slotDomNodeAboutToBeRemoved(DOM::Node, bool)));

	connect(_kafkaPart, SIGNAL(domNodeNewCursorPos(DOM::Node, int)),
		this, SLOT(slotdomNodeNewCursorPos(DOM::Node, int)));

}

WKafkaPart::~WKafkaPart()
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "WKafkaPart::~WKafkaPart()" << endl;
#endif
	//delete the empty node linked to the DOM::Node #document
	disconnectAllDomNodes();
}

void WKafkaPart::loadDocument(Document *doc)
{

#ifdef LIGHT_DEBUG
	kdDebug(25001) << "WKafkaPart::loadDocument()" << endl;
#endif
	Node *node;
	Tag *tag;
	if(!_kafkaPart) return;
	if(!doc) return;
#ifdef LIGHT_DEBUG
	QTime t;
	t.start();
#endif
	_currentDoc = doc;

	(static_cast<HTMLEnhancer *>(mainEnhancer))->setBaseURL(quantaApp->view()->baseURL());

	//create a empty document with a basic tree : HTML, HEAD, BODY
	_kafkaPart->newDocument();
	/** creating and linking an empty node to the root DOM::Node (#document) and to HEAD, HTML, BODY*/
	node = new Node(0L);
	tag = new Tag();
	tag->name = "#document";
	tag->notInTree = true;
	node->tag = tag;
	connectDomNodeToQuantaNode(_kafkaPart->document(), node);
	node = new Node(0L);
	tag = new Tag();
	tag->name = "HTML";
	tag->notInTree = true;
	node->tag = tag;
	connectDomNodeToQuantaNode(_kafkaPart->document().firstChild(), node);
	html = _kafkaPart->document().firstChild();
	node = new Node(0L);
	tag = new Tag();
	tag->name = "HEAD";
	tag->notInTree = true;
	node->tag = tag;
	connectDomNodeToQuantaNode(_kafkaPart->document().firstChild().firstChild(), node);
	head = _kafkaPart->document().firstChild().firstChild();
	node = new Node(0L);
	tag = new Tag();
	tag->name = "BODY";
	tag->notInTree = true;
	node->tag = tag;
	connectDomNodeToQuantaNode(_kafkaPart->document().firstChild().lastChild(), node);
	body = _kafkaPart->document().firstChild().lastChild();

	//load the nodes
	Node* _node = baseNode;
	while(_node)
	{
#ifdef HEAVY_DEBUG
		kdDebug(25001) << "WKafkaPart::loadDocument - Node name :" <<
			_node->tag->name.upper() << "; type : " <<
			_node->tag->type << "; tagstr : " << _node->tag->tagStr() <<
			" is opened :" << _node->opened << endl;
#endif
		buildKafkaNodeFromNode(_node);
		_node = _node->nextSibling();
	}
	_kafkaPart->finishedLoading();
	_docLoaded = true;
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "WKafkaPart::loadDocument() in " << t.elapsed() << " ms only!" << endl;
#endif
	if(quantaApp->view()->writeExists())
		quantaApp->view()->write()->docUndoRedo->kafkaLoaded();
	emit loaded();
}

void WKafkaPart::unloadDocument()
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "WKafkaPart::unloadDocument()" << endl;
#endif
	Node *node;

	domNodeProps.clear();
	//clean the kafkapart
	while(_kafkaPart->document().hasChildNodes())
		_kafkaPart->document().removeChild(_kafkaPart->document().firstChild());
	_currentDoc = 0L;
	_docLoaded = false;
	node = baseNode;
	while(node)
	{
		node->_rootNode = DOM::Node();
		node->_leafNode = DOM::Node();
		node = node->nextSibling();
	}
	emit unloaded();
}

void WKafkaPart::reloadDocument()
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "WKafkaPart::reloadDocument()" << endl;
#endif
	Node *node;

	if(!_docLoaded)
		return;

	domNodeProps.clear();
	while(_kafkaPart->document().hasChildNodes())
		_kafkaPart->document().removeChild(_kafkaPart->document().firstChild());
	node = baseNode;
	while(node)
	{
		node->_rootNode = DOM::Node();
		node->_leafNode = DOM::Node();
		node = node->nextSibling();
	}
	loadDocument(_currentDoc);
}

void WKafkaPart::readConfig(KConfig *m_config)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "WKafkaPart::readConfig()" << endl;
#endif
	//for each nodeEnhancer
	if(mainEnhancer)
		mainEnhancer->readConfig(m_config);
	//reload the document
	reloadDocument();
}

void WKafkaPart::slotDomNodeInserted(DOM::Node _domNode, bool insertChilds)
{
#ifdef LIGHT_DEBUG
	if(!_domNode.isNull())
		kdDebug(25001)<< "WKafkaPart::slotDomNodeInserted() - DOM::Node: " <<
			_domNode.nodeName().string() << endl;
	else
		kdDebug(25001)<< "WKafkaPart::slotDomNodeInserted() - DOM::Node: NULL" << endl;
#endif
	Node *_nodeParent = 0L, *_nodeNext = 0L, *_node = 0L;
	DOM::Node tmpDomNode;
	bool b = false;
	NodeModifsSet modifs;

#ifdef LIGHT_DEBUG
	QTime t;
	t.start();
#endif
	modifs.cursorX = 0;
	modifs.cursorY = 0;
	modifs.cursorX2 = 0;
	modifs.cursorY2 = 0;
	modifs.isModified = true;//TODO:determine this

	_nodeParent = searchCorrespondingNode(_domNode.parentNode());

	if(!_nodeParent)
	{//DOM::Node not found, strange...
		kdDebug(25001)<< "WKafkaPart::slotDomNodeInserted() - *ERROR* the" <<
			" corresponding DOM::Node is not found!" << endl;
		return;
	}
	if(!_domNode.nextSibling().isNull())
	{
		_nodeNext = searchCorrespondingNode(_domNode.nextSibling());
		if(!_nodeNext)
		{
			kdDebug(25001)<< "WKafkaPart::slotDomNodeInserted() - *ERROR2*" <<
			" the corresponding DOM::Node is not found!" << endl;
			return;
		}
	}

	_node = buildNodeFromKafkaNode(_domNode, _nodeParent, _nodeNext, 0, 0L, 0, modifs);

	if(insertChilds && _domNode.hasChildNodes())
	{
		//TODO: check if it is working
		tmpDomNode = _domNode.firstChild();
		while(!tmpDomNode.isNull())
		{
			buildNodeFromKafkaNode(tmpDomNode,
				searchCorrespondingNode(tmpDomNode.parentNode()), 0L, 0, 0L, 0, modifs);
			tmpDomNode = kafkaCommon::getNextDomNode(tmpDomNode, b, false, _domNode);
		}
	}
	_currentDoc->docUndoRedo->addNewModifsSet(modifs, undoRedo::KafkaModif);

#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "WKafkaPart::slotDomNodeInserted() in " << t.elapsed() <<
		" ms only!" << endl;
#endif
#ifdef HEAVY_DEBUG
	kafkaCommon::coutTree(baseNode, 2);
#endif

}

void WKafkaPart::slotDomNodeModified(DOM::Node _domNode)
{
#ifdef LIGHT_DEBUG
	if(!_domNode.isNull())
		kdDebug(25001)<< "WKafkaPart::slotDomNodeModified() - DOM::Node: " <<
			_domNode.nodeName().string() << endl;
	else
		kdDebug(25001)<< "WKafkaPart::slotDomNodeModfied() - DOM::Node: NULL" << endl;
#endif
	Node *_node = 0L;
	NodeModifsSet modifs;
	NodeModif modif;

#ifdef LIGHT_DEBUG
	QTime t;
	t.start();
#endif
	modifs.cursorX = 0;
	modifs.cursorY = 0;
	modifs.cursorX2 = 0;
	modifs.cursorY2 = 0;
	modifs.isModified = true;//TODO:determine this
	modif.type = undoRedo::NodeModified;

	//first look which Node correspond to this DOM::Node
	_node = searchCorrespondingNode(_domNode);

	if(!_node)
	{//DOM::Node not found, weird...
		kdDebug(25001)<< "WKafkaPart::slotDomNodeModified() - *ERROR* the" <<
			" corresponding DOM::Node is not found!" << endl;
		return;
	}

	modif.tag = new Tag(*(_node->tag));
	modif.location = kafkaCommon::getLocation(_node);
	modif.node = 0L;

	buildNodeFromKafkaNode(_node, _domNode);

	modifs.NodeModifList.append(modif);
	_currentDoc->docUndoRedo->addNewModifsSet(modifs, undoRedo::KafkaModif);

#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "WKafkaPart::slotDomNodeModified() in " << t.elapsed() <<
		" ms only!" << endl;
#endif

#ifdef HEAVY_DEBUG
	kafkaCommon::coutTree(baseNode, 2);
#endif
}

void WKafkaPart::slotDomNodeAboutToBeRemoved(DOM::Node _domNode, bool deleteChilds)
{
#ifdef LIGHT_DEBUG
	if(!_domNode.isNull())
		kdDebug(25001)<< "WKafkaPart::slotDomNodeAboutToBeRemoved() - DOM::Node: " <<
			_domNode.nodeName().string() << " bool :" << deleteChilds << endl;
	else
		kdDebug(25001)<< "WKafkaPart::slotDomNodeAboutToBeRemoved() - DOM::Node: NULL bool :" <<
			deleteChilds << endl;
#endif
	Node *_node = 0L, *_nodeNext = 0L, *_tmpNode = 0L, *n = 0L;
	Tag *_tag;
	int i;
	bool hasClosingNode = false, b;
	NodeModifsSet modifs;
	NodeModif modif;

#ifdef LIGHT_DEBUG
	QTime t;
	t.start();
#endif

	if(!quantaApp->view()->writeExists())
	{
		kdDebug(25001)<< "WKafkaPart::slotDomNodeAboutToBeRemoved() - *BIG ERROR*" << endl;
		return;
	}
	_node = searchCorrespondingNode(_domNode);
	if(!_node)
	{
		kdDebug(25001)<<"WKafkaPart::slotDomNodeAboutToBeRemoved() - *ERROR* the" <<
			" corresponding DOM::Node is not found!" << endl;
		return;
	}

	modifs.cursorX = 0;
	modifs.cursorY = 0;
	modifs.cursorX2 = 0;
	modifs.cursorY2 = 0;
	modifs.isModified = true;//TODO:determine this

	if(_node->prev)
	{
		//delete the previous empty tag if present
		_nodeNext = _node;
		_node = _node->prev;
		if(_node && _node->tag->type == Tag::Empty)
		{
			modif.location = kafkaCommon::getLocation(_node);
			if(_node->parent && _node->parent->child == _node)
				_node->parent->child = _node->next;
			if(_node->prev)
				_node->prev->next = _node->next;
			if(_node->next)
				_node->next->prev = _node->prev;
			if(_node == baseNode)
				baseNode = _node->next;
			_node->parent = 0L;
			_node->prev = 0L;
			_node->next = 0L;
			_node->child = 0L;
			modif.type = undoRedo::NodeRemoved;
			modif.node = _node;
			modif.childsNumber = 0;
			modif.childsNumber2 = 0;
			modifs.NodeModifList.append(modif);
		}
		_node = _nodeNext;
	}

	//delete the Node
	if(deleteChilds)
		modif.type = undoRedo::NodeAndChildsRemoved;
	else
		modif.type = undoRedo::NodeRemoved;
	modif.location = kafkaCommon::getLocation(_node);

	if(_node->next && _node->next->closesPrevious)
		hasClosingNode = true;
	else
		hasClosingNode = false;
	//_node->removeAll = false;

	if(_node->parent && _node->parent->child == _node)
		_node->parent->child = _node->next;
	if(_node->next)
		_node->next->prev = _node->prev;
	if(_node->prev)
		_node->prev->next = _node->next;

	i = 0;
	if(_node->child && deleteChilds)
	{
		_tmpNode = _node->child;
		b = false;
		while(_tmpNode)
		{
			if(!_tmpNode->_rootNode.isNull())
				disconnectDomNodeFromQuantaNode(_tmpNode->_rootNode);
			if(!_tmpNode->_leafNode.isNull())
				disconnectDomNodeFromQuantaNode(_tmpNode->_leafNode);
			_tmpNode = kafkaCommon::getNextNode(_tmpNode, b, _node);
		}
		//delete _node->child;
	}
	else if(_node->child)// && !deleteChilds
	{
		if(!_node->parent->child)
			_node->parent->child = _node->child;
		if(_node->prev)
			_node->prev->next = _node->child;
		_tmpNode = _node->child;
		while(_tmpNode)
		{
			i++;
			_tmpNode->parent = _node->parent;
			n = _tmpNode;
			_tmpNode = _tmpNode->next;
			if(!_tmpNode)
			{
				n->next = _node->next;
				if(_node->next)
					_node->next->prev = n;
			}
		}
	}
	if(_node == baseNode)
		baseNode = _node->next;

	if(!_node->_rootNode.isNull())
		disconnectDomNodeFromQuantaNode(_node->_rootNode);
	if(!_node->_leafNode.isNull())
		disconnectDomNodeFromQuantaNode(_node->_leafNode);
	_node->parent = 0L;
	_nodeNext = _node->next;
	_node->next = 0L;
	_node->prev = 0L;
	_node->child = 0L;
	modif.node = _node;
	modif.childsNumber2 = 0;
	//delete _node;
	modif.childsNumber = i;
	modifs.NodeModifList.append(modif);
	_node = _nodeNext;

	if(hasClosingNode)
	{
		//delete the closing Node if present
		if(_node->parent && _node->parent->child == _node)
			_node->parent->child = _node->next;
		if(_node->prev)
			_node->prev->next = _node->next;
		if(_node->next)
			_node->next->prev = _node->prev;
		_node->parent = 0L;
		_nodeNext = _node->next;
		_node->next = 0L;
		_node->prev = 0L;
		_node->child = 0L;
		modif.type = undoRedo::NodeRemoved;
		modif.location = kafkaCommon::getLocation(_node);
		modif.node = _node;
		modif.childsNumber = 0;
		modif.childsNumber2 = 0;
		modifs.NodeModifList.append(modif);
		//delete _node;
		_node = _nodeNext;
	}

	if(_node && _node->tag->type == Tag::Empty)
	{
		//delete the next empty tag if present
		modif.location = kafkaCommon::getLocation(_node);
		if(_node->parent && _node->parent->child == _node)
			_node->parent->child = _node->next;
		if(_node->prev)
			_node->prev->next = _node->next;
		if(_node->next)
			_node->next->prev = _node->prev;
		_node->parent = 0L;
		_nodeNext = _node->next;
		_node->prev = 0L;
		_node->next = 0L;
		_node->child = 0L;
		modif.type = undoRedo::NodeRemoved;
		modif.node = _node;
		modif.childsNumber = 0;
		modif.childsNumber2 = 0;
		modifs.NodeModifList.append(modif);
		_node = _nodeNext;
	}

	if(_node && _node->prev && (_node->tag->type == Tag::Empty || _node->tag->type == Tag::Text) &&
		(_node->prev->tag->type == Tag::Empty || _node->prev->tag->type == Tag::Text))
	{
		//merge two consecutive emtpy/text Nodes
		_tag = new Tag(*(_node->prev->tag));
		modif.type = undoRedo::NodeModified;
		modif.location = kafkaCommon::getLocation(_node->prev);
		modif.node = 0L;
		modif.tag = _tag;
		modifs.NodeModifList.append(modif);
		_node->prev->tag->setStr(_node->prev->tag->tagStr() + _node->tag->tagStr());
		//_node->prev->tag->cleanStrBuilt = false;
		if(!(_node->tag->type == Tag::Empty && _node->prev->tag->type == Tag::Empty))
			_node->prev->tag->type = Tag::Text;
		modif.type = undoRedo::NodeRemoved;
		modif.location = kafkaCommon::getLocation(_node);
		if(_node->parent && _node->parent->child == _node)
			_node->parent->child = _node->next;
		if(_node->prev)
			_node->prev->next = _node->next;
		if(_node->next)
			_node->next->prev = _node->prev;
		_node->parent = 0L;
		_node->prev = 0L;
		_node->next = 0L;
		_node->child = 0L;
		modif.childsNumber = 0;
		modif.childsNumber2 = 0;
		modif.node = _node;
		modif.location = kafkaCommon::getLocation(_node);
	}
	_currentDoc->docUndoRedo->addNewModifsSet(modifs, undoRedo::KafkaModif);

#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "WKafkaPart::slotDomNodeDeleted() in " << t.elapsed() <<
		" ms only!" << endl;
#endif
#ifdef HEAVY_DEBUG
	kafkaCommon::coutTree(baseNode, 2);
#endif

}

bool WKafkaPart::buildKafkaNodeFromNode(Node *node, bool insertNode)
{
#ifdef LIGHT_DEBUG
	if(node)
		kdDebug(25001)<< "WKafkaPart::buildKafkaNodeFromNode() " << endl;
#endif
	DOM::Node newNode, newNode2, attr, nextNode, parentNode;
	QString str, nodeValue;
	Node *n;
	int i;

	if(node->tag->type == Tag::XmlTag || (node->tag->type == Tag::Text && !node->insideSpecial))
	{
		if(!node->tag->single && node->next && node->next->tag->type != Tag::XmlTagEnd)
		{
			//TODO: ERROR missing closing tags, set the kafka behavior according to this
#ifdef LIGHT_DEBUG
			kdDebug(25001)<< "WKafkaPart::buildKafkaNodeFromNode() - Node missing closing Tag" <<
				endl;
#endif
		}
		str = node->tag->name.lower();
		if(!node->parent)
		{//FIXME:html, head and body are HTML-specific tag, for others DTDs it might result to some pbs.
			if(str == "html")
			{
				if(!html.isNull())//delete the empty Node
					disconnectDomNodeFromQuantaNode(html);
				newNode = html;
				insertNode = false;
			}
			else if(str == "body")
			{
				if(!body.isNull())
					disconnectDomNodeFromQuantaNode(body);
				newNode = body;
				insertNode = false;
			}
			else if(str == "head")
			{
				if(!head.isNull())
					disconnectDomNodeFromQuantaNode(head);
				newNode = head;
				insertNode = false;
			}
			else
			{
				if(node->tag->type == Tag::Text)
				{
					newNode = kafkaCommon::createTextDomNode("", _kafkaPart->document());
				}
				else
				{
					newNode = kafkaCommon::createDomNode(node->tag->name, _kafkaPart->document());
				}
			}
		}
		else if(str == "body" && node->parent && !node->parent->parent)
		{
			if(!body.isNull())
				disconnectDomNodeFromQuantaNode(body);
			newNode = body;
			insertNode = false;
		}
		else if(str == "head" && node->parent && !node->parent->parent)
		{
			if(!head.isNull())
				disconnectDomNodeFromQuantaNode(head);
			newNode = head;
			insertNode = false;
		}
	/**	else if(node->parent->tag->str == "html")*/
		else
		{
			if(node->tag->type == Tag::Text)
			{
				newNode = kafkaCommon::createTextDomNode("", _kafkaPart->document());
			}
			else
			{
				newNode = kafkaCommon::createDomNode(node->tag->name, _kafkaPart->document());
			}
		}

		if(newNode.isNull())
			return true;

		connectDomNodeToQuantaNode(newNode, node);
		if(node->tag->type == Tag::Text)
		{
			nodeValue = node->tag->tagStr();
			nodeValue = getDecodedText(nodeValue);
			newNode.setNodeValue(nodeValue);
		}

		for(i = 0; i < node->tag->attrCount(); i++)
		{
			attr = kafkaCommon::createDomNodeAttribute(node->tag->attribute(i), _kafkaPart->document());
			if(!attr.isNull())
			{
				attr.setNodeValue(node->tag->attributeValue(i));
				kafkaCommon::insertDomNodeAttribute(newNode, attr);
			}
		}

		if(node->next && node->next->tag && node->next->tag->name ==
			("/" + node->tag->name))
		{
			//DEPRECATED, NO USE TO CHANGE THE above check
			node->_closingNode = node->next;
		}

		if(insertNode)
		{
			node->_rootNode = newNode;
			n = node;
			while(n->next)
			{
				n = n->next;
				if(!n->_rootNode.isNull())
				{
					nextNode = n->_rootNode;
						break;
				}
			}
			if(node->parent && !node->parent->_leafNode.isNull())
				parentNode = node->parent->_leafNode;
			else if(node->parent && node->parent->_leafNode.isNull())
			{
				//the parent tag was invalid and khtml refuse to insert it
				//so impossible to inser the current node
				disconnectDomNodeFromQuantaNode(newNode);
				node->_rootNode = DOM::Node();
				return false;
			}
			else
				parentNode = body;

			if(nextNode.isNull())
			{
				if(!kafkaCommon::insertDomNode(newNode, parentNode))
				{
					disconnectDomNodeFromQuantaNode(newNode);
					node->_rootNode = DOM::Node();
					return false;
				}
			}
			else
			{
				if(!kafkaCommon::insertDomNode(newNode, parentNode, nextNode))
				{
					disconnectDomNodeFromQuantaNode(newNode);
					node->_rootNode = DOM::Node();
					return false;
				}
			}
			node->_leafNode = newNode;
			mainEnhancer->enhanceNode(node, parentNode, nextNode);
		}
		else
		{
			node->_rootNode = newNode;
			node->_leafNode = newNode;
		}
	}
	else
	{
		if(node->parent && !node->parent->_leafNode.isNull())
			parentNode = node->parent->_leafNode;
		else
			parentNode = body;
		n = node;
		while(n->next)
		{
			n = n->next;
			if(!n->_rootNode.isNull())
			{
				nextNode = n->_rootNode;
				break;
			}
		}
		mainEnhancer->enhanceNode(node, parentNode, nextNode);
	}
	return true;
}

void WKafkaPart::buildNodeFromKafkaNode(Node *_node, DOM::Node _domNode)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "void WKafkaPart::buildNodeFromKafkaNode() - Node* DOM::Node" << endl;
#endif
	long i;

	if(!_node) return;

	_node->tag->cleanStrBuilt = false;
	if(_domNode.nodeType() == DOM::Node::TEXT_NODE)
	{
		_node->tag->setStr(_domNode.nodeValue().string());
	}
	else
	{
		while(_node->tag->attrCount())
			_node->tag->deleteAttribute(0);
		for(i = 0; (unsigned)i < _domNode.attributes().length(); i++)
		{
			TagAttr attr;
			attr.name = _domNode.attributes().item(i).nodeName().string();
			attr.value = _domNode.attributes().item(i).nodeValue().string();
			attr.quoted = true;
			_node->tag->addAttribute(attr);
		}
	}
}

Node * WKafkaPart::buildNodeFromKafkaNode(DOM::Node _domNode, Node *_nodeParent,
	Node *_beginNode, int beginOffset, Node *_endNode, int endOffset, NodeModifsSet &modifs)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "Node* WKafkaPart::buildNodeFromKafkaNode() - DOM::Node 2xNode* int: " <<
		beginOffset << " Node* int: " << endOffset << " NodeModifsSet " << endl;
#endif
	Node *_node, *_nodeXmlEnd = 0L, *_emptyNode, *n = 0L;
	Tag *_tag, *_tagEnd, *_tagEmptyNode;
	NodeModif modif;

	if(_domNode.isNull())
	{
		kdDebug(25001)<< "Node* WKafkaPart::buildNodeFromKafkaNode(DOM::Node, 2xNode*)" <<
			" *ERROR* - empty _domNode"<< endl;
	}

	_node = new Node(_nodeParent);
	connectDomNodeToQuantaNode(_domNode, _node);
	_node->_rootNode = _domNode;
	_node->_leafNode = _domNode;

	//split _beginNode
	if(beginOffset != 0 && beginOffset != -1)
	{
		//TODO
	}
	//split _endNode
	if(endOffset != -1)
	{
		//TODO
	}

	//if we want, for example, to add text to a body Node which is not in the Tree
	//cf loadDocument, use 0L for the node's parent
	if(_nodeParent->tag->notInTree)
		_nodeParent = 0L;

	_node->parent = _nodeParent;
	if(_beginNode)
	{
		if(_beginNode->prev)
			_beginNode->prev->next = _node;
		_node->prev = _beginNode->prev;
		_beginNode->parent = _node;
		_node->child = _beginNode;
	}
	else
	{
		if(_nodeParent)
			n = _nodeParent->child;
		else
			n = baseNode;
		if(!n)
		{
			if(_nodeParent)
				_nodeParent->child = _node;
			else
				baseNode = _node;
			_node->prev = 0L;
			_node->next = 0L;
		}
		else
		{
			while(n->next)
				n = n->next;
			n->next = _node;
			_node->prev = n;
			_node->next = 0L;
		}
	}
	_tag = new Tag();
	_node->tag = _tag;
	_tag->setWrite(_currentDoc);
	_tag->cleanStrBuilt = false;
	modif.type = undoRedo::NodeAdded;
	modif.node = 0L;
	modif.childsNumber = 0;
	modif.childsNumber2 = 0;
	modif.location = kafkaCommon::getLocation(_node);
	modifs.NodeModifList.append(modif);
	if(_domNode.nodeType() == DOM::Node::TEXT_NODE)
	{
		_tag->type = Tag::Text;
		_tag->single = true;
		_tag->dtd = _currentDoc->defaultDTD();
		buildNodeFromKafkaNode(_node, _domNode);
		if(_endNode && !(_endNode == _beginNode && beginOffset == endOffset))
		{
			//TODO
		}
	}
	else
	{
		//it is an element
		_tag->type = Tag::XmlTag;
		_tag->name = _domNode.nodeName().string();
		_tag->single = QuantaCommon::isSingleTag(_currentDoc->defaultDTD()->name,
			 _tag->name);
		_tag->dtd = _currentDoc->defaultDTD();
		buildNodeFromKafkaNode(_node, _domNode);

		if(!_tag->single)
		{
			_nodeXmlEnd = new Node(_nodeParent);
			_node->_closingNode = _nodeXmlEnd;
			_nodeXmlEnd->prev = _node;
			_nodeXmlEnd->next = _node->next;
			if(_node->next)
				_node->next->prev = _nodeXmlEnd;
			_node->next = _nodeXmlEnd;
			modif.location = kafkaCommon::getLocation(_nodeXmlEnd);
			modifs.NodeModifList.append(modif);

			_tagEnd = new Tag();
			_nodeXmlEnd->tag = _tagEnd;
			_nodeXmlEnd->closesPrevious = true;
			_tagEnd->cleanStrBuilt = false;
			_tagEnd->setWrite(_currentDoc);
			_tagEnd->type = Tag::XmlTagEnd;
			_tagEnd->single = true;
			_tagEnd->dtd = _currentDoc->defaultDTD();
			_tagEnd->name = "/" + _tag->name;
		}
	}

	if(_node->prev)
		n = _node->prev;
	else if(_node->parent)
		n = _node->parent;
	if(n && _node->tag->type != Tag::Text && (n->tag->type == Tag::XmlTag || n->tag->type == Tag::XmlTagEnd ||
		n->tag->type == Tag::Comment || n->tag->type == Tag::CSS || n->tag->type == Tag::ScriptTag))
	{
		_emptyNode = new Node(n->parent);
		_tagEmptyNode = new Tag();
		_emptyNode->tag = _tagEmptyNode;
		_tagEmptyNode->cleanStrBuilt = false;
		_tagEmptyNode->setTagPosition(-2, -2, -2, -2);//-1 is valid
		_tagEmptyNode->setWrite(_currentDoc);
		_tagEmptyNode->type = Tag::Empty;
		_tagEmptyNode->single = true;
		_tagEmptyNode->dtd = _currentDoc->defaultDTD();
		if(_node->prev)
			_node->prev->next = _emptyNode;
		_emptyNode->prev = _node->prev;
		_emptyNode->next = _node;
		_node->prev = _emptyNode;
		if(_node->parent && _node->parent->child == _node)
			_node->parent->child = _emptyNode;
		_emptyNode->parent = _node->parent;
		modif.location = kafkaCommon::getLocation(_emptyNode);
		modifs.NodeModifList.append(modif);
	}
	if(_tag->single)
		n = _node->nextSibling();
	else
		n = _nodeXmlEnd->nextSibling();
	if(n && _node->tag->type != Tag::Text && (n->tag->type == Tag::XmlTag || n->tag->type == Tag::XmlTagEnd ||
		n->tag->type == Tag::Comment || n->tag->type == Tag::CSS || n->tag->type == Tag::ScriptTag))
	{
		_emptyNode = new Node(n->parent);
		_tagEmptyNode = new Tag();
		_emptyNode->tag = _tagEmptyNode;
		_tagEmptyNode->setTagPosition(-2, -2, -2, -2);//-1 is valid
		_tagEmptyNode->cleanStrBuilt = false;
		_tagEmptyNode->setWrite(_currentDoc);
		_tagEmptyNode->type = Tag::Empty;
		_tagEmptyNode->single = true;
		_tagEmptyNode->dtd = _currentDoc->defaultDTD();
		if(!_tag->single)
		{
			if(_nodeXmlEnd->next)
				_nodeXmlEnd->next->prev = _emptyNode;
			_emptyNode->next = _nodeXmlEnd->next;
			_emptyNode->prev = _nodeXmlEnd;
			_nodeXmlEnd->next = _emptyNode;
			_emptyNode->parent = _nodeXmlEnd->parent;
		}
		else
		{
			if(_node->next)
				_node->next->prev = _emptyNode;
			_emptyNode->next = _node->next;
			_emptyNode->prev = _node;
			_node->next = _emptyNode;
			_emptyNode->parent = _node->parent;
		}
		modif.location = kafkaCommon::getLocation(_emptyNode);
		modifs.NodeModifList.append(modif);
	}
	return _node;
}

QString WKafkaPart::getDecodedChar(const QString &encodedChar)
{
	QMap<QString, QString>::Iterator it = encodedChars.find(encodedChar);
	if(it == encodedChars.end())
		return "";
	return it.data();
}

QString WKafkaPart::getEncodedChar(const QString &decodedChar, const QString &previousDecodedChar)
{
	if(decodedChar[0].isSpace() && !previousDecodedChar[0].isSpace())
		return " ";
	else if(decodedChar[0].isSpace())
		//FIXME:for an unknown reason, by default it return &iquest; instead of &nbsp;
		return "&nbsp;";
	QMap<QString, QString>::Iterator it = decodedChars.find(decodedChar);
	if(it == decodedChars.end())
		return decodedChar;

	return it.data();
	}

QString WKafkaPart::getDecodedText(const QString &a_encodedText)
{
        QString encodedText = a_encodedText;
	QString decodedChar;
	int i, j;
#ifdef LIGHT_DEBUG
	QString oldEncodedText = encodedText;
#endif

	i = -1;
	while((unsigned)++i < encodedText.length())
	{
		if(encodedText[i].isSpace())
		{
			encodedText.remove(i, 1);
			encodedText.insert(i, " ");
			while((unsigned)++i < encodedText.length() && encodedText[i].isSpace())
			{
				encodedText.remove(i, 1);
				i--;
			}
		}
	}
	i = -1;
	while((unsigned)++i < encodedText.length())
	{
		if(QString(encodedText[i]) == "&")
		{
			j = i;
			while((unsigned)++i < encodedText.length() &&
				QString(encodedText[i]) != ";")
				{}
			decodedChar = getDecodedChar(encodedText.mid(j, i - j + 1));
			encodedText.remove(j, i - j + 1);
			//TODO:set a special behavior if the encoded symbol doesn't exist
			encodedText.insert(j, decodedChar);
			i = j + decodedChar.length() - 1;
		}
	}
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "WKafkaPart::getDecodedText() - " << oldEncodedText << " -> " <<
		encodedText << endl;
#endif
	return encodedText;
	//return KGlobal::charsets()->resolveEntities(encodedText); =>nice but not sufficient
}

QString WKafkaPart::getEncodedText(const QString &a_decodedText, int bLine, int bCol, int &eLine, int &eCol)
{
        QString decodedText = a_decodedText;
	QString Encodedchar;
	QString decodedChar, previousDecodedChar;
#ifdef LIGHT_DEBUG
	QString oldDecodedText = decodedText;
#endif
	int i;
	int _bLine = bLine, _bCol = bCol;

	i = -1;
	while((unsigned)++i < decodedText.length())
	{
		previousDecodedChar = decodedChar;
		decodedChar = QString(decodedText[i]);
		if(decodedText[i].isSpace() && !previousDecodedChar[0].isSpace()
			&& bCol > EDITOR_MAX_COL)
		{
			Encodedchar = "\n";
			bCol = 0;
			bLine++;
		}
		else
		{
			Encodedchar = getEncodedChar(QString(decodedText[i]),
				(i>=1)?previousDecodedChar:QString(""));
			bCol += Encodedchar.length();
		}
		decodedText.remove(i,1);
		decodedText.insert(i, Encodedchar);
		i += Encodedchar.length() - 1;
	}
	eLine = bLine;
	eCol = bCol - 1;
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "WKafkaPart::getEncodedText() - " << oldDecodedText << " -> " << decodedText <<
		" - " << _bLine << ":" << _bCol << " - " << eLine << ":" << eCol << endl;
#endif
	return decodedText;
}

QString WKafkaPart::getEncodedText(const QString& decodedText)
{
	int a = 0, b = 0, c, d;
	return getEncodedText(decodedText, a, b, c, d);
}

QString WKafkaPart::generateCodeFromNode(Node *_node, int bLine, int bCol, int &eLine, int &eCol)
{
	QString text, _char;
	int j = 0;

	if(!_node) return "";

	if(_node->tag->type == Tag::XmlTag)
	{
		text = "<" + QuantaCommon::tagCase(_node->tag->name);
		bCol += _node->tag->name.length() + 1;
		for(j = 0; j < _node->tag->attrCount(); j++)
		{
			text += " ";
			text += _node->tag->attribute(j);
			text += "=";
			if(_node->tag->isQuotedAttribute(j))
			{
				text += qConfig.attrValueQuotation;
				bCol++;
			}
			text += _node->tag->attributeValue(j);
			if(_node->tag->isQuotedAttribute(j))
			{
				text += qConfig.attrValueQuotation;
				bCol++;
			}
			bCol += _node->tag->attribute(j).length() + _node->tag->attributeValue(j).length() + 2;
		}
		if ( _node->tag->dtd->singleTagStyle == "xml" &&
			(_node->tag->single || (!qConfig.closeOptionalTags &&
			QuantaCommon::isOptionalTag(_node->tag->dtd->name, _node->tag->name))))
		{
			text += "/";
			bCol++;
		}
		text += ">";
		eCol = bCol;
		eLine = bLine;
	}
	else if(_node->tag->type == Tag::XmlTagEnd)
	{
		text = "<" + QuantaCommon::tagCase(_node->tag->name) + ">";
		bCol += text.length();
		eCol = bCol - 1;
		eLine = bLine;
	}
	else if(_node->tag->type == Tag::Text)
	{
		text = getEncodedText(_node->tag->tagStr(), bLine, bCol, eLine, eCol);
		/** Can't use KGlobal::charsets()->toEntity() :
		 * It translate all chars into entities! */
	}
	else if(_node->tag->type == Tag::ScriptTag)
	{
		//WARNING : HTML SPECIFIC
		if(_node->tag->name.contains("style"));
		{
			text = "<" + QuantaCommon::tagCase("style") + ">";
			bCol += text.length();
			eCol = bCol - 1;
			eLine = bLine;
		}
	}
	/**else if(_node->tag->type == Tag::Empty)
	{
		text = " ";
		eCol = bCol;
		eLine = bLine;
	}*/
	else
	{
		//default behavior : return node->tag->tagStr()
		text = _node->tag->tagStr();
		kafkaCommon::getEndPosition(text, bLine, bCol, eLine, eCol);
	}
	return text;
}

Node *WKafkaPart::searchCorrespondingNode(DOM::Node _domNode)
{
	if(_domNode.isNull() || _domNode.nodeName().string() == "#document")
	{
		kdDebug(25001)<< "WKafkaPart::searchCorrespondingNode() - Bad Node given" <<
			endl;
		return 0L;
	}
	kNodeAttrs *props = domNodeProps[_domNode.handle()];
	if(props == 0)
	{
		kdDebug(25001)<< "WKafkaPart::searchCorrespondingNode() -" <<
			" Corresponding Node not Found!!!" << endl;
		return 0L;
	}
	return props->getNode();
	//return domNodeToNode[_domNode.handle()];
}

kNodeAttrs *WKafkaPart::getAttrs(DOM::Node _domNode)
{
	return domNodeProps[_domNode.handle()];
}

void WKafkaPart::slotdomNodeNewCursorPos(DOM::Node, int)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "WKafkaPart::slotdomNodeNewCursorPos()" << endl;
#endif
	//int line, col;
	//dont calculate cursor pos until the next view update
	//getQuantaCursorPosition(line, col);
	//emit newCursorPosition(line, col);
}

void WKafkaPart::translateKafkaIntoQuantaCursorPosition(DOM::Node _currentDomNode, int offset, int &line, int &col)
{
	Node *_currentNode;
	QString decodedText, encodedChar, currentLine, currentChar;
	QChar curChar, oldChar;
	int currentOffset;
	int curLine, curCol, endLine, endCol;
	bool waitForSpace = false, found = false;

	//_kafkaPart->getCurrentNode(_currentDomNode, offset);
	currentOffset = offset;
	if(_currentDomNode.isNull())
	{
		kdDebug(25001)<< "WKafkaPart::getQuantaCursorPosition() - DOM::Node not found!" << endl;
		line = 0;
		col = 0;
		return;
	}
	_currentNode = searchCorrespondingNode(_currentDomNode);
	if(!_currentNode)
	{
		kdDebug(25001)<< "WKafkaPart::getQuantaCursorPosition() - Node not found!" << endl;
		line = 0;
		col = 0;
		return;
	}
	decodedText = _currentDomNode.nodeValue().string();
	_currentNode->tag->beginPos(curLine, curCol);
	_currentNode->tag->endPos(endLine, endCol);
	currentLine = _currentDoc->editIf->textLine(curLine);
	if(curLine < endLine)
		currentLine += " ";//remplace the \n

	if(offset == -1)
	{
		_currentDoc->selectionIf->setSelection(curLine, curCol, endLine, endCol + 1);
		line = endLine;
		col = endCol + 1;
		return;
	}
#ifdef HEAVY_DEBUG
//	kdDebug(25001)<< "WKafkaPart::getQuantaCursorPosition() - decodedText:"<< decodedText << ":" << endl;
#endif
	if(_currentNode->tag->type == Tag::Text)
	{
		while(currentOffset > 0)
		{
			curChar = decodedText[offset - currentOffset];
#ifdef HEAVY_DEBUG
//			kdDebug(24000)<< "CurChar:" << QString(curChar) << ": oldChar:" << QString(oldChar) << endl;
#endif
			encodedChar = getEncodedChar(QString(curChar), QString(oldChar));
			if(encodedChar == "&nbsp;")
				encodedChar = " ";
			found = false;
			waitForSpace = false;
			while(!found)
			{
#ifdef HEAVY_DEBUG
//				kdDebug(25001)<< "look 4 :" << encodedChar << ": found :" <<
//					currentLine.mid(curCol, encodedChar.length()) << endl;
//				kdDebug(25001)<< "pos " << curLine << ":" << curCol << endl;
//				kdDebug(25001)<< "endPos " << endLine << ":" << endCol << endl;
#endif
				if(encodedChar != " " && encodedChar == currentLine.mid(curCol, encodedChar.length()))
					found = true;
				else if(encodedChar == " " && currentLine.mid(curCol, 1).at(0).isSpace())
					waitForSpace = true;
				else if(waitForSpace)
					break;//no more spaces
				else if(encodedChar == " " && currentLine.mid(curCol, 6) == "&nbsp;")
				{
					encodedChar = "&nbsp;";
					found = true;
				}
				if((curCol > endCol && curLine == endLine) || curLine > endLine ||
					encodedChar.length() == 0)
				{
					//encodedChar.length() == 0 =>prevent infinite loop
					curLine = endLine;
					curCol = endCol + 1;
					currentOffset = 0;
					break;
				}
				else if((curCol + encodedChar.length()) >= currentLine.length())
				{
					curLine++;
					curCol = 0;
					currentLine = _currentDoc->editIf->textLine(curLine);
					if(curLine < endLine)
						currentLine += " ";//remplace the \n
				}
				else
				{
						curCol += encodedChar.length();
				}
			}
			oldChar = curChar;
			currentOffset--;
		}
	}

	_currentDoc->selectionIf->setSelection(curLine, curCol, curLine, curCol);
	line = curLine;
	col = curCol;
#ifdef LIGHT_DEBUG
	kdDebug(25001)<<"WKafkaPart::getQuantaCursorPosition() - " << line << ":" << col << endl;
#endif
	return;
}

void WKafkaPart::translateQuantaIntoKafkaCursorPosition(uint curLine, uint curCol, DOM::Node &domNode, int &offset)
{
	Node *node;
	int bCol, bLine, eCol, eLine, col, line;
	QString curChar, decodedChar, currentLine;
	bool lookForEntity, lookForSpaces, found;

	node = baseNode;
	while(node)
	{
		node->tag->beginPos(bLine, bCol);
		node->tag->endPos(eLine, eCol);
		if((bLine < (signed)curLine || (bLine == (signed)curLine && bCol <= (signed)curCol)) &&
			(eLine > (signed)curLine || (eLine == (signed)curLine && eCol >= (signed)curCol)))
		{
			if(bLine == (signed)curLine && bCol == (signed)curCol && node->tag->type != Tag::Text &&
				node->previousSibling() && node->previousSibling()->tag->type == Tag::Text)
			{
				//if we are at the end of a text which is recognized as the beginning of the next tag
				node = node->previousSibling();
				//we can go directly to the last offset
				domNode = node->_rootNode;
				if(domNode.isNull())
				{
					offset = 0;
					kdDebug(25001)<< "WKafkaPart::getKafkaCursorPosition() - ERROR DOM::Node not found"
						<< endl;
					return;
				}
				offset = domNode.nodeValue().string().length();
#ifdef LIGHT_DEBUG
				kdDebug(25001)<< "WKafkaPart::getKafkaCursorPosition() - " <<
					domNode.nodeName().string() << ":" << offset << endl;
#endif
				return;
			}
			col = bCol;
			line = bLine;
			break;
		}
		node = node->nextSibling();

	}
	if(!node)
	{
		kdDebug(25001)<< "WKafkaPart::getKafkaCursorPosition() - ERROR node not found" << endl;
		return;
	}
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "WKafkaPart::getKafkaCursorPosition() - node :" << node->tag->name << ": type:" <<
		node->tag->type << ": pos " << bLine << ":" << bCol << " - " << eLine << ":" << eCol << endl;
#endif

	domNode = node->_rootNode;
	offset = 0;
	if(!domNode.isNull() && domNode.nodeType() == DOM::Node::TEXT_NODE)
	{
		currentLine = _currentDoc->editIf->textLine(line);
		if(line < eLine)
			currentLine += " ";//remplace the \n
		while(line < (signed)curLine || (col < (signed)curCol && line == (signed)curLine))
		{
			lookForEntity = false;
			lookForSpaces = false;
			curChar = currentLine.mid(col, 1);
			if(curChar == "&")
				lookForEntity = true;
			else if(curChar[0].isSpace())
				lookForSpaces = true;
			found = false;
			while(!found)
			{
				if((lookForEntity && curChar == ";") ||
					!(lookForSpaces || lookForEntity))
					found = true;
				else if(lookForSpaces && !curChar[0].isSpace())//curChar != " ")
					break;
				if((col + 1) >= (signed)currentLine.length())
				{
					line++;
					col = 0;
					currentLine = _currentDoc->editIf->textLine(line);
					if(line < eLine)
						currentLine += " ";//remplace the \n
				}
				else col++;
				curChar = currentLine.mid(col, 1);
			}
#ifdef HEAVY_DEBUG
			kdDebug(25001)<< "endpos at " << line << ":" << col << endl;
#endif
			offset++;
		}
	}
	else if(!domNode.isNull())
		offset = 0;//shoud we select?
	else
		_kafkaPart->finishedLoading();//set the cursor in the first text
#ifdef LIGHT_DEBUG
	if(!domNode.isNull())
		kdDebug(25001)<< "WKafkaPart::getKafkaCursorPosition() - " << domNode.nodeName().string() <<
			":" << offset << endl;
	else
		kdDebug(25001)<< "WKafkaPart::getKafkaCursorPosition() - NULL domNode" << endl;
#endif
}

void WKafkaPart::connectDomNodeToQuantaNode(DOM::Node _domNode, Node *_node)
{
	QString name;
	kNodeAttrs *props;

	if(_domNode.isNull())
	{
		kdDebug(25001)<< "WKafkaPart::connectDomNodeToQuantaNode() *ERROR*" << endl;
				return;
	}
	/**qtag = QuantaCommon::tagFromDTD(_currentDoc->defaultDTD(),
		_domNode.nodeName().string());*/
	props = new kNodeAttrs();
	name = _domNode.nodeName().string().lower();

	if(_domNode.nodeType() == DOM::Node::TEXT_NODE)
	{
		props->setCBDeleted(true);
		props->setCBModified(true);
		props->setCHCursorFocus(kNodeAttrs::textNode);
		props->setCCEnter(true);
	}
	else if(name == "abbr" || name == "acronym" || name == "address" || name == "b" || name == "bdo" ||
		name == "big" || name == "center" || name == "cite" || name == "code" || name == "dd" ||
		name == "del" || name == "dfn" || name == "dir" || name == "dl" || name == "dt" ||
		name == "em" || name == "fieldset" || name == "font" || name == "i" || name == "iframe" ||
		name == "ins" || name == "kbd" || name == "label" || name == "legend" || name == "menu" ||
		name == "noframes" || name == "pre" || name == "s" || name == "samp" ||
		name == "small" || name == "span" || name == "strike" || name == "strong" || name == "sub" ||
		name == "sup" || name == "tt" || name == "u" || name == "var" || name == "a" ||
		name == "blockquote" || name == "div" || name == "dl" || name == "dt" ||
		name == "em" || name == "form" || name == "h1" || name == "h2" || name == "h3" ||
		name == "h4" || name == "h5" || name == "h6" || name == "ins" || name == "li" ||
		name == "ol" || name == "p" || name == "q" ||  name == "tt" || name == "ul")
	{
		//TODO:HARDCODED HTML VALUE, remove with the new editing capabilities
		//It was in .tag files but i told too quickly to remove them :p
		props->setCBDeleted(true);
		props->setCBModified(true);
		props->setCHCursorFocus(kNodeAttrs::no);
		props->setCCEnter(true);
	}
	else if(name == "applet" || name == "button" || name == "img" || name == "map" || name == "object" ||
		name == "hr" || name == "input" || name == "select" || name == "table" || name == "textarea" ||
		 name == "br")
	{
		props->setCBDeleted(true);
		props->setCBModified(true);
		props->setCHCursorFocus(kNodeAttrs::singleNodeAndItself);
		props->setCCEnter(false);
	}
	else if(name == "basefont" || name == "location" || name == "fieldset" || name == "noscript" ||
		name == "script")
	{
		props->setCBDeleted(true);
		props->setCBModified(true);
		props->setCHCursorFocus(kNodeAttrs::no);
		props->setCCEnter(false);
	}
	else if(name == "caption" || name == "frame" || name == "frameset" || name == "isindex" ||
		name == "optgroup" || name == "param" || name == "title" || name == "area" || name == "base" ||
		name == "body" || name == "col" || name == "colgroup" || name == "head" || name == "html" ||
		name == "link" || name == "meta" || name == "option" || name == "style" || name == "tbody" ||
		name == "td" || name == "tfoot" || name == "th" || name == "thead" || name == "tr")
	{
		props->setCBDeleted(false);
		props->setCBModified(false);
		props->setCHCursorFocus(kNodeAttrs::no);
		props->setCCEnter(false);
	}
	else
	{
		kdDebug(25001)<< "WKafkaPart::connectDomNodeToQuantaNode () - " <<
			"No QTag found! Setting default parameters..." << endl;
		props->setCBDeleted(false);
		props->setCBModified(false);
		props->setCHCursorFocus(kNodeAttrs::no);
		props->setCCEnter(false);
	}
#ifdef HEAVY_DEBUG
	kdDebug(25001)<< "WKafkaPart::connectDomNodeToQuantaNode() - tag name :" <<
		name << " canBeDeleted:" << props->cbDel() << " canBeModified:" <<
		props->cbMod() << " canHaveCursorFocus:" << props->chCurFoc() <<
		" cursorCanEnter:" << props->ccanEnter() << endl;
#endif
	props->setNode(_node);
	domNodeProps.insert(_domNode.handle(), props);
}

void WKafkaPart::disconnectDomNodeFromQuantaNode(DOM::Node _domNode)
{
	if(_domNode.isNull())
		return;
	domNodeProps.remove(_domNode.handle());
}

void WKafkaPart::disconnectAllDomNodes()
{
	domNodeProps.clear();
}

