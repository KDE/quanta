/***************************************************************************
                               kafkahtmlpart.h
                             -------------------

    copyright            : (C) 2001 - The Kafka Team
                           (C) 2003, 2004 - Nicolas Deschildre
    email                : kde-kafka@master.kde.org && ndeschildre@kdewebdev.org
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

class DOMString;
class KafkaWidgetPrivate;
class KafkaDocument;
class NodeModifsSet;
class QPopupMenu;

/*
 * It is the main central widget which get the cursor focus and takes care of "simple" edition
 * such as typing text.
 */

class KafkaWidget : public KHTMLPart
{
    Q_OBJECT
public:
    KafkaWidget(QWidget *parent, QWidget *widgetParent, KafkaDocument *part,
                const char *name = 0);
    ~KafkaWidget();

    /**
     * Category: Standard Functions
     * Called by KafkaPart to create a new document
     */
    void newDocument();

    /**
     * Returns the current Node (which has the focus) and its offset.
     * @param currentNode The Node which has the cursor focus.
     * @param offset The offset of the cursor in _currentNode.
     */
    void getCurrentNode(DOM::Node &currentNode, long &offset);

    /**
     * Set the focus.
     * @param node The DOM::Node which will get the focus.
     * @param offset The offset of the cursor if node is a DOM::Node::TEXT_NODE
     */
    void setCurrentNode(DOM::Node node, int offset);

    /**
     * Convenience function to set the current node. 
     * @param node The Quanta node which will get the focus.
     * @param offset The offset of the cursor if node tag is of type Tag::Text.
     */
    void setCurrentNode(Node* cursorNode, int cursorOffset);
    
    /**
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
    DOM::Node getNextNode(DOM::Node _node, bool &goUp, bool skipParentNodes = false,
                          bool dontBlock = false, DOM::Node _endNode = DOM::Node());

    /**
     * The same that above, but this function search the previous DOM::Node.
     */
    DOM::Node getPrevNode(DOM::Node _node, bool &goUp, bool skipParentNodes = false,
                          bool dontBlock = false, DOM::Node _endNode = DOM::Node());

    /**
     * Set the TagActions checked/unchecked in context.
     * This is called when the cursor position changes.
     * @param domNode 
     * @param offset 
     */
    void updateToggableTagActions(/*const DOM::Node &domNode, long offset*/) const;


    /** --------------- DOM::Node modifications -------------------------- */

    /**
     * It will move DOM::Nodes from startNode to endNode as children of newParent. It does NOT check
     * if the move is valid, so it may crash. Please check before with kafkaCommon::parentSupports().
     * @param newParent The new parent of the DOM::Nodes.
     * @param startNode The first node to move.
     * @param endNode Starting from endNode, the last sibling to move.
     * @param refNode The reference DOM::Node.
     * @bool before If set to true, the DOM::nodes will be moved at the left of refNode,
     * otherwise they will be happenend at the right of refNode.
     */
    void moveDomNodes(DOM::Node newParent, DOM::Node startNode, DOM::Node endNode,
                      DOM::Node refNode, bool before);

public slots:

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
     * Puts all the child Text DOM::Node  into a "normal" form where only
    * structure (e.g., elements, comments, processing instructions, CDATA
    * sections, and entity references) separates Text nodes, i.e., there are
    * neither adjacent Text nodes nor empty Text nodes.
    * Should be used instead of DOM::Node::normalize().
    * @param _node This node is the parent node of the childs normalized
    */
    void normalize(DOM::Node _node);

    /**
     * Set the cursor after having loaded the DOM tree from scratch.
     * Look for the first available location for the cursor.
     */
    void putCursorAtFirstAvailableLocation();

    /**
     * Reload the TagAttributesTree if necessary;
     * @param domNode The current DOM::Node.
     * @param offset The current cursor offset.
     */
    void slotNewCursorPos(const DOM::Node &domNode, long offset);

    /**
     * Called to set the cursor Position with a QTimer::singleShot() after calling
     *  document->updateRendering()
     */
    void slotDelayedSetCaretPosition();

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
     * @param node is the node created.
     * @param insertChilds Specifies if we should insert the _node's childs
            * @param modifs The changes made are logged into modifs.
     */
    void domNodeInserted(DOM::Node node, bool insertChilds, NodeModifsSet *modifs);

    /**
     * Category: HTML Editing Signal
     * Is emitted whenever a DOM node has its properties modified.
     * @param node is the node modified.
            * @param modifs The changes made are logged into modifs.
     */
    void domNodeModified(DOM::Node node, NodeModifsSet *modifs);

    /**
     * Category: HTML Editing Signal
     * Is emitted whenever a DOM node is about to be removed from the tree.
     * @param node is the node to be deleted.
     * @param deleteChilds Specifies if we should delete the child nodes of _node.
            * @param modifs The changes made are logged into modifs.
     */
    void domNodeIsAboutToBeRemoved(DOM::Node node, bool deleteChilds, NodeModifsSet *modifs);

    /**
     * Is emitted whenever a DOM::Node is about to be moved in the tree.
     * @param node The DOM::Node which will be moved.
     * @param newParent The new parent of domNode.
     * @param before domNode will be placed before before.
            * @param modifs The changes made are logged into modifs.
     */
    void domNodeIsAboutToBeMoved(DOM::Node node, DOM::Node newParent, DOM::Node before, NodeModifsSet *modifs);

    /**
     * Category: HTML Editing Signal
     * Is emitted whenever the cursor position change in one DOM::Node.
     * @param node The DOM::Node which contains the cursor.
     * @param _offset The new cursorOffset.
     */
    void domNodeNewCursorPos(DOM::Node node, int offset);

    /**
     * Category: Widget Signal
     * Is emitted whenever the KafkaWidget widget get or lost the focus
     * @param focus True if it has got the focus, false otherwise.
     */
    void hasFocus(bool focus);

protected:
    bool eventFilter(QObject *object, QEvent *event);

    virtual void khtmlMouseMoveEvent(khtml::MouseMoveEvent *event);
    virtual void khtmlMousePressEvent(khtml::MousePressEvent *event);
    virtual void khtmlMouseReleaseEvent(khtml::MouseReleaseEvent *event);
    virtual void khtmlDrawContentsEvent(khtml::DrawContentsEvent *event);

private slots:

    void slotContextMenuRequested(const QString& url, const QPoint& point);

private:
#if 0
    /**
     * NOT IMPLEMENTED.
     * Internal function used by keyBackspace(), keyDelete() to delete Nodes.
     * @param startNode The DOM::Node location of the point where we start deleting Nodes.
     * @param offset The offset location of the point where we start deleting Nodes. They can be
     * updated.
     * @param backspace Specifies if it should delete Nodes backward (backspace keystroke)
     * or torwards (delete backspace).
     */
    void keyDeleteNodes(DOM::Node &startNode, long &offset, bool backspace);
#endif

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
     * @param specialPressed If a special touch is pressed e.g. ctrl, we try to insert a BR,
     * otherwise we try to insert a P.
     */
    void keyReturn(bool specialPressed);

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
     * Removes the selection and places the cursor in the right place.
     */
    void removeSelection();

    void applyQueuedToggableTagActions();

private:
    /**
     * When some changes are made, store the changes made in m_modifs.
     * It will be created when a key is pressed, and will be submitted to
     * the undoRedo system after the changes have been made.
     */
    NodeModifsSet *m_modifs;

    DOM::Node m_currentNode;
    KafkaWidgetPrivate *d;
    KafkaDocument *w;
    friend class KafkaWidgetPrivate;

    QPopupMenu* m_contextPopupMenu;
};

#endif
