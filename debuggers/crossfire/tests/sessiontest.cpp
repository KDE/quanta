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
#include <debugger/interfaces/ibreakpointcontroller.h>

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
        KConfigGroup pmCfg = cfg.group("Path Mappings").group("0");
        pmCfg.writeEntry("Remote", KUrl(buildBaseUrl));
        pmCfg.writeEntry("Local", KUrl(QDir::currentPath()));
    }
    ~TestLaunchConfiguration() {
        delete c;
    }
    virtual const KConfigGroup config() const { return cfg; }
    virtual KConfigGroup config() { return cfg; }
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


class TempTestFile : public QTemporaryFile
{
public:
    TempTestFile(const QStringList &contents, const QString &templateName = "crossfiretestXXXXXXXXXXXX.html")
        : QTemporaryFile(templateName)
    {
        open();
        write(contents.join("\n").toUtf8());
        close();
        setPermissions(QFile::ReadOther | permissions());
        m_url = buildBaseUrl + fileName();
        m_localPath = QDir::currentPath() + '/' + fileName();
    }
    KUrl url()
    {
        return m_url;
    }
    QString localPath()
    {
        return m_localPath;
    }
private:
    KUrl m_url;
    QString m_localPath;
};



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
    TempTestFile file(contents);

    TestDebugSession session;

    TestLaunchConfiguration cfg(file.url());
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
    TempTestFile file(contents);

    KDevelop::ICore::self()->debugController()->breakpointModel()->addCodeBreakpoint(file.url(), 6);

    TestDebugSession session;

    TestLaunchConfiguration cfg(file.url());
    TestDebugJob job(&session, &cfg);
    job.start();

    QVERIFY(session.waitForHandshake());
    QTest::qWait(5000);
    QCOMPARE(session.state(), KDevelop::IDebugSession::PausedState);
    QCOMPARE(session.line(), 6);
    QCOMPARE(session.url(), KUrl(file.localPath()));

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
    TempTestFile file(contents);

    KDevelop::ICore::self()->debugController()->breakpointModel()->addCodeBreakpoint(file.url(), 5);
    KDevelop::ICore::self()->debugController()->breakpointModel()->addCodeBreakpoint(file.url(), 10);

    TestDebugSession session;

    TestLaunchConfiguration cfg(file.url());
    TestDebugJob job(&session, &cfg);
    job.start();

    QVERIFY(session.waitForHandshake());
    QTest::qWait(6000);
    QCOMPARE(session.state(), KDevelop::IDebugSession::PausedState);
    QCOMPARE(session.line(), 5);
    QCOMPARE(session.url(), KUrl(file.localPath()));

    KDevelop::ICore::self()->debugController()->breakpointModel()->removeRow(0);
    QTest::qWait(1000);
    session.run();
    QTest::qWait(1000);

    QCOMPARE(session.state(), KDevelop::IDebugSession::PausedState);
    QTest::qWait(1000);
    QCOMPARE(session.line(), 10);
    QCOMPARE(session.url(), KUrl(file.localPath()));
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

    TempTestFile file(contents);

    KDevelop::ICore::self()->debugController()->breakpointModel()->addCodeBreakpoint(file.url(), 5);

    TestDebugSession session;

    TestLaunchConfiguration cfg(file.url());
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
    COMPARE_DATA(tIdx.child(0, 2), file.url().url()+":6");
    COMPARE_DATA(tIdx.child(1, 0), "1");
    COMPARE_DATA(tIdx.child(1, 1), "bsdf");
    COMPARE_DATA(tIdx.child(1, 2), file.url().url()+":9");
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
    TempTestFile file(contents);

    KDevelop::ICore::self()->debugController()->breakpointModel()->addCodeBreakpoint(file.url(), 7);

    TestDebugSession session;
    session.variableController()->setAutoUpdate(KDevelop::IVariableController::UpdateLocals);

    TestLaunchConfiguration cfg(file.url());
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
    TempTestFile file(contents);

    KDevelop::ICore::self()->debugController()->breakpointModel()->addCodeBreakpoint(file.url(), 7);

    TestDebugSession session;
    session.variableController()->setAutoUpdate(KDevelop::IVariableController::UpdateWatches);

    TestLaunchConfiguration cfg(file.url());
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

void SessionTest::testInsertInvalidBreakpoint()
{
    QStringList contents;
    contents << "<html>"
            << "<body>"
            << "foo"
            << "<script type=\"text/javascript\">"
            << "setTimeout(function() {"
            << "}, 1000);"
            << "</script>"
            << "</body>"
            << "</html>";
    TempTestFile file(contents);

    KDevelop::Breakpoint* b = KDevelop::ICore::self()->debugController()->breakpointModel()->addCodeBreakpoint(KUrl("nonexisting"), 6);

    TestDebugSession session;

    TestLaunchConfiguration cfg(file.url());
    TestDebugJob job(&session, &cfg);
    job.start();

    QVERIFY(session.waitForHandshake());
    QTest::qWait(5000);
    QSet<KDevelop::Breakpoint::Column> errors = session.breakpointController()->breakpointErrors(b);
    QVERIFY(errors.contains(KDevelop::Breakpoint::LocationColumn));
}

void SessionTest::testStepInto()
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
    TempTestFile file(contents);


    KDevelop::ICore::self()->debugController()->breakpointModel()->addCodeBreakpoint(file.url(), 6);

    TestDebugSession session;

    TestLaunchConfiguration cfg(file.url());
    TestDebugJob job(&session, &cfg);
    job.start();

    QVERIFY(session.waitForHandshake());
    QTest::qWait(5000);
    QCOMPARE(session.state(), KDevelop::IDebugSession::PausedState);
    QCOMPARE(session.line(), 6);
    QCOMPARE(session.url(), KUrl(file.localPath()));
    session.stepInto();
    QTest::qWait(2000);
    QCOMPARE(session.state(), KDevelop::IDebugSession::PausedState);
    QCOMPARE(session.line(), 7);
    QCOMPARE(session.url(), KUrl(file.localPath()));
}

void SessionTest::testContinue()
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
    TempTestFile file(contents);


    KDevelop::ICore::self()->debugController()->breakpointModel()->addCodeBreakpoint(file.url(), 6);
    KDevelop::ICore::self()->debugController()->breakpointModel()->addCodeBreakpoint(file.url(), 7);

    TestDebugSession session;

    TestLaunchConfiguration cfg(file.url());
    TestDebugJob job(&session, &cfg);
    job.start();

    QVERIFY(session.waitForHandshake());
    QTest::qWait(5000);
    QCOMPARE(session.state(), KDevelop::IDebugSession::PausedState);
    QCOMPARE(session.line(), 6);
    QCOMPARE(session.url(), KUrl(file.localPath()));
    session.run();
    QTest::qWait(2000);
    QCOMPARE(session.state(), KDevelop::IDebugSession::PausedState);
    QCOMPARE(session.line(), 7);
    QCOMPARE(session.url(), KUrl(file.localPath()));
}

void SessionTest::testPrepocessedMerged()
{
    QStringList jsContents1;
    jsContents1 << "var i = 0;";
    TempTestFile file1(jsContents1, "crossfiretest-test1-XXXXXXXXXXXX.js");

    QStringList jsContents2;
    jsContents2 << "setTimeout(function() {";
    jsContents2 << "i++;";
    jsContents2 << "console.log(i);";
    jsContents2 << "}, 3000);";
    TempTestFile file2(jsContents2, "crossfiretest-test2-XXXXXXXXXXXX.js");

    QStringList jsContentsMerged;
    jsContentsMerged << "//line 1 \""+file1.localPath()+"\"";
    jsContentsMerged << jsContents1;
    jsContentsMerged << "//line 1 \""+file2.localPath()+"\"";
    jsContentsMerged << jsContents2;
    TempTestFile fileMerged(jsContentsMerged, "crossfiretest-merged-XXXXXXXXXXXX.js");

    QStringList contents;
    contents << "<html>"
            << "<body>"
            << "foo"
            << "<script type=\"text/javascript\" src=\""+fileMerged.url().url()+"\">"
            << "</script>"
            << "</body>"
            << "</html>";
    TempTestFile file(contents);

    KDevelop::ICore::self()->debugController()->breakpointModel()->addCodeBreakpoint(file2.localPath(), 2);

    TestDebugSession session;

    TestLaunchConfiguration cfg(file.url());
    TestDebugJob job(&session, &cfg);
    job.start();

    QVERIFY(session.waitForHandshake());
    QTest::qWait(2500);
    fileMerged.remove(); //because else the merged file (which actually exists on disk) is used
    QTest::qWait(5000);
    QCOMPARE(session.state(), KDevelop::IDebugSession::PausedState);
    QCOMPARE(session.line(), 2);
    QCOMPARE(session.url(), KUrl(file2.localPath()));
}

void SessionTest::testPrepocessedMergedLinesRemoved()
{
    QStringList jsContents1;
    jsContents1 << "var i = 0;";
    TempTestFile file1(jsContents1, "crossfiretest-test1-XXXXXXXXXXXX.js");

    QStringList jsContents2;
    jsContents2 << "setTimeout(function() {";      //0
    jsContents2 << "i++;";                         //1
    jsContents2 << "/*";                           //2
    jsContents2 << "Useless comment that will get removed serverside (at least we pretend so)";
    jsContents2 << "*/";                           //3
    jsContents2 << "console.log(i);";              //4
    jsContents2 << "}, 3000);";
    TempTestFile file2(jsContents2, "crossfiretest-test2-XXXXXXXXXXXX.js");

    QStringList jsContentsMerged;
    jsContentsMerged << "//line 1 \""+file1.localPath()+"\"";
    jsContentsMerged << jsContents1;
    jsContentsMerged << "//line 1 \""+file2.localPath()+"\"";
    jsContentsMerged << jsContents2.mid(0, 2);
    jsContentsMerged << "//line 6 \""+file2.localPath()+"\"";
    jsContentsMerged << jsContents2.mid(5);
    TempTestFile fileMerged(jsContentsMerged, "crossfiretest-merged-XXXXXXXXXXXX.js");

    QStringList contents;
    contents << "<html>"
            << "<body>"
            << "foo"
            << "<script type=\"text/javascript\" src=\""+fileMerged.url().url()+"\">"
            << "</script>"
            << "</body>"
            << "</html>";
    TempTestFile file(contents);

    KDevelop::ICore::self()->debugController()->breakpointModel()->addCodeBreakpoint(file2.localPath(), 1);
    KDevelop::ICore::self()->debugController()->breakpointModel()->addCodeBreakpoint(file2.localPath(), 4);

    TestDebugSession session;
    TestLaunchConfiguration cfg(file.url());
    TestDebugJob job(&session, &cfg);
    job.start();

    QVERIFY(session.waitForHandshake());
    QTest::qWait(2500);
    fileMerged.remove(); //because else the merged file (which actually exists on disk) is used



    QPair<KUrl, int> u = session.convertToRemoteUrl(qMakePair(KUrl(file1.localPath()), 0));
    kDebug() << u.first << fileMerged.url();
    QCOMPARE(u.first, fileMerged.url());
    QCOMPARE(u.second, 1);

    u = session.convertToRemoteUrl(qMakePair(KUrl(file2.localPath()), 0));
    QCOMPARE(u.second, 3);

    u = session.convertToRemoteUrl(qMakePair(KUrl(file2.localPath()), 1));
    QCOMPARE(u.second, 4);

    u = session.convertToRemoteUrl(qMakePair(KUrl(file2.localPath()), 4));
    QCOMPARE(u.second, 6);

    u = session.convertToLocalUrl(qMakePair(KUrl(fileMerged.url()), 1));
    QCOMPARE(u.first, KUrl(file1.localPath()));
    QCOMPARE(u.second, 0);

    u = session.convertToLocalUrl(qMakePair(KUrl(fileMerged.url()), 3));
    QCOMPARE(u.first, KUrl(file2.localPath()));
    QCOMPARE(u.second, 0);

    u = session.convertToLocalUrl(qMakePair(KUrl(fileMerged.url()), 6));
    QCOMPARE(u.first, KUrl(file2.localPath()));
    QCOMPARE(u.second, 4);

    QTest::qWait(2000);
    QCOMPARE(session.state(), KDevelop::IDebugSession::PausedState);
    QCOMPARE(session.line(), 1);
    QCOMPARE(session.url(), KUrl(file2.localPath()));
    session.run();
    QTest::qWait(2000);
    QCOMPARE(session.state(), KDevelop::IDebugSession::PausedState);
    QCOMPARE(session.line(), 4);
    QCOMPARE(session.url(), KUrl(file2.localPath()));
}



QTEST_KDEMAIN(SessionTest, GUI)

#include "moc_sessiontest.cpp"
#include "sessiontest.moc"
