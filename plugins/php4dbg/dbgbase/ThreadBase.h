/***************************************************************************
                          ThreadBase.h  -  description
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

#ifndef _THREADBASE_H_
#define _THREADBASE_H_

#include <list>
#include <vector>

class ThreadBase;
class DbgSiteBase;

#include "dbg_core.h"
#include "ListenerGlobals.h"
#include "RequestorBase.h"

class ThreadBase : public RequestorBase {
private:
protected:
	int fsock;
	cfgprm fcfg;
	int flags;
	sockaddr fclient_addr;
	SCOPELIST scopelist;
	BREAKPOINTLIST breakpointlist;
	DOCLIST fdoclist;
	DbgSiteBase *fsite;

	virtual int ack(int cmd , dbg_packet *pack);
	virtual int process_cmd(int cmd, dbg_packet *pack, int client_flags);
	virtual DbgSiteBase* create_site() = 0;
	int request(dbg_header_struct *hdr, dbg_packet *pack);
	void submitbreakpoints(dbg_packet *pack);
public:
	ThreadBase(): fsock(0), flags(0), fsite(0) {
		DBGTRACE("ThreadBase::ThreadBase\n");
		memset(&fclient_addr, 0, sizeof(fclient_addr));
		memset(&fcfg, 0, sizeof(fcfg));
	}
	virtual ~ThreadBase() {}
	virtual bool initialize(int sock, sockaddr *client_addr, cfgprm *cfg);
	virtual void close();
	virtual void process();
	char isfinished();
	virtual void stop(bool) {} // shoud be realized with MultiThreaded listener
	virtual void destroy() {
		stop(true);
		delete this; // should be replaced with _Release() in COM case.
	}
	int sock() {return fsock;}

//////////////// RequestorBase PART ////////////////////
/* info */
	HRESULT getdbgversion(
			/* [out] */ long *dbg_ver,
			/* [out] */ std::string *sdbg_descr);
	HRESULT getserveraddr(
			/* [out] */ sockaddr_in *addr);
/* modules */
	HRESULT getdoclist(
			/* [in] */ DOCLIST *doclist);
    HRESULT requestdocsrc(
            /* [in] */  long mod_no,
            /* [out] */ std::string *mod_name,
            /* [out] */ std::string *src);
    HRESULT requestdocparent( 
            /* [in] */ long mod_no,
            /* [out] */ long *parent_mod_no);
    HRESULT requestdocfilename( 
            /* [in] */ long mod_no,
            /* [out] */ std::string *mod_name);
	HRESULT getlinesinfolist(
            /* [in] */ long mod_no,
			/* [in] */ LINESINFOLIST *linesinfolist);
/* scopes */
	HRESULT getscopelist(
			/* [in] */ SCOPELIST *ascopelist);
	HRESULT getcurloc(
			/* [out] */ long *mod_no,
			/* [out] */ long *line_no);
/* breakpoints */
	HRESULT setbreakpoint(
			/* [in] */ const BPOINTITEM bpm, 
			/* [in] */ const char *mod_name);
	HRESULT getbreakpointlist(
			/* [in] */ BREAKPOINTLIST *bplist);
/* evaluate */
	HRESULT evaluate(
			/* [in] */ const char *expr,
			/* [in] */ long scope_id,
			/* [out]*/ std::string *serial_str,
			/* [out]*/ std::string *err_str);
};

#endif
