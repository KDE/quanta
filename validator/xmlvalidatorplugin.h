/*
Copyright (C) 2009  Ruan Strydom <ruan@jcell.co.za>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KDEVXMLFORMATTERPLUGIN_H_
#define KDEVXMLFORMATTERPLUGIN_H_

#include <QObject>

#include <interfaces/iplugin.h>
#include <interfaces/contextmenuextension.h>
#include <KDE/KAction>
#include <KDE/KUrl>

class XmlFormatter;
class XmlFormatterPreferences;

class XmlValidatorPlugin : public KDevelop::IPlugin {
    Q_OBJECT

public:
    XmlValidatorPlugin ( QObject *parent, const QVariantList & = QVariantList() );
    ~XmlValidatorPlugin();
    virtual KDevelop::ContextMenuExtension contextMenuExtension ( KDevelop::Context* context );

protected slots:
    void slotValidateXml();
    void slotValidate();
protected:
    KAction *m_validateAction;
    /** Returns a local file for the schema.
     * If an local file does not exist, it will be downloaded and saved in
     * tmp
     */
    QString getLocalURLForSchema ( const KUrl &file, const KUrl &schemaUrl );
};

#endif //KDEVXMLFORMATTERPLUGIN_H_
