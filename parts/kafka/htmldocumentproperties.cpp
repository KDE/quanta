/***************************************************************************
                     htmldocumentproperties.cpp
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


#include <kiconloader.h>
#include <klineedit.h>
#include <klistview.h>
#include <kpushbutton.h>
#include <kurlrequester.h>
#include <kurl.h>
#include <kdebug.h>

#include "../../parser/node.h"
#include "../../parser/tag.h"
#include "../../parser/qtag.h"
#include "../../resource.h"
#include "../../quanta.h"
#include "../../quantaview.h"
#include "../../document.h"
#include "../../treeviews/tagattributetree.h"
#include "../../qextfileinfo.h"
#include "kafkacommon.h"
#include "wkafkapart.h"
#include "undoredo.h"
#include "htmldocumentproperties.h"

htmlDocumentProperties::htmlDocumentProperties( QWidget* parent, const char* name,
 	bool modal, WFlags fl) :
	htmlDocumentPropertiesui(parent, name, modal, fl), titleNode( 0L ), htmlNode( 0L ),
	headNode( 0L ), linkNode( 0L ), bodyNode( 0L), doctypeNode( 0L ), CSSNode ( 0L ),
	titleDirty(false), linkDirty(false)
{
	Node *node;
	QString text, nodeName;
	bool b;
	int index;
	KURL url, baseURL;

	//set the "autodefault" property
	metaItemsAdd->setAutoDefault(false);
	metaItemsDelete->setAutoDefault(false);
	cssRulesAdd->setAutoDefault(false);
	cssRulesEdit->setAutoDefault(false);
	cssRulesDelete->setAutoDefault(false);
	cssStylesheet->button()->setAutoDefault(false);
	cancel->setAutoDefault(false);
	ok->setAutoDefault(true);

	//set the current DTD name
	currentDTD->setText(quantaApp->view()->write()->defaultDTD()->nickName);

	//set the metaItems DualEditableTree
	metaItems->addColumn("name");
	metaItems->addColumn("content");
	metaItems->setFrameStyle( QFrame::Panel | QFrame::Sunken );
	metaItems->setLineWidth( 2 );
	metaItems->setSorting(-1);


	//set the cssRules KListView
	//cssRules->setEditable(false);
	cssRules->addColumn("selector");
	cssRules->addColumn("rule");
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
			if(nodeName == "html")
				htmlNode = node;
			if(nodeName == "head")
				headNode = node;
			if(nodeName == "body")
				bodyNode = node;
			if(nodeName == "!doctype")
				doctypeNode = node;
			if(nodeName == "title")
				titleNode = node;
			if(nodeName == "link")
				linkNode = node;
			if(nodeName == "meta")
				loadMetaNode(node);
			if(nodeName.contains("style"))
				loadCSS(node);
			node = node->next;
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
				if(nodeName.contains("style"))
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
				if(nodeName.contains("style"))
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
		title->setText(quantaApp->view()->getKafkaInterface()->getDecodedText(text));
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
					baseURL.setPath(quantaApp->view()->write()->url().directory());
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
	connect(ok, SIGNAL(clicked()), this, SLOT(aboutToClose()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(close()));
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

}

void htmlDocumentProperties::loadCSS(Node *node)
{
	NodeLinkedViewItem *item;

	CSSNode = node;
	node = node->child;
	while(node)
	{
		if(node->tag->type == Tag::ScriptStructureBegin)
		{
			if(node->child)
				item = new NodeLinkedViewItem(cssRules, node->tag->name,
					node->child->tag->tagStr());
			else
				item = new NodeLinkedViewItem(cssRules, node->tag->name, "");
			item->node = node;
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


void htmlDocumentProperties::aboutToClose()
{
	Node *node, *nodeNext;
	NodeLinkedViewItem *item;
	TagAttr attr;
	NodeModifsSet *modifs = new NodeModifsSet();
	KURL url, baseURL;
	QString finalURL;
	//TODO:see for !doctype

	//set the TITLE if necessary.
	if(titleDirty)
	{
		if(!titleNode)
		{
			addBasicNodes(modifs);
			//create title
			titleNode = kafkaCommon::createAndInsertNode("title", "", Tag::XmlTag,
				quantaApp->view()->write(), headNode, 0L, 0L, modifs);
		}
		node = titleNode->child;
		if(node && (node->next || node->tag->type != Tag::Text))
		{
			while(node)
			{
				nodeNext = node->next;
				kafkaCommon::extractAndDeleteNode(node, modifs);
				node = nodeNext;
			}
		}
		if(!titleNode->child)
		{
			//create text!
			node = kafkaCommon::createAndInsertNode("", title->text(),Tag::Text,
				quantaApp->view()->write(), titleNode, 0L, 0L, modifs);
		}
		else
			node->tag->setStr(quantaApp->view()->getKafkaInterface()->getEncodedText(title->text()));
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
					if((static_cast<AttributeItem *>(item))->editorText(0) != "" ||
						(static_cast<AttributeItem *>(item))->editorText(1) != "")
					{
						addBasicNodes(modifs);
						//create the meta!!
						node = kafkaCommon::createAndInsertNode("meta", "",Tag::XmlTag,
							quantaApp->view()->write(), headNode, 0L, 0L, modifs);
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
					node->tag->cleanStrBuilt = false;
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
					node = (static_cast<NodeLinkedViewItem *>(item))->node;
					if(node->prev && node->next->tag->type == Tag::ScriptStructureEnd)
						kafkaCommon::extractAndDeleteNode(node->next, modifs);
					kafkaCommon::extractAndDeleteNode(node, modifs);
				}
			}
			else
			{
				if(!(static_cast<NodeLinkedViewItem *>(item))->node)
				{
					if((static_cast<AttributeItem *>(item))->editorText(0) != "" ||
						(static_cast<AttributeItem *>(item))->editorText(1) != "")
					{
						addBasicNodes(modifs);
						addBasicCssNodes(modifs);
						//create the CSS Nodes!
						node = kafkaCommon::createAndInsertNode(
							(static_cast<AttributeItem *>(item))->editorText(0), "",Tag::ScriptStructureBegin,
							quantaApp->view()->write(), CSSNode, 0L, 0L, modifs);
						(void)kafkaCommon::createAndInsertNode("#text", "",Tag::Text,
							quantaApp->view()->write(), node, 0L, 0L, modifs);
						nodeNext = kafkaCommon::createAndInsertNode("", "}",Tag::ScriptStructureEnd,
							quantaApp->view()->write(), CSSNode, 0L, 0L, modifs);
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
			linkNode = kafkaCommon::createAndInsertNode("link", "",Tag::XmlTag, quantaApp->view()->write(),
				headNode, 0L, 0L, modifs);
		}
		//modify it!
		QuantaCommon::setUrl(url, cssStylesheet->url());
		baseURL = quantaApp->view()->write()->url();
		baseURL.setPath(baseURL.directory());
		url = QExtFileInfo::toRelative(url, baseURL);
		finalURL = url.url();
		if (url.protocol() == quantaApp->view()->write()->url().protocol())
			finalURL.remove(0, url.protocol().length() + 1);
		if(linkNode->tag->attributeValue("rel").lower() != "stylesheet");
			linkNode->tag->editAttribute("rel", "stylesheet");
		linkNode->tag->editAttribute("href", finalURL);
		linkNode->tag->cleanStrBuilt = false;
	}

	quantaApp->view()->write()->docUndoRedo->addNewModifsSet(modifs, undoRedo::NodeTreeModif);

	//quantaApp->view()->reloadBothViews();

	close();
}

void htmlDocumentProperties::addBasicCssNodes(NodeModifsSet *modifs)
{
	if(CSSNode || !htmlNode || !headNode)
		return;
	//TODO:quick hack, modify createAndInsertNode
	CSSNode = kafkaCommon::createAndInsertNode("style", "", Tag::XmlTag, quantaApp->view()->write(),
		headNode, 0L, 0L, modifs);
	CSSNode->tag->type = Tag::ScriptTag;
	CSSNode->tag->name = "style |  block";
}

void htmlDocumentProperties::addBasicNodes(NodeModifsSet *modifs)
{
	Node *allTheNodes, /***htmlEnd, *headEnd, *bodyEnd,*/ *lastHeadChild, *lastBodyChild, *lastHtmlChild;
	Node *n;
	bool htmlNodeCreated = false, bodyNodeCreated = false;
	QTag *qHead, *qBody;
	QString tagName;

	if(headNode)
		return;

	if(!htmlNode && !headNode)
	{
		//if the HTML node is not present, create it
		allTheNodes = baseNode;
		baseNode = 0L;
		htmlNode = kafkaCommon::createAndInsertNode("html", "", Tag::XmlTag, quantaApp->view()->write(),
			0L, 0L, 0L, modifs);
		htmlNode->child = allTheNodes;
		while(allTheNodes)
		{
			allTheNodes->parent = baseNode;
			allTheNodes = allTheNodes->next;
		}
		htmlNodeCreated = true;
	}

	//Create the HEAD Node.
	allTheNodes = htmlNode->child;
	htmlNode->child = 0L;
	headNode = kafkaCommon::createAndInsertNode("head", "", Tag::XmlTag, quantaApp->view()->write(),
		htmlNode, 0L, 0L, modifs);

	if(!bodyNode && htmlNodeCreated)
	{
		//let's create BODY to take all the Nodes which can't be in the newly created HTML
		bodyNode = kafkaCommon::createAndInsertNode("body", "", Tag::XmlTag, quantaApp->view()->write(),
			htmlNode, 0L, 0L, modifs);
		bodyNodeCreated = true;
	}

	//we now move the necessary Nodes to HEAD (and to BODY if htmlNodeCreated)
	qHead = QuantaCommon::tagFromDTD(quantaApp->view()->write()->defaultDTD(), "head");
	qBody = QuantaCommon::tagFromDTD(quantaApp->view()->write()->defaultDTD(), "body");
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
: AttributeItem(listView, title, title2)
{
	node = 0L;
	dirty = false;
	deleted = false;
}

NodeLinkedViewItem::~NodeLinkedViewItem()
{

}
