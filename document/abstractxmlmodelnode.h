/*
Copyright (C) 2009  Ruan Strydom <ruan@jcell.co.za>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef ABSTRACTXMLMODELNODE_H
#define ABSTRACTXMLMODELNODE_H

class AbstractXmlModelNode
{
  public:
    virtual ~AbstractXmlModelNode(){}
    virtual AbstractXmlModelNode *parent() = 0;
    virtual AbstractXmlModelNode *child ( int i ) = 0;
    virtual int childCount () = 0;
    virtual int row() = 0;
    virtual int lineNumber() = 0;
    virtual int columnNumber() = 0;
    virtual bool isRoot() {return parent() == 0;}
};

#endif // ABSTRACTXMLMODELNODE_H
