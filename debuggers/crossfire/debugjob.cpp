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

#include "debugjob.h"

#include <KDebug>
#include <KProcess>

#include <interfaces/icore.h>
#include <interfaces/iplugincontroller.h>
#include <outputview/outputmodel.h>

#include <iexecutebrowserplugin.h>

#include "debugsession.h"

namespace Crossfire {


DebugJob::DebugJob(DebugSession* session, KDevelop::ILaunchConfiguration* cfg, QObject* parent)
    : OutputJob(parent), m_session(session), m_browserPid(0)
{
    setCapabilities(Killable);

    session->setLaunchConfiguration(cfg);

    IExecuteBrowserPlugin* iface = KDevelop::ICore::self()->pluginController()
        ->pluginForExtension("org.kdevelop.IExecuteBrowserPlugin")->extension<IExecuteBrowserPlugin>();
    Q_ASSERT(iface);

    QString err;
    m_url = iface->url(cfg, err);
    if (!err.isEmpty()) {
        m_url.clear();
        setError( -1 );
        setErrorText( err );
        return;
    }
    setStandardToolView(KDevelop::IOutputView::RunView);
    setBehaviours(KDevelop::IOutputView::AllowUserClose | KDevelop::IOutputView::AutoScroll);
    setModel( new KDevelop::OutputModel() );

    connect(m_session, SIGNAL(finished()), SLOT(sessionFinished()));
    connect(m_session, SIGNAL(outputLine(QString)), model(), SLOT(appendLine(QString)));
    connect(m_session, SIGNAL(outputLine(QString)), SLOT(outputLine(QString)));

    m_session->setStartUrl(m_url);
}


void DebugJob::outputLine(QString line)
{
    kDebug() << line;
}


void DebugJob::start()
{
    kDebug() << "launching?" << m_url;
    if (!m_url.isEmpty()) {
        if (!m_session->listenForConnection()) {
            kWarning() << "listening for connection failed";
            emitResult();
            return;
        }
        QStringList p;
        p << "/opt/firefox/firefox-bin"; //TODO this should obviously not stay
        //p << "firefox";
        p << "--crossfire-host=localhost";
        p << "--crossfire-port=5000";
        p << m_url.url();
        startOutput();
        model()->appendLine( i18n("Opening: %1", m_url.url() ) );
        kDebug() << p;
        m_browserPid = KProcess::startDetached(p);
        if (!m_browserPid) {
            kWarning() << "openUrl failed, something went wrong when creating the job";
            emitResult();
        }
    }
}

KDevelop::OutputModel* DebugJob::model()
{
    return dynamic_cast<KDevelop::OutputModel*>(KDevelop::OutputJob::model());
}

bool DebugJob::doKill()
{
    kDebug();
    m_session->stopDebugger();
    return true;
}

void DebugJob::sessionFinished()
{
    emitResult();
}

}
#include "debugjob.moc"
