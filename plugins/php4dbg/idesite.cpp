
#include <string>
#include <assert.h>
#include <stdio.h>
 
#include <stdlib.h>
#include <iostream.h>
 
#include "idesite.h"
#include "listener.h"

#include "idesite.h"

HRESULT IdeSite::initialize(RequestorBase *req, int dbg_ver, char *dbg_descr)
{
  requestor = req;
  
  sockaddr_in addr;
  requestor->getserveraddr(&addr);
  
  QString mes = "::log:Start session on ";
  mes+=inet_ntoa(addr.sin_addr);
  emit message(mes);
  
  return S_OK;
}

HRESULT IdeSite::handleerror(int errtype, char *errmsg, BRACTION *action)
{
  assert(requestor!=NULL);
  
  long mod_no = 0,line_no = 0;
  std::string mod_name;
  HRESULT hr;
 
  hr = requestor->getcurloc(&mod_no,&line_no);
  if (SUCCEEDED(hr)) hr = requestor->requestdocfilename(mod_no, &mod_name);
 
  QString t;
  QString mes;
  
  if (SUCCEEDED(hr)) 
  {
    mes = mod_name.data();
    t.sprintf(":%d:",line_no);
    mes += t;
  }
  else mes+="::";
  mes+="error:";
  mes+=errmsg;
  
  emit message(mes);
  
  *action=BR_CONTINUE;
  
  return S_OK;
}

HRESULT IdeSite::handlebreakpoint(BRREASON reason, BRACTION *action)
{
  assert(requestor!=NULL);
  
  long mod_no = 0,line_no = 0;
  std::string mod_name;
  HRESULT hr;
 
  hr = requestor->getcurloc(&mod_no,&line_no);
  if (SUCCEEDED(hr)) hr = requestor->requestdocfilename(mod_no, &mod_name);
 
  QString t;
  QString mes;
  
  if (SUCCEEDED(hr)) 
  {
    mes = mod_name.data();
    t.sprintf(":%d:",line_no);
    mes += t;
  }
  else mes+="::";
  mes+="breakpoint:";
  
  t.sprintf("%s=%d:","reason",reason);
  mes+=t;
  
  emit message(mes);
  
  *action=BR_CONTINUE;
  
  return S_OK;
}

HRESULT IdeSite::close()
{
  if (requestor==NULL) return S_OK;
  
  sockaddr_in addr;
  requestor->getserveraddr(&addr);
  
  QString mes = "::log:Stop session on ";
  mes+=inet_ntoa(addr.sin_addr);
  emit message(mes);
  
  requestor = NULL;
  return S_OK;
}

void IdeSite::print_location()
{
  long mod_no = 0,line_no = 0;
  std::string mod_name;
  HRESULT hr;
 
  hr = requestor->getcurloc(&mod_no,&line_no);
  if (SUCCEEDED(hr)) hr = requestor->requestdocfilename(mod_no, &mod_name);
 
  if (SUCCEEDED(hr)) 
  {
    QString mes = "\n:LOCATION:";
    mes += mod_name.data();
    QString t;
    t.sprintf(":%s=%d","mod_no",mod_no);
    mes += t;
    t.sprintf(":%s=%d:","line_no",line_no);
    mes += t;
    
    emit message(mes);
  } 
  else print_error("Failed to get current location");
}

void IdeSite::print_error(char *msg)
{
  QString mes = "\n:ERROR:";
  mes += msg;
  
  emit message(mes);
}
