
/***************************************************************************
                          options_unix.c  -  *nix specific option functions
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

#include <libxml/parser.h>
#include <stdlib.h>
#include <string.h>
#include "xsldbg.h"
#include "options.h"

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
int
optionsPlatformInit(void)
{
    return 1;
}


  /**
   * optionsPlatformFree:
   *
   * Free memory used by the platform specific options module
   *
   *  This is a platform specific interface
   *
   */
void
optionsPlatformFree(void)
{
    /* empty */
}

  /**
   * optionsConfigFileName:
   * 
   * Returns A copy of the file name that will be used to load xsldbgs
   *           configuration from,
   *         NULL otherwise
   */
xmlChar *
optionsConfigFileName(void)
{
    xmlChar *result = NULL;
    const char *homeDir = getenv("HOME");
    const char *configName = "xsldbg.rc";
    int bufferSize = 0;

    if (homeDir) {
        /* give ourselves a bit of room to move */
        bufferSize = strlen(homeDir) + strlen(configName) + 10;
        result = (xmlChar *) xmlMalloc(bufferSize);
        snprintf((char *) result, bufferSize, "%s/%s", homeDir,
                 configName);
    }
    return result;
}


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
int
optionsLoad(void)
{
    int result = 0;
    xmlDocPtr doc = xmlParseFile((char *) optionsConfigFileName());

    if (doc)
        result = optionsReadDoc(doc);
    return 0;
}


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
int
optionsSave(void)
{
    return optionsSavetoFile(optionsConfigFileName());
}
