
/***************************************************************************
                          breakpoint.c  -  breakpoint implementation
                             -------------------
    begin                : Fri Nov 2 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ***************************************************************************/

#include "xsldbg.h"
#include "breakpoint.h"
#include "arraylist.h"
#include "options.h"

extern int xsldbgValidateBreakpoints; /*located in debugXSL.c*/

/*-----------------------------------------------------------
       Private functions
-----------------------------------------------------------*/

/**
 * lineNoItemNew:
 * 
 * Returns a new hash table for break points
 */
xmlHashTablePtr lineNoItemNew(void);


/**
 * lineNoItemFree:
 * @item: valid hashtable of break points
 * 
 * Free @item and all its contents
 */
void lineNoItemFree(void *item);


/**
 * lineNoItemDelete:
 * @breakPointHash: Is valid
 * @breakPtr: Is valid
 * 
 * Returns 1 if able to delete @breakPtr from @breakPointHash,
 *         0 otherwise
 */
int lineNoItemDelete(xmlHashTablePtr breakPointHash,
                     breakPointPtr breakPtr);

/**
 * lineNoItemAdd:
 * @breakPointHash: is valid
 * @breakPtr: is valid
 *
 * Add breakpoint to hash 
 *
 * Returns 1 if able to add @breakPtr to @breakPointHash,
 *         0 otherwise
 */
int lineNoItemAdd(xmlHashTablePtr breakPointHash, breakPointPtr breakPtr);

/*-----------------------------------------------------------
       Breakpoint debugger functions
-----------------------------------------------------------*/


/* This is our major structure, it is a list of hash tables. Each 
 hash table has breakpoints with the same line number. A line
 number is used as an index into this list to get the right hash table.
 Then its just a matter of a simple hash table lookup  */
arrayListPtr breakList;

/* keep track of what break point id we're up to*/
int breakPointCounter = 0;

/* What is the current breakpoint is only valid up to the start of 
 xsldbg command prompt. ie don't use it after deletion of breakpoints */
breakPointPtr activeBreakPointItem = NULL;


/**
 * lineNoItemNew:
 * 
 * Returns a new hash table for break points
 */
xmlHashTablePtr
lineNoItemNew(void)
{
    xmlHashTablePtr hash;

    hash = xmlHashCreate(4);

    return hash;
}


/**
 * lineNoItemFree:
 * @item: valid hashtable of break points
 * 
 * Free @item and all its contents
 */
void
lineNoItemFree(void *item)
{
    xmlHashTablePtr hash = (xmlHashTablePtr) item;

    if (item) {
#if 0
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Freeing breakpoint line hash"
                         " with %d elements \n", xmlHashSize(item));
#endif
#endif
        xmlHashFree(hash, breakPointItemFree);
    }
}


/**
 * lineNoItemDelete:
 * @breakPointHash: is valid
 * @breakPtr: is valid
 * 
 * Returns 1 if able to delete @breakPtr from @breakPointHash,
 *         0 otherwise
 */
int
lineNoItemDelete(xmlHashTablePtr breakPointHash, breakPointPtr breakPtr)
{
    int result = 0;

    if (breakPointHash && breakPtr) {
        if (xmlHashRemoveEntry(breakPointHash, breakPtr->url,
                               breakPointItemFree) == 0)
            result = 1;
	else
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
    xsltGenericError(xsltGenericErrorContext,"lineNoItemDelete failed");
#endif

    }else {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
    xsltGenericError(xsltGenericErrorContext, "lineNoItemDelete failed args %d %d", breakPointHash, breakPtr);
#endif
    }
    return result;
}


/**
 * lineNoItemAdd:
 * @breakPointHash: is valid
 * @breakPtr: is valid
 *
 * Returns 1 if able to add @breakPtr to @breakPointHash,
 *         0 otherwise
 */
int
lineNoItemAdd(xmlHashTablePtr breakPointHash, breakPointPtr breakPtr)
{
    int result = 0;

    if (breakPointHash && breakPtr) {
        if (xmlHashAddEntry(breakPointHash, breakPtr->url, breakPtr) == 0)
            result = 1;
    }
    return result;
}

/**
 * breakPointGetLineNoHash:
 * @lineNo: Line number of of breakpoints of interest
 *
 * Return A hash of breakpoints with same line number
 *
 * Returns A hash of breakpoints with a line number of @lineNo
 */
xmlHashTablePtr
breakPointGetLineNoHash(long lineNo)
{
    if (!breakList) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "BreakPoints not initialized\n");
#endif
        return NULL;
    } else
        return (xmlHashTablePtr) arrayListGet(breakList, lineNo);
}


/**
 * breakPointInit:
 *
 * Returns 1 if breakpoints have been initialized properly and all
 *               memory required has been obtained,
 *         0 otherwise
*/
int
breakPointInit(void)
{
    int result = 0;

    /* the average file has 395 lines of code so add 100 lines now */
    breakList = arrayListNew(100, lineNoItemFree);
    if (breakList) {
        /*
         * We don't need to do any thing else, as its done when we add the 
         *    breakPoints
         */
        result = 1;
    } else {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Unable to intialize breakPoints: memory error\n");
#endif
    }
    return result;
}


/**
 * breakPointFree:
 *
 * Free all memory used by breakPoints 
 */
void
breakPointFree(void)
{
    if (breakList)
        arrayListFree(breakList);
    breakList = NULL;
}


/**
 * breakPointEmpty:
 *
 * Empty the break point collection
 *
 * Returns 1 if able to empty the breakpoint list of its contents,
 *         0  otherwise
 */
int
breakPointEmpty(void)
{
    return arrayListEmpty(breakList);
}


/** 
 * breakPointItemNew:
 * 
 * Create a new break point item
 * Returns valid break point with default values set if successful, 
 *         NULL otherwise
 */
breakPointPtr
breakPointItemNew(void)
{
    breakPointPtr breakPtr = (breakPointPtr) xmlMalloc(sizeof(breakPoint));

    if (breakPtr) {
        breakPtr->url = NULL;
        breakPtr->lineNo = -1;
        breakPtr->templateName = NULL;
	breakPtr->modeName = NULL;
        breakPtr->flags = BREAKPOINT_ENABLED;
        breakPtr->id = ++breakPointCounter;
        breakPtr->type = DEBUG_BREAK_SOURCE;
    }
    return breakPtr;
}


/**
 * breakPointItemFree:
 * @payload: valid breakPointPtr 
 * @name: not used
 *
 * Free memory associated with this break point
 */
void
breakPointItemFree(void *payload, xmlChar * name ATTRIBUTE_UNUSED)
{
    if (payload) {
        breakPointPtr breakPtr = (breakPointPtr) payload;

        if (breakPtr->url)
            xmlFree(breakPtr->url);
        if (breakPtr->templateName)
            xmlFree(breakPtr->templateName);
	if (breakPtr->modeName)
	  xmlFree(breakPtr->modeName);
        xmlFree(breakPtr);
    }
}


/**
 * breakPointActiveBreakPoint:
 * 
 * Get the active break point
 *
 * Returns The last break point that we stoped at
 *
 * Depreciated
 */
breakPointPtr
breakPointActiveBreakPoint(void)
{
    /* This function is depreciated */
    return NULL;                /* activeBreakPointItem; */
}



/**
 * breakPointSetActiveBreakPoint:
 * @breakPtr: Is valid break point or NULL
 *
 * Set the active break point
 *
 * Depreciated
 */
void
breakPointSetActiveBreakPoint(breakPointPtr breakPtr)
{
    /*
     * activeBreakPointItem = breakPtr;
     */

}


/**
 * breakPointAdd:
 * @url: Non-null, non-empty file name that has been loaded by
 *                    debugger
 * @lineNumber: @lineNumber >= 0 and is available in url specified and
 *                points to an xml element
 * @templateName: The template name of breakPoint or NULL
 * @modeName : The mode of breakpoint or NULL
 * @type: Valid BreakPointTypeEnum
 *
 * Add break point at file and line number specified
 *
 * Returns 1 if successful,
 *	   0 otherwise
*/
int
breakPointAdd(const xmlChar * url, long lineNumber,
              const xmlChar * templateName, 
	      const xmlChar * modeName,
	      BreakPointTypeEnum type)
{
    int result = 0, breakPointType = type;
    xmlHashTablePtr breakPointHash = NULL;     /* hash of breakPoints */
    breakPointPtr breakPtr;

    if (!breakList) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: breakPoints not initialized\n");
#endif
        return result;
    }

    if (!url || (lineNumber == -1)) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Invalid url or line number to breakPointAdd\n");
#endif
        return result;
    }

    /* if breakpoint already exists then don;t add it */
    if (breakPointIsPresent(url, lineNumber)) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Warning: Breakpoint at file %s: line %d exists\n",
                         url, lineNumber);
#endif
        return result;
    }

    breakPtr = breakPointItemNew();
    if (breakPtr) {
        breakPtr->url = (xmlChar *) xmlMemStrdup((char *) url);
        breakPtr->lineNo = lineNumber;
        if (templateName)
            breakPtr->templateName =
                xmlStrdup( templateName);
        else
            breakPtr->templateName = NULL;
	if (modeName)
	    breakPtr->modeName = 
	      xmlStrdup(modeName);
	else
	  breakPtr->modeName = NULL;
        breakPtr->type = breakPointType;

        /* add new breakPoint to the right hash table */
        breakPointHash = breakPointGetLineNoHash(lineNumber);
        if (breakPointHash) {
            result = lineNoItemAdd(breakPointHash, breakPtr);
        } else {
            /* Grow breakList size */
            int lineIndex;
            int newEntries = breakList->count;
            xmlHashTablePtr hash;

            result = 1;
            if ((lineNumber < breakList->count) && breakList->count) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
                xsltGenericError(xsltGenericErrorContext,
                                 "Error: Unable to find breakpoint line hash at %d\n",
                                 lineNumber);
#endif
            } else {
                if (breakList->count + newEntries < lineNumber)
                    newEntries = lineNumber - breakList->count + 1;


#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
                /*
                 * xsltGenericError(xsltGenericErrorContext,
                 * "Size of line list was %d adding %d entries\n",
                 * breakList->count, newEntries);
                 */
#endif
                lineIndex = 0;
                while ((lineIndex < newEntries) && result) {
                    hash = lineNoItemNew();
                    if (hash) {
                        result = result && arrayListAdd(breakList, hash);
                    } else {
                        result = 0;
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
                        xsltGenericError(xsltGenericErrorContext,
                                         "Error: Unable to create hash table breakPoint list: memory error\n");
#endif
                        return result;
                    }
                    lineIndex++;
                }
                /* find the newly added hashtable of breakpoints */
                breakPointHash = breakPointGetLineNoHash(lineNumber);
                if (breakPointHash) {
                    result = lineNoItemAdd(breakPointHash, breakPtr);
                } else {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
                    xsltGenericError(xsltGenericErrorContext,
                                     "Error: Unable to create new breakPoint:interal error\n");
#endif
                    return result;
                }
            }

        }
    } else {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Unable to create new breakPoint: memory error\n");
#endif
    }

    if (result && (optionsGetIntOption(OPTIONS_GDB) > 1) && 
	    (xsldbgValidateBreakpoints != BREAKPOINTS_BEING_VALIDATED)){
      breakPointPrint(NULL, breakPtr);
      xsltGenericError(xsltGenericErrorContext,"\n");
    }
    return result;
}


/**
 * breakPointDelete:
 * @breakPtr: Is valid
 *
 * Delete the break point specified if it can be found using 
 *    @breakPoint's url and lineNo
 *
 * Returns 1 if successful,
 *	   0 otherwise
*/
int
breakPointDelete(breakPointPtr breakPtr)
{
    int result = 0;
    xmlHashTablePtr breakPointHash;     /* hash of breakPoints */

    if (!breakPtr)
        return result;

    breakPointHash = breakPointGetLineNoHash(breakPtr->lineNo);
    if (breakPointHash) {
        result = lineNoItemDelete(breakPointHash, breakPtr);
    } else {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Break point not found: xslDeleteBreakPoint\n");
#endif
    }
    return result;
}


/**
 * breakPointEnable:
 * @breakPtr: A valid breakpoint
 * @enable: Enable break point if 1, disable if 0, toggle if -1
 *
 * Enable or disable a break point
 *
 * Returns 1 if successful,
 *	   0 otherwise
*/
int
breakPointEnable(breakPointPtr breakPtr, int enable)
{
    int result = 0;

    if (breakPtr) {
	int enableFlag = 1;
        if (enable != XSL_TOGGLE_BREAKPOINT){
	    enableFlag = enable;
        }else {
	    if (breakPtr->flags & BREAKPOINT_ENABLED)
		enableFlag = 0;
        }
	if (enableFlag)
	    breakPtr->flags |= BREAKPOINT_ENABLED;
	else
	    breakPtr->flags = breakPtr->flags & (BREAKPOINT_ALLFLAGS ^ BREAKPOINT_ENABLED);
        result = 1;
    }
    return result;
}


/**
 * breakPointLinesCount:
 *
 * Return the number of hash tables of break points with the same line number
 *
 * Returns The number of hash tables of break points with the same line number
 */
int
breakPointLinesCount(void)
{
    if (!breakList) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: BreakPoints not initialized\n");
#endif
        return 0;
    } else
        return arrayListCount(breakList);
}


/**
 * breakPointLinesList:
 *
 * Returns The list of hash tables for break points
 *        Dangerous function to use!! 
 */
arrayListPtr
breakPointLineList(void)
{
    return breakList;
}


/**
 * breakPointGet:
 * @url: Non-null, non-empty file name that has been loaded by
 *                    debugger
 * @lineNumber: lineNumber >= 0 and is available in @url
 *
 * Get a break point for the breakpoint collection
 *
 * Returns break point if break point exists at location specified,
 *	   NULL otherwise
*/
breakPointPtr
breakPointGet(const xmlChar * url, long lineNumber)
{
    xmlHashTablePtr breakHash = breakPointGetLineNoHash(lineNumber);
    breakPointPtr breakPtr = NULL;

    if (!breakHash || !url)
        return breakPtr;

    breakPtr = xmlHashLookup(breakHash, url);
    return breakPtr;
}


/**
 * breakPointPrint:
 * @file: Is valid, or NULL to use libxslt error function
 * @breakPtr: A valid break point
 *
 * Print the details of @breakPtr to @file
 *
 * Returns 1 if successful,
 *	   0 otherwise
 */
int
breakPointPrint(FILE * file, breakPointPtr breakPtr)
{
    int result = 0;

    if (!breakPtr)
        return result;

    if (file) {
        /* support old meaning of file parameter */
        fprintf(file, "Breakpoint %d ", breakPtr->id);
	if (breakPtr->flags & BREAKPOINT_ENABLED) 
            fprintf(file, "enabled ");
        else
            fprintf(file, "disabled ");

        if (breakPtr->templateName) {
	    if (breakPtr->modeName)
	      fprintf(file, "for template :\"%s\" mode :\"%s\" ", 
		      breakPtr->templateName, breakPtr->modeName);
	    else
	      fprintf(file, "for template :\"%s\" mode :\"\" ", 
		      breakPtr->templateName);
        }

        if (breakPtr->url) {
            fprintf(file, "in file %s : line %ld",
                    breakPtr->url, breakPtr->lineNo);
        } else {
            fprintf(file, "in file <n/a>, line %ld", breakPtr->lineNo);
        }
    } else {
        xsltGenericError(xsltGenericErrorContext,
                        "Breakpoint %d ", breakPtr->id);
	if (breakPtr->flags & BREAKPOINT_ENABLED) 
            xsltGenericError(xsltGenericErrorContext, "enabled ");
        else
            xsltGenericError(xsltGenericErrorContext, "disabled ");

        if (breakPtr->templateName) {
	    if (breakPtr->modeName)
	         xsltGenericError(xsltGenericErrorContext,
		    "for template :\"%s\" mode :\"%s\" ", 
		     breakPtr->templateName, breakPtr->modeName);
	    else
	         xsltGenericError(xsltGenericErrorContext,
		      "for template :\"%s\" mode :\"\" ", 
		      breakPtr->templateName);
        }

        if (breakPtr->url) {
            xsltGenericError(xsltGenericErrorContext,
                             "in file %s : line %ld",
                             breakPtr->url, breakPtr->lineNo);
        } else {
            xsltGenericError(xsltGenericErrorContext,
                             "in file <n/a>, line %ld", breakPtr->lineNo);
        }
    }
    return ++result;
}


/**
 * breakPointIsPresent:
 * @url: Non-null, non-empty file name that has been loaded by
 *                    debugger
 * @lineNumber: @lineNumber >= 0 and is available in @url
 *
 * Determine if there is a break point at file and line number specified
 *
 * Returns 1  if successful,  
 *         0 otherwise
*/
int
breakPointIsPresent(const xmlChar * url, long lineNumber)
{
    int result = 0;

    if (!url || (lineNumber == -1))
        return result;

    result = (breakPointGet(url, lineNumber) != NULL);

    return result;
}


/**
 * breakPointIsPresentNode:
 * @node: node != NULL
 *
 * Determine if a node is a break point
 *
 * Returns 1 on success, 
 *         0 otherwise
 */
int
breakPointIsPresentNode(xmlNodePtr node)
{
    int result = 0;

    if (!node || !node->doc)
        return result;

    if (xmlGetLineNo(node) == -1)
        return result;

    if (node->doc->URL) {
        result = breakPointIsPresent(node->doc->URL, xmlGetLineNo(node));
    }

    return result;
}
