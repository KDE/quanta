
/**************************************************************************
                          debug.h  -  declare major debugger functions
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

#ifndef XSL_DEBUG_H
#define XSL_DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif


#ifdef USE_GNOME_DOCS

/**
 * debugInit:
 *
 * Initialize debugger allocating any memory needed by debugger
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Initialize debugger allocating any memory needed by debugger
 *
 * @returns 1 on success,
 *          0 otherwise
 */
#endif
#endif
    int debugInit(void);



#ifdef USE_GNOME_DOCS

/**
 * debugFree:
 *
 * Free up any memory taken by debugger
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Free up any memory taken by debugger
 */
#endif
#endif
    void debugFree(void);


#ifdef USE_GNOME_DOCS

/** 
 * debugGotControl:
 * @reached: 1 if debugger has received control, -1 to read its value,
               0 to clear the flag
 *
 * Set flag that debuger has received control to value of @reached
 *
 * Returns 1 if any break point was reached previously,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/** 
 * Set flag that debuger has received control to value of @p reached
 *
 * @returns 1 if any breakpoint was reached previously,
 *         0 otherwise
 *
 * @param reached 1 if debugger has received control, -1 to read its value,
 *              0 to clear the flag
 */
#endif
#endif
    int debugGotControl(int reached);

#ifdef __cplusplus
}
#endif
#endif
