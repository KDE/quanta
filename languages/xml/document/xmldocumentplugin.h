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

#ifndef KDEVXMLSUPPORTPLUGIN_H_
#define KDEVXMLSUPPORTPLUGIN_H_

#include <interfaces/iplugin.h>

#include <QtCore/QVariantList>

#include <interfaces/contextmenuextension.h>

class KAction;
class XMLDocumentFactory;


class XmlDocumentPlugin: public KDevelop::IPlugin {
    Q_OBJECT
public:
    XmlDocumentPlugin ( QObject *parent, const QVariantList & = QVariantList() );
    ~XmlDocumentPlugin();
private:
    XMLDocumentFactory *m_docFactory;
};

#endif //KDEVXMLSUPPORTPLUGIN_H_

