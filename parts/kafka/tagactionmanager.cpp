/***************************************************************************
                               tagactionmanager.cpp
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

#include <kstaticdeleter.h>
#include <kactioncollection.h>
#include <kaction.h>
#include <khtmlview.h>
#include <klocale.h>
#include <dom/dom_node.h>

#include <qwidget.h>

#include "tagactionmanager.h"
#include "tagactionset.h"
#include "resource.h"
#include "wkafkapart.h"


TagActionManager* TagActionManager::s_mSelf = 0;
static KStaticDeleter<TagActionManager> staticDeleter;


TagActionManager::TagActionManager()
    : m_actionCollection(0)
{
    s_mSelf = this;
    initActions(KafkaDocument::ref()->getKafkaWidget()->view());
}

TagActionManager::~TagActionManager()
{}

TagActionManager* TagActionManager::self()
{
    if (!s_mSelf)
    {
        staticDeleter.setObject(s_mSelf, new TagActionManager());
    }

    return s_mSelf;
}

void TagActionManager::initActions(QWidget* parent)
{
    Q_ASSERT(parent);
    
    m_actionCollection = new KActionCollection(parent);
    
    TagActionSet* general(new TagActionSet());
    general->initActions(parent);
    m_tagActionSets.append(general);
        
    TableTagActionSet* table(new TableTagActionSet());
    table->initActions(parent);
    m_tagActionSets.append(table);
}

void TagActionManager::fillWithTagActions(QWidget* widget, DOM::Node const& node)
{
    TagActionSetAbstract* tagActionSet = 0;
    for(tagActionSet = m_tagActionSets.first(); tagActionSet; tagActionSet = m_tagActionSets.next())
        tagActionSet->fillWithTagActions(widget, node);
}

bool TagActionManager::canIndentDTD(QString const& dtd)
{
    return (dtd.contains("HTML", false) ||
            dtd.contains("XML", false));
}
