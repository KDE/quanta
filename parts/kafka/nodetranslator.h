/***************************************************************************
                               nodetranslator.h
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

#ifndef NODETRANSLATOR_H
#define NODETRANSLATOR_H

class Node;
namespace DOM
{
	class Node;
}

/**
 * Base class which define an interface to "translate" DOM::Nodes
 * i.e. to modify the corresponding DOM::Node of a Node.
 * The aim is to modify the kafka view to provide visual aids
 * like a error system which highlight syntax error, and so on...
 * Defining this in XML files could be a nice idea once kafka 
 * support all the DTDs.
 */

class NodeTranslator
{
public:
	NodeTranslator(){}
	virtual ~NodeTranslator(){}

	/**
	 * This function modify the DOM::Node of the node.
	 * The DOM::Node must be built before calling this
	 * function.
	 * @param node The Node we want to "translate".
	 * @param parentDNode the parent DOM::Node of the root DOM::Node of node.
	 * @param nextDNode the DOM::Node next to the root DOM::Node of node.
	 */
	virtual bool translateNode(Node *node, DOM::Node parentDNode, DOM::Node nextDNode) = 0;

	/**
	 * This need to be strongly extended.
	 */
};

#endif
