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
{
	_kafkaPart = new KafkaHTMLPart(parent, widgetParent, name);
	_kafkaPart->showDomTree();
	//_parser = new Parser();
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

	connect(_kafkaPart, SIGNAL(domNodeInserted(DOM::Node)),
		this, SLOT(slotDomNodeInserted(DOM::Node)));
	connect(_kafkaPart, SIGNAL(domNodeModified(DOM::Node)),
		this, SLOT(slotDomNodeModified(DOM::Node)));
	connect(_kafkaPart, SIGNAL(domNodeIsAboutToBeRemoved(DOM::Node)),
		this, SLOT(slotDomNodeAboutToBeRemoved(DOM::Node)));
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

	_rootNode = parser->parse(_currentDoc );

	//load the nodes
	bool goingUp = false;
	Node* _node = _rootNode;
	while(_node)
	{
		kdDebug(25001) << "Node name :" << _node->tag->name.upper() << "; type : "
			<<_node->tag->type << "; tagstr : " << _node->tag->tagStr() <<
			" is opened :" << _node->opened << endl;
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
	kdDebug(25001)<< "WKafkaPart::loadDocument() in " << t.elapsed() << " ms only!" << endl;
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
	kdDebug(25001)<< "WKafkaPart::slotDomNodeInserted() node :" <<
		_domNode.nodeName().string() << endl;
	Node *_nodeParent = 0L, *_nodePrev = 0L, *_node = 0L;
	Node *_nodeNext = 0L, *_nodeXmlEnd = 0L, *__nextNode = 0L;
	Tag *_tag = 0L, *_tagEnd = 0L;
	long i;
	int j, col, line, foo = 0, taglenCol, taglenLine;
	int beginCol, beginLine, lastCol, lastLine, attrLine, attrCol;
	int NNbeginLine, NNbeginCol, NNlastLine, NNlastCol;
	bool b = false;

	/** FIRST PART: create the new Quanta node and insert it into the tree */

	if(_domNode.hasChildNodes())
	{
		kdDebug(25001)<< "WKafkaPart::slotDomNodeInserted() - doesnt support" <<
		 "inserting nodes with childs for the moment. Sorry" << endl;
		return;
	}
	if(!_rootNode)
	{//we are in the case when a minimal kafka tree has been set
		//TODO
	}

	//TODO:remplace with a QPtrDict
	_nodeParent = searchCorrespondingNode(_domNode.parentNode());
	if(!_nodeParent)
	{//DOM::Node not found, maybe was it created before
		kdDebug(25001)<< "WKafkaPart::slotDomNodeInserted() - *ERROR* the" <<
			" corresponding DOM::Node is not found!" << endl;
		return;
	}
	kdDebug(25001)<< "_nodeParent name:" << _nodeParent->tag->name << endl;
	if(_domNode.previousSibling() != 0)
	{
		_nodePrev = searchCorrespondingNode(_domNode.previousSibling());
		if(!_nodePrev)
		{
			kdDebug(25001)<< "WKafkaPart::slotDomNodeInserted() - *ERROR2*" <<
			" the corresponding DOM::Node is not found!" << endl;
			return;
		}
		kdDebug(25001)<< "_nodePrev name:" << _nodePrev->tag->name << endl;
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
	{
		if(_nodeParent->child)
		{
			_node->next = _nodeParent->child;
			_nodeParent->child->prev = _node;
		}
		else
			_node->next = 0L;
	}

	if(_nodePrev)
	{
		if(_nodePrev->kafkaAddon && _nodePrev->kafkaAddon->_closingNode)
			_nodePrev->kafkaAddon->_closingNode->tag->endPos(beginLine, beginCol);
		else
			_nodePrev->tag->endPos(beginLine, beginCol);
		beginCol++;
	}
	else if(_nodeParent)
	{
		_nodeParent->tag->endPos(beginLine, beginCol);
		beginCol++;
	}
	else
	{
		beginLine = 1;
		beginCol = 0;
	}

	_tag = new Tag();
	if(_domNode.nodeType() == DOM::Node::TEXT_NODE)
	{//if it is some text
		kdDebug(25001)<< "slotDomNodeInserted() - _domNode.type == TEXT_NODE" << endl;
		QString domText = _domNode.nodeValue().string();
		QString Encodedchar;
		j = -1;
		while((unsigned)++j < domText.length())
		{
			Encodedchar == getEncodedChar(QString(domText[j]), false);
			if(Encodedchar != "")
			{
				domText.remove(j,1);
				domText.insert(j, Encodedchar);
				j += Encodedchar.length() - 1;
			}
		}

		j = -1;
		while((unsigned)++j < domText.length())
		{
			if(domText[j].isSpace())
			{
				while((unsigned)++j < domText.length() && domText[j].isSpace())
				{
					domText.remove(j, 1);
					domText.insert(j, "&nbsp;");
					j += 5;

				}
			}
		}
		_tag->setStr(domText);
		_tag->setTagPosition(beginLine, beginCol, beginLine, beginCol +
			domText.length());
		_tag->setWrite(_currentDoc);
		_tag->type = Tag::Text;
		_tag->single = true;
		_node->tag = _tag;
		_tag->dtd = _rootNode->tag->dtd;
		/** SECOND PART: insert the HTML code in the editor*/
		kdDebug(25001)<< "slotDomNodeInserted() - insert text1:" << beginLine <<
			":" << beginCol << ":" << domText << endl;
		_currentDoc->editIf->insertText(beginLine, beginCol, domText);
	}
	else
	{//it is an element
		kdDebug(25001)<< "slotDomNodeInserted() - _domNode.type != TEXT_NODE" << endl;
		_tag->name = _domNode.nodeName().string();
		_tag->setTagPosition(beginLine, beginCol, beginLine, beginCol +
			 _tag->name.length() + 2);
		for(i = 0; (unsigned)i < _domNode.attributes().length(); i++)
		{
			TagAttr attr;
			attr.name = _domNode.attributes().item(i).nodeName().string();
			attr.value = _domNode.attributes().item(i).nodeValue().string();
			if(_tag->attrCount() != 0)
			{
				attrCol = _tag->getAttribute(_tag->attrCount() - 1).valueCol +
					 _tag->getAttribute(_tag->attrCount() -
					  1).value.length() + 1;
				attrLine = _tag->getAttribute(_tag->attrCount() - 1).valueLine;
			}
			else
			{
				attrCol = beginCol + + 1 + _tag->name.length() + 1;
				attrLine = beginLine;
			}
			attr.nameCol = attrCol;
			attr.nameLine = attrLine;
			attr.valueCol = attrCol + _tag->name.length() + 1;
			attr.valueLine = attrLine;
			//attr.quoted = ???
			_tag->addAttribute(attr);
		}
		_tag->type = Tag::XmlTag;
		_tag->setWrite(_currentDoc);
		_node->tag = _tag;
		/**some random problem with parser::nodeAt in Document::currentDTD method*/
		_tag->dtd = _rootNode->tag->dtd;
		_tag->single = QuantaCommon::isSingleTag(_rootNode->tag->dtd->name,
			 _tag->name);

		/** SECOND PART: insert the HTML code in the editor*/

		QString text = "<" + QuantaCommon::tagCase(_tag->name);
		lastCol = beginCol + text.length() + 1;
		lastLine = beginLine;
		_currentDoc->editIf->insertText(beginLine, beginCol, text);
		kdDebug(25001)<< "slotDomNodeInserted() - insert text1:" << beginLine <<
			":" << beginCol << ":" << text << endl;
		for(j = 0; j < _tag->attrCount(); j++)
		{
			text = _tag->attribute(j) + "=";
			if(_tag->isQuotedAttribute(j)) text += "\"";
			text += _tag->attributeValue(j);
			if(_tag->isQuotedAttribute(j)) text += "\"";
			_tag->attributeNamePos(j, line, col);
			_currentDoc->editIf->insertText(line, col, text);
			kdDebug(25001)<< "slotDomNodeInserted() - insert text2:" << line <<
				":" << col << ":" << text << endl;
			_tag->attributeValuePos(j, line, col);
			lastCol = col + _tag->attributeValue(j).length() +
				((_tag->isQuotedAttribute(j))?2:0);
			lastLine = line;
		}
		text = "";
		if ( _tag->dtd->singleTagStyle == "xml" &&
			(_tag->single || (!qConfig.closeOptionalTags &&
			QuantaCommon::isOptionalTag(_tag->dtd->name, _tag->name))))
		{
			text = "/";
			foo = 1;
		}
		else foo = 0;
		text += ">";
		kdDebug(25001)<< "slotDomNodeInserted() - insert text3:" << lastLine <<
			":" << lastCol << ":" << text << endl;
		_currentDoc->editIf->insertText(lastLine, lastCol, text);
		lastCol += foo;
		_tag->setTagPosition(beginLine, beginCol, lastLine, lastCol);
		/** temporary XmlTagEnd inserting TODO:make it work for node with childs */
		if(!_tag->single)
		{
			_nodeXmlEnd = new Node(_nodeParent);
			_node->kafkaAddon->_closingNode = _nodeXmlEnd;
			_nodeXmlEnd->prev = _node;
			_nodeXmlEnd->next = _node->next;
			_node->next = _nodeXmlEnd;
			_tagEnd = new Tag();
			_nodeXmlEnd->tag = _tagEnd;
			_tagEnd->single = true;
			lastCol++;
			_currentDoc->editIf->insertText(lastLine, lastCol, "</" +
				 _tag->name + ">");
			_tagEnd->setTagPosition(lastLine, lastCol, lastLine, lastCol + 2 +
				 _tag->name.length());
			lastCol = lastCol + 2 + _tag->name.length();
		}
		/** end temp*/

	}

	/** THIRD PART: change the neighbour Tag positions */

	taglenCol = lastCol - beginCol;
	//taglenLine = lastLine - beginLine;
	__nextNode = getNextNode(_node, b);
	while(__nextNode)
	{
		__nextNode->tag->beginPos(NNbeginLine, NNbeginCol);
		__nextNode->tag->endPos(NNlastLine, NNlastCol);
		if(NNbeginCol != beginCol) break;
		__nextNode->tag->setTagPosition(NNbeginLine, NNbeginCol +
			taglenCol, NNlastLine, NNlastCol + taglenCol);
		for(j = 0; j < __nextNode->tag->attrCount(); j++)
		{
			if(__nextNode->tag->getAttribute(j).nameLine != lastLine)
				break;
			//__nextNode->tag->getAttribute(j).nameLine += taglenLine;
			__nextNode->tag->getAttribute(j).nameCol += taglenCol;
			//__nextNode->tag->getAttribute(j).valueLine += taglenLine;
			__nextNode->tag->getAttribute(j).valueCol += taglenCol;
		}
		__nextNode = getNextNode(__nextNode, b);
	}

	baseNode = parser->parse(_currentDoc);
}

void WKafkaPart::slotDomNodeModified(DOM::Node _domNode)
{
	kdDebug(25001)<< "WKafkaPart::slotDomNodeModified()" << endl;
	Node *_node = 0L;
	if(!_rootNode)
	{//we are in the case when a minimal kafka tree has been set
		//TODO:createBasicQuantaTree();
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

	if(_domNode.nodeType() == DOM::Node::TEXT_NODE)
	{//"parse" the new text : spaces when possible else &nbsp;
		//parsing here...
	}
}

void WKafkaPart::slotDomNodeAboutToBeRemoved(DOM::Node _domNode)
{
	/** TODO: for the moment, it delete _domNode and all its childs*/
	/** TODO: make the 3rd step works for nodes which take more than 1 line */
	kdDebug(25001)<< "WKafkaPart::slotDomNodeAboutToBeRemoved() node:" <<
		_domNode.nodeName().string() << endl;
	Node *_node = 0L, *_nodePrev = 0L, *_nodeParent = 0L, *_nodeNext = 0L;
	Node *_nodeNextBackup = 0L, *_nodeParentBackup = 0L, *closingNode = 0L;
	Node *__nextNode = 0L;
	int startLine, startCol, endLine, endCol, taglenCol, taglenLine, j;
	int startLine2, startCol2, endLine2, endCol2;
	int NNbeginLine, NNbeginCol, NNlastLine, NNlastCol;
	bool b = false;

	/** FIRST PART: remove the HTML code from the editor*/

	coutTree(_rootNode, 2);
	if(_domNode.hasChildNodes())
	{
		kdDebug(25001)<< "WKafkaPart::slotDomNodeAboutToBeRemoved() - doesnt" <<
		 " support inserting nodes with childs for the moment. Sorry" << endl;
		return;
	}
	_node = searchCorrespondingNode(_domNode);
	if(!_node)
	{
		kdDebug(25001)<<"WKafkaPart::slotDomNodeModified() - *ERROR* the" <<
			" corresponding DOM::Node is not found!" << endl;
		return;
	}
	_nodeNextBackup = _node->next;
	_nodeParentBackup = _node->parent;
	if(!_node->parent)
	{
		kdDebug(25001)<<"WKafkaPart::slotDomNodeModified() - *ERROR2* trying" <<
			" to remove the rootNode" << endl;
		return;
	}
	_node->tag->beginPos(startLine, startCol);
	_node->tag->endPos(endLine, endCol);
	_currentDoc->editIf->removeText(startLine, startCol, endLine, endCol + 1);
	taglenCol = endCol - startCol;
	taglenLine = endLine - startLine;
	/**if(_node->kafkaAddon && _node->kafkaAddon->_closingNode)
	{
		_node->kafkaAddon->_closingNode->tag->beginPos(startLine2, startCol2);
		_node->kafkaAddon->_closingNode->tag->endPos(endLine2, endCol2);
		_currentDoc->editIf->removeText(startLine2, startCol2 - taglenCol, endLine2,
			endCol2 - taglenCol + 1);
	}*/

	/** SECOND PART: remove the node from the quanta tree and delete it */

	while(1)
	{
		_node->removeAll = false;
		_nodePrev = _node->prev;
		_nodeNext = _node->next;
		_nodeParent = _node->parent;
		if(_node->kafkaAddon && _node->kafkaAddon->_closingNode )
			closingNode = _node->kafkaAddon->_closingNode;
		if(_node->child)
			delete _node->child;
		delete _node;
		_node = 0L;

		if(_nodePrev)
		{
			if(_nodeNext)
				_nodePrev->next = _nodeNext;
			else
				_nodePrev->next = 0L;
		}
		if(_nodeNext)
		{
			if(_nodePrev)
				_nodeNext->prev = _nodePrev;
			else
				_nodeNext->prev = 0L;
		}
		if(_nodeParent->child == 0L)
		{//it means that it was _node before
			if(_nodePrev)
				_nodeParent->child = _nodePrev;
			else if(_nodeNext)
				_nodeParent->child = _nodeNext;
			else
				_nodeParent->child = 0L;
		}
		if(closingNode)
		{
			_node = closingNode;
			closingNode = 0L;
		}
		else break;
	}

	/** THIRD PART: change the neighbour Tag positions */

	__nextNode = (_nodeNextBackup)?_nodeNextBackup:_nodeParentBackup;
	__nextNode = getNextNode(__nextNode, b);
	while(__nextNode)
	{
		__nextNode->tag->beginPos(NNbeginLine, NNbeginCol);
		__nextNode->tag->endPos(NNlastLine, NNlastCol);
		if(NNbeginCol != endCol) break;
		__nextNode->tag->setTagPosition(NNbeginLine , NNbeginCol -
			taglenCol, NNlastLine , NNlastCol - taglenCol);
		for(j = 0; j < __nextNode->tag->attrCount(); j++)
		{
			if(__nextNode->tag->getAttribute(j).nameLine != endLine)
				break;
			//__nextNode->tag->getAttribute(j).nameLine -= taglenLine;
			__nextNode->tag->getAttribute(j).nameCol -= taglenCol;
			//__nextNode->tag->getAttribute(j).valueLine -= taglenLine;
			__nextNode->tag->getAttribute(j).valueCol -= taglenCol;
		}
		__nextNode = getNextNode(__nextNode, b);
	}
	coutTree(_rootNode, 2);
}

void WKafkaPart::slotDomNodeGetFocus(DOM::Node _domNode)
{
	/**Node *_node = searchCorrespondingNode(_domNode);
	if(_node)
	{
		int line, col;
		_node->tag->beginPos(line, col);
		emit newCursorPosition( line, col);
	}*/
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
		//_node->parent->kafkaNodeEnd.applyChanges();
		if(newNode.nodeName().string() == "TABLE")
		{
			newNode2 = _kafkaPart->createNode("TBODY");
			try
			{
				newNode2 = newNode.appendChild(newNode2);
			} catch(DOM::DOMException e)
			{kdDebug(25001)<< "WKafkart::synchronizeXMLTag() *ERROR* - code : " <<
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
			newNode = _kafkaPart->htmlDocument().appendChild(newNode);
		} catch(DOM::DOMException e)
		{kdDebug(25001)<< "WKafkart::synchronizeXMLTag() *ERROR* - code : " <<
			e.code << endl;}
		kdDebug(25001)<< "WKafkaPart::synchronizeXMLTag() *WARNING* - adding" <<
		" node to the root node" << endl;
		_node->kafkaAddon->_leafNode = newNode;
	}
	_node->kafkaAddon->_rootNode = newNode;

	if(_node->parent)
	kdDebug(25001)<< "Added DOM::Node : " <<
		_node->kafkaAddon->_rootNode.nodeName().string() <<
		" - Node : " << _node->tag->tagStr() <<
		" - parent DOM::Node : " <<
			_node->parent->kafkaAddon->_rootNode.nodeName().string() <<
		" - parent Node : " << _node->parent->tag->tagStr() << endl;
	else
	kdDebug(25001)<< "Added DOM::Node : " <<
		_node->kafkaAddon->_rootNode.nodeName().string() <<
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
	newNode.setNodeValue(NodeValue);
	if (newNode.isNull())
		return;

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
		{kdDebug(25001)<< "WKafkart::synchronizeTextTag() *ERROR* - code : " <<
			e.code << endl;}
		kdDebug(25001)<< "WKafkaPart::synchronizeTextTag() *WARNING* - adding" <<
			" node to the root node" << endl;
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

QString WKafkaPart::getEncodedChar(QString specialChar, bool encodeWhiteSpaces)
{
	if (!encodeWhiteSpaces && specialChar == " ") return "";
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

void WKafkaPart::coutTree(Node *node, int indent)
{
 QString output;
 while (node)
 {
   output = "";
   output.fill('.', indent);
   if (node->tag->type != Tag::Text)
       output += node->tag->name.replace('\n'," ");
   else
       output+= node->tag->tagStr().replace('\n'," ");
   kdDebug(25001) << output <<" (" << node->tag->type << ")" << endl;
   if (node->child)
       coutTree(node->child, indent + 4);
  // treeSize += sizeof(node) + sizeof(node->tag);
   //treeSize += node->size();
   //treeSize++;
   node = node->next;
 }
}
