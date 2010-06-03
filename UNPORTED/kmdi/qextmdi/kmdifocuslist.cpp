/* This file is part of the KDE project
   Copyright (C) 2003 Joseph Wenninger <jowenn@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include <qobjectlist.h>
#ifndef NO_KDE
#include <kdebug.h>
#endif
#include "kmdifocuslist.h"

KMdiFocusList::KMdiFocusList(QObject *parent):QObject(parent) {
}
	
KMdiFocusList::~KMdiFocusList() {
}

void KMdiFocusList::addWidgetTree(QWidget* w) {
	//this method should never be called twice on the same hierarchy
	m_list.insert(w,w->focusPolicy());
	w->setFocusPolicy(QWidget::ClickFocus);
//	kdDebug()<<"KMdiFocusList::addWidgetTree: adding toplevel"<<endl;
	connect(w,SIGNAL(destroyed(QObject *)),this,SLOT(objectHasBeenDestroyed(QObject*)));
	QObjectList *l=w->queryList("QWidget");
	QObjectListIt it( *l );
	QObject *obj;
        while ( (obj = it.current()) != 0 ) {
		QWidget *wid=(QWidget*)obj;
		m_list.insert(wid,wid->focusPolicy());	
		wid->setFocusPolicy(QWidget::ClickFocus);
//		kdDebug()<<"KMdiFocusList::addWidgetTree: adding widget"<<endl;
		connect(wid,SIGNAL(destroyed(QObject *)),this,SLOT(objectHasBeenDestroyed(QObject*)));
        	++it;
    	}
    	delete l;
}
	
void KMdiFocusList::restore() {
	for (QMap<QWidget*,QWidget::FocusPolicy>::iterator it=m_list.begin();it!=m_list.end();++it) {
		it.key()->setFocusPolicy(it.data());
	}
	m_list.clear();
}


void KMdiFocusList::objectHasBeenDestroyed(QObject* o) {
    if (!o || !o->isWidgetType()) return;
	QWidget *w=(QWidget*)o;
	m_list.remove(w);
}

#ifndef NO_INCLUDE_MOCFILES
#include "kmdifocuslist.moc"
#endif
