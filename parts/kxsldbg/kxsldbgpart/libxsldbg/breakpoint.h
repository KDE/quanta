
/**************************************************************************
                          breakpoint.h  -  public functions for the
                                               breakpoint API
                             -------------------
    begin                : Fri Dec 7 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 **************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 **************************************************************************/


#ifndef XSLBREAKPOINT_H
#define XSLBREAKPOINT_H

#ifdef USE_KDE_DOCS

/**
 * Provide a basic break point support
 *
 * @short break point support
 *
 * @author Keith Isdale <k_isdale@tpg.com.au>
 */
#endif

#ifdef WITH_XSLT_DEBUG
#ifndef WITH_XSLT_DEBUG_BREAKPOINTS
#define WITH_XSLT_DEBUG_BREAKPOINTS
#endif
#endif

#ifndef BUILD_DOCS
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/xsltutils.h>

#include "arraylist.h"
#endif /* BUILD_DOCS */

#ifdef __cplusplus
extern "C" {
#endif

    /* indicate that we are to toggle a breakpoint , used for enableBreakPoint */
#define XSL_TOGGLE_BREAKPOINT -1

    /* Define the types of status whilst debugging */
#ifndef USE_KDOC
    typedef enum {
        DEBUG_NONE = 0,         /* must start at zero!! */
        DEBUG_INIT,
        DEBUG_STEP,
        DEBUG_STEPUP,
        DEBUG_STEPDOWN,
        DEBUG_NEXT,
        DEBUG_STOP,
        DEBUG_CONT,
        DEBUG_RUN,
        DEBUG_RUN_RESTART,
        DEBUG_QUIT,
        DEBUG_TRACE,
        DEBUG_WALK
    } DebugStatusEnum;


    typedef enum {
        DEBUG_BREAK_SOURCE = 300,
        DEBUG_BREAK_DATA
    } BreakPointTypeEnum;


/*Indicate what type of variable to print out.
  Is used by print_variable and searching functions */
    typedef enum {
        DEBUG_GLOBAL_VAR = 200, /* pick a unique starting point */
        DEBUG_LOCAL_VAR,
        DEBUG_ANY_VAR
    } VariableTypeEnum;

/*What type of flags can breakpoints have  */
    typedef enum {
	BREAKPOINT_ENABLED = 1,
	BREAKPOINT_ORPHANED = 2,
	BREAKPOINT_ALLFLAGS = 255
    } BreakPointFlags;

/*What state of breakpoint validation can we be in */
   typedef enum {
	BREAKPOINTS_ARE_VALID,
	BREAKPOINTS_NEED_VALIDATION,
	BREAKPOINTS_BEING_VALIDATED
   } BreakPointValidationStates;
#else
    /* keep kdoc happy */
    enum DebugStatusEnum {
        DEBUG_NONE = 0,         /* must start at zero!! */
        DEBUG_INIT,
        DEBUG_STEP,
        DEBUG_STEPUP,
        DEBUG_STEPDOWN,
        DEBUG_NEXT,
        DEBUG_STOP,
        DEBUG_CONT,
        DEBUG_RUN,
        DEBUG_RUN_RESTART,
        DEBUG_QUIT,
        DEBUG_TRACE,
        DEBUG_WALK
    };


    enum BreakPointTypeEnum {
        DEBUG_BREAK_SOURCE = 300,
        DEBUG_BREAK_DATA
    };


/*Indicate what type of variable to print out.
  Is used by print_variable and searching functions */
    enum VariableTypeEnum {
        DEBUG_GLOBAL_VAR = 200, /* pick a unique starting point */
        DEBUG_LOCAL_VAR,
        DEBUG_ANY_VAR
    } VariableTypeEnum;

/*What type of flags can breakpoints have  */
    enum BreakPointFlags {
	BREAKPOINT_ENABLED = 1,
	BREAKPOINT_ORPHANED = 2,
	BREAKPOINT_ALLFLAGS = 255
    } BreakPointFlags;

/*What state of breakpoint validation can we be in */
   enum BreakPointValidationStates {
	BREAKPOINTS_ARE_VALID,
	BREAKPOINTS_NEED_VALIDATION,
	BREAKPOINTS_BEING_VALIDATED
   } BreakPointValidationStates;
#endif

    /* The main structure for holding breakpoints */
    typedef struct _breakPoint breakPoint;
    typedef breakPoint *breakPointPtr;
    struct _breakPoint {
        xmlChar *url;
        long lineNo;
        xmlChar *templateName, *modeName;
        int flags;
        BreakPointTypeEnum type;
        int id;
    };


#ifdef USE_GNOME_DOCS

/**
 * breakPointInit:
 *
 * Intialized the breakpoint module
 *
 * Returns 1 if breakpoint module haas been initialized properly and all
 *               memory required has been obtained,
 *         0 otherwise
*/
#else
#ifdef USE_KDE_DOCS

/**
 * Intialized the breakpoint module
 *
 * @returns 1 if breakpoint module has been initialized properly and all
 *               memory required has been obtained,
 *          0 otherwise
*/
#endif
#endif
    int breakPointInit(void);



#ifdef USE_GNOME_DOCS

/**
 * breakPointFree:
 *
 * Free all memory used by breakpoint module
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Free all memory used by breakpoint module
 */
#endif
#endif
    void breakPointFree(void);



#ifdef USE_GNOME_DOCS

/**
 * breakPointActiveBreakPoint:
 *
 * Get the active break point
 *
 * Returns the last break point that we stoped at
 *
 * Depreciated
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Get the active break point
 *
 * @returns The last break point that we stoped at
 *
 * Depreciated
 */
#endif
#endif
    breakPointPtr breakPointActiveBreakPoint(void);



#ifdef USE_GNOME_DOCS

/**
 * breakPointSetActiveBreakPoint:
 * @breakPtr: Is valid break point or NULL
 *
 * Set the active break point
 *
 * Depreciated
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Set the active break point
 *
 * @param breakPoint Is valid break point or NULL
 *
 * Depreciated
 */
#endif
#endif
    void breakPointSetActiveBreakPoint(breakPointPtr breakPtr);



#ifdef USE_GNOME_DOCS

/**
 * breakPointAdd:
 * @url: Non-null, non-empty file name that has been loaded by
 *                    debugger
 * @lineNumber: @lineNumber >= 0 and is available in url specified and
 *                points to an xml element
 * @templateName: The template name of breakPoint or NULL
 * @modeName : The mode of breakpoint or NULL
 * @type: Valid BreakPointTypeEnum
 *
 * Add break point at file and line number specified
 *
 * Returns 1 if successful,
 *	   0 otherwise
*/
#else
#ifdef USE_KDE_DOCS

/**
 * Add break point at file and line number specified
 *
 * @returns 1 if successful,
 *	    0 otherwise
 *
 * @param url Non-null, non-empty file name that has been loaded by
 *                    debugger
 * @param lineNumber @p lineNumber >= 0 and is available in url specified and
 *                points to an xml element
 * @param temlateName The template name of break point or NULL
 * @param modeName : The mode of breakpoint or NULL
 * @param type Valid BreakPointTypeEnum
*/
#endif
#endif
    int breakPointAdd(const xmlChar * url, long lineNumber,
                      const xmlChar * templateName,
		      const xmlChar * modeName,
                      BreakPointTypeEnum type);



#ifdef USE_GNOME_DOCS

/**
 * breakPointDelete:
 * @breakPtr: Is valid
 *
 * Delete the break point specified if it can be found using
 *    @breakPoint's url and lineNo
 *
 * Returns 1 if successful,
 *	   0 otherwise
*/
#else
#ifdef USE_KDE_DOCS

/**
 * Delete the break point specified if it can be found using
 *    @p breakPtr's url and lineNo
 *
 * @returns 1 if successful,
 *	    0 otherwise
 *
 * @param breakPoint Is valid
 *
*/
#endif
#endif
    int breakPointDelete(breakPointPtr breakPtr);



#ifdef USE_GNOME_DOCS

/**
 * breakPointEmpty:
 *
 * Empty the break point collection
 *
 * Returns 1 if able to empty the breakpoint list of its contents,
 *         0  otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Empty the break point collection
 *
 * @returns 1 if able to empty the break point list of its contents,
 *          0  otherwise
 */
#endif
#endif
    int breakPointEmpty(void);



#ifdef USE_GNOME_DOCS

/**
 * breakPointEnable:
 * @breakPtr: A valid breakpoint
 * @enable: Enable break point if 1, disable if 0, toggle if -1
 *
 * Enable or disable a break point
 *
 * Returns 1 if successful,
 *	   0 otherwise
*/
#else
#ifdef USE_KDE_DOCS

/**
 * Enable or disable a break point
 *
 * @returns 1 if successful,
 *	    0 otherwise
 *
 * @param breakPoint A valid breakpoint
 * @param enable Enable break point if 1, disable if 0, toggle if -1
*/
#endif
#endif
    int breakPointEnable(breakPointPtr breakPtr, int enable);



#ifdef USE_GNOME_DOCS

/**
 * breakPointGet:
 * @url: Non-null, non-empty file name that has been loaded by
 *                    debugger
 * @lineNumber: lineNumber >= 0 and is available in @url
 *
 * Get a break point for the breakpoint collection
 *
 * Returns break point if break point exists at location specified,
 *	   NULL otherwise
*/
#else
#ifdef USE_KDE_DOCS

/**
 * Get a break point for the breakpoint collection
 *
 * @returns break point if break point exists at location specified,
 *	    NULL otherwise
 *
 * @param url Non-null, non-empty file name that has been loaded by
 *                    debugger
 * @param lineNumber @p lineNumber >= 0 and is available in url specified
*/
#endif
#endif
    breakPointPtr breakPointGet(const xmlChar * url, long lineNumber);



#ifdef USE_GNOME_DOCS

/**
 * breakPointGetLineNoHash:
 * @lineNo: Line number of of breakpoints of interest
 *
 * Return A hash of breakpoints with same line number
 *
 * Returns A hash of breakpoints with a line number of @lineNo
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Return A hash of breakpoints with same line number
 *
 * @param lineNo : Line number of of breakpoints of interest
 *
 * @returns A hash of breakpoints with a line number of @p lineNo
 */
#endif
#endif
    xmlHashTablePtr breakPointGetLineNoHash(long lineNo);



#ifdef USE_GNOME_DOCS

/**
 * breakPointItemNew:
 *
 * Create a new break point item
 * Returns A valid break point with default values set if successful,
 *         NULL otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Create a new break point item
 *
 * @returns A valid break point with default values set if successful,
 *          NULL otherwise
 */
#endif
#endif
    breakPointPtr breakPointItemNew(void);



#ifdef USE_GNOME_DOCS

/**
 * breakPointItemFree:
 * @payload: valid breakPointPtr
 * @name: not used
 *
 * Free memory associated with this break point
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Free memory associated with this break point
 *
 * @param payload Valid breakPointPtr
 * @param name not used
 *
 */
#endif
#endif
    void breakPointItemFree(void *payload, xmlChar * name);



#ifdef USE_GNOME_DOCS

/**
 * breakPointLinesCount:
 *
 * Return the number of hash tables of break points with the same line number
 *
 * Returns the number of hash tables of break points with the same line number
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Return the number of hash tables of break points with the same line number
 *
 * @returns the number of hash tables of break points with the same line number
 */
#endif
#endif
    int breakPointLinesCount(void);



#ifdef USE_GNOME_DOCS

/**
 * breakPointPrint:
 * @file: Is valid, or NULL to use libxslt's error display function
 * @breakPtr: A valid break point
 *
 * Print the details of @breakPtr to @file
 *
 * Returns 1 if successful,
 *	   0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Print the details of @p breakPtr to @p file
 *
 * @returns 1 if successful,
 *	    0 otherwise
 *
 * @param file Is valid, or NULL to use libxslt's error display function
 * @param breakPoint A valid break point
 */
#endif
#endif
    int breakPointPrint(FILE * file, breakPointPtr breakPtr);



#ifdef USE_GNOME_DOCS

/**
 * breakPointIsPresent:
 * @url: Non-null, non-empty file name that has been loaded by
 *                    debugger
 * @lineNumber: @lineNumber >= 0 and is available in @url
 *
 * Determine if there is a break point at file and line number specified
 *
 * Returns 1  if successful,
 *         0 otherwise
*/
#else
#ifdef USE_KDE_DOCS

/**
 * Determine if there is a break point at file and line number specified
 *
 * @returns 1  if successful,
 *          0 otherwise
 *
 * @param url Non-null, non-empty file name that has been loaded by
 *                    debugger
 * @lineNumber @p lineNumber >= 0 and is available in url specified
*/
#endif
#endif
    int breakPointIsPresent(const xmlChar * url, long lineNumber);



#ifdef USE_GNOME_DOCS

/**
 * breakPointIsPresentNode:
 * @node: node != NULL
 *
 * Determine if a node is a break point
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Determine if a node is a break point
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param node Is valid
 */
#endif
#endif
    int breakPointIsPresentNode(xmlNodePtr node);



#ifdef USE_GNOME_DOCS

/**
 * breakPointLinesList:
 *
 * Return The list of hash tables for break points
 *        Dangerous function to use!!
 *
 * Returns The list of hash tables for break points
 *        Dangerous function to use!!
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Return The list of hash tables for break points
 *        Dangerous function to use!!
 *
 * Returns The list of hash tables for break points
 *        Dangerous function to use!!
 */
#endif
#endif
    arrayListPtr breakPointLineList(void);


#ifdef __cplusplus
}
#endif
#endif
