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

#include "sessiontest.h"

#include <QTcpServer>
#include <QTcpSocket>

#include <KDebug>
#include <KProcess>
#include <qtest_kde.h>
#include <KConfigGroup>

#include <interfaces/ilaunchconfiguration.h>
#include <interfaces/idebugcontroller.h>
#include <debugger/breakpoint/breakpointmodel.h>
#include <tests/autotestshell.h>
#include <tests/testcore.h>

#include "connection.h"
#include "debugsession.h"
#include "debugjob.h"

/**
To run this tests you have to:
- enable firebug for for all websites
- close any running firefox instance
- run a webserver pointing to crossfire build directory
- eventually adapt buildBaseUrl below
*/
static const char *buildBaseUrl = "http://localhost/crossfire-build/";


using namespace Crossfire;

class TestLaunchConfiguration : public KDevelop::ILaunchConfiguration
{
public:
    TestLaunchConfiguration(KUrl script) {
        c = new KConfig();
        cfg = c->group("launch");
        cfg.writeEntry("Server", script.host());
        cfg.writeEntry("Path", script.path());
        cfg.writeEntry("Arguments", script.query());
    }
    ~TestLaunchConfiguration() {
        delete c;
    }
    virtual const KConfigGroup config() const { return cfg; }
    virtual QString name() const { return QString("Test-Launch"); }
    virtual KDevelop::IProject* project() const { return 0; }
    virtual KDevelop::LaunchConfigurationType* type() const { return 0; }
private:
    KConfigGroup cfg;
    KConfig *c;
};

class TestDebugJob : public DebugJob
{
public:
    TestDebugJob(DebugSession* session, KDevelop::ILaunchConfiguration* cfg, QObject* parent = 0)
        : DebugJob(session, cfg, parent)
    {}

    ~TestDebugJob()
    {
        Q_ASSERT(browserPid());
        if (browserPid()) {
            KProcess p;
            p << "kill";
            p << QString::number(browserPid());
            p.execute();
            p.waitForFinished();
        }
    }
};

class TestDebugSession : public DebugSession
{
    Q_OBJECT
public:
    TestDebugSession() : DebugSession(), m_line(0)
    {
        qRegisterMetaType<KUrl>("KUrl");
        connect(this, SIGNAL(showStepInSource(KUrl, int)), SLOT(slotShowStepInSource(KUrl, int)));

        KDevelop::ICore::self()->debugController()->addSession(this);
    }
    KUrl url() { return m_url; }
    int line() { return m_line; }

private slots:
    void slotShowStepInSource(const KUrl &url, int line)
    {
        m_url = url;
        m_line = line;
    }
private:
    KUrl m_url;
    int m_line;

};

void SessionTest::init()
{
    qRegisterMetaType<DebugSession*>("DebugSession*");
    qRegisterMetaType<KUrl>("KUrl");

    AutoTestShell::init();
    m_core = new KDevelop::TestCore();
    m_core->initialize(KDevelop::Core::NoUi);

    //remove all breakpoints - so we can set our own in the test
    KDevelop::BreakpointModel* m = KDevelop::ICore::self()->debugController()->breakpointModel();
    m->removeRows(0, m->rowCount());
}

void SessionTest::cleanup()
{
    m_core->cleanup();
    delete m_core;
}

void SessionTest::testOutput()
{
    QStringList contents;
    contents << "<html>"
            << "<body>"
            << "foo"
            << "<script type=\"text/javascript\">"
            << "setTimeout(function() {"
            << "  console.log('hello world');"
            << "}, 1000);"
            << "</script>"
            << "</body>"
            << "</html>";
    QTemporaryFile file("crossfiretest");
    file.open();
    KUrl url(buildBaseUrl + file.fileName());
    file.write(contents.join("\n").toUtf8());
    file.close();
    file.setPermissions(QFile::ReadOther | file.permissions());

    TestDebugSession session;

    TestLaunchConfiguration cfg(url);
    TestDebugJob job(&session, &cfg);

    QSignalSpy outputLineSpy(&session, SIGNAL(outputLine(QString)));

    job.start();

    QVERIFY(session.waitForHandshake());
    QTest::qWait(5000);

    {
        QCOMPARE(outputLineSpy.count(), 1);
        QList<QVariant> arguments = outputLineSpy.takeFirst();
        QCOMPARE(arguments.count(), 1);
        QCOMPARE(arguments.at(0).toString(), QString("hello world"));
    }
}


void SessionTest::testInsertBreakpoint()
{
    QStringList contents;
    contents << "<html>"
            << "<body>"
            << "foo"
            << "<script type=\"text/javascript\">"
            << "setTimeout(function() {"
            << "  var i=0;"
            << "  i++;"
            << "  i++;"
            << "}, 3000);"
            << "</script>"
            << "</body>"
            << "</html>";
    QTemporaryFile file("crossfiretest");
    file.open();
    KUrl url(buildBaseUrl + file.fileName());
    file.write(contents.join("\n").toUtf8());
    file.close();
    file.setPermissions(QFile::ReadOther | file.permissions());

    KDevelop::ICore::self()->debugController()->breakpointModel()->addCodeBreakpoint(url, 6);

    TestDebugSession session;

    TestLaunchConfiguration cfg(url);
    TestDebugJob job(&session, &cfg);
    job.start();

    QVERIFY(session.waitForHandshake());
    QTest::qWait(5000);
    QCOMPARE(session.state(), KDevelop::IDebugSession::PausedState);
    QCOMPARE(session.line(), 6);
    QCOMPARE(session.url(), url);

}

void SessionTest::testRemoveBreakpoint()
{
    QStringList contents;
    contents << "<html>"                             // 0
            << "<body>"                              // 1
            << "foo"                                 // 2
            << "<script type=\"text/javascript\">"   // 3
            << "function asdf() {"                   // 4
            << "  var i=0;"                          // 5
            << "}"                                   // 6
            << "setTimeout(function() {"             // 7
            << "  asdf();"                           // 8
            << "  asdf();"                           // 9
            << "  asdf();"                           //10
            << "}, 2000);"
            << "</script>"
            << "</body>"
            << "</html>";
    QTemporaryFile file("crossfiretest");
    file.open();
    KUrl url(buildBaseUrl + file.fileName());
    file.write(contents.join("\n").toUtf8());
    file.close();
    file.setPermissions(QFile::ReadOther | file.permissions());

    KDevelop::ICore::self()->debugController()->breakpointModel()->addCodeBreakpoint(url, 5);
    KDevelop::ICore::self()->debugController()->breakpointModel()->addCodeBreakpoint(url, 10);

    TestDebugSession session;

    TestLaunchConfiguration cfg(url);
    TestDebugJob job(&session, &cfg);
    job.start();

    QVERIFY(session.waitForHandshake());
    QTest::qWait(3000);
    QCOMPARE(session.state(), KDevelop::IDebugSession::PausedState);
    QCOMPARE(session.line(), 5);
    QCOMPARE(session.url(), url);

    KDevelop::ICore::self()->debugController()->breakpointModel()->removeRow(0);
    QTest::qWait(1000);
    session.run();
    QTest::qWait(1000);

    QCOMPARE(session.state(), KDevelop::IDebugSession::PausedState);
    QCOMPARE(session.line(), 10);
    QCOMPARE(session.url(), url);
}


QTEST_KDEMAIN(SessionTest, GUI)

#include "moc_sessiontest.cpp"
#include "sessiontest.moc"
