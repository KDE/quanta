
/***************************************************************************
                          xsldbgmsg.h  -  description
                             -------------------
    begin                : Thu Dec 20 2001
    copyright            : (C) 2001 by keith
    email                : keith@linux
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef XSLDBGMSG_H
#define XSLDBGMSG_H

#ifdef WIN32
# include <libxsldbg/xsldbgwin32config.h>
#else
#  ifndef XSLDBG_SO_API
#    define XSLDBG_SO_API
#  endif
#endif

#ifndef BUILD_DOCS
#include <libxml/tree.h>        /* needed for the definition of xmlChar */
#include "arraylist.h"
#endif

typedef enum {
    /* thread status */
    XSLDBG_MSG_THREAD_NOTUSED,  /* 0:  Thread are not to be used */
    XSLDBG_MSG_THREAD_INIT,     /* 1: The xsldbg thread is initializing */
    XSLDBG_MSG_THREAD_RUN,      /* 2: The xsldbg thread is running */
    XSLDBG_MSG_THREAD_STOP,     /* 3: The xsldbg thread is about to die */
    XSLDBG_MSG_THREAD_DEAD,     /* 4: The xsldbg thread died */

    /* input status ( once thread is running) */
    XSLDBG_MSG_AWAITING_INPUT,  /* 5: Waiting for user input */
    XSLDBG_MSG_READ_INPUT,      /* 6: Read user input */
    XSLDBG_MSG_PROCESSING_INPUT,        /* 7: Processing user's request */

    /* provide more informatiom about state of xsldbg (optional) */
    XSLDBG_MSG_PROCESSING_RESULT,       /* 8: An error occured performing command
                                         * requested command */
    XSLDBG_MSG_LINE_CHANGED,    /* 9: Changed to new line number
                                 * ie a step */
    XSLDBG_MSG_FILE_CHANGED,    /* 10: Loaded source/data file */
    XSLDBG_MSG_BREAKPOINT_CHANGED,      /* 11: Response to a showbreak command */
    XSLDBG_MSG_PARAMETER_CHANGED,       /* 12: Response to showparam command */
    XSLDBG_MSG_TEXTOUT,         /* 13 : Free form text from xsldg */
    XSLDBG_MSG_FILEOUT,         /* 14 : Response to cat commmand, ie
                                 * Free form text in file */
    XSLDBG_MSG_LOCALVAR_CHANGED,        /* 15 : Response to locals command ie a
                                         * local variable */
    XSLDBG_MSG_GLOBALVAR_CHANGED,       /* 16 : Response to globals command
                                         * ie a global variable */
    XSLDBG_MSG_TEMPLATE_CHANGED,        /* 17 : Response to templates commmand
                                         * ie template details */
    XSLDBG_MSG_SOURCE_CHANGED,  /* 18 : Response to stylesheets command,
                                 * a normal stylesheet */
    XSLDBG_MSG_INCLUDED_SOURCE_CHANGED, /* 19: Response to stylesheets
                                         * command, a xmlNodeptr of
                                         * a included stylesheet */
    XSLDBG_MSG_CALLSTACK_CHANGED,       /* 20: Response to where command,
                                         * ie a item on the call stack */
    XSLDBG_MSG_ENTITIY_CHANGED, /* 21: Response to entities
                                 * command */
    XSLDBG_MSG_RESOLVE_CHANGE,  /* 22: Response to system or
                                 * public command */
    XSLDBG_MSG_LIST,            /* 23 : As list of messages  */

    XSLDBG_MSG_INTOPTION_CHANGE,        /* 23* Response to options command */
    XSLDBG_MSG_STRINGOPTION_CHANGE      /* 24* Response to options command */
} XsldbgMessageEnum;


typedef enum {
    XSLDBG_COMMAND_FAILED,      /* generic error */
    XSLDBG_COMMAND_WARNING,
    XSLDBG_COMMAND_INFO,
    XSLDBG_COMMAND_NOTUSED
} XsldbgCommandStateEnum;



/**
 * Notify the application that something happened to the xsldbg thread
 *
 * @param type : A valid XsldbgMessageEnum
 *
 * @param data : The meaning of data can have a different meaning for each value of @type
 *<pre>
 *   Value of @type                       Meaning of @data
 *    --------------------------  +++ ---------------------
 * XSLDBG_MSG_THREAD_NOTUSED,               not used
 * XSLDBG_MSG_THREAD_INIT,		    not used
 * XSLDBG_MSG_THREAD_RUN,		    not used
 * XSLDBG_MSG_THREAD_STOP,		    not used
 * XSLDBG_MSG_THREAD_DEAD,    	     	    not used
 * XSLDBG_MSG_AWAITING_INPUT, 		    not used
 * XSLDBG_MSG_READ_INPUT,		A value of the char* for user input
 * XSLDBG_MSG_PROCESSING_INPUT,  	    not used
 * XSLDBG_MSG_PROCESSING_RESULT,      A value of type xsldbgErrorMsgPtr
 * XSLDBG_MSG_LINE_CHANGED            Is non-NULL if reached breakpoint
 *                                      otherwise just change in line number
 *                                      of displayed source/data
 * XSLDBG_MSG_FILE_CHANGED,      		not used
 * XSLDBG_MSG_BREAKPOINT_CHANGED      A breakPointPtr of the breakpoint
 * XSLDBG_MSG_PARAMETER_CHANGED     A parameterItemPtr of libxslt pameter.
 * XSLDBG_MSG_TEXTOUT               A char * to buffer for text output
 * XSLDBG_MSG_FILEOUT               A FILE * for text to output
 * XSLDBG_MSG_LOCALVAR_CHANGED,     A local variable of type xsltStackElemPtr
 * XSLDBG_MSG_GLOBALVAR_CHANGED,    A global variable of type xsltStackElemPtr
 * XSLDBG_MSG_TEMPLATE_CHANGED,     A template of type xsltTemplatePtr
 * XSLDBG_MSG_SOURCE_CHANGED,           A xsltStylesheetPtr of a normal stylesheet
 * XSLDBG_MSG_INCLUDED_SOURCE_CHANGED  A xmlNodePtr of a included stylsheet
 * XSLDBG_MSG_CALLSTACK_CHANGED        A callPointPtr of a call stack item
 * XSLDBG_MSG_ENTITIY_CHANGED           A const entityInfoPtr
 *                                        for the included entity
 * XSLDBG_MSG_RESOLVE_CHANGE           A xmlChar* of URI that
 *                                        SystemID or PublicID resolves to
 * XSLDBG_MSG_LIST                     A notifyMessageListPtr
 *
 * XSLDBG_MSG_INTOPTION_CHANGE      A paramItemPtr, value is not used
 * XSLDBG_MSG_STRINGOPTION_CHANGE   A paramItemPtr, intValue is not used
 *
 *
 * Legend :
 *	        not used  :- value may be NULL but must not be used
 *
 * All values are to treated as volitile and are only guaranteed
 *     to be valid for the life of the notification messages. ie make a
 *       NEW copy of value if needed for longer  than that.
 *  Unless stated otherwise, if reponse can return a value and the value is
 *     NULL then that indicates the start of a list of values
 *
 *<pre>
 * @returns 1 on sucess
 *          0 otherwise
*/

typedef struct _xsldbgErrorMsg xsldbgErrorMsg;
typedef xsldbgErrorMsg *xsldbgErrorMsgPtr;
struct _xsldbgErrorMsg {
    XsldbgMessageEnum type;
    int commandId;
    XsldbgCommandStateEnum commandState;
    xmlChar *text;
    xmlChar *messagefileName;   /* used when send large chunks of data  */
};


typedef struct _notifyMessageList notifyMessageList;
typedef notifyMessageList *notifyMessageListPtr;
struct _notifyMessageList {
    XsldbgMessageEnum type;
    arrayListPtr list;
};

#ifdef __cplusplus
extern "C" {
#endif

int notifyXsldbgApp(XsldbgMessageEnum type, const void *data);

int notifyStateXsldbgApp(XsldbgMessageEnum type, int commandId,
                             XsldbgCommandStateEnum commandState,
                             const char *text);

int notifyTextXsldbgApp(XsldbgMessageEnum type, const char *text);


    /* The following functions implemented in xsldbgthread.c */
	int notifyListStart(XsldbgMessageEnum type);

    /* Data must be valid for until the next notifyListStart.
     * Memory pointed to by @data will not be freed. Added @data items
     * queued to list must be of the same data type as required by the
     * XsldbgMessageEnum used with the matching notifyListSend */
    int notifyListQueue(const void *data);

    /* The notified application is responsible for free memory used
     * by the ArrayList and notifyMessageList of notify message */
    int notifyListSend(void);


#ifdef __cplusplus
}
#endif
#endif
