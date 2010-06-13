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

#ifndef KDEVXMLFORMATTERPLUGIN_H_
#define KDEVXMLFORMATTERPLUGIN_H_

#include <QObject>

#include <KDE/KAction>
#include <KDE/KUrl>

#include <interfaces/iplugin.h>
#include <interfaces/contextmenuextension.h>

class KJob;
class XmlFormatter;
class XmlFormatterPreferences;

class XmlValidatorPlugin : public KDevelop::IPlugin {
    Q_OBJECT

public:
    XmlValidatorPlugin ( QObject *parent, const QVariantList & = QVariantList() );
    ~XmlValidatorPlugin();
    virtual KDevelop::ContextMenuExtension contextMenuExtension ( KDevelop::Context* context );
public slots:
    void showDialog();
protected slots:
    void slotValidate();
    void slotValidated(KJob *);
protected:
    KAction *m_validateAction;
};

#endif //KDEVXMLFORMATTERPLUGIN_H_
