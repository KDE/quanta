/***************************************************************************
                               nodeproperties.h
                             -------------------

    copyright            : (C) 2003, 2004 - Nicolas Deschildre
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

#ifndef NODEPROPERTIES_H
#define NODEPROPERTIES_H

#include <dom/dom_node.h>

class Node;

/**
 * This class basically acts as a link containing attributes between Nodes and DOM::Nodes.
 * Node->rootNode(), Node->leafNode() are Node->DOM::Node links (cf node.h)
 * domNodeProps[domNode.handle()] is DOM::Node->kNodeAttr link (cf wkafkapart.h)
 */
class kNodeAttrs
{
public:
	kNodeAttrs();
	~kNodeAttrs();

	/**
	 * Get/set the Node this kNodeAttrs is linked to.
	 */
	Node *getNode() {return m_node;}
	void setNode(Node *node) {m_node = node;}

	/**
	 * Get/set the DOM::Node this kNodeAttrs is linked to.
	 */
	DOM::Node getDomNode() {return m_domNode;}
	void setDomNode(DOM::Node domNode) {m_domNode = domNode;}

	/**
	 * Get/set if the DOM::Node linked by this kNodeAttrs has a corresponding Node.
	 * If not, this may reveal that it is a special DOM::Node which could be handled separatly.
	 */
	bool isLinkedToNode() {return m_isLinkedToNode;}
	void setIsLinkedToNode(bool isLinkedToNode) {m_isLinkedToNode = isLinkedToNode;}

	/**
	 * The different possible reasons why this kNodeAttr isn't linked to a Node.
	 */
	enum specialBehavior
	{
		/**
		 * No special behavior.
		 */
		none = 0,

		/**
		 * The DOM::Node linked by this kNodeAttrs is an empty Text DOM::Node
		 * which has been set at the left of a block so that the user can
		 * access this area with the cursor.
		 */
		emptyTextSurroundingBlockElementAtTheLeft,

		/**
		 * Same as above but at the right of a block.
		 */
		emptyTextSurroundingBlockElementAtTheRight,

		/**
		 * The DOM::Node linked by this kNodeAttrs is an empty Text DOM::Node
		 * which has been set as the only child of a childless element so that the user
		 * can access this area with the cursor.
		 */
		emptyTextAsChildOfAChildlessElement
	};

	/**
	 * Get/set the reason why this kNodeAttrs isn't linked to a Node.
	 */
	int specialBehavior() {return m_specialBehavior;}
	void setSpecialBehavior(int specialBehavior) {m_specialBehavior = specialBehavior;}

	/**
	 * DEPRECATED. Keept for backward compatibility with old kafkaWIdget code!
	 */
	int getType() {return m_type;}
	void setType(int type) {m_type = type;}
	bool cbDel() {return m_cbDeleted;}
	void setCBDeleted(bool cbDeleted) {m_cbDeleted = cbDeleted;}
	bool cbMod() {return m_cbModified;}
	void setCBModified(bool cbModified) {m_cbModified = cbModified;}
	int chCurFoc() {return m_chCursorFocus;}
	void setCHCursorFocus(int chCursorFocus) {m_chCursorFocus = chCursorFocus;}
	bool ccanEnter() {return m_ccEnter;}
	void setCCEnter(bool ccEnter) {m_ccEnter = ccEnter;}

	enum cursorFocus
	{
		no = 0,
		left,
		right,
		leftAndRight,
		singleNode,
		singleNodeAndItself,
		textNode
	};

private:
	Node* m_node;
	DOM::Node m_domNode;
	bool m_isLinkedToNode;
	int m_specialBehavior;
	int m_type;
	bool m_cbDeleted;
	bool m_cbModified;
	int m_chCursorFocus;
	bool m_ccEnter;

};

#endif
