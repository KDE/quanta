
#include "thread.h"

#include "ListenerGlobals.h"
#include "idesite.h"
 
void Thread::stop(char bWait) {
}
 
DbgSiteBase* Thread::create_site() 
{
  IdeSite* site = new IdeSite();
  
  QObject::connect(site,SIGNAL(message(QString)),
                   _dbg,SLOT(viewMessage(QString)));
  
  return site;
}
 
void Thread::close() 
{
  IdeSite* site = (IdeSite*)fsite;
  QObject::disconnect(site,SIGNAL(message(QString)),
                      _dbg,SLOT(viewMessage(QString)));
  ThreadBase::close();
  flags|=DBGF_FINISHED;
}
