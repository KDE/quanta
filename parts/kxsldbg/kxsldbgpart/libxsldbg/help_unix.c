
/***************************************************************************
                          help.c  -  help system for *nix platform
                             -------------------
    begin                : Tue Jan 29 2002
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

/*
 * Uses docs/xsldoc.xsl docs/xsldoc.xml and xslproc to generate text
 */

#ifdef VERSION
#undef VERSION
#endif

#include "xsldbg.h"
#include "options.h"
#include "utils.h"
#include "debugXSL.h"
#include "help.h"
#include "files.h"
#include <stdlib.h>

#ifdef WITH_DEBUG_HELP

/**
 * helpTop:
 * @args : Is valid command or empty string
 *
 * Display help about the command in @args
 *
 * This is a platform specific interface
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
helpTop(const xmlChar * args)
{
    char buff[500], helpParam[100];

    const char *docsDirPath =
        (const char *) optionsGetStringOption(OPTIONS_DOCS_PATH);
    int result = 0;

    if (xmlStrLen(args) > 0) {
        snprintf(helpParam, 100, "--param help %c'%s'%c", QUOTECHAR, args,
                 QUOTECHAR);
    } else
        xmlStrCpy(helpParam, "");
    if (docsDirPath && filesTempFileName(0)) {
        snprintf((char *) buff, sizeof(buff), "%s %s"
                 " --param xsldbg_version %c'%s'%c "
                 " -o %s "
                 "%s%cxsldoc.xsl %s%cxsldoc.xml",
                 XSLDBG_BIN, helpParam,
                 QUOTECHAR, VERSION, QUOTECHAR,
                 filesTempFileName(0),
                 docsDirPath, PATHCHAR, docsDirPath, PATHCHAR);
        if (xslDbgShellExecute((xmlChar *) buff, 1) == 0) {
            if (docsDirPath)
                xsltGenericError(xsltGenericErrorContext,
                                 "Help failed : Maybe help files not found in %s or "
                                 "xsldbg not found in path\n",
                                 docsDirPath);
            /* FIXME: Comments not correct - the command is that invoked  */
            else
                xsltGenericError(xsltGenericErrorContext,
                                 "Unable to find xsldbg or help files\n");
        } else {
            if (filesMoreFile((xmlChar*)filesTempFileName(0), NULL) == 1) {
                result = 1;
            } else {
                xsltGenericError(xsltGenericErrorContext,
                                 "Unable to print help file\n");
            }
        }

    } else {
        xsltGenericError(xsltGenericErrorContext,
                         "No path to documentation aborting help\n");
#ifdef USE_DOCS_MACRO
        xsltGenericError(xsltGenericErrorContext,
                         "Error in value of USE_DOC_MACRO look at Makefile.am\n");
#else
        xsltGenericError(xsltGenericErrorContext,
                         "Required environment variable %s is not set "
                         "to the directory of xsldbg documentation\n",
                         XSLDBG_DOCS_DIR_VARIABLE);
#endif
    }
    return result;
}

#else


/**
 * helpTop:
 * @args : Is valid command or empty string
 *
 * Display help about the command in @args
 *
 * This is a platform specific interface
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
helpTop(const xmlChar * args ATTRIBUTE_UNUSED)
{
    int result = 0;
    xmlChar buff[500];
    const char *docTextFile = "xsldoc.txt";
    const char *docsDirPath =
        (char *) optionsGetStringOption(OPTIONS_DOCS_PATH);
    xmlChar *fileName =
        (xmlChar *) xmlMalloc(strlen(docsDirPath) + strlen(docTextFile) +
                              1);

    if (docsDirPath) {
        if (!fileName) {
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Out of memory\n");
            return result;
        }

        xmlStrCpy(fileName, docsDirPath);
        xmlStrCat(fileName, docTextFile);
        result = filesMoreFile(fileName, NULL);
    } else {
        xsltGenericError(xsltGenericErrorContext,
                         "No path to documentation aborting help\n");
#ifdef USE_DOCS_MACRO
        xsltGenericError(xsltGenericErrorContext,
                         "Error in value of USE_DOCS_MACRO look at Makefile.am\n");
#else
        xsltGenericError(xsltGenericErrorContext,
                         "Required environment variable %s not set "
                         "to the directory of xsldbg documentation\n",
                         XSLDBG_DOCS_DIR_VARIABLE);
#endif
    }

    xmlFree(fileName);
    return result;
}

#endif
