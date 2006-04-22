/***************************************************************************
                     htmldocumentproperties.cpp
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


#include <kiconloader.h>
#include <klineedit.h>
#include <klistview.h>
#include <kpushbutton.h>
#include <kurlrequester.h>
#include <kurl.h>
#include <kmessagebox.h>
#include <klocale.h>
#include <kdebug.h>

#include "node.h"
#include "tag.h"
#include "qtag.h"
#include "resource.h"
#include "quanta.h"
#include "quantacommon.h"
#include "quantaview.h"
#include "document.h"
#include "tagattributetree.h"
#include "qextfileinfo.h"
#include "kafkacommon.h"
#include "wkafkapart.h"
#include "undoredo.h"
#include "cursors.h"
#include "htmldocumentproperties.h"

#include "viewmanager.h"

htmlDocumentProperties::htmlDocumentProperties( QWidget* parent, bool forceInsertionOfBasicNodes, const char* name,
 	bool modal, WFlags fl) :
	htmlDocumentPropertiesui(parent, name, modal, fl), titleNode( 0L ), htmlNode( 0L ),
	headNode( 0L ), linkNode( 0L ), bodyNode( 0L), doctypeNode( 0L ), CSSNode ( 0L ),
	xmlNode( 0L ), titleDirty(false), linkDirty(false)
{
	Node *node;
	QString text, nodeName;
	bool b;
	int index;
	KURL url, baseURL;

        m_forceInsertionOfBasicNodes = forceInsertionOfBasicNodes;

	//set the "autodefault" property
	metaItemsAdd->setAutoDefault(false);
	metaItemsDelete->setAutoDefault(false);
	cssRulesAdd->setAutoDefault(false);
	cssRulesEdit->setAutoDefault(false);
	cssRulesDelete->setAutoDefault(false);
	cssStylesheet->button()->setAutoDefault(false);
	cancel->setAutoDefault(false);
	ok->setAutoDefault(true);

	//set the taborder and disable focus for some widgets.
	currentDTD->setFocusPolicy(QWidget::NoFocus);
	QWidget::setTabOrder(title, metaItems);
	QWidget::setTabOrder(metaItems, metaItemsAdd);
	QWidget::setTabOrder(metaItemsAdd, metaItemsDelete);
	QWidget::setTabOrder(metaItemsDelete, cssRules);
	QWidget::setTabOrder(cssRules, cssRulesAdd);
	QWidget::setTabOrder(cssRulesAdd, cssRulesEdit);
	QWidget::setTabOrder(cssRulesEdit, cssRulesDelete);
	QWidget::setTabOrder(cssRulesDelete, cssStylesheet);
	QWidget::setTabOrder(cssStylesheet, ok);
	QWidget::setTabOrder(ok, cancel);

	//set the current DTD name
	currentDTD->setText(ViewManager::ref()->activeDocument()->defaultDTD()->nickName);

	//set the metaItems DualEditableTree
	metaItems->addColumn(i18n("Name"));
	metaItems->addColumn(i18n("Content"));
	metaItems->setFrameStyle( QFrame::Panel | QFrame::Sunken );
	metaItems->setLineWidth( 2 );
	metaItems->setSorting(0, true);


	//set the cssRules KListView
	//cssRules->setEditable(false);
	cssRules->addColumn(i18n("Selector"));
	cssRules->addColumn(i18n("Rule"));
	cssRules->setFrameStyle( QFrame::Panel | QFrame::Sunken );
	cssRules->setLineWidth( 2 );
	//cssRules->setFocusPolicy(QWidget::ClickFocus);
	cssRules->setSorting(-1);

	//search for the head, html, title Node
	if(baseNode)
	{
		node = baseNode;
		while(node)
		{
			nodeName = node->tag->name.lower();
			if(nodeName == "?xml" || nodeName.contains("xml pi block"))
				xmlNode = node;
			if(nodeName == "html")
				htmlNode = node;
			if(nodeName == "head")
				headNode = node;
			if(nodeName == "body")
				bodyNode = node;
			if(nodeName == "!doctype" || nodeName.contains("dtd block"))
				doctypeNode = node;
			if(nodeName == "title")
				titleNode = node;
			if(nodeName == "link")
				linkNode = node;
			if(nodeName == "meta")
				loadMetaNode(node);
			if(nodeName == "style")
				loadCSS(node);
			node = node->next;
		}
		//support for old Node organization
		if(doctypeNode)
		{
			node = doctypeNode->child;
			while(node)
			{
				nodeName = node->tag->name.lower();
				if(nodeName == "html")
					htmlNode = node;
				if(nodeName == "head")
					headNode = node;
				if(nodeName == "body")
					bodyNode = node;
				if(nodeName == "title")
					titleNode = node;
				if(nodeName == "link")
					linkNode = node;
				if(nodeName == "meta")
					loadMetaNode(node);
				if(nodeName == "style")
					loadCSS(node);
				node = node->next;
			}
		}
		if(htmlNode)
		{
			node = htmlNode->child;
			while(node)
			{
				nodeName = node->tag->name.lower();
				if(nodeName == "head")
					headNode = node;
				if(nodeName == "body")
					bodyNode = node;
				if(nodeName == "title")
					titleNode = node;
				if(nodeName == "link")
					linkNode = node;
				if(nodeName == "meta")
					loadMetaNode(node);
				if(nodeName == "style")
					loadCSS(node);
				node = node->next;
			}
		}
		if(headNode)
		{
			node = headNode->child;
			while(node)
			{
				nodeName = node->tag->name.lower();
				if(nodeName == "title")
					titleNode = node;
				if(nodeName == "link")
					linkNode = node;
				if(nodeName == "meta")
					loadMetaNode(node);
				if(nodeName == "style")
					loadCSS(node);
				node = node->next;
			}
		}
	}

	//set the current title
	if(titleNode)
	{
		node = titleNode->child;
		b = false;
		while(node)
		{
			text += node->tag->tagStr();
			node = kafkaCommon::getNextNode(node, b, titleNode);
		}
		title->setText(KafkaDocument::ref()->getDecodedText(text));
	}

	//set the link
	if(linkNode)
	{
		index = linkNode->tag->attributeIndex("rel");
		if(index != -1)
		{
			if(linkNode->tag->attributeValue(index).lower() == "stylesheet")
			{
				index = linkNode->tag->attributeIndex("href");
				if(index != -1)
				{
					cssStylesheet->setMode(KFile::File | KFile::ExistingOnly );
					baseURL.setPath(ViewManager::ref()->activeDocument()->url().directory());
					QuantaCommon::setUrl(url, linkNode->tag->attributeValue(index));
					url = QExtFileInfo::toAbsolute(url, baseURL);
					cssStylesheet->setURL(url.url());
				}
			}
		}
	}

        ok->setIconSet(SmallIconSet("button_ok"));
        cancel->setIconSet(SmallIconSet("button_cancel"));

	//connect buttons
	connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
	connect(cssRulesAdd, SIGNAL(clicked()), this, SLOT(newCSSRule()));
	connect(cssRulesEdit, SIGNAL(clicked()), this, SLOT(editCSSRule()));
	connect (cssRulesDelete, SIGNAL(clicked()), this, SLOT(deleteCurrentCSSRule()));
	connect(metaItemsAdd, SIGNAL(clicked()), this, SLOT(newMetaItem()));
	connect(metaItemsDelete, SIGNAL(clicked()), this, SLOT(deleteCurrentMetaItem()));
	connect(title, SIGNAL(textChanged(const QString &)), this, SLOT(titleChanged(const QString &)));
	connect(metaItems, SIGNAL(itemModified( QListViewItem * )),
		this, SLOT(metaChanged(QListViewItem * )));
	connect(cssRules, SIGNAL(itemModified( QListViewItem * )),
		this, SLOT(CSSChanged(QListViewItem * )));
	connect(cssStylesheet, SIGNAL(textChanged(const QString &)),
		this, SLOT(linkChanged( const QString& )));
}

htmlDocumentProperties::~htmlDocumentProperties()
{

}

void htmlDocumentProperties::loadMetaNode(Node *node)
{
	int index;
	QString name, content;
	NodeLinkedViewItem *item;

	index = node->tag->attributeIndex("name");
	if(index != -1)
		name = node->tag->attributeValue(index);
	index = node->tag->attributeIndex("http-equiv");
	if(index != -1)
		name = node->tag->attributeValue(index);
	index = node->tag->attributeIndex("content");
	if(index != -1)
		content = node->tag->attributeValue(index);
	item = new NodeLinkedViewItem(metaItems, name, content);
	item->node = node;
	metaList.append(item);

	metaItems->sort();
}

void htmlDocumentProperties::loadCSS(Node *node)
{
	NodeLinkedViewItem *item;
	QString selector;

	CSSNode = node;
	node = node->child;
	while(node)
	{
		if(node->tag->type == Tag::ScriptStructureBegin)
		{
			selector = node->tag->tagStr();
			selector = selector.left((uint)selector.find("{")).stripWhiteSpace();
			if(node->child)
				item = new NodeLinkedViewItem(cssRules, selector,
					node->child->tag->tagStr().replace('\n'," "));
			else
				item = new NodeLinkedViewItem(cssRules, selector, "");
			item->node = node;
			item->moveItem(cssRules->lastChild());
			CSSList.append(item);
		}
		node = node->next;
	}
}

void htmlDocumentProperties::newMetaItem()
{
	NodeLinkedViewItem *item;
	item = new NodeLinkedViewItem(metaItems, "", "");
	item->node = 0L;
	metaList.append(item);

	if(metaItems->lastItem())
		item->moveItem(metaItems->lastItem());
}

void htmlDocumentProperties::deleteCurrentMetaItem()
{
	if(metaItems->currentItem())
	{
		QListViewItem *item = metaItems->currentItem();
		(static_cast<NodeLinkedViewItem *>(item))->deleted = true;
		(static_cast<NodeLinkedViewItem *>(item))->dirty = true;
		(static_cast<AttributeItem *>(item))->hideEditor(0);
		(static_cast<AttributeItem *>(item))->hideEditor(1);
		metaItems->takeItem(metaItems->currentItem());
	}
}

void htmlDocumentProperties::newCSSRule()
{
	NodeLinkedViewItem *item;
	item = new NodeLinkedViewItem(cssRules, "", "");
	item->node = 0L;
	CSSList.append(item);

	if(cssRules->lastItem())
		item->moveItem(cssRules->lastItem());
}

void htmlDocumentProperties::editCSSRule()
{
//Make this using the big CSS dialog, need parsing!!
	KMessageBox::information(this, i18n("Sorry, VPL does not support this functionality yet."));
}

void htmlDocumentProperties::deleteCurrentCSSRule()
{
	if(cssRules->currentItem())
	{
		QListViewItem *item = cssRules->currentItem();
		(static_cast<NodeLinkedViewItem *>(item))->deleted = true;
		(static_cast<NodeLinkedViewItem *>(item))->dirty = true;
		(static_cast<AttributeItem *>(item))->hideEditor(0);
		(static_cast<AttributeItem *>(item))->hideEditor(1);
		cssRules->takeItem(cssRules->currentItem());
	}
}


void htmlDocumentProperties::titleChanged(const QString &)
{
	titleDirty = true;
}

void htmlDocumentProperties::metaChanged(QListViewItem * item)
{
	if(item)
		(static_cast<NodeLinkedViewItem *>(item))->dirty = true;
}

void htmlDocumentProperties::CSSChanged(QListViewItem * item)
{
	if(item)
		(static_cast<NodeLinkedViewItem *>(item))->dirty = true;
}

void htmlDocumentProperties::linkChanged( const QString& )
{
	linkDirty = true;
}


void htmlDocumentProperties::accept()
{
	Node *node, *nodeNext;
	NodeLinkedViewItem *item;
	TagAttr attr;
	NodeModifsSet *modifs = new NodeModifsSet();
	KURL url, baseURL;
	QString finalURL;
        NodeSelection *cursorPos;
        bool goUp;
	//TODO:see for !doctype

        QuantaView *view = ViewManager::ref()->activeView();
	//set the TITLE if necessary.
	if(titleDirty)
	{
		if(!titleNode)
		{
			addBasicNodes(modifs);
			//create title
			titleNode = kafkaCommon::createAndInsertNode("title", "", Tag::XmlTag,
				view->document(), headNode, 0L, 0L, modifs);
		}
		node = titleNode->child;
		if(node && (node->next || node->tag->type != Tag::Text))
		{
			while(node)
			{
				nodeNext = node->next;
				kafkaCommon::extractAndDeleteNode(node, modifs, true, false);
				node = nodeNext;
			}
		}
		if(!titleNode->child)
		{
			//create text!
			node = kafkaCommon::createAndInsertNode("", title->text(),Tag::Text,
				view->document(), titleNode, 0L, 0L, modifs);
		}
		else
      titleNode->child->tag->setStr(KafkaDocument::ref()->getEncodedText(title->text()));
	}

	//set the METAs if necessary
	if(metaItems->currentItem())
		(static_cast<NodeLinkedViewItem *>(metaItems->currentItem()))->dirty = true;
	item = metaList.first();
	while(item)
	{
		if((static_cast<NodeLinkedViewItem *>(item))->dirty)
		{
			if((static_cast<NodeLinkedViewItem *>(item))->deleted)
			{
				if((static_cast<NodeLinkedViewItem *>(item))->node)
				{
					//delete the meta
					kafkaCommon::extractAndDeleteNode(
						(static_cast<NodeLinkedViewItem *>(item))->node, modifs);
				}
			}
			else
			{
				if(!(static_cast<NodeLinkedViewItem *>(item))->node)
				{
					if(!(static_cast<AttributeItem *>(item))->editorText(0).isEmpty() ||
						!(static_cast<AttributeItem *>(item))->editorText(1).isEmpty())
					{
						addBasicNodes(modifs);
						//create the meta!!
						node = kafkaCommon::createAndInsertNode("meta", "",Tag::XmlTag,
							view->document(), headNode, 0L, 0L, modifs);
					}
					else
						node = 0L;
				}
				else
					node = (static_cast<NodeLinkedViewItem *>(item))->node;
				if(node)
				{
					if(node->tag->hasAttribute("name"))
						node->tag->editAttribute("name", (static_cast<AttributeItem *>(item))->editorText(0));
					else
						node->tag->editAttribute("http-equiv", (static_cast<AttributeItem *>(item))->editorText(0));
					node->tag->editAttribute("content", (static_cast<AttributeItem *>(item))->editorText(1));
					node->tag->setCleanStrBuilt(false);
				}
			}
		}
		item = metaList.next();
	}

	//set the CSSs rules if necessary
	if(cssRules->currentItem())
		(static_cast<NodeLinkedViewItem *>(cssRules->currentItem()))->dirty = true;
	item = CSSList.first();
	while(item)
	{
		if((static_cast<NodeLinkedViewItem *>(item))->dirty)
		{
			if((static_cast<NodeLinkedViewItem *>(item))->deleted)
			{
				if((static_cast<NodeLinkedViewItem *>(item))->node)
				{
					//Delete the CSS Node
					node = (static_cast<NodeLinkedViewItem *>(item))->node;
					if(node->next && node->next->tag->type == Tag::ScriptStructureEnd)
						kafkaCommon::extractAndDeleteNode(node->next, modifs);
					kafkaCommon::extractAndDeleteNode(node, modifs);
				}
			}
			else
			{
				if(!(static_cast<NodeLinkedViewItem *>(item))->node)
				{
					if(!(static_cast<AttributeItem *>(item))->editorText(0).isEmpty() ||
						!(static_cast<AttributeItem *>(item))->editorText(1).isEmpty())
					{
						addBasicNodes(modifs);
						addBasicCssNodes(modifs);
						//create the CSS Nodes!
						node = kafkaCommon::createAndInsertNode(
							(static_cast<AttributeItem *>(item))->editorText(0),
							(static_cast<AttributeItem *>(item))->editorText(0) + "{",
							Tag::ScriptStructureBegin, view->document(),
							CSSNode, 0L, 0L, modifs);
						(void)kafkaCommon::createAndInsertNode("#text", "",Tag::Text,
							view->document(), node, 0L, 0L, modifs);
						nodeNext = kafkaCommon::createAndInsertNode("", "}",Tag::ScriptStructureEnd,
							view->document(), CSSNode, 0L, 0L, modifs);
					}
					else
						node = 0L;
				}
				else
					node = (static_cast<NodeLinkedViewItem *>(item))->node;
				if(node && node->child)
				{
					node->tag->name = (static_cast<AttributeItem *>(item))->editorText(0);
					node->tag->setStr((static_cast<AttributeItem *>(item))->editorText(0) + "{");
					node->child->tag->setStr((static_cast<AttributeItem *>(item))->editorText(1));
				}
			}
		}
		item = CSSList.next();
	}

	//set the LINK, if necessary
	if(linkDirty)
	{
		if(!linkNode)
		{
			//create LINK
			addBasicNodes(modifs);
			linkNode = kafkaCommon::createAndInsertNode("link", "",Tag::XmlTag, view->document(),
				headNode, 0L, 0L, modifs);
		}
		//modify it!
		QuantaCommon::setUrl(url, cssStylesheet->url());
		baseURL = view->document()->url();
		baseURL.setPath(baseURL.directory());
		url = QExtFileInfo::toRelative(url, baseURL);
		finalURL = url.url();
		if (url.protocol() == view->document()->url().protocol())
			finalURL.remove(0, url.protocol().length() + 1);
		if(linkNode->tag->attributeValue("rel").lower() != "stylesheet")
			linkNode->tag->editAttribute("rel", "stylesheet");
		linkNode->tag->editAttribute("href", finalURL);
		linkNode->tag->setCleanStrBuilt(false);
	}

        if(m_forceInsertionOfBasicNodes)
          addBasicNodes(modifs);
          
        //Set the cursor at the beginning of the document.
        goUp = false;
        node = kafkaCommon::getNextNode(bodyNode, goUp, bodyNode);
        while(node && node->tag->type != Tag::Text)
          node = kafkaCommon::getNextNode(node, goUp, bodyNode);
        cursorPos = new NodeSelection();
        cursorPos->setCursorNode(node?node:bodyNode);
        cursorPos->setCursorOffset(0);

	view->document()->docUndoRedo->addNewModifsSet(modifs, undoRedo::NodeTreeModif, cursorPos);

        delete cursorPos;

	//view->reloadBothViews();

	done(0);
}

void htmlDocumentProperties::reject()
{
  NodeModifsSet *modifs = new NodeModifsSet();

  if(m_forceInsertionOfBasicNodes)
    addBasicNodes(modifs);

  ViewManager::ref()->activeDocument()->docUndoRedo->addNewModifsSet(modifs, undoRedo::NodeTreeModif);

  done(0);
}

void htmlDocumentProperties::addBasicCssNodes(NodeModifsSet *modifs)
{
	if(CSSNode || !htmlNode || !headNode)
		return;
	//TODO:quick hack, modify createAndInsertNode
	CSSNode = kafkaCommon::createAndInsertNode("style", "", Tag::XmlTag, ViewManager::ref()->activeDocument(),
		headNode, 0L, 0L, modifs);
}

void htmlDocumentProperties::addBasicNodes(NodeModifsSet *modifs)
{
	Node *allTheNodes, *lastHeadChild, *lastBodyChild, *lastHtmlChild;
	Node *n, *nextNode;
	bool htmlNodeCreated = false, bodyNodeCreated = false;
	QTag *qHead, *qBody;
	QString tagName;

	if(headNode)
		return;

        QuantaView *view = ViewManager::ref()->activeView();

	if(!xmlNode && view->document()->defaultDTD()->name.contains("XHTML", false))
	{
		//if the xml node is not present and the current DTD is a xhtml like, create it.
		allTheNodes = baseNode;
		baseNode = 0L;
		xmlNode = kafkaCommon::createXmlDeclarationNode(view->document(),
			quantaApp->defaultEncoding());
		nextNode = xmlNode->next;
		xmlNode = kafkaCommon::insertNode(xmlNode, 0L, 0L, modifs);
		kafkaCommon::insertNode(nextNode, 0L, 0L, modifs);
		xmlNode->next->next = allTheNodes;
	}

	if(!doctypeNode)
	{
		//if the !doctype node is not present, create it
		if(!view->document()->defaultDTD()->name.contains("XHTML", false))
		{
			allTheNodes = baseNode;
			baseNode = 0L;
		}
		else
		{
			allTheNodes = xmlNode->next->next;
			xmlNode->next->next = 0L;
		}
		doctypeNode = kafkaCommon::createDoctypeNode(view->document());
		nextNode = doctypeNode->next;
		doctypeNode = kafkaCommon::insertNode(doctypeNode, 0L, 0L, modifs);
		kafkaCommon::insertNode(nextNode, 0L, 0L, modifs);
		doctypeNode->next->next = allTheNodes;
	}

	if(!htmlNode && !headNode)
	{
		//if the HTML node is not present, create it
		allTheNodes = doctypeNode->next->next;
		doctypeNode->next->next = 0L;
		htmlNode = kafkaCommon::createAndInsertNode("html", "", Tag::XmlTag,
			view->document(), 0L, 0L, 0L, modifs);

		//TODO: hardcoded
		//If it is XML, it add the namespace.
		if(view->document()->defaultDTD()->name.contains("XHTML", false))
			htmlNode->tag->editAttribute("xmlns", "http://www.w3.org/1999/xhtml");

		htmlNode->child = allTheNodes;
		while(allTheNodes)
		{
			allTheNodes->parent = htmlNode;
			allTheNodes = allTheNodes->next;
		}
		htmlNodeCreated = true;
	}

	//Create the HEAD Node.
	allTheNodes = htmlNode->child;
	htmlNode->child = 0L;
	headNode = kafkaCommon::createAndInsertNode("head", "", Tag::XmlTag, view->document(),
		htmlNode, 0L, 0L, modifs);

	if(!bodyNode && htmlNodeCreated)
	{
		//let's create BODY to take all the Nodes which can't be in the newly created HTML
		bodyNode = kafkaCommon::createAndInsertNode("body", "", Tag::XmlTag,
			view->document(), htmlNode, 0L, 0L, modifs);
		bodyNodeCreated = true;
	}

	//we now move the necessary Nodes to HEAD (and to BODY if htmlNodeCreated)
	qHead = QuantaCommon::tagFromDTD(view->document()->defaultDTD(), "head");
	qBody = QuantaCommon::tagFromDTD(view->document()->defaultDTD(), "body");
	lastHeadChild = 0L;
	lastBodyChild = 0L;
	lastHtmlChild = htmlNode->child;
	while(lastHtmlChild && lastHtmlChild->next)
		lastHtmlChild = lastHtmlChild->next;
	while(allTheNodes)
	{
		n = allTheNodes->next;
		if(qHead->isChild(allTheNodes))
		{
			/*//TODO:LOG this into the modif!!
			allTheNodes->parent = headNode;
			if(lastHeadChild)
			{
				lastHeadChild->next = allTheNodes;
				allTheNodes->prev = lastHeadChild;
			}
			else
			{
				headNode->child = allTheNodes;
			}
			lastHeadChild = allTheNodes;*/
			kafkaCommon::moveNode(allTheNodes, headNode, 0L, modifs);
		}
		else if(bodyNodeCreated && htmlNodeCreated && qBody->isChild(allTheNodes))
		{
			//TODO:log!!
			/**allTheNodes->parent = bodyNode;
			if(lastBodyChild)
			{
				lastBodyChild->next = allTheNodes;
				allTheNodes->prev = lastBodyChild;
			}
			else
			{
				bodyNode->child = allTheNodes;
			}
			lastBodyChild = allTheNodes;*/
			kafkaCommon::moveNode(allTheNodes, bodyNode, 0L, modifs);
		}
		else
		{
			//TODO:log? no.
			/**allTheNodes->parent = htmlNode;
			lastHtmlChild->next = allTheNodes;
			allTheNodes->prev = lastHtmlChild;
			lastHtmlChild = allTheNodes;*/
			kafkaCommon::moveNode(allTheNodes, htmlNode, 0L, modifs);
		}
		/**n = allTheNodes->next;
		if(allTheNodes->next)
			allTheNodes->next->prev = 0L;
		allTheNodes->next = 0L;
		allTheNodes = n;*/
		allTheNodes = n;
	}
}

NodeLinkedViewItem::NodeLinkedViewItem(EditableTree *listView, const QString& title, const QString& title2)
: AttributeItem(listView, title, title2, 0L)
{
	node = 0L;
	dirty = false;
	deleted = false;
}

NodeLinkedViewItem::~NodeLinkedViewItem()
{

}

#include "htmldocumentproperties.moc"
