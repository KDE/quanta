/***************************************************************************
                              nodeproperties.cpp
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

 #include "../../parser/node.h"

 #include "nodeproperties.h"

kNodeAttrs::kNodeAttrs()
{
	node = 0L;
	type = 0;
	cbDeleted = false;
	cbModified = false;
	chCursorFocus = kNodeAttrs::no;
	ccEnter = false;
}

kNodeAttrs::~kNodeAttrs()
{

}
