
#include "listener.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <qstring.h>
#include <cstdarg>
 
#include "debugger.h"
#include "dbgbase/ThreadBase.h"
#include "thread.h"

Listener::Listener(PHP4Debugger *dbg)
  :ListenerBase()
{
  this->_dbg=dbg;
} 

ThreadBase* Listener::create_client(int sock, sockaddr *client_addr) 
{
  Thread *cl;
 
  cl = new Thread();
  if (!cl->initialize(sock, client_addr, &fcfg)) 
  {
    cl->destroy();
    cl = NULL;
  }
  return cl;
}

void Listener::log(ListenerLogKind kind, char *message,...) 
{
  va_list args;
  const int bufsize = 1024;
  char *buf = new char[bufsize];
 
  if (message) 
  {
    va_start(args, message);
    vsnprintf(buf, bufsize, message, args);
    va_end(args);
  }
  else {
    strcpy(buf," ");
  }
  
  QString mes;
  
  mes.sprintf("::log:");
  
  switch ((int)kind) {
    case 0: mes+="Message - ";break;
    case 1: mes+="Warning - ";break;
    case 2: mes+="Error - ";break;
  }
  mes+=buf;
  
  _dbg->viewMessage(mes);
  
  delete buf;
}
