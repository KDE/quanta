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

class Node;

/**
@author Paulo Moura Guedes
*/
class TagActionSet : public QObject
{
    Q_OBJECT
public:
    TagActionSet(QObject *parent = 0, const char *name = 0);
    virtual ~TagActionSet();
    
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
    KActionSeparator* m_separator;

private:
    DOM::Node m_currentDomNode;
};


class TableTagActionSet : public TagActionSet
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
    void slotInsertTable();
    void slotInsertRowAbove();
    void slotInsertRowBellow();
    void slotInsertColumnLeft();
    void slotInsertColumnRight();

protected:
    virtual void initActionMenus(QWidget* widget);

private:
    bool canInsertTable() const;
    bool canInsertRowAbove() const;
    bool canInsertRowBellow() const;
    bool canInsertColumnLeft() const;
    bool canInsertColumnRight() const;

    Node* tableStart() const;
    int numberOfColumns() const;
    int currentColumnIndex() const;
    Node* buildEmptyRowSubtree() const;
    Node* buildEmptyTBodyCellSubtree() const;
    Node* buildEmptyTHeadCellSubtree() const;
    
private:
    KActionMenu* m_tableActionMenu_0;
    KActionMenu* m_insertActionMenu_1;
};

#endif
