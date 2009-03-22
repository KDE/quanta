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
#include <QtCore/QSocketNotifier>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QTextCodec>

#include <KDebug>
#include <KLocale>

#include <interfaces/irunprovider.h>
#include "debugsession.h"

namespace XDebug {

Connection::Connection(QTcpSocket* socket, QObject * parent)
    : QObject(parent),
    m_socket(socket),
    m_currentState(DebugSession::NotStartedState)
{
    Q_ASSERT(m_socket);

    m_codec = QTextCodec::codecForLocale();

    connect(m_socket, SIGNAL(disconnected()), this, SLOT(closed()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error( QAbstractSocket::SocketError)));

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

Connection::~Connection()
{
    delete m_socket;
}

void Connection::closed()
{
    setState(DebugSession::StoppedState);
}

void Connection::close() {
    delete m_socket;
    m_socket = 0;
}


void Connection::error(QAbstractSocket::SocketError error)
{
    //kWarning() << m_socket->errorString();
    kWarning() << error;
}

void Connection::readyRead()
{
    while (m_socket && m_socket->bytesAvailable()) {
        long length;
        {
            QByteArray data;
            char c;
            while(m_socket->getChar(&c)) {
                if(c==0) break;
                data.append(c);
            }
            length = data.toLong();
        }
        QByteArray data;
        while (data.length() <= length) {
            if (!data.isEmpty() && !m_socket->waitForReadyRead()) {
                return;
            }
            data += m_socket->read(length+1);
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
    m_socket->write(out);
    m_socket->write("\0", 1);
}

void Connection::processInit(QXmlStreamReader* xml)
{
    sendCommand("feature_get -i 3 -n encoding");
    sendCommand("stderr -i 1 -c 1"); //copy stderr to IDE
    sendCommand("stdout -i 2 -c 1"); //copy stdout to IDE
    kDebug() << "idekey" << xml->attributes().value("idekey");
    emit initDone(xml->attributes().value("idekey").toString());

    setState(DebugSession::StartingState);
}

void Connection::processResponse(QXmlStreamReader* xml)
{
    if (xml->attributes().value("status") == "running") {
        setState(DebugSession::ActiveState);
    } else if (xml->attributes().value("status") == "stopped") {
        setState(DebugSession::StoppedState);
    } else if (xml->attributes().value("status") == "break") {
        setState(DebugSession::PausedState);
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

void Connection::setState(DebugSession::DebuggerState state)
{
    kDebug() << state;
    if (m_currentState != state) {
        m_currentState = state;
        if (state == DebugSession::StoppedState) {
            close();
        }
        emit stateChanged(state);
    }
}

DebugSession::DebuggerState Connection::currentState()
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



QTcpSocket* Connection::socket() {
    return m_socket;
}


}

#include "connection.moc"
