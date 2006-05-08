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

#ifndef KDEVQUANTAFILESTREE_H
#define KDEVQUANTAFILESTREE_H

#include <qpointer.h>

//kdevelop includes
#include <interfaces/kdevplugin.h>

class QMenu;
class KAction;
class KDialogBase;
class Context;
class ConfigWidgetProxy;
class QWidget;
class FilesTreeView;


/**
Please read the README.dox file for more info about this part
*/
class QuantaFilesTreePart: public KDevPlugin
{
    Q_OBJECT
public:
    QuantaFilesTreePart(QObject *parent, const QStringList &args);
    ~QuantaFilesTreePart();
  
private slots:
    void init();
    
    void insertConfigWidget(const KDialogBase *dlg, QWidget *page, unsigned int pageNo);
    void contextMenu(QMenu *popup, const Context *context);
    void projectOpened();
    void projectClosed();
    
//     void doSomething();

private:
    void setupActions();
    
    KAction *action;
    
    QPointer<QWidget> m_widget;
    ConfigWidgetProxy *m_configProxy;
    FilesTreeView * m_tree;
};

#endif
