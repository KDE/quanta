
#include "thread.h"

#include "ListenerGlobals.h"
#include "idesite.h"

void Thread::stop(char /*bWait*/) {
}

DbgSiteBase* Thread::create_site()
{
  IdeSite* site = new IdeSite();

  QObject::connect(site,SIGNAL(message(const QString&)),
                   _dbg,SLOT(viewMessage(const QString&)));

  return site;
}

void Thread::close()
{
  IdeSite* site = (IdeSite*)fsite;

  if (fsite) {
    fsite->close();
    QObject::disconnect(site,SIGNAL(message(const QString&)),
                        _dbg,SLOT(viewMessage(const QString&)));
    delete fsite;
    fsite = NULL;
  }

  ThreadBase::close();
  flags|=DBGF_FINISHED;
}
