
/***************************************************************************
                          os_cmds.c  - operating system commands for xsldbg
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
#include "files.h"

/* -----------------------------------------

   Operating system related commands

  ------------------------------------------- */


/**
 * xslDbgShellChangeWd:
 * @path: The path to change to
 *
 * Change the current working directory of the operating system
 *
 * Returns 1 if able to change xsldbg's working directory to @path
 *         0 otherwise
 */
int
xslDbgShellChangeWd(xmlChar * path)
{
    int result = 0;

    if (xmlStrLen(path)) {
        /* call function in files.c to do the work */
        result = changeDir(path);
    } else
        xsltGenericError(xsltGenericErrorContext,
                         "Missing path name after chdir command\n");
    return result;
}


/**
 * xslDbgShellExecute:
 * @name: The name of command string to be executed by operating system shell
 * @verbose: If 1 then print extra debugging messages,
 *            normal messages otherwise
 * 
 * Execute an operating system command
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
xslDbgShellExecute(xmlChar * name, int verbose)
{
    int result = 0;

    /* Quick check to see if we have a command processor; embedded systems
     * may not have such a thing */
    if (system(NULL) == 0) {
        xsltGenericError(xsltGenericErrorContext,
                         "No command processor available for shell command\"%s\"\n",
                         name);
    } else {
        int return_code;

        if (verbose)
            xsltGenericError(xsltGenericErrorContext,
                             "Starting shell command \"%s\"\n", name);

        return_code = system((char *) name);
        /* JRF: Strictly system returns an implementation defined value -
         * we are interpreting that value here, so we need
         * implementation specific code to handle each case */

#ifdef __riscos
        /* on RISC OS -2 means 'system call failed', otherwise it is the
         * return code from the sub-program */
        if (return_code != -2) {
#else
        if (return_code == 0) {
#endif
            if (verbose)
                xsltGenericError(xsltGenericErrorContext,
                                 "\nFinished shell command\n");
            result = 1;
        } else {
            if (verbose)
                xsltGenericError(xsltGenericErrorContext,
                                 "\nUnable to run command (system error %d)\n",
                                 return_code);
        }
    }
    return result;
}
