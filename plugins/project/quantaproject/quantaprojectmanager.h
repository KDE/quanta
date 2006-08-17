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

#ifndef QUANTAPROJECTMANAGER_H
#define QUANTAPROJECTMANAGER_H

#include "kdevtreeview.h"

class QuantaProjectPart;
class KDevProjectModel;
class KDevProjectFolderItem;
class KDevProjectFileItem;
class KDevProjectTargetItem;
class KDevProjectItem;
class KUrl;

class QuantaProjectManager: public KDevTreeView
{
  Q_OBJECT
public:
  QuantaProjectManager(QuantaProjectPart *part, QWidget *parent);
  virtual ~QuantaProjectManager();

  QuantaProjectPart *part() const;
  KDevProjectModel *projectModel() const;

  KDevProjectFolderItem *currentFolderItem() const;
  KDevProjectFileItem *currentFileItem() const;
  KDevProjectTargetItem *currentTargetItem() const;

  virtual void reset();

signals:
  void activateURL(const KUrl &url);
  void currentChanged(KDevProjectItem *item);

protected slots:
  void slotActivated(const QModelIndex &index);
  void slotCurrentChanged(const QModelIndex &index);
  void popupContextMenu(const QPoint &pos);

private:
  QuantaProjectPart *m_part;
};

#endif // QUANTAPROJECTMANAGER_H

// kate: space-indent on; indent-width 2; replace-tabs on;
