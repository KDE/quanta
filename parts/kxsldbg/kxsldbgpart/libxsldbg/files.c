
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

#ifdef VERSION
#undef VERSION
#endif

/* We want skip most of these includes when building documentation */
#ifndef BUILD_DOCS

#include "xsldbg.h"
#include <stdio.h>
#include <libxml/entities.h>
#include <libxml/tree.h>
#include <libxml/catalog.h>
#include <libxml/parserInternals.h>
#include <libxml/encoding.h>    /* needed by filesTranslate, filesEncoding functions */
#include <libxml/uri.h>    /* needed for  xmlURIUnescapeString */
#include "debugXSL.h"
#include "files.h"
#include "utils.h"
#include "options.h"
#include "xsldbgthread.h"
#ifdef WIN32
#include <direct.h>
#endif

#endif /* BUILD_DOCS */


/* top xml document */
static xmlDocPtr topDocument;

/* temporary xml document */
static xmlDocPtr tempDocument;

/* used as a scratch pad for temporary results*/
static xmlChar buffer[DEBUG_BUFFER_SIZE];

/* top stylsheet */
static xsltStylesheetPtr topStylesheet;

/* what is the base path for top stylesheet */
static xmlChar *stylePathName = NULL;

/* what is the path for current working directory*/
static xmlChar *workingDirPath = NULL;

static arrayListPtr entityNameList = NULL;

/* Current encoding to use for standard output*/
static xmlCharEncodingHandlerPtr stdoutEncoding = NULL;

/* input and output buffers for encoding*/
static xmlBufferPtr encodeInBuff = NULL;
static xmlBufferPtr encodeOutBuff = NULL;

/* Current line number and URI for xsldbg*/
static int currentLineNo = -1;
static xmlChar *currentUrl = NULL;

/* -----------------------------------------
   Private function declarations for files.c
 -------------------------------------------*/

/**
 * guessStylesheetHelper:
 * @payload: valid xsltStylesheetPtr
 * @data: valid searchInfoPtr of type SEARCH_NODE
 * @name: not used
 *
 * Try to guess what the complete file/URI is. If successful the search
 *   info will be set to found and the search data will contain the
 *   file name found. We are given our payload via walkStylesheets
 */
static void guessStylesheetHelper(void *payload, void *data,
                                  xmlChar * name ATTRIBUTE_UNUSED);


/**
 * guessStylesheetHelper2:
 * @payload: valid xmlNodePtr of the included stylesheet 
 * @data: valid searchInfoPtr of type SEARCH_NODE
 * @name: not used
 *
 * Try to guess what the complete file/URI is. If successful the search
 *   info will be set to found and the search data will contain the
 *   file name found. We are given our payload via walkIncludes
 */
static void guessStylesheetHelper2(void *payload, void *data,
                                   xmlChar * name ATTRIBUTE_UNUSED);


entityInfoPtr filesNewEntityInfo(const xmlChar * SystemID,
                                 const xmlChar * PublicID);

void filesFreeEntityInfo(entityInfoPtr info);

void filesAddEntityName(const xmlChar * SystemID,
                        const xmlChar * PublicID);


/* ------------------------------------- 
    End private functions
---------------------------------------*/


FILE *terminalIO;

/* No longer needed
   static FILE *oldStdin, *oldStdout, *oldStderr;*/

static char *ttyName = NULL;           /* what is the name of the default terminal */
static char *termName = NULL;   /* what is the name of terminal we are redirected to */


/**
 * redirectToTerminal:
 * @device: terminal to redirect i/o to , will not work under win32
 *
 * Open communications to the terminal device @device
 *
 * Returns 1 if sucessful
 *         0 otherwise
 */
int
openTerminal(xmlChar * device)
{
    int result = 0;

    if (!device) {              /* Failed; there's no device */
        xsltGenericError(xsltGenericErrorContext,
                         "Error: NULL argument provided\n");
        return result;
    }

    /*
     * On RISC OS, you get one terminal - the screen.
     * we assume that the parameter is meant to be an output device as
     * per normal - we can use vdu:, rawvdu: or :tt, or a filename for
     * normal VDU output, VDU output without newline expansion,
     * C terminal output with control code escaping, or a raw file
     * respectively.
     * The name passed is expected to be in native file format - no
     * URI escaping here.
     * One assumes that you might use a socket or a pipe here.
     */

    if (terminalIO) {
        fclose(terminalIO);
        terminalIO = NULL;
    }


    switch (device[0]) {
        case '\0':
        case '0':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            /* look like we are supposed to close the terminal 
             * but we've already done that     
             */
            break;

        case '1':
            if (termName) {
                terminalIO = fopen((char *) termName, "w");
                if (terminalIO != NULL) {
                    xmlFree(termName);
                    termName = xmlMemStrdup((char *) device);
                    result = 1;
                } else {
                    xsltGenericError(xsltGenericErrorContext,
                                     "Error: Unable to open terminal %s\n",
                                     termName);
                }
            } else {
                xsltGenericError(xsltGenericErrorContext,
                                 "Error: Did not previously open terminal\n");
            }
            break;

        case '2':
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Terminal level 2 not implemented\n");
            break;


        default:
            terminalIO = fopen((char *) device, "w");
            if (terminalIO != NULL) {
                if (termName)
                    xmlFree(termName);
                termName = xmlMemStrdup((char *) device);
                result = 1;
            } else {
                xsltGenericError(xsltGenericErrorContext,
                                 "Error: Unable to open terminal %s\n", device);
            }

    }

    return result;
}


/**
 * guessStylesheetHelper:
 * @payload: valid xsltStylesheetPtr
 * @data: valid searchInfoPtr of type SEARCH_NODE
 * @name: not used
 *
 * Try to guess what the complete file/URI is. If successful the search
 *   info will be set to found and the search data will contain the
 *   file name found. We are given our payload via walkStylesheets
 */
void
guessStylesheetHelper(void *payload, void *data,
                      xmlChar * name ATTRIBUTE_UNUSED)
{
    xsltStylesheetPtr style = (xsltStylesheetPtr) payload;
    searchInfoPtr searchCriteria = (searchInfoPtr) data;
    nodeSearchDataPtr searchData = NULL;
    /* where did the directory/URI separator occur */
    char *lastSlash;

    if (!style || !style->doc || !searchCriteria || !searchCriteria->data
        || (searchCriteria->type != SEARCH_NODE))
        return;

    searchData = (nodeSearchDataPtr) searchCriteria->data;
    if (searchData->nameInput && (searchData->absoluteNameMatch == NULL)) {
        /* at this point we know that we have not made an absolute match 
         * but we may have made a relative match */
        if (xmlStrCmp(style->doc->URL, searchData->nameInput) == 0) {
            /* absolute path match great! */
            searchData->absoluteNameMatch =
                (xmlChar *) xmlMemStrdup((char *) style->doc->URL);
            searchData->node = (xmlNodePtr) style->doc;
            searchCriteria->found = 1;
            return;
        }


        /* try to guess we assume that the files are unique */
        xmlStrCpy(buffer, "__#!__");
        /* try relative to top stylesheet directory */
        if (stylePath()) {
            xmlStrCpy(buffer, stylePath());
            xmlStrCat(buffer, searchData->nameInput);
        }
        if (xmlStrCmp(style->doc->URL, buffer) == 0) {
            /* guessed right! */
            searchData->guessedNameMatch =
                (xmlChar *) xmlMemStrdup((char *) buffer);
            searchData->node = (xmlNodePtr) style->doc;
            searchCriteria->found = 1;
            return;
        }

        if (workingPath()) {
            /* try relative to working directory */
            xmlStrCpy(buffer, workingPath());
            xmlStrCat(buffer, searchData->nameInput);
        }
        if (xmlStrCmp(style->doc->URL, buffer) == 0) {
            /* guessed right! */
            searchData->guessedNameMatch =
                (xmlChar *) xmlMemStrdup((char *) buffer);
            searchData->node = (xmlNodePtr) style->doc;
            searchCriteria->found = 1;
            return;
        }


	/* Find the last separator of the stylsheet's URL */
	lastSlash = xmlStrChr(style->doc->URL, URISEPARATORCHAR);
	if (!lastSlash)
	  lastSlash = xmlStrChr(style->doc->URL, PATHCHAR);

        if (lastSlash) {
            /* Last try, assume nameInput contains only a file name
             * Strip of the file name at end of the stylesheet doc URL */
	  lastSlash++;    /* skip the slash */
	  if (xmlStrCmp(lastSlash, searchData->nameInput) == 0) {
	    /* guessed right! */
	    searchData->guessedNameMatch =
                        (xmlChar *) xmlMemStrdup((char *) style->doc->URL);
                    searchData->node = (xmlNodePtr) style->doc;
                    searchCriteria->found = 1;
                }
            }
    }
}


/**
 * guessStylesheetHelper2:
 * @payload: valid xmlNodePtr of the included stylesheet 
 * @data: valid searchInfoPtr of type SEARCH_NODE
 * @name: not used
 *
 * Try to guess what the complete file/URI is. If successful the search
 *   info will be set to found and the search data will contain the
 *   file name found. We are given our payload via walkIncludes
 */
void
guessStylesheetHelper2(void *payload, void *data,
                       xmlChar * name ATTRIBUTE_UNUSED)
{
    xmlNodePtr node = (xmlNodePtr) payload;
    searchInfoPtr searchCriteria = (searchInfoPtr) data;
    nodeSearchDataPtr searchData = NULL;
    /* where did the directory/URI separator occur */
    char *lastSlash;

    if (!node || !node->doc || !searchCriteria || !searchCriteria->data ||
        (searchCriteria->type != SEARCH_NODE))
        return;

    searchData = (nodeSearchDataPtr) searchCriteria->data;
    if (searchData->nameInput && (searchData->absoluteNameMatch == NULL)) {
        /* at this point we know that we have not made an absolute match 
         * but we may have made a relative match */
        if (xmlStrCmp(node->doc->URL, searchData->nameInput) == 0) {
            /* absolute path match great! */
            searchData->absoluteNameMatch =
                (xmlChar *) xmlMemStrdup((char *) node->doc->URL);
            searchData->node = node;
            searchCriteria->found = 1;
            return;
        }


        /* try to guess we assume that the files are unique */
        xmlStrCpy(buffer, "__#!__");
        /* try relative to top stylesheet directory */
        if (stylePath()) {
            xmlStrCpy(buffer, stylePath());
            xmlStrCat(buffer, searchData->nameInput);
        }
        if (xmlStrCmp(node->doc->URL, buffer) == 0) {
            /* guessed right! */
            searchData->guessedNameMatch =
                (xmlChar *) xmlMemStrdup((char *) buffer);
            searchData->node = node;
            searchCriteria->found = 1;
            return;
        }

        if (workingPath()) {
            /* try relative to working directory */
            xmlStrCpy(buffer, workingPath());
            xmlStrCat(buffer, searchData->nameInput);
        }
        if (xmlStrCmp(node->doc->URL, buffer) == 0) {
            /* guessed right! */
            searchData->guessedNameMatch =
                (xmlChar *) xmlMemStrdup((char *) buffer);
            searchData->node = node;
            searchCriteria->found = 1;
            return;
        }


	/* Find the last separator of the stylsheet's URL */
	lastSlash = xmlStrChr(node->doc->URL, URISEPARATORCHAR);
	if (!lastSlash)
	  lastSlash = xmlStrChr(node->doc->URL, PATHCHAR);

        if (lastSlash) {
	  /* Last try, assume nameInput contains only a file name
	   * Strip of the file name at end of the stylesheet doc URL */
	  lastSlash++;    /* skip the slash */
	  if (xmlStrCmp(lastSlash, searchData->nameInput) == 0) {
	    /* guessed right! */
	    searchData->guessedNameMatch =
                        (xmlChar *) xmlMemStrdup((char *) node->doc->URL);
	    searchData->node = node;
	    searchCriteria->found = 1;
	  }
	}
    }
}


/**
 * guessStylesheetName:
 * @searchInf: Is valid
 *
 * Try to find a matching stylesheet name
 * Sets the values in @searchinf depending on outcome of search
 */
void
guessStylesheetName(searchInfoPtr searchCriteria)
{
    nodeSearchDataPtr searchData;

    if (!searchCriteria)
        return;

    searchData = (nodeSearchDataPtr) searchCriteria->data;
    if (searchData->nameInput == NULL)
        return;                 /* must supply name of file to look for */

    walkStylesheets((xmlHashScanner) guessStylesheetHelper,
                    searchCriteria, filesGetStylesheet());
    if (!searchCriteria->found) {
        /* try looking in the included stylesheets */
        walkIncludes((xmlHashScanner) guessStylesheetHelper2,
                     searchCriteria, filesGetStylesheet());
    }
}


/**
 * stylePath:
 *
 * Return The base path for the top stylesheet ie
 *        ie URL minus the actual file name
 *
 * Returns The base path for the top stylesheet ie
 *        ie URL minus the actual file name
 */
xmlChar *
stylePath(void)
{
    return stylePathName;
}


/**
 * workingPath:
 * 
 * Return the working directory as set by changeDir function
 *
 * Returns The working directory as set by changeDir function
 */
xmlChar *
workingPath(void)
{
    return workingDirPath;
}


/**
 * changeDir:
 * @path: The path to adopt as new working directory
 *
 * Change working directory to path 
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
changeDir(const xmlChar * path)
{
    int result = 0;
    int charIndex;
    const char endString[2] = { PATHCHAR, '\0' };
    xmlChar *expandedName = NULL;


    if (path && (xmlStrLen(path) > 0)) {
        expandedName = filesExpandName(path);
    } else {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Null Input to changeDir %s %d\n",
                         __FILE__, __LINE__);
        return result;
    }

    if (!expandedName)
        return result;          /* out of memory ? */

    if (xmlStrLen(expandedName) + 1 > sizeof(buffer)) {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Path name too big\n");
        return result;
    }

    xmlStrCpy(buffer, expandedName);
    /* strip off any extra PATHCHAR's as win32's chdir function 
     * fails if we don't */
    charIndex = xmlStrLen(buffer) - 1;
    while (charIndex && (buffer[charIndex] == PATHCHAR)) {
        charIndex--;
    }
    buffer[charIndex + 1] = '\0';


    if (chdir((char *) buffer) == 0) {
        if (workingDirPath)
            xmlFree(workingDirPath);
        /* must have path char at end of path name */
        xmlStrCat(buffer, endString);
        workingDirPath = (xmlChar *) xmlMemStrdup((char *) buffer);
        result = 1;
    }
    xmlFree(expandedName);      /* this will always be valid time */
    if (!result) {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Unable to change to directory %s\n",
                         path);
    } else {
        xsltGenericError(xsltGenericErrorContext,
                         "Change to directory %s\n", path);
    }
    return result;
}


/**
 * filesLoadXmlFile:
 * @path: xml file to load
 * @fileType: A valid FileTypeEnum 
 * 
 * Load specified file type, freeing any memory previously used 
 *
 * Returns 1 on success,
 *         0 otherwise 
 */
int
filesLoadXmlFile(const xmlChar * path, FileTypeEnum fileType)
{
    int result = 0;

    if (!filesFreeXmlFile(fileType))
        return result;

    switch (fileType) {
        case FILES_XMLFILE_TYPE:
            if (path && xmlStrLen(path)) {
                if (optionsGetIntOption(OPTIONS_SHELL)) {
                    xsltGenericError(xsltGenericErrorContext,
                                     "Setting xml data file name to %s\n",
                                     path);
                }
                optionsSetStringOption(OPTIONS_DATA_FILE_NAME, path);
            }
            topDocument = xsldbgLoadXmlData();
            if (topDocument)
                result = 1;
            break;

        case FILES_SOURCEFILE_TYPE:
            if (path && xmlStrLen(path)) {
                if (optionsGetIntOption(OPTIONS_SHELL)) {
                    xsltGenericError(xsltGenericErrorContext,
                                     "Setting stylesheet file name to %s\n",
                                     path);
                }
                optionsSetStringOption(OPTIONS_SOURCE_FILE_NAME, path);
            }
            topStylesheet = xsldbgLoadStylesheet();
            if (topStylesheet && topStylesheet->doc) {
                /* look for last slash (or baskslash) of URL */
                char *lastSlash = xmlStrrChr(topStylesheet->doc->URL,
                                             PATHCHAR);
                const char *docUrl =
                    (const char *) topStylesheet->doc->URL;

                result = 1;
                if (docUrl && lastSlash) {
                    stylePathName = (xmlChar *) xmlMemStrdup(docUrl);
                    stylePathName[lastSlash - docUrl + 1] = '\0';
                    if (optionsGetIntOption(OPTIONS_SHELL)) {
                        xsltGenericError(xsltGenericErrorContext,
                                         "Setting stylesheet base path to %s\n",
                                         stylePathName);
                    }
                } else {
                    const char cwd[4] = { '.', PATHCHAR, '\0' };

                    /* ie for *nix this becomes "./" */
                    stylePathName = xmlStrdup(BAD_CAST cwd);
                }

                /* try to find encoding for this stylesheet */
                if (optionsGetIntOption(OPTIONS_AUTOENCODE))
                    filesSetEncoding((char *) topStylesheet->encoding);
            }
            break;

        case FILES_TEMPORARYFILE_TYPE:
            if (!path || !xmlStrLen(path)) {
                xsltGenericError(xsltGenericErrorContext,
                                 "Missing file name\n");
                break;
            }
            topDocument = xsldbgLoadXmlTemporary(path);
            if (tempDocument)
                result = 1;
            break;
    }
    return result;
}


/**
 * filesFreeXmlFile:
 * @fileType: A valid FileTypeEnum 
 * 
 * Free memory associated with the xml file 
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
filesFreeXmlFile(FileTypeEnum fileType)
{
    int result = 0, type = fileType;

    switch (type) {
        case FILES_XMLFILE_TYPE:
            if (topDocument)
                xmlFreeDoc(topDocument);
            topDocument = NULL;
            result = 1;
            break;

        case FILES_SOURCEFILE_TYPE:
            if (topStylesheet)
                xsltFreeStylesheet(topStylesheet);
            if (stylePathName)
                xmlFree(stylePathName);
            stylePathName = NULL;
            topStylesheet = NULL;
            result = 1;
            break;

        case FILES_TEMPORARYFILE_TYPE:
            if (tempDocument)
                xmlFreeDoc(tempDocument);
            tempDocument = NULL;
            result = 1;
            break;
    }
    return result;
}


/**
 * filesGetStylesheet:
 *
 * Return The topmost stylesheet non-null on success,
 *         NULL otherwise
 *
 * Returns The topmost stylesheet non-null on success,
 *         NULL otherwise
 */
xsltStylesheetPtr
filesGetStylesheet(void)
{
    return topStylesheet;
}


/**
 * filesGetTemporaryDoc:
 *
 * Return The current "temporary" document
 *
 * Returns The current "temporary" document
 */
xmlDocPtr
filesGetTemporaryDoc(void)
{
    return tempDocument;
}


/**
 * filesGetMainDoc:
 *
 * Return The main docment
 *
 * Returns The main docment
 */
xmlDocPtr
filesGetMainDoc(void)
{
    return topDocument;
}


/**
 * filesReloaded:
 * @reloaded: if = -1 then ignore @reloaded
 *             otherwise change the status of files to value of @reloaded   
 *
 * Returns 1 if stylesheet or its xml data file has been "flaged" as reloaded,
 *         0 otherwise
 */
int
filesReloaded(int reloaded)
{
    static int changed = 0;

    if (reloaded >= 0) {
        changed = reloaded;
    }

    return changed;
}



/**
 * filesInit:
 *
 * Initialize the file related structures
 * Returns 1 on success,
 *         0 otherwise
 */
int
filesInit(void)
{
    int result = 0;

    terminalIO = NULL;
#ifdef __riscos
    ttyName = ":tt";            /* Default tty */
#endif
#ifdef HAVE_UNISTD
    ttyName = ttyname(fileno(stdin));
    /* save out io for when/if we send debugging to a terminal */
    oldStdin = stdin;
    oldStdout = stdout;
    oldStderr = stderr;
#endif
    topDocument = NULL;
    tempDocument = NULL;
    topStylesheet = NULL;
    entityNameList = arrayListNew(4, (freeItemFunc) filesFreeEntityInfo);
#if defined(HAVE_INCLUDE_FIX) && (LIBXML_VERSION < 20508)
    xmlSetEntityReferenceFunc(filesEntityRef);
#endif

    /* setup the encoding */
    encodeInBuff = xmlBufferCreate();
    encodeOutBuff = xmlBufferCreate();

    /* check the result so far and lastly perform platform specific
     * initialization */
    if (entityNameList && encodeInBuff && encodeOutBuff &&
        filesPlatformInit())
        result = 1;
    return result;
}

/**
 * filesFree:
 *
 * Free memory used by file related structures
 */
void
filesFree(void)
{
    int result;

    if (terminalIO) {
        fclose(terminalIO);
        terminalIO = NULL;
    }
    if (termName) {
        xmlFree(termName);
        termName = NULL;
    }

    result = filesFreeXmlFile(FILES_SOURCEFILE_TYPE);
    if (result)
        result = filesFreeXmlFile(FILES_XMLFILE_TYPE);
    if (result)
        result = filesFreeXmlFile(FILES_TEMPORARYFILE_TYPE);
    if (!result)
        xsltGenericError(xsltGenericErrorContext,
                         "Unable to free memory used by xml/xsl files\n");
    if (stylePathName) {
        xmlFree(stylePathName);
        stylePathName = NULL;
    }

    if (workingDirPath) {
        xmlFree(workingDirPath);
        workingDirPath = NULL;
    }

    if (entityNameList) {
        arrayListFree(entityNameList);
        entityNameList = NULL;
    }

    /* Free memory used by encoding related structures */
    if (encodeInBuff)
        xmlBufferFree(encodeInBuff);

    if (encodeOutBuff)
        xmlBufferFree(encodeOutBuff);

    /* close current encoding */
    filesSetEncoding(NULL);

    if (currentUrl)
        xmlFree(currentUrl);

    /* free any memory used by platform specific files module */
    filesPlatformFree();
}


/**
 * filesIsSourceFile:
 * @fileName : is valid
 * 
 * Returns true if @name has the ".xsl" externsion
 */
int
filesIsSourceFile(xmlChar * fileName)
{
    return strstr((char *) fileName, ".xsl") ||
        strstr((char *) fileName, ".Xsl") ||
        strstr((char *) fileName, ".XSL");
}



entityInfoPtr
filesNewEntityInfo(const xmlChar * SystemID, const xmlChar * PublicID)
{

    entityInfoPtr result = (entityInfoPtr) xmlMalloc(sizeof(entityInfo));

    if (result) {
        if (SystemID)
            result->SystemID = xmlStrdup(SystemID);
        else
            result->SystemID = xmlStrdup(BAD_CAST "");

        if (PublicID)
            result->PublicID = xmlStrdup(PublicID);
        else
            result->PublicID = xmlStrdup(BAD_CAST "");
    }
    return result;
}

void
filesFreeEntityInfo(entityInfoPtr info)
{
    if (!info)
        return;

    if (info->SystemID)
        xmlFree(info->SystemID);

    if (info->PublicID)
        xmlFree(info->PublicID);
    xmlFree(info);
}

/**
 * filesAddEntityName:
 * @name : is valid
 *
 * Add name to entity name list of know external entities if 
 *  it doesn't already exist in list
 */
void

filesAddEntityName(const xmlChar * SystemID, const xmlChar * PublicID)
{
    int entityIndex = 0;
    entityInfoPtr tempItem;

    if (!SystemID || !filesEntityList())
        return;

    for (entityIndex = 0;
         entityIndex < arrayListCount(filesEntityList()); entityIndex++) {
        tempItem =
            (entityInfoPtr) arrayListGet(filesEntityList(), entityIndex);
        if (tempItem && xmlStrEqual(SystemID, tempItem->SystemID)) {
            /* name aready exits so don't add it */
            return;
        }

    }

    tempItem = filesNewEntityInfo(SystemID, PublicID);
    arrayListAdd(filesEntityList(), tempItem);
}


/**
 * filesEntityRef :
 * @ent : Is valid as provided by libxslt
 * @firstNode : Is valid
 * @lastNode : Is Valid
 *
 * Fixes the nodes from firstNode to lastNode so that debugging can occur
   */
void

filesEntityRef(xmlEntityPtr ent, xmlNodePtr firstNode, xmlNodePtr lastNode)
{
    xmlNodePtr node = firstNode;
    if (!firstNode || !ent || !ent->SystemID ||
         (ent->etype != XML_EXTERNAL_GENERAL_PARSED_ENTITY) )
        return;

    if (ent->ExternalID)
        filesAddEntityName(ent->SystemID, ent->ExternalID);
    else
        filesAddEntityName(ent->URI, BAD_CAST "");
    while (node){
        filesSetBaseUri(node, ent->URI);
	if (node != lastNode)
           node = node->next;
	else
	   node = NULL;
    }
}



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

int
filesSetBaseUri(xmlNodePtr node, const xmlChar * uri)
{
    int result = 0;

    if (!node || !uri)
        return result;
    else {
        if (node->type == XML_ELEMENT_NODE){
            xmlChar *xsldbgUrlCopy = xmlGetProp(node, BAD_CAST "xsldbg:uri");
            if (!xsldbgUrlCopy)
                xmlNewProp(node, BAD_CAST "xsldbg:uri", uri);
            else
                xmlFree(xsldbgUrlCopy);
        }
        result = 1;
    }
    return result;
}


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
xmlChar *
filesGetBaseUri(xmlNodePtr node)
{
    xmlChar *result = NULL;

    if (!node || !node->doc)
        return result;

    while (node && node->parent) {
        /*
         * result =  xmlGetNsProp(node, BAD_CAST "uri", XSLDBG_XML_NAMESPACE);
         */
        if (node->type == XML_ELEMENT_NODE) {
            result = xmlGetProp(node, BAD_CAST "xsldbg:uri");
            if (result)
                break;
        }
        node = node->parent;
    }

    if (!result && node->doc && node->doc->URL)
        result = xmlStrdup(node->doc->URL);

    return result;
}




/**
 * filesEntityList:
 *
 * Return the list entity names used for documents loaded
 *
 * Returns the list entity names used for documents loaded
 */
arrayListPtr
filesEntityList(void)
{
    return entityNameList;
}


extern int intVolitileOptions[OPTIONS_VERBOSE - OPTIONS_XINCLUDE + 1];

/**
 * filesLoadCatalogs:
 *
 * Load the catalogs specifed by OPTIONS_CATALOG_NAMES if 
 *      OPTIONS_CATALOGS is enabled
 * Returns 1 if sucessful
 *         0 otherwise   
 */
int
filesLoadCatalogs(void)
{
    int result = 0;
    const char *catalogs = NULL;

    xmlCatalogCleanup();
    if (optionsGetIntOption(OPTIONS_CATALOGS)) {
      if (optionsGetStringOption(OPTIONS_CATALOG_NAMES) == NULL) {
	/* use the SGML catalog */
#ifdef __riscos
	catalogs = getenv("SGML$CatalogFiles");
#else
	catalogs = getenv("SGML_CATALOG_FILES");
#endif
	if (catalogs == NULL) {
#ifdef __riscos
	  xsltGenericError(xsltGenericErrorContext,
			   "Variable SGML$CatalogFiles not set\n");
#else
	  xsltGenericError(xsltGenericErrorContext,
			   "Variable $SGML_CATALOG_FILES not set\n");
#endif
	} else
	  /* copy the current catalog name(s) for user to see */
	  optionsSetStringOption(OPTIONS_CATALOG_NAMES,
				 (xmlChar *) catalogs);
      } else
	/* Use the current catalog settings from users*/
	catalogs = (char *)
	  optionsGetStringOption(OPTIONS_CATALOG_NAMES);

      result = 1;
    }

    if (catalogs){
      /* Load the new cataog selection */
      xmlLoadCatalogs(catalogs);
    }else{
      /* Use default catalogs */
      xmlInitializeCatalog();
    }
    return result;
}




  /**
   * filesEncode:
   * @text: Is valid, text to translate from UTF-8, 
   *
   * Return  A  new string of converted @text
   *
   * Returns  A  new string of converted @text, may be NULL
   */
xmlChar *
filesEncode(const xmlChar * text)
{
    xmlChar *result = NULL;

    if (!text)
        return result;

    if (!stdoutEncoding || !encodeInBuff || !encodeOutBuff)
        return xmlStrdup(text); /* no encoding active return as UTF-8 */

    xmlBufferEmpty(encodeInBuff);
    xmlBufferEmpty(encodeOutBuff);
    xmlBufferCat(encodeInBuff, text);

    if (xmlCharEncOutFunc(stdoutEncoding, encodeOutBuff, encodeInBuff)
        >= 0) {
        result = xmlStrdup(xmlBufferContent(encodeOutBuff));
    } else {
        xsltGenericError(xsltGenericErrorContext,
                         "Encoding of text failed\n");
        return xmlStrdup(text); /*  panic,  return as UTF-8 */
    }
    return result;
}



  /**
   * filesDeccode:
   * @text: Is valid, text to translate from current encoding to UTF-8, 
   *
   * Return  A  string of converted @text
   *
   * Returns  A  string of converted @text, may be NULL
   */
xmlChar *
filesDecode(const xmlChar * text)
{
    xmlChar *result = NULL;

    if (!text)
        return result;

    if (!stdoutEncoding || !encodeInBuff || !encodeOutBuff)
        return xmlStrdup(text); /* no encoding active return as UTF-8 */

    xmlBufferEmpty(encodeInBuff);
    xmlBufferEmpty(encodeOutBuff);
    xmlBufferCat(encodeInBuff, text);

    if (xmlCharEncInFunc(stdoutEncoding, encodeOutBuff, encodeInBuff)
        >= 0) {
        result = xmlStrdup(xmlBufferContent(encodeOutBuff));
    } else {
        xsltGenericError(xsltGenericErrorContext,
                         "Encoding of text failed\n");
        return xmlStrdup(text); /*  panic,  return @text unchanged */
    }
    return result;
}


    /*
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
int
filesSetEncoding(const char *encoding)
{
    int result = 0;

    if (encoding) {
        /* don't switch encoding unless we've found a valid encoding */
        xmlCharEncodingHandlerPtr tempEncoding =
            xmlFindCharEncodingHandler(encoding);
        if (tempEncoding) {
            filesSetEncoding(NULL);     /* re-use code to close encoding */
            stdoutEncoding = tempEncoding;
            result =
                (xmlCharEncOutFunc(stdoutEncoding, encodeOutBuff, NULL)
                 >= 0);
            if (!result) {
                xmlCharEncCloseFunc(stdoutEncoding);
                stdoutEncoding = NULL;
                xsltGenericError(xsltGenericErrorContext,
                                 "Unable to initialize encoding %s",
                                 encoding);
            } else
                optionsSetStringOption(OPTIONS_ENCODING,
                                       (xmlChar *) encoding);
        } else {
            xsltGenericError(xsltGenericErrorContext,
                             "Invalid encoding %s\n", encoding);
        }
    } else {
        /* close encoding and use UTF-8 */
        if (stdoutEncoding)
            result = (xmlCharEncCloseFunc(stdoutEncoding) >= 0);
        else
            result = 1;
        stdoutEncoding = NULL;
    }
    return result;
}



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
int
filesMoreFile(const xmlChar * fileName, FILE * file)
{
    int result = 0;
    int openedFile = 0;
    int lineCount;
    int reachedEof = 0;

    if (fileName && !file) {
#ifdef __riscos
        /* convert into RISC OS format a *nix style file name */
        fileName = (const xmlChar *) riscosfilename((char *) fileName);
#endif
        file = fopen((char *) fileName, "r");
        openedFile = 1;         /* since we opened the file we must close it */
    }
    if (file) {
        while (!feof(file) && !reachedEof) {
            lineCount = 0;
            while (!feof(file) && (lineCount < FILES_NO_LINES) &&
                   !reachedEof) {
                if (fgets((char *) buffer, sizeof(buffer), file)) {
                    xsltGenericError(xsltGenericErrorContext, "%s",
                                     buffer);
                    lineCount++;
                } else {
                    reachedEof = 1;
                }
            }

            if (!feof(file) && !reachedEof) {
                xsltGenericError(xsltGenericErrorContext,
                                 " ----- more ---- \n");
                fflush(stderr);
                if (fgets((char *) buffer, sizeof(buffer), stdin)) {
                    if ((*buffer == 'q') || (*buffer == 'Q'))
                        reachedEof = 1;
                } else {
                    reachedEof = 1;
                }
            }
        }

        if (openedFile) {
            fclose(file);
        }
        xsltGenericError(xsltGenericErrorContext, "\n");
        result = 1;
    } else {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: No valid file provided to print\n");
    }


    return result;
}


  /**
   * filesSearchResultsPath:
   *
   * Get the base path to be used for storing search results
   *
   * Returns The base path to be used for storing search results
   */
const xmlChar *
filesSearchResultsPath()
{
    const xmlChar *result;

    if (optionsGetStringOption(OPTIONS_SEARCH_RESULTS_PATH))
        result = optionsGetStringOption(OPTIONS_SEARCH_RESULTS_PATH);
    else
        result = stylePath();

    return result;
}


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
xmlChar *filesURItoFileName(const xmlChar* uri)
{
  xmlChar *result = NULL;
  xmlChar *unescapedFileName = NULL;
  const xmlChar* tempName = NULL;

  if (uri){
    if (!xmlStrnCmp(uri, "file://localhost", 16 )){
      tempName = uri + 16;
    }else{
#if defined(WIN32) && ! defined(CYGWIN)
      if (!xmlStrnCmp(uri, "file:///", 8))
	tempName = uri + 8;
#else
      if (!xmlStrnCmp(uri, "file://", 7))
	tempName = uri + 6; /* we need the leading '/'*/
#endif
    }

    /* If we've found something check to see if the file name 
       found is to be valid */
    if (tempName)
      result = (xmlChar*) xmlStrdup(tempName);
      unescapedFileName =  (xmlChar*) xmlStrdup(tempName);
      if (result && unescapedFileName){
	if (PATHCHAR != URISEPARATORCHAR){
	  /* Must convert path separators first */
	  xmlChar *probe = result;
	  while(*probe != '\0'){
	    if (*probe == (xmlChar)URISEPARATORCHAR)
	      *probe = (xmlChar)PATHCHAR;
	    probe++;
	  }
	}
	/* Now unescape the file name in result so far
	* NB: An unescaped name takes less memory that an escaped name
	*/
	xmlURIUnescapeString((char*)result, -1,  (char*)unescapedFileName);
	xmlFree(result);
	/* success we've got an local unescaped file name */
	result = unescapedFileName;
      }else{
        xsltGenericError(xsltGenericErrorContext,
			 "Error: Out of memory\n");	
	if (result){
	  xmlFree(result);
	}
	if (unescapedFileName) /* not needed, here for completeness */
	  xmlFree(unescapedFileName);

	result = NULL;
      }
    }else{
        xsltGenericError(xsltGenericErrorContext,
			 "Error: Unable to convert %s to local file name\n",
			 uri);
    }


  return result;
}


/* TODO in xsldbg 3.x rename these to use files prefix */

/**
 * xsldbgUpdateFileDetails:
 * @node : A valid node
 * 
 * Update the URL and  line number that we stoped at 
 */
void
xsldbgUpdateFileDetails(xmlNodePtr node)
{
    if ((node != NULL) && (node->doc != NULL)
        && (node->doc->URL != NULL)) {
        if (currentUrl != NULL)
            xmlFree(currentUrl);
        currentUrl =  filesGetBaseUri(node);
        currentLineNo = xmlGetLineNo(node);
    }
}


/**
 * xsldbgLineNo:
 *
 * What line number are we at 
 *
 * Returns The current line number of xsldbg, may be -1
 **/
int
xsldbgLineNo(void)
{
    return currentLineNo;
}


/**
 * xsldbgUrl:
 * 
 * What URL did we stop at
 *
 * Returns A NEW copy of URL stopped at. Caller must free memory for URL.
 *  May be NULL  
 */
xmlChar *
xsldbgUrl(void)
{
    if (currentUrl != NULL)
        return (xmlChar *) xmlMemStrdup((char *) currentUrl);
    else
        return NULL;
}
