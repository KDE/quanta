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

WKafkaPart::WKafkaPart(QWidget *parent, QWidget *widgetParent, const char *name)
:domNodeToNode(1021)
{
	_kafkaPart = new KafkaHTMLPart(parent, widgetParent, name);
	_kafkaPart->showDomTree();
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
			int begin = tmp.find("(") + 1;
			if(begin == (-1 + 1)) break;//"(" not found : invalid line
			int length = tmp.find(")") - begin;
			decodedChars.insert(tmp.left(1), tmp.mid(begin, length));
			encodedChars.insert(tmp.mid(begin, length), tmp.left(1));
		}
		file.close();
	}

	connect(_kafkaPart, SIGNAL(domNodeInserted(DOM::Node)),
		this, SLOT(slotDomNodeInserted(DOM::Node)));
	connect(_kafkaPart, SIGNAL(domNodeModified(DOM::Node)),
		this, SLOT(slotDomNodeModified(DOM::Node)));
	connect(_kafkaPart, SIGNAL(domNodeIsAboutToBeRemoved(DOM::Node, bool)),
		this, SLOT(slotDomNodeAboutToBeRemoved(DOM::Node, bool)));

	connect(_kafkaPart, SIGNAL(domNodeNewCursorPos(DOM::Node, int)),
		this, SLOT(slotdomNodeNewCursorPos(DOM::Node, int)));
}

WKafkaPart::~WKafkaPart()
{

}

void WKafkaPart::loadDocument(Document *doc)
{
	kdDebug(25001) << "WKafkaPart::loadDocument()" << endl;
	if(!_kafkaPart) return;
	if(!doc) return;
	QTime t;
	t.start();
	_currentDoc = doc;
	if(_currentDoc->editIf->text().stripWhiteSpace() == "")
	{//set a minimum Nodes to be able to write if the document is empty
		_kafkaPart->newDocument();
		_kafkaPart->finishedLoading();
		return;
	}

	_rootNode = parser->parse(_currentDoc );

	//load the nodes
	bool goingUp = false;
	Node* _node = _rootNode;
	while(_node)
	{
		kdDebug(25001) << "WKafkaPart::loadDocument - Node name :" <<
			_node->tag->name.upper() << "; type : " <<
			_node->tag->type << "; tagstr : " << _node->tag->tagStr() <<
			" is opened :" << _node->opened << endl;
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
	kdDebug(25001)<< "WKafkaPart::loadDocument() in " << t.elapsed() << " ms only!" << endl;
	//coutTree(_rootNode, 2);
}

void WKafkaPart::unloadDocument()
{
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

void WKafkaPart::slotDomNodeInserted(DOM::Node _domNode)
{
	/** TODO: for the moment, we suppose that _domNode has no child*/
	/** TODO: avoid to put all the nodes in the same line */
	kdDebug(25001)<< "WKafkaPart::slotDomNodeInserted() - node :" <<
		_domNode.nodeName().string() << endl;
	Node *_nodeParent = 0L, *_nodePrev = 0L, *_node = 0L;

	if(_domNode.hasChildNodes())
	{
		kdDebug(25001)<< "WKafkaPart::slotDomNodeInserted() - doesnt support" <<
		 "inserting nodes with childs for the moment. Sorry" << endl;
		return;
	}
	if(!_rootNode)
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

	//TODO:remplace with a QPtrDict
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

	baseNode = parser->parse(_currentDoc);
	//coutTree(_rootNode, 2);
}

void WKafkaPart::slotDomNodeModified(DOM::Node _domNode)
{
	kdDebug(25001)<< "WKafkaPart::slotDomNodeModified()" << endl;
	Node *_node = 0L;
	int beginLine, beginCol, endLine, endCol;
	int oldTaglenCol, oldTaglenLine, taglenCol, taglenLine;
	bool b = false;

	if(!_rootNode)
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
	//_node = domNodeToNode[_domNode];

	if(!_node)
	{//DOM::Node not found, maybe was it created before
		kdDebug(25001)<< "WKafkaPart::slotDomNodeModified() - *ERROR* the" <<
			" corresponding DOM::Node is not found!" << endl;
		return;
	}

	_node->tag->beginPos(beginLine, beginCol);
	_node->tag->endPos(endLine, endCol);
	_currentDoc->editIf->removeText(beginLine, beginCol, endLine, endCol + 1);
	oldTaglenCol = endCol - beginCol;
	oldTaglenLine = endLine - beginLine;
	buildNodeFromKafkaNode(_node, _domNode);

	/** TODO:change -> SECOND PART: change the neighbour Tag positions */

	_node->tag->beginPos(beginLine, beginCol);
	_node->tag->endPos(endLine, endCol);
	taglenCol = endCol - beginCol;
	taglenLine = endLine - beginLine;
	_node = getNextNode(_node, b);
	fitsNodesPosition(_node, taglenCol - oldTaglenCol, taglenLine - oldTaglenLine);

	//coutTree(_rootNode, 2);
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
	if(_node->kafkaAddon && _node->kafkaAddon->_closingNode)
	{
		hasClosingNode = true;
		_nodeNextClosingBackup = _node->kafkaAddon->_closingNode->next;
		_nodeParentClosingBackup = _node->kafkaAddon->_closingNode->parent;
		_node->kafkaAddon->_closingNode->tag->beginPos(startLine2, startCol2);
		_node->kafkaAddon->_closingNode->tag->endPos(endLine2, endCol2);
		closingTaglenCol = startCol2 - endCol2;
		closingTaglenLine = startLine2 - endLine2;
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
	taglenCol = startCol - endCol;
	taglenLine = startLine - endLine;

	/** SECOND PART: remove the node from the quanta tree and delete it */

	_node->removeAll = false;
	_nodePrev = _node->prev;
	if(hasClosingNode)
	{
		_nodeNext = _node->kafkaAddon->_closingNode->next;
		_node->kafkaAddon->_closingNode->removeAll = false;
	}
	else
		_nodeNext = _node->next;
	_nodeParent = _node->parent;
	if(hasClosingNode)
		delete _node->kafkaAddon->_closingNode;
	if(_node->child && deleteChilds)
		delete _node->child;
	else
		_nodeChilds = _node->child;
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
		fitsNodesPosition(_tmpNode, - taglenCol, - taglenLine - 1);
	}

	//coutTree(_rootNode, 2);

	baseNode = parser->parse(_currentDoc);
}

void WKafkaPart::slotdomNodeNewCursorPos(DOM::Node _domNode, int offset)
{
	kdDebug(25001)<< "WKafkaPart::slotdomNodeNewCursorPos()" << endl;
	int line, col;
	getQuantaCursorPosition(line, col);
	emit newCursorPosition(line, col);
}

void WKafkaPart::synchronizeXMLTag(Node* _node)
{
	DOM::Node newNode, newNode2;
	DOM::Node attr;
	int i;

	if(!_node->tag->single && _node->next && _node->next->tag->type != Tag::XmlTagEnd)
	{
		//TODO: ERROR missing closing tags, set the kafka behavior according to this
		kdDebug(25001)<< "WKafkaPart::synchronizeXMLTag() - Node missing closing Tag"
			<< endl;
	}

	newNode = _kafkaPart->createNode(_node->tag->name);
	if (newNode.nodeName().string().upper() != _node->tag->name.upper())
		return;
	//domNodeToNode.insert( newNode.handle(), _node);

	for(i = 0; i < _node->tag->attrCount(); i++)
	{
		attr = _kafkaPart->htmlDocument().createAttribute(_node->tag->attribute(i));
		if(!attr.isNull())
		{
			attr.setNodeValue(_node->tag->attributeValue(i));
			newNode.attributes().setNamedItem(attr);
			kdDebug(25001)<< "WKafkaPart::synchronizeXMLTag() -  Attr added : " <<
				_node->tag->attribute(i) <<
				" value : " << _node->tag->attributeValue(i) << endl;
		}
	}

	_node->kafkaAddon = new kNodeProperties();
	if(_node->next && _node->next->tag && _node->next->tag->name ==
		("/" + _node->tag->name))
	{
		kdDebug(25001)<< "WKafkart::synchronizeXMLTag()" <<
			"_node->kafkaAddon->_closingNode set." << endl;
		_node->kafkaAddon->_closingNode = _node->next;
	}

	if(_node->parent && _node->parent->kafkaAddon &&
		!_node->parent->kafkaAddon->_leafNode.isNull())
	{
		try
		{
			newNode = _node->parent->kafkaAddon->_leafNode.appendChild(newNode);
		} catch(DOM::DOMException e)
		{kdDebug(25001)<< "WKafkart::synchronizeXMLTag() *ERROR* - code : " <<
			e.code << endl;}
		if(newNode.nodeName().string().upper() == "TABLE")
		{
			newNode2 = _kafkaPart->createNode("TBODY");
			//domNodeToNode.insert(newNode2, _node);
			try
			{
				newNode2 = newNode.appendChild(newNode2);
			} catch(DOM::DOMException e)
			{kdDebug(25001)<< "WKafkart::synchronizeXMLTag() *ERROR2* - code : " <<
				e.code << endl;}
			_node->kafkaAddon->_leafNode = newNode2;
		}
		else
			_node->kafkaAddon->_leafNode = newNode;

	}
	else//we suppose it is on the top of the tree
	{
		try
		{
			newNode = _kafkaPart->document().appendChild(newNode);
		} catch(DOM::DOMException e)
		{kdDebug(25001)<< "WKafkart::synchronizeXMLTag() *ERROR3* - code : " <<
			e.code << endl;}
		kdDebug(25001)<< "WKafkaPart::synchronizeXMLTag() *WARNING* - adding" <<
		" node to the root node" << endl;
		_node->kafkaAddon->_leafNode = newNode;
	}
	_node->kafkaAddon->_rootNode = newNode;

	if(_node->parent)
	kdDebug(25001)<< "WKafkaPart::synchronizeXMLTag() - Added DOM::Node : " <<
		_node->kafkaAddon->_rootNode.nodeName().string() <<
		" - Node : " << _node->tag->tagStr() <<
		" - parent DOM::Node : " <<
			_node->parent->kafkaAddon->_leafNode.nodeName().string() <<
		" - parent Node : " << _node->parent->tag->tagStr() << endl;
	else
	kdDebug(25001)<< "WKafkaPart::synchronizeXMLTag() - Added DOM::Node : " <<
		_node->kafkaAddon->_rootNode.nodeName().string() <<
		" - Node : " << _node->tag->tagStr() << endl;
}

void WKafkaPart::synchronizeTextTag(Node* _node)
{
	DOM::Node newNode;
	QString nodeValue;
	DOM::Node _nextNode;

	newNode = _kafkaPart->createNode("TEXT");
	if (newNode.isNull())
	{
		kdDebug(25001)<< "WKafkaPart::synchronizeTextTag() - *ERROR* : null Node" << endl;
		return;
	}
	//domNodeToNode.insert(newNode, _node);
	nodeValue = _node->tag->tagStr();
	nodeValue = getDecodedText(nodeValue);
	newNode.setNodeValue(nodeValue);

	_node->kafkaAddon = new kNodeProperties();

	if(_node->parent && _node->parent->kafkaAddon &&
		!_node->parent->kafkaAddon->_leafNode.isNull())
	{
		try
		{
			_node->parent->kafkaAddon->_leafNode.appendChild(newNode);
		} catch(DOM::DOMException e)
		{kdDebug(25001)<< "WKafkart::synchronizeTextTag() *ERROR* - code : " <<
			e.code << endl;}
		_node->parent->kafkaAddon->_leafNode.applyChanges();
	}
	else//we suppose it is on the top of the tree
	{
		try
		{
			newNode = _kafkaPart->htmlDocument().appendChild(newNode);
		} catch(DOM::DOMException e)
		{kdDebug(25001)<< "WKafkart::synchronizeTextTag() *ERROR2* - code : " <<
			e.code << endl;}
		kdDebug(25001)<< "WKafkaPart::synchronizeTextTag() *WARNING* - adding" <<
			" node to the root node" << endl;
	}
	_node->kafkaAddon->_rootNode = newNode;
	_node->kafkaAddon->_leafNode = newNode;
	kdDebug(25001)<< "WKafkaPart::synchronizeTextTag() - Added text DOM::Node : " <<
		newNode.nodeName().string() << endl;
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
	if(_domNode.isNull()) return 0L;
	bool b = false;
	Node *nextNode = _rootNode;
	while(nextNode)
	{
		if(nextNode->kafkaAddon && nextNode->kafkaAddon->_rootNode == _domNode)
		{
			kdDebug(25001)<< "WKafkaPart::searchCorrespondingNode() - Node found!"
				 << endl;
			break;
		}
		nextNode = getNextNode(nextNode, b);
	}
	return nextNode;
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
	int j, SNbeginLine, SNbeginCol, SNlastLine, SNlastCol;
	int beginLine, beginCol, lastLine, lastCol;
	Node *_node = _startNode;

	if(!_node) return;

	_startNode->tag->beginPos(SNbeginLine, SNbeginCol);
	_startNode->tag->endPos(SNlastLine, SNlastCol);

	while(_node)
	{
		_node->tag->beginPos(beginLine, beginCol);
		_node->tag->endPos(lastLine, lastCol);
		if(beginLine >= lineEnd && beginCol >= colEnd &&
			colEnd != -2 && lineEnd != -2)
			return;
		if(beginLine == SNbeginLine && lastLine == SNlastLine)
			_node->tag->setTagPosition(beginLine + lineMovement,
				beginCol + colMovement, lastLine + lineMovement ,
				lastCol + colMovement);
		else if(beginLine == SNbeginLine)//&&lastLine != SNlastLine
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

	if(!_node) return;

	_node->tag->beginPos(beginLine, beginCol);

	if(_domNode.nodeType() == DOM::Node::TEXT_NODE)
	{
		domText = getEncodedText(_domNode.nodeValue().string());
		//TODO:try to keep the indentation
		_node->tag->setStr(domText);
		_node->tag->setTagPosition(beginLine, beginCol, beginLine, beginCol +
			domText.length() - 1);
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
	_node->kafkaAddon = new kNodeProperties();
	_node->kafkaAddon->_rootNode = _domNode;
	//TODO:handle the special case of TABLE which need absolutely TBODY
	_node->kafkaAddon->_leafNode = _domNode;
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
				if(_nodePrev2->kafkaAddon &&
					_nodePrev2->kafkaAddon->_closingNode)
					_nodePrev2->kafkaAddon->_closingNode->tag->endPos(
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
		if(_nodePrev->kafkaAddon && _nodePrev->kafkaAddon->_closingNode)
			_nodePrev->kafkaAddon->_closingNode->tag->endPos(beginLine, beginCol);
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
		_tag->dtd = _currentDoc->currentDTD();
		//_tag->dtd = _rootNode->tag->dtd;
		_tag->setTagPosition(beginLine, beginCol, -1, -1);
		buildNodeFromKafkaNode(_node, _domNode);
	}
	else
	{//it is an element
		_node->tag = _tag;
		_tag->setWrite(_currentDoc);
		_tag->type = Tag::XmlTag;
		_tag->name = _domNode.nodeName().string();
		_tag->single = QuantaCommon::isSingleTag(_currentDoc->currentDTD()->name,
			 _tag->name);
		/**some random problem with parser::nodeAt in Document::currentDTD method*/
		_tag->dtd = _currentDoc->currentDTD();
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
			_node->kafkaAddon->_closingNode = _nodeXmlEnd;
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
			//_tagEnd->dtd = _rootNode->tag->dtd;
			_tagEnd->dtd = _currentDoc->currentDTD();
			_tagEnd->name = _tag->name;
			//temp adding node code to editor
			QString text = generateCodeFromNode(_nodeXmlEnd);
			_node->tag->endPos(endLine, endCol);
			_currentDoc->editIf->insertText(endLine, endCol + 1, text);
			_tagEnd->setTagPosition(endLine, endCol + 1, endLine, endCol + 1 +
				text.length() - 1);
			kdDebug(25001)<< "Node* WKafkaPart::buildNodeFromKafkaNode() - " <<
				"NodeEnd CREATED at pos: " << endLine << ":" << endCol + 1 <<
				" - " << endLine << ":" << endCol + 1 + text.length() - 1 << endl;
		}
	}
	_node->tag->beginPos(beginLine, beginCol);
	if(!_node->tag->single)
	{
		_nodeXmlEnd->tag->endPos(endLine, endCol);
		__nextNode = getNextNode(_nodeXmlEnd, b);
	}
	else
	{
		_node->tag->endPos(endLine, endCol);
		__nextNode = getNextNode(_node, b);
	}
	b = true;
	fitsNodesPosition(__nextNode, endCol - beginCol + 1, endLine - beginLine);
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
		return;
	}
	decodedText = _currentDomNode.nodeValue().string();
	_currentNode = searchCorrespondingNode(_currentDomNode);
	if(!_currentNode)
	{
		kdDebug(25001)<< "WKafkaPart::getQuantaCursorPosition() - Node not found!" << endl;
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
			(i>=1)?QString(decodedText[i-1]):QString(""));
		decodedText.remove(i,1);
		decodedText.insert(i, Encodedchar);
		currentChar = currentLine.mid(curCol, Encodedchar.length());
		while(currentLine.mid(curCol, Encodedchar.length()) != Encodedchar ||
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
		if(offset == (currentOffset + 1))
		{
			if(cursorAfterChar)
				curCol += Encodedchar.length();
			break;
		}
		i += Encodedchar.length() - 1;
		curCol += Encodedchar.length();
		currentOffset++;
	}

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
