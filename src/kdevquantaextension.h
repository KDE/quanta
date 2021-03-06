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
#ifndef KDEVQUANTAEXTENSION_H
#define KDEVQUANTAEXTENSION_H

#include <shell/shellextension.h>

#include <QStringList>

class KDevQuantaExtension : public KDevelop::ShellExtension {
public:
    static void init();

    virtual QString xmlFile();

    virtual QString binaryPath();

    virtual KDevelop::AreaParams defaultArea();

    virtual QString projectFileExtension();

    virtual QString projectFileDescription();
    
    virtual QStringList defaultPlugins() { return QStringList(); }
    
protected:
    KDevQuantaExtension();
};

#endif
