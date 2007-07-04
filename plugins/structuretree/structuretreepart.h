/***************************************************************************
 *   Copyright (C) 2005 by Jens Herden   *
 *   jens@kdewebdev.org   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef STRUCTURETREEPART_H
#define STRUCTURETREEPART_H

#include <QPointer>

//kdevelop includes
#include <iplugin.h>

class QMenu;
class KDialog;
namespace Koncrete { class Context; }
class ConfigWidgetProxy;
class QToolBox;
class QuantaCoreIf;
class StructureTreeWidget;
class GroupsWidget;


/**
Please read the README.dox file for more info about this part
*/
class StructureTreePart: public KDevelop::IPlugin
{
  Q_OBJECT
public:
  StructureTreePart(QObject *parent, const QStringList &args);
  ~StructureTreePart();
  
  // the methods to embed the view 
  virtual QWidget *pluginView() const;
    
  virtual Qt::DockWidgetArea dockWidgetAreaHint() const 
    {return Qt::LeftDockWidgetArea;}
  
private slots:
  void init();
  
  void insertConfigWidget(const KDialog *dlg, QWidget *page, unsigned int pageNo);
  void contextMenu(QMenu *popup, const Koncrete::Context *context);
  void projectOpened();
  void projectClosed();

private:
  void setupActions();
  
  QuantaCoreIf * m_qcore;
  QToolBox *m_widget;
  ConfigWidgetProxy * m_configProxy;
  StructureTreeWidget * m_documentTree;
  GroupsWidget * m_groupsTree;
};

#endif

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
