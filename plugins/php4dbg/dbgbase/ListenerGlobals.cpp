/***************************************************************************
                          ListenerGlobals.cpp  -  description
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

#include <stdarg.h>
#include <stdio.h>
#include "ListenerGlobals.h"
#include "string.h"

#ifdef WIN32
#include <Windows.h>
#else
#include <errno.h>
#endif

#ifdef _DEBUG
void __dbgtrace(const char *str,...) {
	va_list args;

	if (!str) return;
	va_start(args, str);	
#ifdef WIN32
	{
		char buf[1024];
		_vsnprintf(buf, sizeof(buf), str, args);
		OutputDebugString(buf);
	}
#else
	vfprintf(stdout, str, args);
	fflush(stdout);
#endif
	va_end(args);
}
#endif

int oserrno(int isnetwork) {
#if PHP_WIN32
	if (isnetwork)
		return WSAGetLastError();
	else
		return GetLastError();
#else
	return errno;
#endif
}

void oserrnostr(char *buf, int bufsize, int err) {
	if (!bufsize || !buf) return;
	buf[0] = 0;
	if (err == -1) {
		err = oserrno(0);
		if (!err) err = oserrno(1);
	}
	if (!err) return;
#if PHP_WIN32
	{
		int i;
		FormatMessage(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			err,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), /* Default language */
			buf,
			bufsize,
			NULL
		);
		i=strlen(buf);
		while (--i>0 && buf[i]<32);
		buf[i+1] = 0;
	}
#else
	{
	  strerror_r(err, buf, bufsize);
	}
#endif

}

// SCOPELIST

void SCOPELIST::clear() {
	iterator it;

	for (it = begin(); it != end(); it++) {
		if ((*it).scope_descr) ::free((*it).scope_descr);
	}
	SCOPELISTBASE::clear();
}

void SCOPELIST::assign(dbg_packet *pack) {
	dbg_frame *p;
	char *str;
	scopelistitem stkitem;
	dbg_stack_body *stk;
	int siz;

	clear();
	p = dbg_packet_findfirstframe(pack, FRAME_STACK);
	while (p) {
		stk = (dbg_stack_body *)FRAME_DATA_PTR(p);
		
		stkitem.line_no = stk->line_no;
		stkitem.mod_no = stk->mod_no;
		stkitem.scope_id = stk->scope_id;
		stkitem.scope_descr = NULL;
		if (dbg_packet_findstring(pack, stk->idescr, &str, &siz)) {
			stkitem.scope_descr = strdup(str);
		}
		push_back(stkitem);
		p = dbg_packet_findnextframe(pack, FRAME_STACK, p);
	}
}

void SCOPELIST::assign(SCOPELIST *pscopelist) {
	pscopelistitem item;
	scopelistitem newitem;
	iterator it;
	
	clear();
	if (pscopelist) {
		for (it = pscopelist->begin(); it != pscopelist->end(); it++) {
			item = it;
			memcpy(&newitem, item, sizeof(newitem));
			newitem.scope_descr = (item->scope_descr) ? strdup(item->scope_descr) : NULL;
			push_back(newitem);
		}
	}
}

// BREAKPOINTLIST
void BREAKPOINTLIST::clear() {
	iterator it;

	for (it = begin(); it != end(); it++) {
		if ((*it).mod_name) ::free((*it).mod_name);
	}
	BREAKPOINTLISTBASE::clear();
}

void BREAKPOINTLIST::assign(BREAKPOINTLIST *list) {
	pbreakpointlistitem ititem;
	breakpointlistitem item;
	iterator it;

	clear();
	if (!list) return;
	for (it = list->begin(); it != list->end(); it++) {
		ititem = it;
		memcpy(&item, ititem, sizeof(item));
		item.mod_name = (ititem->mod_name) ? strdup(ititem->mod_name) : NULL;
		push_back(item);
	}
}

void BREAKPOINTLIST::assign(dbg_packet *pack) {
	dbg_bpr_body *body;
	char *mod_name;
	int siz;
	breakpointlistitem bpitem;
	dbg_frame *frame;

	frame = dbg_packet_findfirstframe(pack, FRAME_BPR);
	while (frame) {
		body = (dbg_bpr_body *)FRAME_DATA_PTR(frame);
		mod_name = NULL;
		if (body->imod_name != 0) dbg_packet_findstring(pack, body->imod_name, &mod_name, &siz);
		bpitem.line_no = body->line_no;
		bpitem.mod_no = body->mod_no;
		bpitem.state = (mod_name==NULL) ? BPS_UNRESOLVED : BPS_ENABLED;
		bpitem.mod_name = (mod_name) ? strdup(mod_name) : NULL;
		push_back( bpitem);
		frame = dbg_packet_findnextframe(pack, FRAME_BPR, frame);
	}
}

void BREAKPOINTLIST::add(const BPOINTITEM bpm, const char *mod_name) {
	breakpointlistitem bpitem, *pbpitem;
	bool found = false;
	iterator it;
	
	bpitem.line_no = bpm.line_no;
	bpitem.mod_no = bpm.mod_no;
	bpitem.state = bpm.bps;
	bpitem.mod_name = strdup((mod_name) ? mod_name : "");
	for (it = begin(); it!=end(); it++) {
		if (it->mod_no == bpitem.mod_no &&
			strcmp(it->mod_name, bpitem.mod_name) == 0 &&
			it->line_no == bpitem.line_no) {
			pbpitem = it;
			found = true;
			break;
		}
	}
	if (found) {
		pbpitem->state = bpm.bps; // update state only
	} else {
		push_back(bpitem);
	}
}


// DOCLIST
void DOCLIST::clear() {
	iterator it;

	for (it = begin(); it != end(); it++) {
		if ((*it).mod_name) ::free((*it).mod_name);
	}
	DOCLISTBASE::clear();
}

void DOCLIST::assign(DOCLIST *list) {
	doclistitem item, *ititem;
	iterator it;

	clear();
	if (!list) return;
	for (it = list->begin(); it != list->end(); it++) {
		ititem = it;
		memcpy(&item, ititem, sizeof(item));
		item.mod_name = (ititem->mod_name) ? strdup(ititem->mod_name) : NULL;
		push_back(item);
	}
}

void DOCLIST::assign(dbg_packet *pack) {
	dbg_src_tree_body *body;
	char *mod_name;
	int siz;
	doclistitem item;
	dbg_frame *frame;

	frame = dbg_packet_findfirstframe(pack, FRAME_SRC_TREE);
	while (frame) {
		body = (dbg_src_tree_body *)FRAME_DATA_PTR(frame);
		mod_name = NULL;
		if (body->imod_name != 0) dbg_packet_findstring(pack, body->imod_name, &mod_name, &siz);
		item.mod_name = (mod_name) ? strdup(mod_name) : NULL;
		item.mod_no = body->mod_no;
		item.parent_mod_no = body->parent_mod_no;
		push_back(item);
		frame = dbg_packet_findnextframe(pack, FRAME_SRC_TREE, frame);
	}
}


// LINESINFOLIST
void LINESINFOLIST::assign(dbg_packet *pack) {
	dbg_srclinesinfo_body *body;
	linesinfoitem item;
	dbg_frame *frame;

	frame = dbg_packet_findfirstframe(pack, FRAME_SRCLINESINFO);
	while (frame) {
		body = (dbg_srclinesinfo_body *)FRAME_DATA_PTR(frame);

		item.mod_no = body->mod_no;
		item.start_line_no = body->start_line_no;
		item.lines_count = body->lines_count;
		item.ctx_id = body->ctx_id;
		
		push_back(item);
		frame = dbg_packet_findnextframe(pack, FRAME_SRCLINESINFO, frame);
	}
}
	
void LINESINFOLIST::assign(LINESINFOLIST *li) {
	linesinfoitem item, *ititem;
	iterator it;

	clear();
	if (!li) return;
	for (it = li->begin(); it != li->end(); it++) {
		ititem = it;
		memcpy(&item, ititem, sizeof(item));		
		push_back(item);
	}
}


/*************************************************************************************
 *
 *			DEBUGGER HOST PART
 *
 *************************************************************************************/

int dbh_create_listen_socket(char* addr, unsigned short port ){
	int sock;
	struct sockaddr_in saddr;
	 struct hostent *host_info;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock <= 0) return -1;
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	if (addr && addr[0]) {
		saddr.sin_addr.s_addr = inet_addr(addr);
		if (saddr.sin_addr.s_addr == INADDR_NONE){
			host_info = gethostbyname(addr);
			if (host_info == 0) return -3;
			saddr.sin_addr.s_addr = ((struct in_addr *) host_info->h_addr_list[0])->s_addr;
		}
	}
	else {
		saddr.sin_addr.s_addr = INADDR_ANY;
	}
	saddr.sin_port = htons(port);
	if (bind(sock, (struct sockaddr *) &saddr, sizeof(saddr)) || listen(sock,5)) {
		SCLOSE(sock);
		return -2;
	}
	return sock;
}

int dbh_check_new_client_connect(int listen_socket, sockaddr *addr, int timeoutms) {
	fd_set rset;
	int ret_val;
	timeval timeout;
	sockaddr laddr;
	NET_SIZE_T siz;
	
	if (listen_socket<=0) return 0;
	FD_ZERO(&rset);
	FD_SET((unsigned int)listen_socket, &rset);
	if (timeoutms >= 0) {
		timeout.tv_sec = timeoutms / 1000;
		timeout.tv_usec = (timeoutms>1000) ? 0 : timeoutms * 1000;
	} else {
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;
	}

	ret_val = select(listen_socket + 1, &rset, NULL, NULL, &timeout);
	if (ret_val>0) {
		siz = sizeof(laddr);
		ret_val = accept(listen_socket, &laddr, &siz);
	}
	if (ret_val <= 0) {
		return (ret_val) ? (-1) : (0);
	}
	if (addr) *addr = laddr;
	return ret_val;
}

int dbh_wait_debug_event(dbg_header_struct *hdr, int client_sock, dbg_packet *pack, int timeoutms) {
	return dbg_packet_recv(hdr, pack, client_sock, timeoutms);
}

int dbh_request_debug_info() {
	return 0;
}

void dbh_close_socket(int socket) {
	SCLOSE(socket);
}

int dbh_ack(int cmd , dbg_packet *pack, int socket, int flags) {
	return dbg_packet_send(cmd, pack, socket, flags);
}

