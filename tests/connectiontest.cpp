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

#include "connectiontest.h"

#include <QTcpServer>
#include <QTcpSocket>

#include <KDebug>
#include <KProcess>
#include <qtest_kde.h>

#include "connection.h"

/**
To run this tests you have to:
- enable firebug for for all websites
- close any running firefox instance
- eventually adapt firefoxCmd below
*/

static const char* firefoxCmd = "/opt/firefox/firefox-bin";

using namespace Crossfire;

void ConnectionTest::testConnect()
{
    QTcpServer server;
    QVERIFY(server.listen(QHostAddress::Any, 5000));

    KProcess p;
    p << firefoxCmd;
    p << "--crossfire-host=localhost";
    p << "--crossfire-port=5000";
    p << "www.kde.org";
    p.start();

    QVERIFY(server.waitForNewConnection(30000));
    QTcpSocket* socket = server.nextPendingConnection();
    Connection connection(socket);
    QVERIFY(connection.waitForHandshake());
    connection.sendCommand("version");

    QTest::qWait(3000);
    p.kill();
    p.waitForFinished();
}

void ConnectionTest::testLogOutput()
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
    KUrl url(QDir::currentPath() + "/" + file.fileName());
    file.write(contents.join("\n").toUtf8());
    file.close();

    QTcpServer server;
    QVERIFY(server.listen(QHostAddress::Any, 5000));

    KProcess p;
    p << firefoxCmd;
    p << "-no-remote";
    p << "--crossfire-host=localhost";
    p << "--crossfire-port=5000";
    p << url.url();
    p.start();

    QVERIFY(server.waitForNewConnection(30000));
    QTcpSocket* socket = server.nextPendingConnection();
    Connection connection(socket);

    QSignalSpy outputLineSpy(&connection, SIGNAL(outputLine(QString)));

    QVERIFY(connection.waitForHandshake());
    QTest::qWait(3000);

    {
        QCOMPARE(outputLineSpy.count(), 1);
        QList<QVariant> arguments = outputLineSpy.takeFirst();
        QCOMPARE(arguments.count(), 1);
        QCOMPARE(arguments.at(0).toString(), QString("hello world"));
    }

    p.kill();
    p.waitForFinished();
}

void ConnectionTest::testStack()
{
    QStringList contents;
    contents << "<html>"
            << "<body>"
            << "foo"
            << "<script type=\"text/javascript\">"
            << "function foo() {"
            << "  var j=0;"
            << "  j++;"
            << "  debugger;"
            << "  j++;"
            << "}"
            << "setTimeout(function() {"
            << "  foo();"
            << "}, 1000);"
            << "</script>"
            << "</body>"
            << "</html>";
    QTemporaryFile file("crossfiretest");
    file.open();
    KUrl url(QDir::currentPath() + "/" + file.fileName());
    file.write(contents.join("\n").toUtf8());
    file.close();

    QTcpServer server;
    QVERIFY(server.listen(QHostAddress::Any, 5000));

    KProcess p;
    p << firefoxCmd;
    p << "-no-remote";
    p << "--crossfire-host=localhost";
    p << "--crossfire-port=5000";
    p << url.url();
    p.start();

    QVERIFY(server.waitForNewConnection(30000));
    QTcpSocket* socket = server.nextPendingConnection();
    Connection connection(socket);
    QVERIFY(connection.waitForHandshake());
    QTest::qWait(3000);
    connection.sendCommand("listcontexts");
    connection.sendCommand("backtrace", url.url());
    QTest::qWait(3000);
    //no verifying here, this test is just for debugging

    p.kill();
    p.waitForFinished();
}



QTEST_KDEMAIN(ConnectionTest, GUI)

#include "connectiontest.moc"
