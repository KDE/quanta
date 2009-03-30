/*
 * XDebug Debugger Support
 *
 * Copyright 1999-2001 John Birch <jbb@kdevelop.org>
 * Copyright 2001 by Bernd Gehrmann <bernd@kdevelop.org>
 * Copyright 2006 Vladimir Prus <ghost@cs.msu.su>
 * Copyright 2007 Hamish Rodda <rodda@kde.org>
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

#include "xdebugplugin.h"

#include <QDir>
#include <QToolTip>
#include <QByteArray>
#include <QTimer>
#include <QMenu>
#include <QDBusConnectionInterface>
#include <QDBusInterface>
#include <QSignalMapper>

#include <kaction.h>
#include <kactioncollection.h>
#include <kdebug.h>
#include <kfiledialog.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kmainwindow.h>
#include <kstatusbar.h>
#include <kparts/part.h>
#include <kmessagebox.h>
#include <kapplication.h>
#include <kpluginfactory.h>
#include <kaboutdata.h>
#include <KToolBar>
#include <KDialog>
#include <kwindowsystem.h>
#include <KXmlGuiWindow>
#include <KXMLGUIFactory>

#include <sublime/view.h>

#include <interfaces/icore.h>
#include <interfaces/iuicontroller.h>
#include <interfaces/idocumentcontroller.h>
#include <interfaces/iprojectcontroller.h>
#include <interfaces/iruncontroller.h>
#include <interfaces/context.h>
#include <interfaces/idebugcontroller.h>
#include <interfaces/iplugincontroller.h>

#include "debugsession.h"
#include "server.h"


K_PLUGIN_FACTORY(KDevXDebugDebuggerFactory, registerPlugin<XDebug::XDebugPlugin>(); )
K_EXPORT_PLUGIN(KDevXDebugDebuggerFactory("kdevxdebug"))

namespace XDebug
{

XDebugPlugin::XDebugPlugin( QObject *parent, const QVariantList & ) :
    KDevelop::IPlugin( KDevXDebugDebuggerFactory::componentData(), parent )
{
    KDEV_USE_EXTENSION_INTERFACE( KDevelop::IRunProvider )

    setXMLFile("kdevxdebugui.rc");

    m_server = new Server(this);
    m_server->listen();
    
    connect(m_server, SIGNAL(sessionStarted(DebugSession*)), SLOT(sessionStarted(DebugSession*)));
    connect(m_server, SIGNAL(outputLine(DebugSession*,QString,KDevelop::IRunProvider::OutputTypes)), SLOT(outputLine(DebugSession*,QString,KDevelop::IRunProvider::OutputTypes)));
    connect(m_server, SIGNAL(stateChanged(DebugSession*,KDevelop::IDebugSession::DebuggerState)), SLOT(debuggerStateChanged(DebugSession*,KDevelop::IDebugSession::DebuggerState)));

    setupActions();
}

void XDebugPlugin::setupActions()
{
    KActionCollection* ac = actionCollection();

    KAction* action = m_startDebugger = new KAction(KIcon("dbgrun"), i18n("&Start with XDebug"), this);
    action->setShortcut(Qt::Key_F9);
    action->setToolTip( i18n("Start in debugger") );
    action->setWhatsThis( i18n("<b>Start in debugger</b><p>"
                               "Starts the debugger with the project's main "
                               "executable. You may set some breakpoints "
                               "before this, or you can interrupt the program "
                               "while it is running, in order to get information "
                               "about variables, frame stack, and so on.") );
    ac->addAction("debug_run", action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(slotStartDebugger()));
}

XDebugPlugin::~XDebugPlugin()
{
}

void XDebugPlugin::sessionStarted(DebugSession* session)
{
    kDebug() << session;
    KDevelop::ICore::self()->debugController()->addSession(session);
}

void XDebugPlugin::outputLine(XDebug::DebugSession* session, QString line, KDevelop::IRunProvider::OutputTypes type)
{
    if (session->process() && m_jobs.contains(session->process())) {
        emit output(m_jobs[session->process()], line, type);    
    }
}


bool XDebugPlugin::execute(const KDevelop::IRun & run, KJob* job)
{
    Q_ASSERT(instrumentorsProvided().contains(run.instrumentor()));

    QString path = run.executable().toLocalFile();
    if (path.endsWith("php")) {
        path = "";
    }
    path += " " + run.arguments().join(" ");
    path = path.trimmed();
    kDebug() << path;

    KProcess* process = m_server->startDebugger(path);
    m_jobs[process] = job;

    return !!job;
}

void XDebugPlugin::abort(KJob* job) {

}


QStringList XDebugPlugin::instrumentorsProvided() const
{
    return QStringList() << "xdebug";
}

QString XDebugPlugin::translatedInstrumentor(const QString&) const
{
    return i18n("XDebug");
}

void XDebugPlugin::slotStartDebugger()
{
    m_startDebugger->setEnabled(false);

    KDevelop::IRun run = KDevelop::ICore::self()->runController()->defaultRun();
    run.setInstrumentor("xdebug");
    KDevelop::ICore::self()->runController()->execute(run);
}

void XDebugPlugin::debuggerStateChanged(DebugSession* session, KDevelop::IDebugSession::DebuggerState state)
{
    switch (state) {
        case KDevelop::IDebugSession::StartingState:
        case KDevelop::IDebugSession::ActiveState:
            break;
        case KDevelop::IDebugSession::PausedState:
            break;
        case KDevelop::IDebugSession::NotStartedState:
        case KDevelop::IDebugSession::StoppingState:
        case KDevelop::IDebugSession::StoppedState:
            m_startDebugger->setEnabled(true);
            if (session->process() && m_jobs.contains(session->process())) {
                emit finished(m_jobs[session->process()]);
            }
            break;
    }
}

}

#include "xdebugplugin.moc"
