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

#include "dbgpnetwork.h"

#include <kserversocket.h>
#include <kstreamsocket.h>
#include <kdebug.h>
#include <klocale.h>

DBGpNetwork::DBGpNetwork()
: QObject()
{
  m_socket = NULL;
  m_server = NULL;
  m_datalen = -1;
  m_transaction_id = 0;
}

DBGpNetwork::~DBGpNetwork()
{
}

void DBGpNetwork::sessionStart(bool useproxy, const QString& server, const QString & service)
{

  kdDebug(24002) << k_funcinfo << ", m_server: " << m_server << ", m_socket" << m_socket << endl;
  m_useproxy = useproxy;

  if(m_useproxy)
  {
    if(m_socket)
    {
//       m_socket->setBufferSize(-1);
      connect(m_socket, SIGNAL(gotError(int)), this, SLOT(slotError(int)));
      connect(m_socket, SIGNAL(connected(const KResolverEntry &)), this, SLOT(slotConnected(const KNetwork::KResolverEntry &)));
      connect(m_socket, SIGNAL(closed()), this, SLOT(slotConnectionClosed()));
      connect(m_socket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
      connect(m_socket, SIGNAL(destroyed()), this, SLOT(slotSocketDestroyed()));
      m_socket->connect();
      emit active(false);
      kdDebug(24002) << k_funcinfo << ", proxy:" << server << ", " << service << endl;
    }
  }
  else
  {
    if(!m_server)
    {
      m_server = new KNetwork::KServerSocket(service);

      m_server->setAddressReuseable(true);
      connect(m_server, SIGNAL(readyAccept()), this, SLOT(slotReadyAccept()));
      connect(m_server, SIGNAL(gotError(int)), this, SLOT(slotError(int)));

      if(m_server->listen())
      {
        emit active(true);
        emit networkError(i18n("Listening on port %1").arg(service), true);
      }
      else
      {
        delete m_server;
        m_server = NULL;
        emit active(false);
        emit networkError(i18n("Unable to listen on port %1").arg(service), true);
      }
    }
  }
}


void DBGpNetwork::sessionEnd()
{
  // Close socket
  if(m_socket)
  {
    m_socket->flush();
    disconnect(m_socket, SIGNAL(closed()), this, SLOT(slotConnectionClosed()));
    if (m_socket)
      m_socket->close();
    delete m_socket;
    m_socket = 0L;
  }

  // Close the server
  if(m_server)
  {
    m_server->close();
    delete m_server;
    m_server = NULL;
  }

  // Fake a connection closed signal
  slotConnectionClosed();
  emit active(false);
}


// Socket errors
void DBGpNetwork::slotError(int)
{
  kdDebug(24002) << k_funcinfo << ", m_server: " << m_server << ", m_socket" << m_socket << endl;
  if(m_socket)
  {
    kdDebug(24002) << k_funcinfo << ", " << m_socket->errorString() << endl;
    if(m_socket->error() == KNetwork::KSocketBase::RemotelyDisconnected)
    {
       slotConnectionClosed();
       emit networkError(i18n("Disconnected from remote host"), true);
      return;
    }

    if(m_socket->error())
    {      
      emit networkError(m_socket->errorString(), true);
    }
  }

  if(m_server && m_server->error())
  {
    kdDebug(24002) << k_funcinfo << ", " << m_server->errorString() << endl;
    emit networkError(m_server->errorString(), true);
  }
}

// slotReadyAccept
void DBGpNetwork::slotReadyAccept()
{

  kdDebug(24002) << k_funcinfo << ", m_server: " << m_server << ", m_socket" << m_socket << endl;
  if(!m_socket)
  {
    disconnect(m_server, SIGNAL(readyAccept()), this, SLOT(slotReadyAccept()));

    m_socket = (KNetwork::KStreamSocket *)m_server->accept(); // KSocketServer returns a KStreamSocket (!)
    if(m_socket)
    {
      kdDebug(24002) << k_funcinfo << ", ready" << ", m_socket" << m_socket << endl;
      m_socket->enableRead(true);
      m_socket->setAddressReuseable(true);
//       m_socket->setSocketFlags(KExtendedSocket::inetSocket |  KExtendedSocket::inputBufferedSocket);
//       m_socket->setBufferSize(-1);
      connect(m_socket, SIGNAL(gotError(int)), this, SLOT(slotError(int)));
      connect(m_socket, SIGNAL(connected(const KResolverEntry &)), this, SLOT(slotConnected(const KResolverEntry &)));
      connect(m_socket, SIGNAL(closed()), this, SLOT(slotConnectionClosed()));
      connect(m_socket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
      connected();
    }
    else
    {
      kdDebug(24002) << k_funcinfo << ", " << m_server->errorString() << endl;
    }
  }

}

// Connection established
void DBGpNetwork::slotConnected(const KResolverEntry &)
{
  connected();
}

bool DBGpNetwork::isConnected()
{
  return  m_socket != NULL && m_socket->state() == KNetwork::KClientSocketBase::Connected;
}

bool DBGpNetwork::isActive()
{
  return
       (m_socket != NULL && m_socket->state() == KNetwork::KClientSocketBase::Connected) 
    || (m_server != NULL);
}

void DBGpNetwork::connected()
{

  kdDebug(24002) << k_funcinfo << endl;
  emit connected(true);
//   debuggerInterface()->enableAction("debug_disconnect", true);
//   debuggerInterface()->enableAction("debug_request", false);
}

// Connectio closed
void DBGpNetwork::slotConnectionClosed()
{
  kdDebug(24002) << k_funcinfo << ", m_server: " << m_server << ", m_socket" << m_socket << endl;

  // Check if we have more data to read
  slotReadyRead();
//   kdDebug(24002) << k_funcinfo << "buffer: " << m_buffer << endl;

  if(m_socket)
  {
    m_socket->flush();
    m_socket->close();
    delete m_socket;
    m_socket = NULL;
  }

  if(m_server)
    connect(m_server, SIGNAL(readyAccept()), this, SLOT(slotReadyAccept()));

  // Disable all session related actions and enable connection action
  emit connected(false);
  emit active(m_server != NULL);

}


//called when m_socket is destroyed either by deleting it or if XDebug disconnects from the client
void DBGpNetwork::slotSocketDestroyed()
{
  kdDebug(24002) << k_funcinfo << " , m_server: " << m_server << ", m_socket" << m_socket << endl;

  m_socket = NULL; //m_socket is already wrong, without this the app would crash on the next m_socket->close() or delete m_socket call.
  slotConnectionClosed();
}


// Data from socket
void DBGpNetwork::slotReadyRead()
{

  // Data from dbgp  
  while(m_socket && (m_socket->bytesAvailable() > 0 || m_fifo.length() >= (unsigned long)m_datalen))
  {
    int bytes;
    QString data;

    if(m_socket && m_socket->bytesAvailable() > 0 )
    {
      // Read all available bytes from socket and append them to the buffer
      bytes = m_socket->bytesAvailable();
      char* buffer = new char[bytes];
      m_socket->readBlock(buffer, bytes);

      // Put it in the fifo buffer
      m_fifo.append(buffer, bytes);

      delete[] buffer;
    }

    while(1)
    {
      // If datalen == -1, we didnt read the size yet, otherwise we're reading data.
      if(m_datalen == -1)
      {
        bytes = m_fifo.find('\0');
        if(bytes < 0)
          break;

        data = m_fifo.retrieve();
        m_datalen = data.toLong();

      }
      if(m_datalen != -1 && (long)m_fifo.length() >= m_datalen + 1)
      {
        data = m_fifo.retrieve();
        m_datalen = -1;
        emit command(data);
      }
      else
        break;
    }
  }
}

long DBGpNetwork::sendCommand(const QString & command)
{
  return sendCommand(command, "");
}

long DBGpNetwork::sendCommand(const QString & command, const QString & arguments)
{
  if(!isConnected())
    return false;

  m_transaction_id++;
  QString commandline = command + QString(" -i %1").arg(m_transaction_id) + (!arguments.isEmpty() ? " " : "") + arguments;

  kdDebug(24002) << k_funcinfo << ", sending: " << commandline << endl;

  m_socket->writeBlock(commandline.latin1(), commandline.length() + 1); // Send string + NULL termination

  return m_transaction_id;
}

long DBGpNetwork::sendCommand( const QString & command, const QString & arguments, const QString & data )
{
  QByteArrayFifo buffer;
  buffer.append(data.ascii(), data.length());
  return sendCommand(command, arguments + " -- " + buffer.base64Encoded());
}

// #include "dbgpnetwork.moc"

#include "dbgpnetwork.moc"
