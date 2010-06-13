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


#include "connection.h"

#include <QtNetwork/QTcpSocket>
#include <QtCore/QTextCodec>
#include <QtCore/QDebug>
#include <QtCore/QVariant>

#include <KDebug>
#include <KLocale>
#include <KUrl>

#include <qjson/parser.h>
#include <qjson/serializer.h>

namespace Crossfire {

static const char* HANDSHAKE_STRING = "CrossfireHandshake\r\n";


Connection::Connection(QTcpSocket* socket, QObject * parent)
    : QObject(parent),
    m_socket(socket), m_handsShaked(false), m_commandPending(false), m_lastSeqNr(0)
{

    Q_ASSERT(m_socket);
    m_socket->setParent(this);

    connect(m_socket, SIGNAL(disconnected()), this, SLOT(closed()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error( QAbstractSocket::SocketError)));
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

Connection::~Connection()
{
}

bool Connection::waitForHandshake(int timeout)
{
    if (m_handsShaked) return true;
    if (!m_socket->waitForConnected(timeout)) return false;
    if (!m_socket->waitForReadyRead(timeout)) return false;
    kDebug() << m_handsShaked;
    return m_handsShaked;
}

void Connection::closed()
{
}

void Connection::close() {
    delete m_socket;
    m_socket = 0;
}


void Connection::error(QAbstractSocket::SocketError error)
{
    kWarning() << "socket error" << error << m_socket->errorString();
}

void Connection::readyRead()
{
    kDebug() << m_handsShaked;;
    if (!m_handsShaked) {
        QByteArray data = m_socket->read(strlen(HANDSHAKE_STRING));
        kDebug() << data;
        if (data == HANDSHAKE_STRING) {
            m_socket->write(HANDSHAKE_STRING);
            m_socket->flush();
            m_socket->waitForBytesWritten();
            kDebug() << "handshake successful";
            m_handsShaked = true;
            if (!m_commandQueue.isEmpty()) {
                sendData(m_commandQueue.dequeue());
            }
        }
    }
    while (m_socket && m_socket->bytesAvailable()) {
        long length = 0;
        {
            QByteArray buff;
            char prev = 0;
            char curr = 0;
            while (prev != '\r' && curr != '\n') {
                prev = curr;
                if (!m_socket->getChar(&curr)) {
                    kDebug() << "getChar failed";
                    return;
                }
                buff.append(curr);
            }
            int cLen = buff.indexOf("Content-Length:");
            kDebug() << buff;
            if (cLen > -1 && buff.length() > 16) {
                length = buff.mid(cLen+15, buff.length()-2-(cLen+15)).toInt();
            }
        }
        if (!length) return;
        kDebug() << "Content-Length:" << length;

        QByteArray data;
        while (data.length() < length) {
            kDebug() << "should read" << (length - data.length());
            if (!data.isEmpty() && !m_socket->waitForReadyRead()) {
                kDebug() << "timeout";
                return;
            }
            data += m_socket->read(length - data.length());
            kDebug() << "now we have" << data.length() << "more?" << (data.length() < length);
        }

        kDebug() << "finished reading" << data.length() << "bytes";
        for (int i=0; i<data.length()/80; ++i) {
            kDebug() << data.mid(i*80, 80);
        }

        QJson::Parser parser;
        bool ok;
        QVariantMap result = parser.parse(data, &ok).toMap();
        if (!ok) {
            qWarning() << parser.errorString();
            return;
        }

        if (result["type"] == "response") {
            if (result["request_seq"].toInt() && m_callbacks.contains(result["request_seq"].toInt())) {
                m_callbacks[result["request_seq"].toInt()]->execute(result);
                delete m_callbacks[result["request_seq"].toInt()];
                m_callbacks.remove(result["request_seq"].toInt());
            }
//             result["success"];
//             result["running"];
            kDebug() << "command queue size" << m_commandQueue.size();
            if (!m_commandQueue.isEmpty()) {
                sendData(m_commandQueue.dequeue());
            } else {
                m_commandPending = false;
            }
        } else if (result["type"] == "event") {
            if (result["event"] == "onConsoleLog"
                || result["event"] == "onConsoleDebug"
                || result["event"] == "onConsoleInfo"
                || result["event"] == "onConsoleWarn"
                || result["event"] == "onConsoleError"
            ) {
                //TODO: compare result["context_id"]; with current context
                foreach (const QVariant &v, result["data"].toMap().values()) {
                    kDebug() << v.toString();
                    emit outputLine(v.toString());
                }
            }
            emit eventReceived(result);
        }
    }
}
void Connection::sendCommand(const QString &cmd, const QString &contextId,
                             const QVariantMap &arguments, CallbackBase* callback)
{
    QVariantMap data;
    data["context_id"] = contextId;
    data["type"] = "request";
    data["command"] = cmd;
    data["arguments"] = arguments;
    data["seq"] = ++m_lastSeqNr;

    QJson::Serializer serializer;
    const QByteArray out = serializer.serialize(data);

    if (callback) {
        m_callbacks[m_lastSeqNr] = callback;
    }
    
    if (!m_commandPending && m_socket) {
        Q_ASSERT(m_socket->isOpen());
        kDebug() << "sending" << out;
        sendData(out);
        m_commandPending = true;
    } else {
        kDebug() << "queueing" << out;
        m_commandQueue.enqueue(out);
    }
}

void Connection::sendData(const QByteArray& data)
{
    m_socket->write("Content-Length:"+QByteArray::number(data.length())+"\r\n");
    m_socket->write(data+"\r\n");
    m_socket->flush();
}

QTcpSocket* Connection::socket() {
    return m_socket;
}

}

#include "connection.moc"
