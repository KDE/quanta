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

#include "../../document.h"
#include "../../quanta.h"

class Node;

/** This class gathers all the basic functions needed by kafka. */

class kafkaCommon
{
public:
	kafkaCommon() {}
	~kafkaCommon() {}

	/**
	 * This function returns the next Node after _node.
	 * @param _node It is the Node from which we want the next Node.
	 * @param goUp This boolean specifies if we should go up or down in the tree.
	 * For a normal use, It must be set to false at the
	 * beginning and then the same boolean must be used when using
	 * several times this function.
	 * @param endNode Specifies at which Node the search should end.
	 * @return Returns the next Node.
	 */
	Node *getNextNode(Node *_node, bool &goUp, Node *endNode = 0L);

	/**
	 * Prints in stdout the current Node tree.
	 */
	void coutTree(Node *node, int indent);

	 /**
	 *@return Returns true if the kafkaPart is loaded. It does not imply that
	 * the kafkaPart has the cursor focus. See kafkaPartHasCursorFocus().
	 */
	/**bool kafkaPartIsLoaded();*/

	/**
	 * @return Returns true if the kafkaPart has the cursor focus.
	 */
	/**bool kafkaPartHasCursorFocus();*/

};

#endif
