/*
 * Crossfire Debugger Support
 *
 * Copyright 2009 Niko Sams <niko.sams@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "crossfireplugin.h"

#include <KPluginFactory>
#include <KDebug>

#include <interfaces/icore.h>
#include <interfaces/idebugcontroller.h>
#include <interfaces/launchconfigurationtype.h>
#include <interfaces/iplugincontroller.h>
#include <interfaces/iruncontroller.h>

#include <iexecutebrowserplugin.h>

#include "debugsession.h"
#include "launchconfig.h"

namespace Crossfire {

K_PLUGIN_FACTORY(KDevCrossfireDebuggerFactory, registerPlugin<Crossfire::CrossfirePlugin>(); )
K_EXPORT_PLUGIN(KDevCrossfireDebuggerFactory(KAboutData("kdevcrossfire", "kdevcrossfire", ki18n("Crossfire Support"), "0.1", ki18n("Support for debugging scripts in Firefox using the Crossfire extension"), KAboutData::License_GPL)
    .addAuthor(ki18n("Niko Sams"), ki18n("Author"), "niko.sams@gmail.com", "http://nikosams.blogspot.com")
))

CrossfirePlugin::CrossfirePlugin( QObject *parent, const QVariantList & ) :
    KDevelop::IPlugin( KDevCrossfireDebuggerFactory::componentData(), parent )
{
    core()->debugController()->initializeUi();

    kDebug();
    {
        IExecuteBrowserPlugin* iface = KDevelop::ICore::self()->pluginController()->pluginForExtension("org.kdevelop.IExecuteBrowserPlugin")->extension<IExecuteBrowserPlugin>();
        Q_ASSERT(iface);
        KDevelop::LaunchConfigurationType* type = core()->runController()->launchConfigurationTypeForId( iface->browserAppConfigTypeId() );
        Q_ASSERT(type);
        type->addLauncher( new BrowserLauncher( this ) );
    }
}

CrossfirePlugin::~CrossfirePlugin()
{
}

DebugSession* CrossfirePlugin::createSession() const
{
    DebugSession *session = new DebugSession();
    KDevelop::ICore::self()->debugController()->addSession(session);
    return session;
}

}

#include "crossfireplugin.moc"
