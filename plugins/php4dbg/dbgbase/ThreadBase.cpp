/***************************************************************************
                          ThreadBase.cpp  -  description
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

#include "ThreadBase.h"
#include "ListenerGlobals.h"
#include "dbg_core.h"
#include <stdio.h>
#include "DbgSite.h"

bool ThreadBase::initialize(int sock, sockaddr *client_addr, cfgprm *cfg) {
	fsock = sock;
	fcfg = *cfg;
	fclient_addr = *client_addr;
	return true;
}

void ThreadBase::close() {
	if (fsock) {
		dbh_close_socket(fsock);
		fsock = 0;
	}
	scopelist.clear();
	breakpointlist.clear();
	if (fsite) {
		fsite->close();
		delete fsite;
		fsite = NULL;
	}
	flags &= ~DBGF_WAITACK;
}

int ThreadBase::ack(int cmd, dbg_packet *pack) {
	int ret_val;
	bool dofree = (pack == NULL);
	dbg_packet thepack;

	if (dofree) {
		pack = &thepack;
		dbg_packet_new(pack);
	}
	if (cmd == DBGA_REQUEST) 
		flags |= DBGF_WAITACK;
	else {
		flags &= ~DBGF_WAITACK;
		submitbreakpoints(pack);
		fdoclist.clear();
	}
	ret_val = dbh_ack(cmd, pack, fsock, flags);
	if (dofree) dbg_packet_free(pack);
	return ret_val;
}

int ThreadBase::process_cmd(int cmd, dbg_packet *pack, int client_flags) {
	dbg_frame *p;
	dbg_error_body *err;
	char *str;
	bool is_error;	
	char *errmsg;
	BRREASON reason;
	int ret_val = 0, ret_cmd, siz;
	BRACTION action = BR_CONTINUE;
	HRESULT hr;

	is_error = cmd == DBGC_ERROR;
	switch (cmd) {
		case DBGC_STARTUP:
			reason = (fcfg.breakonload)? BR_BREAKPOINT : (BRREASON)-1;
			break;
		case DBGC_END:			
			reason = BR_BREAKPOINT;
			break;
		case DBGC_BREAKPOINT:
			reason = BR_EMBEDDED;
			break;
		case DBGC_STEPINTO_DONE:
			reason = BR_STEPINTO;
			break;
		case DBGC_STEPOVER_DONE:
			reason = BR_STEPOVER;
			break;
		case DBGC_STEPOUT_DONE:
			reason = BR_STEPOUT;
			break;
		case DBGC_ERROR:
			reason = (BRREASON)-2;
			break;
		default:
			reason = (BRREASON)-1;
	}
	if (reason == (BRREASON)-1) return 0;

	scopelist.assign(pack);

	if (!(flags & DBGF_STARTED) && cmd != DBGC_END) {
		dbg_version_body *body;
		dbg_frame *frame;
		long dbg_ver = 0x120;
		char *dbg_descr = NULL;
		int sz;
		std::string sdbg_descr;
		

		flags |= DBGF_STARTED;

		frame = dbg_packet_findfirstframe(pack,FRAME_VER);
		if (frame!=NULL) {
			body = (dbg_version_body*)FRAME_DATA_PTR(frame);
			dbg_ver = ((body->major_version & 0xFF) << 8) | (body->minor_version & 0xFF);
			dbg_packet_findstring(pack, body->idescription, &dbg_descr, &sz);
		} else {
			if (SUCCEEDED(getdbgversion(&dbg_ver, &sdbg_descr))) {
				dbg_descr = &sdbg_descr[0];
			}
		}

		if (fsite == NULL) {
			fsite = create_site();
			hr = fsite ? (fsite->initialize(this, dbg_ver, dbg_descr)) : E_FAIL;
			if (FAILED(hr)) {
				flags &= ~DBGF_STARTED;
				if (fsite) {
					fsite->close();
					delete fsite;
					fsite = NULL;
				}
				ack(DBGA_STOP, NULL);
				close();
				return 0;
			}
		}
	}
	if (!(flags & DBGF_STARTED) || fsite == NULL) return 0;
	
	if (is_error) {
		p = dbg_packet_findfirstframe(pack, FRAME_ERROR);
		if (!p) return 0;
		err = (dbg_error_body *)FRAME_DATA_PTR(p);
		errmsg = NULL;
		if (dbg_packet_findstring(pack, err->imessage, &str, &siz)) {
			errmsg = str;
		}
		hr = fsite->handleerror(err->type, errmsg, &action);
	}
	else {
		hr = fsite->handlebreakpoint(reason, &action);
	}
	if (!SUCCEEDED(hr))
		ret_cmd = DBGA_STOP;
	else {
		switch (action) {
			case BR_ABORT:
				ret_cmd = DBGA_STOP;
				break;
			case BR_STEP_INTO:
				ret_cmd = DBGA_STEPINTO;
				break;
			case BR_STEP_OVER:
				ret_cmd = DBGA_STEPOVER;
				break;
			case BR_STEP_OUT:
				ret_cmd = DBGA_STEPOUT;
				break;
			default:
				ret_cmd = DBGA_CONTINUE;
		}
	}	
	ret_val = 0;
	if (ret_cmd) {
		ret_val = ack(ret_cmd, NULL);
		ret_val = (ret_val) ? 1:0;
	}
	if (cmd == DBGC_END) {
		flags &= ~DBGF_STARTED;
	}
	return ret_val;
}

void ThreadBase::process() {
	dbg_packet pack;
	int ret_val = 0;
	dbg_header_struct hdr;
	
	if (flags & DBGF_WAITACK) return;

	dbg_packet_new(&pack);
	ret_val = dbh_wait_debug_event(&hdr, fsock, &pack, fcfg.timeoutms_client);
	if (ret_val > 0) {
		if (hdr.flags & DBGF_WAITACK!=0) 
			flags |= DBGF_WAITACK;
		ret_val = process_cmd(hdr.cmd, &pack, hdr.flags);
		if (!ret_val && flags & DBGF_WAITACK) {
			ack(DBGA_CONTINUE, NULL);
		}
	}
	else if (ret_val < 0) {
		close();
	}
	dbg_packet_free(&pack);
}

char ThreadBase::isfinished() {
	return ((flags & DBGF_FINISHED)!=0);
}

int ThreadBase::request(dbg_header_struct *hdr, dbg_packet *pack) {
	int ret_val;

	if (!(flags & DBGF_WAITACK)) {
		memset(hdr, 0, sizeof(dbg_header_struct));
		dbg_packet_clear(pack);
		return 0;
	}

	ret_val = dbh_ack(DBGA_REQUEST, pack, fsock, DBGF_WAITACK);
	if (ret_val > 0) {
		ret_val = dbg_packet_recv(hdr, pack, fsock, fcfg.timeoutms_client);
	} else {
		dbg_packet_clear(pack);
		ret_val = 0;
	}
	return ret_val;
}

void ThreadBase::submitbreakpoints(dbg_packet *pack) {
	dbg_bps_request bps;
	BREAKPOINTLIST::iterator it;

	if (breakpointlist.size() == 0) return;
	DBGTRACE("ThreadBase::submitbreakpoints\n");	

	for (it = breakpointlist.begin(); it != breakpointlist.end(); it++) {		
		bps.line_no = it->line_no;
		bps.mod_no = it->mod_no;
		bps.status = (int)(it->state);
		bps.imod_name = dbg_packet_add_string(pack, it->mod_name);

		dbg_packet_add_frame(pack, FRAME_BPS, &bps, sizeof(bps));
	}
	breakpointlist.clear();
}


//////////////// RequestorBase PART ////////////////////


HRESULT ThreadBase::getdbgversion(
	/* [out] */ long *dbg_ver,
	/* [out] */ std::string *sdbg_descr) {
	dbg_packet pack;
	dbg_header_struct hdr;
	int ret_val, sz;
	dbg_version_body lstnr_ver, *body;
	dbg_frame *frame;
	char *dbg_descr;
	HRESULT hr;

	DBGTRACE("ThreadBase::getdbgversion\n");
	if (!sdbg_descr && !dbg_ver) return E_POINTER;

	if (!(flags & DBGF_WAITACK)) return E_FAIL;

	*sdbg_descr = "";
	*dbg_ver = 0x120;

	dbg_packet_new(&pack);

	lstnr_ver.major_version = DBG_API_MAJOR_VESION;
	lstnr_ver.minor_version = DBG_API_MINOR_VESION;
	lstnr_ver.idescription = dbg_packet_add_string(&pack, DBG_API_DESCRIPTION);
	dbg_packet_add_frame(&pack, FRAME_VER, &lstnr_ver, sizeof(lstnr_ver));
	
	hr = E_FAIL;
	ret_val = request(&hdr, &pack);
	if (ret_val) {
		frame = dbg_packet_findfirstframe(&pack,FRAME_VER);
		if (frame!=NULL) {
			body = (dbg_version_body*)FRAME_DATA_PTR(frame);
			*dbg_ver = ((body->major_version & 0xFF) << 8) | (body->minor_version & 0xFF);
			if (dbg_packet_findstring(&pack, body->idescription, &dbg_descr, &sz)) {
				*sdbg_descr = dbg_descr;
			}
			hr = S_OK;
		}
	}

	dbg_packet_free(&pack);
	return hr;
}

HRESULT ThreadBase::getserveraddr(
		/* [out] */ sockaddr_in *addr) {
	if (!addr) return E_POINTER;
	*addr = *(sockaddr_in *) &fclient_addr;
	return S_OK;
}

HRESULT ThreadBase::getdoclist(
		/* [in] */ DOCLIST *doclist) {
	dbg_packet pack;
	dbg_header_struct hdr;
	int ret_val;

	if (!doclist) return E_POINTER;

	DBGTRACE("ThreadBase::getdoclist\n");
	if (fdoclist.size()==0) {
		if (!(flags & DBGF_WAITACK)) return E_FAIL;
		
		fdoclist.clear();
		dbg_packet_new(&pack);
		dbg_packet_add_frame(&pack, FRAME_SRC_TREE, NULL, 0);
		ret_val = request(&hdr, &pack);
		if (ret_val>0) {
			fdoclist.assign(&pack);
		}
		dbg_packet_free(&pack);
	};
	if (fdoclist.size()==0) return E_FAIL;
	if (doclist!=&fdoclist) doclist->assign(&fdoclist);
	return S_OK;
}

HRESULT ThreadBase::getlinesinfolist(
        /* [in] */ long mod_no,
		/* [in] */ LINESINFOLIST *linesinfolist) {
	dbg_srclinesinfo_request req;	
	dbg_packet pack;
	dbg_header_struct hdr;
	DOCLIST::iterator it;
	int ret_val;

	DBGTRACE("ThreadBase::getlinesdata\n");
	if (!linesinfolist) return E_POINTER;
	
	if (!(flags & DBGF_WAITACK)) return E_FAIL;
	if (mod_no==0) {
		HRESULT hr=getdoclist(&fdoclist);
		if (FAILED(hr)) return hr;
	}
	
	linesinfolist->clear();
	dbg_packet_new(&pack);
	if (mod_no==0) {
		for (it = fdoclist.begin(); it!=fdoclist.end(); it++) {
			req.mod_no = (*it).mod_no;
			dbg_packet_add_frame(&pack, FRAME_SRCLINESINFO, &req, sizeof(req));
		}
	} else {
		req.mod_no = mod_no;
		dbg_packet_add_frame(&pack, FRAME_SRCLINESINFO, &req, sizeof(req));
	}
	ret_val = request(&hdr, &pack);
	if (ret_val>0) {
		linesinfolist->assign(&pack);
	}
	dbg_packet_free(&pack);
	return ret_val>0 ? S_OK:E_FAIL;
}


HRESULT ThreadBase::requestdocsrc(
        /* [in] */  long mod_no,
        /* [out] */ std::string *mod_name,
        /* [out] */ std::string *mod_src) {
	dbg_packet pack;
	dbg_header_struct hdr;
	dbg_frame *frame;
	dbg_source_request src_req;
	dbg_source_body *src_body;	
	int ret_val, errcnt = 0;
	int fpos, fullsize, siz;
	bool firsttime = true;
	char *str;
	bool ok;

	DBGTRACE("ThreadBase::requestdocsrc\n");
	if (!(flags & DBGF_WAITACK)) return E_FAIL;

	if (!mod_name) return E_POINTER;
	*mod_name = "";
	*mod_src = "";
	dbg_packet_new(&pack);
	src_req.mod_no = mod_no;
	fpos = 0;	
	while (true) {
		src_req.from_filepos = fpos;
		dbg_packet_clear(&pack);
		dbg_packet_add_frame(&pack, FRAME_SOURCE, &src_req, sizeof(src_req));
		ret_val = request(&hdr, &pack);
		frame = (ret_val > 0)? dbg_packet_findfirstframe(&pack,FRAME_SOURCE) : NULL;
		ok = (frame!=NULL);
		if (ok) {
			src_body = (dbg_source_body*) FRAME_DATA_PTR(frame);
			if (firsttime) {
				firsttime = false;
				if (dbg_packet_findstring(&pack, src_body->imod_name, &str, &siz)) {
					mod_name->assign(str,siz);
				}
				fullsize = src_body->full_size;
			}
			str = NULL;
			siz = 0;
			ok = (dbg_packet_findstring(&pack, src_body->itext, &str, &siz)!=0);
			if (ok) {
				mod_src->append(str,siz);
				errcnt = 0;
				fpos += siz;
				if (fpos >= fullsize) break;
			}
		}
		if (!ok && (++errcnt > 3)) break;
	}
	
	dbg_packet_free(&pack);

	if (!ok) {
		*mod_src="";
		*mod_name="";
		return E_FAIL;
	}
	return S_OK;
}

HRESULT ThreadBase::requestdocparent( 
        /* [in] */ long mod_no,
        /* [out] */ long *parent_mod_no) {
	HRESULT hr;
	DOCLIST::iterator it;

	DBGTRACE("ThreadBase::requestdocparent\n");

	hr=getdoclist(&fdoclist);
	if (FAILED(hr)) return hr;
	if (!parent_mod_no) return E_POINTER;

	*parent_mod_no = -1;
	hr = E_FAIL;
	for (it = fdoclist.begin(); it!=fdoclist.end(); it++) {
		if ((*it).mod_no == mod_no) {
			*parent_mod_no = (*it).parent_mod_no;
			hr = S_OK;
			break;
		}
	}
	return hr;
}

HRESULT ThreadBase::requestdocfilename( 
        /* [in] */ long mod_no,
        /* [out] */ std::string *mod_name) {
	HRESULT hr;
	DOCLIST::iterator it;

	DBGTRACE("ThreadBase::requestdocfilename\n");

	hr=getdoclist(&fdoclist);
	if (FAILED(hr)) return hr;
	if (!mod_name) return E_POINTER;

	*mod_name = "";

	hr = E_FAIL;
	for (it = fdoclist.begin(); it!=fdoclist.end(); it++) {
		if ((*it).mod_no == mod_no) {
			*mod_name = (*it).mod_name;
			hr = S_OK;
			break;
		}
	}
	return hr;
}

HRESULT ThreadBase::evaluate(
	/* [in] */ const char *expr,
	/* [in] */ long scope_id,
	/* [out]*/ std::string *serial_str,
	/* [out]*/ std::string *rslt_error) {
	dbg_packet pack;

	dbg_header_struct hdr;
	dbg_eval_request eval;
	dbg_frame *frame;
	dbg_eval_body *evalbody;
	char *rslt_str, *str;
	int ret_val, siz;
	bool ok = false;

	DBGTRACE("ThreadBase::evaluate()\n");
	if (!serial_str) return E_POINTER;
	*serial_str = "";
	if (!(flags & DBGF_WAITACK)) return E_FAIL;

	if (expr && strlen(expr)==0) expr = NULL;
    if (rslt_error) *rslt_error="";

	dbg_packet_new(&pack);
	eval.scope_id = scope_id;
	eval.istr = (expr) ? dbg_packet_add_string(&pack, expr) : 0;
	dbg_packet_add_frame(&pack, FRAME_EVAL, &eval, sizeof(eval));

	ret_val = request(&hdr, &pack);
	if (ret_val>0) {
		frame = dbg_packet_findfirstframe(&pack, FRAME_EVAL);
		while (frame) {
			evalbody = (dbg_eval_body *)FRAME_DATA_PTR(frame);
			if ((evalbody->istr == 0 &&  !expr) ||
			    (dbg_packet_findstring(&pack, evalbody->istr, &str, &siz)  && strcmp(str,expr)==0)) {

				ok = true;

				if (evalbody->ierror!=0 &&
					dbg_packet_findstring(&pack, evalbody->ierror, &rslt_str, &siz) && siz > 0) {
					if (rslt_error==NULL) 
						ok=false;
						else rslt_error->assign(rslt_str, siz);
				}
				
				if (dbg_packet_findstring(&pack, evalbody->iresult, &rslt_str, &siz) && siz > 0) {
					serial_str->assign(rslt_str, siz);
				}
				break;
			}
			frame = dbg_packet_findnextframe(&pack, FRAME_EVAL, frame);
		}
	}
	dbg_packet_free(&pack);

	fdoclist.clear(); // doc list might changed upon eval "include 'xxx.xxx'"

	return (ok) ? S_OK:E_FAIL;
}

HRESULT ThreadBase::setbreakpoint(
	/* [in] */ const BPOINTITEM bpm, 
	/* [in] */ const char *mod_name) {
	long parent_mod_no;

	if (bpm.mod_no < 0) return E_FAIL;
	if (bpm.mod_no == 0 && (!mod_name || strlen(mod_name)==0)) return E_FAIL;
	if (bpm.mod_no > 0 && FAILED(requestdocparent(bpm.mod_no, &parent_mod_no))) return E_FAIL;
	breakpointlist.add(bpm, mod_name);
	return S_OK;
}

HRESULT ThreadBase::getbreakpointlist(
	/* [in] */ BREAKPOINTLIST *bplist) {
	dbg_packet pack;
	dbg_header_struct hdr;
	int ret_val;
	BREAKPOINTLIST::iterator it;
	BPOINTITEM bpm;

	DBGTRACE("ThreadBase::getbreakpointlist\n");
	if (!(flags & DBGF_WAITACK) || !bplist) return E_FAIL;

	dbg_packet_new(&pack);
	dbg_packet_add_frame(&pack, FRAME_BPR, NULL, 0);

	bplist->clear();
	ret_val = request(&hdr, &pack);
	if (ret_val>0) {
		bplist->assign(&pack);
	}
	dbg_packet_free(&pack);

	for (it = breakpointlist.begin(); it != breakpointlist.end(); it++) {
		bpm.line_no = it->line_no;
		bpm.mod_no = it->mod_no;
		bpm.bps = it->state;
		bplist->add(bpm, it->mod_name);
	}

	return (ret_val > 0) ? S_OK:E_FAIL;
}

HRESULT ThreadBase::getscopelist(
	/* [in] */ SCOPELIST *ascopelist) {
	if (!ascopelist) return E_POINTER;
	ascopelist->assign(&scopelist);
	return S_OK;
}

HRESULT ThreadBase::getcurloc(
	/* [out] */ long *mod_no,
	/* [out] */ long *line_cookie) {
	if (scopelist.size() == 0) return E_FAIL;
	if (!line_cookie || !mod_no) return E_POINTER;
	*mod_no = scopelist[0].mod_no;
	*line_cookie = scopelist[0].line_no;
	return S_OK;
}
