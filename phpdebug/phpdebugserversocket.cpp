
// qt includes
#include <qmessagebox.h>
#include <qserversocket.h>

// app includes
#include "phpdebugserversocket.h"


PhpDebugServerSocket::PhpDebugServerSocket( int port, QObject *parent, const char *name )
  :QServerSocket( port, 0, parent, name )
{
  if ( !ok() ) {
    QString s;
	  QMessageBox::critical( 0, "Php Debugger",
                           "Sorry, but I can't listen port N "+s.sprintf("%i",port) );
	}
}

void PhpDebugServerSocket::newConnection( int socket )
{
  debug("\nConnect...\n\n");
  
  QSocket* s = new QSocket(this);
	connect(s,SIGNAL(readyRead()),           this,SLOT(readClient()));
	connect(s,SIGNAL(delayedCloseFinished()),this,SLOT(discardClient()));
	s->setSocket(socket);
	emit newConnect();
}

void PhpDebugServerSocket::readClient()
{
  QSocket* socket = (QSocket*)sender();
	if (socket->canReadLine()) 
	{
	  QString s = socket->readLine();
	  emit data( s );
	}
}

void PhpDebugServerSocket::discardClient()
{
  QSocket* socket = (QSocket*)sender();
	delete socket;
	emit endConnect();
}
