#ifndef IDESITE_H
#define IDESITE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "thread.h"
#include "dbgbase/DbgSite.h"

class IdeSite : public DbgSiteBase {
  Q_OBJECT
private:
  RequestorBase *requestor;

public:
  IdeSite() : requestor(NULL) {};

  HRESULT initialize(RequestorBase *req, int dbg_ver, char *dbg_descr);
  HRESULT handleerror(int errtype, char *errmsg, BRACTION *action);
  HRESULT handlebreakpoint(BRREASON reason, BRACTION *action);
  HRESULT close();

  void print_error(QString);
  void print_location();

signals:
  void message(QString);

};

#endif

