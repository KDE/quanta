/***************************************************************************
                          DbgSite.h  -  description
                             -------------------
    begin                : Fri Mar 2 2001
    copyright            : (C) 2001 by Dmitri Dmitrienko
    email                : dd@cron.ru
    www                  : http://dd.cron.ru
    license              : This source file is subject to version 1.00 of 
                           the License,  that is bundled with this package 
                           in the file LICENSE, and is available at through 
                           the world-wide-web at http://dd.cron.ru/license
 ***************************************************************************/

#ifndef _DBGSITE_H_
#define _DBGSITE_H_

#include <qobject.h>
#include "ListenerGlobals.h"
#include "RequestorBase.h"

class DbgSiteBase : public QObject {
  Q_OBJECT
private:
protected:
public:
	DbgSiteBase():QObject() {}
	virtual ~DbgSiteBase() {}
	virtual HRESULT initialize(RequestorBase *req, int dbg_ver, char *dbg_descr) = 0; 
	virtual HRESULT handleerror(int errtype, char *errmsg, BRACTION *action) = 0;  
	virtual HRESULT handlebreakpoint(BRREASON reason, BRACTION *action) = 0;
	virtual HRESULT close() = 0;
	virtual HRESULT isalive() {return S_OK;}
};

#endif
