/***************************************************************************
 *   Copyright (C) 2005 by Andras Mantia   *
 *   amantia@kde.org   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KDEVCREATEQUANTAPROJECT_H
#define KDEVCREATEQUANTAPROJECT_H

#include <QPair>
#include <QList>

#include <kurl.h>

//kdevelop includes
#include <iplugin.h>


/**
Please read the README.dox file for more info about this part
*/
class CreateQuantaProjectPart: public KDevelop::IPlugin
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
    
    QAction *action;
    QList<QPair<KUrl, KUrl> > m_files;
};

#endif
