class KProcess;
/*
 * XDebug Debugger Support
 *
 * Copyright 1999-2001 John Birch <jbb@kdevelop.org>
 * Copyright 2001 by Bernd Gehrmann <bernd@kdevelop.org>
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

#ifndef XDEBUGPLUGIN_H
#define XDEBUGPLUGIN_H

#include <QPointer>
#include <QByteArray>
#include <QLabel>
#include <QtCore/QVariant>

#include <KConfigGroup>
#include <KTextEditor/Cursor>

#include <interfaces/iplugin.h>
#include <interfaces/irunprovider.h>
#include <debugger/interfaces/idebugsession.h>

class KAction;
namespace KDevelop {
    class IDebugController;
}

namespace XDebug
{
class DebugSession;
class Server;
class BreakpointController;


class XDebugPlugin : public KDevelop::IPlugin, public KDevelop::IRunProvider
{
    Q_OBJECT
    Q_INTERFACES(KDevelop::IRunProvider)

public:
    XDebugPlugin( QObject *parent, const QVariantList & = QVariantList() );
    ~XDebugPlugin();

    //BEGIN IRunProvider
    virtual QStringList instrumentorsProvided() const;
    virtual QString translatedInstrumentor(const QString& instrumentor) const;
    virtual bool execute(const KDevelop::IRun& run, KJob* job);
    virtual void abort(KJob* job);

Q_SIGNALS:
    void finished(KJob* job);
    void output(KJob* job, const QString& line, KDevelop::IRunProvider::OutputTypes type);
    //END IRunProvider

private:
    void setupActions();

private Q_SLOTS:
    void slotStartDebugger();
    void sessionStarted(DebugSession* session);
    void outputLine(DebugSession* session, QString line, KDevelop::IRunProvider::OutputTypes type);
    void debuggerStateChanged(DebugSession*, KDevelop::IDebugSession::DebuggerState state);


Q_SIGNALS:
    void startDebugger(const KDevelop::IRun & run, KJob* job);

private:
    Server* m_server;
    QMap<KProcess*, KJob*> m_jobs;
    BreakpointController* m_breakpointController;
    KDevelop::IDebugController* m_debugController;
    KAction* m_startDebugger;
    
    KJob* m_currentJob;
};

}

#endif
