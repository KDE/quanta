/*
 * XDebug Debugger Support
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

#include "connectiontest.h"

#include <iostream>
#include <QtTest/QTest>
#include <QtTest/QSignalSpy>
#include <QtCore/QTemporaryFile>
#include <QtCore/QDir>

#include <KDebug>
#include <KProcess>
#include <shell/testcore.h>
#include <shell/shellextension.h>
#include <debugger/interfaces/stackmodel.h>

#include "connection.h"
#include "server.h"
#include "debugsession.h"

using namespace XDebug;
namespace KParts {
    class MainWindow;
}
namespace Sublime {
    class Controller;
}
namespace KDevelop {
    class IToolViewFactory;
}

class AutoTestShell : public KDevelop::ShellExtension
{
public:
    QString xmlFile() { return QString(); }
    QString defaultProfile() { return "kdevtest"; }
    KDevelop::AreaParams defaultArea() {
        KDevelop::AreaParams params;
        params.name = "test";
        params.title = "Test";
        return params;
    }
    QString projectFileExtension() { return QString(); }
    QString projectFileDescription() { return QString(); }
    QStringList defaultPlugins() { return QStringList(); }

    static void init() { s_instance = new AutoTestShell; }
};

void ConnectionTest::init()
{
    qRegisterMetaType<KDevelop::IRunProvider::OutputTypes>("KDevelop::IRunProvider::OutputTypes");
    qRegisterMetaType<DebugSession*>("DebugSession*");
    qRegisterMetaType<KUrl>("KUrl");

    AutoTestShell::init();
    m_core = new KDevelop::TestCore();
    m_core->initialize(KDevelop::Core::NoUi);

}

void ConnectionTest::cleanup()
{
    m_core->cleanup();
    delete m_core;
}

void ConnectionTest::testStdOutput()
{
    QStringList contents;
    contents << "<?php"
            << "$i = 0;"
            << "echo \"foo\\n\";"
            << "$i++;"
            << "echo \"foo\";"
            << "echo \"bar\";"
            << "echo \"\\n\";";
    QTemporaryFile file("xdebugtest");
    file.open();
    QString fileName = file.fileName();
    file.write(contents.join("\n").toUtf8());
    file.close();

    Server server;
    server.listen(9001);

    QSignalSpy outputLineSpy(&server, SIGNAL(outputLine(DebugSession*,QString, KDevelop::IRunProvider::OutputTypes)));
    server.startDebugger(fileName);
    server.waitForConnected();
    DebugSession* session = server.lastSession();
    QSignalSpy outputSpy(session, SIGNAL(output(QString, KDevelop::IRunProvider::OutputTypes)));
    session->waitForState(DebugSession::StartingState);
    session->run();
    session->waitForFinished();
    session->stopDebugger();
    session->waitForState(DebugSession::StoppedState);
    {
        QCOMPARE(outputSpy.count(), 4);
        QList<QVariant> arguments = outputSpy.takeFirst();
        QCOMPARE(arguments.count(), 2);
        QCOMPARE(arguments.first().toString(), QString("foo\n"));
    }
    {
        QCOMPARE(outputLineSpy.count(), 2);
        QList<QVariant> arguments = outputLineSpy.takeFirst();
        QCOMPARE(arguments.count(), 3);
        QCOMPARE(arguments.at(1).toString(), QString("foo"));
        arguments = outputLineSpy.takeFirst();
        QCOMPARE(arguments.count(), 3);
        QCOMPARE(arguments.at(1).toString(), QString("foobar"));
    }
    
    delete session;
}

void ConnectionTest::testShowStepInSource()
{
    QStringList contents;
    contents << "<?php"
            << "$i = 0;"
            << "$i++;";
    QTemporaryFile file("xdebugtest");
    file.open();
    QString fileName = file.fileName();
    file.write(contents.join("\n").toUtf8());
    file.close();

    Server server;
    server.listen(9001);

    server.startDebugger(fileName);
    server.waitForConnected();
    DebugSession* session = server.lastSession();
    QSignalSpy showStepInSourceSpy(session, SIGNAL(showStepInSource(KUrl, int)));

    session->waitForState(DebugSession::StartingState);
    session->stepInto();
    session->waitForState(DebugSession::PausedState);
    session->stepInto();
    session->waitForState(DebugSession::PausedState);
    session->run();
    session->waitForFinished();
    session->stopDebugger();
    session->waitForState(DebugSession::StoppedState);

    {
        QCOMPARE(showStepInSourceSpy.count(), 2);
        QList<QVariant> arguments = showStepInSourceSpy.takeFirst();
        QCOMPARE(arguments.first().value<KUrl>(), KUrl("file://"+QDir::currentPath()+'/'+fileName));
        QCOMPARE(arguments.at(1).toInt(), 1);

        arguments = showStepInSourceSpy.takeFirst();
        QCOMPARE(arguments.first().value<KUrl>(), KUrl("file://"+QDir::currentPath()+'/'+fileName));
        QCOMPARE(arguments.at(1).toInt(), 2);
    }
    delete session;
}


void ConnectionTest::testMultipleSessions()
{
    QStringList contents;
    contents << "<?php"
            << "$i = 0;"
            << "$i++;";
    QTemporaryFile file("xdebugtest");
    file.open();
    QString fileName = file.fileName();
    file.write(contents.join("\n").toUtf8());
    file.close();

    Server server;
    server.listen(9001);

    for (int i=0; i<10; ++i) {
        server.startDebugger(fileName);
        server.waitForConnected();
        DebugSession* session = server.lastSession();
        kDebug() << session;
        session->waitForState(DebugSession::StartingState);
        session->run();
        session->waitForFinished();
        session->stopDebugger();
        session->waitForState(DebugSession::StoppedState);
        delete session;        
    }
}

void ConnectionTest::testStackModel()
{
    QStringList contents;
    contents << "<?php"         // 1
            << "function x() {" // 2
            << "  echo 'x';"    // 3
            << "}"              // 4
            << "x();"           // 5
            << "echo 'y';";     // 6
    QTemporaryFile file("xdebugtest");
    file.open();
    QString fileName = file.fileName();
    file.write(contents.join("\n").toUtf8());
    file.close();

    Server server;
    server.listen(9001);

    server.startDebugger(fileName);
    server.waitForConnected();
    DebugSession* session = server.lastSession();
    kDebug() << session;
    session->waitForState(DebugSession::StartingState);
    
    //step into function
    for (int i=0; i<3; ++i) {
        session->stepInto();
        session->waitForState(DebugSession::PausedState);
    }
    
    KDevelop::StackModel* model = session->stackModel();
    model->setAutoUpdate(true);
    QTest::qWait(100);
    
    QCOMPARE(model->rowCount(QModelIndex()), 1); //one fake thread
    
    KDevelop::FramesModel* fmodel=model->modelForThread(0);

    QCOMPARE(fmodel->data(fmodel->index(0,0), Qt::DisplayRole).toString(), QString("0"));
    QCOMPARE(fmodel->data(fmodel->index(0,1), Qt::DisplayRole).toString(), QString("x"));
    QCOMPARE(fmodel->data(fmodel->index(0,2), Qt::DisplayRole).toString(), "file://"+QDir::currentPath()+"/"+fileName+QString(":3"));
    QCOMPARE(fmodel->data(fmodel->index(1,0), Qt::DisplayRole).toString(), QString("1"));
    QCOMPARE(fmodel->data(fmodel->index(1,1), Qt::DisplayRole).toString(), QString("{main}"));
    QCOMPARE(fmodel->data(fmodel->index(1,2), Qt::DisplayRole).toString(), "file://"+QDir::currentPath()+"/"+fileName+QString(":5"));
    
    session->stepInto();
    session->waitForState(DebugSession::PausedState);
    session->stepInto();
    session->waitForState(DebugSession::PausedState);
    QTest::qWait(100);
    QCOMPARE(model->rowCount(QModelIndex()), 1);
    QCOMPARE(fmodel->rowCount(QModelIndex()), 1);

    session->run();
    session->waitForFinished();
    session->stopDebugger();
    session->waitForState(DebugSession::StoppedState);
    delete session;
}

/*
void ConnectionTest::testBreakpoint()
{
    QStringList contents;
    contents << "<?php"         // 1
            << "function x() {" // 2
            << "  echo 'x';"    // 3
            << "}"              // 4
            << "x();"           // 5
            << "echo 'y';";     // 6
    QTemporaryFile file("xdebugtest");
    file.open();
    QString fileName = file.fileName();
    file.write(contents.join("\n").toUtf8());
    file.close();

    Server server;
    server.listen(9001);
    BreakpointController *breakpointController = server.breakpointController();
    QString location("file://"+QDir::currentPath()+"/"+fileName+QString(":3"));
    breakpointController->breakpointsItem()->addCodeBreakpoint(location);

    server.startDebugger(fileName);
    server.waitForConnected();
    DebugSession* session = server.lastSession();
    kDebug() << session;
    QSignalSpy showStepInSourceSpy(session, SIGNAL(showStepInSource(KUrl, int)));

    session->waitForState(DebugSession::StartingState);
    session->run();
    session->waitForState(DebugSession::PausedState);

    session->run();
    session->waitForFinished();
    session->waitForFinished();
    {
        QCOMPARE(showStepInSourceSpy.count(), 1);
        QList<QVariant> arguments = showStepInSourceSpy.takeFirst();
        QCOMPARE(arguments.first().value<KUrl>(), KUrl("file://"+QDir::currentPath()+'/'+fileName));
        QCOMPARE(arguments.at(1).toInt(), 3);
    }
}
*/

//     controller.connection()->sendCommand("property_get -i 123 -n $i");
//     controller.connection()->sendCommand("eval -i 124", QStringList(), "eval(\"function test124() { return rand(); } return test124();\")");
//     controller.connection()->sendCommand("eval -i 126", QStringList(), "test124();");


QTEST_MAIN( ConnectionTest )

#include "connectiontest.moc"
