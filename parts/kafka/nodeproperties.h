/***************************************************************************
                               nodeproperties.h
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

#ifndef NODEPROPERTIES_H
#define NODEPROPERTIES_H

#include <dom/dom_node.h>

class Node;

class kNodeAttrs
{
public:
	kNodeAttrs();
	~kNodeAttrs();


enum cursorFocus {
	no = 0,
	left,
	right,
	leftAndRight,
	singleNode,
	singleNodeAndItself,
	textNode
};

	int type;
	Node* node;
	bool cbDeleted;
	bool cbModified;
	int chCursorFocus;
	bool ccEnter;

};

#endif
