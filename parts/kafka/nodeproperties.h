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

 class kNodeProperties
 {
 public:
 	kNodeProperties(){}
 	~kNodeProperties(){}

	//temporary, will change with multi-DTD support
	DOM::Node _rootNode;
	DOM::Node _leafNode;

 };

 #endif
