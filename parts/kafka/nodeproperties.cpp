/***************************************************************************
                              nodeproperties.cpp
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

#include "node.h"

#include "nodeproperties.h"

kNodeAttrs::kNodeAttrs()
{
	m_node = 0L;
	m_isLinkedToNode = true;
	m_specialBehavior = kNodeAttrs::none;
	m_type = 0;
	m_cbDeleted = false;
	m_cbModified = false;
	m_chCursorFocus = kNodeAttrs::no;
	m_ccEnter = false;
}

kNodeAttrs::~kNodeAttrs()
{

}
