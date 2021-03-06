/* This file is part of Quanta
    Copyright (C) 2005 Roberto Raggi <roberto@kdevelop.org>
              (C) 2006 Jens Herden   <jens@kdewebdev.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "quantaprojectmanager.h"
#include "quantaprojectpart.h"
#include "kdevprojectmodel.h"

#include <QtGui/QHeaderView>

#include <kdevcore.h>
#include <kdevmainwindow.h>
#include <kdevcontext.h>
#include <kmenu.h>
#include <kdebug.h>
#include <kurl.h>
#include <klocale.h>


QuantaProjectManager::QuantaProjectManager(QuantaProjectPart *part, QWidget *parent)
  : Koncrete::TreeView(parent),
    m_part(part)
{
  header()->hide();

  setContextMenuPolicy(Qt::CustomContextMenu);

  connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(popupContextMenu(QPoint)));
  connect(this, SIGNAL(activated(QModelIndex)), this, SLOT(slotActivated(QModelIndex)));
}

QuantaProjectManager::~QuantaProjectManager()
{
}

QuantaProjectPart *QuantaProjectManager::part() const
{
  return m_part;
}

void QuantaProjectManager::reset()
{
  Koncrete::TreeView::reset();
}

Koncrete::ProjectFolderItem *QuantaProjectManager::currentFolderItem() const
{
  Q_ASSERT(projectModel() != 0);

  QItemSelectionModel *selection = selectionModel();
  QModelIndex current = selection->currentIndex();

  while (current.isValid())
    {
      if (Koncrete::ProjectFolderItem *folderItem = dynamic_cast<Koncrete::ProjectFolderItem*>(projectModel()->item(current)))
        return folderItem;

      current = projectModel()->parent(current);
    }

  return 0;
}

Koncrete::ProjectFileItem *QuantaProjectManager::currentFileItem() const
{
  Q_ASSERT(projectModel() != 0);

  QItemSelectionModel *selection = selectionModel();
  QModelIndex current = selection->currentIndex();

  while (current.isValid())
    {
      if (Koncrete::ProjectFileItem *fileItem = dynamic_cast<Koncrete::ProjectFileItem*>(projectModel()->item(current)))
        return fileItem;

      current = projectModel()->parent(current);
    }

  return 0;
}

Koncrete::ProjectTargetItem *QuantaProjectManager::currentTargetItem() const
{
  Q_ASSERT(projectModel() != 0);

  QItemSelectionModel *selection = selectionModel();
  QModelIndex current = selection->currentIndex();

  while (current.isValid())
    {
      if (Koncrete::ProjectTargetItem *targetItem = dynamic_cast<Koncrete::ProjectTargetItem*>(projectModel()->item(current)))
        return targetItem;

      current = projectModel()->parent(current);
    }

  return 0;
}

Koncrete::ProjectModel *QuantaProjectManager::projectModel() const
{
  return qobject_cast<Koncrete::ProjectModel*>(model());
}

void QuantaProjectManager::slotActivated(const QModelIndex &index)
{
  Koncrete::ProjectItem *item = projectModel()->item(index);

  if (item && item->file())
    {
      emit activateURL(item->file()->url());
    }
}

void QuantaProjectManager::popupContextMenu(const QPoint &pos)
{
  QModelIndex index = indexAt(pos);

  if (Koncrete::ProjectItem *item = projectModel()->item(index))
    {
      KMenu menu(this);

      if (Koncrete::ProjectFolderItem *folder = item->folder())
        {
          menu.addTitle(i18n("Folder: %1", folder->url().directory()));
        }
      else if (Koncrete::ProjectFileItem *file = item->file())
        {
          menu.addTitle(i18n("File: %1", file->url().fileName()));
        }
      else if (Koncrete::ProjectTargetItem *target = item->target())
        {
          menu.addTitle(i18n("Target: %1", target->text()));
        }

      Koncrete::ProjectItemContext context(item);
      Koncrete::Core::mainWindow()->fillContextMenu(&menu, &context);

      menu.exec(mapToGlobal(pos));
    }
}

void QuantaProjectManager::slotCurrentChanged(const QModelIndex &index)
{
  if (Koncrete::ProjectItem *item = projectModel()->item(index))
    {
      emit currentChanged(item);
    }
}

#include "quantaprojectmanager.moc"

// kate: space-indent on; indent-width 2; replace-tabs on;
