
/**************************************************************************
                          utils.c  -  declaration for misc utils this is
                                       mixed bag of functions so it is
                                       not realy a module hense no need
                                       for a utils prefix its functions 

                             -------------------
    begin                : Thur Jan 31 2002
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 **************************************************************************/

/**************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 **************************************************************************/

#ifndef XSLDBG_UTILS_H
#define XSLDBG_UTILS_H

#ifndef BUILD_DOCS
#include <stdio.h>
#include <string.h>
#include <libxml/tree.h>
#include <libxml/debugXML.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/xsltutils.h>
#include <libxml/xpath.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* 
  Make things simpler when working between char* and xmlChar*  .
   By definition a char is the same size as an xmlChar(unsigned char). 
*/

#ifndef BUILD_DOCS
#define xmlStrLen(text) strlen((char*)(text))
#define xmlStrCat(a, b) strcat((char*)(a), (char*)(b))
#define xmlStrCmp(a, b) strcmp((char*)(a), (char*)(b))
#define xmlStrnCmp(a, b, c) strncmp((char*)(a), (char*)(b), c)
#define xmlStrCpy(a, b) strcpy((char*)(a), (char*)(b))
#define xmlStrnCpy(a, b, c) strncpy((char*)(a),(char*)(b), c)
#define xmlStrChr(a, b) strchr((char*)(a), b)
#define xmlStrrChr(a, b) strrchr((char*)(a), b)
#endif

/* what char is use to separate directories in an URI*/
#define URISEPARATORCHAR '/'

    /* Handle the differences in path and quote character between
     * win32 and *nix systems */
#ifdef WIN32
#define  QUOTECHAR ' '
#define  PATHCHAR '\\'
#else
#define  QUOTECHAR '\"'
#define  PATHCHAR  '/'
#endif


/* JRF: Although RISC OS native paths use . as a separator, the arguments
        to xsldbg are passed in unix or URI form, and thus the above
        specification is correct. */



/**
 * IS_BLANK:
 * @c:  an UNICODE value (int)
 *
 * Macro to check the following production in the XML spec
 *
 * [3] S ::= (#x20 | #x9 | #xD | #xA)+
 */
#define IS_BLANK(c) (((c) == 0x20) || ((c) == 0x09) || ((c) == 0xA) ||	\
                     ((c) == 0x0D))


#ifdef USE_GNOME_DOCS

/**
 * trimString:
 * @text : A valid string with leading or trailing spaces
 *
 * Remove leading and trailing spaces off @text
 *         stores result back into @text
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Remove leading and trailing spaces off @p text
 *         stores result back into @p text
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param text A valid string with leading or trailing spaces
 */
#endif
#endif
    int trimString(xmlChar * text);


#ifdef USE_GNOME_DOCS

/**
 * splitString:
 * @textIn: The string to split
 * @maxStrings: The max number of strings to put into @out
 * @out: Is valid and at least the size of @maxStrings
 *
 * Split string by white space and put into @out
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Spit string by white space and put into @p out
 * 
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param textIn The string to split
 * @param maxStrings The max number of strings to put into @p out
 * @param out Is valid and at least the size of @p maxStrings
 */
#endif
#endif
    int splitString(xmlChar * textIn, int maxStrings, xmlChar ** out);



#ifdef USE_GNOME_DOCS

/**
 * lookupName:
 * @name : Is valid
 * @matchList : A NULL terminated list of names to use as lookup table
 *
 * Lookup and name in a list
 *
 * Returns The id of name found in @matchList,
 *         0 otherwise
*/
#else
#ifdef USE_KDE_DOCS

/** 
 * Lookup and name in a list
 *
 *
 * @returns The id of name found in @p matchList
 *         0 otherwise
 *
 * @param name Is valid
 * @param matchList A NULL terminated list of names to use as lookup table
 *
*/
#endif
#endif
    int lookupName(xmlChar * name, xmlChar ** matchList);

#ifdef USE_GNOME_DOCS

/**
 * fullQName:
 * @nameURI : QName part of name
 * @name : Local part of name 
 *
 * Join nameURI to name
 *
 * Returns a copy of "nameURI:name"
 *
 */

#else
#ifdef USE_KDE_DOCS

/**
 * Join nameURI to name
 *
 * @returns a copy of "nameURI:name"

 * fullQName:
 * @param nameURI : QName part of name
 * @param name : Local part of name 
 *
 *
 */
#endif
#endif

  xmlChar * fullQName(const xmlChar* nameURI, const xmlChar * name);

#ifdef __cplusplus
}
#endif
#endif
