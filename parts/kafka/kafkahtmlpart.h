/***************************************************************************
                               kafkahtmlpart.h
                             -------------------

    copyright            : (C) 2001 - The Kafka Team
                           (C) 2003 - Nicolas Deschildre
    email                : kde-kafka@master.kde.org && nicolasdchd@ifrance.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KAFKAHTMLPART_H
#define KAFKAHTMLPART_H

#include <kurl.h>
#include <khtml_part.h>
#include <khtml_events.h>
#include <kparts/event.h>

#include <qstringlist.h>
#include <qptrlist.h>
#include <qdockwindow.h>
#include <qevent.h>

#include <dom/dom_node.h>
#include <dom/dom2_range.h>
#include <dom/dom_element.h>

//#include "kafkahtmlpartplugin.h"



//*** if you want a very useful DOMTreeView attached to the kafkahtmlPart ***//
#define KAFKA_DEBUG_UTILITIES

#ifdef KAFKA_DEBUG_UTILITIES
class KafkaDOMTreeDialog;
#endif

class DOMString;
class KafkaHTMLPartPrivate;

/*
 * The main kafka class which handles user input(keyboard, mouse), DOM object
 * editing.
 */

class KafkaHTMLPart : public KHTMLPart
{
Q_OBJECT
public:
	KafkaHTMLPart(QWidget *parent, QWidget *widgetParent, const char *name);
	~KafkaHTMLPart();

	/**
	 * Category: Standard Functions
	 * Called by KafkaPart to create a new document
	 */
	void newDocument();

	/**
	 * Category: Standard Functions
	 * Called by KafkaPart to open a document
	 * @param url Specifies the URL to open
	 * @return Returns true if it worked, else false
	 */
	bool openDocument(const KURL &url);

	/**
	 * Category: Standard Functions
	 * Called by KafkaPart to save the current document
	 * @param url Specifies the URL to save to
	 * @return Returns true if it worked, else false
	 */
	bool saveDocument(const KURL &url);

	/**
	 * Category: Standard Functions
	 * Called by KafkaPart to set the readonly variable
	 * @param value Specifies wheter we accept keyboard input or not
	 */
	void setReadOnly(bool value);

	/**
	 * Category: Standard Functions
	 * Is the KafkaHTMLPart ReadOnly?
	 * @return Return wheter the part is writeable or not
	 */
	bool readOnly();

	/**
	 * Category: Standart Function
	 * Called to create new DOM::Nodes
	 * @param The name of the DOM::Node to be created
	 * @return Return the DOM::Node created
	 */
	 DOM::Node createNode(QString NodeName);

	/**
	 * Category: Standart Function
	 * Returns the current Node (which has the focus) and its offset.
	 * @param _currentNode The Node which has the cursor focus.
	 * @param offset The offset of the cursor in _currentNode.
	 */
	 void getCurrentNode(DOM::Node &_currentNode, int &offset);

	/**
	 * Category : Standart Function
	 * Get the next DOM::Node after _node.
	 * @param _node The DOM::Node the search starts from.
	 * @param goingTowardsRootNode A needed boolean for several consecutive call to
	 * this function.
	 * @param skipParentNodes Specifies if we should skip the parent Node when going up :
	 * this implies that some Nodes will not be returned two times.
	 * @param dontBlock Specifies if the search should or not be blocked by BlockingTags.
	 * @return Returns the next Node :)
	 */
	DOM::Node getNextNode(DOM::Node _node, bool &goingTowardsRootNode, bool skipParentNodes = false, bool dontBlock = false);

	/**
	 * The same that above, but this function search the previous DOM::Node.
	 */
	DOM::Node getPrevNode(DOM::Node _node, bool &goingTowardsRootNode, bool skipParentNodes = false, bool dontBlock = false);

	/**
	 * Category: Temporary function
	 * For debugging purposes only, shows the kafka DOM tree
	 * @return Return the view widget
	 */
	 void showDomTree();

public slots:
	/**
	 * Category: HTML Editing Functions
	 * Inserts a DOM::Node into the DOM::Document, after or before the panode.
	 * Should be used instead of DOM::Node::appendChild().
	 * @param node The node to add into the document
	 * @param panode The node before/after node
	 * @param insertBefore Specifies wheter to add node before panode or after panode
	 * @return Returns true if it worked, else false
	 */
	bool insertNode(DOM::Node node, DOM::Node panode, bool insertBefore);

	/**
	 * Category: HTML Editing Functions
	 * Inserts a DOM::Node into the DOM::Document, after or before the current DOM::Node.
	 * Should be used instead of DOM::Node::appendChild().
	 * @param node The node to add into the document
	 * @param insertBefore Specifies wheter to add node before the current node
	 * or after the current node
	 * @return Returns true if it worked, else false
	 */
	bool insertNode(DOM::Node node, bool insertBefore);

	/**
	 * Category: HTML Editing Functions
	 * Removes a DOM::Node from the DOM::Document.
	 * Should be used instead of DOM::Node::removeChild()
	 * @param node The Node to remove from the document
	 * @retun Returns true if it worked, else false
	 */
	bool removeNode(DOM::Node node);

	/**
	 * Category: HTML Editing Functions
	 * Adds text into a DOM::Node of type DOM::Node::TEXT_NODE
	 * @param node The node where text should be added
	 * @param text The text to be added
	 * @param position Specifies the position where to add text
	 */
	void insertText(DOM::Node node, const QString &text, int position);

	/**
	 * Category: HTML Editing Functions
	 * Adds text into the current DOM::Node
	 * @param text The text to add
	 * @param position Specifies the position where to add text, if it's -1
	 * the text will be appended after the last position
	 */
	void insertText(const QString &text, int position);

	/**
	 * Category: HTML Editing Functions
	 * Moves the cursor x-characters to the left
	 * @param value Specifies the charachters to move to the left
	 */
	void nextOffset(int value);

	/**
	 * Category: HTML Editing Functions
	 * Moves the cursor x-characters to the right
	 * @param value Specifies the charachters to move to the right
	 */
	void previousOffset(int value);

	/**
	 * Puts all the child Text DOM::Node  into a "normal" form where only
	* structure (e.g., elements, comments, processing instructions, CDATA
	* sections, and entity references) separates Text nodes, i.e., there are
	* neither adjacent Text nodes nor empty Text nodes.
	* Should be used instead of DOM::Node::normalize().
	* @param _node This node is the parent node of the childs normalized
	*/
	void normalize(DOM::Node _node);

	/**
	 * Set the cursor after having loaded the DOM tree from scratch
	 */
	void finishedLoading();

signals:
	/**
	 * Category: HTML Editing Signal
	 * Is emitted whenever the DOM tree has changed
	 * Not yet implemented
	 */
	void domChanged();

	/**
	 * Category: HTML Editing Signal
	 * Is emitted whenever a dom Node is inserted to the tree.
	 * @param _node is the node created.
	 */
	void domNodeInserted(DOM::Node _node);

	/**
	 * Category: HTML Editing Signal
	 * Is emitted whenever a DOM node has its properties modified.
	 * @param _node is the node modified.
	 */
	void domNodeModified(DOM::Node _node);

	/**
	 * Category: HTML Editing Signal
	 * Is emitted whenever a DOM node is about to be removed from the tree.
	 * @param _node is the node to be deleted.
	 * @param deleteChilds Specifies if we should delete the child nodes of _node.
	 */
	void domNodeIsAboutToBeRemoved(DOM::Node _node, bool deleteChilds);

	/**
	 * Category: HTML Editing Signal
	 * Is emitted whenever the cursor position change in one DOM::Node.
	 * @param _domNode The DOM::Node which contains the cursor.
	 * @param _offset The new cursorOffset.
	 */
	void domNodeNewCursorPos(DOM::Node _domNode, int offset);

protected:
	bool eventFilter(QObject *object, QEvent *event);

	virtual void khtmlMouseMoveEvent(khtml::MouseMoveEvent *event);
	virtual void khtmlMousePressEvent(khtml::MousePressEvent *event);
	virtual void khtmlMouseReleaseEvent(khtml::MouseReleaseEvent *event);
	virtual void khtmlDrawContentsEvent(khtml::DrawContentsEvent *event);

	virtual void timerEvent(QTimerEvent *e );


private:
//for debugging purposes
#ifdef KAFKA_DEBUG_UTILITIES
	KafkaDOMTreeDialog *domdialog;
#endif
//end

	void paintCursor(QPainter *p);
	void keyLeft();
	void keyRight();
	void keyUp();
	void keyDown();
	void keyBackspace();
	void keyDelete();
	void keyReturn();
	//temporary universam method to get cursor coordinates
	bool getCursor(DOM::Node _node, int offset, int &_x, int &_y, int &height);
	void postprocessCursorPosition();
	void makeCursorVisible(int xMargin = 50, int yMargin = 50);
	DOM::Node m_currentNode;
	DOM::Range m_currentSelection;
	KafkaHTMLPartPrivate *d;
	friend class KafkaHTMLPartPrivate;
};

#endif
