/***************************************************************************
                          node.cpp  -  description
                             -------------------
    begin                : Sun Apr 16 2000
    copyright            : (C) 2000 by Dmitry Poplavsky
    email                : pdima@mail.univ.kiev.ua
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

Node::Node( Node *parent )
{
  this->parent = parent;
  prev = next = child = 0L;
  tag = 0L;
  listItem = 0L;
  opened = false;
}


Node::~Node()
{
  if (child) { delete(child); child = 0L;}
  if (next)  { delete(next); next = 0L;}
  if ( tag ) { delete(tag); tag = 0L;}
}
