#ifndef PHPDEBUGSERVERSOCKET_H
#define PHPDEBUGSERVERSOCKET_H

#include <qserversocket.h>


class PhpDebugServerSocket : public QServerSocket  {
  Q_OBJECT
  public:
    PhpDebugServerSocket( int port, QObject *parent=0, const char *name=0 );

    virtual void newConnection( int socket );
  
  public slots:
    void readClient();
    void discardClient();
      
  signals:
    void newConnect();
    void endConnect();
    void data( QString );
    void wroteToClient();
};

#endif
