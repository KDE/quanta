
/***************************************************************************
                          oc_cmds.c  - libxslt parameter commands for xsldbg
                             -------------------
    begin                : Wed Nov 21 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifdef VERSION
#undef VERSION
#endif

#include "xsldbg.h"
#include "debugXSL.h"
#include "options.h"


/* -----------------------------------------

   Tracing related commands

  ------------------------------------------- */


/**
 * xslDbgShellTrace:
 * @arg: Not used
 *
 * Start the tracing of the stylesheet. First need to restart it.
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
xslDbgShellTrace(xmlChar * arg ATTRIBUTE_UNUSED)
{
    xslDebugStatus = DEBUG_RUN_RESTART;
    optionsSetIntOption(OPTIONS_TRACE, TRACE_ON);
    return 1;
}


/**
 * xslDbgShellWalk:
 * @arg: An interger between 0 and 9 indicate the speed of walk
 *
 * Start walking through the stylesheet.
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
xslDbgShellWalk(xmlChar * arg)
{
    int result = 0;

    long speed = WALKSPEED_NORMAL;

    if (xmlStrLen(arg)
        && !sscanf((char *) arg, "%ld", &speed)) {
        xsltGenericError(xsltGenericErrorContext,
                         "\tUnable read speed number assuming normal speed\n");
        speed = WALKSPEED_NORMAL;
    }
    if ((speed < 0) || (speed > 9)) {
        xsltGenericError(xsltGenericErrorContext,
                         "Invalid walk speed, use 0 to 9 only\n");
        xsltGenericError(xsltGenericErrorContext,
                         "\tAssuming normal speed\n");
        speed = WALKSPEED_NORMAL;
    }
    result = 1;
    optionsSetIntOption(OPTIONS_WALK_SPEED, speed);
    xslDebugStatus = DEBUG_WALK;

    return result;
}
