/***************************************************************************
                               tagactionset.cpp
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

#include <kactioncollection.h>
#include <kactionclasses.h>
#include <dom/dom_node.h>
#include <dom/dom_string.h>
#include <klocale.h>

#include <qwidget.h>

#include "tagactionset.h"
#include "tagactionmanager.h"
#include "kafkacommon.h"
#include "undoredo.h"
#include "wkafkapart.h"
#include "node.h"
#include "quantaview.h"
#include "viewmanager.h"
#include "tag.h"
#include "quantacommon.h"
#include "document.h"
#include "resource.h"


TagActionSet::TagActionSet(QObject *parent, const char *name)
    : QObject(parent, name), m_currentNode(0), m_separator(0)
{
    m_separator = new KActionSeparator();
}

TagActionSet::~TagActionSet()
{}

Node* TagActionSet::parentTag(Node* node, QString const& tagName)
{
    Q_ASSERT(node);
    
    Node* aux = node;
    while(aux && aux->nodeName().lower() != tagName)
        aux = aux->parent;
    
    return aux;
}

Node* TagActionSet::firstChildTag(Node* startParentNode, QString const& tagName)
{
    Node* aux = startParentNode;
    while(aux && aux->nodeName().lower() != tagName)
    {
        aux = aux->nextSibling();
        if(!startParentNode->hasForChild(aux))
            return 0;
    }
        
    return aux;
}

bool TagActionSet::fillWithTagActions(QWidget* /*widget*/, DOM::Node const& node)
{
    m_currentDomNode = node;
    m_currentNode = KafkaDocument::ref()->getNode(m_currentDomNode);
    
    return m_currentNode;
}

//_____________________________________________________________________________

TableTagActionSet::TableTagActionSet(QObject *parent, const char *name)
        : TagActionSet(parent, name), m_tableActionMenu_0(0), m_insertActionMenu_1(0)
{}

bool TableTagActionSet::isInTagContext() const
{
    return parentTag(m_currentNode, "table");
}

void TableTagActionSet::initActionMenus(QWidget* widget)
{
    Q_ASSERT(!m_tableActionMenu_0);

    m_tableActionMenu_0 = new KActionMenu(i18n("Table..."), widget);
    m_insertActionMenu_1 = new KActionMenu(i18n("Insert..."), m_tableActionMenu_0);
}


void TableTagActionSet::initActions(QWidget* parent)
{
    if(!m_tableActionMenu_0)
        initActionMenus(parent);

    KActionCollection* ac(TagActionManager::self()->actionCollection());

    QString actionName = "insert_table";
    //m_actionNames += actionName;
    new KAction(i18n("Insert Table..."), 0, this,
                SLOT(slotInsertTable()),
                ac, actionName);

    actionName = "insert_row_above";
    //m_actionNames += actionName;
    new KAction(i18n("Insert Row above"), 0, this,
                SLOT(slotInsertRowAbove()),
                ac, actionName);

    actionName = "insert_row_bellow";
    //m_actionNames += actionName;
    new KAction(i18n("Insert Row bellow"), 0, this,
                SLOT(slotInsertRowBellow()),
                ac, actionName);

    actionName = "insert_column_left";
    //m_actionNames += actionName;
    new KAction(i18n("Insert Column Left"), 0, this,
                SLOT(slotInsertColumnLeft()),
                ac, actionName);

    actionName = "insert_column_right";
    //m_actionNames += actionName;
    new KAction(i18n("Insert Column Right"), 0, this,
                SLOT(slotInsertColumnRight()),
                ac, actionName);
}

bool TableTagActionSet::fillWithTagActions(QWidget* widget, DOM::Node const& node)
{
    bool validNode = TagActionSet::fillWithTagActions(widget, node);
    
    if(!validNode || !isInTagContext(/*node*/))
    {
        unplugAllActions(widget);
        return false;
    }

    KActionCollection* ac(TagActionManager::self()->actionCollection());

    // Table
    bool emptyTableActionMenu_0 = true;
    
// _____________________________________________________________________________
    
    // Insert
    bool emptyInsertActionMenu_1 = true;
    
    // Insert Table
    KAction* insertTableAction = ac->action("insert_table");
    Q_ASSERT(insertTableAction);

    m_insertActionMenu_1->remove
    (insertTableAction);
    if(canInsertTable())
    {
        emptyTableActionMenu_0 = emptyInsertActionMenu_1 = false;
        m_insertActionMenu_1->insert(insertTableAction);
        
        m_insertActionMenu_1->insert(m_separator);
    }
    // Insert Row Above
    KAction* insertRowAboveAction = ac->action("insert_row_above");
    Q_ASSERT(insertRowAboveAction);

    m_insertActionMenu_1->remove(insertRowAboveAction);
    if(canInsertRowAbove())
    {
        emptyTableActionMenu_0 = emptyInsertActionMenu_1 = false;
        m_insertActionMenu_1->insert(insertRowAboveAction);
        
        //m_insertActionMenu_1->insert(m_separator);
    }
    // Insert Row Bellow
    KAction* insertRowBellowAction = ac->action("insert_row_bellow");
    Q_ASSERT(insertRowBellowAction);

    m_insertActionMenu_1->remove(insertRowBellowAction);
    if(canInsertRowBellow())
    {
        emptyTableActionMenu_0 = emptyInsertActionMenu_1 = false;
        m_insertActionMenu_1->insert(insertRowBellowAction);
        
        m_insertActionMenu_1->insert(m_separator);
    }
    // Insert Column Left
    KAction* insertColumnLeftAction = ac->action("insert_column_left");
    Q_ASSERT(insertColumnLeftAction);

    m_insertActionMenu_1->remove(insertColumnLeftAction);
    if(canInsertColumnLeft())
    {
        emptyTableActionMenu_0 = emptyInsertActionMenu_1 = false;
        m_insertActionMenu_1->insert(insertColumnLeftAction);
        
        //m_insertActionMenu_1->insert(m_separator);
    }
    // Insert Column Left
    KAction* insertColumnRightAction = ac->action("insert_column_right");
    Q_ASSERT(insertColumnRightAction);

    m_insertActionMenu_1->remove(insertColumnRightAction);
    if(canInsertColumnRight())
    {
        emptyTableActionMenu_0 = emptyInsertActionMenu_1 = false;
        m_insertActionMenu_1->insert(insertColumnRightAction);
        
        //m_insertActionMenu_1->insert(m_separator);
    }
    
// _____________________________________________________________________________
    
    // Table
    m_tableActionMenu_0->unplug(widget);
    if(!emptyTableActionMenu_0)
    {
        m_tableActionMenu_0->plug(widget);

        m_tableActionMenu_0->remove(m_insertActionMenu_1);
        if(!emptyInsertActionMenu_1)
            m_tableActionMenu_0->insert(m_insertActionMenu_1);
    }
    
    return true;
}

void TableTagActionSet::unplugAllActions(QWidget* widget) const
{
    m_separator->unplugAll();
    m_tableActionMenu_0->unplug(widget);
}

// _____________________________________________________________________________

bool TableTagActionSet::canInsertTable() const
{
    return false;
//     return isInTagContext(currentDomNode()); // TODO Implement slotInsertTable
}

void TableTagActionSet::slotInsertTable()
{
    Q_ASSERT(m_currentNode);

}

bool TableTagActionSet::canInsertRowAbove() const
{
    return isInTagContext() && parentTag(m_currentNode, "tbody");
}

void TableTagActionSet::slotInsertRowAbove()
{
    Q_ASSERT(m_currentNode);
    
    Node* nearRow = parentTag(m_currentNode, "tr");
    
    if(!nearRow)
        return;

    Node* nodeParent= nearRow->parent;
    
    QuantaView* view = ViewManager::ref()->activeView();
    NodeModifsSet *modifs = new NodeModifsSet();
    Node* nodeToInsert = buildEmptyRowSubtree();

    kafkaCommon::insertNodeSubtree(nodeToInsert, nodeParent, nearRow, modifs);

    view->document()->docUndoRedo->addNewModifsSet(modifs, undoRedo::NodeTreeModif);
    
    kafkaCommon::coutTree(baseNode, 3);    
}

bool TableTagActionSet::canInsertRowBellow() const
{
    return isInTagContext();
}

void TableTagActionSet::slotInsertRowBellow()
{
    Q_ASSERT(m_currentNode);
    
    Node* nearRow = 0;
    Node* aux = parentTag(m_currentNode, "thead");
    if(aux)
        nearRow= firstChildTag(tableStart(), "tr");
    else
        nearRow = parentTag(m_currentNode, "tr");
        
    if(!nearRow)
        return;

    Node* nodeParent= nearRow->parent;
    Node* nextSibling = nearRow->SNext();
    
    QuantaView* view = ViewManager::ref()->activeView();
    NodeModifsSet *modifs = new NodeModifsSet();
    Node* nodeToInsert = buildEmptyRowSubtree();

    kafkaCommon::insertNodeSubtree(nodeToInsert, nodeParent, nextSibling, modifs);

    view->document()->docUndoRedo->addNewModifsSet(modifs, undoRedo::NodeTreeModif);
    
    kafkaCommon::coutTree(baseNode, 3);    
}

bool TableTagActionSet::canInsertColumnLeft() const
{
    return isInTagContext();
}

void TableTagActionSet::slotInsertColumnLeft()
{
    Q_ASSERT(m_currentNode);
    
    QuantaView* view = ViewManager::ref()->activeView();
    NodeModifsSet *modifs = new NodeModifsSet();

    Node* nodeToInsertInBody = buildEmptyTBodyCellSubtree();
    Node* nodeToInsertInHead = buildEmptyTHeadCellSubtree();
    Q_ASSERT(nodeToInsertInBody);
    Q_ASSERT(nodeToInsertInHead);
            
    int const _currentColumnIndex = currentColumnIndex();
    
    // thead
    Node* trChild = firstChildTag(firstChildTag(tableStart(), "thead"), "tr");
    while(trChild)
    {
        Node* thChild = firstChildTag(trChild, "th");
        for(int i = 0; i != _currentColumnIndex; ++i)
            thChild = thChild->SNext();
        
        kafkaCommon::insertNodeSubtree(nodeToInsertInHead, trChild, thChild, modifs);
        
        nodeToInsertInHead = kafkaCommon::duplicateNodeSubtree(nodeToInsertInHead);
        
        trChild = trChild->SNext();
    }    
    
    // tbody
    trChild = firstChildTag(firstChildTag(tableStart(), "tbody"), "tr");
    while(trChild)
    {
        Node* tdChild = firstChildTag(trChild, "td");
        for(int i = 0; i != _currentColumnIndex; ++i)
            tdChild = tdChild->SNext();
        
        kafkaCommon::insertNodeSubtree(nodeToInsertInBody, trChild, tdChild, modifs);
        
        nodeToInsertInBody = kafkaCommon::duplicateNodeSubtree(nodeToInsertInBody);
        
        trChild = trChild->SNext();
    }    
    
    view->document()->docUndoRedo->addNewModifsSet(modifs, undoRedo::NodeTreeModif);
    
    kafkaCommon::coutTree(baseNode, 3);        
}

bool TableTagActionSet::canInsertColumnRight() const
{
    return isInTagContext();
}

void TableTagActionSet::slotInsertColumnRight()
{
    Q_ASSERT(m_currentNode);
    
    QuantaView* view = ViewManager::ref()->activeView();
    NodeModifsSet *modifs = new NodeModifsSet();

    Node* nodeToInsertInBody = buildEmptyTBodyCellSubtree();
    Node* nodeToInsertInHead = buildEmptyTHeadCellSubtree();
    Q_ASSERT(nodeToInsertInBody);
    Q_ASSERT(nodeToInsertInHead);
            
    int const _currentColumnIndex = currentColumnIndex();
    
    // thead
    Node* trChild = firstChildTag(firstChildTag(tableStart(), "thead"), "tr");
    while(trChild)
    {
        Node* thChild = firstChildTag(trChild, "th");
        for(int i = 0; i != _currentColumnIndex + 1; ++i)
            thChild = thChild->SNext();
        
        kafkaCommon::insertNodeSubtree(nodeToInsertInHead, trChild, thChild, modifs);
        
        nodeToInsertInHead = kafkaCommon::duplicateNodeSubtree(nodeToInsertInHead);
        
        trChild = trChild->SNext();
    }    
    
    // tbody
    trChild = firstChildTag(firstChildTag(tableStart(), "tbody"), "tr");
    while(trChild)
    {
        Node* tdChild = firstChildTag(trChild, "td");
        for(int i = 0; i != _currentColumnIndex + 1; ++i)
            tdChild = tdChild->SNext();
        
        kafkaCommon::insertNodeSubtree(nodeToInsertInBody, trChild, tdChild, modifs);
        
        nodeToInsertInBody = kafkaCommon::duplicateNodeSubtree(nodeToInsertInBody);
        
        trChild = trChild->SNext();
    }    
    
    view->document()->docUndoRedo->addNewModifsSet(modifs, undoRedo::NodeTreeModif);
    
    kafkaCommon::coutTree(baseNode, 3);        
}

Node* TableTagActionSet::tableStart() const
{
    Q_ASSERT(isInTagContext());
    Q_ASSERT(m_currentNode);
    
    return parentTag(m_currentNode, "table");
}

int TableTagActionSet::numberOfColumns() const
{
    Node* _tableStart = tableStart();
    if(!_tableStart)
        return -1;
    
    Node* firstTd = firstChildTag(_tableStart, "td");
    
    if(!firstTd)
        return -1;
    
    int count(0);
    Node* aux = firstTd;
    while(aux)
    {
        ++count;
        aux = aux->SNext();
    }
    
    kdDebug(23100) << "Number of columns: " << count << endl;
    return count;
}

int TableTagActionSet::currentColumnIndex() const
{
    Node* nearTd = parentTag(m_currentNode, "td");
    if(!nearTd)
        return -1;
    
    Node* _tableStart = tableStart();
    if(!_tableStart)
        return -1;
    
    Node* firstTd = firstChildTag(_tableStart, "td");    
    if(!firstTd)
        return -1;
    
    int count(0);
    Node* aux = firstTd;
    while(aux && aux != nearTd)
    {
        ++count;
        aux = aux->SNext();
    }
    
    if(!aux)
        count = -1;
    return count;
}

Node* TableTagActionSet::buildEmptyRowSubtree() const
{
    QuantaView* view = ViewManager::ref()->activeView();
    
    Node* nodeToInsert = kafkaCommon::createNode("", "", Tag::XmlTag, view->document());
    nodeToInsert->tag->parse("<tr>", view->document());
    kafkaCommon::createMandatoryNodeSubtree(nodeToInsert, view->document());
    // now we have: <tr><td></td></tr>
    
    //Let's -> <tr><td><br></td></tr>
    Node* brNode = kafkaCommon::createNode("", "", Tag::XmlTag, view->document());
    brNode->tag->parse("<br>", view->document());
    Node* tdNode = nodeToInsert->child;
    kafkaCommon::insertNode(brNode, tdNode, 0, 0);
    
    int _numberOfColumns = numberOfColumns();
    
    if(_numberOfColumns == -1)
        return 0;
    
    for(int i = 1; i != _numberOfColumns; ++i)
    {
        Node* duplicatedTdSubtree = kafkaCommon::duplicateNodeSubtree(tdNode);
        kafkaCommon::insertNodeSubtree(duplicatedTdSubtree, nodeToInsert, 0, 0);
    }

    kafkaCommon::coutTree(nodeToInsert, 3);
        
    return nodeToInsert;
}

Node* TableTagActionSet::buildEmptyTBodyCellSubtree() const
{
    QuantaView* view = ViewManager::ref()->activeView();
    
    // build tree -> <td><br></td>
    Node* nodeToInsert = kafkaCommon::createNode("", "", Tag::XmlTag, view->document());
    nodeToInsert->tag->parse("<td>", view->document());
    Node* brNode = kafkaCommon::createNode("", "", Tag::XmlTag, view->document());
    brNode->tag->parse("<br>", view->document());
    kafkaCommon::insertNode(brNode, nodeToInsert, 0, 0);
    
    return nodeToInsert;
}

Node* TableTagActionSet::buildEmptyTHeadCellSubtree() const
{
    QuantaView* view = ViewManager::ref()->activeView();
    
    // build tree -> <td><br></td>
    Node* nodeToInsert = kafkaCommon::createNode("", "", Tag::XmlTag, view->document());
    nodeToInsert->tag->parse("<th>", view->document());
    Node* brNode = kafkaCommon::createNode("", "", Tag::XmlTag, view->document());
    brNode->tag->parse("<br>", view->document());
    kafkaCommon::insertNode(brNode, nodeToInsert, 0, 0);
    
    return nodeToInsert;
}

//_____________________________________________________________________________

#include "tagactionset.moc"
