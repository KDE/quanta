
/***************************************************************************
                          xsldbgio.h  - declare user input functions
                             -------------------
    begin                : Sun Dec 23 2001
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

#ifndef XSLDBGIO_H
#define XSLDBGIO_H

#ifndef BUILD_DOCS
#include <libxml/tree.h>        /* needed for definition of xmlChar */
#endif

/**
 * xslShellReadline:
 * @prompt:  the prompt value
 *
 * Read a string
 *
 * Returns a copy of the text inputed or NULL if EOF in stdin found. 
 *    The caller is expected to free the returned string.
 */
xmlChar *xslDbgShellReadline(xmlChar * prompt);


#define DEBUG_BUFFER_SIZE 500   /*used by xslDbgShell */


#endif
