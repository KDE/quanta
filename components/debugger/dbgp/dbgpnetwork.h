/*
 * Copyright (C) 2005 by Linus McCabe, <Linus@McCabe.nu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef dbgpnetwork_H
#define dbgpnetwork_H

#include <kserversocket.h>
#include <kstreamsocket.h>

#include "qbytearrayfifo.h"

//using namespace KNetwork;
class KResolverEntry;

class DBGpNetwork : public QObject
{
    Q_OBJECT

  private:
    KNetwork::KStreamSocket *m_socket;
    KNetwork::KServerSocket *m_server;
    QByteArrayFifo m_fifo;
    bool m_useproxy;
    long m_datalen;
    long  m_transaction_id;

    void connected();

   signals:
    void active(bool isOnline);
    void connected(bool isConnected);
    void networkError(const QString &error, bool log);
    void command(const QString& data);

  public:
    DBGpNetwork();
    ~DBGpNetwork();

    virtual void sessionStart(bool useproxy, const QString& server, const QString & service);
    virtual void sessionEnd();

    bool isConnected();
    bool isActive();

    long sendCommand(const QString & command);
    long sendCommand(const QString & command, const QString & arguments);
    long sendCommand(const QString & command, const QString & arguments, const QString & data);

  public slots:
    // Socket slots
    void slotConnected(const KResolverEntry &);
    void slotConnectionClosed();
    void slotError(int);
    void slotReadyRead();
    void slotReadyAccept();
    void slotSocketDestroyed();
};


#endif // dbgpnetwork_H
