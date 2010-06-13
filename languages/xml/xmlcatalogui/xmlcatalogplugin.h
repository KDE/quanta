/*****************************************************************************
 * Copyright (c) 2009 Ruan Strydom <rm3dom@gmail.com>                        *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *****************************************************************************/

#ifndef XMLCATALOGPLUGIN_H
#define XMLCATALOGPLUGIN_H

#include <QtCore/QVariantList>

#include <interfaces/iplugin.h>

class KAction;

class XmlCatalogPlugin: public KDevelop::IPlugin {
    Q_OBJECT
public:
    XmlCatalogPlugin ( QObject *parent, const QVariantList & = QVariantList() );
    ~XmlCatalogPlugin();
    private slots:
        void slotShowDialog();
    private:
        KAction *m_showDialogAction;
        
};
#endif // XMLCATALOGPLUGIN_H
