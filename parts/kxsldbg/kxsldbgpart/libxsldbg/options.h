
/***************************************************************************
                          options.h  -  define option related functions
                             -------------------
    begin                : Sat Nov 10 2001
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

#ifndef OPTIONS_H
#define OPTIONS_H

#include "arraylist.h"

#ifdef USE_KDE_DOCS

/**
 * Provide a mechanism to change option. The Options structure is not in use,
 *   it has been added so that kdoc puts all option related 
 *   functions together 
 *
 * @short file support
 *
 * @author Keith Isdale <k_isdale@tpg.com.au> 
 */
#endif

/* We want skip most of these includes when building documentation */
#ifndef BUILD_DOCS
#include <libxslt/xslt.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif


/* ---------------------------------------  
        Misc options
-------------------------------------------*/

/** The largest number lines of text can be print print printing documents
    This is equivant to gdb shorting of evaluation values
 */
#define GDB_LINES_TO_PRINT 3


#ifndef USE_KDOC
    typedef enum {
        OPTIONS_CONFIG_READVALUE = -1,  /* Read configuration flag */
        OPTIONS_CONFIG_READING = 1,     /* Configuration file is being read */
        OPTIONS_CONFIG_WRITING, /* Configuration file is being written */
        OPTIONS_CONFIG_IDLE     /* We are neither reading or writing */
    } OptionsConfigState;
#else
    /* keep kdoc happy */
    enum OptionsConfigState {
        OPTIONS_CONFIG_READVALUE = -1,  /* Read configuration flag */
        OPTIONS_CONFIG_READING = 1,     /* Configuration file is being read */
        OPTIONS_CONFIG_WRITING, /* Configuration file is being written */
        OPTIONS_CONFIG_IDLE     /* We are neither reading or writing */
    };
#endif

#ifndef USE_KDOC
    typedef enum {
        OPTIONS_XINCLUDE = 500, /* Use xinclude during xml parsing */
        OPTIONS_DOCBOOK,        /* Use of docbook sgml parsing */
        OPTIONS_TIMING,         /* Use of timing */
        OPTIONS_PROFILING,      /* Use of profiling */
        OPTIONS_NOVALID,        /* Disable file validation */
        OPTIONS_NOOUT,          /* Disables output to stdout */
        OPTIONS_HTML,           /* Enable the use of html parsing */
        OPTIONS_DEBUG,          /* Enable the use of xml tree debugging */
        OPTIONS_SHELL,          /* Enable the use of debugger shell */
        OPTIONS_GDB,            /* Run in gdb modem prints more messages) */
        OPTIONS_REPEAT,         /* The number of times to repeat */
        OPTIONS_TRACE,          /* Trace execution */
        OPTIONS_WALK_SPEED,     /* How fast do we walk through code */
        OPTIONS_CATALOGS,       /* Get the catalogs from SGML_CATALOG_FILES and
                                 * store it in OPTIONS_CATALOG_NAMES */
        OPTIONS_PREFER_HTML,    /* Prefer html output for search results */
        OPTIONS_AUTOENCODE,     /* Try to use the encoding from the stylesheet */
        OPTIONS_UTF8_INPUT,     /* All input from user is in UTF-8.This is normaly 
                                 * used when xsldbg is running as a thread */
	OPTIONS_STDOUT,        /* Print all error messages to  stdout, 
				 * normally error messages go to stderr */
    	OPTIONS_AUTORESTART, 	/* When finishing the debug of a XSLT script 
				   automaticly restart at the beginning */
        OPTIONS_VERBOSE,        /* Be verbose with messages */
        OPTIONS_OUTPUT_FILE_NAME,       /* what is the output file name */
        OPTIONS_SOURCE_FILE_NAME,       /*  the stylesheet source to use */
        OPTIONS_DOCS_PATH,      /* path of xsldbg's documentation */
        OPTIONS_CATALOG_NAMES,  /* the names of the catalogs to use when catalogs option is active */
        OPTIONS_ENCODING,       /* What encoding to use for standard output */
        OPTIONS_SEARCH_RESULTS_PATH,    /* Where do we store the results of searching */
        OPTIONS_DATA_FILE_NAME  /* xml data file to use */
    } OptionTypeEnum;



    /* define what tracing is used */
    typedef enum {
        TRACE_OFF = 600,        /* disable tracing */
        TRACE_ON,               /* enable tracing */
        TRACE_RUNNING,          /* tracing is running */
        TRACE_FINISHED          /* not needed but just in case */
    } TraceModeEnum;

    /* what speeds can we walk through a stylesheet */
    /* must start at zero !! */
    typedef enum {
        WALKSPEED_0,
        WALKSPEED_STOP = WALKSPEED_0,
        WALKSPEED_1,
        WALKSPEED_FAST = WALKSPEED_1,
        WALKSPEED_2,
        WALKSPEED_3,
        WALKSPEED_4,
        WALKSPEED_5,
        WALKSPEED_NORMAL = WALKSPEED_5,
        WALKSPEED_6,
        WALKSPEED_7,
        WALKSPEED_8,
        WALKSPEED_9,
        WALKSPEED_SLOW = WALKSPEED_9
    } WalkSpeedEnum;

#else
    /* keep kdoc happy */
    enum OptionsTypeEnum {
        OPTIONS_XINCLUDE = 500, /* Use xinclude during xml parsing */
        OPTIONS_DOCBOOK,        /* Use of docbook sgml parsing */
        OPTIONS_TIMING,         /* Use of timing */
        OPTIONS_PROFILING,      /* Use of profiling */
        OPTIONS_NOVALID,        /* Disable file validation */
        OPTIONS_NOOUT,          /* Disables output to stdout */
        OPTIONS_HTML,           /* Enable the use of html parsing */
        OPTIONS_DEBUG,          /* Enable the use of xml tree debugging */
        OPTIONS_SHELL,          /* Enable the use of debugger shell */
        OPTIONS_GDB,            /* Run in gdb modem prints more messages) */
        OPTIONS_REPEAT,         /* The number of times to repeat */
        OPTIONS_TRACE,          /* Trace execution */
        OPTIONS_WALK_SPEED,     /* How fast do we walk through code */
        OPTIONS_CATALOGS,       /* Get the catalogs from SGML_CATALOG_FILES and
                                 * store it in OPTIONS_CATALOG_NAMES */
        OPTIONS_PREFER_HTML,    /* Prefer html output for search results */
        OPTIONS_AUTOENCODE,     /* try to use the encoding from the stylesheet */
        OPTIONS_UTF8_INPUT,     /* All input from user is in UTF-8.This normaly 
                                 * used when xsldbg is running as a thread */
	OPTIONS_STDOUT,        /* Print all error messages to  stdout, 
				 * normally error messages go to stderr */
        OPTIONS_VERBOSE,        /* Be verbose with messages */
        OPTIONS_OUTPUT_FILE_NAME,       /* what is the output file name */
        OPTIONS_SOURCE_FILE_NAME,       /*  the stylesheet source to use */
        OPTIONS_DOCS_PATH,      /* path of xsldbg's documentation */
        OPTIONS_CATALOG_NAMES,  /* the names of the catalogs to use when catalogs option is active */
        OPTIONS_ENCODING,       /* What encoding to use for standard output */
        OPTIONS_SEARCH_RESULTS_PATH,    /* Where do we store the results of searching */
        OPTIONS_DATA_FILE_NAME  /* xml data file to use */
    };

    /* define what tracing is used */
    enum TraceModeEnum {
        TRACE_OFF = 600,        /* disable tracing */
        TRACE_ON,               /* enable tracing */
        TRACE_RUNNING,          /* tracing is running */
        TRACE_FINISHED          /* not needed but just in case */
    };

    /* what speeds can we walk through a stylesheet */
    /* must start walkSpeed enums from zero !! */
    enum WalkSpeedEnum {
        WALKSPEED_0 = 0,
        WALKSPEED_STOP = WALKSPEED_0,
        WALKSPEED_1,
        WALKSPEED_FAST = WALKSPEED_1,
        WALKSPEED_2,
        WALKSPEED_3,
        WALKSPEED_4,
        WALKSPEED_5,
        WALKSPEED_NORMAL = WALKSPEED_5,
        WALKSPEED_6,
        WALKSPEED_7,
        WALKSPEED_8,
        WALKSPEED_9,
        WALKSPEED_SLOW = WALKSPEED_9
    };
#endif

/* how many microseconds is each speed increase worth*/
#define WALKDELAY 250000


/* for non win32 environments see the macro in xsldebugger/Makefile.am
   Win32 type systems see macro in libxslt/xsltwin32config.h
*/
#ifdef __riscos

/* The environment variable name we are going to use is the readable version
   of the application name */
#define XSLDBG_DOCS_DIR_VARIABLE "XSLDebugDocs$Dir"
#else

/* The environment variable name on normal systems */
#define XSLDBG_DOCS_DIR_VARIABLE "XSLDBG_DOCS_DIR"
#endif

/* used to keep track of libxslt paramters 
 see Parameter related options near end of file
*/
    typedef struct _parameterItem parameterItem;
    typedef parameterItem *parameterItemPtr;
    struct _parameterItem {
        xmlChar *name;          /* libxslt parameter name */
        xmlChar *value;         /* libxslt parameter value */
        int intValue;           /* reserved */
    };



#ifdef USE_GNOME_DOCS

/** 
 * optionsInit:
 *
 * Intialize the options module
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/** 
 * Initialized the options module
 *
 * @returns 1 on success,
 *          0 otherwise
 */
#endif
#endif
    int optionsInit(void);



#ifdef USE_GNOME_DOCS

/**
 * optionsFree:
 *
 * Free memory used by the options module
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Free memory used by the options module
 */
#endif
#endif
    void optionsFree(void);


#ifdef USE_GNOME_DOCS

  /**
   * optionsGetOptionID:
   * @optionName : A valid option name see documentation for "setoption" 
   *        command and program usage documentation
   *
   * Find the option id for a given option name
   *
   * Returns The optionID for @optionName if successful, where  
   *             OPTIONS_XINCLUDE<= optionID <= OPTIONS_DATA_FILE_NAME,
   *         otherwise returns -1
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Find the option id for a given option name
   *
   * @returns The optionID for @optionName if successful, where  
   *             OPTIONS_XINCLUDE<= optionID <= OPTIONS_DATA_FILE_NAME,
   *         otherwise returns -1
   *
   * @param optionName A valid option name see documentation for "setoption" 
   *        command and program usage documentation
   *
   */
#endif
#endif
    int optionsGetOptionID(xmlChar * optionName);



#ifdef USE_GNOME_DOCS

  /**
   * optionsGetOptionName:
   * @ID : A valid option ID
   *
   * Get the name text for an option
   *
   * Returns The name of option if @ID is valid, 
   *         NULL otherwise 
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Get the name text for an option
   *
   * Returns The name of option if @ID is valid, 
   *         NULL otherwise 
   *
   * @param ID A valid option ID
   *
   */
#endif
#endif
    const xmlChar *optionsGetOptionName(OptionTypeEnum ID);


#ifdef USE_GNOME_DOCS

/**
 * optionsSetIntOption:
 * @optionType: Is a valid integer option
 * @value: Value to adopt
 *
 * Set the value of an integer xsldbg option to @value
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Set the value of an integer xsldbg option to @p value
 *
 * @returns 1 on success,
 *         0 otherwise
 *
 * @param optionType Is a valid integer option
 * @param value Is the valid to adopt
 */
#endif
#endif
    int optionsSetIntOption(OptionTypeEnum optionType, int value);



#ifdef USE_GNOME_DOCS

/**
 * optionsGetIntOption:
 * @optionType: A valid integer option
 *
 * Return the state of an integer option
 *
 * Returns The state of a integer xsldbg option
 */
#else
#ifdef USE_KDE_DOCS

/**
 * @returns The state of a integer xsldbg option
 *
 * @param optionType Is a valid integer option
 */
#endif
#endif
    int optionsGetIntOption(OptionTypeEnum optionType);



#ifdef USE_GNOME_DOCS

/**
 * optionsSetStringOption:
 * @optionType: A valid string option
 * @value: The value to copy
 *
 * Set value for a string xsldbg option to @value. 
 * Any memory used currently by option @optionType will be freed
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Set value for a string xsldbg option to @p value. 
 * Any memory used currently by option @p optionType will be freed
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param optionType A valid string option
 * @param value The value to copy
 */
#endif
#endif
    int optionsSetStringOption(OptionTypeEnum optionType,
                               const xmlChar * value);



#ifdef USE_GNOME_DOCS

/**
 * optionsGetStringOption:
 * @optionType: A valid string option 
 *
 * Get value for a string xsldbg option of @optionType

 * Returns current option value which may be NULL
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Get value for a string xsldbg option of @p optionType
 *
 * @returns current option value which may be NULL
 *
 * @param optionType A valid string option 
 */
#endif
#endif
    const xmlChar *optionsGetStringOption(OptionTypeEnum optionType);


#ifdef USE_GNOME_DOCS

  /**
   * optionsCopyVolitleOptions:
   *
   * Copy volitile options to the working area for xsldbg to be used
   *   just after xsldbg starts its processing loop
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Copy volitile options to the working area for xsldbg to be used
   *   just after xsldbg starts its processing loop
   */
#endif
#endif
    void optionsCopyVolitleOptions(void);



/*---------------------------------------------
          Parameter related options 
-------------------------------------------------*/


#ifdef USE_GNOME_DOCS

/**
 * optionsGetParamItemList:
 *
 * Return the list of libxlt parameters
 *
 * Returns The list of parameters to provide to libxslt when doing 
 *           stylesheet transformation if successful
 *        NULL otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Return the list of libxlt parameters
 *
 * @returns The list of parameters to provide to libxslt when doing 
 *              stylesheet transformation if successful
 *          NULL otherwise
 */
#endif
#endif
    arrayListPtr optionsGetParamItemList(void);



#ifdef USE_GNOME_DOCS

/**
 * optionsParamItemNew:
 * @name: Is valid 
 * @value: Is valid 
 *
 * Create a new libxslt parameter item
 * Returns non-null if sucessful
 *         NULL otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Create a new libxslt parameter item
 *
 * @returns non-null if sucessful
 *          NULL otherwise
 *
 * @param name Is valid 
 * @param value Is valid 
 */
#endif
#endif
    parameterItemPtr optionsParamItemNew(const xmlChar * name,
                                         const xmlChar * value);



#ifdef USE_GNOME_DOCS

/**
 * optionsParamItemFree:
 * @item: Is valid
 *
 * Free memory used by libxslt parameter item @item
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Free memory used by libxslt parameter item @p item
 *
 * @param item Is valid
 */
#endif
#endif
    void optionsParamItemFree(parameterItemPtr item);



#ifdef USE_GNOME_DOCS

/**
 * optionsPrintParam:
 * @paramId: 0 =< paramID < arrayListCount(getParamList())
 * 
 * Print parameter information
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Prints all items in parameter list
 *
 * @returns 1 on success,
 *          0 otherwise
 */
#endif
#endif
    int optionsPrintParam(int paramId);



#ifdef USE_GNOME_DOCS

/**
 * optionsPrintParamList:
 *
 * Prints all items in parameter list
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Prints all items in parameter list
 *
 * @returns 1 on success,
 *          0 otherwise
 */
#endif
#endif
    int optionsPrintParamList(void);
#ifdef USE_GNOME_DOCS


/* ---------------------------------------------
          Option serialization functions
-------------------------------------------------*/

  /**
   * optionNode:
   * @optionType : Is valid, option to convert to a xmlNodePtr 
   *
   * Convert option into a xmlNodePtr
   *
   * Returns the option @optionType as a xmlNodePtr if successful,
   *          NULL otherwise
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Convert option into a xmlNodePtr
   *
   * @returns The option @p optionType as a xmlNodePtr if successful,
   *          NULL otherwise
   *
   * @param optionType Is valid, option to convert to a xmlNodePtr 
   *
   */
#endif
#endif
    xmlNodePtr optionsNode(OptionTypeEnum optionType);



#ifdef USE_GNOME_DOCS

  /**
   * optionsReadDoc:
   * @doc : Is valid xsldbg config document, in the format as indicated 
   *        by config.dtd
   *
   * Read options from document provided. 
   *
   * Returns 1 if able to read @doc and load options found,
   *         0 otherwise
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Read options from document provided. 
   *
   * @returns 1 if able to read @p doc and load options found,
   *          0 otherwise
   *
   * @param doc Is valid xsldbg config document, in the format as indicated 
   *        by config.dtd
   */
#endif
#endif
    int optionsReadDoc(xmlDocPtr doc);


#ifdef USE_GNOME_DOCS

  /**
   * optionsSavetoFile:
   * @fileName : Must be NON NULL be a local file that can be written to
   *
   * Save configuation to file specified
   *
   * Returns 1 if able to save options to @fileName,
   *         0 otherwise
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Save configuation to file specified
   *
   * @returns 1 if able to save options to @fileName,
   *          0 otherwise
   *
   * @fileName : Must be NON NULL be a local file that can be written to
   */
#endif
#endif
    int optionsSavetoFile(xmlChar * fileName);


/* ---------------------------------------------
          Platform specific option functions
-------------------------------------------------*/

#ifdef USE_GNOME_DOCS

  /**
   * optionsPlatformInit:
   *
   * Intialize the platform specific options module
   *
   *  This is a platform specific interface
   *
   * Returns 1 if sucessful
   *         0 otherwise  
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Intialize the platform specific options module
   *
   *  This is a platform specific interface
   *
   * @returns 1 if sucessful
   *          0 otherwise  
   */
#endif
#endif
    int optionsPlatformInit(void);


#ifdef USE_GNOME_DOCS

  /**
   * optionsPlatformFree:
   *
   * Free memory used by the platform specific options module
   *
   *  This is a platform specific interface
   *
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Free memory used by the platform specific options module
   *
   *  This is a platform specific interface
   *
   */
#endif
#endif
    void optionsPlatformFree(void);


#ifdef USE_GNOME_DOCS

  /**
   * optionsConfigFileName:
   * 
   * Return xsldbg's the configuration file name 
   *
   * Returns A copy of the file name that will be used to load xsldbgs
   *           configuration from,
   *         NULL otherwise
   *
   *  This is a platform specific interface
   *
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Return xsldbg's the configuration file name 
   *
   * Returns A copy of the file name that will be used to load xsldbgs
   *           configuration from,
   *         NULL otherwise
   *
   *  This is a platform specific interface
   *
   */
#endif
#endif
    xmlChar *optionsConfigFileName(void);


#ifdef USE_GNOME_DOCS

  /**
   * optionsLoad:
   *
   * Load options from configuration file/registry
   *
   * This is a platform specific interface
   * 
   * Returns 1 if able to load options
   *         0 otherwise
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Load options from configuration file/registry
   *
   * This is a platform specific interface
   * 
   * Returns 1 if able to load options
   *         0 otherwise
   */
#endif
#endif
    int optionsLoad(void);


#ifdef USE_GNOME_DOCS

  /**
   * optionsSave:
   *
   * Save options to configuration file/registry
   *
   * This is a platform specific interface
   * 
   * Returns 1 if able to save options
   *         0 otherwise
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Save options to configuration file/registry
   *
   * This is a platform specific interface
   * 
   * Returns 1 if able to load options
   *         0 otherwise
   */
#endif
#endif
    int optionsSave(void);


#ifdef USE_GNOME_DOCS

  /**
   * optionsConfigState:
   * @value : Is valid
   *
   * Set/Get the state of configuration loading/saving. Normally only used
   *    by RISC OS
   *
   * Returns The current/new value of configuration flag. Where
   *         @value means:
   *           OPTIONS_CONFIG_READVALUE : No change return current 
   *               value of read configuration flag
   *           OPTIONS_CONFIG_WRITING  : Clear flag and return 
   *               OPTIONS_CONFIG_WRITING which mean configuration 
   *               file is being written
   *           OPTIONS_CONFIG_READING : Set flag and return 
   *               OPTIONS_CONFIG_READING, which means configuration
   *               file is being read
   *           OPTIONS_CONFIG_IDLE : We are neither reading or writing 
   *               configuration and return OPTIONS_CONFIG_IDLE
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Set/Get the state of configuration loading/saving. Normally only used
   *    by RISC OS
   *
   *
   * Returns The current/new value of configuration flag. Where
   *         @p value means:
   *           OPTIONS_CONFIG_READVALUE : No change return current 
   *               value of read configuration flag
   *           OPTIONS_CONFIG_WRITING  : Clear flag and return 
   *               OPTIONS_CONFIG_WRITING which mean configuration 
   *               file is being written
   *           OPTIONS_CONFIG_READING : Set flag and return 
   *               OPTIONS_CONFIG_READING, which means configuration
   *               file is being read
   *           OPTIONS_CONFIG_IDLE : We are neither reading or writing 
   *               configuration and return OPTIONS_CONFIG_IDLE
   *
   * @param value Is valid
   *
   */
#endif
#endif
    int optionsConfigState(OptionsConfigState value);

  /**
   * optionsAddWatch:
   * @xPath : A valid xPath to evaluate in a context and 
   *          has not already been addded
   *
   * Add xPath to be evaluated and printed out each time the debugger stops
   *
   * Returns 1 if able to add xPath to watched
   *         0 otherwise
   */
  int optionsAddWatch(const xmlChar* xPath);

  /** 
   * optionsGetWatchID:
   * @xPath : A valid watch expression that has already been added
   *
   * Finds the ID of watch expression previously added
   *
   * Returns 0 if not found, 
   *         otherwise returns the ID of watch expression
   */
  int optionsGetWatchID(const xmlChar* xPath);


  /**
   * optionsRemoveWatch:
   * @watchID : A valid watchID as indicated by last optionsPrintWatches
   *
   * Remove the watch with given ID from our list of expressions to watch
   *
   * Returns 1 if able to remove to watch expression
   *         0 otherwise
   */
  int optionsRemoveWatch(int watchID);


  /**
   * optionsGetWatchList:
   * 
   * Return the current list of expressions to watch
   *
   * Return the current list of expressions to watch
   */
  arrayListPtr optionsGetWatchList(void);


#ifdef __cplusplus
}
#endif
#endif
