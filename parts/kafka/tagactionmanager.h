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

class TagActionSetAbstract;
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
    
    /**
     * This method is used to fill context menus with apropriated actions for node.
     * If you want to plug a single action to some widget use actionCollection().
     * @param widget The widget in wich the actions will be pluged.
     * @param node The context/current node.
     */
    void fillWithTagActions(QWidget* widget, DOM::Node const& node);
    
    static bool canIndentDTD(QString const& dtd);

    KActionCollection* actionCollection() const
    {
        return m_actionCollection;
    }

private:
    TagActionManager();
    TagActionManager(TagActionManager const&)
    {}

    void initActions(QWidget* parent);

private:
    static TagActionManager* s_mSelf;

    KActionCollection* m_actionCollection;
    QPtrList<TagActionSetAbstract> m_tagActionSets;
};

#endif
