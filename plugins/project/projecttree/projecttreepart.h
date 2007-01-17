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

#ifndef KDEVPROJECTTREE_H
#define KDEVPROJECTTREE_H

#include <QPointer>

#include <kdevplugin.h>

class QMenu;
class QDomElement;
class KAction;
class KDialog;
namespace Koncrete { class Context; }
class ConfigWidgetProxy;
class ProjectTreeWidget;

/**
Please read the README.dox file for more info about this part
*/
class ProjectTreePart: public Koncrete::Plugin
{
  Q_OBJECT
public:
  ProjectTreePart(QObject *parent, const QStringList &args);
  ~ProjectTreePart();
  
  void savePartialProjectSession(QDomElement *el);
  
  // the methods to embed the view 
  virtual QWidget *pluginView() const;
    
  virtual Qt::DockWidgetArea dockWidgetAreaHint() const 
    {return Qt::RightDockWidgetArea;}

signals:
  /** emitted if the project is about to be closed
   */
  void saveProject();

private slots:
  void init();

  void insertConfigWidget(const KDialog *dlg, QWidget *page, unsigned int pageNo);
  void contextMenu(QMenu *popup, const Koncrete::Context *context);
  void projectOpened();
  void projectClosed();

private:
  void setupActions();

  ProjectTreeWidget *m_widget;
  ConfigWidgetProxy *m_configProxy;
};

#endif

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
