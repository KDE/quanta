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
#include <dom/dom_element.h>

#include "kafkahtmlview.h"



class DOMString;
class KafkaHTMLPartPrivate;
class WKafkaPart;

/*
 * The main kafka class which handles user input(keyboard, mouse), DOM object
 * editing.
 */

class KafkaHTMLPart : public KHTMLPart
{
Q_OBJECT
public:
	KafkaHTMLPart(QWidget *parent, QWidget *widgetParent, WKafkaPart *part,
		const char *name = 0);
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
	 * Set the focus.
	 * @param node The DOM::Node which will get the focus.
	 * @param offset The offset of the cursor if node is a DOM::Node::TEXT_NODE
	 */
	void setCurrentNode(DOM::Node node, int offset);

	 /**
	 * Category : Standart Function
	 * Get the next DOM::Node after _node.
	 * @param _node The DOM::Node the search starts from.
	 * @param goUp A needed boolean for several consecutive call to
	 * this function. Setting it to false by default, or to true if we don't want to go through
	 * the childs of _node.
	 * @param skipParentNodes Specifies if we should skip the parent Node when going up :
	 * this implies that Nodes will not be returned two times.
	 * @param dontBlock Specifies if the search should or not be blocked by BlockingTags.
	 * @param _endNode Specifies at which DOM::Node the search should end. It is useful
	 * when setting skipParentNodes to true.
	 * @return Returns the next Node :)
	 */
	DOM::Node getNextNode(DOM::Node _node, bool &goUp, bool skipParentNodes = false, bool dontBlock = false, DOM::Node _endNode = 0L);

	/**
	 * The same that above, but this function search the previous DOM::Node.
	 */
	DOM::Node getPrevNode(DOM::Node _node, bool &goUp, bool skipParentNodes = false, bool dontBlock = false, DOM::Node _endNode = 0L);

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
	 * @param insertChilds Specifies if we should insert the _node's childs
	 */
	void domNodeInserted(DOM::Node _node, bool insertChilds);

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

	/**
	 * Category: Widget Signal
	 * Is emitted whenever the KafkaHTMLPart widget get or lost the focus
	 * @param focus True if it has got the focus, false otherwise.
	 */
	 void hasFocus(bool focus);

protected:
	bool eventFilter(QObject *object, QEvent *event);

	virtual void khtmlMouseMoveEvent(khtml::MouseMoveEvent *event);
	virtual void khtmlMousePressEvent(khtml::MousePressEvent *event);
	virtual void khtmlMouseReleaseEvent(khtml::MouseReleaseEvent *event);
	virtual void khtmlDrawContentsEvent(khtml::DrawContentsEvent *event);

	virtual void timerEvent(QTimerEvent *e );


private:
	/**
	 * Move the cursor one position left.
	 */
	void keyLeft();
	/**
	 * Move the cursor one position right.
	 */
	void keyRight();
	/**
	 * Move the cursor one position up.
	 */
	void keyUp();
	/**
	 * Move the cursor one position down.
	 */
	void keyDown();
	/**
	 * Delete one character/DOM::Node left to the cursor.
	 */
	void keyBackspace();
	/**
	 * Delete one character/DOM::Node right to the cursor.
	 */
	void keyDelete();
	/**
	 * Break the current line.
	 */
	void keyReturn();
	/**
	 * Temporary universal method to get the cursor coordinates of a DOM::Node.
	 */
	bool getCursor(DOM::Node _node, int offset, int &_x, int &_y, int &height);
	/**
	 * Postprocess the cursor position, i.e. when the cursor is between two
	 * DOM::Nodes, it set the cursor in the right node to make edition as
	 * in a word processor.
	 */
	void postprocessCursorPosition();
	/**
	 * Make the cursor visible whenever it is going :-)
	 * @param xMargin The minimum margin in the X axis between the cursor
	 * and the edge of the widget.
	 * @param yMargin The minimim margin in the Y axis between the cursor
	 * and the edge of the widget.
	 */
	void makeCursorVisible(int xMargin = 50, int yMargin = 50);
	/**
	 * Paints the cursor.
	 * @param p It requires a QPainter to draw the cursor :)
	 */
	void paintCursor(QPainter *p);
	DOM::Node m_currentNode;
	KafkaHTMLPartPrivate *d;
	WKafkaPart *w;
	friend class KafkaHTMLPartPrivate;
};

#endif
