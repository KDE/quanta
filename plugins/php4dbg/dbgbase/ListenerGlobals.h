/***************************************************************************
                          ListenerGlobals.h  -  description
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

#ifndef _LISTENERGLOBALS_H_
#define _LISTENERGLOBALS_H_

#include "phpdbg_idl.h"
#include "dbg_core.h"

#if PHP_WIN32
# include <winsock.h>
#else
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <vector>

#ifdef WIN32
  #include "Windows.h"
  #include "Winerror.h"
#else
  #ifndef _HRESULT_DEFINED
    #define _HRESULT_DEFINED
    typedef long HRESULT;
	#define S_OK							((HRESULT)0x00000000L)
	#define S_FALSE							((HRESULT)0x00000001L)
	#define E_FAIL							((HRESULT)0x80004005L)
	#define E_POINTER						((HRESULT)0x80004003L)
	#define SUCCEEDED(Status)				((HRESULT)(Status) >= 0)
	#define FAILED(Status)					((HRESULT)(Status) < 0)
  #endif
#endif

#if PHP_WIN32
  #define	snprintf _snprintf
  #define	vsnprintf _vsnprintf
#else
  #ifndef stricmp
    #define	stricmp strcasecmp
  #endif
#endif


#ifdef _DEBUG
  void __dbgtrace(const char *str,...);
  #define DBGTRACE __dbgtrace
#else
  #define DBGTRACE
#endif

typedef struct cfgprmtag {
	int timeoutms_client;
	int timeoutms_process;
	unsigned short port;
	char breakonload;
	char address[128]; // would be sufficient either to hold a host name or an IP address.
} cfgprm;

#define DEFAULT_LSTNR_PORT 7869
#define DEFAULT_LSTNR_TIMEOUT 30000 /* 30 sec */
#define DEFAULT_LSTNR_ADDRESS ""
#define DEFAULT_LSTNR_BREAKONSTART 1


////////////////// H E L P E R   C L A S S E S ///////////////////////////////////

/* List of active scopes */
typedef struct scopelistitemtag {
	long scope_id;
	long mod_no;
	long line_no;
	char *scope_descr;
} scopelistitem, *pscopelistitem;

typedef std::vector<scopelistitem> SCOPELISTBASE;

class SCOPELIST: public SCOPELISTBASE {
public:
	void clear();
	void assign(dbg_packet *pack);
	void assign(SCOPELIST *pscopelist);
};

/* List of breakpoints */
typedef struct breakpointlistitemtag {
	long mod_no;
	char *mod_name;
	long line_no;
	BPSTATE state;
} breakpointlistitem, *pbreakpointlistitem;

typedef std::vector<breakpointlistitem> BREAKPOINTLISTBASE;

class BREAKPOINTLIST: public BREAKPOINTLISTBASE {
public:
	void clear();
	void assign(dbg_packet *pack);
	void assign(BREAKPOINTLIST *list);
	void add(const BPOINTITEM bpm, const char *mod_name);
};

/* List of documents */
typedef struct doclistitemtag {
	long mod_no;
	long parent_mod_no;
	char *mod_name;
} doclistitem, *pdoclistitem;

typedef std::vector<doclistitem> DOCLISTBASE;

class DOCLIST: public DOCLISTBASE {
public:
	void clear();
	void assign(dbg_packet *pack);
	void assign(DOCLIST *list);
};


/* List of linesinfo */
typedef struct linesinfoitemtag {
	long mod_no;
	long start_line_no;
	long lines_count;
	long ctx_id;
} linesinfoitem, *plinesinfoitem;

typedef std::vector<linesinfoitem> LINESINFOLISTBASE;

class LINESINFOLIST: public LINESINFOLISTBASE {
public:
	void assign(dbg_packet *pack);
	void assign(LINESINFOLIST *li);
};



#include "ThreadBase.h"

/* List of clients */
typedef struct thlistitemtag {
	ThreadBase *clientthread;
	sockaddr client_addr;

} thlistitem, *pthlistitem;

typedef std::vector<thlistitem> TH_LIST;

/////////////////////////// O S - R E L A T E D ///////////////////////////
/*             OS-SPECIFIC ERROR             */
int oserrno(int isnetwork);
void oserrnostr(char *buf, int bufsize, int err=-1);


/*             LISTENER SOCKET               */
int dbh_create_listen_socket(char* addr, unsigned short port );
int dbh_check_new_client_connect(int listen_socket, sockaddr *addr, int timeoutms);
int dbh_wait_debug_event(dbg_header_struct *hdr, int client_sock, dbg_packet *pack, int timeoutsec);
void dbh_close_socket(int socket);
int dbh_ack(int cmd , dbg_packet *pack, int socket, int flags);

#endif
