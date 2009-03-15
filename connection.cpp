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


#include "connection.h"

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QtCore/QSocketNotifier>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QTextCodec>

#include <KDebug>
#include <KLocale>

#include <interfaces/irunprovider.h>

namespace XDebug {

Connection::Connection(QObject * parent)
    : QObject(parent),
    m_currentClient(0), m_server(0)
{
    m_codec = QTextCodec::codecForLocale();
}

Connection::~Connection()
{
    close();
}

void Connection::closed()
{
    if (m_currentClient) {
        m_currentClient->deleteLater();
        m_currentClient = 0;
    }
    setState(StoppedState);
}

void Connection::close()
{
    if (m_currentClient) {
        delete m_currentClient;
        m_currentClient = 0;
    }
}

bool Connection::isListening() {
    return m_server->isListening();
}

void Connection::incomingConnection()
{
    Q_ASSERT(!m_currentClient);
    m_currentClient = m_server->nextPendingConnection();

    connect(m_currentClient, SIGNAL(disconnected()), this, SLOT(closed()));
    connect(m_currentClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error( QAbstractSocket::SocketError)));

    connect(m_currentClient, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void Connection::readyRead()
{
    while (m_currentClient && m_currentClient->bytesAvailable()) {
        long length;
        {
            QByteArray data;
            char c;
            while(m_currentClient->getChar(&c)) {
                if(c==0) break;
                data.append(c);
            }
            length = data.toLong();
        }
        QByteArray data;
        while (data.length() <= length) {
            if (!data.isEmpty() && !m_currentClient->waitForReadyRead()) {
                return;
            }
            data += m_currentClient->read(length+1);
        }
        //kDebug() << data;
        
        QXmlStreamReader* xml = new QXmlStreamReader(data);
        while (!xml->atEnd()) {
            xml->readNext();
            if (xml->isStartElement()) {
                if (xml->name() == "init") {
                    processInit(xml);
                } else if (xml->name() == "response") {
                    processResponse(xml);
                } else if (xml->name() == "stream") {
                    processStream(xml);
                } else {
                    //kWarning() << "unknown element" << xml->name();
                }
            }
        }
        delete xml;
    }
}
void Connection::sendCommand(const QString& cmd, const QStringList& arguments, const QByteArray& data)
{
    QByteArray out = m_codec->fromUnicode(cmd);
    if (!arguments.isEmpty()) {
        out += ' ' + m_codec->fromUnicode(arguments.join(QString(' ')));
    }
    if (!data.isEmpty()) {
        out += " -- " + data.toBase64();
    }
    kDebug() << out;
    m_currentClient->write(out);
    m_currentClient->write("\0", 1);
}

void Connection::processInit(QXmlStreamReader* xml)
{
    m_idKey = xml->attributes().value("idkey").toString();
    sendCommand("feature_get -i 3 -n encoding");
    sendCommand("stderr -i 1 -c 1"); //copy stderr to IDE
    sendCommand("stdout -i 2 -c 1"); //copy stdout to IDE
    setState(StartingState);
}

void Connection::processResponse(QXmlStreamReader* xml)
{
    if (xml->attributes().value("status") == "running") {
        setState(RunningState);
    } else if (xml->attributes().value("status") == "stopped") {
        setState(StoppedState);
    } else if (xml->attributes().value("status") == "break") {
        setState(BreakState);
        xml->readNext();
        if (xml->isStartElement() && xml->namespaceUri() == "http://xdebug.org/dbgp/xdebug" && xml->name() == "message") {
            QString fileName = xml->attributes().value("filename").toString();
            int lineNum = xml->attributes().value("lineno").toString().toInt()-1;
            emit showStepInSource(fileName, lineNum);
        }
    }
    if (xml->attributes().value("command") == "feature_get" && xml->attributes().value("feature_name") == "encoding") {
        xml->readNext();
        QTextCodec* c = QTextCodec::codecForName(xml->text().toString().toAscii());
        if (c) {
            m_codec = c;
        }
    }
}

void Connection::setState(DebuggerState state)
{
    //kDebug() << state;
    m_currentState = state;
    emit stateChanged(state);
    if (state == StoppedState) {
        close();
    }
}

DebuggerState Connection::currentState()
{
    return m_currentState;
}


void Connection::processStream(QXmlStreamReader* xml)
{
    if (xml->attributes().value("encoding") == "base64") {
        KDevelop::IRunProvider::OutputTypes outputType;
        if (xml->attributes().value("type") == "stdout") {
            outputType = KDevelop::IRunProvider::StandardOutput;
        } else if (xml->attributes().value("type") == "stderr") {
            outputType = KDevelop::IRunProvider::StandardError;
        } else {
            kWarning() << "unknown output type" << xml->attributes().value("type");
            return;
        }
        xml->readNext();
        QString c = m_codec->toUnicode(QByteArray::fromBase64(xml->text().toString().toAscii()));
        //kDebug() << c;
        emit output(c, outputType);
        m_outputLine[outputType] += c;
        int pos = m_outputLine[outputType].indexOf('\n');
        if (pos != -1) {
            emit outputLine(m_outputLine[outputType].left(pos), outputType);
            m_outputLine[outputType] = m_outputLine[outputType].mid(pos+1);
        }
    } else {
        kWarning() << "unknown encoding" << xml->attributes().value("encoding");
    }
}

void Connection::processFinished(int exitCode)
{
    Q_UNUSED(exitCode);
    QMapIterator<KDevelop::IRunProvider::OutputTypes, QString> i(m_outputLine);
    while (i.hasNext()) {
        i.next();
        emit outputLine(i.value(), i.key());
    }
}


void Connection::error(QAbstractSocket::SocketError error)
{
    kWarning() << error << m_server->errorString();
}


bool Connection::listen(int port)
{
    Q_ASSERT(!m_server);
    m_server = new QTcpServer(this);
    if(m_server->listen(QHostAddress::Any, port)) {
        connect(m_server, SIGNAL(newConnection()), this, SLOT(incomingConnection()));
        kDebug() << "listening on" << port;
    } else {
        kDebug() << "Error" << m_server->errorString();
        delete m_server;
        m_server = 0;
    }

    return m_server->isListening();
}

QTcpSocket* Connection::currentClient() {
    return m_currentClient;
}

bool Connection::waitForNewConnection(int msecs) {
    return m_server->waitForNewConnection(msecs);
}

}

#include "connection.moc"
