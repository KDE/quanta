
/***************************************************************************
                          xsldbg.c  -  description
                             -------------------
    begin                : Sun Sep 16 2001
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
 * Based on file xsltproc.c
 *
 * by  Daniel Veillard 
 *     daniel@veillard.com
 *
 *  xsltproc.c is part of libxslt
 *
 *
 */

#ifdef VERSION
#undef VERSION
#endif

/* Justin's port version - do not merge into core!!! */
#define RISCOS_PORT_VERSION "2.01"

#include "xsldbg.h"
#include "debug.h"
#include "options.h"
#include "utils.h"
#include "files.h"
#include "breakpoint.h"
#include "debugXSL.h"
#include "xsldbgmsg.h"
#include "xsldbgthread.h"       /* for getThreadStatus */
#ifdef HAVE_READLINE
#  include <readline/readline.h>
#  ifdef HAVE_HISTORY
#     include <readline/history.h>
#   endif
#endif


/* need to setup catch of SIGINT */
#include <signal.h>

/* needed by printTemplateNames */
#include <libxslt/transform.h>

/* standard includes from xsltproc*/
#include <libxslt/xslt.h>
#include <libexslt/exslt.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STDARG_H
#include <stdarg.h>
#endif
#include <libxml/xmlmemory.h>
#include <libxml/debugXML.h>
#include <libxml/HTMLtree.h>
#include <libxml/xmlIO.h>
#ifdef LIBXML_DOCB_ENABLED
#include <libxml/DOCBparser.h>
#endif
#ifdef LIBXML_XINCLUDE_ENABLED
#include <libxml/xinclude.h>
#endif

#include <libxml/catalog.h>

#include <libxml/parserInternals.h>

#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>
#include <libxslt/extensions.h>

#include <libexslt/exsltconfig.h>

#ifdef WIN32
#ifdef _MSC_VER
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#define gettimeofday(p1,p2)
#define HAVE_TIME_H
#include <time.h>
#define HAVE_STDARG_H
#include <stdarg.h>
#endif /* _MS_VER */
#else /* WIN32 */
#if defined(HAVE_SYS_TIME_H)
#include <sys/time.h>
#elif defined(HAVE_TIME_H)
#include <time.h>
#endif

#endif /* WIN32 */

#ifndef HAVE_STAT
#  ifdef HAVE__STAT

/* MS C library seems to define stat and _stat. The definition
      *         is identical. Still, mapping them to each other causes a warning. */
#    ifndef _MSC_VER
#      define stat(x,y) _stat(x,y)
#    endif
#    define HAVE_STAT
#  endif
#endif

#ifdef __riscos

/* Global definition of our program name on invocation.
   This is required such that we can invoke ourselves for processing
   search or help messages where our executable does not exist on the
   current run path */
char *xsldbgCommand = NULL;
#endif

FILE *errorFile = NULL; /* we'll set this just before starting debugger */

xmlParserInputPtr xmlNoNetExternalEntityLoader(const char *URL,
                                               const char *ID,
                                               xmlParserCtxtPtr ctxt);

/* -----------------------------------------
   Private function declarations for xsldbg.c
 -------------------------------------------*/

/**
 * xsldbgInit:
 * 
 * Returns 1 if able to allocate memory needed by xsldbg
 *         0 otherwise
 */
int xsldbgInit(void);


/**
 * xsldbgFree:
 *
 * Free memory used by xsldbg
 */
void xsldbgFree(void);


/**
 * printTemplates:
 * @style : valid as parsed my xsldbg
 * @doc :    "    "   "     "    "
 *  
 * print out list of template names
 */
void printTemplates(xsltStylesheetPtr style, xmlDocPtr doc);


/**
 * catchSigInt:
 * @value : not used
 *
 * Recover from a signal(SIGINT), exit if needed
 */
void catchSigInt(int value ATTRIBUTE_UNUSED);


/**
 * catchSigTerm:
 * @value : not used
 *
 * Clean up and exit
 */
void
  catchSigTerm(int value ATTRIBUTE_UNUSED);

/**
 * xsldbgGenericErrorFunc:
 * @ctx:  Is Valid
 * @msg:  Is valid
 * @...:  other parameters to use
 * 
 * Handles print output from xsldbg and passes it to the application if 
 *  running as a thread otherwise send to stderr
 */
void
  xsldbgGenericErrorFunc(void *ctx, const char *msg, ...);

xmlEntityPtr (*oldGetEntity)( void * user_data, const xmlChar * name);

static xmlEntityPtr xsldbgGetEntity( void * user_data, const xmlChar * name)
{
    xmlEntityPtr ent = NULL;
    if (oldGetEntity){
	ent =  (oldGetEntity)(user_data, name);
	if (ent)
	    filesEntityRef(ent, ent->children, ent->last);
    }
    return ent;
}

/* ------------------------------------- 
   End private functions
   ---------------------------------------*/


/*
 * Internal timing routines to remove the necessity to have unix-specific
 * function calls
 */

#if defined(HAVE_GETTIMEOFDAY)
static struct timeval begin, end;

/*
 * startTimer: call where you want to start timing
 */
static void
startTimer(void)
{
    gettimeofday(&begin, NULL);
}

/*
 * endTimer: call where you want to stop timing and to print out a
 *           message about the timing performed; format is a printf
 *           type argument
 */
static void
endTimer(const char *format, ...)
{
    char timerBuffer[400];
    long msec;
    va_list ap;

    gettimeofday(&end, NULL);
    msec = end.tv_sec - begin.tv_sec;
    msec *= 1000;
    msec += (end.tv_usec - begin.tv_usec) / 1000;

#ifndef HAVE_STDARG_H
#error "endTimer required stdarg functions"
#endif
    va_start(ap, format);
    vsnprintf(timerBuffer, sizeof(timerBuffer), format, ap);
    va_end(ap);
    xsltGenericError(xsltGenericErrorContext, "%s", timerBuffer);

    xsltGenericError(xsltGenericErrorContext, " took %ld ms\n", msec);
}
#elif defined(HAVE_TIME_H)

/*
 * No gettimeofday function, so we have to make do with calling clock.
 * This is obviously less accurate, but there's little we can do about
 * that.
 */

clock_t begin, end;
static void
startTimer(void)
{
    begin = clock();
}
static void
endTimer(char *format, ...)
{
    long msec;
    va_list ap;

    end = clock();
    msec = ((end - begin) * 1000) / CLOCKS_PER_SEC;

#ifndef HAVE_STDARG_H
#error "endTimer required stdarg functions"
#endif
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
    xsltGenericError(xsltGenericErrorContext, " took %ld ms\n", msec);
}
#else

/*
 * We don't have a gettimeofday or time.h, so we just don't do timing
 */
static void
startTimer(void)
{
    /*
     * Do nothing
     */
}
static void
endTimer(char *format, ...)
{
    /*
     * We cannot do anything because we don't have a timing function
     */
#ifdef HAVE_STDARG_H
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
    xsltGenericError(xsltGenericErrorContext, " was not timed\n", msec);
#else
    /* We don't have gettimeofday, time or stdarg.h, what crazy world is
     * this ?!
     */
#endif
}
#endif

static void
xsltProcess(xmlDocPtr doc, xsltStylesheetPtr cur)
{
    xmlDocPtr res;
    const char *params[8 * 2 + 2];
    int nbparams = 0;
    int paramIndex;
    parameterItemPtr paramItem;

    /* Copy the parameters accross for libxslt */
    for (paramIndex = 0;
         paramIndex < arrayListCount(optionsGetParamItemList());
         paramIndex++) {
        paramItem = arrayListGet(optionsGetParamItemList(), paramIndex);
        if (paramItem) {
            params[nbparams] = (char *) paramItem->name;
            params[nbparams + 1] = (char *) paramItem->value;
            nbparams += 2;
        }
    }

    params[nbparams] = NULL;

#ifdef LIBXML_XINCLUDE_ENABLED
    if (optionsGetIntOption(OPTIONS_XINCLUDE)) {
        if (optionsGetIntOption(OPTIONS_TIMING))
            startTimer();
        xmlXIncludeProcess(doc);
        if (optionsGetIntOption(OPTIONS_TIMING)) {
            endTimer("XInclude processing %s",
                     optionsGetStringOption(OPTIONS_OUTPUT_FILE_NAME));
        }
    }
#endif
    if (optionsGetIntOption(OPTIONS_TIMING) ||
        optionsGetIntOption(OPTIONS_PROFILING))
        startTimer();
    if ((optionsGetStringOption(OPTIONS_OUTPUT_FILE_NAME) == NULL) ||
        optionsGetIntOption(OPTIONS_SHELL)) {
        if (optionsGetIntOption(OPTIONS_REPEAT)) {
            int j;

            for (j = 1; j < optionsGetIntOption(OPTIONS_REPEAT); j++) {
                res = xsltApplyStylesheet(cur, doc, params);
                xmlFreeDoc(res);
                doc = xsldbgLoadXmlData();
            }
        }
        if (optionsGetIntOption(OPTIONS_PROFILING)) {
            if (terminalIO != NULL)
                res = xsltProfileStylesheet(cur, doc, params, terminalIO);
            else if ((optionsGetStringOption(OPTIONS_OUTPUT_FILE_NAME) ==
                      NULL) || (getThreadStatus() != XSLDBG_MSG_THREAD_RUN)
                     || (filesTempFileName(1) == NULL))
                res = xsltProfileStylesheet(cur, doc, params, stderr);
            else {
                /* We now have to output to using notify using 
                 * temp file #1 */
                FILE *tempFile = fopen(filesTempFileName(1), "w");

                if (tempFile != NULL) {
                    res =
                        xsltProfileStylesheet(cur, doc, params, tempFile);
                    fclose(tempFile);
                    /* send the data to application */
                    notifyXsldbgApp(XSLDBG_MSG_FILEOUT,
                                    filesTempFileName(1));
                } else {
                    xsltGenericError(xsltGenericErrorContext,
                                     "Unable to dump temporary results to %s\n",
                                     filesTempFileName(1));
                    res = xsltProfileStylesheet(cur, doc, params, stderr);
                }
            }
        } else {
            res = xsltApplyStylesheet(cur, doc, params);
        }
        if (optionsGetIntOption(OPTIONS_PROFILING)) {
            if (optionsGetIntOption(OPTIONS_REPEAT))
                endTimer("Applying stylesheet %d times",
                         optionsGetIntOption(OPTIONS_REPEAT));
            else
                endTimer("Applying stylesheet");
        }
        if (res == NULL) {
            xsltGenericError(xsltGenericErrorContext, "no result for %s\n",
                             optionsGetStringOption
                             (OPTIONS_OUTPUT_FILE_NAME));
            return;
        }
        if (optionsGetIntOption(OPTIONS_NOOUT)) {
            xmlFreeDoc(res);
            return;
        }
#ifdef LIBXML_DEBUG_ENABLED
        if (optionsGetIntOption(OPTIONS_DEBUG)) {
	    if (xslDebugStatus != DEBUG_RUN_RESTART){
		if (terminalIO != NULL)
		    xmlDebugDumpDocument(terminalIO, res);
		else if ((optionsGetStringOption(OPTIONS_OUTPUT_FILE_NAME) ==
			    NULL) || (getThreadStatus() != XSLDBG_MSG_THREAD_RUN)
			|| (filesTempFileName(1) == NULL))
		    xmlDebugDumpDocument(stdout, res);
		else {
		    FILE *tempFile = fopen(filesTempFileName(1), "w");

		    if (tempFile) {
			xmlDebugDumpDocument(tempFile, res);
			fclose(tempFile);
			/* send the data to application */
			notifyXsldbgApp(XSLDBG_MSG_FILEOUT,
				filesTempFileName(1));
		    } else {
			xsltGenericError(xsltGenericErrorContext,
				"Unable to dump temporary results to %s\n",
				filesTempFileName(1));
			xmlDebugDumpDocument(stdout, res);
		    }

		}
	    }
        } else {
#endif
	    if (xslDebugStatus != DEBUG_RUN_RESTART){
		if (cur->methodURI == NULL) {
		    if (optionsGetIntOption(OPTIONS_TIMING))
			startTimer();
		    if (xslDebugStatus != DEBUG_QUIT) {
			if (terminalIO != NULL)
			    xsltSaveResultToFile(terminalIO, res, cur);
			else if (optionsGetStringOption
				(OPTIONS_OUTPUT_FILE_NAME) == NULL)
			    xsltSaveResultToFile(stdout, res, cur);
			else
			    xsltSaveResultToFilename((const char *)
				    optionsGetStringOption
				    (OPTIONS_OUTPUT_FILE_NAME),
				    res, cur, 0);
		    }
		    if (optionsGetIntOption(OPTIONS_TIMING))
			endTimer("Saving result");
		} else {
		    if (xmlStrEqual(cur->method, (const xmlChar *) "xhtml")) {
			xsltGenericError(xsltGenericErrorContext,
				"non standard output xhtml\n");
			if (optionsGetIntOption(OPTIONS_TIMING))
			    startTimer();
			if (terminalIO != NULL)
			    xsltSaveResultToFile(terminalIO, res, cur);
			else if (optionsGetStringOption
				(OPTIONS_OUTPUT_FILE_NAME) == NULL)
			    xsltSaveResultToFile(stdout, res, cur);
			else
			    xsltSaveResultToFilename((const char *)
				    optionsGetStringOption
				    (OPTIONS_OUTPUT_FILE_NAME),
				    res, cur, 0);
			if (optionsGetIntOption(OPTIONS_TIMING))
			    endTimer("Saving result");
		    } else {
			xsltGenericError(xsltGenericErrorContext,
				"Unsupported non standard output %s\n",
				cur->method);
		    }
		}
	    }
#ifdef LIBXML_DEBUG_ENABLED
        }
#endif

        xmlFreeDoc(res);
    } else {
        xsltTransformContextPtr userCtxt = xsltNewTransformContext(cur, doc);
        if (userCtxt){
        xsltRunStylesheetUser(cur, doc, params,
                          (char *)optionsGetStringOption(OPTIONS_OUTPUT_FILE_NAME),
                          NULL, NULL, NULL, userCtxt);
        if (optionsGetIntOption(OPTIONS_TIMING))
            endTimer("Running stylesheet and saving result");
        xsltFreeTransformContext(userCtxt);
        }else{
                xsltGenericError(xsltGenericErrorContext, "Out of memory\n");
        }
    }
}


static void
usage(const char *name)
{
    xsltGenericError(xsltGenericErrorContext,
                     "Usage: %s [options] stylesheet file. See documenation of source and data commands\n", name);
    xsltGenericError(xsltGenericErrorContext,
                     "Without any parameters xsldbg starts in command mode, ready "
                     "for the source and data to be selected\n");
    xsltGenericError(xsltGenericErrorContext, "   Options:\n");
    /* Options help format is --<option>: <description>
     * or --<option> <para>: <description> */
    xsltGenericError(xsltGenericErrorContext,
                     "      --output file or -o file: Save to a given file. " \
		     "See output command documentatation\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --version or -V : Show the version of libxml and libxslt used\n");
    xsltGenericError(xsltGenericErrorContext,
		     "For documentation on the folowing \"flags\" " \
		     "see the documentation of the setoption command\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --verbose or -v : Show logs of what's happening\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --timing: Display the time used\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --repeat : Run the transformation 20 times\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --debug: Dump the tree of the result instead\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --novalid : Skip the DTD loading phase\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --noout : Do not dump the result\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --maxdepth val : Increase the maximum depth\n");
#ifdef LIBXML_HTML_ENABLED
    xsltGenericError(xsltGenericErrorContext,
                     "      --html : The input document is(are) an HTML file(s)\n");
#endif
#ifdef LIBXML_DOCB_ENABLED
    xsltGenericError(xsltGenericErrorContext,
                     "      --docbook : The input document is SGML docbook\n");
#endif
    xsltGenericError(xsltGenericErrorContext,
                     "      --param name value : Pass a (parameter,value) pair\n");
    xsltGenericError(xsltGenericErrorContext,
                     "            string values must be quoted like \"'string'\"\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --nonet : Refuse to fetch DTDs or entities over network\n");
#ifdef LIBXML_CATALOG_ENABLED
#ifdef __riscos
    xsltGenericError(xsltGenericErrorContext,
                     "      --catalogs : Use the catalogs from SGML$CatalogFiles\n");
#else
    xsltGenericError(xsltGenericErrorContext,
                     "      --catalogs : Use the catalogs from $SGML_CATALOG_FILES\n");
#endif
#endif
#ifdef LIBXML_XINCLUDE_ENABLED
    xsltGenericError(xsltGenericErrorContext,
                     "      --xinclude : do XInclude processing on document intput\n");
#endif
    xsltGenericError(xsltGenericErrorContext,
                     "      --profile or --norman : dump profiling informations \n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --shell : Always go to the xsldbg prompt, to start debugging\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --gdb : Run in gdb mode printing out more information\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --autoencode : Detect and use encodings in the stylesheet\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --utf8input : Treat command line input as encoded in UTF-8\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --preferhtml : Use html output when generating search reports.\n"
                     "                     See search command\n");
  xsltGenericError(xsltGenericErrorContext,
		   "        --stdout : Print all error messages to stdout " \
		   "normally error messages go to stderr\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --cd <PATH> : Change to specfied working directory\n");


}

int
xsldbgMain(int argc, char **argv)
{
    int i, result = 1, noFilesFound = 0;
    xsltStylesheetPtr cur = NULL;
    xmlChar *expandedName;      /* contains file name with path expansion if any */

    /* in some cases we always want to bring up a command prompt */
    int showPrompt;

    /* the xml document we're processing */
    xmlDocPtr doc;

    errorFile = stderr;

#ifdef __riscos
    /* Remember our invocation command such that we may call ourselves */
    xsldbgCommand = argv[0];
#endif

    xmlInitMemory();



    LIBXML_TEST_VERSION xmlLineNumbersDefault(1);

    if (!xsldbgInit()) {
        xsltGenericError(xsltGenericErrorContext,		       
                "Internal error, maybe ran out of memory aborting xsldbg\n");
        xsldbgFree();
        xsltCleanupGlobals();
        xmlCleanupParser();
        xmlMemoryDump();
        return (1);
    }

    if (argc == 1)
        result = optionsSetIntOption(OPTIONS_SHELL, 1);

    if (optionsGetIntOption(OPTIONS_NOVALID) == 0)
        xmlLoadExtDtdDefaultValue = XML_DETECT_IDS | XML_COMPLETE_ATTRS;
    else
        xmlLoadExtDtdDefaultValue = 0;
    for (i = 1; i < argc; i++) {
        if (!result)
            break;
        if (argv[i][0] != '-') {
            expandedName = filesExpandName((xmlChar *) argv[i]);
            if (!expandedName) {
                result = 0;
                break;
            }
            switch (noFilesFound) {
                case 0:
                    optionsSetStringOption(OPTIONS_SOURCE_FILE_NAME,
                                           expandedName);
                    noFilesFound++;
                    break;
                case 1:
                    optionsSetStringOption(OPTIONS_DATA_FILE_NAME,
                                           expandedName);
                    noFilesFound++;
                    break;

                default:
                    xsltGenericError(xsltGenericErrorContext,
                                     "Too many file names supplied\n");
                    result = 0;
            }
            xmlFree(expandedName);
            continue;
        }

        if ((argv[i][0] == '-') && (argv[i][1] == '-'))
            argv[i]++;          /* treat --<OPTION_NAME> as -<OPTION_NAME> */

#ifdef LIBXML_DEBUG_ENABLED
        if (!xmlStrCmp(argv[i], "-debug")) {
            if (result) {
                result = optionsSetIntOption(OPTIONS_DEBUG, 1);
                argv[i] = NULL;
            }
        } else
#endif
        if ((!xmlStrCmp(argv[i], "-v")) || (!xmlStrCmp(argv[i], "-verbose"))) {
            xsltSetGenericDebugFunc(stderr, NULL);
        } else if ((xmlStrEqual((xmlChar*)argv[i], (xmlChar*)"-o")) ||
                   (xmlStrEqual((xmlChar*)argv[i], (xmlChar*)"-output"))) {
	    argv[i] = NULL;
	    i++;
	    result = xslDbgShellOutput((xmlChar*)argv[i]);
	    argv[i] = NULL;
        } else if ((xmlStrEqual((xmlChar*)argv[i], (xmlChar*)"-V")) ||
                   (xmlStrEqual((xmlChar*)argv[i], (xmlChar*)"-version"))) {
            xsltGenericError(xsltGenericErrorContext,
                             " xsldbg created by Keith Isdale <k_isdale@tpg.com.au>\n");
            xsltGenericError(xsltGenericErrorContext,
                             " Version %s, Date created %s\n", VERSION,
                             TIMESTAMP);
            xsltGenericError(xsltGenericErrorContext,
                             "Using libxml %s, libxslt %s and libexslt %s\n",
                             xmlParserVersion, xsltEngineVersion,
                             exsltLibraryVersion);
            xsltGenericError(xsltGenericErrorContext,
                             "xsldbg was compiled against libxml %d, libxslt %d and libexslt %d\n",
                             LIBXML_VERSION, LIBXSLT_VERSION,
                             LIBEXSLT_VERSION);
            xsltGenericError(xsltGenericErrorContext,
                             "libxslt %d was compiled against libxml %d\n",
                             xsltLibxsltVersion, xsltLibxmlVersion);
            xsltGenericError(xsltGenericErrorContext,
                             "libexslt %d was compiled against libxml %d\n",
                             exsltLibexsltVersion, exsltLibxmlVersion);
            argv[i] = NULL;
        } else if (xmlStrEqual((xmlChar*)argv[i], (xmlChar*)"-norman")) {
            if (result) {
                result = optionsSetIntOption(OPTIONS_PROFILING, 1);
                argv[i] = NULL;
            }
        } else if (xmlStrEqual((xmlChar*)argv[i], (xmlChar*)"-nonet")) {
            xmlSetExternalEntityLoader(xmlNoNetExternalEntityLoader);
        } else if (xmlStrEqual((xmlChar*)argv[i], (xmlChar*)"-param")) {
            i++;
            arrayListAdd(optionsGetParamItemList(),
                         optionsParamItemNew((xmlChar *) argv[i],
                                             (xmlChar *) argv[i + 1]));
            i++;
            if (arrayListCount(optionsGetParamItemList()) >= 32) {
                xsltGenericError(xsltGenericErrorContext,
                                 "Too many params\n");
                return (1);
            }
        } else if (xmlStrEqual((xmlChar*)argv[i], (xmlChar*)"-maxdepth")) {
            int value;

            argv[i] = NULL;
            i++;
            if (sscanf(argv[i], "%d", &value) == 1) {
                if (value > 0)
                    xsltMaxDepth = value;
            }
            argv[i] = NULL;


        } else if (xmlStrEqual((xmlChar*)argv[i], (xmlChar*)"-repeat")) {
            if (optionsGetIntOption(OPTIONS_REPEAT) == 0)
                optionsSetIntOption(OPTIONS_REPEAT, 20);
            else
                optionsSetIntOption(OPTIONS_REPEAT, 100);

        } else if (xmlStrEqual((xmlChar*)argv[i], (xmlChar*)"-cd")) {
            argv[i] = NULL;
            if (i + 1 < argc) {
                i++;
                result = changeDir((xmlChar *) argv[i]);
                argv[i] = NULL;
            } else {
                xsltGenericError(xsltGenericErrorContext,
                                 "Missing path name after -cd option\n");
            }

        } else {
            /* From here we're only dealing with integer options */
            /* ignore any non-user option */
            if (result && (argv[i][2] != '*')) {
                int optID = optionsGetOptionID((xmlChar *) argv[i] + 1);

                /* the user might have entered a string option so reject it if so */
                if ((optID >= OPTIONS_XINCLUDE)
                    && (optID <= OPTIONS_VERBOSE)) {
                    result = optionsSetIntOption(optID, optionsGetIntOption(optID) + 1);
                    argv[i] = NULL;
                } else {
                    xsltGenericError(xsltGenericErrorContext,
                                     "Error: Unknown option %s, or unknown "
                                     " integer option. See setoption commmand in "
                                     "xsldbg documentation for full description of "
                                     "integer and string options\n",
                                     argv[i]);
                    result = 0;
                }
            }
        }
    }

    if (!result) {
        usage(argv[0]);
        xsldbgFree();
        return (1);
    }


    /*
     * * Replace entities with their content.
     */
    xmlSubstituteEntitiesDefault(1);

    /*
     * * Register the EXSLT extensions and the test module
     */
    exsltRegisterAll();
    xsltRegisterTestModule();

    /* copy the volitile options over to xsldbg */
    optionsCopyVolitleOptions();

    /*
     * shell interraction
     */
    if (!optionsGetIntOption(OPTIONS_SHELL)) {  /* excecute stylesheet (ie no debugging) */
        xslDebugStatus = DEBUG_NONE;
    } else {
        xslDebugStatus = DEBUG_STOP;
        xsltGenericError(xsltGenericErrorContext, "XSLDBG %s\n", VERSION);
    }


    debugGotControl(0);
    while (xslDebugStatus != DEBUG_QUIT) {
        /* don't force xsldbg to show command prompt */
        showPrompt = 0;
        cur = NULL;
        doc = NULL;
        arrayListEmpty(filesEntityList());
	xsltSetXIncludeDefault(optionsGetIntOption(OPTIONS_XINCLUDE));

        /* copy the volitile options over to xsldbg */
        optionsCopyVolitleOptions();

	/* choose where error messages/xsldbg output get sent to */
	if (optionsGetIntOption(OPTIONS_STDOUT))
	  errorFile = stdout;
	else
	  errorFile = stderr;

	filesLoadCatalogs();

        if (optionsGetIntOption(OPTIONS_SHELL)) {
            debugGotControl(0);
            xsltGenericError(xsltGenericErrorContext,
                             "\nStarting stylesheet\n\n");
            if (optionsGetIntOption(OPTIONS_TRACE) == TRACE_OFF)
                xslDebugStatus = DEBUG_STOP;    /* stop as soon as possible */
        }

        if ((optionsGetStringOption(OPTIONS_SOURCE_FILE_NAME) == NULL) ||
            (optionsGetStringOption(OPTIONS_DATA_FILE_NAME) == NULL)) {
            /* at least on file name has not been set */
            /*goto a xsldbg command prompt */
            showPrompt = 1;
            if (optionsGetStringOption(OPTIONS_SOURCE_FILE_NAME) == NULL)
                xsltGenericError(xsltGenericErrorContext,
                                 "Error: No source file supplied\n");

            if (optionsGetStringOption(OPTIONS_DATA_FILE_NAME) == NULL) {
                xsltGenericError(xsltGenericErrorContext,
                                 "Error: No data file supplied\n");
                if (optionsGetStringOption(OPTIONS_SOURCE_FILE_NAME) !=
                    NULL)
                    xsltGenericError(xsltGenericErrorContext,
                                     "Supplied with source file of %s\n",
                                     optionsGetStringOption
                                     (OPTIONS_SOURCE_FILE_NAME));
            }

        } else {
            filesLoadXmlFile(NULL, FILES_SOURCEFILE_TYPE);
            cur = filesGetStylesheet();
            if ((cur == NULL) || (cur->errors != 0)) {
                /*goto a xsldbg command prompt */
                showPrompt = 1;
                if (xslDebugStatus == DEBUG_NONE) {
                    xslDebugStatus = DEBUG_QUIT;        /* panic !! */
                    result = 0;
                }
            }
        }

        if (showPrompt == 0) {
            filesLoadXmlFile(NULL, FILES_XMLFILE_TYPE);
            doc = filesGetMainDoc();
            if (doc == NULL) {
                if (xslDebugStatus == DEBUG_NONE) {
                    xslDebugStatus = DEBUG_QUIT;        /* panic !! */
                    result = 0;
                } else {
                    /*goto a xsldbg command prompt */
                    showPrompt = 1;
                }
            } else {
                if (xslDebugStatus != DEBUG_QUIT) {
                    xsltProcess(doc, cur);
                    result = 1;
                }
            }

            if (optionsGetIntOption(OPTIONS_SHELL) && (showPrompt == 0)) {
                if ((xslDebugStatus != DEBUG_QUIT)
                    && !debugGotControl(-1)) {
                    xsltGenericError(xsltGenericErrorContext,
                                     "\nDebugger never received control\n");
                    /*goto a xsldbg command prompt */
                    showPrompt = 1;
                    xslDebugStatus = DEBUG_STOP;
                } else {
                    xsltGenericError(xsltGenericErrorContext,
                                     "\nFinished stylesheet\n\032\032\n");
                    {
                        /* handle trace execution */
                        int trace = optionsGetIntOption(OPTIONS_TRACE);

                        switch (trace) {
                            case TRACE_OFF:
                                /* no trace of execution */
                                break;

                            case TRACE_ON:
                                /* tell xsldbg to stop tracing next time we get here */
                                optionsSetIntOption(OPTIONS_TRACE,
                                                    TRACE_RUNNING);
                                xslDebugStatus = DEBUG_TRACE;
                                break;

                            case TRACE_RUNNING:
                                /* turn off tracing */
                                xslDebugStatus = DEBUG_CONT;
                                optionsSetIntOption(OPTIONS_TRACE,
                                                    TRACE_OFF);
                                break;
                        }
                    }
		    if (!optionsGetIntOption(OPTIONS_AUTORESTART) && (xslDebugStatus != DEBUG_RUN_RESTART)){ 
		    /* pass control to user they won't be able to do much
		      other than add breakpoints, quit, run, continue */
		    debugXSLBreak((xmlNodePtr) cur->doc, (xmlNodePtr) doc,
                              NULL, NULL);
		    }
                }
            } else {
                /* request to execute stylesheet only  so we're done */
                xslDebugStatus = DEBUG_QUIT;
            }
        } else {
            /* Some sort of problem loading source file has occured. Quit? */
            if (xslDebugStatus == DEBUG_NONE) {
                xslDebugStatus = DEBUG_QUIT;    /* Panic!! */
                result = 0;
            } else {
                /*goto a xsldbg command prompt */
                showPrompt = 1;
            }
        }

        if (showPrompt && optionsGetIntOption(OPTIONS_SHELL)) {
            xmlDocPtr tempDoc = xmlNewDoc((xmlChar *) "1.0");
            xmlNodePtr tempNode =
                xmlNewNode(NULL, (xmlChar *) "xsldbg_default_node");
            if (!tempDoc || !tempNode) {
                xsldbgFree();
                return (1);
            }
            xmlAddChild((xmlNodePtr) tempDoc, tempNode);

            xsltGenericError(xsltGenericErrorContext,
                             "Going straight to "
                             "command shell! Not all xsldbg commands will "
                             "work as not all needed have been loaded \n");
	    xslDebugStatus = DEBUG_STOP;
            if ((cur == NULL) && (doc == NULL)) {
                /*no doc's loaded */
                debugXSLBreak(tempNode, tempNode, NULL, NULL);
            } else if ((cur != NULL) && (doc == NULL)) {
                /* stylesheet is loaded */
                debugXSLBreak((xmlNodePtr) cur->doc, tempNode, NULL, NULL);
            } else if ((cur == NULL) && (doc != NULL)) {
                /* xml doc is loaded */
                debugXSLBreak(tempNode, (xmlNodePtr) doc, NULL, NULL);
            } else {
                /* unexpected problem, both docs are loaded */
                debugXSLBreak((xmlNodePtr) cur->doc, (xmlNodePtr) doc,
                              NULL, NULL);
            }
            xmlFreeDoc(tempDoc);
        } else if (showPrompt && !optionsGetIntOption(OPTIONS_SHELL)) {
            xslDebugStatus = DEBUG_QUIT;
            result = 0;         /* panic */
        }

        if (optionsGetIntOption(OPTIONS_SHELL)) {
            /* force a refesh of both stlesheet and xml data */
            filesFreeXmlFile(FILES_SOURCEFILE_TYPE);
            filesFreeXmlFile(FILES_XMLFILE_TYPE);
        }
    }

    if (!result) {
        xsltGenericError(xsltGenericErrorContext,
                         "xsldbg is aborting due to unrecoverable errors!\n");
    }
    xsldbgFree();
    xsltCleanupGlobals();
    xmlCleanupParser();
    xmlMemoryDump();
    return !result;
}



/**
 * xsldbgLoadStylesheet:
 *
 * Load the stylesheet and return it 
 *
 * Returns The stylesheet after reloading it if successful
 *         NULL otherwise
 */
xsltStylesheetPtr
xsldbgLoadStylesheet(void)
{
    xsltStylesheetPtr cur = NULL;
    xmlDocPtr style;

    if (optionsGetIntOption(OPTIONS_TIMING))
        startTimer();
    style = xmlParseFile((const char *)
                         optionsGetStringOption(OPTIONS_SOURCE_FILE_NAME));
    if (optionsGetIntOption(OPTIONS_TIMING))
        endTimer("Parsing stylesheet %s",
                 optionsGetStringOption(OPTIONS_SOURCE_FILE_NAME));
    if (style == NULL) {
        xsltGenericError(xsltGenericErrorContext, "cannot parse %s\n",
                         optionsGetStringOption(OPTIONS_SOURCE_FILE_NAME));
        cur = NULL;
        if (!optionsGetIntOption(OPTIONS_SHELL)) {
            xsltGenericError(xsltGenericErrorContext,
                             "Aborting debugger!!\n");
            xslDebugStatus = DEBUG_QUIT;
        } else {
            xsltGenericError(xsltGenericErrorContext, "\n");
            xslDebugStatus = DEBUG_STOP;
        }
    } else {
        cur = xsltLoadStylesheetPI(style);
        if (cur != NULL) {
            /* it is an embedded stylesheet */
            xsltProcess(style, cur);
            xsltFreeStylesheet(cur);
        } else {
            cur = xsltParseStylesheetDoc(style);
            if (cur != NULL) {
                if (cur->indent == 1)
                    xmlIndentTreeOutput = 1;
                else
                    xmlIndentTreeOutput = 0;
            } else {
                xmlFreeDoc(style);
            }
        }
    }
    return cur;
}



/**
 * xsldbgLoadXmlData:
 *
 * Load the xml data file and return it  
 *
 * Returns The data file after reloading it if successful
 *         NULL otherwise
 */
xmlDocPtr
xsldbgLoadXmlData(void)
{
    xmlDocPtr doc = NULL;
    doc = NULL;

    if (optionsGetIntOption(OPTIONS_TIMING))
        startTimer();
#ifdef LIBXML_HTML_ENABLED
    if (optionsGetIntOption(OPTIONS_HTML))
        doc = htmlParseFile((char *)
                            optionsGetStringOption(OPTIONS_DATA_FILE_NAME),
                            NULL);
    else
#endif
#ifdef LIBXML_DOCB_ENABLED
    if (optionsGetIntOption(OPTIONS_DOCBOOK))
        doc = docbParseFile((char *)
                            optionsGetStringOption(OPTIONS_DATA_FILE_NAME),
                            NULL);
    else
#endif
        doc = xmlSAXParseFile(&xmlDefaultSAXHandler,
			     (char *) optionsGetStringOption(OPTIONS_DATA_FILE_NAME), 0);
    if (doc == NULL) {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Unable to parse %s\n",
                         optionsGetStringOption(OPTIONS_DATA_FILE_NAME));
        if (!optionsGetIntOption(OPTIONS_SHELL)) {
            xsltGenericError(xsltGenericErrorContext,
                             "Aborting debugger!!\n");
            xslDebugStatus = DEBUG_QUIT;
        } else {
            xsltGenericError(xsltGenericErrorContext, "\n");
            xslDebugStatus = DEBUG_STOP;
        }
    } else if (optionsGetIntOption(OPTIONS_TIMING))
        endTimer("Parsing document %s",
                 optionsGetStringOption(OPTIONS_DATA_FILE_NAME));

    return doc;
}


/**
 * xsldbgLoadXmlTemporary:
 * @path: The name of temporary file to load 
 *
 * Load the temporary data file and return it 
 *
 * Returns The temporary file after reloading it if successful,
 *         NULL otherwise
 */
xmlDocPtr
xsldbgLoadXmlTemporary(const xmlChar * path)
{
    xmlDocPtr doc = NULL;
    doc = NULL;

    if (optionsGetIntOption(OPTIONS_TIMING))
        startTimer();
#ifdef LIBXML_HTML_ENABLED
    if (optionsGetIntOption(OPTIONS_HTML))
        doc = htmlParseFile((char *) path, NULL);
    else
#endif
#ifdef LIBXML_DOCB_ENABLED
    if (optionsGetIntOption(OPTIONS_DOCBOOK))
        doc = docbParseFile((char *) path, NULL);
    else
#endif
        doc = xmlParseFile((char *) path);
    if (doc == NULL) {
        xsltGenericError(xsltGenericErrorContext, "unable to parse %s\n",
                         path);
    }

    if (optionsGetIntOption(OPTIONS_TIMING)
        && (xslDebugStatus != DEBUG_QUIT)) {
        endTimer("Parsing document %s", path);
    }
    return doc;
}

/**
 * printTemplates:
 * @style : valid as parsed my xsldbg
 * @doc :    "    "   "     "    "
 *  
 * print out list of template names
 */
void
printTemplates(xsltStylesheetPtr style, xmlDocPtr doc)
{
    xsltTransformContextPtr ctxt = xsltNewTransformContext(style, doc);

    if (ctxt) {
        /* don't be verbose when printing out template names */
        xslDbgShellPrintTemplateNames(ctxt, NULL, NULL, 0, 0);
    } else {
        xsltGenericError(xsltGenericErrorContext,
                         "Unable to create context : print templates\n");
    }
}

#ifdef WIN32

/* For the windows world we capture the control event */
BOOL WINAPI
handler_routine(DWORD dwCtrlType)
{

    switch (dwCtrlType) {
        case CTRL_C_EVENT:
        case CTRL_BREAK_EVENT:
        case CTRL_CLOSE_EVENT:
            catchSigInt(SIGINT);
            break;

        case CTRL_LOGOFF_EVENT:
        case CTRL_SHUTDOWN_EVENT:
            xsldbgFree();
            exit(1);
            break;

        default:
            printf("Error: Unknown control event\n");
            break;
    }

    return (TRUE);
}

#endif


/**
 * catchSigInt:
 * @value : not used
 *
 * Recover from a signal(SIGINT), exit if needed
 */
void
catchSigInt(int value ATTRIBUTE_UNUSED)
{
    if ((xslDebugStatus == DEBUG_NONE) || (xsldbgStop == 1) || (xslDebugStatus == DEBUG_STOP)) {
        xsldbgFree();
        exit(1);
    }
#ifdef __riscos
    /* re-catch SIGINT - RISC OS resets the handler when the interupt occurs */
    signal(SIGINT, catchSigInt);
#endif

    if (xslDebugStatus != DEBUG_STOP) {
        /* stop running/walking imediately !! */
        xsldbgStop = 1;
    }
}


/**
 * catchSigTerm:
 * @value : not used
 *
 * Clean up and exit
 */
void
catchSigTerm(int value ATTRIBUTE_UNUSED)
{
    xsldbgFree();
    exit(1);
}



typedef void (*sighandler_t) (int);
static sighandler_t oldHandler;

static int initialized = 0;

/**
 * xsldbgInit:
 * 
 * Returns 1 if able to allocate memory needed by xsldbg
 *         0 otherwise
 */
int
xsldbgInit()
{
    int result = 0;
    int xmlVer = 0;

    if (!initialized) {
        sscanf(xmlParserVersion, "%d", &xmlVer);
        xsldbgHasLineNumberFix = (xmlVer >= 20508);
        if (!debugInit()) {
            xsltGenericError(xsltGenericErrorContext,
                             "Init of debug module failed\n");
            return result;
        }
        if (!filesInit()) {
            xsltGenericError(xsltGenericErrorContext,
                             "Init of files module failed\n");
            return result;
        }

        if (!optionsInit()) {
            xsltGenericError(xsltGenericErrorContext,
                             "Init of options module failed\n");
            return result;
        }

        if (!searchInit()) {
            xsltGenericError(xsltGenericErrorContext,
                             "Init of search module failed\n");
            return result;
        }

	

        /* set up the parser */
        xmlInitParser();
        xmlSetGenericErrorFunc(0, xsldbgGenericErrorFunc);
        xsltSetGenericErrorFunc(0, xsldbgGenericErrorFunc);

	/*
	 * disable CDATA from being built in the document tree
	 */
	xmlDefaultSAXHandlerInit();
	xmlDefaultSAXHandler.cdataBlock = NULL;
if (xsldbgHasLineNumberFix){
	oldGetEntity = xmlDefaultSAXHandler.getEntity;
	xmlDefaultSAXHandler.getEntity = xsldbgGetEntity;
}

        if (getThreadStatus() != XSLDBG_MSG_THREAD_NOTUSED) {
            initialized = 1;
            return 1;           /* this is all we need to do when running as a thread */
        }
#ifndef WIN32
        /* catch SIGINT */
        oldHandler = signal(SIGINT, catchSigInt);
#else
        if (SetConsoleCtrlHandler(handler_routine, TRUE) != TRUE)
            return result;
#endif

#ifndef WIN32
        /* catch SIGTIN tty input available fro child */
        signal(SIGTERM, catchSigTerm);
#endif
        initialized = 1;
    }
    return 1;
}

/**
 * xsldbgFree:
 *
 * Free memory used by xsldbg
 */
void
xsldbgFree()
{
    debugFree();
    filesFree();
    optionsFree();
    searchFree();
#ifndef WIN32
    if (oldHandler != SIG_ERR)
        signal(SIGINT, oldHandler);
#else
    SetConsoleCtrlHandler(handler_routine, FALSE);
#endif
    initialized = 0;

#ifdef HAVE_READLINE
    /*  rl_free_line_state ();
      rl_cleanup_after_signal(); */
#   ifdef HAVE_HISTORY
       clear_history();
#   endif    
#endif

}


char msgBuffer[4000];

/**
 * xsldbgGenericErrorFunc:
 * @ctx:  Is Valid
 * @msg:  Is valid
 * @...:  other parameters to use
 * 
 * Handles print output from xsldbg and passes it to the application if 
 *  running as a thread otherwise send to stderr
 */
void
xsldbgGenericErrorFunc(void *ctx, const char *msg, ...)
{
    va_list args;

    va_start(args, msg);
    if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN) {
        vsnprintf(msgBuffer, sizeof(msgBuffer), msg, args);
        notifyTextXsldbgApp(XSLDBG_MSG_TEXTOUT, msgBuffer);
    } else {
        xmlChar *encodeResult = NULL;

        vsnprintf(msgBuffer, sizeof(msgBuffer), msg, args);
        encodeResult = filesEncode((xmlChar *) msgBuffer);
        if (encodeResult) {
            fprintf(errorFile, "%s", encodeResult);
            xmlFree(encodeResult);
        } else
            fprintf(errorFile, "%s", msgBuffer);
    }
    va_end(args);
}
