
/**************************************************************************
                          debugXSL.h  -  describes the core xsldbg shell functions
                             -------------------
    begin                : Sun Sep 16 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 **************************************************************************/

/**************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 **************************************************************************/

/*
 * Orinal file : debugXML.h : This is a set of routines used for
 *    debugging the tree   produced by the XML parser.
 *
 * New file : debugXSL.h : Debug support version
 *
 * See Copyright for the status of this software.
 *
 * Daniel Veillard <daniel@veillard.com>
 *
 * Permission abtained to modify the LGPL'd code and extend to include 
 *   break points, inspections of stylesheet source, xml data, stylesheet
 *    variables   Keith Isdale <k_isdale@tpg.com.au>
 */

#ifndef __DEBUG_XSL__
#define __DEBUG_XSL__

#ifdef USE_XSLDBG_AS_THREAD
#include "xsldbgmsg.h"
#include "xsldbgthread.h"
#endif

/* We want skip most of these includes when building documentation*/
#ifndef BUILD_DOCS
#include "utils.h"
#include "breakpoint.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG_BUFFER_SIZE 500   /*used by xslDbgShell */

/* how may items have been printed */
    extern int printCount;

  /* used to indicated that xsldbg should stop tracing/walking 
     value : 1 stop tracing at start of next cycle
     value : 0 normal operation 
  */

  extern int xsldbgStop;
  extern int xsldbgValidateBreakpoints;
  /* Some versions of libxml/libxslt need a different type of 
	line numbers handling */
  extern int xsldbgHasLineNumberFix;  

/****************************************************************
 *								*
 *	 The XSL shell related structures and functions		*
 *								*
 ****************************************************************/

/*
   Note that functions that have a prefix of xslDbgShell are NOT implemented
      in debugXSL.c unless stated

   All functions with the prefix of debygXSL are implemented in debugXSL.c

 */

#ifdef USE_GNOME_DOCS

/**
 * debugXSLBreak:
 * @templ: The source node being executed
 * @node: The data node being processed
 * @root: The template being applied to "node"
 * @ctxt: The transform context for stylesheet being processed
 *
 * A break point has been found so pass control to user
 */
#else
#ifdef USE_KDE_DOCS

/**
 * A break point has been found so pass control to user
 *
 * @param templ The source node being executed
 * @param node The data node being processed
 * @param root The template being applied to "node"
 * @param ctxt transform context for stylesheet being processed
 */
#endif
#endif
    void debugXSLBreak(xmlNodePtr templ, xmlNodePtr node,
                       xsltTemplatePtr root, xsltTransformContextPtr ctxt);


#ifdef USE_GNOME_DOCS

/**
 * debugXSLGetTemplate:
 * 
 * Get the last template node found, if any
 *
 * Returns The last template node found, if any
 */
#else
#ifdef USE_KDE_DOCS

/** 
 * Get the last template node found, if any
 *
 * @returns the last template node found, if any
 */
#endif
#endif
    xsltTemplatePtr debugXSLGetTemplate(void);



/* -----------------------------------------
   Break Point related commands

   They are implemented in breakpoint_cmds.c
  ------------------------------------------- */


#ifdef USE_GNOME_DOCS

/**
 * xslDbgShellFrameBreak:
 * @arg: Is valid number of frames to change location by
 * @stepup: If != 1 then we step up, otherwise step down
 *
 * Set a "frame" break point either up or down from here
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Set a "frame" break point either up or down from here
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param arg Is valid  and in UTF-8
 * @param stepup If != 1 then we step up, otherwise step down
 */
#endif
#endif
    int xslDbgShellFrameBreak(xmlChar * arg, int stepup);



#ifdef USE_GNOME_DOCS

/**
 * xslDbgShellBreak:
 * @arg: Is valid and in UTF-8
 * @style: Is valid
 * @ctxt: Is valid
 * 
 * Add break point specified by arg
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Add break point specified by arg
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param arg Is valid and in UTF-8
 * @param style Is valid
 * @param ctxt Is valid
 */
#endif
#endif
    int xslDbgShellBreak(xmlChar * arg, xsltStylesheetPtr style,
                         xsltTransformContextPtr ctxt);



#ifdef USE_GNOME_DOCS

/**
 * xslDbgShellDelete:
 * @arg: Is valid and in UTF-8
 * 
 * Delete break point specified by arg
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Delete break point specified by arg
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param arg Is valid and in UTF-8
 */
#endif
#endif
    int xslDbgShellDelete(xmlChar * arg);


#ifdef USE_GNOME_DOCS

/**
 * xslDbgShellEnableBreakPoint:
 * @payload: A valid xslBreakPointPtr
 * @data: Enable type, a pointer to an integer 
 *         for a value of 
 *                 1 enable break point
 *                 0 disable break point
 *                 -1 toggle enabling of break point 
 * @name: Not used
 *
 * Enable/disable break points via use of scan of break points
*/
#else
#ifdef USE_KDE_DOCS

/**
 * Enable/disable break points via use of scan of break points
 *
 * @param payload Is valid xslBreakPointPtr
 * @param data Enable type, a pointer to an integer 
 *         for a value of 
 *              @li   1 enable break point
 *              @li   0 disable break point
 *              @li   -1 toggle enabling of break point 
 * @param name Not used
*/
#endif
#endif
    void xslDbgShellEnableBreakPoint(void *payload, void *data,
                                     xmlChar * name);



#ifdef USE_GNOME_DOCS

/**
 * xslDbgShellEnable:
 * @arg : is valid enable "commmand text" and in UTF-8
 * @enableType : enable break point if 1, disable if 0, toggle if -1
 *
 * Enable/disable break point specified by arg using enable 
 *      type of @enableType
 * Returns 1 if successful,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

  /**
   * Enable/disable break point specified by arg using enable
   *
   * @param arg: is valid enable "commmand text" and in UTF-8
   * @param enableType : enable break point if 1, disable if 0, toggle if -1
   *
   * @returns 1 if successful,
   *          0 otherwise
   */
#endif
#endif
    int xslDbgShellEnable(xmlChar * arg, int enableType);


#ifdef USE_GNOME_DOCS

/**
 * xslDbgShellPrintBreakPoint:
 * @payload: A valid xslBreakPointPtr
 * @data: Not used
 * @name: Not used
 *
 * Print data given by scan of break points 
*/
#else
#ifdef USE_KDE_DOCS

/**
 * Print data given by scan of break points 
 *
 * @param payload Is valid xslBreakPointPtr
 * @param data Not used
 * @param name Not used
*/
#endif
#endif
    void xslDbgShellPrintBreakPoint(void *payload, void *data,
                                    xmlChar * name);



#ifdef USE_GNOME_DOCS

/**
 * xslDbgShellValidateBreakPoint:
 * @payload: A valid xslBreakPointPtr
 * @data: Not used
 * @name: Not used
 *
 * Print an warning if a breakpoint is invalid
*/
#else
#ifdef USE_KDE_DOCS

/**
 * Print an warning if a breakpoint is invalid
 *
 * @param payload Is valid xslBreakPointPtr
 * @param data Not used
 * @param name Not used
*/
#endif
#endif
    void xslDbgShellValidateBreakPoint(void *payload, void *data,
                                    xmlChar * name);


/* -----------------------------------------
   Template related commands

   They are implemented in template_cmds.c
  ------------------------------------------- */

#ifdef USE_GNOME_DOCS

/**
 * xslDbgShellPrintStyleSheets:
 * @arg: The stylesheets of interests and in UTF-8, is NULL for all stylesheets
 *
 * Print stylesheets that can be found in loaded stylsheet
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Print stylesheets that can be found in loaded stylsheet
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param arg The stylesheets of interests and in UTF-8, is NULL for all stylesheets
 *
 */
#endif
#endif
    int xslDbgShellPrintStyleSheets(xmlChar * arg);



#ifdef USE_GNOME_DOCS

/** 
 * xslDbgShellPrintTemplateNames:
 * @styleCtxt: Is valid 
 * @ctxt: Not used
 * @arg: Not used
 * @verbose: If 1 then print extra messages about templates found,
 *            otherwise print normal messages only 
 * @allFiles: If 1 then look for all templates in stylsheets found in 
 *                 @styleCtxt
 *             otherwise look in the stylesheet found by 
 *                 debugXSLBreak function
 *
 * Print out the list of template names found that match critieria
*
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/** 
 * Print out the list of template names found that match critieria   
 *
 * @param styleCtxt Is valid 
 * @param ctxt Not used
 * @param arg Not used
 * @param verbose If 1 then print extra messages about templates found,
 *            otherwise print normal messages only 
 * @param allFiles If 1 then look for all templates in stylsheets found in 
 *                 @p styleCtxt
 *             otherwise look in the stylesheet found by 
 *                 debugXSLBreak function
 * @returns 1 on success,
 *          0 otherwise
 */
#endif
#endif
    int xslDbgShellPrintTemplateNames(xsltTransformContextPtr styleCtxt,
                                      xmlShellCtxtPtr ctxt,
                                      xmlChar * arg, int verbose,
                                      int allFiles);




/* -----------------------------------------

   Node viewing related commands

  ------------------------------------------- */


#ifdef USE_GNOME_DOCS

/**
 * xslDbgShellPrintList: 
 * @ctxt: The current shell context
 * @arg: What xpath to display  and in UTF-8
 * @dir: If 1 print in dir mode?, 
 *        otherwise ls mode
 *
 * Print list of nodes in either ls or dir format
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Print list of nodes in either ls or dir format
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param ctxt The current shell context
 * @param arg What xpath to display  and in UTF-8
 * @param dir If 1 print in dir mode, 
 *        otherwise ls mode
 */
#endif
#endif
    int xslDbgShellPrintList(xmlShellCtxtPtr ctxt, xmlChar * arg, int dir);



#ifdef USE_GNOME_DOCS

/** 
 * xslDbgShellCat:
 * @styleCtxt: the current stylesheet context
 * @ctxt: The current shell context
 * @arg: The xpath to print and in UTF-8
 *
 * Print the result of an xpath expression. This can include variables
 *        if styleCtxt is not NULL
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/** 
 * Print the result of an xpath expression. This can include variables
 *        if styleCtxt is not NULL
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param styleCtxt Current stylesheet context
 * @param ctxt Current shell context
 * @param arg The xpath to print and in UTF-8
 */
#endif
#endif
    int xslDbgShellCat(xsltTransformContextPtr styleCtxt,
                       xmlShellCtxtPtr ctxt, xmlChar * arg);



#ifdef USE_GNOME_DOCS

/**
 * xslDbgShellPrintVariable:
 * @styleCtxt: The current stylesheet context 
 * @arg: The name of variable to look for '$' prefix is optional and in UTF-8
 * @type: A valid VariableTypeEnum
 *
 *  Print the value variable specified by args.
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 *  Print the value variable specified by args.
 *
 * @returns 1 on success,
 *          0 otherwise

 *
 * @param styleCtxt The current stylesheet context 
 * @param arg The name of variable to look for '$' prefix is optional and in UTF-8
 * @param type Is valid VariableTypeEnum
 */
#endif
#endif
    int xslDbgShellPrintVariable(xsltTransformContextPtr styleCtxt,
                                 xmlChar * arg, VariableTypeEnum type);


/* -----------------------------------------

   File related command

   Implemented in file_cmds.c
  ------------------------------------------- */
#ifdef USE_GNOME_DOCS

/**
 * xslDbgShellOutput:
 * @arg : Is valid, either a local file name which will be expanded 
 *        if needed, or a "file://" protocol URI
 *
 * Set the output file name to use
 *
 * Returns 1 on success, 
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * xslDbgShellOutput:
 * @arg : Is valid, either a local file name which will be expanded 
 *        if needed, or a "file://" protocol URI
 *
 * Set the output file name to use
 *
 * Returns 1 on success, 
 *         0 otherwise
 */
#endif
#endif
  int xslDbgShellOutput(xmlChar *arg);




#ifdef USE_GNOME_DOCS

  /**
   * xslDbgEntities:
   * 
   * Print list of entites found 
   *
   * Returns 1 on sucess,
   *         0 otherwise
   */
#else
#ifdef USE_KDE_DOCS

#endif
#endif
    int xslDbgEntities(void);


#ifdef USE_GNOME_DOCS

  /**
   * xslDbgSystem:
   * @arg : Is valid in UTF-8
   * 
   * Print what a system file @arg maps to via the current xml catalog
   *
   * Returns 1 on sucess,
   *         0 otherwise
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Print what a system file @p arg maps to via the current xml catalog
   *
   * @param arg Is valid in UTF-8
   * 
   * @returns 1 on sucess,
   *          0 otherwise
   */
#endif
#endif
    int xslDbgSystem(const xmlChar * arg);


#ifdef USE_GNOME_DOCS

  /**
   * xslDbgPublic:
   * @arg : Is valid PublicID in UTF-8
   * 
   * Print what a public ID @arg maps to via the current xml catalog
   *
   * Returns 1 on sucess,
   *         0 otherwise
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Print what a public ID @p arg maps to via the current xml catalog
   *
   * @param arg Is valid PublicID in UTF-8
   * 
   * @returns 1 on sucess,
   *          0 otherwise
   */
#endif
#endif
    int xslDbgPublic(const xmlChar * arg);


#ifdef USE_GNOME_DOCS

  /**
   * xslDbgEncoding:
   * @arg: Is valid encoding supported by libxml2
   *
   * Set current encoding to use for output to standard output
   *
   * Returns 1 on sucess,
   *         0 otherwise
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Set current encoding to use for output to standard output
   *
   * @param arg Is valid encoding supported by libxml2
   *
   *
   * Returns 1 on sucess,
   */
#endif
#endif
    int xslDbgEncoding(xmlChar * arg);

/* -----------------------------------------

   Operating system related commands

   Implemented in os_cmds.c
  ------------------------------------------- */


#ifdef USE_GNOME_DOCS

/**
 * xslDbgShellChangeWd:
 * @path: The path to change to and in UTF-8
 *
 * Change the current working directory of the operating system
 *
 * Returns 1 if able to change xsldbg's working directory to @path
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * @returns 1 if able to change xsldbg working direcorty to @p path
 *          0 otherwise
 *
 * @param path Operating system path(directory) to change to and in UTF-8 
 */
#endif
#endif
    int xslDbgShellChangeWd(xmlChar * path);



#ifdef USE_GNOME_DOCS

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
#else
#ifdef USE_KDE_DOCS

/**
 * @returns 1 if able to execute command @p name,
 *          0 otherwise
 *
 * @param name The name of command string to be executed 
 *            by operating system shell
 * @param verbose If 1 then print extra debugging messages,
 *            normal messages otherwise
 */
#endif
#endif
    int xslDbgShellExecute(xmlChar * name, int verbose);




/* -----------------------------------------

   libxslt parameter related commands

   Implemented in param_cmds.c
  ------------------------------------------- */


#ifdef USE_GNOME_DOCS

/** 
 * xslDbgShellAddParam:
 * @arg: A string comprised of two words separated by
 *          one or more spaces which are in UTF-8. 
 *
 * Add a libxslt parameter to be sent to libxslt later on
 *
 * Returns 1 on success,
 *         0 otherwise
*/
#else
#ifdef USE_KDE_DOCS

/* 
 * Add a parameter to be sent to libxslt later on
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param arg A string comprised of two words separated by
 *          one or more spaces which are in UTF-8
 */
#endif
#endif
    int xslDbgShellAddParam(xmlChar * arg);



#ifdef USE_GNOME_DOCS

/**
 * xslDbgShellDelParam:
 * @arg: A single white space trimmed parameter number to look for
 * 
 * Delete a libxslt parameter that was to be sent to libxslt later on
 *
 * Returns 1 if able to delete parameter @name,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Delete a libxslt parameter that was to be sent to libxslt later on
 *
 * @returns 1 if able to delete parameter @p name,
 *          0 otherwise
 *
 * @param arg A single white space trimmed libxslt parameter number to look for
 */
#endif
#endif
    int xslDbgShellDelParam(xmlChar * arg);



#ifdef USE_GNOME_DOCS

/**
 * xslDbgShellShowParam:
 * @arg: Not used
 *
 * Print list of current paramters
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Print list of current paramters
 *
 * @returns 1 on success,
 *         0 otherwise
 *
 * @param arg Not used
 */
#endif
#endif
    int xslDbgShellShowParam(xmlChar * arg);


    /* -----------------------------------------
     * 
     * Option related commands
     * 
     * Implemented in option_cmds.c
     * 
     * ------------------------------------------- */

#ifdef USE_GNOME_DOCS

/**
 * xslDbgShellSetOption:
 * @arg: Is valid, and in the format  <NAME> <VALUE>
 * 
 * Set the value of an option 
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Set the value of an option 
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param arg is valid, and in format   <NAME> <VALUE>
 * 
 */
#endif
#endif
    int xslDbgShellSetOption(xmlChar * arg);


#ifdef USE_GNOME_DOCS

/**
 * xslDbgShellOptions:
 *
 * Prints out values for user options
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Prints out values for user options
 *
 * @returns 1 on success,
 *          0 otherwise
 */
#endif
#endif
    int xslDbgShellOptions(void);


  /**
   * xslDbgShellShowWatches:
   * @styleCtxt: the current stylesheet context
   * @ctxt: The current shell context
   * @showWarnings : If 1 then showWarning messages,
   *                 otherwise do not show warning messages
   *
   * Print the current watches and their values
   *
   * Returns 1 on success,
   *         0 otherwise
   */
  int xslDbgShellShowWatches(xsltTransformContextPtr styleCtxt, 
			       xmlShellCtxtPtr ctx,int showWarnings);

  /**
   * xslDbgShellAddWatch:
   * @arg : A valid xPath of expression to watch the value of
   *
   * Add expression to list of expressions to watch value of
   *
   * Returns 1 on success,
   *         0 otherwise   
   */
  int xslDbgShellAddWatch(xmlChar* arg);

  /**
   * xslDbgShellDeleteWatch:
   * @arg : A watch ID to remove
   *
   * Delete a given watch ID from our list of expressions to watch
   *
   * Returns 1 on success,
   *         0 otherwise
   */
  int xslDbgShellDeleteWatch(xmlChar* arg);


    /* -----------------------------------------
     * 
     * Tracing, walking related commands
     * 
     * Implemented in shell.c
     * 
     * ------------------------------------------- */


#ifdef USE_GNOME_DOCS

/**
 * xslDbgShellTrace:
 * @arg: Not used
 *
 * Start the tracing of the stylesheet. First need to restart it.
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Start the tracing of the stylesheet. First need to restart it.
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param arg Not used
 */
#endif
#endif
    int xslDbgShellTrace(xmlChar * arg);



#ifdef USE_GNOME_DOCS

/**
 * xslDbgShellWalk:
 * @arg: An interger between 0 and 9 indicate the speed of walk
 *
 * Start walking through the stylesheet.
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Start walking through the stylesheet.
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param arg An interger between 0 and 9 indicate the speed of walk
 */
#endif
#endif
    int xslDbgShellWalk(xmlChar * arg);



    /* -----------------------------------------
     * 
     * Seach related commands
     * 
     * Implemented in search_cmds.c
     * ------------------------------------------- */


#ifdef USE_GNOME_DOCS

/**
 * xslDbgShellSearch:
 * @styleCtxt: Is valid
 * @style: Is valid
 * @arg: The xpath query to use for searching dataBase
 * 
 * Displays the result of performing a query on the search dataBase
 *
 * Returns 1 if able to run query with @arg, 
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * @returns 1 if able to run query with @p arg, 
 *          0 otherwise
 *
 * @param styleCtxt Is valid
 * @param style Is valid
 * @param arg The xpath query to use for searching dataBase
 */
#endif
#endif
    int xslDbgShellSearch(xsltTransformContextPtr styleCtxt,
                          xsltStylesheetPtr style, xmlChar * arg);


/* -----------------------------------------
   
   Seach related commands

   Implemented in variable_cmds.c
  ------------------------------------------- */

#ifdef USE_GNOME_DOCS

/**
 * xslDbgShellSetVariable:
 * @styleCtxt : Is valid
 * @arg : Is valid must be in the format of 
 *         <VARIABLE_NAME> <XPATH>
 * 
 * Change the value of a global or local variable
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Change the value of a global or local variable
 *
 * @param styleCtxt Is valid
 * @param arg Is valid must be in the format of 
 *         <VARIABLE_NAME> <XPATH>
 * 
 * @returns 1 on success,
 *          0 otherwise
 */
#endif
#endif

    int xslDbgShellSetVariable(xsltTransformContextPtr styleCtxt,
                               xmlChar * arg);

#ifdef __cplusplus
}
#endif
#endif
 /* __DEBUG_XSL__ */
