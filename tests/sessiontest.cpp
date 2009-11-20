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

    DebugSession session;

    TestLaunchConfiguration cfg(url);
    DebugJob job(&session, &cfg);

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



QTEST_KDEMAIN(SessionTest, GUI)

#include "sessiontest.moc"
