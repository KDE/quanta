/***************************************************************************
                          ListenerBase.cpp  -  description
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

#include "ListenerBase.h"
#include <stdio.h>
#include <assert.h>

ListenerBase::~ListenerBase() {
	//DBGTRACE("ListenerBase::~ListenerBase\n");
	stop(true);
	th_list.clear();
}

int ListenerBase::start(cfgprm *cfg, void * /*extcfg*/) {
	//DBGTRACE("ListenerBase::start\n");
	fcfg = *cfg;
	
	if (listensock) {
		//DBGTRACE("socket was not closed.\n");
		dbh_close_socket(listensock);
		listensock = 0;
	}
	listensock = dbh_create_listen_socket(fcfg.address, fcfg.port);
	if (listensock <= 0) {
		_error = oserrno(1); // get network error
		fstatus = LSTNRS_initerror;
		switch (listensock) {
			case -1: 
				//DBGTRACE("API socket()");
				break;
			case -2:
				//DBGTRACE("API bind()");
				break;
			case -3:
				//DBGTRACE("API gethostbyname()");
				break;
			default: 
				//DBGTRACE("socket creation");
        break;
		}
		//DBGTRACE(" failed (err=%d). ",listensock);
		listensock = 0;	
	} else {
		_error = 0;
		fstatus = LSTNRS_ok;
	}
	return listensock>0;
}

void ListenerBase::stop(bool stopclients) {
	TH_LIST::iterator p;

	//DBGTRACE("ListenerBase::stop\n");
	if (listensock > 0) {
		dbh_close_socket(listensock);
		listensock = 0;
	}
	if (stopclients) {
		for (p = th_list.begin(); p!=th_list.end(); p++) {
			(*p).clientthread->stop(false);
		}
		for (p = th_list.begin(); p!=th_list.end(); p++) {
			(*p).clientthread->stop(true);
		}
		for (p = th_list.begin(); p!=th_list.end(); p++) {
			(*p).clientthread->destroy();
		}
		th_list.clear();
	}
	_error = 0;
	fstatus = LSTNRS_stopped;
}

int ListenerBase::restart(cfgprm *newcfg, void *extcfg) {
	if (listensock <= 0 || (
		fcfg.address && newcfg->address &&	stricmp(fcfg.address, newcfg->address)!=0)
		|| fcfg.port!=newcfg->port) {
		stop(false);
		return start(newcfg, extcfg);
	} else {
		fcfg = *newcfg;
		return listensock>0;
	}
}

int ListenerBase::peek() {
	sockaddr client_addr;
	int	 sock = 0;
	thlistitem li;
	TH_LIST::iterator p;
		
	if (listensock > 0) {
		sock = dbh_check_new_client_connect(listensock, &client_addr, fcfg.timeoutms_process);
	}
	if (sock > 0) {
		li.clientthread = create_client(sock, &client_addr);
		li.client_addr = client_addr;
		if (li.clientthread) th_list.push_back(li);
		return 1;
	}
	else {
		for (p = th_list.begin(); p!=th_list.end(); p++) {
			if ((*p).clientthread->isfinished()) {
				destroy_client((*p).clientthread,&(*p).client_addr);
				th_list.erase(p);
				return -1; /* process one object per working time-slice */
			}
		}

	}
	return 0;
}

void ListenerBase::logstatus() {
	char buf[256], sbuf[256];

	switch (fstatus) {
		case LSTNRS_notinitialized:
			log(LSTNRL_warn,(char *)("listener is not initialized,yet."));
			break;
		case LSTNRS_initerror:
			oserrnostr(buf, sizeof(buf), _error);
			snprintf(sbuf, sizeof(sbuf)-1, (char *)"Error '%s' (OSERR=%d) occurred.", buf, _error);
			log(LSTNRL_error,sbuf);
			break;
		case LSTNRS_stopped:
			log(LSTNRL_warn,(char *)"listener has been stopped.");
			break;
		case LSTNRS_ok:
			log(LSTNRL_msg,(char *)"listener is working properly.");
			break;
		default:
			snprintf(sbuf, sizeof(sbuf)-1, "unknown status %d.", fstatus);
			log(LSTNRL_warn,sbuf);
	}	
}

void ListenerBase::logsyserror(char *message, int err) {
	char buf[256], sbuf[256];

	if (!err) err = oserrno(0);
	if (!err) err = oserrno(1);
	if (!err) return;
	oserrnostr(buf, sizeof(buf), err);
	snprintf(sbuf, sizeof(sbuf), "OSERR (0x%X) '%s'", err, buf);
	log(LSTNRL_error, message, sbuf);
}

