#ifndef LISTENER_H
#define LISTENER_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "dbgbase/ListenerBase.h"

class PHP4Debugger;

class Listener: public ListenerBase {
public:
  Listener(PHP4Debugger *dbg);
  ~Listener(){}
        
  int socket() { return listensock; }
  TH_LIST* getthlist() { return &th_list; }
  void log(ListenerLogKind kind, char *message,...);
  void destroy_client_p(ThreadBase *client) { client->destroy(); }
  
protected:
  ThreadBase* create_client(int sock, sockaddr *client_addr);
  PHP4Debugger *_dbg;
};

#endif

