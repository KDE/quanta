/***************************************************************************
                               tagactionmanager.h
                             -------------------
 
    copyright            : (C) 2004 - Paulo Moura Guedes
    email                : moura@kdewebdev.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TAGACTIONMANAGER_H
#define TAGACTIONMANAGER_H

class KActionCollection;
namespace DOM
{
class Node;
}

#include <qptrlist.h>
class QWidget;

class TagActionSet;
class TableTagActionSet;


/**
 * @author Paulo Moura Guedes
 * This class is a singleton.
*/
class TagActionManager
{
public:
    static TagActionManager* self();
    ~TagActionManager();

    void initActions(QWidget* parent);
    void fillWithTagActions(QWidget* widget, DOM::Node const& node);

    KActionCollection* actionCollection() const
    {
        return m_actionCollection;
    }

private:
    TagActionManager();
    TagActionManager(TagActionManager const&)
    {}

private:
    static TagActionManager* s_mSelf;

    KActionCollection* m_actionCollection;
    QPtrList<TagActionSet> m_tagActionSets;
};

#endif