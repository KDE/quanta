#ifndef PHP3Debugger_H
#define PHP3Debugger_H

#include <qserversocket.h>


class PHP3Debugger : public QServerSocket  {
  Q_OBJECT
  public:
    PHP3Debugger( int port, QObject *parent=0, const char *name=0 );

    virtual void newConnection( int socket );

  public slots:
    void readClient();
    void discardClient();

  signals:
    void newConnect();
    void endConnect();
    void data(const QString& );
    void wroteToClient();
};

#endif
