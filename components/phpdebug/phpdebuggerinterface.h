#ifndef PHPDebuggerInterface_H
#define PHPDebuggerInterface_H

#include "phpdebugsocket.h"
#include "messageoutput.h"
#include <qobject.h>
#include <qstring.h>

class PHPDebuggerInterface : public QObject {
  Q_OBJECT

  private:
    PHPDebugSocket *m_debugsocket;

    // Keep track of current position
    QString m_currentFile;
    long m_currentLine;

    // Internal help functions
    void setMark(QString, long, bool, int);
    QString mapServerPathToLocal(QString serverpath);
    QString mapLocalPathToServer(QString localpath);
    
  public:
    PHPDebuggerInterface();

    // Breakpoints
    bool setBreakpoint ( KURL file, int line);
    void haveBreakpoint (QString file, int line);
    bool removeBreakpoint ( KURL file, int line );
    
    // Public help functions
    bool showStatus(QString message, bool log);
    bool setActiveLine (KURL file, int line);
    void fileOpened(KURL file);
  
    void enableAction(QString action, bool enable);
  
  public slots:
    /** Execution control slots **/
    void slotDebugRun();
    void slotDebugLeap();
    void slotDebugSkip();
    void slotDebugStepOver();
    void slotDebugStepInto();
    void slotDebugStepOut();
    void slotDebugPause();
    void slotDebugKill();
    
    // Connection related slots
    void slotDebugConnect();
    void slotDebugDisconnect();    
};

#endif

