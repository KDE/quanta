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

#ifndef KDEVTEMPLATESTREE_H
#define KDEVTEMPLATESTREE_H

#include <QPointer>

#include <kurl.h>

//kdevelop includes
#include <interfaces/kdevplugin.h>

class QMenu;
class KAction;
class KDialogBase;
class Context;
class ConfigWidgetProxy;
class TemplatesTreeView;
class QuantaCoreIf;

/**
Please read the README.dox file for more info about this part
*/
class TemplatesTreePart: public KDevPlugin
{
  Q_OBJECT
public:
  TemplatesTreePart(QObject *parent, const QStringList &args);
  ~TemplatesTreePart();

  QuantaCoreIf * quantaCore() {return m_qcore;};
  
private slots:
  
  void init();
  
  void insertConfigWidget(const KDialogBase *dlg, QWidget *page, unsigned int pageNo);
  void contextMenu(QMenu *popup, const Context *context);
  void projectOpened();
  void projectClosed();
  
  void slotCreateSiteTemplate();

private:
  
  void setupActions();
  
  KAction *m_createTemplateAction;
  KUrl::List m_fileContextList;
  
  QPointer<TemplatesTreeView> m_widget;
  ConfigWidgetProxy *m_configProxy;
  QuantaCoreIf * m_qcore;
};

#endif
