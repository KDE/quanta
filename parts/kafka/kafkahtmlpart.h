/***************************************************************************
                               kafkahtmlpart.h
                             -------------------

    copyright            : (C) 2001 - The Kafka Team
    email                : kde-kafka@master.kde.org
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

#include "kafkahtmlpartplugin.h"



//*** if you want a very useful DOMTreeView attached to the kafkahtmlPart ***//
#define DEBUG_UTILITIES

class DOMTreeView;
class KafkaHTMLPartPrivate;

class PluginEntry: public QObject
{
    Q_OBJECT
public:
    PluginEntry(QObject *parent):QObject(parent){;}
    ~PluginEntry(){;}
    KafkaHTMLPartPlugin *plugin;
    QStringList tags;                       
};

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
	 * Category: Standard Function
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
	
public slots:
	/**
	 * Category: HTML Editing Functions
	 * Inserts a DOM::Element into the DOM::Document, after or before the panode
	 * @param element The element to add into the document
	 * @param paelement The element before/after element
	 * @param insertBefore Specifies wheter to add element before panode or after panode
	 * @return Returns true if it worked, else false
	 */
	bool insertElement(DOM::Element element, DOM::Node panode, bool insertBefore);
	
	/**
	 * Category: HTML Editing Functions
	 * Inserts a DOM::Element into the DOM::Document, after or before the current DOM::Node
	 * @param node The element to add into the document
	 * @param insertBefore Specifies wheter to add element before the current node
	 * or after the current node
	 * @return Returns true if it worked, else false
	 */
	bool insertElement(DOM::Element node, bool insertBefore);

	/**
	 * Category: HTML Editing Functions
	 * Removes a DOM::Element from the DOM::Document
	 * @param element The element to remove from the document
	 * @retun Returns true if it worked, else false
	 */
	bool removeElement(DOM::Element element);
	
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
	 * FIXME: doesn't work for an value > 1
	 * @param value Specifies the charachters to move to the left
	 */
	void nextOffset(int value);

	/**
	 * Category: HTML Editing Functions
	 * Moves the cursor x-characters to the right
	 * FIXME: doesn't work for an value > 1
	 * @param value Specifies the charachters to move to the right
	 */
	void previousOffset(int value);

	/**
	 * Choose the right plugin on node change
         */
	void setNode(const DOM::Node &);

	/**
	* Activate changed node properties
	*/
	void setCurrentNode(DOM::Node);

signals:
	/**
	 * Category: HTML Editing Signal
	 * Is emitted whenever the DOM tree has changed
	 */
	void domChanged();
	
protected:
	bool eventFilter(QObject *object, QEvent *event);

	virtual void khtmlMouseMoveEvent(khtml::MouseMoveEvent *event);
	virtual void khtmlMousePressEvent(khtml::MousePressEvent *event);
	virtual void khtmlMouseReleaseEvent(khtml::MouseReleaseEvent *event);
	virtual void khtmlDrawContentsEvent(khtml::DrawContentsEvent *event);

	virtual void timerEvent(QTimerEvent *e );
	
	void loadPlugins();
	KafkaHTMLPartPlugin *loadPlugin(const QString& name);
	
	QPtrList<PluginEntry> plugins;
	QDockWindow *tagEditorParent;

private:
//for debugging purposes
#ifdef DEBUG_UTILITIES
	QDockWindow *DOMTreeParent;
	DOMTreeView *DOMTreeWidget;
#endif
//end

	void paintCursor(QPainter *p);
	void keyLeft();
	void keyRight();
	void keyUp();
	void keyDown();
	void keyBackspace();
	void keyDelete();
	//temporary universam method to get cursor coordinates
	bool getCursor(DOM::Node _node, int offset, int &_x, int &_y, int &height);
	//get the next node when possible (e.g. stops on Blockings tags)
	DOM::Node getNextNode(DOM::Node _node, bool &goingTowardsRootNode);
	DOM::Node getPrevNode(DOM::Node _node, bool &goingTowardsRootNode);
	void postprocessCursorPosition();
	void makeCursorVisible(int xMargin = 50, int yMargin = 50);
	DOM::Node m_currentNode;
	DOM::Range m_currentSelection;
	KafkaHTMLPartPrivate *d;
	friend class KafkaHTMLPartPrivate;
};

#endif
