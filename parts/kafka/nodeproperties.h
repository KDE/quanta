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
	int getType() {return type;}
	Node *getNode() {return node;}
	bool cbDel() {return cbDeleted;}
	bool cbMod() {return cbModified;}
	int chCurFoc() {return chCursorFocus;}
	bool ccanEnter() {return ccEnter;}
	void setType(int _type) {type = _type;}
	void setNode(Node *_node) {node = _node;}
	void setCBDeleted(bool _cbDeleted) {cbDeleted = _cbDeleted;}
	void setCBModified(bool _cbModified) {cbModified = _cbModified;}
	void setCHCursorFocus(int _chCursorFocus) {chCursorFocus = _chCursorFocus;}
	void setCCEnter(bool _ccEnter) {ccEnter = _ccEnter;}

enum cursorFocus {
	no = 0,
	left,
	right,
	leftAndRight,
	singleNode,
	singleNodeAndItself,
	textNode
};

private:
	int type;
	Node* node;
	bool cbDeleted;
	bool cbModified;
	int chCursorFocus;
	bool ccEnter;

};

#endif
