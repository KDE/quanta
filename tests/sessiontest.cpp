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
#include <debugger/interfaces/iframestackmodel.h>
#include <debugger/interfaces/ivariablecontroller.h>
#include <debugger/variable/variablecollection.h>

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

#define COMPARE_DATA(index, expected) \
    compareData((index), (expected), __FILE__, __LINE__)
void compareData(QModelIndex index, QString expected, const char *file, int line)
{
    QString s = index.model()->data(index, Qt::DisplayRole).toString();
    if (s != expected) {
        kFatal() << QString("'%0' didn't match expected '%1' in %2:%3").arg(s).arg(expected).arg(file).arg(line);
    }
}


void SessionTest::init()
{
    qRegisterMetaType<DebugSession*>("DebugSession*");
    qRegisterMetaType<KUrl>("KUrl");

    KDevelop::AutoTestShell::init();
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
    QTemporaryFile file("crossfiretestXXXXXXXXXXXX.html");
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
    QTemporaryFile file("crossfiretestXXXXXXXXXXXX.html");
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
            << "}, 4000);"
            << "</script>"
            << "</body>"
            << "</html>";
    QTemporaryFile file("crossfiretestXXXXXXXXXXXX.html");
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
    QTest::qWait(6000);
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

void SessionTest::testFrameStack()
{
    QStringList contents;
    contents << "<html>"                             // 0
            << "<body>"                              // 1
            << "foo"                                 // 2
            << "<script type=\"text/javascript\">"   // 3
            << "function asdf() {"                   // 4
            << "  var i=0;"                          // 5
            << "}"                                   // 6
            << "function bsdf() {"                   // 7
            << "  asdf();"                           // 8
            << "}"                                   // 9
            << "function csdf() {"                   // 10
            << "  bsdf();"                           // 11
            << "}"                                   // 12
            << "function dsdf() {"                   // 10
            << "  csdf();"                           // 11
            << "}"                                   // 12
            << "setTimeout(function() {"             // 13
            << "  dsdf();"                           // 14
            << "}, 2000);"
            << "</script>"
            << "</body>"
            << "</html>";

    QTemporaryFile file("crossfiretestXXXXXXXXXXXX.html");
    file.open();
    KUrl url(buildBaseUrl + file.fileName());
    file.write(contents.join("\n").toUtf8());
    file.close();
    file.setPermissions(QFile::ReadOther | file.permissions());

    KDevelop::ICore::self()->debugController()->breakpointModel()->addCodeBreakpoint(url, 5);

    TestDebugSession session;

    TestLaunchConfiguration cfg(url);
    TestDebugJob job(&session, &cfg);
    job.start();

    QVERIFY(session.waitForHandshake());
    QTest::qWait(5000);
    QCOMPARE(session.state(), KDevelop::IDebugSession::PausedState);
    QCOMPARE(session.line(), 5);

    KDevelop::IFrameStackModel* stackModel = session.frameStackModel();

    QCOMPARE(stackModel->rowCount(QModelIndex()), 1); //one fake thread

    QModelIndex tIdx = stackModel->index(0,0);
    QCOMPARE(stackModel->rowCount(tIdx), 5);
    QCOMPARE(stackModel->columnCount(tIdx), 3);
    COMPARE_DATA(tIdx.child(0, 0), "0");
    COMPARE_DATA(tIdx.child(0, 1), "asdf");
    COMPARE_DATA(tIdx.child(0, 2), url.url()+":6");
    COMPARE_DATA(tIdx.child(1, 0), "1");
    COMPARE_DATA(tIdx.child(1, 1), "bsdf");
    COMPARE_DATA(tIdx.child(1, 2), url.url()+":9");
    COMPARE_DATA(tIdx.child(2, 1), "csdf");
    COMPARE_DATA(tIdx.child(3, 1), "dsdf");
    COMPARE_DATA(tIdx.child(4, 1), "anonymous");
}

KDevelop::VariableCollection *variableCollection()
{
    return KDevelop::ICore::self()->debugController()->variableCollection();
}

void SessionTest::testVariablesLocals()
{
    QStringList contents;
    contents << "<html>"                             // 0
            << "<body>"                              // 1
            << "foo"                                 // 2
            << "<script type=\"text/javascript\">"   // 3
            << "function asdf(x) {"                  // 4
            << "  var o = { foo: 'foo', bar: 1 };"   // 5
            << "  var i=0;"                          // 6
            << "  i++;"                              // 7 **
            << "}"                                   // 8
            << "setTimeout(function() {"             // 9
            << "  asdf(10);"                         // 10
            << "}, 2000);"
            << "</script>"
            << "</body>"
            << "</html>";
    QTemporaryFile file("crossfiretestXXXXXXXXXXXX.html");
    file.open();
    KUrl url(buildBaseUrl + file.fileName());
    file.write(contents.join("\n").toUtf8());
    file.close();
    file.setPermissions(QFile::ReadOther | file.permissions());

    KDevelop::ICore::self()->debugController()->breakpointModel()->addCodeBreakpoint(url, 7);

    TestDebugSession session;
    session.variableController()->setAutoUpdate(KDevelop::IVariableController::UpdateLocals);

    TestLaunchConfiguration cfg(url);
    TestDebugJob job(&session, &cfg);
    job.start();

    QVERIFY(session.waitForHandshake());
    QTest::qWait(4000);
    QCOMPARE(session.state(), KDevelop::IDebugSession::PausedState);
    QCOMPARE(session.line(), 7);
    QTest::qWait(1000);

    QVERIFY(variableCollection()->rowCount() == 2);
    QModelIndex i = variableCollection()->index(1, 0);
    COMPARE_DATA(i, "Locals");
    QCOMPARE(variableCollection()->rowCount(i), 3);
    COMPARE_DATA(variableCollection()->index(0, 0, i), "i");
    COMPARE_DATA(variableCollection()->index(0, 1, i), "0");
    COMPARE_DATA(variableCollection()->index(1, 0, i), "o");
    COMPARE_DATA(variableCollection()->index(1, 1, i), "");
    COMPARE_DATA(variableCollection()->index(2, 0, i), "x");
    COMPARE_DATA(variableCollection()->index(2, 1, i), "10");
    i = variableCollection()->index(1, 0, i);
    variableCollection()->expanded(i);
    QTest::qWait(1000);
    QCOMPARE(variableCollection()->rowCount(i), 2);
    COMPARE_DATA(variableCollection()->index(0, 0, i), "bar");
    COMPARE_DATA(variableCollection()->index(0, 1, i), "1");
    COMPARE_DATA(variableCollection()->index(1, 0, i), "foo");
    COMPARE_DATA(variableCollection()->index(1, 1, i), "foo");
}


void SessionTest::testVariablesWatch()
{
    QStringList contents;
    contents << "<html>"                             // 0
            << "<body>"                              // 1
            << "foo"                                 // 2
            << "<script type=\"text/javascript\">"   // 3
            << "function asdf(x) {"                  // 4
            << "  var o = { foo: 'foo', bar: 1 };"   // 5
            << "  var i=0;"                          // 6
            << "  i++;"                              // 7
            << "}"                                   // 8
            << "setTimeout(function() {"             // 9
            << "  asdf(10);"                         // 10
            << "}, 2000);"
            << "</script>"
            << "</body>"
            << "</html>";
    QTemporaryFile file("crossfiretestXXXXXXXXXXXX.html");
    file.open();
    KUrl url(buildBaseUrl + file.fileName());
    file.write(contents.join("\n").toUtf8());
    file.close();
    file.setPermissions(QFile::ReadOther | file.permissions());

    KDevelop::ICore::self()->debugController()->breakpointModel()->addCodeBreakpoint(url, 7);

    TestDebugSession session;
    session.variableController()->setAutoUpdate(KDevelop::IVariableController::UpdateWatches);

    TestLaunchConfiguration cfg(url);
    TestDebugJob job(&session, &cfg);
    job.start();

    QVERIFY(session.waitForHandshake());
    QTest::qWait(6000);
    QVERIFY(KDevelop::ICore::self()->debugController()->variableCollection()->watches()->add("i"));
    QCOMPARE(session.state(), KDevelop::IDebugSession::PausedState);
    QCOMPARE(session.line(), 7);
    QTest::qWait(1000);

    QVERIFY(variableCollection()->rowCount() == 2);
    QModelIndex i = variableCollection()->index(0, 0);
    COMPARE_DATA(i, "Auto");
    QCOMPARE(variableCollection()->rowCount(i), 1);
    COMPARE_DATA(variableCollection()->index(0, 0, i), "i");
    COMPARE_DATA(variableCollection()->index(0, 1, i), "0");
}


QTEST_KDEMAIN(SessionTest, GUI)

#include "moc_sessiontest.cpp"
#include "sessiontest.moc"
