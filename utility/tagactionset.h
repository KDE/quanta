/***************************************************************************
                               tagactionset.h
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

#ifndef TAGACTIONSET_H
#define TAGACTIONSET_H

namespace DOM
{
class Node;
}
class KActionSeparator;

#include <qobject.h>
class QWidget;
class KActionSet;
class KActionMenu;

class Node;
class NodeModifsSet;

/**
@author Paulo Moura Guedes
*/
class TagActionSetAbstract : public QObject
{
    Q_OBJECT
public:
    TagActionSetAbstract(QObject *parent = 0, const char *name = 0);
    virtual ~TagActionSetAbstract();
    
    DOM::Node const& currentDomNode() const {return m_currentDomNode;}

    static Node* parentTag(Node* node, QString const& tagName);
    static Node* firstChildTag(Node* node, QString const& tagName);
    
    virtual bool isInTagContext(/*DOM::Node const& node*/) const = 0;
    virtual void initActions(QWidget* parent) = 0;
    virtual bool fillWithTagActions(QWidget* widget, DOM::Node const& node);
    virtual void unplugAllActions(QWidget* widget) const = 0;

protected:
    virtual void initActionMenus(QWidget* widget) = 0;
    
protected:
    Node* m_currentNode;

private:
    DOM::Node m_currentDomNode;
};


class TagActionSet : public TagActionSetAbstract
{
    Q_OBJECT
public:
    TagActionSet(QObject *parent = 0, const char *name = 0);
    virtual ~TagActionSet() {}
    
    virtual bool isInTagContext() const;
    virtual void initActions(QWidget* parent);
    virtual bool fillWithTagActions(QWidget* widget, DOM::Node const& node);
    virtual void unplugAllActions(QWidget* widget) const;
    
public slots:
    void slotApplySourceIndentation();
    void slotCopyDivElement();
    void slotCutDivElement();

protected:
    virtual void initActionMenus(QWidget* widget);

private:
    bool isInDivArea() const;
   
private:
    KActionSeparator* m_separator;
};


class TableTagActionSet : public TagActionSetAbstract
{
    Q_OBJECT
public:
    TableTagActionSet(QObject *parent = 0, const char *name = 0);
    virtual ~TableTagActionSet()
    {}

    virtual bool isInTagContext(/*DOM::Node const& node*/) const;
    virtual void initActions(QWidget* parent);
    virtual bool fillWithTagActions(QWidget* widget, DOM::Node const& node);
    virtual void unplugAllActions(QWidget* widget) const;

public slots:
	// Insert
    void slotInsertTable();
    void slotInsertRowAbove();
    void slotInsertRowBelow();
    void slotInsertColumnLeft();
    void slotInsertColumnRight();
    // Remove
	void slotRemoveTable();
	void slotRemoveRows();
	void slotRemoveColumns();
	void slotRemoveCells();
	void slotRemoveCellsContent();
    // Merge
    void slotMergeSelectedCells();
    
protected:
    virtual void initActionMenus(QWidget* widget);

private:
	// Insert
    bool canInsertTable() const;
    bool canInsertRowAbove() const;
    bool canInsertRowBelow() const;
    bool canInsertColumnLeft() const;
    bool canInsertColumnRight() const;
    // Remove
    bool canRemoveTable() const;
	bool canRemoveRows() const;
	bool canRemoveColumns() const;
	bool canRemoveCells() const;
	bool canRemoveCellsContent() const;
    // Merge
    bool canMergeSelectedCells() const;

    Node* tableStart() const;
    int numberOfColumns() const;
    int currentColumnIndex() const;
    int columnIndex(Node* node) const;
    Node* buildEmptyRowSubtree() const;
    Node* buildEmptyTBodyCellSubtree() const;
    Node* buildEmptyTHeadCellSubtree() const;
    
    void removeColumn(int index, NodeModifsSet* modifs);
    void clearCellContents(Node* tdNode, NodeModifsSet* modifs);
    
private:
    KActionSeparator* m_separator;
        
    KActionMenu* m_tableActionMenu_0;
    KActionMenu* m_insertActionMenu_1;
    KActionMenu* m_removeActionMenu_1;
};

#endif
