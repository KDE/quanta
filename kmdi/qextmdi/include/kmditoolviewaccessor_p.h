//----------------------------------------------------------------------------
//    filename             : kmditoolviewaccessor_p.h
//----------------------------------------------------------------------------
//    Project              : KDE MDI extension
//
//    begin                : 08/2003       by Joseph Wenninger (jowenn@kde.org)
//    changes              : ---
//    patches              : ---
//
//    copyright            : (C) 2003 by Joseph Wenninger (jowenn@kde.org)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU Library General Public License as
//    published by the Free Software Foundation; either version 2 of the
//    License, or (at your option) any later version.
//
//----------------------------------------------------------------------------


#ifndef KMDI_TOOLVIEW_ACCESSOR_PRIVATE_H_
#define KMDI_TOOLVIEW_ACCESSOR_PRIVATE_H_


#include <qwidget.h>
#include <kmdidockwidget.h>

class KMdiToolViewAccessorPrivate {
public:
	KMdiToolViewAccessorPrivate() {
		widgetContainer=0;
		widget=0;
	}
	~KMdiToolViewAccessorPrivate() {
	}
	KDockWidget *widgetContainer;
	QWidget *widget;
};


#endif


