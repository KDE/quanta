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

class KURLRequester;
class KListView;
class KListViewItem;
class QListViewItem;
class QLineEdit;
class QComboBox;

class TagAttributeTree;

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


#endif
