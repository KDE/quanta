#ifndef DEBUGGER_H
#define DEBUGGER_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qobject.h>

class Listener;
class QSocketNotifier;

class PHP4Debugger : public QObject {
  Q_OBJECT

public:  
  PHP4Debugger( QObject * parent=0, const char * name=0 );
  ~PHP4Debugger();
  
  void init();
  
public slots:
  void activated(int);
  void viewMessage(QString);
  
signals:
  void message(QString);
  
private:
  Listener *listener;
  QSocketNotifier *sn; 
};

#endif

