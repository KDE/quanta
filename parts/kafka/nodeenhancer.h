/***************************************************************************
                             nodeenhancer.h
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

#ifndef NODEENHANCER_H
#define NODEENHANCER_H

class KConfig;
class Node;
namespace DOM
{
	class Node;
}

/**
 * Base class which define an interface to enhance DOM::Nodes
 * i.e. to modify the corresponding DOM::Node of a Node.
 * The aim is to modify the kafka view to provide visual aids
 * like a error system which highlight syntax error, and so on...
 * Defining this in XML files could be a nice idea once kafka
 * support all the DTDs.
 */

class NodeEnhancer
{
public:
	NodeEnhancer(){}
	virtual ~NodeEnhancer(){}

	/**
	 * This function modify the DOM::Node of the node.
	 * The DOM::Node must be built before calling this
	 * function.
	 * @param node The Node we want to enhance.
	 * @param parentDNode the parent DOM::Node of the root DOM::Node of node.
	 * @param nextDNode the DOM::Node next to the root DOM::Node of node.
	 */
	virtual bool enhanceNode(Node *node, DOM::Node parentDNode, DOM::Node nextDNode) = 0;

	/**
	 * This function apply some modifications once the whole DOM::Node tree is built.
	 * @param domNode The domNode we want to add some modifications.
	 */
	virtual void postEnhanceNode(DOM::Node domNode) = 0;

	/**
	 * This function un-apply the modifications made by postEnhanceNode()
	 * @param domNode The DOM::Node we want to un-enhance!
	 */
	virtual void postUnenhanceNode(DOM::Node domNode) = 0;

	/**
	* Read the config.
	* @m_config The config to read.
	*/
	virtual void readConfig(KConfig *m_config) = 0;

	/**
	 * This need to be strongly extended.
	 */
};

#endif
