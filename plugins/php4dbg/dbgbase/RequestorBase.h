/***************************************************************************
                          RequestorBase.h  -  description
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

#ifndef _REQUESTORBASE_H_
#define _REQUESTORBASE_H_

#include <string>

#include "ListenerGlobals.h"

class RequestorBase {
private:
protected:
public:
	virtual ~RequestorBase() {}
/* info */
	virtual HRESULT getdbgversion(
			/* [out] */ long *dbg_ver,
			/* [out] */ std::string *sdbg_descr) = 0;
	virtual HRESULT getserveraddr(
			/* [out] */ sockaddr_in *addr) = 0;
/* modules */
    virtual HRESULT requestdocsrc(
            /* [in] */  long mod_no,
            /* [out] */ std::string *mod_name,
            /* [out] */ std::string *src) = 0;
    virtual HRESULT requestdocparent( 
            /* [in] */ long mod_no,
            /* [out] */ long *parent_mod_no) = 0;
    virtual HRESULT requestdocfilename( 
            /* [in] */ long mod_no,
            /* [out] */ std::string *mod_name) = 0;
	virtual HRESULT getdoclist(
			/* [in] */ DOCLIST *doclist) = 0;
	virtual HRESULT getlinesinfolist(
            /* [in] */ long mod_no,
			/* [in] */ LINESINFOLIST *linesinfolist) = 0;
/* scopes */
	virtual HRESULT getscopelist(
			/* [in] */ SCOPELIST *ascopelist) = 0; 
	virtual HRESULT getcurloc(
			/* [out] */ long *mod_no,
			/* [out] */ long *line_cookie) = 0;
/* breakpoints */
	virtual HRESULT setbreakpoint(
			/* [in] */ const BPOINTITEM bpm, 
			/* [in] */ const char *mod_name) = 0;
	virtual HRESULT getbreakpointlist(
			/* [in] */ BREAKPOINTLIST *bplist) = 0;
/* evaluate */
	virtual HRESULT evaluate(
			/* [in] */ const char *expr,
			/* [in] */ long scope_id,
			/* [out]*/ std::string *serial_str,
			/* [out]*/ std::string *err_str) = 0;

};

#endif

