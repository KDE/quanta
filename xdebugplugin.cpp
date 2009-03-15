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

#include "debuggercontroller.h"
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
#include <interfaces/iproject.h>
#include <interfaces/context.h>
#include <util/processlinemaker.h>
#include <interfaces/context.h>
#include <interfaces/contextmenuextension.h>
#include <language/interfaces/editorcontext.h>


K_PLUGIN_FACTORY(KDevXDebugDebuggerFactory, registerPlugin<XDebug::XDebugPlugin>(); )
K_EXPORT_PLUGIN(KDevXDebugDebuggerFactory("kdevxdebug"))

namespace XDebug
{

XDebugPlugin::XDebugPlugin( QObject *parent, const QVariantList & ) :
    KDevelop::IPlugin( KDevXDebugDebuggerFactory::componentData(), parent ), m_currentJob(0)
{
    KDEV_USE_EXTENSION_INTERFACE( KDevelop::IRunProvider )

    setXMLFile("kdevxdebugui.rc");

    m_controller = new DebuggerController(this);
    connect(m_controller->connection(), SIGNAL(stateChanged(DebuggerState)), this, SLOT(debuggerStateChanged(DebuggerState)));
    connect(m_controller->connection(), SIGNAL(outputLine(QString,KDevelop::IRunProvider::OutputTypes)), this, SLOT(outputLine(QString,KDevelop::IRunProvider::OutputTypes)));

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

    m_stepInto = action = new KAction(KIcon("dbgstep"), i18n("Step &Into (XDebug)"), this);
    action->setShortcut(Qt::Key_F11);
    action->setToolTip( i18n("Step into the next statement") );
    action->setWhatsThis( i18n("<b>Step into</b><p>"
                               "Executes exactly one line of source. If the source line "
                               "is a call to a function then execution will stop after "
                               "the function has been entered.") );
    action->setEnabled(false);
    connect(action, SIGNAL(triggered(bool)), m_controller, SLOT(stepInto()));
    ac->addAction("debug_stepinto", action);

}

XDebugPlugin::~XDebugPlugin()
{
}

bool XDebugPlugin::execute(const KDevelop::IRun & run, KJob* job)
{
    Q_ASSERT(instrumentorsProvided().contains(run.instrumentor()));
    
    m_currentJob = job;
    
    QString path = run.executable().toLocalFile();
    if (path.endsWith("php")) {
        path = "";
    }
    path += " " + run.arguments().join(" ");
    path = path.trimmed();
    kDebug() << path;
    return m_controller->startDebugging(path);
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
    m_stepInto->setEnabled(true);

    KDevelop::IRun run = KDevelop::ICore::self()->runController()->defaultRun();
    run.setInstrumentor("xdebug");
    KDevelop::ICore::self()->runController()->execute(run);
}

void XDebugPlugin::debuggerStateChanged(XDebug::DebuggerState state)
{
    if (state == StoppedState) {
        m_startDebugger->setEnabled(true);
        m_stepInto->setEnabled(false);
        emit finished(m_currentJob);
    }
}
void XDebugPlugin::outputLine(const QString& line, KDevelop::IRunProvider::OutputTypes type)
{
    emit output(m_currentJob, line, type);
}




}

#include "xdebugplugin.moc"
