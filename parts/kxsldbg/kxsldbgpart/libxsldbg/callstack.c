
/***************************************************************************
                          callstack.c  -  call stack implementation 
                             -------------------
    begin                : Fri Nov 2 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ***************************************************************************/

#include "xsldbg.h"
#include "utils.h"
#include "breakpoint.h"
#include "arraylist.h"
#include "callstack.h"
#include "xsldbgmsg.h"


/*------------------------------------------------------
                  Private functions
 -----------------------------------------------------*/

/**
 * addCallInfo:
 * @templateName: Template name to add 
 * @templateURI: QName part of template name to add
 * @modeName: Mode of template
 * @modeURI: QName part of node of template
 * @url: The url for the template
 *
 * Add template "call" to call stack
 *
 * Returns A reference to the added info if successful, 
 *         NULL otherwise
 */
callPointInfoPtr
addCallInfo(const xmlChar * templateName, const xmlChar *templateURI,
	    const xmlChar * modeName, const xmlChar* modeURI, const xmlChar * url);


/*------------------------------------------------------
                  Xsl call stack related 
-----------------------------------------------------*/

/* keep track of the top and bottom of call stack*/

/* This is the major structure and contains a stack of call points */
callPointPtr callStackBot, callStackTop;

/* save memory by keep only one copy of data used for several 
 items on call stack */
callPointInfoPtr callInfo;

/* What frame depth are we to stop at */
int stopDepth = -1;


/**
 * callStackInit:
 *
 * Returns If callStack has been initialized properly and all
 *               memory required has been obtained,
 *         0 otherwise
 *
 * Returns 1 if callStack has been initialized properly and all
 *               memory required has been obtained,
 *         0 otherwise
*/
int
callStackInit(void)
{

    callInfo = (callPointInfoPtr) xmlMalloc(sizeof(callPointInfo));
    if (callInfo) {
        callInfo->next = NULL;
        callInfo->templateName = NULL;
	callInfo->templateURI = NULL;
	callInfo->modeName = NULL;
	callInfo->modeURI = NULL;	 
        callInfo->url = NULL;
    }
    callStackBot = (callPointPtr) xmlMalloc(sizeof(callPoint));
    if (callStackBot) {
        callStackBot->next = NULL;
        callStackBot->info = NULL;
        callStackBot->lineNo = -1;
        callStackTop = callStackBot;
    }
    return (callInfo != NULL) && (callStackBot != NULL);
}



/**
 * callStackFree:
 *
 *
 * Free all memory used by callStack
 */
void
callStackFree(void)
{

    callPointInfoPtr curInfo = callInfo, nextInfo;
    callPointPtr curCall = callStackBot, nextCall;

    /* remove all call info's */
    while (curInfo) {
        nextInfo = curInfo->next;
        if (curInfo->templateName)
            xmlFree(curInfo->templateName);
        if (curInfo->templateURI)
            xmlFree(curInfo->templateURI);	
        if (curInfo->modeName)
            xmlFree(curInfo->modeName);
        if (curInfo->modeURI)
            xmlFree(curInfo->modeURI);
        if (curInfo->url)
            xmlFree(curInfo->url);
	xmlFree(curInfo);
        curInfo = nextInfo;
    }
    curInfo = NULL;

    /* remove all call stack items left. There should be none !! */
    while (curCall) {
        nextCall = curCall->next;
        xmlFree(curCall);
        curCall = nextCall;
    }

    callStackBot = NULL;
    callStackTop = NULL;
    callInfo = NULL;
}


/**
 * addCallInfo:
 * @templateName: Template name to add 
 * @templateURI: QName part of template name to add
 * @modeName: Mode of template
 * @modeURI: QName part of node of template
 * @url: The url for the template
 *
 * Add template "call" to call stack
 *
 * Returns A reference to the added info if successful, 
 *         NULL otherwise
 */
callPointInfoPtr
addCallInfo(const xmlChar * templateName, const xmlChar *templateURI,
	    const xmlChar * modeName, const xmlChar* modeURI, const xmlChar * url)
{
    callPointInfoPtr result = NULL, cur = callInfo;
    int found;

    if (!templateName || !url) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Null template name or url:"
                         " addCallInfo\n");
        if (templateName)
            xsltGenericError(xsltGenericErrorContext, "template :'%s'\n",
                             templateName);
        if (url)
            xsltGenericError(xsltGenericErrorContext, "url :'%s'\n", url);

#endif
        return result;
    }

    while (cur->next) {
        found = 1;
        if (templateName && cur->templateName 
	    && !xmlStrEqual(cur->templateName, templateName)) 
	    found = 0;
	if (found && !xmlStrEqual(cur->templateURI, templateURI)) 
	    found = 0;
	if (found && !xmlStrEqual(cur->modeName, modeName))
	    found = 0;
	if (found && !xmlStrEqual(cur->modeURI, modeURI))
	    found = 0;
	if (found && !xmlStrEqual(cur->url, url))
	   found = 0;

	if (found){
            result = cur;
            break;
        }
        cur = cur->next;
      }

    if (!result && cur) {
        result = (callPointInfoPtr) xmlMalloc(sizeof(callPointInfo));
        if (result) {
	  if ((cur == callInfo) && !cur->templateName &&  !cur->templateURI 
				 && !cur->modeName && !cur->modeURI 
				 && !cur->url){
	    xmlFree(callInfo);
	    callInfo = result;	    
	  } else{
            cur->next = result;
	  }
	  result->templateName =
	    (xmlChar *) xmlMemStrdup((char *) templateName);
	  result->templateURI =
	    (xmlChar *) xmlMemStrdup((char *) templateURI);
	  result->modeName =
                (xmlChar *) xmlMemStrdup((char *) modeName);
	  result->modeURI =
	    (xmlChar *) xmlMemStrdup((char *) modeURI);
	  result->url = (xmlChar *) xmlMemStrdup((char *) url);
	  result->next = NULL;
	}else {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Unable to create callPointInfo from :"
                             " addCallInfo\n");
#endif
	}
    }
    if (!cur)
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Unable to create callPointInfo from :"
                             " addCallInfo\n");
#endif
    return result;
}


/**
 * callStackAdd:
 * @templ: The current template being applied
 * @source: The source node being processed
 *
 * Add template "call" to call stack
 *
 * Returns 1 on success, 
 *         0 otherwise
 */
int
callStackAdd(xsltTemplatePtr templ, xmlNodePtr source)
{
    int result = 0;
    const char *name = "Default template";
    callPointInfoPtr info;

    if (!templ || !source)
        return result;

    if (!source->doc || !source->doc->URL) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Invalid document url in call from : callStackAdd\n");
#endif
        return result;
    }

    /* are at a "frame" break point ie "step down" */
    if ((xslDebugStatus == DEBUG_STEPDOWN)
        && (stopDepth == callStackGetDepth())) {
        xslDebugStatus = DEBUG_STOP;
        stopDepth = 0;
    }

    /* this need not be an error just we've got a text in source */
    if (xmlGetLineNo(source) == -1) {
        return result;
    }

    if (templ) {
        if (templ->name)
            name = (char *) templ->name;
        else {
            if (templ->match)
                name = (char *) templ->match;
        }
    }

    if (!name) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Invalid template name : callStackAdd\n");
#endif
        return result;
    }

    info = addCallInfo((xmlChar *) name, (xmlChar *) templ->nameURI,
		       (xmlChar *) templ->mode, (xmlChar *) templ->modeURI,
		       source->doc->URL);

    if (info) {
        callPointPtr cur;

        cur = (callPointPtr) xmlMalloc(sizeof(callPoint));
        if (cur) {
            callStackTop->next = cur;
            callStackTop = cur;
            cur->info = info;
            cur->lineNo = xmlGetLineNo(source);
            cur->next = NULL;
            result = 1;
        } else {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Unable to create call point : callStackAdd\n");
#endif
        }
    } else {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Unable to create call info : callStackAdd\n");
#endif
    }

    return result;
}


/**
 * callStackDrop:
 *
 *
 * Drop the topmost item off the call stack
 */
void
callStackDrop(void)
{

    if (!callStackBot) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: callStackDrop failed invalid call stack:"
                         " dbgcallstack.c");
#endif
        return;
    }

    /* are we at a "frame" break point ie "step up". if we've gone too
     * far stop imediately */
    if ((xslDebugStatus == DEBUG_STEPUP)
        && (-1 * callStackGetDepth()) >= stopDepth) {
        xslDebugStatus = DEBUG_STOP;
        stopDepth = 0;
    }

    if (callStackBot->next) {
        callPointPtr cur = callStackBot;

        while (cur->next && cur->next->next) {
            cur = cur->next;
        }
        if (cur->next)
            xmlFree(cur->next);
        cur->next = NULL;
        callStackTop = cur;
    } else {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: callStackDrop failed "
                         "no items on call stack : dbgcallstack.c");
#endif
    }
}


/** 
 * callStackStepup:
 * @depth:The frame depth to step up to  
 *             0 < @depth <= callStackGetDepth()
 *
 * Set the frame depth to step up to
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
callStackStepup(int depth)
{
    int result = 0;

    if ((depth > 0) && (depth <= callStackGetDepth())) {
        stopDepth = -1 * depth;
        xslDebugStatus = DEBUG_STEPUP;
        result = 1;
    } else {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: callStackStepup failed invalid depth %d: "
                         " callstack.c", depth);
#endif
    }
    return result;
}


/** 
 * callStackStepdown:
 * @depth: The frame depth to step down to, 
 *             0 < @depth <= callStackGetDepth()
 *
 * Set the frame depth to step down to
 *
 * Returns 1 on success, 
 *         0 otherwise
 */
int
callStackStepdown(int depth)
{
    int result = 0;

    if ((depth > 0) && (depth >= callStackGetDepth())) {
        stopDepth = depth;
        xslDebugStatus = DEBUG_STEPDOWN;
        result = 1;
    } else {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: callStackStepdown failed invalid depth %d: "
                         " dbgcallstack.c", depth);
#endif
    }
    return result;
}


/**
 * callStackGet:
 * @depth: 0 < @depth <= callStackGetDepth()  
 *
 * Retrieve the call point at specified call depth 

 * Returns Non-null if depth is valid,
 *         NULL otherwise 
 */
callPointPtr
callStackGet(int depth)
{
    callPointPtr result = NULL, cur = callStackBot;

    if (!callStackBot) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: callStackGet failed invalid call stack:"
                         " callstack.c");
#endif
        return result;
    }
    if ((depth < 1) && (depth > callStackGetDepth())) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: callStackGet failed invalid call depth:"
                         " callstack.c");
#endif
        return result;
    }

    while (depth > 0 && cur->next) {
        cur = cur->next;
        depth--;
    }

    if (depth == 0)
        result = cur;
    else {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: callStackGet failed invalid call depth:"
                         " callstack.c");
#endif
    }
    return result;
}


/** 
 * callStackGetTop:
 *
 * Get the top item in the call stack
 *
 * Returns The top of the call stack
 */
callPointPtr
callStackGetTop(void)
{
    return callStackTop;
}


/** 
 * callStackGetDepth:
 * 
 * Return the depth of call stack
 *
 * Returns The depth of call stack
 */
int
callStackGetDepth(void)
{
    callPointPtr cur = callStackBot;
    int depthCount = 0;

    if (!callStackBot) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: calldepth failed invalid call stack:"
                         " dbgcallstack.c");
#endif
        return depthCount;
    }


    while (cur->next) {
        depthCount++;
        cur = cur->next;
    }
    return depthCount;
}
