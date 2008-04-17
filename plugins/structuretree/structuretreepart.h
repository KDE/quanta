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
#include <QVariant>
//kdevelop includes
#include <iplugin.h>

class QMenu;
class KDialog;
class ConfigWidgetProxy;
class QToolBox;
class QuantaCoreIf;

namespace KDevelop {
  class IProject;
}

/**
Please read the README.dox file for more info about this part
*/
class StructureTreePart: public KDevelop::IPlugin
{
  Q_OBJECT
public:
  StructureTreePart(QObject *parent, const QVariantList &args);
  ~StructureTreePart();
  
private slots:
  void init();
  
  void insertConfigWidget(const KDialog *dlg, QWidget *page, unsigned int pageNo);
  void contextMenu(QMenu *popup, const KDevelop::Context *context);
  void projectOpened(KDevelop::IProject *project);
  void projectClosed(KDevelop::IProject *project);

private:
  void setupActions();
  
  QuantaCoreIf * m_qcore;
  ConfigWidgetProxy * m_configProxy;
};

#endif

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
