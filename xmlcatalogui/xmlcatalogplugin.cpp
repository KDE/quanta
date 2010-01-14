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

#include "xmlcatalogplugin.h"
#include "xmlcatalogdialog.h"

#include <KDE/KPluginFactory>
#include <KDE/KPluginLoader>
#include <KDE/KLocalizedString>
#include <KDE/KAboutData>
#include <KDE/KAction>
#include <KDE/KActionCollection>
#include <KParts/MainWindow>

#include <interfaces/contextmenuextension.h>
#include <interfaces/icore.h>
#include <interfaces/iuicontroller.h>

using namespace KDevelop;


K_PLUGIN_FACTORY ( KdevPlugFactory, registerPlugin<XmlCatalogPlugin>(); )
K_EXPORT_PLUGIN ( KdevPlugFactory ( KAboutData ( "kdevxmlcatalogui", "kdevxmlcatalogui", ki18n ( "Xml catalog user interface" ), "0.1", ki18n ( "User interface to manage XML schema and DTD documents" ), KAboutData::License_GPL ) ) )

XmlCatalogPlugin::XmlCatalogPlugin ( QObject* parent, const QVariantList& ) :
        IPlugin ( KdevPlugFactory::componentData(), parent ) {

    setXMLFile ("kdevxmlcatalogui.rc");
    m_showDialogAction= actionCollection()->addAction ( "xmlcatalog_settings" );
    m_showDialogAction->setText ( i18n ( "Configure XML Catalog..." ) );
    m_showDialogAction->setToolTip ( i18n ( "Manage XML schema and DTD documents" ) );
    m_showDialogAction->setWhatsThis ( i18n ( "<b>XML Catalog</b><p> Manage XML schema and DTD documents </p>" ) );
    m_showDialogAction->setEnabled ( true );
    connect ( m_showDialogAction, SIGNAL ( triggered ( bool ) ), this, SLOT ( slotShowDialog() ) );
}

XmlCatalogPlugin::~XmlCatalogPlugin() {

}

void XmlCatalogPlugin::slotShowDialog() {
    static XmlCatalogDialog *w = ( {
        XmlCatalogDialog *w = new XmlCatalogDialog ( ICore::self()->uiController()->activeMainWindow()->widget() );
        w;
    } );
    w->show();
}

