/***************************************************************************
                          dbg_core.h  -  description
                             -------------------
    begin                : Sun Sep 24 2000
    copyright            : (C) 2001 by Dmitri Dmitrienko
    email                : dd@cron.ru
    www                  : http://dd.cron.ru
    license              : This source file is subject to version 1.00 of 
                           the License,  that is bundled with this package 
                           in the file LICENSE, and is available at through 
                           the world-wide-web at http://dd.cron.ru/license
 ***************************************************************************/
#ifndef _DBG_CORE_H_
#define _DBG_CORE_H_

#define DBG_API_MAJOR_VESION 0x02
#define DBG_API_MINOR_VESION 0x00
#define DBG_API_FULL_VESION (DBG_API_MAJOR_VESION << 8 | DBG_API_MINOR_VESION)

#define DBG_API_MAJOR_VESION_STR "2"
#define DBG_API_MINOR_VESION_STR "03"
#define DBG_API_MAJOR_VESION_STR_L L"2"
#define DBG_API_MINOR_VESION_STR_L L"03"

#define DBG_API_FULL_VESION_STR DBG_API_MAJOR_VESION_STR "." DBG_API_MINOR_VESION_STR
#define DBG_API_FULL_VESION_STR_L DBG_API_MAJOR_VESION_STR_L L"." DBG_API_MINOR_VESION_STR_L

#define DBG_API_DESCRIPTION    "DBG Php debugger, version " DBG_API_FULL_VESION_STR "b, copyright 2001 Dmitri Dmitrienko, dd@cron.ru"
#define DBG_API_DESCRIPTION_L L"DBG Php debugger, version " DBG_API_FULL_VESION_STR_L L"b, copyright 2001 Dmitri Dmitrienko, dd@cron.ru"

#define dbgint long	/* to avoid cross-platform problems, it must be 32-bit signed integer only*/

#if defined(AIX) && (AIX >= 42)
#define NET_SIZE_T size_t
#endif

#if defined(SEQUENT) && (SEQUENT >= 44)
#define NET_SIZE_T size_t
#endif

#if defined(__GLIBC__) && (__GLIBC__ > 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ > 0))
#define NET_SIZE_T socklen_t
#endif
#if (defined(__FreeBSD_version) && (__FreeBSD_version >= 400000))
#define NET_SIZE_T socklen_t
#endif

#if defined(SVR4) || defined(UW) || defined(OS390)
#define NET_SIZE_T size_t
#endif

#ifndef NET_SIZE_T
#define NET_SIZE_T int
#endif


#if PHP_WIN32
  #ifdef DBG_EXPORTS
    #define DBGAPI __declspec(dllexport)
  #else
    #ifdef DBG_IMPORTS
      #define DBGAPI __declspec(dllimport)
    #endif
  #endif
#endif

#if PHP_WIN32
  #include <windows.h>
  #include <windef.h>
  #include <winsock.h>
#else
  #if defined(__FreeBSD__) || defined(__OpenBSD__)
    #include <sys/types.h>
  #endif        
  #include <sys/socket.h>
  #include <unistd.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <netdb.h>
  #include <sys/time.h>
#endif


#ifndef DBGAPI
  #define DBGAPI
#endif


#if PHP_WIN32
  #define SCLOSE(a) closesocket(a)
#else
  #define SCLOSE(a) close(a)
#endif
#define SSEND(a,b,c) send(a,b,c,0)
#define SREAD(a,b,c) recv(a,b,c,0)


/*********** C O M M A N D   I N T E R F A C E ******************************************/


/* debugger_flags */
/* state: */
#define DBGF_STARTED		0x0001		/* debugger has been started */
#define DBGF_FINISHED		0x0002		/* DBGC_END notification has been sent */		
#define DBGF_WAITACK		0x0004		/* awaiting replay|request */
#define	DBGF_UNSYNC			0x0008		/* protocol has been unsynchronized */
/* execution:  */
#define DBGF_STEPINTO		0x0100		/* break on next script instruction */
#define DBGF_STEPOVER		0x0200		/* break on next script instruction on the same context */
#define DBGF_STEPOUT		0x0400		/* break on next script instruction on the outer context */
#define DBGF_ABORT			0x0800		/* stop instruction passed */

/* standard header */
/*  WARNING! All numbers are in TCP/IP network byte order (which is big-endian) */
#define DBGSYNC				0x53590000
#define MAX_PACKET_SIZE		1024*1024		/* size of packet would never exceed 1M */
#define CHUNKSIZE			16*1024			/* data is transmitted by relatively small portions which size is CHUNKSIZE */
typedef struct {
	dbgint sync;
	dbgint cmd;					/* command one of DBGC_xxxx, DBGR_xxxx, DBGA_xxxx */
	dbgint flags;				/* combination of DBGF_xxxx */
	dbgint bodysize;			/* size of the rest packet body (pack of all frames) */
} dbg_header_struct;

typedef char framename[4];
/* packet frame structure. All packets must have a header, 
  also they may have a body that consists of one ore more frames packed all together */
typedef struct {
	framename name;
	dbgint size;				/* size of pure data */
} dbg_frame;

#define FRAME_DATA_PTR(a) ((char*)a+sizeof(dbg_frame))
/*
  PHP ENGINE to DEBUGGER CLIENT WAY:
*/

/* php-engine commands/events */
#define DBGC_REPLY			0x0000		/* reply to previous DBGA_REQUEST request */
#define DBGC_STARTUP		0x0001		/* script startup */
#define	DBGC_END			0x0002		/* script done */
#define	DBGC_BREAKPOINT		0x0003		/* user definded breakpoint occured */
#define	DBGC_STEPINTO_DONE	0x0004		/* step to the next statement is completed */
#define	DBGC_STEPOVER_DONE	0x0005		/* step to the next statement is completed */
#define	DBGC_STEPOUT_DONE	0x0006		/* step to the next statement is completed */
#define	DBGC_ERROR			0x0010		/* error occured */
#define	DBGC_LOG			0x0011		/* logging support */

#define	FRAME_STACK			"STK"		/* "call:stack" - e.g. backtrace */
#define FRAME_SOURCE		"SRC"		/* source text */
#define FRAME_SRC_TREE		"SRT"		/* tree of source files */
#define FRAME_RAWDATA		"RAW"		/* raw data or string */
#define FRAME_ERROR			"ERR"		/* error notification */
#define FRAME_EVAL			"EVL"		/* evaluating/watching */
#define FRAME_BPS			"BPS"		/* set/remove breakpoint */
#define FRAME_BPR			"BPR"		/* breakpoint(s) request = get the list */
#define FRAME_VER			"VER"		/* version request */
#define FRAME_SRCLINESINFO	"LNI"		/* source lines info */
#define FRAME_LOG			"LOG"		/* logging */

#define FRAME_REQUEST		"REQ"

#define CURLOC_SCOPE_ID	1		/* nested locations are 2,3... and so on in backward order, so 2 represents most out-standing stack context*/
#define GLOBAL_SCOPE_ID	-1		/* it is global context, not stacked */

/* responces: */

typedef struct tag_dbg_version_body {			/*FRAME_VER*/
	dbgint major_version;
	dbgint minor_version;
	dbgint idescription;
} dbg_version_body;

typedef struct tag_dbg_stack_body {				/* FRAME_STACK */
	dbgint line_no;
	dbgint mod_no;
	dbgint scope_id;		/*-1 means current location, 0 never used, +1 first depth*/
	dbgint idescr;				/*ID of description string*/
} dbg_stack_body;

typedef struct tag_dbg_source_body {			/*FRAME_SOURCE*/
	dbgint mod_no;		
	dbgint from_filepos;		/* file offset */
	dbgint error;				/* 0 = ok */	
	dbgint full_size;
	dbgint imod_name;			/* ID of mod_name string */
	dbgint itext;				/* ID of text rawdata */
} dbg_source_body;

typedef struct tag_dbg_src_tree_body {			/* FRAME_SRC_TREE */
	dbgint parent_mod_no;
/*	dbgint parent_line_no;*/
	dbgint mod_no;
	dbgint imod_name;
} dbg_src_tree_body;

typedef struct tag_dbg_rawdata_body {			/* FRAME_RAWDATA */
	dbgint rawid;
	dbgint datasize;
} dbg_rawdata_body;

typedef struct tag_dbg_error_body {				/* FRAME_ERROR */
	dbgint type;				/* type of error */
	dbgint imessage;			/* ID of error message */
} dbg_error_body;

typedef struct tag_dbg_eval_body {				/*FRAME_EVAL*/
	dbgint istr;
	dbgint iresult;	
	dbgint ierror;	
} dbg_eval_body;

typedef struct tag_dbg_bpr_body { /* FRAME_BPR */
	dbgint mod_no;
	dbgint line_no;
	dbgint imod_name;
	dbgint status;				/*1-set,2-unresolved*/
} dbg_bpr_body;

typedef struct tag_dbg_srclinesinfo_body {		/* FRAME_SRCLINESINFO */
	dbgint mod_no;
	dbgint start_line_no;
	dbgint lines_count;
	dbgint ctx_id;
} dbg_srclinesinfo_body;

typedef struct tag_dbg_log_body {				/* FRAME_LOG */
	dbgint ilog;
} dbg_log;


/* requests: */


typedef struct tag_dbg_source_request {			/*FRAME_SOURCE*/
	dbgint mod_no;
	dbgint from_filepos;		/* file offset */
} dbg_source_request;

/* FRAME_STACK, FRAME_VER, FRAME_BPR and FRAME_SRC_TREE must be requested without any additional parameters (by frame name)*/
/* FRAME_ERROR can not be rerequested at all */

typedef struct tag_dbg_eval_request { /* FRAME_EVAL */
	dbgint istr;
	dbgint scope_id;
} dbg_eval_request;

typedef struct tag_dbg_bps_request {			/* FRAME_BPS */
	dbgint mod_no;
	dbgint line_no;
	dbgint status;					
	dbgint imod_name;
} dbg_bps_request;

typedef struct tag_dbg_srclinesinfo_request {	/* FRAME_SRCLINESINFO */
	dbgint mod_no;
} dbg_srclinesinfo_request;


/*
  DEBUGGER CLIENT to PHP ENGINE WAY:
*/

/* debugger client makes acknowledgement */
#define DBGA_CONTINUE		0x8001		/* php should continue run */
#define DBGA_STOP			0x8002
#define DBGA_STEPINTO		0x8003
#define DBGA_STEPOVER		0x8004
#define DBGA_STEPOUT		0x8005
#define DBGA_IGNORE			0x8006
#define DBGA_REQUEST		0x8010		/* debugger client requests some information from PHP engine */


/*********** S O F T W A R E   I N T E R F A C E ******************************************/


/* buffer to hold frames */
typedef struct {
	dbg_frame *buf;
	int limit;  /* size of buffer in memory */
	int size;   /* actual data size in the buffer */
	int lastrawid;
} dbg_packet;

/* Let packet buffer grows by 128byte-inctrements. 
   It reduces reallocation frequency if we add a lot of small frames to one packet. */

#define PACKET_LIMIT_ROUND(s) \
	((s+127) & ~127) /* round to nearest upper value 128*n */


#ifdef __cplusplus
extern "C"{
#endif

DBGAPI int dbg_packet_new(dbg_packet *pack);
DBGAPI void dbg_packet_free(dbg_packet *pack);
DBGAPI void dbg_packet_clear(dbg_packet *pack);
DBGAPI int dbg_packet_add_frame(dbg_packet *pack, framename frname, void *data, int datasize);
DBGAPI int dbg_packet_add_rawdata(dbg_packet *pack, const char *data, int datasize);

DBGAPI dbg_frame* dbg_packet_firstframe(dbg_packet *pack);
DBGAPI dbg_frame* dbg_packet_nextframe(dbg_packet *pack, dbg_frame *frame);
DBGAPI dbg_frame* dbg_packet_findfirstframe(dbg_packet *pack, framename frname);
DBGAPI dbg_frame* dbg_packet_findnextframe(dbg_packet *pack, framename frname, dbg_frame *frame);
DBGAPI int dbg_packet_findrawdata(dbg_packet *pack, int rawid, char **data, int *datasize);

DBGAPI int dbg_packet_recv(dbg_header_struct *hdr, dbg_packet *pack, int socket, int timeoutsec);
DBGAPI int dbg_packet_send(int cmd , dbg_packet *pack, int socket, int flags);

#define dbg_packet_findstring(pack,rawid,data,datasize) dbg_packet_findrawdata(pack,rawid,data,datasize)
#define dbg_packet_add_string(pack,str) dbg_packet_add_rawdata(pack,str,strlen(str)+1)

#ifdef __cplusplus
}
#endif

#endif
