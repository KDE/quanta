/***************************************************************************
                          quantatreeview.cpp  -  description
                             -------------------
    begin                : Wed Dec 19 2001
    copyright            : (C) 2001 by Andras Mantia
    email                : amantia@freemail.hu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "quantatreeview.h"
#include "projecttreefolder.h"
#include "projecttreefile.h"

//qt includes
#include <qlistview.h>

QuantaTreeView::QuantaTreeView(QWidget *parent, const char *name )	
		: QListView(parent,name)
{
}

QuantaTreeView::~QuantaTreeView()
{
}

/** No descriptions */
void QuantaTreeView::slotReloadTree( QStringList fileList, bool newtree)
{

}
