
/***************************************************************************
                          files_unix.c  -  file functions *nix platform
                                                specific
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

#include "xsldbg.h"
#include "files.h"
#include "utils.h"
#include "options.h"

static char *tempNames[2] = { NULL, NULL };

  /**
   * filesPlatformInit:
   *
   * Intialize the platform specific files module
   *
   *  This is a platform specific interface
   *
   *
   * Returns 1 if sucessful
   *         0 otherwise  
   */
int
filesPlatformInit(void)
{
    const char *namePrefix = "/tmp/";
    int nameIndex;
    int result = 1;

    /* The "base" names for files files to use */
    const char *names[] = {
        "_xsldbg_tmp1.txt",
        "_xsldbg_tmp2.txt"
    };

    if (getenv("USER")) {
        for (nameIndex = 0; nameIndex < 2; nameIndex++) {
            tempNames[nameIndex] =
                xmlMalloc(strlen(namePrefix) + strlen(getenv("USER")) +
                          strlen(names[nameIndex]) + 1);
            if (tempNames[nameIndex]) {
                xmlStrCpy(tempNames[nameIndex], namePrefix);
                xmlStrCat(tempNames[nameIndex], getenv("USER"));
                xmlStrCat(tempNames[nameIndex], names[nameIndex]);
            } else {
                xsltGenericError(xsltGenericErrorContext,
                                 "Error: Out of memory in filesPlatformInit\n");
                break;
                result = 0;
            }
        }
    } else {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: USER environment variable is not set\n");
    }
    return result;
}


  /**
   * filesPlatformFree:
   *
   * Free memory used by the platform specific files module
   *
   *  This is a platform specific interface
   *
   */
void
filesPlatformFree(void)
{
    int nameIndex;

    for (nameIndex = 0; nameIndex < 2; nameIndex++) {
        if (tempNames[nameIndex])
            xmlFree(tempNames[nameIndex]);
    }
}

  /**
   * filesTempFileName:
   * @fileNumber : Number of temp file required
   *
   * Return the name of tempfile. For each call to this function
   *     with the same @fileNumber the same file name will be returned
   *     File number : 0 is used by cat command
   *     File number : 1 is used by profiling output  
   *
   *  This is a platform specific interface
   *
   * Returns The name of temp file to be used for temporary results, 
   *         NULL otherwise
   */
const char *
filesTempFileName(int fileNumber)
{

    const char *result = NULL;

    if ((fileNumber < 0) || ((fileNumber + 1) > (int) sizeof(tempNames)))
        xsltGenericError(xsltGenericErrorContext,
                         "Unable to allocate temporary file %d for xsldbg\n",
                         fileNumber);
    else
        result = tempNames[fileNumber];

    return result;
}



  /**
   * filesExpandName:
   * @fileName : A valid fileName
   *
   * Converts a fileName to an absolute path
   *          If operating system supports it a leading "~" in the fileName
   *          will be converted to the user's home path. Otherwise
   *          the same name will be returned
   *
   * Returns A copy of the converted @fileName or a copy of 
   *           the @fileName as supplied. May return NULL
   */
xmlChar *
filesExpandName(const xmlChar * fileName)
{
    xmlChar *result = NULL;
    const char pathSepString[2] = { PATHCHAR, '\0' };

    if (fileName) {
        if ((fileName[0] == '~') && getenv("HOME")) {
            result =
                (xmlChar *) xmlMalloc(xmlStrLen(fileName) +
                                      strlen(getenv("HOME")) + 1);
            if (result) {
                xmlStrCpy(result, getenv("HOME"));
                xmlStrCat(result, pathSepString);
                xmlStrCat(result, &fileName[1]);
            } else {
                xsltGenericError(xsltGenericErrorContext,
                                 "Error: Out of memory\n");
            }
        } else {
            /* don't make any changes, return a copy only */
            result = xmlStrdup(fileName);
        }
    }
    return result;
}


  /**
   * filesSearchFileName:
   * @fileType : Is valid
   *
   * Return a copy of the file name to use as an argument to searching
   *
   * Returns A copy of the file name to use as an argument to searching
   */
xmlChar *
filesSearchFileName(FilesSearchFileNameEnum fileType)
{
    xmlChar *result = NULL;
    int type = fileType;
    int preferHtml = optionsGetIntOption(OPTIONS_PREFER_HTML);
    const xmlChar *baseDir = NULL;
    const xmlChar *name = NULL;
    static const char *searchNames[] = {
        /* Note: File names here are in native format, to be appended to the
         *  help directory name or search results path
         */
        /* First list names when prefer html is false */
        "searchresult.xml",     /* input  */
        "search.xsl",           /* stylesheet to use */
        "searchresult.txt",     /* where to put the result */
        /*Now for the names to use when prefer html is true */
        "searchresult.xml",     /* input  */
        "searchhtml.xsl",       /* stylesheet to use */
        "searchresult.html"     /* where to put the result */
    };

    if (!optionsGetStringOption(OPTIONS_DOCS_PATH)
        || !filesSearchResultsPath()) {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Null docs dir path or Null search results path\n");
        return result;
    }


    name = (xmlChar *) searchNames[(preferHtml * 3) + type];
    switch (type) {
        case FILES_SEARCHINPUT:
            baseDir = filesSearchResultsPath();
            break;

        case FILES_SEARCHXSL:
            baseDir = optionsGetStringOption(OPTIONS_DOCS_PATH);
            break;

        case FILES_SEARCHRESULT:
            baseDir = filesSearchResultsPath();
            break;
    }

    result = xmlMalloc(xmlStrLen(baseDir) + xmlStrLen(name) + 1);
    if (result) {
        xmlStrCpy(result, baseDir);
        xmlStrCat(result, name);
    }
    return result;
}
