
/* *************************************************************************
                          xslsearch.h  -  public functions for
                                               searching
                             -------------------
    begin                : Fri Dec 7 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ************************************************************************* */

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef XSLSEARCH_H
#define XSLSEARCH_H

#ifdef USE_KDE_DOCS

/**
 * Provide a searching support
 *
 * @short search support
 *
 * @author Keith Isdale <k_isdale@tpg.com.au> 
 */
#endif

/* We want skip most of these includes when building documentation*/
#ifndef BUILD_DOCS
#include "breakpoint.h"
#include "callstack.h"
#endif


#ifdef WITH_XSLT_DEBUG
#ifndef WITH_XSLT_DEBUG_BREAKPOINTS
#define WITH_XSLT_DEBUG_BREAKPOINTS
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

    /* what types of searches are there */
#ifndef USE_KDOC
    typedef enum {
        SEARCH_BREAKPOINT = 400,
        SEARCH_NODE,
        SEARCH_XSL,
        SEARCH_VARIABLE
    } SearchEnum;
#else
    /* keep kdoc happy */
    enum SearchEnum {
        SEARCH_BREAKPOINT = 400,
        SEARCH_NODE,
        SEARCH_XSL,
        SEARCH_VARIABLE
    };
#endif

    /* define a common structure to be used when searching */
    typedef struct _searchInfo searchInfo;
    typedef searchInfo *searchInfoPtr;
    struct _searchInfo {
        int found;              /* found is 1 if search is finished */
        int type;               /* what type of search see SearchEnum */
        int error;              /* did an error occur */
        void *data;             /* extra data to pass to walkFunc */
    };


    /* data to pass to via searchInfoPtr when searching for break points */
    typedef struct _breakPointSearchData breakPointSearchData;
    typedef breakPointSearchData *breakPointSearchDataPtr;
    struct _breakPointSearchData {
        int id;                 /* what id to look for, 
                                 * if -1 then ignore */
        xmlChar *templateName;  /* template to look for 
                                 * if NULL then ignore */
        breakPointPtr breakPtr; /* the break point found by search */
    };


    /* data to pass via searchInfoPtr when searching for nodes */
    typedef struct _nodeSearchData nodeSearchData;
    typedef nodeSearchData *nodeSearchDataPtr;
    struct _nodeSearchData {
        long lineNo;            /* what line number to look for 
                                 * if < 0 then ignore */
        xmlChar *url;           /* what URl to look for 
                                 * if NULL then ignore */
        int fileSearch;         /* if true then we are trying 
                                 * to match a file name */
        xmlChar *nameInput;     /* what file/node name are we
                                 * trying to match */
        xmlChar *guessedNameMatch;      /* possible name match */
        xmlChar *absoluteNameMatch;     /* full name match */
        xmlNodePtr node;        /* the node that the match 
                                 * occured in */
    };

    /* data to pass to via searchInfoPtr when searching for variables points */
    typedef struct _variableSearchData variableSearchData;
    typedef variableSearchData *variableSearchDataPtr;
    struct _variableSearchData {
        xmlChar *name;
        xmlChar *nameURI;
        xmlChar *select;        /* new value to adopt if any */
    };

#ifdef USE_GNOME_DOCS

/**
 * searchInit:
 *
 * Initialize the search module
 *
 * Returns 1 if search structures have been initialized properly and all
 *               memory required has been obtained,
 *         0 otherwise
*/
#else
#ifdef USE_KDE_DOCS

/**
 * Initialize the search module
 *
 * @returns 1 if search structures have been initialized properly and all
 *               memory required has been obtained,
 *          0 otherwise
*/
#endif
#endif
    int searchInit(void);


#ifdef USE_GNOME_DOCS

/**
 * searchFree:
 *
 * Free all memory used by the search module
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Free all memory used by the search module
 */
#endif
#endif
    void searchFree(void);


#ifdef USE_GNOME_DOCS

/**
 * searchNewInfo:
 * @type: What type of search is required
 * 
 * Create a new search
 *
 * Returns A valid search info pointer if successful
 *         NULL otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Create a new search
 *
 * @returns valid search info pointer if successful
 *          NULL otherwise
 *
 * @param type What type of search is required
 */
#endif
#endif
    searchInfoPtr searchNewInfo(SearchEnum type);


#ifdef USE_GNOME_DOCS

/**
 * searchFreeInfo:
 * @info: A valid search info
 *
 * Free memory used by @info
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Free memory used by @p info
 *
 * @param info A valid search info
 *
 */
#endif
#endif
    void searchFreeInfo(searchInfoPtr info);


#ifdef USE_GNOME_DOCS

/**
 * searchEmpty:
 *
 * Empty the seach data base of its contents
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Empty the seach dataBase of its contents
 *
 * @returns 1 on success,
 *          0 otherwise
 */
#endif
#endif
    int searchEmpty(void);


#ifdef USE_GNOME_DOCS

/**
 * searchDoc:
 *
 * Return the document used for seaching ie the search dataBase
 *
 * Returns The document used for searching
 *         Dangerous function to use! Does NOT return a copy of 
 *             search data  so don't free it
 */
#else
#ifdef USE_KDE_DOCS

/** 
 * Return the document used for seaching ie the search dataBase
 *
 * @returns the document used for searching
 *         Dangerous function to use! Does NOT return a copy of 
 *             search data  so don't free it
 */
#endif
#endif
    xmlDocPtr searchDoc(void);


#ifdef USE_GNOME_DOCS

/**
 * searchRootNode:
 *
 * Get the topmost node in the search dataBase
 *
 * Returns The topmost xml node in search dataBase.
 *         Dangerous function to use! Does NOT return a copy of 
 *             search root node  so don't free it
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Get the topmost node in the search dataBase
 *
 * @returns The topmost xml node in search dataBase.
 *         Dangerous function to use! Does NOT return a copy of 
 *             search root node  so don't free it
 */
#endif
#endif
    xmlNodePtr searchRootNode(void);


#ifdef USE_GNOME_DOCS

/**
 * searchAdd:
 * @node: Is valid
 *
 * Add a node to the search dataBase
 *
 * Returns 1 if able to add @node to top node in search dataBase ,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Add a node to the search dataBase
 *
 * @returns 1 if able to add @p node to top node in search dataBase,
 *          0 otherwise
 *
 * @param node Is valid
 */
#endif
#endif
    int searchAdd(xmlNodePtr node);


#ifdef USE_GNOME_DOCS

/**
 * searchSave:
 * @fileName: A valid file name, or NULL for the default
 *
 * Save the search dataBase to @fileName
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Save the search dataBase to @p fileName  
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param fileName Valid file name
 */
#endif
#endif
    int searchSave(const xmlChar * fileName);


#ifdef USE_GNOME_DOCS

/**
 * searchQuery:
 * @query: The query to run . If NULL then query is "//search/ *"
 * @tempFile: Where do we load the search dataBase from to execute
 *             query. If tempFile is NULL "searchresult.xml" is used
 * @outputFile : Where do we store the result. If NULL
 *             then default to  "searchresult.html"
 * 
 * Send query as parameter for execution of search.xsl using
 *    data stored in @tempFile 
 *
 * Returns 1 on success,
 *         0 otherwise   
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Send query as parameter for execution of search.xsl using
 *    data stored in @p tempFile 
 *
 * @returns 1 on success,
 *          0 otherwise   
 *
 * @param query The Query to run. If NULL then @p query defaults to "//search/ *"
 * @param tempFile Where do we load the search dataBase from to execute
 *             query. If @p tempFile is NULL default is "search.data"
 * @param outputFile Where do we store the result. If NULL
 *             then default to  "searchresult.html"
 */
#endif
#endif
    int searchQuery(const xmlChar * tempFile, const xmlChar * outputFile,
                    const xmlChar * query);


#ifdef USE_GNOME_DOCS

/**
 * updateSearchData:
 * @styleCtxt: Not used
 * @style: Is valid
 * @data: Not used but MUST be NULL for the moment
 * @variableTypes: What types of variables to look
 *
 * Update the searchDatabase
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Update the search dataBase
 * 
 * @returns 1 if able to update the search dataBase,
 *         0 otherwise
 * @param styleCtxt Not used
 * @param style Is valid
 * @param data Not used but MUST be NULL for the moment
 * @param variableTypes What types of variables to look
 */
#endif
#endif
    int updateSearchData(xsltTransformContextPtr styleCtxt,
                         xsltStylesheetPtr style,
                         void *data, VariableTypeEnum variableTypes);


#ifdef USE_GNOME_DOCS

/**
 * scanForBreakPoint: 
 * @payload: A valid breakPointPtr 
 * @data: The criteria to look for and a valid searchInfoPtr of
 *          type SEARCH_BREAKPOINT 
 * @name: Not used 
 *
 * Test if break point matches criteria given by @data. If so then 
 *      set @data->found to 1 and stores  reference to break point found in 
 *         @data->data->node
 *     otherwise @data is unchanged
*/
#else
#ifdef USE_KDE_DOCS

/**
 * Test if break point matches criteria given by @p data. If so then 
 *      set @p data->found to 1 and stores  reference to break point found in 
 *         @p data->data->node
 *     otherwise @p data is unchanged
 *
 * @param payload A valid breakPointPtr 
 * @param data The criteria to look for and a valid searchInfoPtr of
 *          type SEARCH_BREAKPOINT 
 * @param name Not used 
 *
*/
#endif
#endif
    void scanForBreakPoint(void *payload, void *data, xmlChar * name);


#ifdef USE_GNOME_DOCS

/**
 * scanForNode: 
 * @payload: A valid xmlNodePtr
 * @data: The criteria to look for and a valid searchInfo of
 *          type SEARCH_NODE 
 * @name: Not used

 * Test if node matches criteria given by @data if so then set @data->found 
 *   to 1 and  stores reference to node found in @data->data->node
 *     otherwise @data is unchanged
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Test if node matches criteria given by @p data if so then 
 *     set @p data->found to 1 and  stores reference to node found in
 *     @p data->data->node.
 *  otherwise @p data is unchanged
 *
 * @param payload A valid xmlNodePtr
 * @param data The criteria to look for and a valid searchInfo of
 *          type SEARCH_NODE 
 * @param name Not used
*/
#endif
#endif
    void scanForNode(void *payload, void *data, xmlChar * name);


#ifdef USE_GNOME_DOCS

/**
 * findNodeByLineNo:
 * @ctxt: Valid ctxt to look into
 * @url: Non-null, non-empty file name that has been loaded by debugger
 * @lineNumber: @lineNumber >= 0 and is available in @url
 *
 * Finds the closest line number in file specified that can be a point 
 *
 * Returns The node at line number number specified if successfull,
 *          NULL otherwise
 */

#else
#ifdef USE_KDE_DOCS

/**
 * Find the closest line number in file specified that can be a point 
 *
 * @returns The node at line number specified if successful,
 *	    NULL otherwise
 *
 * @param ctxt Valid ctxt to look into
 * @param url Non-null, non-empty file name that has been loaded by
 *           debugger
 * @param lineNumber @p lineNumber >= 0 and is available in @p url
*/
#endif
#endif
    xmlNodePtr findNodeByLineNo(xsltTransformContextPtr ctxt,
                                const xmlChar * url, long lineNumber);


#ifdef USE_GNOME_DOCS

/**
 * findTemplateNode: 
 * @style: A valid stylesheet collection to look into 
 * @name: A valid template name to look for
 *
 * Find a template node
 *
 * Returns The template node found if successful,
 *         NULL otherwise 
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Find a template node
 *
 * @returns The template node found if successful,
 *          NULL otherwise 
 *
 * @param style A Valid stylesheet collection to look into 
 * @param name Valid template name to look for
 */
#endif
#endif
    xmlNodePtr findTemplateNode(xsltStylesheetPtr style,
                                const xmlChar * name);


#ifdef USE_GNOME_DOCS

/**
 * findBreakPointByName:
 * @templateName: The template name to look for
 *
 * Find the breakpoint at template with "match" or "name" equal 
 *    to templateName
 *
 * Returns The break point that matches @templateName
 *         NULL otherwise
*/
#else
#ifdef USE_KDE_DOCS

/**
 * Find the breakpoint at template with "match" or "name" equal 
 *    to templateName
 *
 * @returns The break point that matches @p templateName
 *          NULL otherwise
 *
 * @param templateName Valid template name to look for
*/
#endif
#endif
    breakPointPtr findBreakPointByName(const xmlChar * templateName);


#ifdef USE_GNOME_DOCS

/**
 * findBreakPointById:
 * @id: The break point id to look for
 *
 * Find a break point by its id
 *
 * Returns The break point with given the break point id if found,
 *         NULL otherwise 
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Find a break point by its id
 *
 * @returns The break point with given the break point id if found,
 *          NULL otherwise 
 *
 * @param id The break point id to look for
 */
#endif
#endif
    breakPointPtr findBreakPointById(int id);


#ifdef USE_GNOME_DOCS

/**
 * findNodesByQuery:
 * @query: The xpath query to run, see docs/en/search.dtd for more details
 *  
 * Find nodes in search dataBase using an xpath query
 *
 * Returns The nodes that match the given query on success,
 *         NULL otherwise 
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Find nodes in search dataBase using an xpath query
 *
 * @returns The nodes that match the given query on success,
 *          NULL otherwise 
 *
 * @param query The xpath query to run, see docs/en/search.dtd for more details
 */
#endif
#endif
    xmlXPathObjectPtr findNodesByQuery(const xmlChar * query);


#ifdef USE_GNOME_DOCS

/**
 * walkBreakPoints:
 * @walkFunc: The function to callback for each break point found
 * @data: The extra data to pass onto walkFunc
 *
 * Walks through all break points calling walkFunc for each. The payload
 *  sent to walkFunc is of type breakPointPtr 
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Walks through all break points calling walkFunc for each. The payload
 *  sent to walkFunc is of type breakPointPtr 
 *
 * @param walkFunc The function to callback for each break point found
 * @param data The extra data to pass onto @p walkFunc
 */
#endif
#endif
    void walkBreakPoints(xmlHashScanner walkFunc, void *data);


#ifdef USE_GNOME_DOCS

/**
 * walkTemplates:
 * @walkFunc: The function to callback for each template found
 * @data: The extra data to pass onto walkFunc
 * @style: The stylesheet to start from
 *
 * Walks through all templates calling walkFunc for each. The payload
 *   of walkFunc is of type xsltTemplatePtr
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Walks through all templates found in @p style calling walkFunc for each.
 *   The payload of walkFunc is of type xsltTemplatePtr
 *
 * @param walkFunc The function to callback for each template found
 * @param data The extra data to pass onto @p walkFunc
 * @param style The stylesheet to start from
 */
#endif
#endif
    void walkTemplates(xmlHashScanner walkFunc, void *data,
                       xsltStylesheetPtr style);


#ifdef USE_GNOME_DOCS

/**
 * walkStylesheets:
 * @walkFunc: The function to callback for each stylesheet found
 * @data: The extra data to pass onto walkFunc
 * @style: The stylesheet to start from
 *
 * Walks through all templates calling walkFunc for each. The payload
 *   sent to walkFunc is of type xsltStylesheetPtr
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Walks through all stylesheets found in @p style calling walkFunc for
 *   each. The payload sent to walkFunc is of type xsltStylesheetPtr
 *
 * @param walkFunc The function to callback for each stylesheet found
 * @param data The extra data to pass onto @p walkFunc
 * @param style The stylesheet to start from
 */
#endif
#endif
    void walkStylesheets(xmlHashScanner walkFunc, void *data,
                         xsltStylesheetPtr style);


#ifdef USE_GNOME_DOCS

/**
 * walkGlobals:
 * @walkFunc: The function to callback for each gobal variable found
 * @data: The extra data to pass onto walkFunc
 * @style: The stylesheet to start from
 *
 * Call walkFunc for each global variable. The payload
 *   sent to walkFunc is of type  xmlNodePtr
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Call walkFunc for each global variable found in @p style. The payload
 *   sent to walkFunc is of type xmlNodePtr
 *
 * @param walkFunc The function to callback for each gobal variable found
 * @param data The extra data to pass onto @p walkFunc
 * @param style The stylesheet to start from
 */
#endif
#endif
    void walkGlobals(xmlHashScanner walkFunc,
                     void *data, xsltStylesheetPtr style);


#ifdef USE_GNOME_DOCS

/**
 * walkLocals:
 * @walkFunc: The function to callback for each local variable found
 * @data: The extra data to pass onto walkFunc
 * @style: The stylesheet to start from
 *
 * Walks through all local variables calling walkFunc for each. The payload
 *   of walkFunc is of type xmlNodePtr
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Walks through all local variables found in @p style calling 
 *   walkFunc for each. The payload of walkFunc is of type xmlNodePtr
 *
 * @param walkFunc The function to callback for each local variable found
 * @param data The extra data to pass onto @p walkFunc
 * @param style The stylesheet to start from
 */
#endif
#endif
    void walkLocals(xmlHashScanner walkFunc, void *data,
                    xsltStylesheetPtr style);


#ifdef USE_GNOME_DOCS

/**
 * walkIncludes:
 * @walkFunc: The function to callback for each included stylesheet
 * @data: The extra data to pass onto walkFunc
 * @style: The stylesheet to start from
 *
 * Walks through all included stylesheets calling walkFunc for each. 
 * The payload of walkFunc is of type xmlNodePtr
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Walks through all included stylesheets found in @p style,
 *   calling walkFunc for each. The payload of walkFunc is of
 *   type xmlNodePtr
 *
 * @param walkFunc The function to callback for each included stylesheet
 * @param data The extra data to pass onto @p walkFunc
 * @param style The stylesheet to start from
 */
#endif
#endif
    void walkIncludes(xmlHashScanner walkFunc, void *data,
                      xsltStylesheetPtr style);


#ifdef USE_GNOME_DOCS

/**
 * walkIncludeInst:
 * @walkFunc: The function to callback for each xsl:include instruction found
 * @data: The extra data to pass onto walkFunc
 * @style: The stylesheet to start from
 *
 * Walks through all xsl:include calling walkFunc for each. The payload
 *   of walkFunc is of type xmlNodePtr
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Walks through all xsl:include calling walkFunc for each. The payload
 *   of walkFunc is of type xmlNodePtr
 *
 * @param walkFunc The function to callback for each xsl:include instruction found
 * @param data The extra data to pass onto @p walkFunc
 * @param style The stylesheet to start from
 */
#endif
#endif
    void walkIncludeInst(xmlHashScanner walkFunc, void *data,
                         xsltStylesheetPtr style);


#ifdef USE_GNOME_DOCS

/**
 * walkChildNodes:
 * @walkFunc: The function to callback for each child/sibling found
 * @data: The extra data to pass onto walkFunc
 * @node: Is valid
 *
 * Call walkFunc for each child of @node the payload sent to walkFunc is
 *   a xmlNodePtr
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Call walkFunc for each child of @p node the payload sent to walkFunc is
 *   a xmlNodePtr
 *
 * @param walkFunc The function to callback for each child/sibling found
 * @param data The extra data to pass onto @p walkFunc
 * @param node Valid xmlNodePtr
 */
#endif
#endif
    void walkChildNodes(xmlHashScanner walkFunc, void *data,
                        xmlNodePtr node);



#ifdef USE_GNOME_DOCS

/**
 * searchBreakPointNode:
 * @breakPtr: Is valid
 *
 * Convert @breakPtr into search dataBase format
 *
 * Returns @breakPtr as a new xmlNode in search dataBase format 
 *               if successful,
 *         NULL otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Convert @p breakPtr into search dataBase format
 *
 * @returns @p breakPtr as a new xmlNode in search dataBase format 
 *               if successful,
 *          NULL otherwise
 *
 * @param breakPtr Is valid
 */
#endif
#endif
    xmlNodePtr searchBreakPointNode(breakPointPtr breakPtr);


#ifdef USE_GNOME_DOCS

/**
 * searchTemplateNode:
 * @templNode: Is valid
 * 
 * Convert @templateNode into search dataBase format
 *
 * Returns @templNode as a new xmlNode in search dataBase format 
 *               if successful,
 *         NULL otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Convert @p templateNode into search dataBase format
 *
 * @returns @p templNode as a new xmlNode in search dataBase format 
 *               if successful,
 *          NULL otherwise
 *
 * @param templNode A valid template node
 */
#endif
#endif
    xmlNodePtr searchTemplateNode(xmlNodePtr templNode);


#ifdef USE_GNOME_DOCS

/** 
 * searchGlobalNode:
 * @globalVariable: Is valid
 *
  * Convert @globalVariable into search dataBase format
  *
 * Returns @globalVariable as a new xmlNode in search dataBase format 
 *             if successful,
 *         NULL otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Convert @p globalVariable into search dataBase format
 *
 * @returns @p globalVariable as a new xmlNode in search dataBase 
 *               format if successful,
 *          NULL otherwise
 *
 * @param globalVariable A valid xmlNodePtr node
 * 
 */
#endif
#endif
    xmlNodePtr searchGlobalNode(xmlNodePtr globalVariable);


#ifdef USE_GNOME_DOCS

/** 
 * searchLocalNode:
 * @localvariable: Is valid
 *
 * Convert @localVariable into search dataBase format
 *
 * Returns @localVariable as a new xmlNode in search dataBase format 
 *             if successful,
 *         NULL otherwise
 */
#else
#ifdef USE_KDE_DOCS

/** 
 * Convert @p localVariable into search dataBase format
 *
 * @returns @p localVariable as a new xmlNode in search dataBase 
 *              format if successful,
 *         NULL otherwise
 *
 * @param localVariable Is valid
 * 
 */
#endif
#endif
    xmlNodePtr searchLocalNode(xmlNodePtr localVariable);


#ifdef USE_GNOME_DOCS

/**
 * searchSourceNode:
 * @style: Is valid
 * 
 * Convert @style into search dataBase format
 *
 * Returns @style as a new xmlNode in search dataBase format if successful,
 *         NULL otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**  
 * Convert @p style into search dataBase format
 *
 * @returns @p style as a new xmlNode in search dataBase format if successful,
 *         NULL otherwise
 *
 * @param style Is valid
 */
#endif
#endif
    xmlNodePtr searchSourceNode(xsltStylesheetPtr style);


#ifdef USE_GNOME_DOCS

/**
 * searchIncludeNode:
 * @include: Is a valid xsl:include instruction
 *   
 * Convert @include into search dataBase format
 *
 * Returns @include as a new xmlNode in search dataBase format if successful,
 *         NULL otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
  * Convert @p include into search dataBase format
  *
 * @returns @p include as a new xmlNode in search dataBase format
 *              if successful,
 *         NULL otherwise
 *
 * @param include Is a valid xsl:include instruction
 * 
 */
#endif
#endif
    xmlNodePtr searchIncludeNode(xmlNodePtr include);


#ifdef USE_GNOME_DOCS

/**
 * searchCallStackNode:
 * @callStackItem: Is valid
 * 
 * Convert @callStackItem into search dataBase format
 *
 * Returns @callStackItem as a new xmlNode in search dataBase format 
 *            if successful,
 *         NULL otherwise
 */
#else
#ifdef USE_KDE_DOCS

  /**
   *Convert @p include into search dataBase format
   *
   * @returns @p callStackItem as a new xmlNode in search dataBase
   *               format if successful,
   *            NULL otherwise  
   * @param callStackItem Is valid
   */
#endif
#endif
    xmlNodePtr searchCallStackNode(callPointPtr callStackItem);



#ifdef USE_GNOME_DOCS

/**
 * searchCommentNode:
 * @sourceNode: Is valid
 * 
 * Find documentation comment that applies to @sourceNode. If found convert comment 
 *         into search dataBase format required
 *
 * Returns Documentation comment for @node as a new xmlNode in search dataBase format 
 *            if successful,
 *         NULL otherwise
 */
#else
#ifdef USE_KDE_DOCS

  /**
   * Find documentation comment that applies to @p sourceNode. If found convert comment 
   *         into search dataBase format required
   *
   * Returns Documentation comment for @node as a new xmlNode in search dataBase format 
   *            if successful,
   *         NULL otherwise
   *
   * @param node Is valid
   */
#endif
#endif
    xmlNodePtr searchCommentNode(xmlNodePtr node);


#ifdef __cplusplus
}
#endif
#endif
