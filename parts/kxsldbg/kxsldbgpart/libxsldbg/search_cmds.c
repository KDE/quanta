
/***************************************************************************
                          search_cmds.c  - search related commands for xsldbg
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
#include "search.h"

/* -----------------------------------------
   
           Seach related commands
   
  ------------------------------------------- */


/**
 * xslDbgShellSearch:
 * @styleCtxt: Is valid
 * @style: Is valid
 * @arg: The xpath query to use for searching dataBase and in UTF-8
 * 
 * Displays the result of performing a query on the search dataBase
 *
 * Returns 1 if able to run query with @arg, 
 *         0 otherwise
 */
int
xslDbgShellSearch(xsltTransformContextPtr styleCtxt,
                  xsltStylesheetPtr style, xmlChar * arg)
{
    int result = 0;
    xmlChar buff[DEBUG_BUFFER_SIZE];
    const xmlChar *sortOption = (xmlChar *) "-sort ";
    int sortOptionLen = xmlStrLen(sortOption);

    if (optionsGetStringOption(OPTIONS_DOCS_PATH) == NULL) {
        xsltGenericError(xsltGenericErrorContext,
                         "No path to documentation aborting searching\n");
#ifdef USE_DOCS_MACRO
        xsltGenericError(xsltGenericErrorContext,
                         "Error in value of USE_DOCS_MACRO look at Makefile.am\n");
#else
        xsltGenericError(xsltGenericErrorContext,
                         "Required environment variable %s not set "
                         "to the directory of xsldbg documentation\n",
                         XSLDBG_DOCS_DIR_VARIABLE);
#endif
        return result;          /* failed */
    }

    if (!styleCtxt || !style) {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Unable to search, no stylesheet loaded\n");
        return result;
    }

    result = updateSearchData(styleCtxt, style, NULL, DEBUG_ANY_VAR);
    trimString(arg);
    if (xmlStrLen(arg) == 0) {
        arg = (xmlChar *) "//search/*";
    }
    strncpy((char *) buff, (char *) arg, sortOptionLen);
    if (xmlStrEqual(buff, sortOption)) {
        /* yep do sorting as well */
        if (snprintf
            ((char *) buff, DEBUG_BUFFER_SIZE,
             "--param dosort 1 --param query \"%s\"",
             &arg[sortOptionLen])) {
            result = result && searchQuery(NULL, NULL, buff);
        }
    } else {
        if (snprintf
            ((char *) buff, DEBUG_BUFFER_SIZE,
             "--param dosort 0 --param query \"%s\"", arg)) {
            result = result && searchQuery(NULL, NULL, buff);
        }
    }
    return result;
}
