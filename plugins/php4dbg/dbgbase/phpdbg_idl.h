/***************************************************************************
                          phpdbg_idl.h  -  description
                             -------------------
    begin                : Fri Mar 2 2001
    copyright            : (C) 2001 by Dmitri Dmitrienko
    email                : dd@cron.ru
 ***************************************************************************/

#ifndef _PHPDBG_IDL_
#define _PHPDBG_IDL_


typedef enum tagBRREASON {
    BR_STEPINTO     = 1,		// Caused by the stepping mode
    BR_STEPOVER     = 2,		// Caused by the stepping mode
    BR_STEPOUT      = 3,		// Caused by the stepping mode
    BR_BREAKPOINT   = 4,	// Caused by an explicit breakpoint
    BR_EMBEDDED     = 5,	// Caused by a scripted break
    BR_DEBUGGER_REQ = 6	// Caused by debugger IDE requested break
} BRREASON;

const long	ERR_ERROR		= 0x01;
const long	ERR_WARNING		= 0x02;
const long	ERR_PARSE		= 0x04;
const long	ERR_NOTICE		= 0x08;
const long	ERR_CORE_ERROR		= 0x10;
const long	ERR_CORE_WARNING	= 0x20;
			

			
typedef enum tagBR_ACTION{
    BR_ABORT = 0,		// Abort the application
    BR_CONTINUE = 1,	// Continue running
    BR_STEP_INTO = 2,	// Step into a procedure
    BR_STEP_OVER = 3,	// Step over a procedure
    BR_STEP_OUT = 4		// Step out of the current procedure
} BRACTION;
					    
typedef enum tagBPSTATE {
    BPS_DELETED  = 0,
    BPS_DISABLED = 1,
    BPS_ENABLED  = 2,
    BPS_UNRESOLVED = 0x8000
} BPSTATE;
							
typedef struct tagBPOINTITEM {
    BPSTATE bps;
    long line_no;
    long mod_no;
} BPOINTITEM;

/* Listener cfg flags */
const long	LCF_BREAKONLOAD	= 0x01;

/* Listener status */
typedef enum tagListenerStatus {
	LSTNRS_ok = 0,
	LSTNRS_notinitialized,
	LSTNRS_stopped,
	LSTNRS_initerror
} ListenerStatus;

/* Listener log */
typedef enum tagListenerLogKind {
	LSTNRL_msg = 0,
	LSTNRL_warn,
	LSTNRL_error
} ListenerLogKind;

								    
#endif
