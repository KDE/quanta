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

#ifndef QUANTAFILESTREEPART_H
#define QUANTAFILESTREEPART_H

#include <QPointer>

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
class QWidget;
class FilesTreeView;


/**
Please read the README.dox file for more info about this part
*/
class QuantaFilesTreePart: public KDevelop::IPlugin
{
    Q_OBJECT
public:
    QuantaFilesTreePart(QObject *parent, const QStringList &args);
    ~QuantaFilesTreePart();
    // the methods to embed the view 
    virtual QWidget *pluginView() const;
    
    virtual Qt::DockWidgetArea dockWidgetAreaHint() const 
    	{return Qt::LeftDockWidgetArea;}
    
private slots:
    void init();
    
    void insertConfigWidget(const KDialog *dlg, QWidget *page, unsigned int pageNo);
    void contextMenu(QMenu *popup, const KDevelop::Context *context);
    void projectOpened();
    void projectClosed();
    
//     void doSomething();

private:
    void setupActions();
    
    KAction *action;
    
    QWidget *m_widget;
    ConfigWidgetProxy *m_configProxy;
    FilesTreeView * m_tree;
};

#endif
