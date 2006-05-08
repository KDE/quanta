/***************************************************************************
 *   Copyright (C) 2005 by Andras Mantia   *
 *   amantia@kde.org   *
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
 *   51 Franklin Steet, Fifth Floor, Boston, MA 02110-1301, USA.           *
 ***************************************************************************/

#ifndef KDEVCREATEQUANTAPROJECT_H
#define KDEVCREATEQUANTAPROJECT_H

#include <qpair.h>
#include <qlist.h>

#include <kurl.h>

//kdevelop includes
#include <interfaces/kdevplugin.h>

class KAction;
class KDialogBase;

/**
Please read the README.dox file for more info about this part
*/
class CreateQuantaProjectPart: public KDevPlugin
{
    Q_OBJECT
public:
    CreateQuantaProjectPart(QObject *parent, const QStringList &args);
    ~CreateQuantaProjectPart();
  
private slots:
    void init();
    
    /**
     * Creates a new project using a wizard.
     */
    void slotCreateNewProject();
    /**
     * Delayed adition of adding the files, so we are sure the project object exists after an openProject call.
     */
    void slotDelayedAddFiles();

private:
    void setupActions();
    
    KAction *action;
    QList<QPair<KUrl, KUrl> > m_files;
};

#endif
