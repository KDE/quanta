
/***************************************************************************
                          variable_cmds.c  -  description
                             -------------------
    begin                : Sun Dec 30 2001
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

#include <libxml/xpath.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/variables.h>  /* needed for xsltVariablesComp */
#include <libxml/valid.h>       /* needed for xmlSplitQName2 */
#include "xsldbg.h"
#include "debugXSL.h"
#include "search.h"


int
xslDbgShellSetVariable(xsltTransformContextPtr styleCtxt, xmlChar * arg)
{
    int result = 0, showUsage = 0;
    xmlChar *name, *nameURI, *selectExpr, *opts[3];

    if (!styleCtxt) {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Unable to set, No stylesheet properly loaded\n");
        return result;
    }

    if (!arg) {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: NULL argument provided\n");
        return result;
    }

    if (xmlStrLen(arg) > 1) {
        if (splitString(arg, 2, opts) == 2) {
            nameURI = NULL;
	    /* ignore any "$" prefix as user probably didn't mean that 
	       "$" is part of variable name*/
	    if (*opts[0] =='$'){
	      opts[0] = opts[0] + 1;
	    }
            name = xmlSplitQName2(opts[0], &nameURI);
            if (name == NULL)
                name = xmlStrdup(opts[0]);
            selectExpr = xmlStrdup(opts[1]);
            if (name && selectExpr) {
                xsltStackElemPtr def = NULL;

                if (styleCtxt->varsBase) {
                    /* try finding varaible in stack */
                    xsltStackElemPtr item =
                        styleCtxt->varsTab[styleCtxt->varsBase];
                    while (item) {
                        if ((xmlStrCmp(name, item->name) == 0) &&
                            (item->nameURI == NULL
                             || (xmlStrCmp(name, item->nameURI) == 0))) {
                            def = item;
                            break;
                        }
                        item = item->next;
                    }
                }

                if (def == NULL)
                    def = (xsltStackElemPtr)
                        xmlHashLookup2(styleCtxt->globalVars,
                                       name, nameURI);
                if (def != NULL) {
                    if (def->select) {
                        /* we've found the variable so change it */
                        xmlFree(def->select);
                        def->select = selectExpr;
                        if (def->comp->comp)
                            xmlXPathFreeCompExpr(def->comp->comp);
                        def->comp->comp = xmlXPathCompile(def->select);
                        if (def->value)
                            xmlXPathFreeObject(def->value);
                        def->value =
                            xmlXPathEval(def->select,
                                         styleCtxt->xpathCtxt);
                        result = 1;
                    } else {
                        xmlFree(selectExpr);
                        xsltGenericError(xsltGenericErrorContext,
                                         "Error: Can't change a variable "
                                         "that doesn't use the select attribute\n");
                    }
                } else
                    xsltGenericError(xsltGenericErrorContext,
                                     "Error: Variable %s not found\n",
                                     name);
                xmlFree(name);
            } else
                xsltGenericError(xsltGenericErrorContext,
                                 "Error: Internal error set failed\n");
        } else {
            showUsage = 1;
        }

        if (showUsage == 1)
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Invalid format, expected\n set <VARIABLE_NAME> <XPATH>\n");
    }
    return result;
}
