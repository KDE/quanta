
/* *************************************************************************
                          xslcallpoint.h  -  public functions for the
                                               the call stack
                             -------------------
    begin                : Fri Dec 7 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ************************************************************************* */

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef XSLCALLSTACK_H
#define XSLCALLSTACK_H

/**
 * Provide a call stack support
 *
 * @short call stack support
 *
 * @author Keith Isdale <k_isdale@tpg.com.au> 
 */

#ifndef BUILD_DOCS
#include "breakpoint.h"
#endif

#ifdef WITH_XSLT_DEBUG
#ifndef WITH_XSLT_DEBUG_BREAKPOINTS
#define WITH_XSLT_DEBUG_BREAKPOINTS
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif



    typedef struct _callPointInfo callPointInfo;
    typedef callPointInfo *callPointInfoPtr;
    struct _callPointInfo {
        xmlChar *templateName, *templateURI;
        xmlChar *modeName, *modeURI;
        xmlChar *url;
        callPointInfoPtr next;
    };

    typedef struct _callPoint callPoint;
    typedef callPoint *callPointPtr;
    struct _callPoint {
        callPointInfoPtr info;
        long lineNo;
        callPointPtr next;
    };


#ifdef USE_GNOME_DOCS

/**
 * callStackInit:
 *
 * Returns If callStack has been initialized properly and all
 *               memory required has been obtained,
 *         0 otherwise
 *
 * Returns 1 if callStack has been initialized properly and all
 *               memory required has been obtained,
 *         0 otherwise
*/
#endif
    int
      callStackInit(void);



#ifdef USE_GNOME_DOCS

/**
 * callStackFree:
 *
 *
 * Free all memory used by callStack
 */
#endif
    void
      callStackFree(void);



#ifdef USE_GNOME_DOCS

/**
 * callStackAdd:
 * @templ: The current template being applied
 * @source: The source node being processed
 *
 * Add template "call" to call stack
 *
 * Returns 1 on success, 
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

  /**
   * Add template "call" to call stack
   *
   * @param templ The current template being applied 
   * @param source The source node being processed
   *
   * @returns 1 on success, 
   *          0 otherwise
   */
#endif
#endif
    int callStackAdd(xsltTemplatePtr templ, xmlNodePtr source);


#ifdef USE_GNOME_DOCS

/**
 * callStackDrop:
 *
 *
 * Drop the topmost item off the call stack
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Drop the topmost item off the call stack
 */
#endif
#endif
    void callStackDrop(void);


#ifdef USE_GNOME_DOCS

/** 
 * callStackStepup:
 * @depth:The frame depth to step up to  
 *             0 < @depth <= callDepth()
 *
 * Set the frame depth to step up to
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/** 
 * Set the frame depth to step up to
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param depth The frame depth to step up to  
 *            0 < @p depth <= callDepth()
 */
#endif
#endif
    int callStackStepup(int depth);


#ifdef USE_GNOME_DOCS

/** 
 * callStackStepdown:
 * @depth: The frame depth to step down to, 
 *             0 < @depth <= callDepth()
 *
 * Set the frame depth to step down to
 *
 * Returns 1 on success, 
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/** 
 * Set the frame depth to step down to
 *
 * @returns 1 on success, 
 *          0 otherwise
 *
 * @param depth The frame depth to step down to  
 *            0 < @p depth <= callDepth()
 */
#endif
#endif
    int callStackStepdown(int depth);


#ifdef USE_GNOME_DOCS

/**
 * callStackGet:
 * @depth: 0 < @depth <= callDepth()
 *
 * Retrieve the call point at specified call depth 

 * Returns Non-null a if depth is valid,
 *         NULL otherwise 
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Retrieve the call point at specified call depth 

 * @returns Non-null a if depth is valid,
 *          NULL otherwise 
 *
 * @param depth 0 < @p depth <= callDepth()
 */
#endif
#endif
    callPointPtr callStackGet(int depth);


#ifdef USE_GNOME_DOCS

/** 
 * callStackGetTop:
 *
 * Get the top item in the call stack
 *
 * Returns The top of the call stack
 */
#else
#ifdef USE_KDE_DOCS

/**  
 * Get the top item in the call stack
 *
 * @returns The top of the call stack
 */
#endif
#endif
    callPointPtr callStackGetTop(void);


#ifdef USE_GNOME_DOCS

/**
 * callStackGetDepth:
 *
 * Return the depth of call stack
 *
 * Returns The depth of call stack
 */
#else
#ifdef USE_KDE_DOCS

/**
 * @returns the depth of call stack
 */
#endif
#endif
    int callStackGetDepth(void);

#ifdef __cplusplus
}
#endif
#endif
