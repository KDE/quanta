
/***************************************************************************
                          files.h  -  define file related functions
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

#ifndef FILES_H
#define FILES_H

#ifndef BUILD_DOCS

#include "search.h"
#include "arraylist.h"

#endif

#ifdef USE_KDE_DOCS

/**
 * Provide a file support
 *
 * @short file support
 *
 * @author Keith Isdale <k_isdale@tpg.com.au> 
 */
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>             /* need chdir function */
#endif

    extern FILE *terminalIO;

#define XSLDBG_XML_NAMESPACE \
    (const xmlChar *) "http://xsldbg.sourceforge.net/namespace"

    typedef struct _entityInfo entityInfo;
    typedef entityInfo *entityInfoPtr;
    struct _entityInfo {
        xmlChar *SystemID;
        xmlChar *PublicID;
    };


    /* how many lines do we print before pausing when 
     * performing "more" on a UTF-8 file. See function filesMoreFile */
#define FILES_NO_LINES 20

    /* Define the types of file names that we are intested in when creating
     * search results */
#ifndef USE_KDOC
    typedef enum {
        FILES_SEARCHINPUT,
        FILES_SEARCHXSL,
        FILES_SEARCHRESULT
    } FilesSearchFileNameEnum;
#else
    /* keep kdoc happy */
    enum FilesSearchFileNameEnum {
        FILES_SEARCHINPUT,
        FILES_SEARCHXSL,
        FILES_SEACHRESULT
    };
#endif



  /*-----------------------------------------------------------
    General function for working with files
    -----------------------------------------------------------*/

#ifdef USE_GNOME_DOCS

  /**
   * filesEntityRef :
   * @ent : Is valid as provided by libxslt
   * @firstNode : Is valid
   * @lastNode : Is Valid
   *
   * Fixes the nodes from firstNode to lastNode so that debugging can occur
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Fixes the nodes from firstNode to lastNode so that debugging can occur
   *
   * @param uri Is valid as provided by libxslt
   * @param firstNode Is valid
   * @param lastNode Is Valid
   */
#endif
#endif
    void filesEntityRef(xmlEntityPtr ent, xmlNodePtr firstNode,
                        xmlNodePtr lastNode);


#ifdef USE_GNOME_DOCS

  /**
   * filesEntityList:
   *
   * Return the list entity names used for documents loaded
   *
   * Returns The list entity names used for documents loaded
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Return the list entity names used for documents loaded
   *
   * @returns The list entity names used for documents loaded
   */
#endif
#endif
    arrayListPtr filesEntityList(void);


#ifdef USE_GNOME_DOCS

  /**
   * filesSetBaseUri:
   * @node : Is valid and has a doc parent
   * @uri : Is Valid
   * 
   * Set the base uri for this node. Function is used when xml file
   *    has external entities in its DTD
   * 
   * Returns 1 if successful,
   *        0 otherwise
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Set the base uri for this node. Function is used when xml file
   *    has external entities in its DTD
   * 
   * @param node Is valid and has a doc parent
   * @param uri Is Valid
   * 
   * @returns 1 if successful,
   *          0 otherwise
   */
#endif
#endif
    int filesSetBaseUri(xmlNodePtr node, const xmlChar * uri);


#ifdef USE_GNOME_DOCS

  /**
   * filesGetBaseUri:
   * @node : Is valid and has a doc parent
   * 
   * Get a copy of the base uri for this node. Function is most usefull 
   *  used when xml file  has external entities in its DTD
   * 
   * Returns the a copy of the base uri for this node,
   *         NULL otherwise
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Get a copy of the base uri for this node. Function is most usefull 
   *  used when xml file  has external entities in its DTD
   * 
   * @param node : Is valid and has a doc parent
   * 
   * @returns The a copy of the base uri for this node,
   *          NULL otherwise
   */
#endif
#endif
    xmlChar *filesGetBaseUri(xmlNodePtr node);


#ifdef USE_GNOME_DOCS

  /**
   * filesTempFileName:
   * @fleNumber : Number of temp file required
   *     where @fileNumber is 
   *      0 : file name used by cat command
   *      1 : file name used by profiling output
   *
   * Return the name of tempfile requirested
   *
   *  This is a platform specific interface
   *
   * Returns The name of temp file to be used for temporary results if sucessful,
   *         NULL otherwise
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Return the name of tempfile requested.
   * @param fleNumber : Number of temp file required
   *     where @p fileNumber is 
   *      0 : file name used by cat command
   *      1 : file name used by profiling output
   *
   *  This is a platform specific interface
   *
   * Returns The name of temp file to be used for temporary results if sucessful,
   *         NULL otherwise
   */
#endif
#endif
    const char *filesTempFileName(int fileNumber);


#ifdef USE_GNOME_DOCS

  /**
   * filesLoadCatalogs:
   *
   * Load the catalogs specifed by OPTIONS_CATALOG_NAMES if 
   *      OPTIONS_CATALOGS is enabled
   *
   * Returns 1 if sucessful
   *         0 otherwise   
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Load the catalogs specifed by OPTIONS_CATALOG_NAMES if 
   *      OPTIONS_CATALOGS is enabled
   *
   * @returns 1 if sucessful
   *          0 otherwise   
   */
#endif
#endif
    int filesLoadCatalogs(void);


#ifdef USE_GNOME_DOCS

  /**
   * filesEncode:
   * @text: Is valid, text to translate from UTF-8, 
   *
   * Return  A  string of converted @text
   *
   * Returns  A  string of converted @text, may be NULL
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Return  A  string of converted @text
   *
   * @param text Is valid, text to translate from UTF-8, 
   *
   * Returns  A  string of converted @text, may be NULL
   */
#endif
#endif
    xmlChar *filesEncode(const xmlChar * text);


#ifdef USE_GNOME_DOCS

  /**
   * filesDeccode:
   * @text: Is valid, text to translate from current encoding to UTF-8, 
   *
   * Return  A  string of converted @text
   *
   * Returns  A  string of converted @text, may be NULL
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Return  A  string of converted @text
   *
   * @param test Is valid, text to translate from current encoding to UTF-8, 
   *
   * Returns  A  string of converted @text, may be NULL
   */
#endif
#endif
    xmlChar *filesDecode(const xmlChar * text);


#ifdef USE_GNOME_DOCS

  /**
   * filesSetEncoding:
   * @encoding : Is a valid encoding supported by the iconv library or NULL
   *
   * Opens encoding for all standard output to @encoding. If  @encoding 
   *        is NULL then close current encoding and use UTF-8 as output encoding
   *
   * Returns 1 if successful in setting the encoding of all standard output
   *           to @encoding
   *         0 otherwise
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Opens encoding for all standard output to @p encoding. If  @p encoding 
   *        is NULL then close current encoding and use UTF-8 as output encoding
   *
   * @param encoding Is a valid encoding supported by the iconv library or NULL
   *
   * Returns 1 if successful in setting the encoding of all standard output
   *           to @p encoding
   *         0 otherwise
   */
#endif
#endif
    int filesSetEncoding(const char *encoding);


#ifndef USE_KDOC
    /* used by filesLoadXmlFile, filesFreeXmlFile functions */
    typedef enum {
        FILES_XMLFILE_TYPE = 100,       /* pick a unique starting point */
        FILES_SOURCEFILE_TYPE,
        FILES_TEMPORARYFILE_TYPE
    } FileTypeEnum;
#else

    /* used by filesLoadXmlFile, filesFreeXmlFile functions */
    enum FileTypeEnum {
        FILES_XMLFILE_TYPE = 100,       /* pick a unique starting point */
        FILES_SOURCEFILE_TYPE,
        FILES_TEMPORARYFILE_TYPE
    };
#endif


#ifdef USE_GNOME_DOCS

  /**
   * openTerminal
   * @device: terminal to redirect i/o to , will not work under win32
   *
   * Open communications to the terminal device @device
   * Returns 1 if sucessful
   *         0 otherwise
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Open communications to the terminal device @p device
   *
   * @param device Terminal to redirect i/o to , will not work under win32
   *
   * @returns 1 if sucessful
   *          0 otherwise
   */
#endif
#endif
    int openTerminal(xmlChar * device);


#ifdef USE_GNOME_DOCS

  /**
   * guessStyleSheetName:
   * @searchInf : Is valid
   *
   * Try to find a matching stylesheet name
   * Sets the values in @searchinf depending on outcome of search
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Try to find a matching stylesheet name
   * Sets the values in @p searchinf depending on outcome of search
   *
   * @param searchInf Is valid
   */
#endif
#endif
    void guessStylesheetName(searchInfoPtr searchInf);


#ifdef USE_GNOME_DOCS

  /**
   * stylePath:
   *
   * Returns the base path for the top stylesheet ie
   *        ie URL minus the actual file name
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Return the base path for the top stylesheet ie
   *        ie URL minus the actual file name
   *
   * @returns The base path for the top stylesheet ie
   *        ie URL minus the actual file name
   */
#endif
#endif
    xmlChar *stylePath(void);


#ifdef USE_GNOME_DOCS

  /**
   * workingPath:
   *
   * Return the working directory as set by changeDir function
   */
#else
#ifdef USE_KDE_DOCS

  /** 
   * Return the working directory as set by changeDir function
   *
   * @return the working directory as set by changeDir function
   */
#endif
#endif
    xmlChar *workingPath(void);


#ifdef USE_GNOME_DOCS

  /**
   * changeDir:
   * @path : path to adopt as new working directory
   *
   * Change working directory to path 
   *
   * Returns 1 on success,
   *         0 otherwise
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Change working directory to path 
   *
   * @param path The operating system path(directory) to adopt as 
   *         new working directory
   *
   * @returns 1 on success,
   *          0 otherwise
   */
#endif
#endif
    int changeDir(const xmlChar * path);


#ifdef USE_GNOME_DOCS

  /**
   * filesLoadXmlFile:
   * @path : xml file to load
   * @fileType : A valid FileTypeEnum
   *
   * Load specified file type, freeing any memory previously used 
   *
   * Returns 1 on success,
   *         0 otherwise 
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Load specified file type, freeing any memory previously used 
   *
   * @returns 1 on success,
   *         0 otherwise 
   *
   * @param path The xml file to load
   * @param fileType A valid FileTypeEnum
   */
#endif
#endif
    int filesLoadXmlFile(const xmlChar * path, FileTypeEnum fileType);


#ifdef USE_GNOME_DOCS

  /**
   * filesFreeXmlFile:
   * @fileType : A valid FileTypeEnum
   * 
   * Free memory associated with the xml file 
   *
   * Returns 1 on success,
   *         0 otherwise
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Free memory associated with the xml file 
   *
   * @returns 1 on success,
   *         0 otherwise
   *
   * @param fileType : A valid FileTypeEnum
   */
#endif
#endif
    int filesFreeXmlFile(FileTypeEnum fileType);


#ifdef USE_GNOME_DOCS

  /**
   * filesGetStylesheet:
   *
   * Return the topmost stylesheet 
   *
   * Returns non-null on success,
   *         NULL otherwise
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Return the topmost stylesheet 
   *
   * @returns Non-null on success,
   *         NULL otherwise
   */
#endif
#endif
    xsltStylesheetPtr filesGetStylesheet(void);


#ifdef USE_GNOME_DOCS

  /**
   * filesGetTemporaryDoc:
   *
   * Return the current "temporary" document
   *
   * Returns the current "temporary" document
   */
#else
#ifdef USE_KDE_DOCS

  /** 
   * Return the current "temporary" document
   *
   * @returns non-null on success,
   *          NULL otherwise
   */
#endif
#endif
    xmlDocPtr filesGetTemporaryDoc(void);


#ifdef USE_GNOME_DOCS

  /**
   * filesGetMainDoc:
   *
   * Returns the main docment
   */
#else
#ifdef USE_KDE_DOCS

  /** 
   * Return the main docment
   *
   * @returns the main document
   */
#endif
#endif
    xmlDocPtr filesGetMainDoc(void);


#ifdef USE_GNOME_DOCS

  /**
   * filesReloaded:
   * @reloaded : if = -1 then ignore @reloaded
   *             otherwise change the status of files to value of @reloaded   
   *
   * Returns 1 if stylesheet or its xml data file has been "flaged" as reloaded,
   *         0 otherwise
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * @returns 1 if stylesheet or its xml data file has been "flaged" as reloaded,
   *         0 otherwise
   *
   * @param reloaded If = -1 then ignore @p reloaded
   *             otherwise change the status of files to value of @p reloaded
   */
#endif
#endif
    int filesReloaded(int reloaded);


#ifdef USE_GNOME_DOCS

  /**
   * filesInit:
   *
   * Initialize the file module
   * Returns 1 on success,
   *         0 otherwise
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Initialize the file module
   *
   * @returns 1 on success,
   *          0 otherwise
   */
#endif
#endif
    int filesInit(void);


#ifdef USE_GNOME_DOCS

  /**
   * filesFree:
   *
   * Free memory used by file related structures
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Free memory used by file related structures
   */
#endif
#endif
    void filesFree(void);


#ifdef USE_GNOME_DOCS

  /**
   * filesIsSourceFile:
   * @fileName : Is valid
   * 
   * Test if filename could be  a stylesheet 
   *
   * Returns true if @name has the ".xsl" extension
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Test if filename could be  a stylesheet 
   *
   * @returns True if @name has the ".xsl" extension
   *
   *  @param fileName Is valid
   */
#endif
#endif
    int filesIsSourceFile(xmlChar * fileName);



#ifdef USE_GNOME_DOCS

  /**
   * filesMoreFile:
   * @fileName : May be NULL
   * @file : May be NULL
   *
   * Do a "more" like print of file specified by @fileName OR
   *   @file. If both are provided @file will be used. The content 
   *   of file chosen must be in UTF-8, and will be  printed in 
   *   the current encoding selected.The function will pause output 
   *   after FILES_NO_LINES lines have been printed waiting for
   *   user to enter "q" to quit or any other text to continue.
   *
   * Returns 1 if successful,
   *         0 otherwise
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Do a "more" like print of file specified by @fileName OR
   *   @file. If both are provided @file will be used. The content 
   *   of file chosen must be in UTF-8, and will be  printed in 
   *   the current encoding selected. The function will pause output 
   *   after FILES_NO_LINES lines have been printed waiting for
   *   user to enter "q" to quit or any other text to continue.
   *
   * @returns 1 if successful,
   *          0 otherwise
   *
   * @param fileName May be NULL
   * @param file May be NULL
   *
   */
#endif
#endif
    int filesMoreFile(const xmlChar * fileName, FILE * file);

#ifdef USE_GNOME_DOCS

  /**
   * filesSearchResultsPath:
   *
   * Get the base path to be used for storing search results
   *
   * Returns The base path to be used for storing search results
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Get the base path to be used for storing search results
   *
   * @returns The base path to be used for storing search results
   */
#endif
#endif
    const xmlChar *filesSearchResultsPath(void);



#ifdef USE_GNOME_DOCS

  /**
   * filesURItoFileName:
   * @uri : A valid URI that uses the "file://" prefix
   *
   * Return A copy of the conversion of @uri to a file name
   *        that is suitable to be used  with the fopen function.
   *        May be NULL, if out of memory, @uri does not use the
   *        "file://" prefix, or unable to convert to a valid file name
   *
   * Returns A copy of the conversion of @uri to a file name
   *        that is suitable to be used with the fopen function.
   *        May be NULL, if out of memory, @uri does not use the
   *        "file://" prefix, or unable to convert to a valid file name
   *    
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Return A copy of the conversion of @uri to a file name
   *        that is suitable to be used with the fopen function.
   *        May be NULL, if out of memory, @uri does not use the
   *        "file://" protocol, or unable to convert to a valid file name
   *
   * Returns A copy of the conversion of @uri to a file name
   *        that is suitable to be used with the fopen function.
   *        May be NULL, if out of memory, @uri does not use the
   *        "file://" prefix, or unable to convert to a valid file name
   *    
   * @param uri A valid URI that uses the "file://" prefix
   *
   */
#endif
#endif
  xmlChar *filesURItoFileName(const xmlChar* uri);


#ifdef USE_GNOME_DOCS

/**
 * xsldbgUpdateFileDetails:
 * @node : A valid node
 * 
 * Update the URL and  line number that we stoped at 
 */
#else
#ifdef USE_KDE_DOCS

  /** 
   * Update the URL and  line number that we stoped at 
   *
   * @param node A valid node
   */
#endif
#endif
    void xsldbgUpdateFileDetails(xmlNodePtr node);



#ifdef USE_GNOME_DOCS

  /**
   * xsldbgLineNo:
   *
   * What line number are we at 
   *
   * Returns The current line number of xsldbg, may be -1
   **/
#else
#ifdef USE_KDE_DOCS

  /**
   * What line number are we at 
   *
   * @returns The current line number of xsldbg, may be -1
   **/
#endif
#endif
    int xsldbgLineNo(void);


#ifdef USE_GNOME_DOCS

  /**
   * xsldbgUrl:
   * 
   * What URL did we stop at
   *
   * Returns A NEW copy of URL stopped at. Caller must free memory for URL,   
   *         may be NULL
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * What URL did we stop at
   *
   * @returns A NEW copy of URL stopped at. Caller must free memory for URL,   
   *         may be NULL
   */
#endif
#endif
    xmlChar *xsldbgUrl(void);

  /*-----------------------------------------------------------
    Platform specific file functions
    -----------------------------------------------------------*/


#ifdef USE_GNOME_DOCS

  /**
   * filesPlatformInit:
   *
   * Intialize the platform specific files module
   *
   *  This is a platform specific interface
   *
   * Returns 1 if sucessful
   *         0 otherwise  
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Intialize the platform specific files module
   *
   *  This is a platform specific interface
   *
   * @returns 1 if sucessful
   *          0 otherwise  
   */
#endif
#endif
    int filesPlatformInit(void);


#ifdef USE_GNOME_DOCS

  /**
   * filesPlatformFree:
   *
   * Free memory used by the platform specific files module
   *
   *  This is a platform specific interface
   *
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Free memory used by the platform specific files module
   *
   *  This is a platform specific interface
   *
   */
#endif
#endif
    void filesPlatformFree(void);



#ifdef USE_GNOME_DOCS

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
#else
#ifdef USE_KDE_DOCS

  /**
   * Converts a fileName to an absolute path
   *          If operating system supports it a leading "~" in the fileName
   *          will be converted to the user's home path. Otherwise
   *          the same name will be returned
   *
   * Returns A copy of the converted @p fileName or a copy of 
   *           the @p fileName as supplied. May return NULL
   *
   * @param fileName A valid fileName
   */
#endif
#endif
    xmlChar *filesExpandName(const xmlChar * fileName);



#ifdef USE_GNOME_DOCS

  /**
   * filesSearchFileName:
   * @fileType : Is valid
   *
   * Return a copy of the file name to use as an argument to searching
   *
   * Returns A copy of the file name to use as an argument to searching
   */
#else
#ifdef USE_KDE_DOCS

  /**
   * Return a copy of the file name to use as an argument to searching
   *
   * @returns A copy of the file name to use as an argument to searching
   *
   * @param fileType Is valid
   *
   */
#endif
#endif
    xmlChar *filesSearchFileName(FilesSearchFileNameEnum fileType);

#ifdef __cplusplus
}
#endif
#endif
