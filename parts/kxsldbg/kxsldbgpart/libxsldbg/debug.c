
/***************************************************************************
                          debug.c  -  main functions for debugger use
                             -------------------
    begin                : Fri Nov 2 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ***************************************************************************/

#include "xsldbg.h"
#include "debug.h"
#include "debugXSL.h"           /* needed for debugXSLBreak function */
#include "breakpoint.h"
#include "callstack.h"
#include "files.h"
#include "options.h"

#include <libxslt/xsltutils.h>  /* need for breakpoint callback support */

/* setup debugger callbacks */
struct DebuggerCallbacks {
    xsltHandleDebuggerCallback debuggercallback;
    xsltAddCallCallback addcallback;
    xsltDropCallCallback dropcallback;
} debuggerDriver;


/* -----------------------------------------
              Private functions
 -------------------------------------------*/

/**
 * debugHandleDebugger:
 * @cur : source node being executed
 * @node : data node being processed
 * @templ : temlate that applies to node
 * @ctxt : the xslt transform context 
 * 
 * If either cur or node are a breakpoint, or xslDebugStatus in state 
 *   where debugging must occcur at this time, then transfer control
 *   to the debugXSLBreak function
 */
void debugHandleDebugger(xmlNodePtr cur, xmlNodePtr node,
                         xsltTemplatePtr templ,
                         xsltTransformContextPtr ctxt);

/* ------------------------------------- 
    End private functions
---------------------------------------*/


/*-----------------------------------------------------------
       Main debugger functions
-----------------------------------------------------------*/



/**
 * debugInit :
 *
 * Initialize debugger
 * Returns 1 on success,
 *         0 otherwise
 */
int
debugInit(void)
{
    int result;

    xslDebugStatus = DEBUG_NONE;
    result = breakPointInit();
    result = result && callStackInit();

    /* setup debugger callbacks */
    debuggerDriver.debuggercallback = debugHandleDebugger;
    debuggerDriver.addcallback = callStackAdd;
    debuggerDriver.dropcallback = callStackDrop;
    xsltSetDebuggerCallbacks(3, &debuggerDriver);
    return result;
}


/**
 * debugFree :
 *
 * Free up any memory taken by debugging
 */
void
debugFree(void)
{
    breakPointFree();
    callStackFree();
}


/** 
 * debugGotControl:
 * @reached: 1 if debugger has received control, -1 to read its value,
               0 to clear the flag
 *
 * Set flag that debugger has received control to value of @reached
 *
 * Returns 1 if any break point was reached previously,
 *         0 otherwise
 */
int
debugGotControl(int reached)
{
    static int hasReached;
    int result = hasReached;

    if (reached != -1)
        hasReached = reached;
    return result;
}


/**
 * debugHandleDebugger:
 * @cur : source node being executed
 * @node : data node being processed
 * @templ : temlate that applies to node
 * @ctxt : the xslt transform context 
 * 
 * If either cur or node are a breakpoint, or xslDebugStatus in state 
 *   where debugging must occcur at this time then transfer control
 *   to the debugXSLBreak function
 */
void
debugHandleDebugger(xmlNodePtr cur, xmlNodePtr node,
                    xsltTemplatePtr templ, xsltTransformContextPtr ctxt)
{

    if (!cur && !node) {
        xsltGenericError(xsltGenericErrorContext,
                         "XSLT source and XML aata are NULL can't enter debugger\n");
    } else {
	if (optionsGetIntOption(OPTIONS_GDB)){
	    int doValidation = 0;
	    switch(xsldbgValidateBreakpoints){
		case BREAKPOINTS_ARE_VALID:
		    if (!filesGetStylesheet() || !filesGetMainDoc()) {
			xsldbgValidateBreakpoints = BREAKPOINTS_NEED_VALIDATION;
			doValidation = 1;
		    }
			
		break;
	    
		case BREAKPOINTS_NEED_VALIDATION:
		    if (filesGetStylesheet() && filesGetMainDoc() && templ){ 
			xsldbgValidateBreakpoints = BREAKPOINTS_BEING_VALIDATED;
			doValidation = 1;
		    }
		break;
    
		case BREAKPOINTS_BEING_VALIDATED:
		    /*should never be in the state for any length of time */
		     xsltGenericError(xsltGenericErrorContext, 
			"Error: Unexpected BreakPoint validation state %d", xsldbgValidateBreakpoints);	    
		break;
	    }
	    if (doValidation){
		    /* breakpoints will either be marked as orphaned or not as needed */
		    xsldbgValidateBreakpoints = BREAKPOINTS_BEING_VALIDATED;
		    walkBreakPoints((xmlHashScanner)
			    xslDbgShellValidateBreakPoint, ctxt);
		    if (filesGetStylesheet() && filesGetMainDoc() && templ){
			xsldbgValidateBreakpoints = BREAKPOINTS_ARE_VALID;
		    }else{
			xsldbgValidateBreakpoints = BREAKPOINTS_NEED_VALIDATION;
		    }
	    }
	}
        switch (xslDebugStatus) {

                /* A temparary stopping point */
            case DEBUG_WALK:
            case DEBUG_TRACE:
                /* only allow breakpoints at xml elements */
                if (xmlGetLineNo(cur) != -1)
                    debugXSLBreak(cur, node, templ, ctxt);
                break;

            case DEBUG_STOP:
                xslDebugStatus = DEBUG_CONT;
                /* only allow breakpoints at xml elements */
                if (xmlGetLineNo(cur) != -1)
                    debugXSLBreak(cur, node, templ, ctxt);
                break;

            case DEBUG_STEP:
                /* only allow breakpoints at xml elements */
                if (xmlGetLineNo(cur) != -1)
                    debugXSLBreak(cur, node, templ, ctxt);
                break;

            case DEBUG_CONT:
                {
                    breakPointPtr breakPtr = NULL;
                    xmlChar *baseUri = NULL;

                    if (cur) {
                        breakPtr =
                            breakPointGet(cur->doc->URL,
                                          xmlGetLineNo(cur));

                        if (breakPtr && (breakPtr->flags & BREAKPOINT_ENABLED) ){
			    debugXSLBreak(cur, node, templ, ctxt);
			    return;
			}
                    }
		    if (node) {
			baseUri = filesGetBaseUri(node);
                        if (baseUri != NULL) {
                            breakPtr =
                                breakPointGet(baseUri, xmlGetLineNo(node));
                        } else {
                            breakPtr =
                                breakPointGet(node->doc->URL,
                                              xmlGetLineNo(node));
                        }
                        if (breakPtr) {
                            if (breakPtr->flags & BREAKPOINT_ENABLED) {
                                debugXSLBreak(cur, node, templ, ctxt);
			    }
			}
                        if (baseUri)
                            xmlFree(baseUri);
                    }
                }
                break;
        }
    }
}
