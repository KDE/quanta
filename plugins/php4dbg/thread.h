#ifndef THREAD_H
#define THREAD_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "debugger.h"
#include "dbgbase/ListenerGlobals.h"
#include "dbgbase/ThreadBase.h"
 
class Thread: public ThreadBase {
protected:
  DbgSiteBase* create_site();
  
public:
  void stop(char bWait);
  void close();
  
  PHP4Debugger *_dbg;
};

#endif

