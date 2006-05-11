/***************************************************************************
 *   Copyright (C) 2005 by Jens Herden                                     *
 *   jens@kdewebdev.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
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
#include <interfaces/kdevcore.h>
#include <interfaces/kdevdocumentcontroller.h>
#include <interfaces/kdevplugin.h>
#include <interfaces/kdevmainwindow.h>


ProjectTreeWidget::ProjectTreeWidget(ProjectTreePart *part)
    : QWidget(part->mainWindow()->main(), "projecttree widget"), m_part(part)
{
  m_tree = new ProjectTreeView(part, this);

  K3ListViewSearchLineWidget * sl = new K3ListViewSearchLineWidget(m_tree, this);
  
  QVBoxLayout *l = new QVBoxLayout(this);
  l->addWidget(sl); 
  
  l->addWidget(m_tree); 
  
  connect(part->core(), SIGNAL(projectOpened()), m_tree, SLOT(slotProjectOpened()));
  connect(part, SIGNAL(saveProject()), m_tree, SLOT(slotProjectClosed()));
  
  connect(part->documentController(), SIGNAL(activePartChanged(KParts::Part *)), m_tree, SLOT(slotActivePartChanged(KParts::Part *)));

}

ProjectTreeWidget::~ProjectTreeWidget()
{
}

#include "projecttreewidget.moc"

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
