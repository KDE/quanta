
#include "thread.h"
#include "idesite.h"
#include "debugger.h"
#include "listener.h"
#include <qsocketnotifier.h>

PHP4Debugger::PHP4Debugger( QObject * parent=0, const char * name=0 )
  :QObject(parent,name)
{
}

void PHP4Debugger::init()
{
  cfgprm cfg = {0,0,0,0,0};
  memset(&cfg, 0, sizeof(cfg));
  cfg.breakonload       = DEFAULT_LSTNR_BREAKONSTART;
  cfg.port              = DEFAULT_LSTNR_PORT;
  cfg.timeoutms_client  = DEFAULT_LSTNR_TIMEOUT;
  cfg.timeoutms_process = 100; /* 100ms */
  
  listener = new Listener(this);
  
  if (!listener->start(&cfg,NULL)) 
  {
    listener->logstatus();
    return;
  }
  
  listener->logstatus();
  
  int sock = listener->socket();
  
  sn = new QSocketNotifier( sock, QSocketNotifier::Read );
  QObject::connect(sn,SIGNAL(activated(int)),this,SLOT(activated(int)));
}

PHP4Debugger::~PHP4Debugger()
{
  listener->stop(true);
  listener->logstatus();
  
  delete listener;
}

void PHP4Debugger::activated(int)
{
  QObject::disconnect(sn,SIGNAL(activated(int)),this,SLOT(activated(int)));
  
  if (listener->numberofclients() <= 0) listener->peek();
  if (listener->numberofclients() > 0 ) 
  {
      Thread* cl = (Thread*)(listener->client(0));
      cl->_dbg = this;
      while (true) {
        cl->process();
        if (cl->isfinished()) break;
      }
  }
  if (listener->error()) listener->logstatus();
  
  QObject::connect(sn,SIGNAL(activated(int)),this,SLOT(activated(int)));
}

void PHP4Debugger::viewMessage(QString mes)
{
  emit message(mes);
}
