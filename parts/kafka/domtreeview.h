/***************************************************************************
                               domtreeview.cpp
                             -------------------

    copyright            : (C) 2001 - The Kafka Team
    email                : kde-kafka@master.kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef DOMTREEVIEW_H
#define DOMTREEVIEW_H

#include "kafkacommon.h"

#ifdef HEAVY_DEBUG
#include <klistview.h>
#include <kdebug.h>
#include <qlistview.h>
#include <qptrdict.h>
#include <dom/dom_core.h>
#include <qdialog.h>

class QString;
class QGridLayout;

/**
 * This class is very useful to see the DOM tree in a KListView
 */

class DOMTreeView : public KListView
{
    Q_OBJECT
    public:
	DOMTreeView(QWidget *parent, KHTMLPart *part, const char * name = 0);
	~DOMTreeView();
        void setTitle(const QString &str);
	void recursive(const DOM::Node &pNode, const DOM::Node &node);

    signals:
	void sigNodeClicked(const DOM::Node &);

    public slots:
	void showTree(const DOM::Node &pNode);

    protected slots:
	void slotItemClicked(QListViewItem *);

    private:
	QString title;
	QListViewItem *titleItem;
	QPtrDict<QListViewItem> m_itemdict;
	QPtrDict<DOM::Node> m_nodedict;
	DOM::Node document;
	KHTMLPart *part;

};

class KafkaDOMTreeDialog : public QDialog
{
	Q_OBJECT
public:
	KafkaDOMTreeDialog(QWidget *parent = 0, KHTMLPart *part = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
	~KafkaDOMTreeDialog();
	DOMTreeView *domview;
	QGridLayout *DialogLayout;
};

#endif
#endif
