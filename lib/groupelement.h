/***************************************************************************
    begin                : Fri Jul 15 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef GROUPELEMENT_H
#define GROUPELEMENT_H

#include "tag.h"

class Node;
class XMLStructGroup;

struct GroupElement{

    GroupElement() : tag(0) {};
    
    ~GroupElement() {delete tag;};  // I am the owner of this Tag
    
    /** The node which contains the element */
    Node *node;
    
    /** The tag which point to the actual place in the node 
     *  Attention! GroupElement will take ownership and destroy this in
     *  the destructor
     */
    Tag *tag;
    
    /**The parent node indicating the beginning of a structure */
    Node *parentNode;
    bool global;
    bool deleted;
    QString type; ///< If the group element has a TypeRx specified, the result of the search for TypeRx. Example: this will give the class name for an object
    XMLStructGroup *group; ///<is part of this group
  };

#endif
