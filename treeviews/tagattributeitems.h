/***************************************************************************
                             tagattributeitems.h
                             -------------------
    copyright            : (C) 2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

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

class Attribute;
class EditableTree;
class TagAttributeTree;
class Node;
class StyleEditor;

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
    AttributeItem(EditableTree *listView, QListViewItem* parent, const QString& title, const QString& title2);
    AttributeItem(EditableTree *listView, const QString& title, const QString& title2);
    AttributeItem(EditableTree *listView, const QString& title, const QString& title2, QListViewItem* after);

    virtual ~AttributeItem();
    virtual void replaceCurrentEditor();
    virtual void showEditor(int column = 1);
    virtual void hideEditor(int column = 1);
    virtual void placeEditor(QWidget *w, int column = 1);
    virtual QString editorText(int column = 1);

    //I'm not sane... lin is the right editor, lin2 is the left one.
    QLineEdit *lin, *lin2;

protected:

    void paintCell( QPainter *p, const QColorGroup &cg,
                    int column, int width, int align );

    EditableTree *m_listView;
};

class AttributeNameSpaceItem:public AttributeItem
{
public:
    AttributeNameSpaceItem(QListViewItem* parent, const QString& title, const QString& title2): AttributeItem(parent, title, title2){};
    AttributeNameSpaceItem(EditableTree *listView, QListViewItem* parent, const QString& title, const QString& title2):AttributeItem(listView, parent, title, title2){};
    virtual ~AttributeNameSpaceItem(){};

};

class AttributeBoolItem : public AttributeItem
{
public:
    AttributeBoolItem(TagAttributeTree *listView, QListViewItem* parent, const QString& title, const QString& title2);

    virtual ~AttributeBoolItem();
    virtual void showEditor(int column = 1);
    virtual void hideEditor(int column = 1);
    virtual QString editorText(int column = 1);

private:
    QComboBox* combo;
};

class AttributeUrlItem : public AttributeItem
{
public:
    AttributeUrlItem(TagAttributeTree *listView, QListViewItem* parent, const QString& title, const QString& title2);

    virtual ~AttributeUrlItem();
    virtual void showEditor(int column = 1);
    virtual void hideEditor(int column = 1);
    virtual QString editorText(int column = 1);

private:
    KURLRequester* urlRequester;
};

class AttributeListItem : public AttributeItem
{
public:
    AttributeListItem(EditableTree *listView, QListViewItem* parent, const QString& title, const QString& title2, Attribute *attr = 0L);

    virtual ~AttributeListItem();
    virtual void showEditor(int column = 1);
    virtual void hideEditor(int column = 1);
    virtual QString editorText(int column = 1);

private:
    QComboBox* combo;
};

class AttributeColorItem : public AttributeItem
{
public:
    AttributeColorItem(EditableTree *listView, QListViewItem* parent, const QString& title, const QString& title2);

    virtual ~AttributeColorItem();
    virtual void showEditor(int column = 1);
    virtual void hideEditor(int column = 1);
    virtual QString editorText(int column = 1);

private:
    KColorCombo* combo;
};

class AttributeStyleItem : public AttributeItem
{
public:
    AttributeStyleItem(EditableTree *listView, QListViewItem* parent, const QString& title, const QString& title2);

    virtual ~AttributeStyleItem();
    virtual void showEditor(int column = 1);
    virtual void hideEditor(int column = 1);
    virtual QString editorText(int column = 1);

private:

    StyleEditor *m_se;
};


#endif
