/***************************************************************************
                          ListenerBase.h  -  description
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

#ifndef _LISTENERBASE_H_
#define _LISTENERBASE_H_

#include <vector>
#include <string.h>
#include "ListenerGlobals.h"
#include "ThreadBase.h"

class ListenerBase {
private:
protected:
	int listensock;
	cfgprm fcfg;
	ListenerStatus fstatus;
	int _error;
	TH_LIST th_list;
	virtual ThreadBase *create_client(int sock, sockaddr *client_addr) = 0; // pure method
	virtual void destroy_client(ThreadBase *client, sockaddr *) {
		client->destroy();
	}

public:
	ListenerBase(): listensock(0), fstatus(LSTNRS_notinitialized), _error(0) {
		DBGTRACE("ListenerBase::ListenerBase\n");
		memset(&fcfg, 0, sizeof(fcfg));
	}
	virtual ~ListenerBase();
	ListenerStatus status() {
		return fstatus;
	}
	int numberofclients() {
		return th_list.size();
	}
	ThreadBase *client(int index) {
		return (index >= 0 && index < (int)th_list.size()) ? th_list[index].clientthread : NULL;
	}
	int error() {
		return _error;
	}

	virtual int start(cfgprm *cfg, void *extcfg);
	virtual void stop(bool stopclients);
	virtual int restart(cfgprm *newcfg, void *extcfg);

	virtual int peek();
	virtual void log(ListenerLogKind kind, char *message,...) = 0;
	void logstatus();
	virtual void logsyserror(char *message, int err=0);

};


#endif
