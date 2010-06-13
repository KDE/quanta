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


#include "launchconfig.h"

#include <klocale.h>
#include <KDebug>
#include <KConfigGroup>

#include <interfaces/ilaunchconfiguration.h>

#include "debugsession.h"
#include "debugjob.h"
#include "crossfireplugin.h"
#include "launchconfigurationpage.h"

namespace Crossfire {

BrowserLauncher::BrowserLauncher( CrossfirePlugin* p ) : m_plugin( p )
{
    m_factoryList << new ConfigPageFactory();
}

QList< KDevelop::LaunchConfigurationPageFactory* > BrowserLauncher::configPages() const
{
    return m_factoryList;
}

QString BrowserLauncher::id()
{
    return "crossfire";
}

QString BrowserLauncher::name() const
{
    return i18n("Crossfire");
}

KJob* BrowserLauncher::start(const QString& launchMode, KDevelop::ILaunchConfiguration* cfg)
{
    Q_ASSERT(cfg);
    if( !cfg ) return 0;

    if( launchMode == "debug" ) {
        return new DebugJob( m_plugin->createSession(), cfg );
    }
    kWarning() << "Unknown launch mode" << launchMode << "for config:" << cfg->name();
    return 0;
}

QStringList BrowserLauncher::supportedModes() const
{
    return QStringList() << "debug";
}

QString BrowserLauncher::description() const
{
    return i18n("Debugs JavaScript in a remote Firefox using Firebug and Crossfire");
}

}
