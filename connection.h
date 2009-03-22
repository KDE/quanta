/*
 * XDebug Debugger Support
 *
 * Copyright (C) 2004-2006 by Thiago Silva <thiago.silva@kdemail.net>
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

#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtCore/QObject>
#include <QtNetwork/QAbstractSocket>
#include <QStringList>
#include <interfaces/irunprovider.h>

#include "debugsession.h"


class QXmlStreamReader;
class QTcpSocket;

namespace XDebug {

class Connection : public QObject
{
    Q_OBJECT
public:
    Connection(QTcpSocket* socket, QObject * parent = 0);
    ~Connection();

    void close();

    void sendCommand(const QString& cmd, const QStringList& arguments = QStringList(), const QByteArray& data = QByteArray());
    DebugSession::DebuggerState currentState();

    QTcpSocket* socket();

public Q_SLOTS:
    void processFinished(int exitCode);

Q_SIGNALS:
    void stateChanged(KDevelop::IDebugSession::DebuggerState status);
    void showStepInSource(const QString &fileName, int lineNum);
    void output(QString content, KDevelop::IRunProvider::OutputTypes type);
    void outputLine(QString content, KDevelop::IRunProvider::OutputTypes type);
    void initDone(const QString& ideKey);

private Q_SLOTS:
    void closed();
    void readyRead();
    void error(QAbstractSocket::SocketError error);

private:
    void processInit(QXmlStreamReader* xml);
    void processResponse(QXmlStreamReader* xml);
    void processStream(QXmlStreamReader* xml);
    void setState(DebugSession::DebuggerState state);

    QTcpSocket* m_socket;
    
    QMap<KDevelop::IRunProvider::OutputTypes, QString> m_outputLine;
    DebugSession::DebuggerState m_currentState;
    QTextCodec* m_codec;
};

}
#endif
