#ifndef PHPDEBUGGUBED_H
#define PHPDEBUGGUBED_H

#include <qserversocket.h>
#include <qsocket.h>
#include <qptrlist.h>
#include <kurl.h>

#include "phpdebugsocket.h"

class PHPDebugGubed : public PHPDebugSocket {
  Q_OBJECT
  
  public:
    PHPDebugGubed(PHPDebuggerInterface *);
    const uint getCapabilities();
    
    // Execution control
    bool run();
    bool leap();
    bool skip();
    bool stepInto();
    bool stepOver();
    bool stepOut();
    bool pause();
    bool kill();
    
    // Connection
    bool tryConnect();
    bool disConnect();
    
    // Return name of debugger
    QString getName();
    
    // New file opened in quanta
    void fileOpened(KURL file);
    
    // Breakpoints
    bool setBreakpoint ( QString file, int line);
    bool removeBreakpoint ( QString file, int line );
  
  private:
    QSocket *m_socket;
    QString m_command;
    
    bool sendCommand(QString, QString);
    void debuggingState(bool enable);
    
  public slots:
    // Socket slots
    void slotConnected();
    void slotConnectionClosed();
    void slotError(int);
    void slotReadyRead();

  signals:

};

#endif
