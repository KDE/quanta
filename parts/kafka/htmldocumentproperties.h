/***************************************************************************
                     htmldocumentproperties.h
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

 #ifndef HTMLDOCUMENTPROPERTIES_H
 #define HTMLDOCUMENTPROPERTIES_H

class Node;
class NodeModifsSet;
class AttributeItem;
class EditableTree;

#include <qptrlist.h>

#include "../../treeviews/tagattributeitems.h"
#include "htmldocumentpropertiesui.h"

class NodeLinkedViewItem : public AttributeItem
{
public:
	NodeLinkedViewItem(EditableTree *listView, const QString& title, const QString& title2);
	virtual ~NodeLinkedViewItem();

	Node *node;
	bool dirty, deleted;
};

/**
 * The HTML Document properties dialog.
 */
class htmlDocumentProperties : public htmlDocumentPropertiesui
{
public:
	htmlDocumentProperties( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
	~htmlDocumentProperties();

private slots:
	virtual void aboutToClose();
	virtual void newMetaItem();
	virtual void deleteCurrentMetaItem();
	virtual void newCSSRule();
	virtual void editCSSRule();
	virtual void deleteCurrentCSSRule();

	virtual void titleChanged(const QString &);
	virtual void metaChanged(QListViewItem * );
	virtual void CSSChanged(QListViewItem * );
	virtual void linkChanged( const QString& );

private:
	/**
	 * If we want to add a new Node, we must first create the basics (if necessary) i.e. html, body
	 * head nodes, and moving if necessary all the existing Nodes.
	 * @param modifs The changes are logged for the undo/redo system.
	 */
	void addBasicNodes(NodeModifsSet &modifs);
	//must be called after addBasicnodes
	void addBasicCssNodes(NodeModifsSet &modifs);
	void loadMetaNode(Node *node);
	void loadCSS(Node *node);

	QPtrList<NodeLinkedViewItem> CSSList, metaList;
	Node *titleNode, *htmlNode, *headNode, *linkNode, *bodyNode, *doctypeNode, *CSSNode;
	bool titleDirty, linkDirty;
};

 #endif
