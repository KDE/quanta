
// qt includes
#include <qmessagebox.h>
#include <qserversocket.h>

// app includes
#include "debugger.h"


PHP3Debugger::PHP3Debugger( int port, QObject *parent, const char *name )
  :QServerSocket( port, 0, parent, name )
{
}

void PHP3Debugger::newConnection( int socket )
{
  debug("PHP debugger connected...");
  
  QSocket* s = new QSocket(this);
	connect(s,SIGNAL(readyRead()),           this,SLOT(readClient()));
	connect(s,SIGNAL(delayedCloseFinished()),this,SLOT(discardClient()));
	s->setSocket(socket);
	emit newConnect();
}

void PHP3Debugger::readClient()
{
  QSocket* socket = (QSocket*)sender();
	while (socket->canReadLine()) 
	{
	  QString s = socket->readLine();
	  emit data( s );
	}
}

void PHP3Debugger::discardClient()
{
  QSocket* socket = (QSocket*)sender();
	delete socket;
	emit endConnect();
}
