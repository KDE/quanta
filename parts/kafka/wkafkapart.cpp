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
#include <ktexteditor/editinterface.h>

#include <qregexp.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qdatetime.h>

#include "../../document.h"
#include "../../resource.h"
#include "../../parser/node.h"
#include "../../parser/parser.h"
#include "../../parser/tag.h"
#include "nodeproperties.h"

#include "wkafkapart.moc"

#define EDITOR_MAX_COL 70

WKafkaPart::WKafkaPart(QWidget *parent, QWidget *widgetParent, const char *name)
:domNodeProps(1021)
{
	_kafkaPart = new KafkaHTMLPart(parent, widgetParent,this, name);
	_kafkaPart->showDomTree();
	_docLoaded = false;
	_rootNode = 0L;
	_currentDoc = 0L;
	domNodeProps.setAutoDelete(true);

	QFile file( locate("appdata","chars") );
	QString tmp;
	if ( file.open(IO_ReadOnly) )
	{
		QTextStream t( &file );        // use a text stream
		while ( !t.eof() )
		{
			tmp = t.readLine();
			int begin = tmp.find("(") + 1;
			if(begin == (-1 + 1)) break;//"(" not found : invalid line
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
	connect(_kafkaPart, SIGNAL(hasFocus(bool)),
		this, SLOT(slotGetFocus(bool)));
}

WKafkaPart::~WKafkaPart()
{
	//delete the empty node linked to the DOM::Node #document
	disconnectDomNodeFromQuantaNode(_kafkaPart->document());
}

void WKafkaPart::loadDocument(Document *doc)
{
	Node *node;
	Tag *tag;
	kdDebug(25001) << "WKafkaPart::loadDocument()" << endl;
	if(!_kafkaPart) return;
	if(!doc) return;
	QTime t;
	t.start();
	_currentDoc = doc;
	/**if(_currentDoc->editIf->text().stripWhiteSpace() == "")
	{//set a minimum Nodes to be able to write if the document is empty
		_kafkaPart->newDocument();
		return;
	}*/
	//create a empty document with a basic tree : HTML, HEAD, BODY
	_kafkaPart->newDocument();
	/** creating and linking an empty node to the root DOM::Node (#document) and to HEAD, HTML, BODY*/
	node = new Node(0L);
	tag = new Tag();
	tag->name = "#document";
	node->tag = tag;
	connectDomNodeToQuantaNode(_kafkaPart->document(), node);
	node = new Node(0L);
	tag = new Tag();
	tag->name = "HTML";
	node->tag = tag;
	connectDomNodeToQuantaNode(_kafkaPart->document().firstChild(), node);
	html = _kafkaPart->document().firstChild();
	node = new Node(0L);
	tag = new Tag();
	tag->name = "HEAD";
	node->tag = tag;
	connectDomNodeToQuantaNode(_kafkaPart->document().firstChild().firstChild(), node);
	head = _kafkaPart->document().firstChild().firstChild();
	node = new Node(0L);
	tag = new Tag();
	tag->name = "BODY";
	node->tag = tag;
	connectDomNodeToQuantaNode(_kafkaPart->document().firstChild().lastChild(), node);
	body = _kafkaPart->document().firstChild().lastChild();

	//TODO:use the current Node tree.
	_rootNode = parser->parse(_currentDoc );

	//load the nodes
	bool goingUp = false;
	Node* _node = _rootNode;
	while(_node)
	{
		/**kdDebug(25001) << "WKafkaPart::loadDocument - Node name :" <<
			_node->tag->name.upper() << "; type : " <<
			_node->tag->type << "; tagstr : " << _node->tag->tagStr() <<
			" is opened :" << _node->opened << endl;*/
		if(!goingUp)
		{
			switch(_node->tag->type)
			{
				case Tag::Unknown:
				break;

				case Tag::XmlTag:
//					synchronizeXMLTag(_node);
					buildKafkaNodeFromNode(_node);
				break;

				case Tag::XmlTagEnd:
				break;

				case Tag::Text:
//					synchronizeTextTag(_node);
					buildKafkaNodeFromNode(_node);
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
	kdDebug(25001)<< "WKafkaPart::loadDocument() in " << t.elapsed() << " ms only!" << endl;
	//coutTree(_rootNode, 2);
}

void WKafkaPart::unloadDocument()
{
	domNodeProps.clear();
	if(_rootNode)
		delete _rootNode;
	_rootNode = 0L;
		//clean the kafkapart
	while(_kafkaPart->document().hasChildNodes())
		_kafkaPart->document().removeChild(_kafkaPart->document().firstChild());
	_docLoaded = false;
}

void WKafkaPart::slotUpdateKafkaTree(bool tryToUpdate)
{
	//temporary
	unloadDocument();
	loadDocument(_currentDoc);
}

void WKafkaPart::slotUpdateQuantaTree()
{

}

void WKafkaPart::slotDomNodeInserted(DOM::Node _domNode, bool insertChilds)
{
	/** TODO: avoid to put all the nodes string in the same line */
	kdDebug(25001)<< "WKafkaPart::slotDomNodeInserted() - node :" <<
		_domNode.nodeName().string() << endl;
	Node *_nodeParent = 0L, *_nodePrev = 0L, *_node = 0L;
	DOM::Node tmpDomNode;
	bool b = false;
	QTime t;

	t.start();
	if(_currentDoc->editIf->text().stripWhiteSpace() == "" && !(_domNode.nodeType() == DOM::Node::TEXT_NODE &&
		_domNode.nodeValue() == ""))
	{//we are in the case when a minimal kafka tree has been set
		_rootNode = buildNodeFromKafkaNode(_kafkaPart->htmlDocument().firstChild(),
			0L, 0L);
		tmpDomNode = _kafkaPart->htmlDocument().firstChild();
		while(!tmpDomNode.isNull())
		{
			tmpDomNode = _kafkaPart->getNextNode(tmpDomNode, b, true, true);
			if(tmpDomNode.isNull() || tmpDomNode.nodeName().string() == "#document")
				break;
			buildNodeFromKafkaNode(tmpDomNode,
				searchCorrespondingNode(tmpDomNode.parentNode()), 0L, true);
		}
		coutTree(_rootNode, 2);
		return;
	}

	_nodeParent = searchCorrespondingNode(_domNode.parentNode());

	if(!_nodeParent)
	{//DOM::Node not found, maybe was it created before
		kdDebug(25001)<< "WKafkaPart::slotDomNodeInserted() - *ERROR* the" <<
			" corresponding DOM::Node is not found!" << endl;
		return;
	}
	if(_domNode.previousSibling() != 0)
	{
		_nodePrev = searchCorrespondingNode(_domNode.previousSibling());
		if(!_nodePrev)
		{
			kdDebug(25001)<< "WKafkaPart::slotDomNodeInserted() - *ERROR2*" <<
			" the corresponding DOM::Node is not found!" << endl;
			return;
		}
	}

	_node = buildNodeFromKafkaNode(_domNode, _nodeParent, _nodePrev);
	if(insertChilds && _domNode.hasChildNodes())
	{/** TODO: chekc if it is working */
		tmpDomNode = _domNode;
		while(!tmpDomNode.isNull())
		{
			tmpDomNode = _kafkaPart->getNextNode(tmpDomNode, b, true,
				true, _domNode);
			if(tmpDomNode.isNull())
				break;
			buildNodeFromKafkaNode(tmpDomNode,
				searchCorrespondingNode(tmpDomNode.parentNode()), 0L, true);
		}
	}

	baseNode = parser->parse(_currentDoc);
	kdDebug(25001)<< "WKafkaPart::slotDomNodeInserted in " << t.elapsed() <<
		" ms only!" << endl;
	coutTree(_rootNode, 2);
}

void WKafkaPart::slotDomNodeModified(DOM::Node _domNode)
{
	kdDebug(25001)<< "WKafkaPart::slotDomNodeModified()" << endl;
	Node *_node = 0L;
	int beginLine, beginCol, endLine, endCol;
	QTime t;

	t.start();
	if(_currentDoc->editIf->text().stripWhiteSpace() == "")
	{//we are in the case when a minimal kafka tree has been set
		DOM::Node domNode;
		bool b = false;
		_rootNode = buildNodeFromKafkaNode(_kafkaPart->htmlDocument().firstChild(),
			0L, 0L);
		domNode = _kafkaPart->htmlDocument().firstChild();
		while(!domNode.isNull())
		{
			domNode = _kafkaPart->getNextNode(domNode, b, true, true);
			if(domNode.isNull() || domNode.nodeName().string() == "#document") break;
			buildNodeFromKafkaNode(domNode,
				searchCorrespondingNode(domNode.parentNode()), 0L, true);
		}
		coutTree(_rootNode, 2);
		return;
	}

	//first look which Node correspond to this DOM::Node
	_node = searchCorrespondingNode(_domNode);

	if(!_node)
	{//DOM::Node not found, maybe was it created before
		kdDebug(25001)<< "WKafkaPart::slotDomNodeModified() - *ERROR* the" <<
			" corresponding DOM::Node is not found!" << endl;
		return;
	}

	_node->tag->beginPos(beginLine, beginCol);
	_node->tag->endPos(endLine, endCol);
	_currentDoc->editIf->removeText(beginLine, beginCol, endLine, endCol + 1);
	buildNodeFromKafkaNode(_node, _domNode);

	coutTree(_rootNode, 2);
	kdDebug(25001)<< "WKafkaPart::slotDomNodeModified in " << t.elapsed() <<
		" ms only!" << endl;
	//I still don't know why it crashs if i don't reparse... :/
	baseNode = parser->parse(_currentDoc);
}

void WKafkaPart::slotDomNodeAboutToBeRemoved(DOM::Node _domNode, bool deleteChilds)
{
	kdDebug(25001)<< "WKafkaPart::slotDomNodeAboutToBeRemoved() node:" <<
		_domNode.nodeName().string() << endl;
	Node *_node = 0L, *_nodePrev = 0L, *_nodeParent = 0L, *_nodeNext = 0L;
	Node *_nodeNextBackup = 0L, *_nodeParentBackup = 0L;
	Node *_nodeNextClosingBackup = 0L, *_nodeParentClosingBackup = 0L;
	Node *_nodeChilds = 0L, *_tmpNode = 0L;
	int startLine, startCol, endLine, endCol;
	int taglenCol, taglenLine, closingTaglenCol, closingTaglenLine;
	int startLine2, startCol2, endLine2, endCol2;
	bool hasClosingNode = false, b;
	QTime t;

	t.start();

	/** FIRST PART: remove the HTML code from the editor*/

	_node = searchCorrespondingNode(_domNode);
	if(!_node)
	{
		kdDebug(25001)<<"WKafkaPart::slotDomNodeAboutToBeRemoved() - *ERROR* the" <<
			" corresponding DOM::Node is not found!" << endl;
		return;
	}
	_nodeNextBackup = _node->next;
	_nodeParentBackup = _node->parent;
	if(!_node->parent)
	{
		kdDebug(25001)<<"WKafkaPart::slotDomNodeAboutToBeRemoved() - *ERROR2* trying" <<
			" to remove the rootNode" << endl;
		return;
	}
	if(_node->_closingNode)
	{
		hasClosingNode = true;
		_nodeNextClosingBackup = _node->_closingNode->next;
		_nodeParentClosingBackup = _node->_closingNode->parent;
		_node->_closingNode->tag->beginPos(startLine2, startCol2);
		_node->_closingNode->tag->endPos(endLine2, endCol2);
		closingTaglenCol = endCol2 - startCol2;
		closingTaglenLine = - endLine2 - startLine2;
		if(!deleteChilds)
			_currentDoc->editIf->removeText(startLine2, startCol2, endLine2,
				endCol2 + 1);
	}
	_node->tag->beginPos(startLine, startCol);
	_node->tag->endPos(endLine, endCol);
	if(hasClosingNode && deleteChilds)
		_currentDoc->editIf->removeText(startLine, startCol, endLine2, endCol2 + 1);
	else
		_currentDoc->editIf->removeText(startLine, startCol, endLine, endCol + 1);
	taglenCol = endCol - startCol;
	taglenLine = endLine - startLine;

	/** SECOND PART: remove the node from the quanta tree and delete it */

	_node->removeAll = false;
	_nodePrev = _node->prev;
	if(hasClosingNode)
	{
		_nodeNext = _node->_closingNode->next;
		_node->_closingNode->removeAll = false;
	}
	else
		_nodeNext = _node->next;
	_nodeParent = _node->parent;
	if(hasClosingNode)
	{
		delete _node->_closingNode;
	}
	if(_node->child && deleteChilds)
	{
		_tmpNode = _node->child;
		b = false;
		while(_tmpNode && _tmpNode != _node)
		{
			if(!_tmpNode->_rootNode.isNull())
				disconnectDomNodeFromQuantaNode(
					_tmpNode->_rootNode);
			if(!_tmpNode->_leafNode.isNull())
				disconnectDomNodeFromQuantaNode(
					_tmpNode->_leafNode);
			_tmpNode = getNextNode(_tmpNode, b);
		}
		delete _node->child;
	}
	else
		_nodeChilds = _node->child;
	if(!_node->_rootNode.isNull())
		disconnectDomNodeFromQuantaNode(_node->_rootNode);
	if(!_node->_leafNode.isNull())
		disconnectDomNodeFromQuantaNode(_node->_leafNode);
	delete _node;
	_node = 0L;

	if(_nodePrev)
	{
		if(!deleteChilds)
			_nodePrev->next = _nodeChilds;
		else if(_nodeNext)
			_nodePrev->next = _nodeNext;
		else
			_nodePrev->next = 0L;
	}
	if(_nodeNext)
	{
		if(!deleteChilds)
		{
			_tmpNode = _nodeChilds;
			while(_tmpNode)
			{
				if(_tmpNode->next)
					_tmpNode = _tmpNode->next;
				else
					break;
			}
			_nodeNext->prev = _tmpNode;
		}
		else if(_nodePrev)
			_nodeNext->prev = _nodePrev;
		else
			_nodeNext->prev = 0L;
	}
	if(_nodeParent->child == 0L)
	{//it means that it was _node before
		if(!deleteChilds)
			_nodeParent->child = _nodeChilds;
		else if(_nodePrev)
			_nodeParent->child = _nodePrev;
		else if(_nodeNext)
			_nodeParent->child = _nodeNext;
		else
			_nodeParent->child = 0L;
	}
	if(!deleteChilds)
	{
		_tmpNode = _nodeChilds;
		while(_tmpNode)
		{
			_tmpNode->parent = _nodeParent;
			_tmpNode = _tmpNode->next;
		}
	}

	/** THIRD PART: change the neighbour Tag positions */

	b = true;
	if(_nodeParentBackup)
		_nodeParentBackup = getNextNode(_nodeParentBackup, b);
	b = true;
	if(_nodeParentClosingBackup)
		_nodeParentClosingBackup = getNextNode(_nodeParentClosingBackup, b);

	if(hasClosingNode && !deleteChilds)
	{
		if(_nodeChilds)
		{
			fitsNodesPosition(_nodeChilds, taglenCol, taglenLine, startLine2,
				startCol2);
		}
		_tmpNode = (_nodeNextClosingBackup)?_nodeNextClosingBackup:
			_nodeParentClosingBackup;
		fitsNodesPosition(_tmpNode, - taglenCol - closingTaglenCol - 1, - taglenLine -
			closingTaglenCol);
	}
	else if(hasClosingNode)//&& deleteChilds
	{
		_tmpNode = (_nodeNextClosingBackup)?_nodeNextClosingBackup:
			_nodeParentClosingBackup;
		fitsNodesPosition(_tmpNode, startCol - endCol2 - 1, endLine - endLine2);
	}
	else
	{
		_tmpNode = (_nodeNextBackup)?_nodeNextBackup:_nodeParentBackup;
		fitsNodesPosition(_tmpNode, - taglenCol - 1, - taglenLine);
	}

	coutTree(_rootNode, 2);
	kdDebug(25001)<< "WKafkaPart::slotDomNodeDeleted in " << t.elapsed() <<
		" ms only!" << endl;
	baseNode = parser->parse(_currentDoc);
}

void WKafkaPart::slotdomNodeNewCursorPos(DOM::Node _domNode, int offset)
{
	kdDebug(25001)<< "WKafkaPart::slotdomNodeNewCursorPos()" << endl;
	int line, col;
	getQuantaCursorPosition(line, col);
	emit newCursorPosition(line, col);
}

void WKafkaPart::buildKafkaNodeFromNode(Node *_node)
{
	DOM::Node newNode, newNode2, attr;
	QString str, nodeValue;
	bool insertNode = true;
	int i;

	if(!_node->tag->single && _node->next && _node->next->tag->type != Tag::XmlTagEnd)
	{
		//TODO: ERROR missing closing tags, set the kafka behavior according to this
		kdDebug(25001)<< "WKafkaPart::buildKafkaNodeFromNode() - Node missing closing Tag" <<
			endl;
	}

	if(!_node->parent)
	{//FIXME:html, head and body are HTML-specific tag, for others DTDs it might result to some pbs.
		str = _node->tag->name.lower();
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
			if(_node->tag->type == Tag::Text)
			{
				newNode = _kafkaPart->createNode("TEXT");
			}
			else
			{
				newNode = _kafkaPart->createNode(_node->tag->name);
				if (newNode.nodeName().string().upper() != _node->tag->name.upper())
					return;
			}
		}
	}
	else if(str == "body" && _node->parent && !_node->parent->parent)
	{
		if(!body.isNull())
			disconnectDomNodeFromQuantaNode(body);
		newNode = body;
		insertNode = false;
	}
	else if(str == "head" && _node->parent && !_node->parent->parent)
	{
		if(!head.isNull())
			disconnectDomNodeFromQuantaNode(head);
		newNode = head;
		insertNode = false;
	}
/**	else if(node->parent->tag->str == "html")*/
	else
	{
		if(_node->tag->type == Tag::Text)
		{
			newNode = _kafkaPart->createNode("TEXT");
		}
		else
		{
			newNode = _kafkaPart->createNode(_node->tag->name);
			if (newNode.nodeName().string().upper() != _node->tag->name.upper())
				return;
		}
	}

	connectDomNodeToQuantaNode(newNode, _node);
	if(_node->tag->type == Tag::Text)
	{
		nodeValue = _node->tag->tagStr();
		nodeValue = getDecodedText(nodeValue);
		newNode.setNodeValue(nodeValue);
	}

	for(i = 0; i < _node->tag->attrCount(); i++)
	{
		attr = _kafkaPart->htmlDocument().createAttribute(_node->tag->attribute(i));
		if(!attr.isNull())
		{
			attr.setNodeValue(_node->tag->attributeValue(i));
			newNode.attributes().setNamedItem(attr);
			/**kdDebug(25001)<< "WKafkaPart::buildKafkaNodeFromNode() -  Attr added : " <<
				_node->tag->attribute(i) <<
				" value : " << _node->tag->attributeValue(i) << endl;*/
		}
	}

	if(_node->next && _node->next->tag && _node->next->tag->name ==
		("/" + _node->tag->name))
	{
		/**kdDebug(25001)<< "WKafkart::buildKafkaNodeFromNode()" <<
			"_node->_closingNode set." << endl;*/
		_node->_closingNode = _node->next;
	}

	if(insertNode)
	{
		_node->_rootNode = newNode;
		if(_node->parent && !_node->parent->_leafNode.isNull())
		{
			try
			{
				newNode = _node->parent->_leafNode.appendChild(newNode);
			} catch(DOM::DOMException e)
			{
				kdDebug(25001)<< "WKafkart::buildKafkaNodeFromNode() *ERROR* - code : " <<
					e.code << endl;
			}
			if(newNode.nodeName().string().upper() == "TABLE")//FIXME:HTML specific
			{
				newNode2 = _kafkaPart->createNode("TBODY");
				connectDomNodeToQuantaNode(newNode2, _node);
				try
				{
					newNode2 = newNode.appendChild(newNode2);
				} catch(DOM::DOMException e)
				{
					kdDebug(25001)<< "WKafkart::buildKafkaNodeFromNode() *ERROR2* - code : " <<
						e.code << endl;
				}
				_node->_leafNode = newNode2;
			}
			else
				_node->_leafNode = newNode;

		}
		else//we suppose it is on the top of the tree
		{
			try
			{
				newNode = body.appendChild(newNode);
			} catch(DOM::DOMException e)
			{
				kdDebug(25001)<< "WKafkart::buildKafkaNodeFromNode() *ERROR3* - code : " <<
					e.code << endl;
			}
			kdDebug(25001)<< "WKafkaPart::buildKafkaNodeFromNode() *WARNING* - adding" <<
			" node to the body (root) node" << endl;
			_node->_leafNode = newNode;
		}
	}
	else
	{
		_node->_rootNode = newNode;
		_node->_leafNode = newNode;
	}
}


QString WKafkaPart::getDecodedChar(QString encodedChar)
{
	QMap<QString, QString>::Iterator it = encodedChars.find(encodedChar);
	if(it == encodedChars.end())
		return "";
	return it.data();
}

QString WKafkaPart::getEncodedChar(QString decodedChar, QString previousDecodedChar)
{
	//TODO:Tab support
	QMap<QString, QString>::Iterator it = decodedChars.find(decodedChar);
	if(it == decodedChars.end())
		return decodedChar;
	if(decodedChar[0].isSpace() && !previousDecodedChar[0].isSpace())
		return " ";
	else if(decodedChar[0].isSpace())
		//FIXME:for an unknown reason, by default it return &iquest; instead of &nbsp;
		return "&nbsp;";
	return it.data();
	}

QString WKafkaPart::getDecodedText(QString encodedText)
{
	QString decodedChar;
	int i, j;

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

	return encodedText;
}

QString WKafkaPart::getEncodedText(QString decodedText)
{
	QString Encodedchar;
	QString decodedChar, previousDecodedChar;
	int i;

	i = -1;
	while((unsigned)++i < decodedText.length())
	{
		previousDecodedChar = decodedChar;
		decodedChar = QString(decodedText[i]);
		Encodedchar = getEncodedChar(QString(decodedText[i]),
			(i>=1)?previousDecodedChar:QString(""));
		decodedText.remove(i,1);
		decodedText.insert(i, Encodedchar);
		i += Encodedchar.length() - 1;
	}
	return decodedText;
}

Node *WKafkaPart::getNextNode(Node *_node, bool &goingTowardsRootNode)
{
	//goto next node, my favorite part :)
	if(!_node) return 0L;
	if(goingTowardsRootNode)
	{
		if(_node->next)
		{
			goingTowardsRootNode = false;
			return _node->next;
		}
		else
			return getNextNode(_node->parent, goingTowardsRootNode);
	}
	else
	{
		if(_node->child)
			return _node->child;
		else if(_node->next)
			return _node->next;
		else
		{
			goingTowardsRootNode = true;
			return getNextNode(_node->parent, goingTowardsRootNode);
		}
	}
}

Node *WKafkaPart::searchCorrespondingNode(DOM::Node _domNode)
{
	kdDebug(25001)<< "WKafkaPart::searchCorrespondingNode()" << endl;
	if(_domNode.isNull() || _domNode.nodeName().string() == "#document")
	{
		kdDebug(25001)<< "WKafkaPart::searchCorrespondingNode() - Bad Node given" <<
			endl;
		return 0L;
	}
	kdDebug(25001)<< "WKafkaPart::searchCorrespondingNode() - current DOM::Node :" <<
		_domNode.nodeName().string() << endl;
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

void WKafkaPart::coutTree(Node *node, int indent)
{
	QString output;
	int bLine, bCol, eLine, eCol, j;
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
		kdDebug(25001) << output <<" (" << node->tag->type << ") at pos " <<
			bLine << ":" << bCol << " - " << eLine << ":" << eCol << endl;
		for(j = 0; j < node->tag->attrCount(); j++)
		{
			kdDebug(25001)<< " attr" << j << " " <<
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

void WKafkaPart::fitsNodesPosition(Node* _startNode, int colMovement, int lineMovement, int colEnd, int lineEnd)
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

QString WKafkaPart::generateCodeFromNode(Node *_node)
{
	QString text;
	int j;

	if(!_node) return "";

	if(_node->tag->type == Tag::XmlTag)
	{
		text = "<" + QuantaCommon::tagCase(_node->tag->name);
		for(j = 0; j < _node->tag->attrCount(); j++)
		{
			text += " ";
			text += _node->tag->attribute(j);
			text += "=";
			if(_node->tag->isQuotedAttribute(j)) text += qConfig.attrValueQuotation;
			text += _node->tag->attributeValue(j);
			if(_node->tag->isQuotedAttribute(j)) text += qConfig.attrValueQuotation;
		}
		if ( _node->tag->dtd->singleTagStyle == "xml" &&
			(_node->tag->single || (!qConfig.closeOptionalTags &&
			QuantaCommon::isOptionalTag(_node->tag->dtd->name, _node->tag->name))))
			text += "/";
		text += ">";
	}
	else if(_node->tag->type == Tag::XmlTagEnd)
	{
		text = "</" + QuantaCommon::tagCase(_node->tag->name) + ">";
	}
	else if(_node->tag->type == Tag::Text)
	{
		text = _node->tag->tagStr();
	}

	return text;
}

void WKafkaPart::buildNodeFromKafkaNode(Node *_node, DOM::Node _domNode)
{
	kdDebug(25001)<< "void WKafkaPart::buildNodeFromKafkaNode(Node*, DOM::Node)" << endl;
	QString domText;
	long i;
	int beginLine, beginCol, endLine, endCol, attrLine, attrCol;
	int oldColLength = 0, oldLineLength = 0;
	bool b;

	if(!_node) return;

	_node->tag->beginPos(beginLine, beginCol);
	_node->tag->endPos(endLine, endCol);
	if(endLine != -1)
	{//we don't come from Node* WKafkaPart::buildNodeFromKafkaNode()
		oldColLength = endCol - beginCol + 1;
		oldLineLength = endLine - beginLine;
	}

	if(_domNode.nodeType() == DOM::Node::TEXT_NODE)
	{
		domText = getEncodedText(_domNode.nodeValue().string());
		//TODO:try to keep the indentation
		/**i = -1;
		endLine = beginLine;
		endCol = beginCol;
		while((unsigned)++i < domText.length())
		{
			if(endCol > EDITOR_MAX_COL && domText[i].isSpace())
			{
				domText.insert((i+1), "\n");
				endCol = -1;
				endLine++;
			}
			else
			{
				endCol++;
			}
		}
		endCol--;*/
		_node->tag->setStr(domText);
		endLine = beginLine;
		endCol = beginCol + domText.length() - 1;
		_node->tag->setTagPosition(beginLine, beginCol, endLine, endCol);
		_currentDoc->editIf->insertText(beginLine, beginCol,
			generateCodeFromNode(_node));
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
			if(_node->tag->attrCount() != 0)
			{
				attrCol = _node->tag->getAttribute(_node->tag->attrCount() -
					1).valueCol + _node->tag->getAttribute(
					_node->tag->attrCount() - 1).value.length() + 1;
				attrLine = _node->tag->getAttribute(_node->tag->attrCount() -
					1).valueLine;
			}
			else
			{
				attrCol = beginCol + + 1 + _node->tag->name.length() + 1;
				attrLine = beginLine;
			}
			attr.nameCol = attrCol;
			attr.nameLine = attrLine;
			attr.valueCol = attrCol + _node->tag->name.length() + 1;
			attr.valueLine = attrLine;
			attr.quoted = true;
			_node->tag->addAttribute(attr);
		}

		QString text = generateCodeFromNode(_node);
		_node->tag->setStr(text);
		_currentDoc->editIf->insertText(beginLine, beginCol, text);
		endLine = beginLine;
		endCol = beginCol + text.length() - 1;
		_node->tag->setTagPosition(beginLine, beginCol, endLine, endCol);
	}

	_node = getNextNode(_node, b);
	b = true;
	fitsNodesPosition(_node, endCol - beginCol + 1 - oldColLength,
		endLine - beginLine - oldLineLength);

}

Node * WKafkaPart::buildNodeFromKafkaNode(DOM::Node _domNode, Node *_nodeParent, Node *_nodePrev, bool appendChild )
{
	kdDebug(25001)<< "Node* WKafkaPart::buildNodeFromKafkaNode(DOM::Node, 2xNode*)" << endl;
	Node *_node, *_nodeNext = 0L, *_nodeXmlEnd, *_nodePrev2 = 0L, *__nextNode;
	Tag *_tag, *_tagEnd;
	int beginLine, beginCol, endLine, endCol;
	bool b;

	if(_domNode.isNull())
	{
		kdDebug(25001)<< "Node* WKafkaPart::buildNodeFromKafkaNode(DOM::Node, 2xNode*)" <<
			" *ERROR* - empty _domNode"<< endl;
	}

	_node = new Node(_nodeParent);
	connectDomNodeToQuantaNode(_domNode, _node);
	_node->_rootNode = _domNode;
	//TODO:handle the special case of TABLE which need absolutely TBODY
	_node->_leafNode = _domNode;
	_node->prev = _nodePrev;
	if(_nodePrev)
	{
		_nodeNext = _nodePrev->next;
		_nodePrev->next = _node;
	}
	if(_nodeNext)
	{
		_node->next = _nodeNext;
		_nodeNext->prev = _node;
	}
	else
		_node->next = 0L;
	if(!_nodePrev)
	{
		if(!_nodeParent)
		{
			_rootNode = _node;
		}
		else if(_nodeParent->child)
		{
			if(appendChild)
			{
				_nodePrev2 = _nodeParent->child;
				while(_nodePrev2->next)
					_nodePrev2 = _nodePrev2->next;
				if(_nodePrev2->_closingNode)
					_nodePrev2->_closingNode->tag->endPos(
						beginLine,beginCol);
				else
					_nodePrev2->tag->endPos(beginLine, beginCol);
				beginCol++;
				_node->prev = _nodePrev2;
				_nodePrev2->next = _node;
			}
			else
			{
				_node->next = _nodeParent->child;
				_nodeParent->child->prev = _node;
				_nodeParent->child = _node;
			}
		}
		else//_nodeParent has no childs
			_nodeParent->child = _node;
	}

	if(_nodePrev)
	{
		if(_nodePrev->_closingNode)
			_nodePrev->_closingNode->tag->endPos(beginLine, beginCol);
		else
			_nodePrev->tag->endPos(beginLine, beginCol);
		beginCol++;
	}
	else if(_nodeParent && (!appendChild || !_nodeParent->child || _nodeParent->child == _node))
	{
		_nodeParent->tag->endPos(beginLine, beginCol);
		beginCol++;
	}
	else if(_nodeParent && appendChild && _nodeParent->child)
	{
		//do nothing
	}
	else
	{
		beginLine = 0;
		beginCol = 0;
	}

	_tag = new Tag();
	if(_domNode.nodeType() == DOM::Node::TEXT_NODE)
	{//if it is some text
		_node->tag = _tag;
		_tag->setWrite(_currentDoc);
		_tag->type = Tag::Text;
		_tag->single = true;
		_tag->dtd = _currentDoc->defaultDTD();
		_tag->setTagPosition(beginLine, beginCol, -1, -1);
		buildNodeFromKafkaNode(_node, _domNode);
	}
	else
	{//it is an element
		_node->tag = _tag;
		_tag->setWrite(_currentDoc);
		_tag->type = Tag::XmlTag;
		_tag->name = _domNode.nodeName().string();
		_tag->single = QuantaCommon::isSingleTag(_currentDoc->defaultDTD()->name,
			 _tag->name);
		_tag->dtd = _currentDoc->defaultDTD();
		_tag->setTagPosition(beginLine, beginCol, -1, -1);
		buildNodeFromKafkaNode(_node, _domNode);
		int tmpEndLine, tmpEndCol;
		_node->tag->endPos(tmpEndLine, tmpEndCol);

		kdDebug(25001)<< "Node* WKafkaPart::buildNodeFromKafkaNode() - Node CREATED" <<
			" at pos: " << beginLine << ":" << beginCol <<
			" - " << tmpEndLine << ":" << tmpEndCol << endl;

		if(!_tag->single)
		{
			_nodeXmlEnd = new Node(_nodeParent);
			_node->_closingNode = _nodeXmlEnd;
			_nodeXmlEnd->prev = _node;
			_nodeXmlEnd->next = _node->next;
			if(_node->next)
				_node->next->prev = _nodeXmlEnd;
			_node->next = _nodeXmlEnd;

			_tagEnd = new Tag();
			_nodeXmlEnd->tag = _tagEnd;
			_tagEnd->setWrite(_currentDoc);
			_tagEnd->type = Tag::XmlTagEnd;
			_tagEnd->single = true;
			_tagEnd->dtd = _currentDoc->defaultDTD();
			_tagEnd->name = _tag->name;
			//temp adding node code to editor
			QString text = generateCodeFromNode(_nodeXmlEnd);
			_node->tag->endPos(endLine, endCol);
			_currentDoc->editIf->insertText(endLine, endCol + 1, text);
			_tagEnd->setTagPosition(endLine, endCol + 1, endLine, endCol + 1 +
				text.length() - 1);
			beginLine = endLine;
			beginCol = endCol + 1;
			endCol = endCol + 1 + text.length() - 1;
			endLine = endLine;
			b = true;
			__nextNode = getNextNode(_nodeXmlEnd, b);
			fitsNodesPosition(__nextNode, endCol - beginCol + 1, endLine - beginLine);
			kdDebug(25001)<< "Node* WKafkaPart::buildNodeFromKafkaNode() - " <<
				"NodeEnd CREATED at pos: " << beginLine << ":" << beginCol <<
				" - " << beginLine << ":" << endCol << endl;
		}
	}
	//coutTree(_rootNode, 2);
	return _node;
}

void WKafkaPart::getQuantaCursorPosition(int &line, int &col)
{
	DOM::Node _currentDomNode;
	Node *_currentNode;
	QString decodedText, Encodedchar, currentLine, currentChar;
	int offset, i, currentOffset = 0;
	int curLine, curCol, endLine, endCol;
	bool _break = false, cursorAfterChar = false;

	_kafkaPart->getCurrentNode(_currentDomNode, offset);
	if(_currentDomNode.isNull())
	{
		kdDebug(25001)<< "WKafkaPart::getQuantaCursorPosition() - DOM::Node not found!" << endl;
		line = 0;
		col = 0;
		return;
	}
	decodedText = _currentDomNode.nodeValue().string();
	_currentNode = searchCorrespondingNode(_currentDomNode);
	if(!_currentNode)
	{
		kdDebug(25001)<< "WKafkaPart::getQuantaCursorPosition() - Node not found!" << endl;
		line = 0;
		col = 0;
		return;
	}
	_currentNode->tag->beginPos(curLine, curCol);
	_currentNode->tag->endPos(endLine, endCol);
	currentLine = _currentDoc->editIf->textLine(curLine);

	if(offset == -1)
	{
		_currentDoc->selectionIf->setSelection(curLine, curCol, endLine, endCol + 1);
		line = endLine;
		col = endCol + 1;
		return;
	}

	cursorAfterChar = ((unsigned)offset == decodedText.length())?true:false;
	offset += ((unsigned)offset == decodedText.length())?0:1;
	i = -1;
	while((unsigned)++i < decodedText.length())
	{
		Encodedchar = getEncodedChar(QString(decodedText[i]),
			(i>=1)?currentChar:QString(""));
		while((currentLine.mid(curCol, Encodedchar.length()) != Encodedchar &&
			currentLine.mid(curCol, 1) != QString(decodedText[i])) ||
			(QString(currentLine.mid(curCol, Encodedchar.length())).at(0).isSpace()
			&& Encodedchar != " "))
		{
			if((curCol > endCol && curLine == endLine) || curLine > endLine)
			{
				_break = true;
				curLine = endLine;
				curCol = endCol + 1;
				break;
			}
			if((curCol + Encodedchar.length()) >= currentLine.length())
			{
				curLine++;
				curCol = 0;
				currentLine = _currentDoc->editIf->textLine(curLine);
			}
			else
				curCol++;
		}
		if(_break) break;
		currentChar = QString(decodedText[i]);
		if(currentLine.mid(curCol, Encodedchar.length()) == Encodedchar)
		{
			decodedText.remove(i,1);
			decodedText.insert(i, Encodedchar);
			if(offset == (currentOffset + 1))
			{
				if(cursorAfterChar)
					curCol += Encodedchar.length();
				break;
			}
			i += Encodedchar.length() - 1;
			curCol += Encodedchar.length();
		}
		else if(currentLine.mid(curCol, 1) == QString(decodedText[i]))
		{
			if(offset == (currentOffset + 1))
			{
				if(cursorAfterChar)
					curCol++;
				break;
			}
			curCol ++;
		}
		currentOffset++;
	}

	_currentDoc->selectionIf->setSelection(curLine, curCol, curLine, curCol);
	line = curLine;
	col = curCol;
	kdDebug(25001)<<"WKafkaPart::getQuantaCursorPosition() - " << line << ":" << col << endl;
	return;
}

int WKafkaPart::getKafkaCursorPosition()
{
	//TODO:to do it :=)
	return -1;
}

void WKafkaPart::connectDomNodeToQuantaNode(DOM::Node _domNode, Node *_node)
{
	QTag *qtag = 0L;
	kNodeAttrs *props;
	if(_domNode.isNull() || _node == 0L)
	{
		kdDebug(25001)<< "WKafkaPart::connectDomNodeToQuantaNode() *ERROR*" << endl;
				return;
	}
	qtag = QuantaCommon::tagFromDTD(_currentDoc->defaultDTD(),
		_domNode.nodeName().string());
	props = new kNodeAttrs();
	if(qtag)
	{
		kdDebug(25001)<< "WKafkaPart::connectDomNodeToQuantaNode() - " <<
			"QTag name : " << qtag->name() <<
			" canBeDeleted:" << qtag->canBeDeleted() <<
			" canBeModified:" << qtag->canBeModified() <<
			" canHaveCursorFocus:" << qtag->canHaveCursorFocus() <<
			" cursorCanEnter:" << qtag->cursorCanEnter() << endl;
		props->setCBDeleted(qtag->canBeDeleted());
		props->setCBModified(qtag->canBeModified());
		props->setCHCursorFocus(qtag->canHaveCursorFocus());
		props->setCCEnter(qtag->cursorCanEnter());
	}
	else if(_domNode.nodeType() == DOM::Node::TEXT_NODE)
	{
		kdDebug(25001)<< "WKafkaPart::connectDomNodeToQuantaNode() - " <<
			"Text Node, setting default text parameters" << endl;
		props->setCBDeleted(true);
		props->setCBModified(true);
		props->setCHCursorFocus(kNodeAttrs::textNode);
		props->setCCEnter(true);
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
	props->setNode(_node);
	domNodeProps.insert(_domNode.handle(), props);
}

void WKafkaPart::disconnectDomNodeFromQuantaNode(DOM::Node _domNode)
{
	domNodeProps.remove(_domNode.handle());
}

void WKafkaPart::slotGetFocus(bool focus)
{
	kdDebug(25001)<< "WKafkaPart::slotGetFocus(" << focus << ")" << endl;
	_currentDoc->activateParser(!focus);
}
