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

#include <kapplication.h>
#include <kconfig.h>
#include <kactioncollection.h>
#include <kactionclasses.h>
#include <dom/dom_node.h>
#include <dom/dom_string.h>
#include <klocale.h>

#include <qwidget.h>

#include "tagactionset.h"
#include "tagactionmanager.h"
#include "kafkacommon.h"
#include "cursors.h"
#include "undoredo.h"
#include "wkafkapart.h"
#include "node.h"
#include "quantaview.h"
#include "viewmanager.h"
#include "tag.h"
#include "quantacommon.h"
#include "document.h"
#include "resource.h"


TagActionSetAbstract::TagActionSetAbstract(QObject *parent, const char *name)
        : QObject(parent, name), m_currentNode(0)
{}

TagActionSetAbstract::~TagActionSetAbstract()
{}

Node* TagActionSetAbstract::parentTag(Node* node, QString const& tagName)
{
    Q_ASSERT(node);

    Node* aux = node;
    while(aux && aux->nodeName().lower() != tagName)
        aux = aux->parent;

    return aux;
}

Node* TagActionSetAbstract::firstChildTag(Node* startParentNode, QString const& tagName)
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

bool TagActionSetAbstract::fillWithTagActions(QWidget* /*widget*/, DOM::Node const& node)
{
    m_currentDomNode = node;
    m_currentNode = KafkaDocument::ref()->getNode(m_currentDomNode);

    return m_currentNode;
}

//_____________________________________________________________________________

TagActionSet::TagActionSet(QObject *parent, const char *name)
    : TagActionSetAbstract(parent, name), m_separator(0)
{
    m_separator = new KActionSeparator();
}

bool TagActionSet::isInTagContext() const
{
    return true;
}

void TagActionSet::initActionMenus(QWidget* /*widget*/)
{
}

void TagActionSet::initActions(QWidget* /*parent*/)
{
    KActionCollection* ac(TagActionManager::self()->actionCollection());

    QString actionName = "apply_source_indentation";
    new KAction(i18n("Apply Source Indentation"), 0, this,
                SLOT(slotApplySourceIndentation()),
                ac, actionName);

    actionName = "copy_div_element";
    new KAction(i18n("Copy DIV Area"), 0, this,
                SLOT(slotCopyDivElement()),
                ac, actionName);

    actionName = "cut_div_element";
    new KAction(i18n("Cut DIV Area"), 0, this,
                SLOT(slotCutDivElement()),
                ac, actionName);
}

bool TagActionSet::fillWithTagActions(QWidget* widget, DOM::Node const& node)
{
    bool validNode = TagActionSetAbstract::fillWithTagActions(widget, node);

    if(!validNode || !isInTagContext())
    {
        unplugAllActions(widget);
        return false;
    }

    m_separator->unplugAll();
    
    KActionCollection* ac(TagActionManager::self()->actionCollection());

    KAction* copyDivAction = ac->action("copy_div_element");
    Q_ASSERT(copyDivAction);
    KAction* cutDivAction = ac->action("cut_div_element");
    Q_ASSERT(cutDivAction);
    
    if(/*!KafkaDocument::ref()->getKafkaWidget()->hasSelection() && */isInDivArea())
    {        
        if(!copyDivAction->isPlugged(widget))
            copyDivAction->plug(widget);
        
        if(!cutDivAction->isPlugged(widget))
            cutDivAction->plug(widget);
        
        m_separator->plug(widget);
    }
    else
    {
        copyDivAction->unplug(widget);
        cutDivAction->unplug(widget);
    }
        
//     KAction* applySourceIndentationAction = ac->action("apply_source_indentation");
//     Q_ASSERT(applySourceIndentationAction);
// 
//     applySourceIndentationAction->unplug(widget); // to keep things in order
//     applySourceIndentationAction->plug(widget);
// 
//     m_separator->plug(widget);

    return true;
}

void TagActionSet::unplugAllActions(QWidget* widget) const
{
    KActionCollection* ac(TagActionManager::self()->actionCollection());

    m_separator->unplugAll();
    
    KAction* applySourceIndentationAction = ac->action("apply_source_indentation");
    Q_ASSERT(applySourceIndentationAction);
    applySourceIndentationAction->unplug(widget);

    KAction* copyDivAction = ac->action("copy_div_element");
    Q_ASSERT(copyDivAction);
    copyDivAction->unplug(widget);

    KAction* cutDivAction = ac->action("cut_div_element");
    Q_ASSERT(cutDivAction);
    cutDivAction->unplug(widget);
}

void TagActionSet::slotApplySourceIndentation()
{
    QuantaView* view = ViewManager::ref()->activeView();
    NodeModifsSet *modifs = new NodeModifsSet();

    KConfig* config = kapp->config();
    config->setGroup("Kate Document Defaults");
    int indentationWidth = config->readNumEntry("Indentation Width", 4);
  
  //Once the changes have been made, we will generate the "clean" string for Text Nodes only, and
  //we will add the empty indentation Nodes.
    int eLine, eCol;
    Node* node = baseNode;
    while(node)
    {
        if(/*!node->tag->cleanStrBuilt() && */node->tag->type == Tag::Text)
        {
            if(!node->insideSpecial)
            {
                node->tag->setStr(KafkaDocument::ref()->generateCodeFromNode(node, 0, 0, eLine, eCol, false));
                node->tag->setCleanStrBuilt(true);
            }
        }
        if(/*!node->tag->indentationDone() && */!node->insideSpecial)
        {
            kafkaCommon::fitIndentationNodes(kafkaCommon::getPrevNodeNE(node), node, modifs);
            bool goUp = false;
            kafkaCommon::fitIndentationNodes(node, kafkaCommon::getNextNodeNE(node, goUp), modifs);
            kafkaCommon::applyIndentation(node, indentationWidth, 0, modifs, qConfig.inlineNodeIndentation);
        }
        node = node->nextSibling();
    }

    view->document()->docUndoRedo->addNewModifsSet(modifs, undoRedo::NodeTreeModif);
}

bool TagActionSet::isInDivArea() const
{
    Q_ASSERT(m_currentNode);
    
    return parentTag(m_currentNode, "div");
}

void TagActionSet::slotCopyDivElement()
{
    Q_ASSERT(m_currentNode);
    
    Node* divNode = parentTag(m_currentNode, "div");
    Q_ASSERT(divNode);
    
    Node* divClosingNode = divNode->getClosingNode();
    if(!divClosingNode)
    {
        kdError(25001) << "DIV element without closing node: " << divNode << endl;
        return;
    }
    
    KafkaDocument::ref()->slotCopy(divNode, 0, divClosingNode, 0, QString());
}

void TagActionSet::slotCutDivElement()
{
    Q_ASSERT(m_currentNode);
    
    Node* divNode = parentTag(m_currentNode, "div");
    Q_ASSERT(divNode);
    
    Node* divClosingNode = divNode->getClosingNode();
    if(!divClosingNode)
    {
        kdError(25001) << "DIV element without closing node: " << divNode << endl;
        return;
    }
    
    NodeSelectionInd selection_ind;
    selection_ind.fillWithVPLCursorSelection();

    int cursorOffset = selection_ind.cursorOffset();
    
    KafkaDocument::ref()->slotCut(divNode, 0, divClosingNode, 0, &m_currentNode, cursorOffset, QString());
    
}
        
//_____________________________________________________________________________

TableTagActionSet::TableTagActionSet(QObject *parent, const char *name)
    : TagActionSetAbstract(parent, name), m_separator(0), m_tableActionMenu_0(0), m_insertActionMenu_1(0)
{
    m_separator = new KActionSeparator();
}

bool TableTagActionSet::isInTagContext() const
{
    return parentTag(m_currentNode, "table");
}

void TableTagActionSet::initActionMenus(QWidget* widget)
{
    Q_ASSERT(!m_tableActionMenu_0);

    m_tableActionMenu_0 = new KActionMenu(i18n("Table..."), widget);
    m_insertActionMenu_1 = new KActionMenu(i18n("Insert..."), m_tableActionMenu_0);
    m_removeActionMenu_1 = new KActionMenu(i18n("Remove..."), m_tableActionMenu_0);
}


void TableTagActionSet::initActions(QWidget* parent)
{
    if(!m_tableActionMenu_0)
        initActionMenus(parent);

    KActionCollection* ac(TagActionManager::self()->actionCollection());

    // Insert___________________________________________________________________________

    QString actionName = "insert_table";
    //m_actionNames += actionName;
    new KAction(i18n("Table..."), 0, this,
                SLOT(slotInsertTable()),
                ac, actionName);

    actionName = "insert_row_above";
    //m_actionNames += actionName;
    new KAction(i18n("Row Above"), 0, this,
                SLOT(slotInsertRowAbove()),
                ac, actionName);

    actionName = "insert_row_below";
    //m_actionNames += actionName;
    new KAction(i18n("Row Below"), 0, this,
                SLOT(slotInsertRowBelow()),
                ac, actionName);

    actionName = "insert_column_left";
    //m_actionNames += actionName;
    new KAction(i18n("Column Left"), 0, this,
                SLOT(slotInsertColumnLeft()),
                ac, actionName);

    actionName = "insert_column_right";
    //m_actionNames += actionName;
    new KAction(i18n("Column Right"), 0, this,
                SLOT(slotInsertColumnRight()),
                ac, actionName);

    // Remove___________________________________________________________________________

    actionName = "remove_table";
    //m_actionNames += actionName;
    new KAction(i18n("Table"), 0, this,
                SLOT(slotRemoveTable()),
                ac, actionName);

    actionName = "remove_rows";
    //m_actionNames += actionName;
    new KAction(i18n("Row(s)"), 0, this,
                SLOT(slotRemoveRows()),
                ac, actionName);

    actionName = "remove_columns";
    //m_actionNames += actionName;
    new KAction(i18n("Column(s)"), 0, this,
                SLOT(slotRemoveColumns()),
                ac, actionName);

    actionName = "remove_cells";
    //m_actionNames += actionName;
    new KAction(i18n("Cell(s)"), 0, this,
                SLOT(slotRemoveCells()),
                ac, actionName);

    actionName = "remove_cells_content";
    //m_actionNames += actionName;
    new KAction(i18n("Cell(s) Content"), 0, this,
                SLOT(slotRemoveCellsContent()),
                ac, actionName);

    // Merge___________________________________________________________________________

    actionName = "merge_selected_cells";
    //m_actionNames += actionName;
    new KAction(i18n("Merge Selected Cells"), 0, this,
                SLOT(slotMergeSelectedCells()),
                ac, actionName);
}

bool TableTagActionSet::fillWithTagActions(QWidget* widget, DOM::Node const& node)
{
    bool validNode = TagActionSetAbstract::fillWithTagActions(widget, node);

    if(!validNode || !isInTagContext(/*node*/))
    {
        unplugAllActions(widget);
        return false;
    }

    m_separator->unplugAll();
    
    KActionCollection* ac(TagActionManager::self()->actionCollection());

    // Table
    bool emptyTableActionMenu_0 = true;

    // Insert _____________________________________________________________________

    // Insert
    bool emptyInsertActionMenu_1 = true;

    // Insert Table
    KAction* insertTableAction = ac->action("insert_table");
    Q_ASSERT(insertTableAction);

    m_insertActionMenu_1->remove(insertTableAction);
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
    // Insert Row Below
    KAction* insertRowBelowAction = ac->action("insert_row_below");
    Q_ASSERT(insertRowBelowAction);

    m_insertActionMenu_1->remove(insertRowBelowAction);
    if(canInsertRowBelow())
    {
        emptyTableActionMenu_0 = emptyInsertActionMenu_1 = false;
        m_insertActionMenu_1->insert(insertRowBelowAction);

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
    // Insert Column Right
    KAction* insertColumnRightAction = ac->action("insert_column_right");
    Q_ASSERT(insertColumnRightAction);

    m_insertActionMenu_1->remove(insertColumnRightAction);
    if(canInsertColumnRight())
    {
        emptyTableActionMenu_0 = emptyInsertActionMenu_1 = false;
        m_insertActionMenu_1->insert(insertColumnRightAction);

        m_insertActionMenu_1->insert(m_separator);
    }
    // Remove _____________________________________________________________________

    // Remove
    bool emptyRemoveActionMenu_1 = true;

    // Remove Table
    KAction* removeTableAction = ac->action("remove_table");
    Q_ASSERT(removeTableAction);

    m_removeActionMenu_1->remove(removeTableAction);
    if(canRemoveTable())
    {
        emptyTableActionMenu_0 = emptyRemoveActionMenu_1 = false;
        m_removeActionMenu_1->insert(removeTableAction);

        m_removeActionMenu_1->insert(m_separator);
    }
    // Remove Row(s)
    KAction* removeRowsAction = ac->action("remove_rows");
    Q_ASSERT(removeRowsAction);

    m_removeActionMenu_1->remove(removeRowsAction);
    if(canRemoveRows())
    {
        emptyTableActionMenu_0 = emptyRemoveActionMenu_1 = false;
        m_removeActionMenu_1->insert(removeRowsAction);

        //m_removeActionMenu_1->insert(m_separator);
    }
    // Remove Column(s)
    KAction* removeColumnsAction = ac->action("remove_columns");
    Q_ASSERT(removeColumnsAction);

    m_removeActionMenu_1->remove(removeColumnsAction);
    if(canRemoveColumns())
    {
        emptyTableActionMenu_0 = emptyRemoveActionMenu_1 = false;
        m_removeActionMenu_1->insert(removeColumnsAction);

        //m_removeActionMenu_1->insert(m_separator);
    }
/*    // Remove Cell(s)
    KAction* removeCellsAction = ac->action("remove_cells");
    Q_ASSERT(removeCellsAction);

    m_removeActionMenu_1->remove(removeCellsAction);
    if(canRemoveCells())
    {
        emptyTableActionMenu_0 = emptyRemoveActionMenu_1 = false;
        m_removeActionMenu_1->insert(removeCellsAction);

        //m_removeActionMenu_1->insert(m_separator);
    }*/
    // Remove Cell(s) Content
    KAction* removeCellsContentAction = ac->action("remove_cells_content");
    Q_ASSERT(removeCellsContentAction);

    m_removeActionMenu_1->remove(removeCellsContentAction);
    if(canRemoveCellsContent())
    {
        emptyTableActionMenu_0 = emptyRemoveActionMenu_1 = false;
        m_removeActionMenu_1->insert(removeCellsContentAction);

        //m_removeActionMenu_1->insert(m_separator);
    }
    // Remove _____________________________________________________________________

    // Merge
//     bool emptyRemoveActionMenu_1 = true;

    // _____________________________________________________________________________

    // Table
    m_tableActionMenu_0->unplug(widget);
    if(!emptyTableActionMenu_0)
    {
        m_tableActionMenu_0->plug(widget);

        m_tableActionMenu_0->remove(m_insertActionMenu_1);
        if(!emptyInsertActionMenu_1)
            m_tableActionMenu_0->insert(m_insertActionMenu_1);

        m_tableActionMenu_0->remove(m_removeActionMenu_1);
        if(!emptyRemoveActionMenu_1)
            m_tableActionMenu_0->insert(m_removeActionMenu_1);
            
        m_tableActionMenu_0->insert(m_separator);
    }
    // Merge selected cells
    KAction* mergeSelectedCellsAction = ac->action("merge_selected_cells");
    Q_ASSERT(mergeSelectedCellsAction);

    m_tableActionMenu_0->remove(mergeSelectedCellsAction);
    if(canMergeSelectedCells())
    {
        emptyTableActionMenu_0 = false;
        m_tableActionMenu_0->insert(mergeSelectedCellsAction);

//         m_removeActionMenu_1->insert(m_separator);
    }

    return true;
}

void TableTagActionSet::unplugAllActions(QWidget* widget) const
{
    m_separator->unplugAll();
    m_tableActionMenu_0->unplug(widget);
}

// Insert _____________________________________________________________________

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

//     kafkaCommon::coutTree(baseNode, 3);
}

bool TableTagActionSet::canInsertRowBelow() const
{
    return isInTagContext();
}

void TableTagActionSet::slotInsertRowBelow()
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

//     kafkaCommon::coutTree(baseNode, 3);
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
        for(int i = 0; (i != _currentColumnIndex && thChild); ++i)
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
        for(int i = 0; (i != _currentColumnIndex && tdChild); ++i)
            tdChild = tdChild->SNext();

        kafkaCommon::insertNodeSubtree(nodeToInsertInBody, trChild, tdChild, modifs);

        nodeToInsertInBody = kafkaCommon::duplicateNodeSubtree(nodeToInsertInBody);

        trChild = trChild->SNext();
    }

    view->document()->docUndoRedo->addNewModifsSet(modifs, undoRedo::NodeTreeModif);

//     kafkaCommon::coutTree(baseNode, 3);
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
        for(int i = 0; (i != _currentColumnIndex + 1 && thChild); ++i)
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
        for(int i = 0; (i != _currentColumnIndex + 1 && tdChild); ++i)
            tdChild = tdChild->SNext();

        kafkaCommon::insertNodeSubtree(nodeToInsertInBody, trChild, tdChild, modifs);

        nodeToInsertInBody = kafkaCommon::duplicateNodeSubtree(nodeToInsertInBody);

        trChild = trChild->SNext();
    }

    view->document()->docUndoRedo->addNewModifsSet(modifs, undoRedo::NodeTreeModif);

//     kafkaCommon::coutTree(baseNode, 3);
}

// Remove ________________________________________________________________

bool TableTagActionSet::canRemoveTable() const
{
    return isInTagContext();
}

void TableTagActionSet::slotRemoveTable()
{
    Q_ASSERT(m_currentNode);

    QuantaView* view = ViewManager::ref()->activeView();
    NodeModifsSet *modifs = new NodeModifsSet();

    Node* _tableStart = tableStart();

    kafkaCommon::extractAndDeleteNode(_tableStart, modifs, true, true);

    view->document()->docUndoRedo->addNewModifsSet(modifs, undoRedo::NodeTreeModif);

//     kafkaCommon::coutTree(baseNode, 3);
}

bool TableTagActionSet::canRemoveRows() const
{
    return isInTagContext();
}

void TableTagActionSet::slotRemoveRows()
{
    Q_ASSERT(m_currentNode);

    QuantaView* view = ViewManager::ref()->activeView();
    NodeModifsSet *modifs = new NodeModifsSet();

    NodeSelectionInd selection;
    selection.fillWithVPLCursorSelection();

    if(!selection.hasSelection())
    {
        Node* nearTr = parentTag(m_currentNode, "tr");
        kafkaCommon::extractAndDeleteNode(nearTr, modifs, true, true);
    }
    else
    {
        Node* startSelection = kafkaCommon::getNodeFromLocation(selection.cursorNode());
        Node* endSelection = kafkaCommon::getNodeFromLocation(selection.cursorNodeEndSel());

        Node* startTr = parentTag(startSelection, "tr");
        Node* endTr = parentTag(endSelection, "tr");

        Node* iteratorNode = startTr;
        bool loop(true);
        while(iteratorNode && loop)
        {
            // the check has to be done before extract
            if(iteratorNode == endTr)
                loop = false;

            Node* aux = iteratorNode;
            iteratorNode = iteratorNode->SNext();

            kafkaCommon::extractAndDeleteNode(aux, modifs, true, true);
        }
    }

    view->document()->docUndoRedo->addNewModifsSet(modifs, undoRedo::NodeTreeModif);

//     kafkaCommon::coutTree(baseNode, 3);
}

bool TableTagActionSet::canRemoveColumns() const
{
    return isInTagContext();
}

void TableTagActionSet::slotRemoveColumns()
{
    Q_ASSERT(m_currentNode);

    QuantaView* view = ViewManager::ref()->activeView();
    NodeModifsSet *modifs = new NodeModifsSet();

    NodeSelectionInd selection;
    selection.fillWithVPLCursorSelection();

    if(!selection.hasSelection())
    {
        int const _currentColumnIndex = currentColumnIndex();
        removeColumn(_currentColumnIndex, modifs);
    }
    else
    {
        Node* startSelection = kafkaCommon::getNodeFromLocation(selection.cursorNode());
        Node* endSelection = kafkaCommon::getNodeFromLocation(selection.cursorNodeEndSel());

        int startColumnIndex = columnIndex(startSelection);
        int endColumnIndex = columnIndex(endSelection);
        int numberOfColumnsSelected = endColumnIndex - startColumnIndex + 1;
        
        if(startColumnIndex == -1 || endColumnIndex == -1)
            return;
            
        m_currentNode = parentTag(m_currentNode, "tbody"); // m_currentNode will become invalid
        for(int i = 0; i != numberOfColumnsSelected; ++i)
            removeColumn(startColumnIndex, modifs);
    }

    view->document()->docUndoRedo->addNewModifsSet(modifs, undoRedo::NodeTreeModif);

//     kafkaCommon::coutTree(baseNode, 3);
}

bool TableTagActionSet::canRemoveCells() const
{
    return isInTagContext();
}

void TableTagActionSet::slotRemoveCells()
{}

bool TableTagActionSet::canRemoveCellsContent() const
{
    return isInTagContext();
}

void TableTagActionSet::slotRemoveCellsContent()
{
    Q_ASSERT(m_currentNode);

    QuantaView* view = ViewManager::ref()->activeView();
    NodeModifsSet *modifs = new NodeModifsSet();

    NodeSelectionInd selection;
    selection.fillWithVPLCursorSelection();

    if(!selection.hasSelection())
    {
        Node* aux = m_currentNode;
        m_currentNode = parentTag(m_currentNode, "tbody");
        
        Node* nearTd = parentTag(aux, "td");
        clearCellContents(nearTd, modifs);
    }
    else
    {
        Node* startSelection = kafkaCommon::getNodeFromLocation(selection.cursorNode());
        Node* endSelection = kafkaCommon::getNodeFromLocation(selection.cursorNodeEndSel());

        Node* startTd = parentTag(startSelection, "td");
        Node* endTd = parentTag(endSelection, "td");
        
        if(!startTd || !endTd)
            return;
            
    }

    view->document()->docUndoRedo->addNewModifsSet(modifs, undoRedo::NodeTreeModif);

//     kafkaCommon::coutTree(baseNode, 3);
}

// Merge ________________________________________________________________

bool TableTagActionSet::canMergeSelectedCells() const
{
    if(!KafkaDocument::ref()->getKafkaWidget()->hasSelection())
        return false;
        
    NodeSelectionInd selection;
    selection.fillWithVPLCursorSelection();
    QValueList<int> start = selection.cursorNode();
    QValueList<int> end = selection.cursorNodeEndSel();
    
    return start != end;
}

void TableTagActionSet::slotMergeSelectedCells()
{
    Q_ASSERT(m_currentNode);

    QuantaView* view = ViewManager::ref()->activeView();
    NodeModifsSet *modifs = new NodeModifsSet();

    NodeSelectionInd selection;
    selection.fillWithVPLCursorSelection();
    
    Q_ASSERT(selection.hasSelection());
    
    Node* startSelection = kafkaCommon::getNodeFromLocation(selection.cursorNode());
    Node* endSelection = kafkaCommon::getNodeFromLocation(selection.cursorNodeEndSel());
    
    Node* startTd = parentTag(startSelection, "td");
    Node* endTd = parentTag(endSelection, "td");
    
    Node* nodeIterator = startTd->SNext();
    Node* stopNode = endTd->SNext();
    int count = 1;
    while(nodeIterator && nodeIterator != stopNode)
    {            
        Node* aux = nodeIterator;
        nodeIterator = nodeIterator->SNext();
        
        Node* child = aux->firstChild();
        while(child)
        {
            Node* next = child->next;
            kafkaCommon::moveNode(child, startTd, 0, modifs);
            child = next;
        }
        
        kafkaCommon::extractAndDeleteNode(aux, modifs);
        
        ++count;
    }
    
    if(count == 1)
        return;

    kafkaCommon::editNodeAttribute(startTd, "colspan", QString::number(count), modifs);
    kafkaCommon::editNodeAttribute(startTd, "rowspan", "1", modifs);

    view->document()->docUndoRedo->addNewModifsSet(modifs, undoRedo::NodeTreeModif);

//     kafkaCommon::coutTree(baseNode, 3);
}

//_____________________________________________________________________________

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
    return columnIndex(m_currentNode);
/*    Node* nearTd = parentTag(m_currentNode, "td");
    if(!nearTd)
        return -1;

    Node* _tableStart = tableStart();
    if(!_tableStart)
        return -1;

    Node* firstTd = firstChildTag(parentTag(m_currentNode, "tr"), "td");
    //Node* firstTd = firstChildTag(_tableStart, "td");
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
    return count;*/
}

int TableTagActionSet::columnIndex(Node* node) const
{
    Node* nearTd = parentTag(node, "td");
    if(!nearTd)
        return -1;

    Node* _tableStart = tableStart();
    if(!_tableStart)
        return -1;

    Node* firstTd = firstChildTag(parentTag(node, "tr"), "td");
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

void TableTagActionSet::removeColumn(int _currentColumnIndex, NodeModifsSet* modifs)
{
    Q_ASSERT(m_currentNode);
    Q_ASSERT(_currentColumnIndex >= 0);
    Q_ASSERT(modifs);

    // thead
    Node* trChild = firstChildTag(firstChildTag(tableStart(), "thead"), "tr");
    while(trChild)
    {
        Node* thChild = firstChildTag(trChild, "th");
        for(int i = 0; (i != _currentColumnIndex && thChild); ++i)
            thChild = thChild->SNext();

        kafkaCommon::extractAndDeleteNode(thChild, modifs, true, true);
        
        trChild = trChild->SNext();
    }

    // tbody
    trChild = firstChildTag(firstChildTag(tableStart(), "tbody"), "tr");
    while(trChild)
    {
        Node* tdChild = firstChildTag(trChild, "td");
        for(int i = 0; (i != _currentColumnIndex && tdChild); ++i)
            tdChild = tdChild->SNext();

        kafkaCommon::extractAndDeleteNode(tdChild, modifs, true, true);
        
        trChild = trChild->SNext();
    }
}

void TableTagActionSet::clearCellContents(Node* tdNode, NodeModifsSet* modifs)
{
    if (!tdNode)
      return;
    Node* tdChild = tdNode->child;
    
    if(!tdChild)
        return;
        
    while(tdChild)
    {   
        Node* aux = tdChild;
        tdChild = tdChild->next;
        
        kafkaCommon::extractAndDeleteNode(aux, modifs, true, false);            
    }
}

//_____________________________________________________________________________

#include "tagactionset.moc"
