/***************************************************************************
                               kafkacommon.h
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

#ifndef KAFKACOMMON_H
#define KAFKACOMMON_H

#include <qvaluelist.h>
#include <dom/dom_node.h>

class Node;
namespace DOM
{
	class Node;
}

/**
 * For heavy debug including Node Tree in stdout printing, a DOM::Node tree widget.
 */
//#define HEAVY_DEBUG

/**
 * Light debugging, including functions name in stdout printing.
 */
#define LIGHT_DEBUG


/** This class gathers all the basic functions needed by kafka. */

class kafkaCommon
{
public:
	kafkaCommon() {}
	~kafkaCommon() {}

	/**
	 * This function returns the next Node after node : the first child of
	 * node if available, else its next sibling if available, else the next
	 * available next sibling of a parent of node.
	 * @param _node It is the Node from which we want the next Node.
	 * @param goUp This boolean specifies if we should go up or down in the tree.
	 * For a normal use, It must be set to false at the
	 * beginning and then the same boolean must be used when using
	 * several times this function.
	 * @param endNode Specifies at which Node the search should end.
	 * @return Returns the next Node.
	 */
	static Node* getNextNode(Node *node, bool &goUp, Node *endNode = 0L);

	/**
	 * This function returns the next DOM::Node after node : the first child of
	 * DOM::Node if available, else its next sibling if available, else the next
	 * available next sibling of a parent of node.
	 * @param node The DOM::Node the search starts from.
	 * @param goUp This boolean specifies if we should go up or down in the tree.
	 * For a normal use, It must be set to false at the
	 * beginning and then the same boolean must be used when using
	 * several times this function.
	 * @param returnParentNode Specifies if there are no child and next sibling, if
	 * we should return the parent.
	 * @param endNode Specifies at which DOM::Node the search should end. It is useful
	 * when setting returnParentNode to false.
	 * @return the next Node.
	 */
	static DOM::Node getNextDomNode(DOM::Node node, bool &goUp, bool returnParentNode = false, DOM::Node endNode = DOM::Node());

	/**
	 * Fits the Nodes position after a change in the Node tree.
	 * @param startNode The Node where the update starts.
	 * @param colMovement The number of columns that should be
	 * added/retrieved from the column position.
	 * @param lineMovement The number of lines that should be
	 * added/retrieved from the line position.
	 * @param colEnd The column position where the update should stop.
	 * @param lineEnd The line position where the update should stop.
	 */
	static void fitsNodesPosition(Node* startNode, int colMovement, int lineMovement = 0,
		int colEnd = -2, int lineEnd = -2);

	/**
	 * Gets the location of a Node in a pointer-independant suit of ints e.g. 1,3,5 means
	 * that the node is the fifth child of the third child of the root Node. Efficient when
	 * deleting the Node tree and rebuilding it when switching between Documents.
	 * @param _node The Node we want the location.
	 * @return Returns the location.
	 */
	static QValueList<int> getLocation(Node* node);

	/**
	 * Get the node corresponding to a location. See the above function.
	 * @param loc We want the Node from this location.
	 * @return Returns the Node at location loc.
	 */
	static Node* getNodeFromLocation(QValueList<int> loc);

	/**
	 * Prints in stdout the current Node tree.
	 */
	static void coutTree(Node *node, int indent);


};

#endif
