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
#include <kdebug.h>
#include <kstandarddirs.h>

#include <qregexp.h>
#include <qfile.h>
#include <qtextstream.h>

#include "../../document.h"
#include "../../parser/node.h"
#include "../../parser/parser.h"
#include "nodeproperties.h"

#include "wkafkapart.moc"

WKafkaPart::WKafkaPart(QWidget *parent, QWidget *widgetParent, const char *name)
{
	_kafkaPart = new KafkaHTMLPart(parent, widgetParent, name);
	_kafkaPart->showDomTree();
	_parser = new Parser();
	_docLoaded = false;
	_rootNode = 0L;
	_currentDoc = 0L;
	QFile file( locate("appdata","chars") );
	QString tmp;
	if ( file.open(IO_ReadOnly) )
	{
		QTextStream t( &file );        // use a text stream
		while ( !t.eof() )
		{
			tmp = t.readLine();
			int begin = tmp.find("(")+1;
			int length = tmp.find(")") - begin;
			specialChars.insert(tmp.left(1),tmp.mid(begin, length));
			encodedChars.insert(tmp.mid(begin, length), tmp.left(1));
		}
		file.close();
	}

	connect(_kafkaPart, SIGNAL(domNodeCreated(DOM::Node)),
		this, SLOT(slotDomNodeCreated(DOM::Node)));
	connect(_kafkaPart, SIGNAL(domNodeModified(DOM::Node)),
		this, SLOT(slotDomNodeModified(DOM::Node)));
	connect(_kafkaPart, SIGNAL(domNodeIsAboutToBeDeleted(DOM::Node)),
		this, SLOT(slotDomNodeAboutToBeDeleted(DOM::Node)));
}

WKafkaPart::~WKafkaPart()
{

}

void WKafkaPart::loadDocument(Document *doc)
{
	kdDebug(25001) << "WKafkaPart::loadDocument()" << endl;
	if(!_kafkaPart) return;
	if(!doc) return;
	_currentDoc = doc;
	/**TEMPORARY kafkaPart loading
	_kafkaPart->openDocument(_currentDoc->url());*/
	if(_currentDoc->text(0,0,10,10).stripWhiteSpace() == "")
	{//set a minimum Nodes to be able to write if the document is empty(dirty here)
		_kafkaPart->newDocument();
		/**DOM::Node _tmpNode = _kafkaPart->createNode("HTML");
		_kafkaPart->htmlDocument().appendChild(_tmpNode);
		DOM::Node _tmpNode2 = _kafkaPart->createNode("BODY");
		_tmpNode.appendChild(_tmpNode2);
		_tmpNode = _kafkaPart->createNode("TEXT");
		_tmpNode2.appendChild(_tmpNode);*/
		_kafkaPart->finishedLoading();
		return;
	}

	_currentDoc->parseVariables();
	_rootNode = _parser->parse(_currentDoc );

	//load the nodes
	bool goingUp = false;
	Node* _node = _rootNode;
	while(_node)
	{
		kdDebug(25001) << "Node name :" << _node->tag->name.upper() << "; type : "
			<<_node->tag->type << "; tagstr : " << _node->tag->tagStr() << " is opened :"
			<< _node->opened << endl;
		//_DOMnode.appendChild(newNode);
		if(!goingUp)
		{
			switch(_node->tag->type)
			{
				case Tag::Unknown:
				break;

				case Tag::XmlTag:
					synchronizeXMLTag(_node);
				break;

				case Tag::XmlTagEnd:
				break;

				case Tag::Text:
					synchronizeTextTag(_node);
				break;

				case Tag::Comment:
					//TODO:Create little icons for Comments
				break;

				case Tag::CSS:
					//TODO:create little icons for CSS?
				break;

				case Tag::ScriptTag:
					//avoid to enter inside scripts
					goingUp = true;
					//TODO:create little icons
				break;

				case Tag::ScriptStructureBegin:
					//avoid to enter inside scripts
					goingUp = true;
					//TODO:create little icons
				break;

				case Tag::ScriptStructureEnd:
				break;

				case Tag::NeedsParsing:
				break;

				case Tag::Empty:
				break;

				case Tag::Skip:
				break;
			}

		}
		_node = getNextNode(_node, goingUp);
	}
	_kafkaPart->finishedLoading();
	_docLoaded = true;
}

void WKafkaPart::unloadDocument()
{
	if(_rootNode)
		delete _rootNode;
		//clean the kafkapart
	while(_kafkaPart->document().hasChildNodes())
		_kafkaPart->document().removeChild(_kafkaPart->document().firstChild());
	_docLoaded = false;
}

void WKafkaPart::slotUpdateKafkaTree(bool tryToUpdate)
{

}

void WKafkaPart::slotUpdateQuantaTree()
{

}

void WKafkaPart::slotDomNodeCreated(DOM::Node _domNode)
{
	kdDebug(25001)<< "WKafkaPart::slotDomNodeCreated()" << endl;
}

void WKafkaPart::slotDomNodeModified(DOM::Node _domNode)
{
	kdDebug(25001)<< "WKafkaPart::slotDomNodeModified()" << endl;
	Node *_node;
	if(!_rootNode)
	{//we are in the case when a minimal kafka tree has been set
		//createBasicQuantaTree();
		return;
	}
	//first look which Node correspond to this DOM::Node
	_node = searchCorrespondingNode(_domNode);

	if(!_node)
	{//DOM::Node not found, maybe was it created before
		kdDebug(25001)<< "WKafkaPart::slotDomNodeModified() - *ERROR* the corresponding DOM::Node is not found!" << endl;
		return;
	}

	/**if( == DOM::Node::TEXT_NODE)
	{//"parse" the new text : spaces when possible else &nbsp;

	}*/
}

void WKafkaPart::slotDomNodeAboutToBeDeleted(DOM::Node _domNode)
{
	kdDebug(25001)<< "WKafkaPart::slotDomNodeAboutToBeDeleted()" << endl;
}

void WKafkaPart::slotDomNodeGetFocus(DOM::Node _domNode)
{
	Node *_node = searchCorrespondingNode(_domNode);
	if(_node)
	{
		int line, col;
		_node->tag->beginPos(line, col);
		emit newCursorPosition( line, col);
	}
}

void WKafkaPart::synchronizeXMLTag(Node* _node)
{
	DOM::Node newNode, newNode2;
	DOM::Node attr;
	int i;

	if(!_node->tag->single && _node->next && _node->next->opened)
	{
		//TODO: ERROR missing closing tags, set the kafka behavior according to this
		return;
	}

	newNode = _kafkaPart->createNode(_node->tag->name);
	if (newNode.nodeName().string().upper() != _node->tag->name.upper())
		return;

	for(i = 0; i < _node->tag->attrCount(); i++)
	{
		attr = _kafkaPart->htmlDocument().createAttribute(_node->tag->attribute(i));
		if(!attr.isNull())
		{
			attr.setNodeValue(_node->tag->attributeValue(i));
			newNode.attributes().setNamedItem(attr);
			kdDebug(25001)<< "  Attr added : " << _node->tag->attribute(i) <<
			" value : " << _node->tag->attributeValue(i) << endl;
		}
	}

	_node->kafkaAddon = new kNodeProperties();

	if(_node->parent && _node->parent->kafkaAddon && !_node->parent->kafkaAddon->_leafNode.isNull())
	{
		try
		{
			newNode = _node->parent->kafkaAddon->_leafNode.appendChild(newNode);
		} catch(DOM::DOMException e)
		{kdDebug(25001)<< "WKafkart::loadDocument() *ERROR* - code : " << e.code << endl;}
		//_node->parent->kafkaNodeEnd.applyChanges();
		if(newNode.nodeName().string() == "TABLE")
		{
			newNode2 = _kafkaPart->createNode("TBODY");
			try
			{
				newNode2 = newNode.appendChild(newNode2);
			} catch(DOM::DOMException e)
			{kdDebug(25001)<< "WKafkart::loadDocument() *ERROR* - code : " << e.code << endl;}
			_node->kafkaAddon->_leafNode = newNode2;
		}
		else
			_node->kafkaAddon->_leafNode = newNode;

	}
	else//we suppose it is on the top of the tree
	{
		try
		{
			newNode = _kafkaPart->htmlDocument().appendChild(newNode);
		} catch(DOM::DOMException e)
		{kdDebug(25001)<< "WKafkart::loadDocument() *ERROR* - code : " << e.code << endl;}
		kdDebug(25001)<< "WKafkaPart::loadDocument() *WARNING* - adding node to the root node" << endl;
		_node->kafkaAddon->_leafNode = newNode;
	}
	_node->kafkaAddon->_rootNode = newNode;

	if(_node->parent)
	kdDebug(25001)<< "Added DOM::Node : " << _node->kafkaAddon->_rootNode.nodeName().string() <<
		" - Node : " << _node->tag->tagStr() <<
		" - parent DOM::Node : " << _node->parent->kafkaAddon->_rootNode.nodeName().string() <<
		" - parent Node : " << _node->parent->tag->tagStr() << endl;
	else
	kdDebug(25001)<< "Added DOM::Node : " << _node->kafkaAddon->_rootNode.nodeName().string() <<
		" - Node : " << _node->tag->tagStr() << endl;
}

void WKafkaPart::synchronizeTextTag(Node* _node)
{
	DOM::Node newNode;
	QString NodeValue;
	DOM::Node _nextNode;

	newNode = _kafkaPart->createNode("TEXT");
	//remplace all spaces/tab/return into one single space
	NodeValue = _node->tag->tagStr().replace(QRegExp("\\s+"), " ");
	//remplace encoded characters by their real value
	NodeValue = NodeValue.replace(QRegExp("&([^;]+);"), getSpecialChar("\\1") );
	/**if(NodeValue.left(1) == " " || NodeValue.right(1) == " ")
	{//try to remove duplicate spaces between two nodes
		if(NodeValue.left(1) == " ")
		{
			_nextNode = _node->parent->kafkaNodeEnd;
			while(_nextNode != 0)
			{

			}
		}
	}*/
	newNode.setNodeValue(NodeValue);
	if (newNode.isNull())
		return;

	_node->kafkaAddon = new kNodeProperties();

	if(_node->parent && _node->parent->kafkaAddon &&!_node->parent->kafkaAddon->_leafNode.isNull())
	{
		try
		{
			_node->parent->kafkaAddon->_leafNode.appendChild(newNode);
		} catch(DOM::DOMException e)
		{kdDebug(25001)<< "WKafkart::loadDocument() *ERROR* - code : " << e.code << endl;}
		_node->parent->kafkaAddon->_leafNode.applyChanges();
	}
	else//we suppose it is on the top of the tree
	{
		try
		{
			newNode = _kafkaPart->htmlDocument().appendChild(newNode);
		} catch(DOM::DOMException e)
		{kdDebug(25001)<< "WKafkart::loadDocument() *ERROR* - code : " << e.code << endl;}
		kdDebug(25001)<< "WKafkaPart::loadDocument() *WARNING* - adding node to the root node" << endl;
	}
	_node->kafkaAddon->_rootNode = newNode;
	_node->kafkaAddon->_leafNode = newNode;
	kdDebug(25001)<< "Added text DOM::Node : " << newNode.nodeName().string() << endl;
}

QString WKafkaPart::getSpecialChar(QString encodedChar)
{
	QMap<QString, QString>::Iterator it = encodedChars.find(encodedChar);
	if(it == encodedChars.end())
		return "";
	return it.data();
}

QString WKafkaPart::getEncodedChar(QString specialChar)
{
	QMap<QString, QString>::Iterator it = specialChars.find(specialChar);
	if(it == specialChars.end())
		return "";
	return it.data();
	}

Node *WKafkaPart::getNextNode(Node *_node, bool &goingTowardsRootNode)
{
	//goto next node, my favorite part :)
	if(goingTowardsRootNode)
	{
		if(_node->next)
		{
			goingTowardsRootNode = false;
			return _node->next;
		}
		else
			return _node->parent;
	}
	else
	{
		if(_node->child)//temporary
			return _node->child;
		else if(_node->next)
			return _node->next;
		else
		{
			goingTowardsRootNode = true;
			return _node->parent;
		}
	}
}

Node *WKafkaPart::searchCorrespondingNode(DOM::Node _domNode)
{
	bool b = false;
	Node *nextNode = _rootNode;
	while(nextNode)
	{
		if(nextNode->kafkaAddon && nextNode->kafkaAddon->_rootNode == _domNode)
		{
			break;
		}
		nextNode = getNextNode(nextNode, b);
	}
	return nextNode;
}
