/***************************************************************************
                          scripttreeview.cpp  -  description
                             -------------------
    begin                : Thu Sep 16 2003
    copyright            : (C) 2003 by Andras Mantia
    email                : amantia@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
//kde includes
#include <kiconloader.h>
#include <klocale.h>
#include <kurl.h>

//app includes
#include "filestreefolder.h"
#include "scripttreeview.h"
#include "../resource.h"

ScriptTreeView::ScriptTreeView(const KURL& projectBaseURL, QWidget *parent, const char *name )
  : FilesTreeView(parent,name)
{
  m_baseURL = projectBaseURL;

  KURL url;
  url.setPath(qConfig.globalDataDir +"quanta/scripts/");
  FilesTreeFolder *globalDir = new FilesTreeFolder( this , i18n("Scripts"), url);
  globalDir->setPixmap( 0, SmallIcon("folder"));
  globalDir->setOpen( true );

  setRootIsDecorated( true );
  //header()->hide();
  setSorting( 0 );

  setFrameStyle( Panel | Sunken );
  setLineWidth( 2 );
  addColumn( i18n("Scripts"), 600 );

  setFocusPolicy(QWidget::ClickFocus);

}

ScriptTreeView::~ScriptTreeView()
{
}


 #include "scripttreeview.moc"