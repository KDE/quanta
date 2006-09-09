/***************************************************************************
                               wkafkapart.cpp
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

#define HEAVY_DEBUG 1

#include <dom/dom_node.h>
#include <dom/dom_exception.h>
#include <dom/dom_string.h>
#include <dom/css_stylesheet.h>
#include <kdebug.h>
#include <khtmlview.h>
#include <kstandarddirs.h>
#include <ktexteditor/editinterface.h>
#include <ktexteditor/selectioninterface.h>
#include <kstandarddirs.h>
#include <klocale.h>
#include <kmultipledrag.h>
#include <kglobal.h>
#include <kcharsets.h>

#include <qregexp.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qdatetime.h>
#include <qclipboard.h>
#include <qptrvector.h>

#include "document.h"
#include "viewmanager.h"
#include "quantacommon.h"
#include "resource.h"
#include "undoredo.h"
#include "node.h"
#include "parser.h"
#include "project.h"
#include "tag.h"
#include "nodeproperties.h"
#include "htmlenhancer.h"
#include "kafkacommon.h"
#include "kafkaresource.h"
#include "cursors.h"
#include "kafkadragobject.h"
#include "cursors.h"

#include "wkafkapart.moc"

KafkaWidget *kafkaWidget;
KafkaDocument *kafkaDoc;

KafkaDocument::KafkaDocument(QWidget *parent, QWidget *widgetParent, const char *name)
:domNodeProps(1021), _docLoaded(false)
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

QString x = i18n("Sorry, VPL does not support this functionality yet.");
QString y = i18n("Merge cells");
QString z = i18n("Split cells");
QString aa = i18n("Edit CSS style of this Tag");
QString ab = i18n("Ident all");
//end

#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "KafkaDocument::KafkaDocument()" << endl;
#endif

	kafkaDoc = this;

	m_kafkaPart = new KafkaWidget(parent, widgetParent,this, name);
	kafkaWidget = m_kafkaPart;
	//m_kafkaPart->showDomTree();
	m_currentDoc = 0L;
	mainEnhancer =  new HTMLEnhancer(this);
	domNodeProps.setAutoDelete(false);

	KStandardDirs *m_stddirs = new KStandardDirs();
	QFile file( m_stddirs->findResource("data", "kafkapart/entities" )/**locate("appdata","chars") */);
	delete m_stddirs;
	QString tmp;
	if ( file.open(IO_ReadOnly) )
	{
		QTextStream t( &file );        // use a text stream
		t.setEncoding(QTextStream::UnicodeUTF8);
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

	connect(m_kafkaPart, SIGNAL(domNodeInserted(DOM::Node, bool, NodeModifsSet*)),
		this, SLOT(slotDomNodeInserted(DOM::Node, bool, NodeModifsSet*)));
	connect(m_kafkaPart, SIGNAL(domNodeModified(DOM::Node, NodeModifsSet*)),
		this, SLOT(slotDomNodeModified(DOM::Node, NodeModifsSet*)));
	connect(m_kafkaPart, SIGNAL(domNodeIsAboutToBeRemoved(DOM::Node, bool, NodeModifsSet*)),
		this, SLOT(slotDomNodeAboutToBeRemoved(DOM::Node, bool, NodeModifsSet*)));
	connect(m_kafkaPart, SIGNAL(domNodeIsAboutToBeMoved(DOM::Node, DOM::Node, DOM::Node, NodeModifsSet*)),
		this, SLOT(slotDomNodeIsAboutToBeMoved(DOM::Node, DOM::Node, DOM::Node, NodeModifsSet*)));

	connect(m_kafkaPart, SIGNAL(domNodeNewCursorPos(DOM::Node, int)),
		this, SLOT(slotdomNodeNewCursorPos(DOM::Node, int)));

}

KafkaDocument::~KafkaDocument()
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "KafkaDocument::~KafkaDocument()" << endl;
#endif
	//delete the empty node linked to the DOM::Node #document
	disconnectAllDomNodes();
}

void KafkaDocument::loadDocument(Document *doc)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001) << "KafkaDocument::loadDocument()" << endl;
#endif
	Node *node;
	Tag *tag;
	DOM::Node domNode;
	bool goUp;

	if(!m_kafkaPart) return;
	if(!doc) return;
#ifdef LIGHT_DEBUG
	QTime t;
	t.start();
#endif
	m_currentDoc = doc;

	(static_cast<HTMLEnhancer *>(mainEnhancer))->setBaseURL(Project::ref()->projectBaseURL());

	//create a empty document with a basic tree : HTML, HEAD, BODY
	m_kafkaPart->newDocument();

	// When loading a weird html file in khtml (e.g. without BODY or HTML), khtml takes care
	// to create the necessary tags. But as we are handling directly the Nodes, we have to handle this!!
	// creating and linking an empty node to the root DOM::Node (#document) and
	// to HEAD, HTML, BODY
	node = new Node(0L);
	tag = new Tag();
	tag->name = "#document";
	tag->setNotInTree(true);
	node->tag = tag;
	connectDomNodeToQuantaNode(m_kafkaPart->document(), node);
	node = new Node(0L);
	tag = new Tag();
	tag->name = "HTML";
	tag->setNotInTree(true);
	node->tag = tag;
	connectDomNodeToQuantaNode(m_kafkaPart->document().firstChild(), node);
	html = m_kafkaPart->document().firstChild();
	node = new Node(0L);
	tag = new Tag();
	tag->name = "HEAD";
	tag->setNotInTree(true);
	node->tag = tag;
	connectDomNodeToQuantaNode(m_kafkaPart->document().firstChild().firstChild(), node);
	head = m_kafkaPart->document().firstChild().firstChild();
	node = new Node(0L);
	tag = new Tag();
	tag->name = "BODY";
	tag->setNotInTree(true);
	node->tag = tag;
	connectDomNodeToQuantaNode(m_kafkaPart->document().firstChild().lastChild(), node);
	body = m_kafkaPart->document().firstChild().lastChild();

	//load the DOM::Nodes from the node tree.
	node = baseNode;
	while(node)
	{
#ifdef HEAVY_DEBUG
		kdDebug(25001) << "KafkaDocument::loadDocument - Node name :" <<
			node->tag->name.upper() << "; type : " <<
			node->tag->type << "; tagstr : " << node->tag->tagStr() <<
			" is opened :" << node->opened << endl;
#endif
		if(!buildKafkaNodeFromNode(node))
			emit loadingError(node);
		node = node->nextSibling();
	}

	//post process the DOM::Node Tree by adding Empty TEXT so that the cursor can go
	//everywhere the user wants.
	domNode = m_kafkaPart->document();
	goUp = false;
	while(!domNode.isNull())
	{
		mainEnhancer->postEnhanceNode(domNode);
		domNode = kafkaCommon::getNextDomNode(domNode, goUp);
	}

	m_kafkaPart->putCursorAtFirstAvailableLocation();
	_docLoaded = true;

	//Avoid moving objects...
	m_kafkaPart->stopAnimations();

	m_currentDoc->docUndoRedo->kafkaLoaded();
	emit loaded();

    m_currentDoc->docUndoRedo->syncKafkaCursorAndSelection(0);
	//m_kafkaPart->document().updateRendering();

#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "KafkaDocument::loadDocument() in " << t.elapsed() << " ms only!" << endl;
#endif
#ifdef HEAVY_DEBUG
	kafkaCommon::coutDomTree(m_kafkaPart->document(), 2);
	coutLinkTree(baseNode, 2);
#endif
}

void KafkaDocument::unloadDocument()
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "KafkaDocument::unloadDocument()" << endl;
#endif
	Node *node;

	domNodeProps.clear();
	//clean the kafkapart
	while(m_kafkaPart->document().hasChildNodes())
		m_kafkaPart->document().removeChild(m_kafkaPart->document().firstChild());
	m_currentDoc = 0L;

	html = body = head = DOM::Node();

	_docLoaded = false;
	node = baseNode;
	while(node)
	{
		if(node->rootNode())
			delete node->rootNode();
		node->setRootNode(0L);
		if(node->leafNode())
			delete node->leafNode();
		node->setLeafNode(0L);
		node = node->nextSibling();
	}
	emit unloaded();
}

void KafkaDocument::reloadDocument()
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "KafkaDocument::reloadDocument()" << endl;
#endif
	Node *node;
	int offsetX, offsetY;

	offsetX = m_kafkaPart->view()->contentsX();
	offsetY = m_kafkaPart->view()->contentsY();

	m_kafkaPart->closeURL();

	if(!_docLoaded)
		return;

	domNodeProps.clear();
	while(m_kafkaPart->document().hasChildNodes())
		m_kafkaPart->document().removeChild(m_kafkaPart->document().firstChild());
	node = baseNode;
	while(node)
	{
		if(node->rootNode())
			delete node->rootNode();
		node->setRootNode(0L);
		if(node->leafNode())
			delete node->leafNode();
		node->setLeafNode(0L);
		node = node->nextSibling();
	}

	/**KParts::URLArgs args(false, offsetX, offsetY);
	(static_cast<KParts::BrowserExtension *>(((KParts::ReadOnlyPart *)m_kafkaPart)->child( 0L,
		 "KParts::BrowserExtension" )))->setURLArgs( args );*/

	loadDocument(m_currentDoc);

	//m_kafkaPart->view()->setContentsPos(offsetX, offsetY);
}

kNodeAttrs *KafkaDocument::getAttrs(DOM::Node _domNode)
{
	return domNodeProps[_domNode.handle()];
}


Node *KafkaDocument::getNode(DOM::Node _domNode)
{
	if(_domNode.isNull() || _domNode.nodeName().string() == "#document")
	{
		kdDebug(25001)<< "KafkaDocument::getNode() - Bad Node given" <<
			endl;
		return 0L;
	}
	kNodeAttrs *props = domNodeProps[_domNode.handle()];
	if(!props)
	{
		kdDebug(25001)<< "KafkaDocument::getNode() -" <<
			" Corresponding Node not Found!!!" << endl;
		return 0L;
	}
	return props->getNode();
}

kNodeAttrs* KafkaDocument::connectDomNodeToQuantaNode(DOM::Node domNode, Node *node)
{
	QString name;
	kNodeAttrs *props;

	if(domNode.isNull())
	{
#ifdef LIGHT_DEBUG
		kdDebug(25001)<< "KafkaDocument::connectDomNodeToQuantaNode()" <<
			" - WARNING empty DOM::Node" << endl;
#endif
	}
	/**qtag = QuantaCommon::tagFromDTD(m_currentDoc->defaultDTD(),
		_domNode.nodeName().string());*/
	else
		name = domNode.nodeName().string().lower();

//     kdDebug(25001) << "KafkaDocument::connectDomNodeToQuantaNode() - domNode name: |" << name << "|" << endl;

	props = new kNodeAttrs();

	//We can split Nodes into several groups essentially for the deletion behavior:
	//1- Text
	//2- Block in which the cursor can't enter in when deleting e.g. TABLE, IMG
	//3- Block in which the cursor can't escape from e.g. TD, BODY
	//4- Block in which the cursor can enter in/escape from when deleting e.g. H1, DIV
	//5- Inline
	//6- "Invisible" Nodes e.g. HTML, HEAD

	//Hmmm... I guess we can do better here...
	if(domNode.nodeType() == DOM::Node::TEXT_NODE)
	{
		props->setCHCursorFocus(kNodeAttrs::textNode);
		props->setCCEnter(true);
		props->setCBModified(true);
		props->setCBDeleted(true);
	}
	else if(name == "abbr" || name == "acronym" || name == "address" || name == "b" || name == "bdo" ||
		name == "big" || name == "cite" || name == "code" ||
		name == "del" || name == "dfn" || name == "dir" ||
		name == "em" || name == "fieldset" || name == "font" || name == "i" || name == "iframe" ||
		name == "ins" || name == "kbd" || name == "label" || name == "legend" || name == "menu" ||
		name == "noframes" || name == "pre" || name == "s" || name == "samp" ||
		name == "small" || name == "span" || name == "strike" || name == "strong" || name == "sub" ||
		name == "sup" || name == "tt" || name == "u" || name == "var" || name == "a" ||
		name == "blockquote" ||
		name == "em" || name == "form" || name == "ins" ||
		name == "q" ||  name == "tt" )
	{
		props->setCHCursorFocus(kNodeAttrs::inlineNode);
		props->setCCEnter(true);
		props->setCBModified(true);
		props->setCBDeleted(true);
	}
	else if(name == "center" || name == "li" || name == "h1" || name == "h2" || name == "h3" ||
		name == "h4" || name == "h5" || name == "h6" || name == "div" || name == "dd" ||
		name == "dt" || name == "p")
	{
		props->setCHCursorFocus(kNodeAttrs::blockNode);
		props->setCCEnter(true);
		props->setCBModified(true);
		props->setCBDeleted(true);
	}
	else if(name == "applet" || name == "button" || name == "img" || name == "map" || name == "object" ||
		name == "hr" || name == "input" || name == "select" || name == "table" || name == "textarea" ||
		 name == "br" || name == "dl" || name == "ul" || name == "ol")
	{
		props->setCHCursorFocus(kNodeAttrs::singleNodeAndItself);
		props->setCCEnter(false);
		props->setCBModified(true);
		props->setCBDeleted(true);
	}
	else if(name == "basefont" || name == "location" || name == "fieldset" || name == "noscript" ||
		name == "script")
	{
		props->setCHCursorFocus(kNodeAttrs::no);
		props->setCCEnter(false);
		props->setCBModified(true);
		props->setCBDeleted(true);
	}
	else if(name == "caption" || name == "frame" || name == "frameset" || name == "isindex" ||
		name == "optgroup" || name == "param" || name == "title" || name == "area" || name == "base" ||
		name == "body" || name == "col" || name == "colgroup" || name == "head" || name == "html" ||
		name == "link" || name == "meta" || name == "option" || name == "style" || name == "tbody" ||
		name == "td" || name == "tfoot" || name == "th" || name == "thead" || name == "tr")
	{
		props->setCHCursorFocus(kNodeAttrs::no);
		props->setCCEnter(false);
		props->setCBModified(false);
		props->setCBDeleted(false);
	}
	else
	{
		kdDebug(25001)<< "KafkaDocument::connectDomNodeToQuantaNode () - " <<
			"No QTag found! Setting default parameters..." << endl;
		props->setCHCursorFocus(kNodeAttrs::no);
		props->setCCEnter(false);
		props->setCBModified(false);
		props->setCBDeleted(false);
	}

#ifdef HEAVY_DEBUG
	kdDebug(25001)<< "KafkaDocument::connectDomNodeToQuantaNode() - tag name :" <<
		name << " canBeDeleted:" << props->cbDel() << " canBeModified:" <<
		props->cbMod() << " canHaveCursorFocus:" << props->chCurFoc() <<
		" cursorCanEnter:" << props->ccanEnter() << endl;
#endif
	props->setNode(node);
	props->setDomNode(domNode);
	domNodeProps.insert(domNode.handle(), props);

	return props;
}

void KafkaDocument::disconnectDomNodeFromQuantaNode(DOM::Node _domNode)
{
	if(_domNode.isNull())
		return;
	domNodeProps.remove(_domNode.handle());
}

void KafkaDocument::disconnectAllDomNodes()
{
	domNodeProps.clear();
}

void KafkaDocument::setCursorAndSelection(NodeSelectionInd *nodeSelection)
{
  bool cursorAtSelectionStart;
  DOM::Node startDomNode, endDomNode, foo;
  Node *startNode, *endNode;
  long startOffset, endOffset;

  cursorAtSelectionStart = nodeSelection->cursorAtSelectionStart();
  startNode = kafkaCommon::getNodeFromLocation(nodeSelection->cursorNode());
  endNode = kafkaCommon::getNodeFromLocation(nodeSelection->cursorNodeEndSel());

  if(startNode && startNode->rootNode())
    startDomNode = *(startNode->rootNode());
  if(endNode && endNode->rootNode())
    endDomNode = *(endNode->rootNode());
  translateNodeIntoKafkaCursorPosition(startNode, nodeSelection->cursorOffset(), foo, startOffset);
  translateNodeIntoKafkaCursorPosition(endNode, nodeSelection->cursorOffsetEndSel(), foo, endOffset);


  if(cursorAtSelectionStart && !startDomNode.isNull())
  {
    m_kafkaPart->setCurrentNode(startDomNode, startOffset);
  }
  else if(!cursorAtSelectionStart && !endDomNode.isNull())
  {
    m_kafkaPart->setCurrentNode(endDomNode, endOffset);
  }

  if(!startDomNode.isNull() && !endDomNode.isNull())
    m_kafkaPart->setSelection(DOM::Range(startDomNode, (long)startOffset, endDomNode, (long)endOffset));
}

void KafkaDocument::setCursor(Node* cursorNode, int cursorOffset)
{
    DOM::Node domNode;
    long longDomNodeOffset;
    translateNodeIntoKafkaCursorPosition(cursorNode, cursorOffset, domNode, longDomNodeOffset);
    if (!domNode.isNull() && domNode.nodeType() != DOM::Node::TEXT_NODE &&
         !domNode.firstChild().isNull() && domNode.firstChild().nodeType() == DOM::Node::TEXT_NODE)
        domNode = domNode.firstChild();
    if (!domNode.isNull())
        m_kafkaPart->setCurrentNode(domNode, (int)longDomNodeOffset);
}

bool KafkaDocument::buildKafkaNodeFromNode(Node *node, bool insertNode)
{
#ifdef LIGHT_DEBUG
	if(node)
		kdDebug(25001)<< "KafkaDocument::buildKafkaNodeFromNode() " << endl;
#endif
	DOM::Node newNode, newNode2, attr, nextNode, parentNode, *ptDomNode;
	bool removeLeftWhitespaces, removeRightWhitespaces;
	QString str, nodeValue;
	Node *n, *parent;
	int i;

//     Don't create DOM::Nodes from Quanta empty nodes outside the body or inside other not allowed element, or KHTML
//     will give us problems.
    bool canInsertEmptyNode = false;
    if(node->tag->type == Tag::Empty)
    {
        if(!m_currentDoc->defaultDTD()->name.contains("HTML", false))
            canInsertEmptyNode = true;
        else
            canInsertEmptyNode = kafkaCommon::hasParent(node, "body");

        Node* parent_node = node->parent;
        QTag* parent_node_description_tag = QuantaCommon::tagFromDTD(parent_node);
        if(parent_node_description_tag && !parent_node_description_tag->isChild(node, false, true))
            canInsertEmptyNode = false;
    }

    if(node->tag->type == Tag::XmlTag ||
       ((node->tag->type == Tag::Text || (node->tag->type == Tag::Empty && canInsertEmptyNode)) && !node->insideSpecial))
    {
		str = node->tag->name.lower();

		//The basics DOM::Nodes HTML, HEAD and BODY are loaded anyway, but we must now
		// link the real  HTML,... to their Nodes.
		//A basic Common tree is !doctype<-html<-(head, body)
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
                if(node->tag->type == Tag::Text || node->tag->type == Tag::Empty)
                {
					newNode = kafkaCommon::createTextDomNode("", m_kafkaPart->document());
				}
				else
				{
					newNode = kafkaCommon::createDomNode(node, m_kafkaPart->document());
				}
			}
		}
		else if(str == "html" && (!node->parent || (node->parent && !node->parent->parent)))
		{
			if(!html.isNull())//delete the empty Node
				disconnectDomNodeFromQuantaNode(html);
			newNode = html;
			insertNode = false;
			}
		else if(str == "body" && ((node->parent && !node->parent->parent) || (node->parent &&
			 node->parent->parent && !node->parent->parent->parent)))
		{
			if(!body.isNull())
				disconnectDomNodeFromQuantaNode(body);
			newNode = body;
			insertNode = false;
		}
		else if(str == "head" && ((node->parent && !node->parent->parent) || (node->parent &&
			 node->parent->parent && !node->parent->parent->parent)))
		{
			if(!head.isNull())
				disconnectDomNodeFromQuantaNode(head);
			newNode = head;
			insertNode = false;
		}
	/**	else if(node->parent->tag->str == "html")*/
		else
		{
            if(node->tag->type == Tag::Text || node->tag->type == Tag::Empty)
            {
				newNode = kafkaCommon::createTextDomNode("", m_kafkaPart->document());
			}
			else
			{
				newNode = kafkaCommon::createDomNode(node->tag->name, m_currentDoc->defaultDTD(),
					m_kafkaPart->document());
			}
		}

		if(newNode.isNull())
		{
#ifdef LIGHT_DEBUG
			kdDebug(25001)<< "KafkaDocument::buildKafkaNodeFromNode() - ERROR null newNode" << endl;
#endif
			return false;
		}

		connectDomNodeToQuantaNode(newNode, node);
        if(node->tag->type == Tag::Text || node->tag->type == Tag::Empty)
        {
			//Now we get if the whitespaces at the left and right are kept or not.
			//Left whitespaces are removed if:
			//- It is the first child of a BLOCK
			//- Previous Node(skipping inlines) is a text with spaces at the end
			//Right whitespaces are removed if:
			//- It is the last child of a BLOCK
			removeLeftWhitespaces = false;
			n = node;
			parent = node->parent;
			while(parent && parent->SFirstChild() == n)
			{
				if(!kafkaCommon::isInline(parent->tag->name))
				{
					removeLeftWhitespaces = true;
					break;
				}
				n = parent;
				parent = parent->parent;
			}
			if(!removeLeftWhitespaces)
			{
				n = node;
				n = kafkaCommon::getPrevNodeNE(n);
				while(n && (n->tag->type == Tag::XmlTagEnd ||
					(n->tag->type == Tag::XmlTag && kafkaCommon::isInline(n->tag->name) &&
                                        !n->tag->single)))
					n = kafkaCommon::getPrevNodeNE(n);
				if(n && n->tag->type == Tag::Text)
				{
					nodeValue = n->tag->tagStr();
					if(nodeValue.length() > 0 && nodeValue[nodeValue.length() - 1].isSpace())
						removeLeftWhitespaces = true;
				}

			}

			removeRightWhitespaces = false;
			n = node;
			parent = node->parent;
			while(parent && parent->SLastChild() == n)
			{
				if(!kafkaCommon::isInline(parent->tag->name))
				{
					removeRightWhitespaces = true;
					break;
				}
				n = parent;
				parent = parent->parent;
			}

			nodeValue = node->tag->tagStr();
			nodeValue = getDecodedText(nodeValue, !kafkaCommon::hasParent(node, "pre"),
				removeLeftWhitespaces, removeRightWhitespaces);
			newNode.setNodeValue(nodeValue);
		}

		for(i = 0; i < node->tag->attrCount(); i++)
		{
			attr = kafkaCommon::createDomNodeAttribute(node, node->tag->attribute(i),
				m_kafkaPart->document());
			if(!attr.isNull())
			{
				//TODO: create a createAttr function and add this (setNodeValue sometimes
				//don't like null QString)
				if(!node->tag->attributeValue(i).isNull())
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
			ptDomNode = new DOM::Node(newNode);
			node->setRootNode(ptDomNode);
			n = node;
			while(n->next)
			{
				n = n->next;
				if(n->rootNode())
				{
					nextNode = *n->rootNode();
						break;
				}
			}
			if(node->parent && node->parent->leafNode())
				parentNode = *node->parent->leafNode();
			else if(node->parent && !node->parent->leafNode())
			{
				//the parent tag was invalid and khtml refuse to insert it
				//so impossible to inser the current node
				disconnectDomNodeFromQuantaNode(newNode);
				if(node->rootNode())
					delete node->rootNode();
				node->setRootNode(0L);
				return false;
			}
			else
				parentNode = body;

			//Set the visual enhancements.
			ptDomNode = new DOM::Node(newNode);
			node->setLeafNode(ptDomNode);
			mainEnhancer->enhanceNode(node, parentNode, nextNode);

            QTag* qTag = QuantaCommon::tagFromDTD(getCurrentDoc()->defaultDTD(),
                    parentNode.nodeName().string());

            if(qTag && qTag->isChild(node, false))
            {

                if(nextNode.isNull())
                {
                    if(!kafkaCommon::insertDomNode(newNode, parentNode))
                    {
                        disconnectDomNodeFromQuantaNode(newNode);
                        if(node->rootNode())
                            delete node->rootNode();
                        node->setRootNode(0L);
                        return false;
                    }
                }
                else
                {
                    if(!kafkaCommon::insertDomNode(newNode, parentNode, nextNode))
                    {
                        disconnectDomNodeFromQuantaNode(newNode);
                        if(node->rootNode())
                            delete node->rootNode();
                        node->setRootNode(0L);
                        return false;
                    }
                }
            }
        }
		else
		{
			ptDomNode = new DOM::Node(newNode);
			node->setRootNode(ptDomNode);
			ptDomNode = new DOM::Node(newNode);
			node->setLeafNode(ptDomNode);
		}
	}
	else
	{
		if(node->parent && node->parent->leafNode())
			parentNode = *node->parent->leafNode();
		else
			parentNode = body;
		n = node;
		while(n->next)
		{
			n = n->next;
			if(n->rootNode())
			{
				nextNode = *n->rootNode();
				break;
			}
		}
		mainEnhancer->enhanceNode(node, parentNode, nextNode);
	}
	return true;
}

void KafkaDocument::buildNodeFromKafkaNode(Node *node, DOM::Node domNode)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "void KafkaDocument::buildNodeFromKafkaNode() - Node* DOM::Node" << endl;
#endif
	long i;

	if(!node) return;

	node->tag->setCleanStrBuilt(false);
  node->tag->setIndentationDone(false);
	if(domNode.nodeType() == DOM::Node::TEXT_NODE)
	{
    QString text = domNode.nodeValue().string();
    text.replace("<", "&lt;");
    text.replace(">", "&gt;");
		node->tag->setStr(text);
	}
	else
	{
		while(node->tag->attrCount())
			node->tag->deleteAttribute(0);
		for(i = 0; (unsigned)i < domNode.attributes().length(); i++)
		{
			TagAttr attr;
			attr.name = domNode.attributes().item(i).nodeName().string();
			attr.value = domNode.attributes().item(i).nodeValue().string();
			attr.quoted = true;
			node->tag->addAttribute(attr);
		}
	}
}

Node * KafkaDocument::buildNodeFromKafkaNode(DOM::Node domNode, Node *nodeParent,
	Node *beginNode, int beginOffset, Node  */*endNode*/, int endOffset, NodeModifsSet *modifs)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "Node* KafkaDocument::buildNodeFromKafkaNode() - DOM::Node 2xNode* int: " <<
		beginOffset << " Node* int: " << endOffset << " NodeModifsSet " << endl;
#else
        Q_UNUSED(beginOffset);
        Q_UNUSED(endOffset);
#endif
	DOM::Node *ptDomNode;
	Node *node;

	if(domNode.isNull())
	{
		kdDebug(25001)<< "Node* KafkaDocument::buildNodeFromKafkaNode(DOM::Node, 2xNode*)" <<
			" *ERROR* - empty _domNode"<< endl;
	}

	//nodeParent can be the false body node which is not in the tree.
	if(nodeParent->tag->notInTree())
		nodeParent = 0L;

	/**_node = new Node(_nodeParent);*/
	if(domNode.nodeType() == DOM::Node::TEXT_NODE)
	{
		node = kafkaCommon::createNode("#text", "",Tag::Text, m_currentDoc);
	}
	else
	{
		node = kafkaCommon::createNode(domNode.nodeName().string(), "",
			Tag::XmlTag, m_currentDoc);
	}
	buildNodeFromKafkaNode(node, domNode);

	connectDomNodeToQuantaNode(domNode, node);

	ptDomNode = new DOM::Node(domNode);
	node->setRootNode(ptDomNode);
	ptDomNode = new DOM::Node(domNode);
	node->setLeafNode(ptDomNode);

	kafkaCommon::insertNode(node, nodeParent, beginNode, beginNode, modifs, false);

	return node;
}

QString KafkaDocument::getDecodedChar(const QString &encodedChar)
{
	QMap<QString, QString>::Iterator it = encodedChars.find(encodedChar);
	if(it == encodedChars.end())
        {
          //try this
	  return KGlobal::charsets()->resolveEntities(encodedChar);;
        }
	return it.data();
}

QString KafkaDocument::getDecodedText(const QString &a_encodedText, bool translateWhiteSpacesAndLineBreaks,
		bool removeLeftWhitespaces, bool removeRightWhitespaces)
{
  QString encodedText = a_encodedText;
  if (encodedText.isEmpty())
    encodedText = " ";
	QString decodedChar;
	int i, j;
#ifdef LIGHT_DEBUG
	QString oldEncodedText = encodedText;
#endif

	i = -1;
	while((unsigned)++i < encodedText.length() && translateWhiteSpacesAndLineBreaks)
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

	if(translateWhiteSpacesAndLineBreaks && removeLeftWhitespaces && encodedText.length() > 0 &&
		encodedText[0].isSpace())
		encodedText.remove(0, 1);

	if(translateWhiteSpacesAndLineBreaks && removeRightWhitespaces && encodedText.length() > 0 &&
		encodedText[encodedText.length() - 1].isSpace())
		encodedText.remove(encodedText.length() - 1, 1);

#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "KafkaDocument::getDecodedText() - \"" << oldEncodedText << "\" -> \"" <<
		encodedText << "\"" << endl;
#endif
	return encodedText;
	//return KGlobal::charsets()->resolveEntities(encodedText); =>nice but not sufficient
}

QString KafkaDocument::getEncodedChar(const QString &decodedChar, const QString &previousDecodedChar)
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

QString KafkaDocument::getEncodedText(const QString &a_decodedText, int bLine, int bCol, int &eLine, int &eCol,
	bool translateWhiteSpaces)
{
        QString decodedText = a_decodedText;
	QString Encodedchar;
	QString decodedChar, previousDecodedChar;
#ifdef LIGHT_DEBUG
	QString oldDecodedText = decodedText;
#endif
	int i;
#ifdef LIGHT_DEBUG
	int _bLine = bLine, _bCol = bCol;
#endif

	i = -1;
	while((unsigned)++i < decodedText.length())
	{
		previousDecodedChar = decodedChar;
		decodedChar = QString(decodedText[i]);

		if(translateWhiteSpaces || !decodedText[i].isSpace())
			Encodedchar = getEncodedChar(QString(decodedText[i]),
				(i>=1)?previousDecodedChar:QString(""));
		else
			Encodedchar = decodedChar;
		bCol += Encodedchar.length();

		decodedText.remove(i,1);
		decodedText.insert(i, Encodedchar);
		i += Encodedchar.length() - 1;
	}
	eLine = bLine;
	eCol = bCol - 1;
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "KafkaDocument::getEncodedText() - " << oldDecodedText << " -> " << decodedText <<
		" - " << _bLine << ":" << _bCol << " - " << eLine << ":" << eCol << endl;
#endif
	return decodedText;
}

QString KafkaDocument::getEncodedText(const QString& decodedText)
{
	int a = 0, b = 0, c, d;
	return getEncodedText(decodedText, a, b, c, d);
}

QString KafkaDocument::generateCodeFromNode(Node *node, int bLine, int bCol, int &eLine, int &eCol, bool encodeText)
{
	QString text, _char;
	Node *openingNode;
	int bLineAttr, bColAttr;
	int j = 0;
        bool hasPreParent;

	if(!node) return "";

	if(node->tag->type == Tag::XmlTag)
	{
		text = "<" + QuantaCommon::tagCase(node->tag->name);
		bCol += node->tag->name.length() + 1;
		for(j = 0; j < node->tag->attrCount(); j++)
		{
			text += " ";
			bCol++;
			bLineAttr = bLine;
			bColAttr = bCol;
			text += node->tag->attribute(j);
			bCol += node->tag->attribute(j).length();

			//doctype have only attrNames.
			if(node->tag->name.lower() != "!doctype" && !node->tag->getAttribute(j).special)
			{
				text += "=";
				bCol++;
				if(node->tag->isQuotedAttribute(j))
				{
					text += qConfig.attrValueQuotation;
					bCol++;
				}
				node->tag->setAttributePosition(j, bLineAttr, bColAttr, bLine, bCol);
				text += node->tag->attributeValue(j);
				bCol += node->tag->attributeValue(j).length();
				if(node->tag->isQuotedAttribute(j))
				{
					text += qConfig.attrValueQuotation;
					bCol++;
				}
			}
			else
				node->tag->setAttributePosition(j, bLineAttr, bColAttr, -2, -2);
		}

		//only single Nodes except !doctype and ?xml nodes in XML tag style get the "/"
		if ( node->tag->dtd()->singleTagStyle == "xml" &&
			(node->tag->single || (!qConfig.closeOptionalTags &&
			QuantaCommon::isOptionalTag(node->tag->dtd()->name, node->tag->name)))
			 && node->tag->name.lower() != "?xml" && node->tag->name.lower() != "!doctype")
		{
			text += " /";
			bCol += 2;
		}
		//?xml nodes get a "?"
		if(node->tag->name.lower() == "?xml")
		{
			text += "?";
			bCol++;
		}

		text += ">";
		eCol = bCol;
		eLine = bLine;
	}
	else if(node->tag->type == Tag::XmlTagEnd)
	{
		openingNode = node->getOpeningNode();
		if(openingNode && openingNode->tag->type == Tag::ScriptTag)
		{
                  if(openingNode->tag->name.contains("XML PI", false) ||
                    openingNode->tag->name.contains("PHP", false))
                    text = "?>";
                  else if(openingNode->tag->name.contains("DTD", false))
                    text = ">";
                  else
                    text = ">";
		}
		else
    {
      if (node->tag->tagStr() == "-->")
        text = "-->";
      else
			 text = "<" + QuantaCommon::tagCase(node->tag->name) + ">";
    }
		bCol += text.length();
		eCol = bCol - 1;
		eLine = bLine;
	}
	else if(node->tag->type == Tag::Text)
	{
          hasPreParent = kafkaCommon::hasParent(node, "pre");
          if(encodeText)
            text = getEncodedText(node->tag->tagStr(), bLine, bCol, eLine, eCol,
              !hasPreParent);
            /** Can't use KGlobal::charsets()->toEntity() :
             * It translate all chars into entities! */
          else if(!hasPreParent)
            text = node->tag->tagStr().replace(QRegExp("\\s+"), " ");
          else
            text = node->tag->tagStr();
	}
	else if(node->tag->type == Tag::ScriptTag)
	{
		//WARNING : HTML SPECIFIC
        if(node->tag->name.contains("style", false))
		{
			text = "<" + QuantaCommon::tagCase("style") + ">";
		}
		else if(node->tag->name.contains("DTD", false))
		{
			text = "<!";
		}
		else if(node->tag->name.contains("XML PI", false))
		{
			text = "<?xml";
		}
        else if(node->tag->name.contains("PHP", false))
        {
            text = "<?php";
        }
		bCol += text.length();
		eCol = bCol - 1;
		eLine = bLine;
	}
	else
	{
		//default behavior : return node->tag->tagStr()
		text = node->tag->tagStr();
		kafkaCommon::getEndPosition(text, bLine, bCol, eLine, eCol);
	}
	return text;
}

void KafkaDocument::translateQuantaIntoKafkaCursorPosition(uint curLine, uint curCol, DOM::Node &domNode, long &offset)
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
				if(!node->rootNode())
				{
					offset = 0;
					kdDebug(25001)<< "KafkaDocument::getKafkaCursorPosition() - ERROR DOM::Node not found"
						<< endl;
					return;
				}
				domNode = *node->rootNode();
				offset = domNode.nodeValue().string().length();
#ifdef LIGHT_DEBUG
				kdDebug(25001)<< "KafkaDocument::getKafkaCursorPosition() - " <<
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
		kdDebug(25001)<< "KafkaDocument::getKafkaCursorPosition() - ERROR node not found" << endl;
		return;
	}
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "KafkaDocument::getKafkaCursorPosition() - node :" << node->tag->name << ": type:" <<
		node->tag->type << ": pos " << bLine << ":" << bCol << " - " << eLine << ":" << eCol << endl;
#endif

	offset = 0;
	if(node->rootNode() && node->rootNode()->nodeType() == DOM::Node::TEXT_NODE)
	{
		domNode = *node->rootNode();
		currentLine = m_currentDoc->editIf->textLine(line);
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
					currentLine = m_currentDoc->editIf->textLine(line);
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
	else if(node->rootNode())
		offset = 0;//shoud we select?
	else
		m_kafkaPart->putCursorAtFirstAvailableLocation();//set the cursor in the first text
#ifdef LIGHT_DEBUG
	if(!domNode.isNull())
		kdDebug(25001)<< "KafkaDocument::getKafkaCursorPosition() - " << domNode.nodeName().string() <<
			":" << offset << endl;
	else
		kdDebug(25001)<< "KafkaDocument::getKafkaCursorPosition() - NULL domNode" << endl;
#endif
}

void KafkaDocument::translateQuantaIntoNodeCursorPosition(uint line, uint col, Node **node, long &offset)
{
  int curCol, curLine, beginCol, beginLine;
  QString currentLine;

  *node = parser->nodeAt(line, col, false);

  offset = 0;
  if(!*node)
   return;

  if((*node)->tag->cleanStrBuilt() && (*node)->tag->indentationDone())
  {
    (*node)->tag->beginPos(beginLine, beginCol);
    curLine = beginLine;
    curCol = beginCol;
    while(curLine < (signed)line)
    {
      currentLine = ViewManager::ref()->activeDocument()->editIf->textLine(curLine);
      if(curLine == beginLine)
        offset += (signed)currentLine.length() - beginCol;
      else
        offset += (signed)currentLine.length();
      offset++;
      curLine++;
    }
    if(beginLine != (signed)line)
      offset += col;
    else
      offset += col - beginCol;
  }
  else
  {
    //TODO
  }
#ifdef LIGHT_DEBUG
  kdDebug(25001)<< "KafkaDocument::translateQuantaIntoNodeCursorPosition() - " << *node <<
    ":" << offset << endl;
#endif
}

void KafkaDocument::translateKafkaIntoNodeCursorPosition(DOM::Node domNode, long domNodeOffset, Node **node, long &offset)
{
	QString decodedText, encodedChar, encodedText, currentChar;
	QChar curChar, oldChar;
	long currentOffset;
	bool waitForSpace = false, found = false;
	int curNodeOffset, bLine = 0, bCol = 0, eLine, eCol;

        offset = 0;
        (*node) = 0L;

	if(domNode.isNull())
	{
#ifdef HEAVY_DEBUG
		kdDebug(25001)<< "KafkaDocument::translateKafkaIntoNodeCursorPosition()" <<
			" - DOM::Node not found!" << endl;
#endif
		return ;
	}

	//get the corresponding Node*
	(*node) = getNode(domNode);
	if(!(*node))
	{
#ifdef HEAVY_DEBUG
		kdDebug(25001)<< "KafkaDocument::translateKafkaIntoNodeCursorPosition()" <<
			" - Node not found!" << endl;
#endif
		return ;
	}
  if(!(*node)->tag)
  {
#ifdef HEAVY_DEBUG
    kdDebug(25001)<< "KafkaDocument::translateKafkaIntoNodeCursorPosition()" <<
      " - Node tag not found!" << endl;
#endif
    (*node) = 0L;
    return ;
  }

	//If this node is selected (-1 means selected) then return "node selected" ;-)
	if(domNodeOffset == -1)
	{
		return ;
	}

	if(!(*node)->tag->cleanStrBuilt())
	{
		//We NEED to have the up-to-date string in node.
		(*node)->tag->setStr(generateCodeFromNode((*node), bLine, bCol, eLine, eCol));
		//FIXME we shouldn't set it but if we don't the text will be re-encoded!
		(*node)->tag->setCleanStrBuilt(true);
	}

	decodedText = domNode.nodeValue().string();
	encodedText = (*node)->tag->tagStr();
	currentOffset = domNodeOffset;
	curNodeOffset = 0;

#ifdef HEAVY_DEBUG
	kdDebug(25001)<< "KafkaDocument::translateKafkaIntoNodeCursorPosition() - decodedText:"<<
		decodedText << ": encoded text:" << encodedText << ":" << endl;
#endif

	if((*node)->tag->type == Tag::Text)
	{
		while(currentOffset > 0)
		{
			curChar = decodedText.at(domNodeOffset - currentOffset);
#ifdef HEAVY_DEBUG
			//kdDebug(24000)<< "CurChar:" << QString(curChar) << ": oldChar:" <<
			//	QString(oldChar) << endl;
#endif
			encodedChar = getEncodedChar(QString(curChar), QString(oldChar));
			if(encodedChar == "&nbsp;")
				encodedChar = " ";
			found = false;
			waitForSpace = false;
			while(!found)
			{
#ifdef HEAVY_DEBUG
				//kdDebug(25001)<< "look 4 :" << encodedChar << ": found :" <<
				//	encodedText.mid(curNodeOffset, encodedChar.length()) << endl;
#endif
				if(encodedChar != " " && encodedChar == encodedText.mid(curNodeOffset,
					encodedChar.length()))
					found = true;
				else if(encodedChar == " " && encodedText.mid(curNodeOffset, 1).at(0).isSpace())
					waitForSpace = true;
				else if(waitForSpace)
					break;//no more spaces
				else if(encodedChar == " " && encodedText.mid(curNodeOffset, 6) == "&nbsp;")
				{
					encodedChar = "&nbsp;";
					found = true;
				}

				if(curNodeOffset > (signed)encodedText.length() || encodedChar.isEmpty())
				{
					//The good nodeOffset was not found. Returns a default value.
					//encodedChar.isEmpty() -->prevent an infinite loop
					curNodeOffset = 0;
					break;
				}
				else
					curNodeOffset += encodedChar.length();
			}
			oldChar = curChar;
			currentOffset--;
		}
	}

#ifdef LIGHT_DEBUG
	kdDebug(25001)<<"KafkaDocument::translateKafkaIntoNodeCursorPosition() - " <<
		curNodeOffset << endl;
#endif
	offset = curNodeOffset;
}

void KafkaDocument::translateKafkaIntoQuantaCursorPosition(DOM::Node _currentDomNode, int offset, int &line, int &col)
{
	Node *_currentNode, *closingNode, *node;
	kNodeAttrs *attrs;
	QString decodedText, encodedChar, currentLine, currentChar;
	QChar curChar, oldChar;
	int currentOffset;
	int curLine, curCol, endLine, endCol;
	int i;
	bool waitForSpace = false, found = false;
	bool tagLeft = false, tagRight = false, tagMiddle = false;
	bool specialBehavior = false;

	//m_kafkaPart->getCurrentNode(_currentDomNode, offset);
	currentOffset = offset;

	if(_currentDomNode.isNull())
	{
		kdDebug(25001)<< "KafkaDocument::getQuantaCursorPosition() - DOM::Node not found!" << endl;
		line = 0;
		col = 0;
		return;
	}

	attrs = getAttrs(_currentDomNode);
	if(!attrs)
	{
		kdDebug(25001)<< "KafkaDocument::getQuantaCursorPosition() - Attrs not found!" << endl;
		line = 0;
		col = 0;
		return;
	}

	//If the current DOM::Node has a special behavior (cf nodeproperties.h), get the nearest
	//node which can get the focus!
	if(attrs->specialBehavior() != kNodeAttrs::none)
	{
		specialBehavior = true;
		if(attrs->specialBehavior() == kNodeAttrs::emptyTextSurroundingBlockElementAtTheLeft)
		{
			_currentDomNode = _currentDomNode.nextSibling();
			//tagRight means to put the cursor at the left of the tag so that the cursor
			//looks at its right place (otherwise as there is no Node for this DOM::Node,
			//the cursor won't go anywhere!)
			tagLeft = true;
		}
		else if(attrs->specialBehavior() == kNodeAttrs::emptyTextSurroundingBlockElementAtTheRight)
		{
			_currentDomNode = _currentDomNode.previousSibling();
			tagRight = true;
		}
		else if(attrs->specialBehavior() == kNodeAttrs::emptyTextAsChildOfAChildlessElement)
		{
			_currentDomNode = _currentDomNode.parentNode();
			tagMiddle = true;
		}
		attrs = getAttrs(_currentDomNode);
		if(!attrs)
		{
			kdDebug(25001)<< "KafkaDocument::getQuantaCursorPosition() - Attrs not found!" << endl;
			line = 0;
			col = 0;
			return;
		}
	}

	_currentNode = attrs->getNode();
  if(!_currentNode /* && attrs->specialBehavior() == kNodeAttrs::none -- Andras: what happens if _currentNode is null, but this part of the if is not true?*/)
	{
		kdDebug(25001)<< "KafkaDocument::getQuantaCursorPosition() - Node not found!" << endl;
		line = 0;
		col = 0;
		return;
	}

	decodedText = _currentDomNode.nodeValue().string();
	_currentNode->tag->beginPos(curLine, curCol);
	_currentNode->tag->endPos(endLine, endCol);
	currentLine = m_currentDoc->editIf->textLine(curLine);
	if(curLine < endLine)
		currentLine += " ";//remplace the \n

	if(offset == -1)
	{
    if (m_currentDoc->selectionIf)
		  m_currentDoc->selectionIf->setSelection(curLine, curCol, endLine, endCol + 1);
		line = endLine;
		col = endCol + 1;
		return;
	}
#ifdef HEAVY_DEBUG
//	kdDebug(25001)<< "KafkaDocument::getQuantaCursorPosition() - decodedText:"<< decodedText << ":" << endl;
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
					++curLine;
					curCol = 0;
					currentLine = m_currentDoc->editIf->textLine(curLine);
					if(curLine < endLine)
						currentLine += " ";//remplace the \n
				}
				else
				{
						curCol += encodedChar.length();
				}
			}
			oldChar = curChar;
			--currentOffset;
		}
	}
	else if(_currentNode->tag->type == Tag::XmlTag)
	{
		//If we are in the special case
		if(specialBehavior)
		{
			if(tagLeft)
				_currentNode->tag->beginPos(curLine, curCol);
			else if(tagRight)
			{
				closingNode = _currentNode->getClosingNode();
				if(closingNode)
					closingNode->tag->endPos(curLine, curCol);
				else
					_currentNode->tag->endPos(curLine, curCol);
				++curCol;
			}
			else
			{
				_currentNode->tag->endPos(curLine, curCol);
				++curCol;
			}
		}
		else
		{
			if(_currentNode->tag->single)
			{
				//The tag is single, there shouldn't be any (tag, x) location, but the old way
				//was (tag,0) for left of the tag and (tag,1) for the right of the tag)

				if(offset == 1)
				{
					_currentNode->tag->endPos(curLine, curCol);
					curCol++;
				}
				else
					_currentNode->tag->beginPos(curLine, curCol);
			}
			else
			{
				//The new way to define cursor position.
				node = _currentNode->SFirstChild();
				i = 1;
				while(i < offset && node && node->SNext())
				{
					node = node->SNext();
					++i;
				}
				if(!node)
					node = _currentNode;
				if(offset == 0)
					node->tag->beginPos(curLine, curCol);
				else
				{
					closingNode = node->getClosingNode();
					if(closingNode)
						closingNode->tag->endPos(curLine, curCol);
					else
						node->tag->endPos(curLine, curCol);
					++curCol;
				}
			}
		}
	}

	line = curLine;
	col = curCol;
#ifdef LIGHT_DEBUG
	kdDebug(25001)<<"KafkaDocument::getQuantaCursorPosition() - " << line << ":" << col << endl;
#endif
	return;
}

void KafkaDocument::translateNodeIntoKafkaCursorPosition(Node *node, int offset, DOM::Node &domNode,
	long &domNodeOffset)
{
	QString textNode, curChar;
	int col;
	bool lookForEntity, lookForSpaces, found;

	if(node && node->rootNode() && node->rootNode()->nodeType() == DOM::Node::TEXT_NODE)
	{
		domNodeOffset = 0;
		domNode = *node->rootNode();
		textNode = node->tag->tagStr();
		col = 0;
		while(col < offset)
		{
			lookForEntity = false;
			lookForSpaces = false;
			curChar = textNode.mid(col, 1);
			if(curChar == "&")
				lookForEntity = true;
			else if(curChar[0].isSpace())
				lookForSpaces = true;
			found = false;
			while(!found && col < offset)
			{
				if((lookForEntity && curChar == ";") ||
					!(lookForSpaces || lookForEntity))
					found = true;
				else if(lookForSpaces && !curChar[0].isSpace())//curChar != " ")
					break;
				col++;
				curChar = textNode.mid(col, 1);
			}
#ifdef HEAVY_DEBUG
			//kdDebug(25001)<< "endpos at " << line << ":" << col << endl;
#endif
			domNodeOffset++;
		}
	}
	else if(node && node->rootNode())
	{
		domNode = *node->rootNode();
		domNodeOffset = 0;//shoud we select?
	}
	else
	{
		domNode = DOM::Node();
		domNodeOffset = 0;
	}
}

void KafkaDocument::translateNodeIntoQuantaCursorPosition(Node *node, int offset, uint &line, uint &col)
{
  int curCol, curLine, curOffset;

  node->tag->beginPos(curLine, curCol);
  line = curLine;
  col = curCol;
  curOffset = offset;
  while(curOffset > 0)
  {
    if(node->tag->tagStr()[offset - curOffset] == '\n')
      line++;
    else
      col++;
    curOffset--;
  }
}

bool KafkaDocument::insertDomNode(DOM::Node node, DOM::Node parent,
	DOM::Node nextSibling, DOM::Node rootNode)
{
	DOM::Node siblingNSpecial;

	//First insert the node
	if(!kafkaCommon::insertDomNode(node, parent, nextSibling, rootNode))
		return false;

	//Then unEnhance and reEnhance the nearest non special nodes so that everything's fine.
	siblingNSpecial = getPrevSiblingNSpecial(node);
	if(!siblingNSpecial.isNull())
	{
		mainEnhancer->postUnenhanceNode(siblingNSpecial);
		mainEnhancer->postEnhanceNode(siblingNSpecial);
	}
	siblingNSpecial = getNextSiblingNSpecial(node);
	if(!siblingNSpecial.isNull())
	{
		mainEnhancer->postUnenhanceNode(siblingNSpecial);
		mainEnhancer->postEnhanceNode(siblingNSpecial);
	}
	mainEnhancer->postUnenhanceNode(node.parentNode());
	mainEnhancer->postEnhanceNode(node.parentNode());

	mainEnhancer->postEnhanceNode(node);
	return true;
}

bool KafkaDocument::removeDomNode(DOM::Node node)
{
	DOM::Node nextSiblingNSpecial, prevSiblingNSpecial, parent;

	//First remove the node
	prevSiblingNSpecial = getPrevSiblingNSpecial(node);
	nextSiblingNSpecial = getNextSiblingNSpecial(node);
	parent = node.parentNode();
	if(!kafkaCommon::removeDomNode(node))
		return false;

	//Then unEnhance and reEnhance the nearest non special nodes so that everything's fine.
	if(!prevSiblingNSpecial.isNull())
	{
		mainEnhancer->postUnenhanceNode(prevSiblingNSpecial);
		mainEnhancer->postEnhanceNode(prevSiblingNSpecial);
	}
	if(!nextSiblingNSpecial.isNull())
	{
		mainEnhancer->postUnenhanceNode(nextSiblingNSpecial);
		mainEnhancer->postEnhanceNode(nextSiblingNSpecial);
	}
	mainEnhancer->postUnenhanceNode(parent);
	mainEnhancer->postEnhanceNode(parent);

	return true;
}

DOM::Node KafkaDocument::getPrevSiblingNSpecial(DOM::Node domNode)
{
	kNodeAttrs *attrs;

	if(domNode.isNull())
		return DOM::Node();

	domNode = domNode.previousSibling();
	while(!domNode.isNull())
	{
		attrs = getAttrs(domNode);
		if(!attrs)
			return DOM::Node();
		if(attrs->specialBehavior() == kNodeAttrs::none)
			return domNode;
		domNode = domNode.previousSibling();
	}

	return DOM::Node();
}

DOM::Node KafkaDocument::getNextSiblingNSpecial(DOM::Node domNode)
{
	kNodeAttrs *attrs;

	if(domNode.isNull())
		return DOM::Node();

	domNode = domNode.nextSibling();
	while(!domNode.isNull())
	{
		attrs = getAttrs(domNode);
		if(!attrs)
			return DOM::Node();
		if(attrs->specialBehavior() == kNodeAttrs::none)
			return domNode;
		domNode = domNode.nextSibling();
	}

	return DOM::Node();
}

void KafkaDocument::readConfig(KConfig *m_config)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "KafkaDocument::readConfig()" << endl;
#endif
	//for each nodeEnhancer
	if(mainEnhancer)
		mainEnhancer->readConfig(m_config);
	//reload the document
	reloadDocument();
}

const DTDStruct* KafkaDocument::defaultDTD()
{
	return m_currentDoc->defaultDTD();
}

#ifdef HEAVY_DEBUG
void KafkaDocument::coutLinkTree(Node *node, int indent)
#else
void KafkaDocument::coutLinkTree(Node *, int)
#endif
{
#ifdef HEAVY_DEBUG
	QString output, dots;
	DOM::Node domNode;
	Node *n = 0L;
	if(!node)
		kdDebug(25001)<< "kafkaDocument::coutTree() - bad node!" << endl;

	while (node)
	{
		dots = "";
		dots.fill('*', indent);
		output = dots;
		if (node->tag->type != Tag::Text)
			output += node->tag->name.replace('\n'," ");
		else
			output+= node->tag->tagStr().replace('\n'," ");
		output += " (";
		output += node->tag->type;
		output += ") ";
		n = 0L;
		if(node->rootNode())
		{
			domNode = *node->rootNode();
			n = getNode(domNode);
		}

		kdDebug(25001) << output <<" (" << node << ") " << domNode.handle() << " - " << n <<endl;

		if (node->child)
			coutLinkTree(node->child, indent + 4);

		node = node->next;
	}
#endif
}

void KafkaDocument::slotDomNodeInserted(DOM::Node domNode, bool insertChilds, NodeModifsSet* modifs)
{
#ifdef LIGHT_DEBUG
	if(!domNode.isNull())
		kdDebug(25001)<< "KafkaDocument::slotDomNodeInserted() - DOM::Node: " <<
			domNode.nodeName().string() << endl;
	else
		kdDebug(25001)<< "KafkaDocument::slotDomNodeInserted() - DOM::Node: NULL" << endl;
#endif
	Node *_nodeParent = 0L, *nodeNext = 0L, *_node = 0L;
	DOM::Node tmpDomNode, nextDomNode;
	bool b = false;

#ifdef LIGHT_DEBUG
	QTime t;
	t.start();
#endif

	_nodeParent = getNode(domNode.parentNode());

	if(!_nodeParent)
	{//DOM::Node not found, strange...
		kdDebug(25001)<< "KafkaDocument::slotDomNodeInserted() - *ERROR* the" <<
			" corresponding DOM::Node is not found!" << endl;
		return;
	}

	nextDomNode = getNextSiblingNSpecial(domNode);
	if(!nextDomNode.isNull())
	{
		nodeNext = getNode(nextDomNode);
		if(!nodeNext)
		{
			kdDebug(25001)<< "KafkaDocument::slotDomNodeInserted() - *ERROR2*" <<
			" the corresponding DOM::Node is not found!" << endl;
			return;
		}
	}

	_node = buildNodeFromKafkaNode(domNode, _nodeParent, nodeNext, 0, 0L, 0, modifs);

	if(insertChilds && domNode.hasChildNodes())
	{
		//TODO: check if it is working
		tmpDomNode = domNode.firstChild();
		while(!tmpDomNode.isNull())
		{
			buildNodeFromKafkaNode(tmpDomNode,
				getNode(tmpDomNode.parentNode()), 0L, 0, 0L, 0, modifs);
			tmpDomNode = kafkaCommon::getNextDomNode(tmpDomNode, b, false, domNode);
		}
	}

#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "KafkaDocument::slotDomNodeInserted() in " << t.elapsed() <<
		" ms only!" << endl;
#endif
#ifdef HEAVY_DEBUG
	kafkaCommon::coutTree(baseNode, 2);
#endif

}

void KafkaDocument::slotDomNodeModified(DOM::Node domNode, NodeModifsSet* modifs)
{
#ifdef LIGHT_DEBUG
	if(!domNode.isNull())
		kdDebug(25001)<< "KafkaDocument::slotDomNodeModified() - DOM::Node: " <<
			domNode.nodeName().string() << endl;
	else
		kdDebug(25001)<< "KafkaDocument::slotDomNodeModfied() - DOM::Node: NULL" << endl;
#endif
	Node *node = 0L;
	NodeModif *modif;
	kNodeAttrs *props, *newProps;
	DOM::Node newDomNode, parentDomNode, nextSiblingDomNode;
	DOM::Node *ptDomNode;
	QTag *qTag;

#ifdef LIGHT_DEBUG
	QTime t;
	t.start();
#endif

	//gets the DOM::Node's kNodeAttrs
	props = domNodeProps[domNode.handle()];
	if(!props)
	{
		kdDebug(25001)<< "KafkaDocument::slotDomNodeModified - *ERROR " <<
			" missing kNodeAttrs for a DOM::Node!!!" << endl;
		return;
	}

	//First look if domNode has a corresponding Node.
	if(props->isLinkedToNode())
	{

		//Look which Node correspond to this DOM::Node
		node = props->getNode();

		if(!node)
		{//DOM::Node not found, weird...
			kdDebug(25001)<< "KafkaDocument::slotDomNodeModified() - *ERROR* the" <<
				" corresponding DOM::Node is not found!" << endl;
			return;
		}

		modif = new NodeModif();
		modif->setType(NodeModif::NodeModified);
		modif->setTag(new Tag(*(node->tag)));
		modif->setLocation(kafkaCommon::getLocation(node));

		buildNodeFromKafkaNode(node, domNode);
    if (!modifs)
      modifs = new NodeModifsSet();
		modifs->addNodeModif(modif);
	}
	else
	{
		//no corresponding Node, we are in a special case with a special behavior.
		qTag = QuantaCommon::tagFromDTD(getNode(domNode.parentNode()));
		if(((!domNode.parentNode().isNull() && domNode.parentNode().nodeName() == "#document") ||
			qTag) && (
			props->specialBehavior() == kNodeAttrs::emptyTextSurroundingBlockElementAtTheLeft ||
			props->specialBehavior() == kNodeAttrs::emptyTextSurroundingBlockElementAtTheRight ||
			props->specialBehavior() == kNodeAttrs::emptyTextAsChildOfAChildlessElement))
		{
			//let's create the corresponding Text Node and the P tag only if necessary
			modifs = new NodeModifsSet();
			modif = new NodeModif();
			parentDomNode = domNode.parentNode();
			nextSiblingDomNode = domNode.nextSibling();

			if(!qTag->isChild("#text", false))
			{
				newDomNode = kafkaCommon::createDomNode("p",
					getNode(domNode.parentNode())->tag->dtd(), m_kafkaPart->document());
				kafkaCommon::removeDomNode(domNode);
				kafkaCommon::insertDomNode(newDomNode, parentDomNode, nextSiblingDomNode);
				kafkaCommon::insertDomNode(domNode, newDomNode);

				node = kafkaCommon::createNode("p", "", Tag::XmlTag, m_currentDoc);
				newProps = connectDomNodeToQuantaNode(newDomNode, node);
				ptDomNode = new DOM::Node(newDomNode);
				node->setRootNode(ptDomNode);
				ptDomNode = new DOM::Node(newDomNode);
				node->setLeafNode(ptDomNode);
				node = kafkaCommon::insertNode(node, getNode(parentDomNode),
					getNode(nextSiblingDomNode), getNode(nextSiblingDomNode), modifs);
				newProps->setNode(node);
				modifs->addNodeModif(modif);

				parentDomNode = newDomNode;
				nextSiblingDomNode = DOM::Node();
				modif = new NodeModif();
			}

			node = kafkaCommon::createNode("", domNode.nodeValue().string(), Tag::Text,
				m_currentDoc);
			ptDomNode = new DOM::Node(domNode);
			node->setRootNode(ptDomNode);
			ptDomNode = new DOM::Node(domNode);
			node->setLeafNode(ptDomNode);
			//avoid the merging of Text Nodes
			node = kafkaCommon::insertNode(node, getNode(parentDomNode),
				getNode(nextSiblingDomNode), modifs, false);
			props->setNode(node);
			modifs->addNodeModif(modif);
                        //If there is a empty Node after of before the text, remove them
                        if(node->prev && node->prev->tag->type == Tag::Empty)
                          kafkaCommon::extractNode(node->prev, modifs);
                        if(node->next && node->next->tag->type == Tag::Empty)
                          kafkaCommon::extractNode(node->next, modifs);

                        //Log the changes
			m_currentDoc->docUndoRedo->addNewModifsSet(modifs, undoRedo::KafkaModif);
		}

		props->setIsLinkedToNode(true);
		props->setSpecialBehavior(kNodeAttrs::none);
	}


#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "KafkaDocument::slotDomNodeModified() in " << t.elapsed() <<
		" ms only!" << endl;
#endif

#ifdef HEAVY_DEBUG
	kafkaCommon::coutTree(baseNode, 2);
#endif
}

void KafkaDocument::slotDomNodeAboutToBeRemoved(DOM::Node _domNode, bool deleteChilds, NodeModifsSet* modifs)
{
#ifdef LIGHT_DEBUG
	if(!_domNode.isNull())
		kdDebug(25001)<< "KafkaDocument::slotDomNodeAboutToBeRemoved() - DOM::Node: " <<
			_domNode.nodeName().string() << "(" << _domNode.handle() << ")"<< " bool :" <<
			deleteChilds << endl;
	else
		kdDebug(25001)<< "KafkaDocument::slotDomNodeAboutToBeRemoved() - DOM::Node: NULL bool :" <<
			deleteChilds << endl;
#endif
	Node *_node = 0L, *_nodeNext = 0L, *_tmpNode = 0L, *n = 0L;
	int i, bLine, bCol, eLine, eCol, bLine2, bCol2;
	bool hasClosingNode = false, b;
	NodeModif *modif;

#ifdef LIGHT_DEBUG
	QTime t;
	t.start();
#endif

	_node = getNode(_domNode);
	if(!_node)
	{
		kdDebug(25001)<<"KafkaDocument::slotDomNodeAboutToBeRemoved() - *ERROR* the" <<
			" corresponding DOM::Node is not found!" << endl;
		return;
	}


	//If we are deleting a PHP Node which is embedded into a tag e.g. <a <? echo boo; ?> >
	//We must regenerate the <a> tag string.
	if(_node->tag->type == Tag::ScriptTag && _node->parent)
	{
		_node->parent->tag->beginPos(bLine, bCol);
		_node->parent->tag->endPos(eLine, eCol);
		_node->tag->beginPos(bLine2, bCol2);
		if(QuantaCommon::isBetween(bLine2, bCol2, bLine, bCol, eLine,eCol) == 0)
			_node->parent->tag->setCleanStrBuilt(false);
	}

	if(_node->prev)
	{
		//delete the previous empty tag if present
		_nodeNext = _node;
		_node = _node->prev;
		if(_node && _node->tag->type == Tag::Empty)
		{
			modif = new NodeModif();
			modif->setType(NodeModif::NodeRemoved);
			modif->setLocation(kafkaCommon::getLocation(_node));
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
			modif->setNode(_node);
			modifs->addNodeModif(modif);
		}
		_node = _nodeNext;
	}

	//delete the Node
	modif = new NodeModif();
	if(deleteChilds)
		modif->setType(NodeModif::NodeAndChildsRemoved);
	else
		modif->setType(NodeModif::NodeRemoved);
	modif->setLocation(kafkaCommon::getLocation(_node));

	if(_node->getClosingNode())
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
			if(_tmpNode->rootNode())
				disconnectDomNodeFromQuantaNode(*_tmpNode->rootNode());
			if(_tmpNode->leafNode())
				disconnectDomNodeFromQuantaNode(*_tmpNode->leafNode());
			_tmpNode = kafkaCommon::getNextNode(_tmpNode, b, _node);
		}
		//delete _node->child;
	}
	else if(_node->child)// && !deleteChilds
	{
    if(_node->parent && !_node->parent->child)
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

	if(_node->rootNode())
		disconnectDomNodeFromQuantaNode(*_node->rootNode());
	if(_node->leafNode())
		disconnectDomNodeFromQuantaNode(*_node->leafNode());
	_node->parent = 0L;
	_nodeNext = _node->next;
	_node->next = 0L;
	_node->prev = 0L;
	_node->child = 0L;
	modif->setNode(_node);
	//delete _node;
	modif->setChildrenMovedUp(i);
	modifs->addNodeModif(modif);
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
		modif = new NodeModif();
		modif->setType(NodeModif::NodeRemoved);
		modif->setLocation(kafkaCommon::getLocation(_node));
		modif->setNode(_node);
		modifs->addNodeModif(modif);
		//delete _node;
		_node = _nodeNext;
	}

	if(_node && _node->tag->type == Tag::Empty)
	{
		//delete the next empty tag if present
		modif = new NodeModif();
		modif->setType(NodeModif::NodeRemoved);
		modif->setLocation(kafkaCommon::getLocation(_node));
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
		modif->setNode(_node);
		modifs->addNodeModif(modif);
		_node = _nodeNext;
	}

	//NO NORMALIZATION!! It is KafkaWidget::normalize()'s job!

#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "KafkaDocument::slotDomNodeDeleted() in " << t.elapsed() <<
		" ms only!" << endl;
#endif
#ifdef HEAVY_DEBUG
	kafkaCommon::coutTree(baseNode, 2);
#endif

}

void KafkaDocument::slotDomNodeIsAboutToBeMoved(DOM::Node domNode, DOM::Node newParent, DOM::Node before, NodeModifsSet* modifs)
{
	Node *node, *parent, *nextSibling, *closingNode;

	if(domNode.isNull())
		return;

	node = getNode(domNode);
	parent = getNode(newParent);
	nextSibling = getNode(before);

	if(!node)
		return;

	closingNode = node->getClosingNode();

	kafkaCommon::moveNode(node, parent, nextSibling, modifs, false);

	if(closingNode)
		kafkaCommon::moveNode(closingNode, parent, nextSibling, modifs, false);

}

void KafkaDocument::slotdomNodeNewCursorPos(DOM::Node, int)
{
#ifdef LIGHT_DEBUG
	kdDebug(25001)<< "KafkaDocument::slotdomNodeNewCursorPos()" << endl;
#endif
	//int line, col;
	//dont calculate cursor pos until the next view update
	//getQuantaCursorPosition(line, col);
	//emit newCursorPosition(line, col);
}

void KafkaDocument::slotCut()
{
    QString text = m_kafkaPart->selectedText();

    NodeSelectionInd selection_ind;
    selection_ind.fillWithVPLCursorSelection();

    int startOffset = selection_ind.cursorOffset();
    int endOffset = selection_ind.cursorOffsetEndSel();
    Node* startNode = kafkaCommon::getNodeFromLocation(selection_ind.cursorNode());
    Node* endNode = kafkaCommon::getNodeFromLocation(selection_ind.cursorNodeEndSel());

    DOM::Node cursorDomNode;
    long cursorOffset;

    m_kafkaPart->getCurrentNode(cursorDomNode, cursorOffset);
    Node* cursorNode = getNode(cursorDomNode);

    slotCut(startNode, startOffset, endNode, endOffset, &cursorNode, cursorOffset, text);
}

void KafkaDocument::slotCut(Node* startNode, int startOffset, Node* endNode, int endOffset,
                            Node** cursorNode, long cursorOffset, QString const& text)
{
    if(!startNode || !endNode)
        return;

    NodeModifsSet *modifs = new NodeModifsSet();

    Node* subtree_root = kafkaCommon::DTDExtractNodeSubtree(startNode, startOffset, endNode, endOffset,
            cursorNode, cursorOffset, modifs);

    m_currentDoc->docUndoRedo->addNewModifsSet(modifs, undoRedo::NodeTreeModif);

    //Now update the VPL cursor position
    kafkaWidget->setCurrentNode(startNode, startOffset);

    if(subtree_root)
    {
        KafkaDragObject* node_drag = new KafkaDragObject(subtree_root);
        QTextDrag* text_drag = new QTextDrag(text);
        KMultipleDrag* drag_object = new KMultipleDrag();
        drag_object->addDragObject(node_drag);
        drag_object->addDragObject(text_drag);

        QApplication::clipboard()->setData(drag_object);
#ifdef LIGHT_DEBUG
        kafkaCommon::coutTree(subtree_root, 3);
#endif
    }
}

void KafkaDocument::slotCopy()
{
    QString text = m_kafkaPart->selectedText();

    NodeSelectionInd selection_ind;
    selection_ind.fillWithVPLCursorSelection();

    int startOffset = selection_ind.cursorOffset();
    int endOffset = selection_ind.cursorOffsetEndSel();
    Node* startNode = kafkaCommon::getNodeFromLocation(selection_ind.cursorNode());
    Node* endNode = kafkaCommon::getNodeFromLocation(selection_ind.cursorNodeEndSel());

    slotCopy(startNode, startOffset, endNode, endOffset, text);
}

void KafkaDocument::slotCopy(Node* startNode, int startOffset, Node* endNode, int endOffset, QString const& text)
{
    if(!startNode || !endNode)
        return;

    Node* subtree_root = kafkaCommon::getNodeSubtree(startNode, startOffset, endNode, endOffset);

    if(subtree_root)
    {
        KafkaDragObject* node_drag = new KafkaDragObject(subtree_root);
        QTextDrag* text_drag = new QTextDrag(text);
        KMultipleDrag* drag_object = new KMultipleDrag();
        drag_object->addDragObject(node_drag);
        drag_object->addDragObject(text_drag);

        QApplication::clipboard()->setData(drag_object);
        // FIXME delete the subtree
#ifdef LIGHT_DEBUG
        kafkaCommon::coutTree(subtree_root, 3);
#endif
    }
}

void KafkaDocument::slotPaste()
{
    QClipboard *cb = QApplication::clipboard();
    QMimeSource* e = cb->data();
    Node* node = new Node(0);

    if(KafkaDragObject::decode(e, node))
    {
        bool go_up = false;
        for(Node* aux = node; aux; aux = kafkaCommon::getNextNode(aux, go_up))
            kafkaCommon::restorePastedNode(aux, getCurrentDoc());

        NodeSelectionInd selection_ind;
        selection_ind.fillWithVPLCursorSelection();

        Node* cursorNode = kafkaCommon::getNodeFromLocation(selection_ind.cursorNode());
        long cursorOffset = selection_ind.cursorOffset();

        NodeModifsSet *modifs = new NodeModifsSet();

        if(selection_ind.hasSelection())
            kafkaCommon::DTDRemoveSelection(selection_ind, &cursorNode, cursorOffset, modifs);
        else
            cursorNode = 0; // use selection_ind
        kafkaCommon::DTDInsertNodeSubtree(node, selection_ind, &cursorNode, cursorOffset, modifs);

        m_currentDoc->docUndoRedo->addNewModifsSet(modifs, undoRedo::NodeTreeModif, 0, false);

        //Now update the VPL cursor position
        kafkaWidget->setCurrentNode(cursorNode, cursorOffset);
    }
}
