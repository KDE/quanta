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

#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtCore/QObject>
#include <QtNetwork/QAbstractSocket>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtCore/QQueue>

class QTcpSocket;

namespace Crossfire {
class StackModel;

class CallbackBase {
public:
    virtual void execute(const QVariantMap &) = 0;
    virtual ~CallbackBase() {};
};

template<class Handler, class Cookie>
class CallbackWithCookie : public CallbackBase {
public:
    CallbackWithCookie(Handler* scope, void (Handler::*method)(Cookie*, const QVariantMap &), Cookie* cookie = 0)
        : m_cookie(cookie), m_scope(scope), m_method(method)
    {}

    virtual void execute(const QVariantMap & data)
    {
        return (m_scope->*m_method)(m_cookie, data);
    }
    
private:
    Cookie* m_cookie;
    Handler* m_scope;
    void (Handler::*m_method)(Cookie*, const QVariantMap &);
};

template<class Handler>
class Callback : public CallbackBase {
public:
    Callback(Handler* scope, void (Handler::*method)(const QVariantMap &))
        : m_scope(scope), m_method(method)
    {}

    virtual void execute(const QVariantMap & data)
    {
        return (m_scope->*m_method)(data);
    }

private:
    Handler* m_scope;
    void (Handler::*m_method)(const QVariantMap &);
};

class Connection : public QObject
{
    Q_OBJECT
public:
    Connection(QTcpSocket* socket, QObject * parent = 0);
    ~Connection();

    bool waitForHandshake(int timeout = 30000);

    void close();

    void sendCommand(const QString &cmd, const QString &contextId = QString(),
                     const QVariantMap &arguments = QVariantMap(),
                     CallbackBase* callback = 0);

    QTcpSocket* socket();

Q_SIGNALS:
    void outputLine(QString content);

private Q_SLOTS:
    void closed();
    void readyRead();
    void error(QAbstractSocket::SocketError error);

private:
    void processResponse(const QVariant &data);
    void sendData(const QByteArray &data);

    QTcpSocket* m_socket;
    bool m_handsShaked;

    QQueue<QByteArray> m_commandQueue;
    bool m_commandPending;
    int m_lastSeqNr;
    QMap<int, CallbackBase*> m_callbacks;
};

}
#endif
