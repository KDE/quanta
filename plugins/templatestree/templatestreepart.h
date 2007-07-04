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

#ifndef TEMPLATESTREEPART_H
#define TEMPLATESTREEPART_H

#include <QPointer>

#include <kurl.h>

//kdevelop includes
#include <iplugin.h>

class QMenu;
class KAction;
class KDialog;
namespace KDevelop 
{ 
  class Context; 
}
class ConfigWidgetProxy;
class TemplatesTreeView;
class QuantaCoreIf;

/**
Please read the README.dox file for more info about this part
*/
class TemplatesTreePart: public KDevelop::IPlugin
{
  Q_OBJECT
public:
  TemplatesTreePart(QObject *parent, const QStringList &args);
  ~TemplatesTreePart();

  QuantaCoreIf * quantaCore() {return m_qcore;};
// the methods to embed the view 
  virtual QWidget *pluginView() const;
    
  virtual Qt::DockWidgetArea dockWidgetAreaHint() const 
    {return Qt::RightDockWidgetArea;}
    
private slots:
  
  void init();
  
  void insertConfigWidget(const KDialog *dlg, QWidget *page, unsigned int pageNo);
  void contextMenu(QMenu *popup, const KDevelop::Context *context);
  void projectOpened();
  void projectClosed();
  
  void slotCreateSiteTemplate();

private:
  
  void setupActions();
  
  KAction *m_createTemplateAction;
  KUrl::List m_fileContextList;
  
  TemplatesTreeView *m_widget;
  ConfigWidgetProxy *m_configProxy;
  QuantaCoreIf * m_qcore;
};

#endif

// kate: space-indent on; indent-width 2; mixedindent off;
