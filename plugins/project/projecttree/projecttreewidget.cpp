/***************************************************************************
 *   Copyright (C) 2005 by Jens Herden                                     *
 *   jens@kdewebdev.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "projecttreewidget.h"
#include "projecttreeview.h"
#include "projecttreepart.h"
#include "searchlinewidget.h"

#include <QLayout>

#include <kurl.h>
#include <klibloader.h>
#include <kparts/part.h>
#include <k3listviewsearchline.h>
#include <kmainwindow.h>
#include <kparts/part.h>

//kdevelop includes
#include <kdevcore.h>
#include <kdevdocumentcontroller.h>
#include <kdevprojectcontroller.h>
#include <kdevplugin.h>
#include <kdevmainwindow.h>


ProjectTreeWidget::ProjectTreeWidget(ProjectTreePart *part)
  : QWidget(Koncrete::Core::mainWindow()), m_part(part)
{
  m_tree = new ProjectTreeView(part, this);

  K3ListViewSearchLineWidget * sl = new K3ListViewSearchLineWidget(m_tree, this);
  
  QVBoxLayout *l = new QVBoxLayout(this);
  l->addWidget(sl); 
  
  l->addWidget(m_tree); 
  
  connect(Koncrete::Core::projectController(), SIGNAL(projectOpened()), m_tree, SLOT(slotProjectOpened()));
  connect(part, SIGNAL(saveProject()), m_tree, SLOT(slotProjectClosed()));
  
  connect(Koncrete::Core::documentController(), SIGNAL(activePartChanged(KParts::Part *)), m_tree, SLOT(slotActivePartChanged(KParts::Part *)));

}

ProjectTreeWidget::~ProjectTreeWidget()
{
}

#include "projecttreewidget.moc"

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
