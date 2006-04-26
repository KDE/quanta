/***************************************************************************
 *   Copyright (C) 2005 by Jens Herden   *
 *   jens@kdewebdev.org   *
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

#ifndef KDEVSTRUCTURETREE_H
#define KDEVSTRUCTURETREE_H

#include <kdevplugin.h>

#include <qpointer.h>

class QMenu;
class KAction;
class KDialogBase;
class Context;
class ConfigWidgetProxy;
class QToolBox;
class QuantaCoreIf;
class StructureTreeWidget;
class GroupsWidget;


/**
Please read the README.dox file for more info about this part
*/
class StructureTreePart: public KDevPlugin
{
  Q_OBJECT
public:
  StructureTreePart(QObject *parent, const char *name, const QStringList &args);
  ~StructureTreePart();

private slots:
  void init();
  
  void insertConfigWidget(const KDialogBase *dlg, QWidget *page, unsigned int pageNo);
  void contextMenu(QMenu *popup, const Context *context);
  void projectOpened();
  void projectClosed();

private:
  void setupActions();
  
  QuantaCoreIf * m_qcore;
  QPointer<QToolBox> m_widget;
  ConfigWidgetProxy * m_configProxy;
  StructureTreeWidget * m_documentTree;
  GroupsWidget * m_groupsTree;
};

#endif

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
