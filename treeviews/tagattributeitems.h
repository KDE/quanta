//
//
// C++ Interface: h
//
// Description:
//
//
// Author: Andras Mantia <amantia@freemail.hu>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TAGATTRIBUTEITEM_H
#define TAGATTRIBUTEITEM_H

#include <qptrlist.h>

class KColorCombo;
class KURLRequester;
class KListView;
class KListViewItem;
class QListViewItem;
class QLineEdit;
class QComboBox;

class TagAttributeTree;
class Node;

class TopLevelItem : public KListViewItem
{

public:
    TopLevelItem(KListView *parent, const QString& title);
    TopLevelItem(KListView *parent, QListViewItem* after, const QString& title);

    virtual ~TopLevelItem();

protected:

    void paintCell( QPainter *p, const QColorGroup &cg,
                    int column, int width, int align );
};

class ParentItem : public KListViewItem
{

public:
    ParentItem(TagAttributeTree *listView, QListViewItem *parent);

    virtual ~ParentItem();
    Node *node(int index);
    void addNode(Node *node);
    void showList(bool show);

protected:

    void paintCell( QPainter *p, const QColorGroup &cg,
                    int column, int width, int align );

private:
    QPtrList<Node> m_nodeList;
    QComboBox *comboBox;
    TagAttributeTree *m_listView;
};

class AttributeItem : public KListViewItem
{
public:
    AttributeItem(QListViewItem* parent, const QString& title, const QString& title2);
    AttributeItem(TagAttributeTree *listView, QListViewItem* parent, const QString& title, const QString& title2);

    virtual ~AttributeItem();
    virtual void showEditor();
    virtual void hideEditor();
    virtual void placeEditor(QWidget *w);
    virtual QString editorText();

protected:

    void paintCell( QPainter *p, const QColorGroup &cg,
                    int column, int width, int align );

    TagAttributeTree *m_listView;

private:
    QLineEdit* lin;
};

class AttributeNameSpaceItem:public AttributeItem
{
public:
    AttributeNameSpaceItem(QListViewItem* parent, const QString& title, const QString& title2): AttributeItem(parent, title, title2){};
    AttributeNameSpaceItem(TagAttributeTree *listView, QListViewItem* parent, const QString& title, const QString& title2):AttributeItem(listView, parent, title, title2){};
    virtual ~AttributeNameSpaceItem(){};

};

class AttributeBoolItem : public AttributeItem
{
public:
    AttributeBoolItem(TagAttributeTree *listView, QListViewItem* parent, const QString& title, const QString& title2);

    virtual ~AttributeBoolItem();
    virtual void showEditor();
    virtual void hideEditor();
    virtual QString editorText();

private:
    QComboBox* combo;
};

class AttributeUrlItem : public AttributeItem
{
public:
    AttributeUrlItem(TagAttributeTree *listView, QListViewItem* parent, const QString& title, const QString& title2);

    virtual ~AttributeUrlItem();
    virtual void showEditor();
    virtual void hideEditor();
    virtual QString editorText();

private:
    KURLRequester* urlRequester;
};

class AttributeListItem : public AttributeItem
{
public:
    AttributeListItem(TagAttributeTree *listView, QListViewItem* parent, const QString& title, const QString& title2);

    virtual ~AttributeListItem();
    virtual void showEditor();
    virtual void hideEditor();
    virtual QString editorText();

private:
    QComboBox* combo;
};

class AttributeColorItem : public AttributeItem
{
public:
    AttributeColorItem(TagAttributeTree *listView, QListViewItem* parent, const QString& title, const QString& title2);

    virtual ~AttributeColorItem();
    virtual void showEditor();
    virtual void hideEditor();
    virtual QString editorText();

private:
    KColorCombo* combo;
};


#endif
